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

#define ROW 100 //行数
#define COL 100 //列数
#define COUPLE 5    //每行最多非零数
#define NONE_ZERO_PERCENT 0.03


int main(int argc, const char * argv[]) {
    //初始化稀疏矩阵
    //sparseMatrix* sparse=new sparseMatrix(ROW,COL,COUPLE);
    //sparse->init(NONE_ZERO_PERCENT);
    //sparse->compress();
    
    //debug 初始输出
    //sparse->show();
    
    //debug 数乘
    //sparse->multi(100);
    //sparse->show();
    //sparse->show('A');
    //multi(sparse, sparse)->show();
    
    //multi(sparse, sparse)->show('B');
    
    //debug 矩阵向量乘法
    //sparseVector* sV=new sparseVector(COL);
    //sV->init();
    //multi(sparse, sV);
    
    //复数运算
    //complexSparseMatrix* sparse=new complexSparseMatrix(ROW,COL,COUPLE);
    //sparse->init(NONE_ZERO_PERCENT);
    //sparse->compress();
    //sparse->show();
    //sparse->multi(100);
    //multi(sparse, sparse)->show();
    //complexSparseVector* sV=new complexSparseVector(COL);
    //sV->init();
    //multi(sparse,sV)->show();
    return 0;
}
