//
//  complexSparseVector.h
//  sparse
//
//  Created by tzry on 15/2/10.
//  Copyright (c) 2015年 tzry. All rights reserved.
//
#include<stdlib.h>
#include <iostream>
#include "complexNumber.h"
#ifndef sparse_complexSparseVector_h
#define sparse_complexSparseVector_h


class complexSparseVector{
private:
    int length;
    complexNumber* ele;
public:
    int getLength(){
        return length;
    }
    complexNumber getValue(int pos){
        return *(ele+pos);
    }
    //构造函数
    complexSparseVector(int l){
        length=l;
        ele=(complexNumber*)malloc(sizeof(complexNumber)*l);
        complexNumber* now=ele;
        
        for(int i=0;i<l;i++){
            (*now).realPart=0;
            (*now).imaginaryPart=0;
            now++;
        }
    }
    //随机初始化
    void init(){
        unsigned int a = 0;
        int MAX = (~a)/2;
        srand((unsigned)time(NULL));
        
        for(int i=0;i<length;i++){
            (*(ele+i)).realPart=(float)(rand()*1.0/MAX);
            (*(ele+i)).imaginaryPart=(float)(rand()*1.0/MAX);
        }
    }
    
    void addEle(int pos,complexNumber value){
        (*(ele+pos)).realPart+=value.realPart;
        (*(ele+pos)).imaginaryPart+=value.imaginaryPart;
    }
    //呈现
    void show(){
        for(int i=0;i<length;i++)
            std::cout<<(ele+i)->toString()<<',';
        std::cout<<std::endl;
    }
};


#endif
