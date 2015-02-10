//
//  complexNumber.h
//  sparse
//
//  Created by tzry on 15/2/10.
//  Copyright (c) 2015年 tzry. All rights reserved.
//
#ifndef sparse_complexNumber_h
#define sparse_complexNumber_h
#include <stdlib.h>
struct complexNumber{
    double realPart;
    double imaginaryPart;
    complexNumber(double a,double b){
        realPart=a;
        imaginaryPart=b;
    }
    complexNumber(){
        realPart=0;
        imaginaryPart=0;
    }
    //格式化输出
    char* toString(){
        char* ret=(char*)malloc(sizeof(char)*30);
        sprintf(ret, "%lf+%lfi", realPart,imaginaryPart);
        return ret;
    }
    //乘法重载
    friend complexNumber operator *(complexNumber &a,complexNumber &b){
        complexNumber cN(a.realPart*b.realPart-a.imaginaryPart*b.imaginaryPart,
                         a.realPart*b.imaginaryPart+a.imaginaryPart*b.imaginaryPart);
        return cN;
    }
    friend complexNumber operator *(complexNumber &a,double b){
        complexNumber cN(a.realPart*b,
                         a.imaginaryPart*b);
        return cN;
    }
    friend complexNumber operator *(double b,complexNumber &a){
        complexNumber cN(a.realPart*b,
                         a.imaginaryPart*b);
        return cN;
    }

    //加法重载
    friend complexNumber operator +(complexNumber &a,complexNumber &b){
        complexNumber cN(a.realPart+b.realPart,a.imaginaryPart+b.imaginaryPart);
        return cN;
    }
};


#endif
