//
//  tmpComplexSparse.h
//  sparse
//
//  Created by tzry on 15/2/10.
//  Copyright (c) 2015年 tzry. All rights reserved.
//

#ifndef sparse_tmpComplexSparse_h
#define sparse_tmpComplexSparse_h
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include"complexNumber.h"
class tmpComplexSparse
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
    
    std::vector<complexNumber> *ele; //存储的数值
    std::vector<int> *col;	//存储的列号
    
    
    tmpComplexSparse(int _row,int _col,int initLength){
        n_col=_col;
        n_row=_row;
        ele=new std::vector<complexNumber>[_row];
        col=new std::vector<int>[_row];
    }
    
    //设置单个位置的值（不考虑重复）
    void setEle(int _row,int _col,complexNumber _ele){
        this->m.lock();
        try
        {
            std::vector<complexNumber> *ev=&ele[_row];
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
    
    
    
};



#endif
