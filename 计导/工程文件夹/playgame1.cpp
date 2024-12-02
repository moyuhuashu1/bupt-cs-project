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
		//��ȡ�����Ϣ
		int x = mouse.x, y = mouse.y, width = BLOCK_SIZE * columns + 2 * LEFT_RIGHT_MARGIN, height = BLOCK_SIZE * rows + 2 * TOP_BOTTOM_MARGIN, i, j;//������Ϸ����ĳ��Ϳ�
		x = mouse.x;
		y = mouse.y;
		//��������return����������Ϸģʽ��Ϊδѡ��״̬��condition����4��
		if (mouse.message == WM_LBUTTONDOWN && x >= width / 2 - 25 && x <= width / 2 + 25 && y >= 0 && y <= 20) {
			condition = 4;
			mciSendString("close ��", NULL, 0, NULL);
		}
		//����������ͼ����Ϸ��Ϊ�ؿ�״̬��condition����3��
		else if (mouse.message == WM_LBUTTONDOWN && x >= (width - EMOJI_SIZE) / 2 - 1 && x <= (width + EMOJI_SIZE) / 2 + 1 && y >= TOP_BOTTOM_MARGIN - EMOJI_SIZE - 3 - 1 && y <= TOP_BOTTOM_MARGIN - 3) {
			condition = 3;
			mciSendString("close ��", NULL, 0, NULL);
		}
		else {
			
			//��Ϸδ�����ҵ�������̽����ڲŲ�����Ч
			if (condition == 1 && y > TOP_BOTTOM_MARGIN && x > LEFT_RIGHT_MARGIN && y < height - TOP_BOTTOM_MARGIN && x < width - LEFT_RIGHT_MARGIN) {
				//����ɶ��ڲ������̵Ĳ�������
				i = (mouse.y - TOP_BOTTOM_MARGIN) / BLOCK_SIZE + 1;
				j = (mouse.x - LEFT_RIGHT_MARGIN) / BLOCK_SIZE + 1;
				//���������Ϊ�������
				if (mouse.message == WM_LBUTTONDOWN) {
					//first��ʾ�����̽���ĵ�һ�²����Ƿ�����ˣ�firstΪ1��ʾ��δ���У�Ϊ0��ʾ�Ѿ�����
					if (first == 1) {
						avoidFirst(i, j, rows, columns);
						//��һ����ʧȥ
						first = 0;
						startTime = clock()	;//ÿ�ζ�Ҫ�������õĳ�ʼʱ��
					}
					//�Բ������̽����������
					click = 1;
					if (rightDown != 1) {
						condition = leftClick(i, j, rows, columns, safePtr, all);
					}
					leftDown = 1;
				}
				if (mouse.message == WM_LBUTTONUP) {
					leftDown = 0;
				}
				//���������Ϊ�Ҽ�����
				if (mouse.message == WM_RBUTTONDOWN) {
					//�Բ������̽����Ҽ�����
					*minesPtr = rightClick(i, j, *minesPtr);
					rightDown = 1;
				}
				if (mouse.message == WM_RBUTTONUP) {
					rightDown = 0;
				}
				//���������Ϊ˫�����м䵥�����߶�ʱ�����������˫����
				if ((mouse.message == WM_LBUTTONDBLCLK) || (mouse.message == WM_MBUTTONUP)||(leftDown==1&&rightDown==1)){
					condition = doubleClick(i, j, rows, columns, safePtr, all);
				}
			}
		}
	}
	//�����˫�������ܸı���Ϸ״̬�����Է�����Ϸ״̬
	return condition;
}

//�����һ�¼�����Χ��̫��
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
