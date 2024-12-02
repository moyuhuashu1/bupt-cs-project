#include "myLib1.h"

extern char** chessBoard;
extern char** originalBoard;
extern clock_t endTime;
extern int first;
extern const int TOP_BOTTOM_MARGIN;
extern const int LEFT_RIGHT_MARGIN;
extern const int BLOCK_SIZE;
extern const int EMOJI_SIZE;
extern const int NUM_WIDTH;
extern const int NUM_HEIGHT;
extern IMAGE  block[15], num[12], emoji[3];


//��ʼ������ͼƬ
void init_IMG() {
	//����ͼƬ��������ӵĳߴ������ӵ�����
	loadimage(&block[0], "img/0.jpg", BLOCK_SIZE, BLOCK_SIZE);
	loadimage(&block[1], "img/1.jpg", BLOCK_SIZE, BLOCK_SIZE);
	loadimage(&block[2], "img/2.jpg", BLOCK_SIZE, BLOCK_SIZE);
	loadimage(&block[3], "img/3.jpg", BLOCK_SIZE, BLOCK_SIZE);
	loadimage(&block[4], "img/4.jpg", BLOCK_SIZE, BLOCK_SIZE);
	loadimage(&block[5], "img/5.jpg", BLOCK_SIZE, BLOCK_SIZE);
	loadimage(&block[6], "img/6.jpg", BLOCK_SIZE, BLOCK_SIZE);
	loadimage(&block[7], "img/7.jpg", BLOCK_SIZE, BLOCK_SIZE);
	loadimage(&block[8], "img/8.jpg", BLOCK_SIZE, BLOCK_SIZE);
	loadimage(&block[9], "img/block.jpg", BLOCK_SIZE, BLOCK_SIZE);
	loadimage(&block[10], "img/bomb.jpg", BLOCK_SIZE, BLOCK_SIZE);
	loadimage(&block[11], "img/red-bomb.jpg", BLOCK_SIZE, BLOCK_SIZE);
	loadimage(&block[12], "img/mark-bom.png", BLOCK_SIZE, BLOCK_SIZE);
	loadimage(&block[13], "img/flag.jpg", BLOCK_SIZE, BLOCK_SIZE);
	loadimage(&block[14], "img/question.jpg", BLOCK_SIZE, BLOCK_SIZE);
	loadimage(&num[0], "img/00.png", NUM_WIDTH, NUM_HEIGHT);
	loadimage(&num[1], "img/11.png", NUM_WIDTH, NUM_HEIGHT);
	loadimage(&num[2], "img/22.png", NUM_WIDTH, NUM_HEIGHT);
	loadimage(&num[3], "img/33.png", NUM_WIDTH, NUM_HEIGHT);
	loadimage(&num[4], "img/44.png", NUM_WIDTH, NUM_HEIGHT);
	loadimage(&num[5], "img/55.png", NUM_WIDTH, NUM_HEIGHT);
	loadimage(&num[6], "img/66.png", NUM_WIDTH, NUM_HEIGHT);
	loadimage(&num[7], "img/77.png", NUM_WIDTH, NUM_HEIGHT);
	loadimage(&num[8], "img/88.png", NUM_WIDTH, NUM_HEIGHT);
	loadimage(&num[9], "img/99.png", NUM_WIDTH, NUM_HEIGHT);
	loadimage(&num[10], "img/����.png", NUM_WIDTH, NUM_HEIGHT);
	loadimage(&num[11], "img/��.png", NUM_WIDTH, NUM_HEIGHT);
	loadimage(&emoji[0], "img/emoji1.jpg", EMOJI_SIZE, EMOJI_SIZE);
	loadimage(&emoji[1], "img/emoji2.jpg", EMOJI_SIZE, EMOJI_SIZE);
	loadimage(&emoji[2], "img/emoji3.jpg", EMOJI_SIZE, EMOJI_SIZE);
}

//��ʾ��Ϸ����
void printBoard(int height, int  width, int rows, int columns, int condition, int mines, int* overPtr) {
	int i = 0, j = 0;//����ʣ�������ĸ�λ��ʮλ����λ
	//�ڶ���д��return
	settextcolor(DARKGRAY);
	outtextxy(width / 2 - 35, TOP_BOTTOM_MARGIN / 10, "��ѡ�ȼ�");
	//��ʾʣ������
	printMinesCount(mines);

	//������Ϸ״̬������״̬���ö�Ӧ����ש��ͼƬ��conditionΪ0����Ϸʧ�ܣ�Ϊ1����Ϸ������Ϊ2����Ϸʤ����
	printChessBoard(rows, columns, condition);
	//��ʾ��Ϸ״̬
	printCondition(height, width, condition, overPtr);
	//����ͼ�ұ�����ɫ�ı���ʾ������ؿ���
	settextcolor(GREEN);
	outtextxy((width + EMOJI_SIZE) / 2 + 10, TOP_BOTTOM_MARGIN - EMOJI_SIZE / 2 - 3, "<--�����ؿ�");

}

//��ʾʣ������
void printMinesCount(int mines) {
	int mines1 = mines % 10, mines10 = mines / 10 % 10, mines100 = mines / 100;
	if (mines <= -99) {
		putimage(LEFT_RIGHT_MARGIN, TOP_BOTTOM_MARGIN - NUM_HEIGHT - 8, &num[10]);
		putimage(LEFT_RIGHT_MARGIN + NUM_WIDTH, TOP_BOTTOM_MARGIN - NUM_HEIGHT - 8, &num[9]);
		putimage(LEFT_RIGHT_MARGIN + 2 * NUM_WIDTH, TOP_BOTTOM_MARGIN - NUM_HEIGHT - 8, &num[9]);
	}
	else if (mines > -99 && mines <= -10) {
		mines1 = (-mines) % 10;
		mines10 = (-mines) / 10 % 10;
		putimage(LEFT_RIGHT_MARGIN, TOP_BOTTOM_MARGIN - NUM_HEIGHT - 8, &num[10]);
		putimage(LEFT_RIGHT_MARGIN + NUM_WIDTH, TOP_BOTTOM_MARGIN - NUM_HEIGHT - 8, &num[mines10]);
		putimage(LEFT_RIGHT_MARGIN + 2 * NUM_WIDTH, TOP_BOTTOM_MARGIN - NUM_HEIGHT - 8, &num[mines1]);
	}
	else if (mines > -10 && mines < 0) {
		mines1 = (-mines) % 10;
		putimage(LEFT_RIGHT_MARGIN, TOP_BOTTOM_MARGIN - NUM_HEIGHT - 8, &num[11]);
		putimage(LEFT_RIGHT_MARGIN + NUM_WIDTH, TOP_BOTTOM_MARGIN - NUM_HEIGHT - 8, &num[10]);
		putimage(LEFT_RIGHT_MARGIN + 2 * NUM_WIDTH, TOP_BOTTOM_MARGIN - NUM_HEIGHT - 8, &num[mines1]);
	}
	else
	{
		//�����ϽǸ���ʣ�������İ�λ��ʮλ����λ������������Ӧ���ֵ�ͼƬ
		putimage(LEFT_RIGHT_MARGIN, TOP_BOTTOM_MARGIN - NUM_HEIGHT - 8, &num[mines100]);
		putimage(LEFT_RIGHT_MARGIN + NUM_WIDTH, TOP_BOTTOM_MARGIN - NUM_HEIGHT - 8, &num[mines10]);
		putimage(LEFT_RIGHT_MARGIN + 2 * NUM_WIDTH, TOP_BOTTOM_MARGIN - NUM_HEIGHT - 8, &num[mines1]);
	}
}

//��ʾ����
void printChessBoard(int rows, int columns, int condition) {
	int i, j;
	for (i = 0; i < rows; i++) {
		for (j = 0; j < columns; j++) {
			//�����Ϸδ�������������Ϊ�׵ĵط�Ҳ��ʾΪδ����
			if (condition == 1 && chessBoard[i][j] == '*') {
				//ͼƬx����Ϊ��߽���+�����̵�����*������ͼ�ߴ磬y����Ϊ�ϱ߽���+�����̵�����*������ͼ�ߴ磨��ͼ���궼��ˣ��������ͣ�
				putimage(j * BLOCK_SIZE + LEFT_RIGHT_MARGIN, i * BLOCK_SIZE + TOP_BOTTOM_MARGIN, &block[9]);
			}
			//�����Ϸ�����������׶�Ҫ��ʾ
			else if ((condition == 0 || condition == 2) && originalBoard[i][j] == '*') {

				//���������㿪������ʾ���ף�������������@��ʾ��
				if (chessBoard[i][j] == '@')/*����*/ {
					putimage(j * BLOCK_SIZE + LEFT_RIGHT_MARGIN, i * BLOCK_SIZE + TOP_BOTTOM_MARGIN, &block[11]);
				}
				//����ǲ������˫�������ף���û���굽�����Դ�������ʾ��������������&��ʾ��
				else if (chessBoard[i][j] == '&')/*����*/ {
					putimage(j * BLOCK_SIZE + LEFT_RIGHT_MARGIN, i * BLOCK_SIZE + TOP_BOTTOM_MARGIN, &block[12]);
				}
				//������׶�����ͨ����ʾ
				else {
					putimage(j * BLOCK_SIZE + LEFT_RIGHT_MARGIN, i * BLOCK_SIZE + TOP_BOTTOM_MARGIN, &block[10]);
				}
			}
			//�������ʲôͼ��ʾ��ʲôͼ��ʾ
			else {
				//���������еġ� . ����ʾΪ�㿪�ĸ���
				if (chessBoard[i][j] == '.') {
					putimage(j * BLOCK_SIZE + LEFT_RIGHT_MARGIN, i * BLOCK_SIZE + TOP_BOTTOM_MARGIN, &block[9]);
				}
				if (chessBoard[i][j] == '!') {
					putimage(j * BLOCK_SIZE + LEFT_RIGHT_MARGIN, i * BLOCK_SIZE + TOP_BOTTOM_MARGIN, &block[13]);
				}
				if (chessBoard[i][j] == '?') {
					putimage(j * BLOCK_SIZE + LEFT_RIGHT_MARGIN, i * BLOCK_SIZE + TOP_BOTTOM_MARGIN, &block[14]);
				}
				//�������ְ�block����˳���ģ�������Χ�������ٶ�Ӧ������˳����Ǹ����ֵ�ͼƬ
				if ((chessBoard[i][j] - '0' >= 0) && (chessBoard[i][j] - '0') <= 9) {
					putimage(j * BLOCK_SIZE + LEFT_RIGHT_MARGIN, i * BLOCK_SIZE + TOP_BOTTOM_MARGIN, &block[chessBoard[i][j] - '0']);
				}
			}
		}
	}
}

//��ʾ��Ϸ״̬
void printCondition(int height, int  width, int condition, int* overPtr) {
	//�����Ϸʧ�ܣ��Ϸ���ʾ�޿�ͼ���·��ı���ʾ��Ϸʧ��
	if (condition == 0) {
		putimage((width - EMOJI_SIZE) / 2, TOP_BOTTOM_MARGIN - EMOJI_SIZE - 3, &emoji[1]);
		settextcolor(BLACK);
		outtextxy(width / 2 - 70, height - 50, "Hit mine, you lose");
		if (*overPtr == 1) {
			mciSendString("open  music/�ױ�ը��Ϸʧ��.mp3 alias ը", NULL, 0, NULL);
			mciSendString("play ը", NULL, 0, NULL);
			MessageBox(GetHWnd(), "�俩  ,  ��ϧ", "��ʾ", MB_OK);
			*overPtr = 0;
		}
	}
	//�����Ϸ�������Ϸ���ʾ����ͼ���·��ı���ʾ��Ϸ����
	if (condition == 1) {
		putimage((width - EMOJI_SIZE) / 2, TOP_BOTTOM_MARGIN - EMOJI_SIZE - 3, &emoji[0]);
		settextcolor(BLACK);
		outtextxy(width / 2 - 70, height - 50, "Game in progress");
		if (first ==0) {
			endTime = clock();
		}
	}
	//�����Ϸʤ�����Ϸ���ʾ����ͼ���·��ı���ʾ��Ϸʤ��
	if (condition == 2) {
		putimage((width - EMOJI_SIZE) / 2, TOP_BOTTOM_MARGIN - EMOJI_SIZE - 3, &emoji[2]);
		settextcolor(BLACK);
		outtextxy(width / 2 - 70, height - 50, "Game over, you win");
		if (*overPtr == 1) {
			mciSendString("open  music/��Ϸʤ��.mp3 alias Ӯ", NULL, 0, NULL);
			mciSendString("play Ӯ", NULL, 0, NULL);
			MessageBox(GetHWnd(), "Ӯ��  ,  ��ϲ", "��ʾ", MB_OK);
			*overPtr = 0;
		}
	}
}
