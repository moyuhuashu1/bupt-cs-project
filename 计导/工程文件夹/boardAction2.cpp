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

//根据棋盘要求分配内存
void creatChessBoard(int rows, int columns) {
	int i;
	//给两个用作棋盘的二维字符数组动态分配内存
	chessBoard = (char**)malloc(rows * sizeof(char*));
	originalBoard = (char**)malloc(rows * sizeof(char*));
	//chessBoard棋盘用于操作、改变、与打印，original Board棋盘用于记录保存棋盘初状态
	if (chessBoard != NULL && originalBoard != NULL) {
		for (i = 0; i < rows; i++) {
			*(chessBoard + i) = (char*)malloc((columns + 1) * sizeof(char));
			*(originalBoard + i) = (char*)malloc((columns + 1) * sizeof(char));
		}
	}
}

//根据棋盘要求布置随机棋盘
void updateChessBoard(int rows, int columns, int allMines) {
	int i, j, count;
	//先将棋盘全部布成无雷
	for (i = 0; i < rows; i++) {
		for (j = 0; j < columns; j++) {
			chessBoard[i][j] = '.';
			originalBoard[i][j] = '.';
		}
	}
	//随机种子
	srand((unsigned)time(NULL));
	//循环至当已布置的雷数等于总雷数，count用于记录已布置雷数
	for (count = 0; count < allMines;) {
		//获取在棋盘范围内的的随机数为横纵坐标，实现每次游戏雷分布随机
		i = rand() % rows;
		j = rand() % columns;
		//避免随机数重复
		if (chessBoard[i][j] != '*') {
			chessBoard[i][j] = '*';
			originalBoard[i][j] = '*';
			count++;
		}
	}
}

//释放内存
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