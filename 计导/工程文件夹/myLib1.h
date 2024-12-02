#define _CRT_SECURE_NO_WARNINGS

/*����������������ͷ�ļ�˵������������������*/
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


/*����������������������˵��������������������*/
//��������Ҫ������ڴ�
void creatChessBoard(int rows, int columns);
//��������Ҫ�󴴽��������
void updateChessBoard(int rows, int columns, int allMines);
//�����һ�¼�����Χ��̫��
void avoidFirst(int x, int y, int rows, int columns);
//�ͷ��ڴ�
void freeChessBoard(int rows);
//ʵ��������
int leftClick(int x, int y, int rows, int columns, int* safePtr, int all);
//��������������Χ�ĵ�������
int countMines(int x, int y, int rows, int columns);
//ʵ���Ҽ����
int rightClick(int x, int y, int mines);
//ʵ��˫��
int doubleClick(int x, int y, int rows, int columns, int* safePtr, int all);
//ѡ��ģʽ��ҳ��
int chooseMode(int* rowsPtr, int* columnsPtr, int* minesPtr);
//ѡ��ģʽ���ڲ�����
int chooseMode_Opra(int* rowsPtr, int* columnsPtr, int* minesPtr);
//��ʼ������ͼƬ
void init_IMG();
//��ӡ��Ϸ����//Ӧ�ÿ���������ʵ�ּ�ʱ
void printBoard(int height, int  width, int rows, int columns, int condition, int mines, int* overPtr);
//��ӡʣ������
void printMinesCount(int mines);
//��ӡ��Ϸ����
void printChessBoard(int rows, int columns, int condition);
//��ӡ��Ϸ״̬
void printCondition(int height, int  width, int condition, int* overPtr);
//��Ϸ�еĲ���
int playgame(int rows, int columns, int* safePtr, int all, int condition,int* minesPtr);
//��ʱ��
void timeCount(int width);
