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

//实现左键点击
int leftClick(int x, int y, int rows, int columns, int* safePtr, int all) {

	x = x - 1;
	y = y - 1;
	int count = 0, left = y - 1, right = y + 1, up = x - 1, down = x + 1, i = 0, j = 0, condition = 1;
	//增加判断使问号也可左键点开
	if (chessBoard[x][y] == '*' || (chessBoard[x][y] == '?' && originalBoard[x][y] == '*')) {
		chessBoard[x][y] = '@';
		//若触雷即游戏失败
		condition = 0;

	}
	//增加判断使问号也可左键点开
	else if (chessBoard[x][y] == '.' || (chessBoard[x][y] == '?' && originalBoard[x][y] == '.')) {
		//计算该点周围雷数
		count = countMines(x + 1, y + 1, rows, columns);
		//使该点显示其周围雷数
		chessBoard[x][y] = count + '0';
		//每点开一个不是雷的格子则当前安全的格子数加一
		(*safePtr)++;
		if (count != 0 && click == 1) {
			mciSendString("seek 单 to start", NULL, 0, NULL);
			mciSendString("play 单", NULL, 0, NULL);
			click = 0;
		}
		if (count == 0) {
			if (click == 1) {
				mciSendString("seek 多 to start", NULL, 0, NULL);
				mciSendString("play 多", NULL, 0, NULL);
				click = 0;
			}
			//增加判断避免对边角坐标点周围格子的操作越界
			if (y == 0) {
				left = y;
			}
			if (y == columns - 1) {
				right = y;
			}
			if (x == 0) {
				up = x;
			}
			if (x == rows - 1) {
				down = x;
			}
			//当操作点周围地雷数为零时则递归地打开与其相邻的格子
			for (i = up; i <= down; i++) {
				for (j = left; j <= right; j++) {
					//增加判断使问号也可左键点开
					if (chessBoard[i][j] == '.' || (chessBoard[i][j] == '?' && originalBoard[i][j] == '.')) {
						condition = leftClick(i + 1, j + 1, rows, columns, safePtr, all);
					}
				}
			}
		}
	}

	//若当前安全格子数等于总无雷格子数则游戏胜利
	if (*safePtr == all) {
		condition = 2;

	}
	return condition;
}

//实现右键点击
int rightClick(int x, int y, int mines) {
	x--;
	y--;
	//如果该位置为未被打开的格子，则被标记为地雷， 以"!"显示
	if (chessBoard[x][y] == '.' || chessBoard[x][y] == '*') {
		mciSendString("seek 旗 to start", NULL, 0, NULL);
		mciSendString("play 旗", NULL, 0, NULL);
		chessBoard[x][y] = '!';
		//标记！时剩余雷数要-1
		mines--;
	}
	//如果该位置为地雷标记的格子，则被标记为问题， 以"?"显示
	else if (chessBoard[x][y] == '!') {
		mciSendString("seek 问 to start", NULL, 0, NULL);
		mciSendString("play 问", NULL, 0, NULL);
		chessBoard[x][y] = '?';
		mines++;
	}
	//如果该位置为问题标记，该位置可以被取消标记，恢复为原本的状态
	else if (chessBoard[x][y] == '?') {
		mciSendString("seek 取 to start", NULL, 0, NULL);
		mciSendString("play 取", NULL, 0, NULL);
		//将显示棋盘该位置恢复为原本的状态
		chessBoard[x][y] = originalBoard[x][y];
	}
	return mines;
}

//实现双击
int doubleClick(int x, int y, int rows, int columns, int* safePtr, int all) {
	x = x - 1;
	y = y - 1;
	int flagCount = 0, left = y - 1, right = y + 1, up = x - 1, down = x + 1, i = 0, j = 0, condition = 1;
	//增加判断避免对边角坐标点周围格子的操作越界
	if (y == 0) {
		left = y;
	}
	if (y == columns - 1) {
		right = y;
	}
	if (x == 0) {
		up = x;
	}
	if (x == rows - 1) {
		down = x;
	}
	//计算该坐标点周围已标记雷数
	for (i = up; i <= down; i++) {
		for (j = left; j <= right; j++) {
			if (chessBoard[i][j] == '!') {
				flagCount++;
			}
		}
	}
	//当双击位置周围已标记雷数等于该位置数字时操作有效
	if (flagCount == chessBoard[x][y] - '0') {
		//判断数字周围没有标错的地雷
		for (i = up; i <= down; i++) {
			for (j = left; j <= right; j++) {
				//如果该数字周围有标错的地雷，则游戏失败
				if (chessBoard[i][j] != '!' && originalBoard[i][j] == '*') {
					chessBoard[i][j] = '&';
					condition = 0;
					//计时判断变量变为0
				}
			}
		}
		//若游戏未结束，则对该数字周围未标记为地雷且未打开的格子均进行一次左键单击操作
		for (i = up; i <= down && condition == 1; i++) {
			for (j = left; j <= right && condition == 1; j++) {
				condition = leftClick(i + 1, j + 1, rows, columns, safePtr, all);
			}
		}
	}
	return condition;
}
