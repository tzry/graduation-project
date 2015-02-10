//
//  sparseVector.h
//  sparse
//
//  Created by tzry on 15/2/10.
//  Copyright (c) 2015年 tzry. All rights reserved.
//
#include<stdlib.h>
#include<thread>
#include<mutex>

#ifndef sparse_sparseVector_h
#define sparse_sparseVector_h

class sparseVector{
private:
    int length;
    double* ele;
    std::mutex* m;
public:
    int getLength(){
        return length;
    }
    double getValue(int pos){
        return *(ele+pos);
    }
    //构造函数
    sparseVector(int l){
        length=l;
        ele=(double*)malloc(sizeof(double)*l);
        m=(std::mutex*)malloc(sizeof(std::mutex)*l);
        double* now=ele;
        std::mutex* nowm;
        for(int i=0;i<l;i++){
            *now=0;
            nowm=new std::mutex();
            now++;
            nowm++;
        }
    }
    //随机初始化
    void init(){
        unsigned int a = 0;
        int MAX = (~a)/2;
        srand((unsigned)time(NULL));
        
        for(int i=0;i<length;i++){
            *(ele+i)=(float)(rand()*1.0/MAX);
        }
    }
    
    
    
};

#endif
