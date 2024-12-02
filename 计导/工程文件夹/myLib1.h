#define _CRT_SECURE_NO_WARNINGS

/*————————头文件说明————————*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include <graphics.h>
#include< windows.h >
#include <chrono>
#include <thread>
#include <conio.h>
#include<mmsystem.h>
#pragma comment(lib, "winmm.lib")


/*—————————函数说明—————————*/
//根据棋盘要求分配内存
void creatChessBoard(int rows, int columns);
//根据棋盘要求创建随机棋盘
void updateChessBoard(int rows, int columns, int allMines);
//避免第一下及其周围雷太多
void avoidFirst(int x, int y, int rows, int columns);
//释放内存
void freeChessBoard(int rows);
//实现左键点击
int leftClick(int x, int y, int rows, int columns, int* safePtr, int all);
//计算给定坐标点周围的地雷数量
int countMines(int x, int y, int rows, int columns);
//实现右键点击
int rightClick(int x, int y, int mines);
//实现双击
int doubleClick(int x, int y, int rows, int columns, int* safePtr, int all);
//选择模式的页面
int chooseMode(int* rowsPtr, int* columnsPtr, int* minesPtr);
//选择模式的内部操作
int chooseMode_Opra(int* rowsPtr, int* columnsPtr, int* minesPtr);
//初始化加载图片
void init_IMG();
//打印游戏界面//应该可以在这里实现计时
void printBoard(int height, int  width, int rows, int columns, int condition, int mines, int* overPtr);
//打印剩余雷数
void printMinesCount(int mines);
//打印游戏棋盘
void printChessBoard(int rows, int columns, int condition);
//打印游戏状态
void printCondition(int height, int  width, int condition, int* overPtr);
//游戏中的操作
int playgame(int rows, int columns, int* safePtr, int all, int condition,int* minesPtr);
//计时器
void timeCount(int width);
