//
//  tmpSparse.h
//  sparse
//
//  Created by tzry on 15/2/7.
//  Copyright (c) 2015年 tzry. All rights reserved.
//

#ifndef tmpSparse_h
#define tmpSparse_h
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>

class tmpSparse
{
private:
    int n_col;		/**< number of the columns. */
    int n_row;		/**< number of the rows. */
    
    std::mutex m;//互斥量
    
public:
    int getRow(){
        return this->n_row;
    }
    int getCol(){
        return this->n_col;
    }
    
    std::vector<double> *ele; //存储的数值
    std::vector<int> *col;	//存储的列号
    
    
    tmpSparse(int _row,int _col,int initLength){
        n_col=_col;
        n_row=_row;
        ele=new std::vector<double>[_row];
        col=new std::vector<int>[_row];
    }
    
    //设置单个位置的值（不考虑重复）
    void setEle(int _row,int _col,double _ele){
        this->m.lock();
        try
        {
            std::vector<double> *ev=&ele[_row];
            std::vector<int> *cv=&col[_row];
            ev->push_back(_ele);
            cv->push_back(_col);
            
            this->m.unlock();
        }
        catch(...)
        {
            this->m.unlock();
            throw;
        }

    }
    
    
    
    //在指定位置增加指定数
    void addEle(int _row,int _col,double _ele){
        this->m.lock();
        try
        {
            std::vector<double> *ev=&ele[_row];
            std::vector<int> *cv=&col[_row];
            for(int i=0;i<cv->size();i++){
                if((*cv)[i]==_col){//已存在
                    (*cv)[i]+=_ele;
                    this->m.unlock();
                    return;
                }
            }
            //不存在
            ev->push_back(_ele);
            cv->push_back(_col);
            
            this->m.unlock();
        }
        catch(...)
        {
            this->m.unlock();
            throw;
        }
    }
    
    
    
};


#endif
