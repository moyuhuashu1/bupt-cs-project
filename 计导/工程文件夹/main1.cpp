#include "myLib1.h"

//��������ȫ�ֱ�����ά�ַ�������Ϊ����
char** chessBoard;	//��������
char** originalBoard;	//��ʼ����	
int click;
int first;
int leftDown = 0, rightDown = 0;
//ͼ����� ������ֱ𴢴�����ͼƬ����������ͼƬ������ͼƬ 
IMAGE  block[15], num[12], emoji[3];
clock_t startTime;//���ó�ʼ��ʱ��Ϊ0
clock_t endTime;
/*��������������ȫ�ֳ���˵��������������������*/
//ȫ�ֳ��� ҳ�����±߿�ĳ���
extern const int TOP_BOTTOM_MARGIN = 75;
//ȫ�ֳ��� ҳ�����ұ߿�ĳ���
extern const int LEFT_RIGHT_MARGIN = 20;
//ȫ�ֳ��� ÿ��������ͼ�ĳߴ�
extern const int BLOCK_SIZE = 30;
//ȫ�ֳ��� ����ͼ�ĳߴ�
extern const int EMOJI_SIZE = 45;
//ȫ�ֳ��� ����ͼ�Ŀ��
extern const int NUM_WIDTH = 30;
//ȫ�ֳ��� ����ͼ�ĸ߶�
extern const int NUM_HEIGHT = 40;

int main(void) {
	//first�����һ�β����Ƿ�ִ�У�0����ִ�У�1��δִ�У���mines����ʣ��������allMines��������
	//conditionΪ0����Ϸʧ�ܣ�Ϊ1����Ϸ������Ϊ2����Ϸʤ����Ϊ3����������״̬��Ϊ4��δѡ��״̬
	//rows:����������columns������������height����Ϸ����߶ȣ�width����Ϸ������
	int rows, columns, mines, height, width, allMInes, condition, safe, all, over;
	//����ͼƬֻ��Ҫ����һ��
	init_IMG();
	//�õ���������״̬
	condition = chooseMode(&rows, &columns, &mines);
	allMInes = mines;
	//��Ϸ����߶ȵ�����������*ÿ����ͼ�ĳߴ�+���±߽�
	height = rows * BLOCK_SIZE + 2 * TOP_BOTTOM_MARGIN;
	//��Ϸ�����ȵ�����������*ÿ����ͼ�ĳߴ�+���ұ߽�
	width = columns * BLOCK_SIZE + 2 * LEFT_RIGHT_MARGIN;
	//��ʼ��������Ϸ����
	creatChessBoard(rows, columns);
	initgraph(width, height, EW_DBLCLKS);
	setbkcolor(RGB(230, 236, 202));
	cleardevice();
	/*��һ��������Ϸ�����Ǳ����Ժ�ÿ��return��ȥ��������Ϸ����ʱ������free���ϴη�������̵��ڴ�*/
	while (true) {
		//��������״̬
		if (condition == 3) {
			//�������̺�������ݣ����õ�һ�ε��״̬��������Ϸ״̬
			updateChessBoard(rows, columns, allMInes);
			safe = 0;
			all = rows * columns - allMInes;
			mines = allMInes;
			condition = 1;
			first = 1;
			over = 1;
			endTime = 0;
			startTime = 0;
			mciSendString("close ը", NULL, 0, NULL);
			mciSendString("close Ӯ", NULL, 0, NULL);
			mciSendString("open  music/���ɳ�ʼ��Ϸ����.mp3 alias ��", NULL, 0, NULL);
			mciSendString("play ��", NULL, 0, NULL);
			mciSendString("close ��", NULL, 0, NULL);
			mciSendString("close ��", NULL, 0, NULL);
			mciSendString("close ȡ", NULL, 0, NULL);
			mciSendString("close ��", NULL, 0, NULL);
			mciSendString("close ��", NULL, 0, NULL);
			mciSendString("open  music/�������.mp3 alias ��", NULL, 0, NULL);
			mciSendString("open  music/����ʺ�.mp3 alias ��", NULL, 0, NULL);
			mciSendString("open  music/ȡ�����.mp3 alias ȡ", NULL, 0, NULL);
			mciSendString("open  music/�������ֻ��һ������.mp3 alias ��", NULL, 0, NULL);
			mciSendString("open  music/��������ݹ��.mp3 alias ��", NULL, 0, NULL);
		}
		//δѡ��״̬
		if (condition == 4) {
			//�ص���Ϸ���洰�ڲ��ͷ�����������ڴ�
			closegraph();
			freeChessBoard(rows);
			//�����������״̬�������»�����Ϸ����
			condition = chooseMode(&rows, &columns, &mines);
			allMInes = mines;
			height = rows * BLOCK_SIZE + 2 * TOP_BOTTOM_MARGIN;
			width = columns * BLOCK_SIZE + 2 * LEFT_RIGHT_MARGIN;
			creatChessBoard(rows, columns);
			initgraph(width, height, EW_DBLCLKS);
			setbkcolor(RGB(230, 236, 202));
			cleardevice();
		}
		//��ӡ��Ϸ����
		//��ʾ��ʱ��
		//��Ϸ����
		condition = playgame(rows, columns, &safe, all, condition, &mines);
		if ((condition == 0 || condition == 1 || condition == 2)) {
			printBoard(height, width, rows, columns, condition, mines, &over);
			timeCount(width);//ÿ�����ж�����ʱ��
		}
	}
	return 0;
}

