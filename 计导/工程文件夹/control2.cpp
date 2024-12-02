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

//ʵ��������
int leftClick(int x, int y, int rows, int columns, int* safePtr, int all) {

	x = x - 1;
	y = y - 1;
	int count = 0, left = y - 1, right = y + 1, up = x - 1, down = x + 1, i = 0, j = 0, condition = 1;
	//�����ж�ʹ�ʺ�Ҳ������㿪
	if (chessBoard[x][y] == '*' || (chessBoard[x][y] == '?' && originalBoard[x][y] == '*')) {
		chessBoard[x][y] = '@';
		//�����׼���Ϸʧ��
		condition = 0;

	}
	//�����ж�ʹ�ʺ�Ҳ������㿪
	else if (chessBoard[x][y] == '.' || (chessBoard[x][y] == '?' && originalBoard[x][y] == '.')) {
		//����õ���Χ����
		count = countMines(x + 1, y + 1, rows, columns);
		//ʹ�õ���ʾ����Χ����
		chessBoard[x][y] = count + '0';
		//ÿ�㿪һ�������׵ĸ�����ǰ��ȫ�ĸ�������һ
		(*safePtr)++;
		if (count != 0 && click == 1) {
			mciSendString("seek �� to start", NULL, 0, NULL);
			mciSendString("play ��", NULL, 0, NULL);
			click = 0;
		}
		if (count == 0) {
			if (click == 1) {
				mciSendString("seek �� to start", NULL, 0, NULL);
				mciSendString("play ��", NULL, 0, NULL);
				click = 0;
			}
			//�����жϱ���Ա߽��������Χ���ӵĲ���Խ��
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
			//����������Χ������Ϊ��ʱ��ݹ�ش��������ڵĸ���
			for (i = up; i <= down; i++) {
				for (j = left; j <= right; j++) {
					//�����ж�ʹ�ʺ�Ҳ������㿪
					if (chessBoard[i][j] == '.' || (chessBoard[i][j] == '?' && originalBoard[i][j] == '.')) {
						condition = leftClick(i + 1, j + 1, rows, columns, safePtr, all);
					}
				}
			}
		}
	}

	//����ǰ��ȫ���������������׸���������Ϸʤ��
	if (*safePtr == all) {
		condition = 2;

	}
	return condition;
}

//ʵ���Ҽ����
int rightClick(int x, int y, int mines) {
	x--;
	y--;
	//�����λ��Ϊδ���򿪵ĸ��ӣ��򱻱��Ϊ���ף� ��"!"��ʾ
	if (chessBoard[x][y] == '.' || chessBoard[x][y] == '*') {
		mciSendString("seek �� to start", NULL, 0, NULL);
		mciSendString("play ��", NULL, 0, NULL);
		chessBoard[x][y] = '!';
		//��ǣ�ʱʣ������Ҫ-1
		mines--;
	}
	//�����λ��Ϊ���ױ�ǵĸ��ӣ��򱻱��Ϊ���⣬ ��"?"��ʾ
	else if (chessBoard[x][y] == '!') {
		mciSendString("seek �� to start", NULL, 0, NULL);
		mciSendString("play ��", NULL, 0, NULL);
		chessBoard[x][y] = '?';
		mines++;
	}
	//�����λ��Ϊ�����ǣ���λ�ÿ��Ա�ȡ����ǣ��ָ�Ϊԭ����״̬
	else if (chessBoard[x][y] == '?') {
		mciSendString("seek ȡ to start", NULL, 0, NULL);
		mciSendString("play ȡ", NULL, 0, NULL);
		//����ʾ���̸�λ�ûָ�Ϊԭ����״̬
		chessBoard[x][y] = originalBoard[x][y];
	}
	return mines;
}

//ʵ��˫��
int doubleClick(int x, int y, int rows, int columns, int* safePtr, int all) {
	x = x - 1;
	y = y - 1;
	int flagCount = 0, left = y - 1, right = y + 1, up = x - 1, down = x + 1, i = 0, j = 0, condition = 1;
	//�����жϱ���Ա߽��������Χ���ӵĲ���Խ��
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
	//������������Χ�ѱ������
	for (i = up; i <= down; i++) {
		for (j = left; j <= right; j++) {
			if (chessBoard[i][j] == '!') {
				flagCount++;
			}
		}
	}
	//��˫��λ����Χ�ѱ���������ڸ�λ������ʱ������Ч
	if (flagCount == chessBoard[x][y] - '0') {
		//�ж�������Χû�б��ĵ���
		for (i = up; i <= down; i++) {
			for (j = left; j <= right; j++) {
				//�����������Χ�б��ĵ��ף�����Ϸʧ��
				if (chessBoard[i][j] != '!' && originalBoard[i][j] == '*') {
					chessBoard[i][j] = '&';
					condition = 0;
					//��ʱ�жϱ�����Ϊ0
				}
			}
		}
		//����Ϸδ��������Ը�������Χδ���Ϊ������δ�򿪵ĸ��Ӿ�����һ�������������
		for (i = up; i <= down && condition == 1; i++) {
			for (j = left; j <= right && condition == 1; j++) {
				condition = leftClick(i + 1, j + 1, rows, columns, safePtr, all);
			}
		}
	}
	return condition;
}
