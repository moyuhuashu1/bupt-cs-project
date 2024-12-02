#include "myLib1.h"

extern char** chessBoard;
extern char** originalBoard;
extern int click;
extern const int TOP_BOTTOM_MARGIN;
extern const int LEFT_RIGHT_MARGIN;
extern const int BLOCK_SIZE;
extern const int EMOJI_SIZE;
extern const int NUM_WIDTH;
extern const int NUM_HEIGHT;

//��������Ҫ������ڴ�
void creatChessBoard(int rows, int columns) {
	int i;
	//�������������̵Ķ�ά�ַ����鶯̬�����ڴ�
	chessBoard = (char**)malloc(rows * sizeof(char*));
	originalBoard = (char**)malloc(rows * sizeof(char*));
	//chessBoard�������ڲ������ı䡢���ӡ��original Board�������ڼ�¼�������̳�״̬
	if (chessBoard != NULL && originalBoard != NULL) {
		for (i = 0; i < rows; i++) {
			*(chessBoard + i) = (char*)malloc((columns + 1) * sizeof(char));
			*(originalBoard + i) = (char*)malloc((columns + 1) * sizeof(char));
		}
	}
}

//��������Ҫ�����������
void updateChessBoard(int rows, int columns, int allMines) {
	int i, j, count;
	//�Ƚ�����ȫ����������
	for (i = 0; i < rows; i++) {
		for (j = 0; j < columns; j++) {
			chessBoard[i][j] = '.';
			originalBoard[i][j] = '.';
		}
	}
	//�������
	srand((unsigned)time(NULL));
	//ѭ�������Ѳ��õ�����������������count���ڼ�¼�Ѳ�������
	for (count = 0; count < allMines;) {
		//��ȡ�����̷�Χ�ڵĵ������Ϊ�������꣬ʵ��ÿ����Ϸ�׷ֲ����
		i = rand() % rows;
		j = rand() % columns;
		//����������ظ�
		if (chessBoard[i][j] != '*') {
			chessBoard[i][j] = '*';
			originalBoard[i][j] = '*';
			count++;
		}
	}
}

//�ͷ��ڴ�
void freeChessBoard(int rows) {
	int i;
	for (i = 0; i < rows; i++) {
		free(*(chessBoard + i));
		free(*(originalBoard + i));
		chessBoard[i] = NULL;
		originalBoard[i] = NULL;
	}
	free(chessBoard);
	free(originalBoard);
	chessBoard = NULL;
	originalBoard = NULL;
}