//
//  sparseMatrix.h
//  sparse
//
//  Created by tzry on 15/2/7.
//  Copyright (c) 2015年 tzry. All rights reserved.
//

#ifndef sparseMatrix_h
#define sparseMatrix_h
#include <vector>
#include <iostream>
#include <fstream>
#include "tmpSparse.h"
#include<thread>
#include"sparseVector.h"

#define THREADCOUNT 1//线程数




class sparseMatrix
{
private:
    int n_col;		/**< number of the columns. */
    int n_row;		/**< number of the rows. */
    int n_couple;
    
    
    std::mutex m;//互斥量
    
public:
    
    int missionRow=0;
    
    std::vector<double> ele; /**< the array to story the values of the elements. */
    std::vector<int> col;	/**< the column  index of the non-zero elements. */
    std::vector<int> row;	/**< the row index of the non-zero elements. */
    
    
    int getCol(){
        return this->n_col;
    }
    int getRow(){
        return this->n_row;
    }
    int getCouple(){
        return this->n_couple;
    }
    
    
    //构造函数
    sparseMatrix(int _n_row, int _n_col){
        n_col = _n_col;
        n_row = _n_row;
    }
    
    /**
     * Construction function.
     *
     * @param _n_row the init. value of the nubmer of the rows.
     * @param _n_col the init. value of the nubmer of the columns.
     * @param _n_couple the max number of the non-zero elements in each rows.
     */
    sparseMatrix(int _n_row, int _n_col, int _n_couple)
    {
        n_col = _n_col;
        n_row = _n_row;
        n_couple=_n_couple;
        ele.resize(_n_row*_n_couple);
        col.resize(_n_row*_n_couple, -1);
        row.resize(_n_row + 1);
        for (int i = 1; i < _n_row + 1; ++i)
            row[i] = _n_couple * i;
        for (int i = 0; i < _n_row; ++i)
            col[row[i]] = i;
    };
    
    /**
     * Set a non-zero element.
     *
     * @param _i the row index of the element to put in.
     * @param _j the column index of the element to put in.
     * @param _ele the value of the element to put in.
     *
     * @return the finish status.
     */
    //rewritten by tzry
    //fix some bugs on special issues
    int set_ele(int _i, int _j, double _ele)
    {
        int i;bool flag=false;
        /**
         * If the input element is at the diagonal, just put
         * it to the first place of its row.
         *
         */
        int start=row[_i];
        int maxI=start+n_couple;
        for(i=start;i<maxI;i++){
            int nowcol=col[i];
            
            if(nowcol==_j){
                col[i]=_j;
                ele[i]=_ele;
                flag=true;
                break;
            }
            else if(nowcol==-1){//尚未使用过
                col[i]=_j;
                ele[i]=_ele;
                flag=true;
                break;
            }
            else if(ele[i]==0){//初始的对角数据
                col[i]=_j;
                ele[i]=_ele;
                flag=true;
                break;
            }
        }
        if(flag)
            return 0;
        else
            return -1;
    };
    
    /**
     * Get an element form the matrix with its row and column
     * index.
     *
     * @param _i the row index.
     * @param _j the column index.
     *
     * @return the element value.
     */
    double get_ele(int _i, int _j)
    {
        for (int j = row[_i]; j < row[_i+1]; ++j)
            if (col[j] == _j)
                return ele[j];
        return 0;
    };
    
    
    /**
     * To list all the non-zero elements with the format: (row
     * index, column index) = value of the element.
     *
     */
    void show()
    {
        for (int i = 0; i < n_row; ++i)
            for (int j = row[i]; j < row[i + 1]; ++j)
                std::cout << "(" << i << ", " << col[j] << ")=" << ele[j] << std::endl;
        return;
    };
    
    
    int gs_step(std::vector<double> &_b, std::vector<double> &_u)
    {
        for (int i = 0; i < n_row; ++i)
        {
            double S =_b[i];
            for (int j = row[i] + 1; j < row[i + 1]; ++j)
                S -= ele[j] * _u[col[j]];
            _u[i] = S / ele[row[i]];
        }
        return 0;
    };
    
    
    /**
     * Compress the empty non-zero elements. Just renumber the
     * index of the non-zero elements and push them to the front
     * places of the element array and the column array. And
     * release the memory.
     *
     */
    //rewritten by tzry
    //fix some bugs on special issues
    int compress()
    {
        
        int p1 = 0;
        int i;
        int old_row = row[0];
        for(i = 0; i < n_row; i++)
        {
            
            for(int k = old_row; k < old_row+n_couple; k++)
            {
                if(col[k]==-1){//未分配
                    break;
                }
                //已分配，记录
                col[p1]=col[k];
                ele[p1]=ele[k];
                
                p1++;
                
            }
            old_row = row[i + 1];
            row[i + 1] = p1;
        }
        ele.resize(p1);
        col.resize(p1);
        return 0;
    };
    
    
    
    //Added by tzry
    //noZeroPercent是稀疏度
    int init(double noZeroPercent){
        //初始化稀疏矩阵
        
        unsigned int a = 0;
        
        int MAX = (~a)/2;
        
        srand((unsigned)time(NULL));
        for(int i=0;i<n_row;i++){
            for(int j=0;j<n_col;j++){
                if(rand()*1.0/MAX<noZeroPercent){
                    this->set_ele(i, j, (float)(rand()*1.0/MAX));
                }
            }
        }
        return 0;
    }
    
    //数乘
    void multi(double times){
        std::vector<double>::iterator it;
        for(it=this->ele.begin();it!=this->ele.end();it++){
            (*it)*=times;
        }
    }
    
    
    //多线程获取任务行数
    int getMissionRow(){
        this->m.lock();
        try
        {
            int mR=this->missionRow;
            this->missionRow++;
            this->m.unlock();
            return mR;
        }
        catch(...)
        {
            this->m.unlock();
            throw;
        }
    }
    
    //测试文件输出
    void log(char* filename,char key,int row,int col,double value){
        std::ofstream fout(filename,std::ios::app);
        //fout<<key<<"("<<row<<","<<col<<")="<<value<<";"<<std::endl;
        fout<<row<<' '<<col<<' '<<value<<std::endl;
    }
    void show(char key)
    {
        for (int i = 0; i < n_row; ++i)
            for (int j = row[i]; j < row[i + 1]; ++j)
                log("/Users/tzry/Desktop/graduation-project/sparse/output.txt",key,i+1,col[j]+1,ele[j]);
        return;
    }
    
    
    
    
    void E(){
        for(int i=0;i<this->getRow();i++){
            this->set_ele(i, i, 1);
        }
    }
};



//线程工作函数
 void workFun(sparseMatrix* a,sparseMatrix* b,tmpSparse* result,int choice){
     while(choice<a->getRow()){
         //choice为当前A行数
         
         double orin[a->getCol()];
         double r[b->getCol()];
         for(int i=0;i<a->getCol();i++)
             orin[i]=0;
         for(int i=0;i<b->getCol();i++)
             r[i]=0;
         
         for(int i=a->row.at(choice);i<a->row.at(choice+1)&&a->col.at(i)!=-1&&a->ele.at(i)!=0;i++){
             orin[a->col.at(i)]=a->ele.at(i);
         }
         
         for(int row=0;row<b->getRow();row++){
             for(int colIndex=b->row[row];b->col[colIndex]!=-1&&colIndex<b->row[row+1];colIndex++){
                 r[b->col[colIndex]]=r[b->col[colIndex]]+orin[row]*b->ele[colIndex];
             }
         }
         
         for(int i=0;i<b->getCol();i++){
             if(r[i]!=0)
                 result->setEle(choice, i, r[i]);
         }
         
         choice+=THREADCOUNT;
     }
     
}





//格式转换
sparseMatrix* sparse(tmpSparse* tps){
    int index=0;
    int couple=0;
    sparseMatrix* result=new sparseMatrix(tps->getRow(),tps->getCol());
    for(int i=0;i<tps->getRow();i++){
        //每一行
        if(tps->col[i].size()==0){
            //无ele
            result->ele.push_back(0);
            if(i<=tps->getCol())
                result->col.push_back(i);
            else
                result->col.push_back(tps->getCol());
            result->row.push_back(index);
            index++;
        }
        else{
            result->row.push_back(index);
            for(int tpsi=0;tpsi<tps->col[i].size();tpsi++){
                //每一个符合要求的
                result->col.push_back(tps->col[i][tpsi]);
                result->ele.push_back(tps->ele[i][tpsi]);
                index++;
            }
        }
    }
    result->row.push_back(index);
    return result;
}

//稀疏矩阵乘法
//认为阶数是正常的
sparseMatrix* multi(sparseMatrix* a,sparseMatrix* b){
    a->missionRow=0;
    if(a->getCol()!=b->getRow()){
        throw "cannot be multied";
    }
    
    tmpSparse* result=new tmpSparse(a->getRow(),b->getCol(),a->getCouple()>b->getCouple()?a->getCouple():b->getCouple());
    
    std::thread threads[THREADCOUNT];
    for(int i=0;i<THREADCOUNT;i++){
        threads[i]=std::thread(workFun,a,b,result,i);
        
    }
    for(int i=0;i<THREADCOUNT;i++)
    {
        (threads[i]).join();
    }
    return sparse(result);
}




//向量乘法线程工作函数
void sparseVectorWorkFun(sparseMatrix* sM,sparseVector* sV,sparseVector* result,int row){
    while(row<sV->getLength()){
        
        for(int i=sM->row.at(row);i<sM->row.at(row+1)&&sM->col.at(i)!=-1&&sM->ele.at(i)!=0;i++){
            result->addEle(row, sM->ele.at(i)*sV->getValue(sM->col.at(i)));
        }
        
        row+=THREADCOUNT;
    }
}


//向量乘法
sparseVector* multi(sparseMatrix* sM,sparseVector* sV){
    sM->missionRow=0;
    if(sM->getCol()!=sV->getLength()){
        throw "cannot be multied";
    }
    sparseVector* result=new sparseVector(sM->getCol());
    std::thread threads[THREADCOUNT];
    for(int i=0;i<THREADCOUNT;i++){
        threads[i]=std::thread(sparseVectorWorkFun,sM,sV,result,i);
        
    }
    for(int i=0;i<THREADCOUNT;i++)
    {
        (threads[i]).join();
    }
    return result;
}






sparseMatrix* add(sparseMatrix* a,sparseMatrix*b){
    if(a->getRow()!=b->getRow()||a->getCol()!=b->getCol()){
        throw "can't add";
    }
    int couple=a->getCouple()+b->getCouple();
    sparseMatrix* ret=new sparseMatrix(a->getRow(),a->getCol(),couple);
    for(int i=1;i<=a->getRow();i++){
        for(int j=a->row[i-1];j<a->row[i]&&a->ele[j]!=0;j++){
            ret->set_ele(i-1, a->col[j], a->ele[j]);
        }
        for(int j=b->row[i-1];j<b->row[i]&&b->ele[j]!=0;j++){
            if(ret->get_ele(i-1, b->col[j])==0){
                ret->set_ele(i-1, b->col[j], b->ele[j]);
            }
            else{
                ret->set_ele(i-1, b->col[j], b->ele[j]+ret->get_ele(i-1, b->col[j]));
            }
        }
    }
    ret->compress();
    return ret;
    
}


#endif
