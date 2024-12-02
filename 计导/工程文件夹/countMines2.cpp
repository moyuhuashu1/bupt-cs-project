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

//��������������Χ�ĵ�������
int countMines(int x, int y, int rows, int columns) {
	x--;
	y--;
	int count = 0, i = 0, j = 0, left = y - 1, right = y + 1, up = x - 1, down = x + 1;
	//�����жϱ���Ա߽��������Χ���ӵļ���Խ��
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
	//�Ը��������Χ�ĸ��ӽ����жϣ���һ���׾�������һ
	for (i = up; i <= down; i++) {
		for (j = left; j <= right; j++) {
			if (originalBoard[i][j] == '*') {
				count++;
			}
		}
	}
	return count;
}
