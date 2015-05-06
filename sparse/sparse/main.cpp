//
//  main.cpp
//  sparse
//
//  Created by tzry on 15/2/7.
//  Copyright (c) 2015年 tzry. All rights reserved.
//

#include <iostream>
#include "sparseMatrix.h"
#include<stdlib.h>
#include "complexSparseMatrix.h"

#define ROW 100000 //行数
#define COL 100000 //列数
#define COUPLE 1500   //每行最多非零数
#define NONE_ZERO_PERCENT 0.01


int main(int argc, const char * argv[]) {
    //初始化稀疏矩阵
    sparseMatrix* sparse=new sparseMatrix(ROW,COL,COUPLE);
    sparse->init(NONE_ZERO_PERCENT);
    sparse->compress();
    
    //debug 初始输出
    sparse->show('m');
    
    //debug 数乘
    /*sparse->multi(100);
    sparse->show();
    sparse->show('A');*/
    
    clock_t start,finish;double duration;
    start=clock();
    multi(sparse, sparse);
    finish=clock();
    duration=1000*(double)(finish-start)/CLOCKS_PER_SEC;
    std::cout<<duration<<std::endl;
   /* multi(sparse, sparse)->show();
    
    multi(sparse, sparse)->show('B');
    
    //debug 矩阵向量乘法
    sparseVector* sV=new sparseVector(COL);
    sV->init();
    multi(sparse, sV);*/

    
    
    //复数运算
    /*
    complexSparseMatrix* Csparse=new complexSparseMatrix(ROW,COL,COUPLE);
    Csparse->init(NONE_ZERO_PERCENT);
    Csparse->compress();
    Csparse->show();
    Csparse->multi(100);
    multi(Csparse, Csparse)->show();
    complexSparseVector* csV=new complexSparseVector(COL);
    csV->init();
    multi(Csparse,csV)->show();*/
    return 0;
}
