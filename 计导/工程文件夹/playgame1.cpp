#include "myLib1.h"

extern char** chessBoard;
extern char** originalBoard;
extern clock_t startTime;
extern int click;
extern int first;
extern int leftDown;
extern int rightDown;
extern const int TOP_BOTTOM_MARGIN;
extern const int LEFT_RIGHT_MARGIN;
extern const int BLOCK_SIZE;
extern const int EMOJI_SIZE;
extern const int NUM_WIDTH;
extern const int NUM_HEIGHT;


int playgame(int rows, int columns, int* safePtr, int all, int condition,int* minesPtr) {
	
	ExMessage mouse;
	if (peekmessage(&mouse, EX_MOUSE, true)) {
		//获取鼠标信息
		int x = mouse.x, y = mouse.y, width = BLOCK_SIZE * columns + 2 * LEFT_RIGHT_MARGIN, height = BLOCK_SIZE * rows + 2 * TOP_BOTTOM_MARGIN, i, j;//计算游戏界面的长和宽
		x = mouse.x;
		y = mouse.y;
		//左键点击“return”区域则游戏模式变为未选择状态（condition等于4）
		if (mouse.message == WM_LBUTTONDOWN && x >= width / 2 - 25 && x <= width / 2 + 25 && y >= 0 && y <= 20) {
			condition = 4;
			mciSendString("close 初", NULL, 0, NULL);
		}
		//左键点击表情图则游戏变为重开状态（condition等于3）
		else if (mouse.message == WM_LBUTTONDOWN && x >= (width - EMOJI_SIZE) / 2 - 1 && x <= (width + EMOJI_SIZE) / 2 + 1 && y >= TOP_BOTTOM_MARGIN - EMOJI_SIZE - 3 - 1 && y <= TOP_BOTTOM_MARGIN - 3) {
			condition = 3;
			mciSendString("close 初", NULL, 0, NULL);
		}
		else {
			
			//游戏未结束且点击在棋盘界面内才操作有效
			if (condition == 1 && y > TOP_BOTTOM_MARGIN && x > LEFT_RIGHT_MARGIN && y < height - TOP_BOTTOM_MARGIN && x < width - LEFT_RIGHT_MARGIN) {
				//换算成对于操作棋盘的操作坐标
				i = (mouse.y - TOP_BOTTOM_MARGIN) / BLOCK_SIZE + 1;
				j = (mouse.x - LEFT_RIGHT_MARGIN) / BLOCK_SIZE + 1;
				//如果鼠标操作为左键单击
				if (mouse.message == WM_LBUTTONDOWN) {
					//first表示对棋盘界面的第一下操作是否进行了，first为1表示还未进行，为0表示已经进行
					if (first == 1) {
						avoidFirst(i, j, rows, columns);
						//第一次已失去
						first = 0;
						startTime = clock()	;//每次都要更新设置的初始时间
					}
					//对操作棋盘进行左键单击
					click = 1;
					if (rightDown != 1) {
						condition = leftClick(i, j, rows, columns, safePtr, all);
					}
					leftDown = 1;
				}
				if (mouse.message == WM_LBUTTONUP) {
					leftDown = 0;
				}
				//如果鼠标操作为右键单击
				if (mouse.message == WM_RBUTTONDOWN) {
					//对操作棋盘进行右键单击
					*minesPtr = rightClick(i, j, *minesPtr);
					rightDown = 1;
				}
				if (mouse.message == WM_RBUTTONUP) {
					rightDown = 0;
				}
				//如果鼠标操作为双击（中间单击或者短时间内左键两次双击）
				if ((mouse.message == WM_LBUTTONDBLCLK) || (mouse.message == WM_MBUTTONUP)||(leftDown==1&&rightDown==1)){
					condition = doubleClick(i, j, rows, columns, safePtr, all);
				}
			}
		}
	}
	//左键和双击都可能改变游戏状态，所以返回游戏状态
	return condition;
}

//避免第一下及其周围雷太多
void avoidFirst(int x, int y, int rows, int columns) {
	x = x - 1;
	y = y - 1;
	int left = y - 1, right = y + 1, up = x - 1, down = x + 1, i = 0, j = 0;
	{
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
	}
	for (i = up; i <= down; i++) {
		for (j = left; j <= right; j++) {
			if (originalBoard[i][j] == '*') {
				int tempX, tempY;
				for (tempX = rand() % rows, tempY = rand() % columns; true;) {
					if ((tempX<up || tempX>down || tempY > right || tempY < left) && originalBoard[tempX][tempY] != '*') {
						break;
					}
					tempX = rand() % rows;
					tempY = rand() % columns;
				}
				originalBoard[tempX][tempY] = '*';
				chessBoard[tempX][tempY] = '*';
				originalBoard[i][j] = '.';
				chessBoard[i][j] = '.';
			}
		}
	}
}
