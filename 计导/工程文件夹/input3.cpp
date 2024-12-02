#include "myLib1.h"

extern char** chessBoard;
extern char** originalBoard;
extern const int TOP_BOTTOM_MARGIN;
extern const int LEFT_RIGHT_MARGIN;
extern const int BLOCK_SIZE;
extern const int EMOJI_SIZE;
extern const int NUM_WIDTH;
extern const int NUM_HEIGHT;

//ѡ��ģʽ��ҳ��
int chooseMode(int* rowsPtr, int* columnsPtr, int* minesPtr) {
	int condition = 4;
	//��ʼ����ͼ����
	initgraph(340, 430, EW_DBLCLKS);
	setbkcolor(RGB(230, 236, 202));
	cleardevice();
	//�������δѡ��״̬��condition����4������һֱ����ѡ��ģʽ�Ľ���
	for (; condition == 4;)
	{
		//ѡ��ģʽ���沼��
		settextcolor(BLACK);
		setfillcolor(BLUE);
		rectangle(80, 30, 260, 100);
		outtextxy(155, 60, "����");
		rectangle(80, 130, 260, 200);
		outtextxy(155, 160, "�м�");
		rectangle(80, 230, 260, 300);
		outtextxy(155, 260, "�߼�");
		rectangle(80, 330, 260, 400);
		outtextxy(147.5, 360, "�Զ���");
		//����ѡ��ģʽ�ڲ�������ѡ��ɹ���ı�choice������ѡ��ģʽ���棩
		condition = chooseMode_Opra(rowsPtr, columnsPtr, minesPtr);
	}
	//ѡ����Ͼ͹ص�����
	closegraph();
	return condition;
}

//ѡ��ģʽ���ڲ�����
int chooseMode_Opra(int* rowsPtr, int* columnsPtr, int* minesPtr) {
	//���������Ϣ�Ľṹ��
	ExMessage mouse;
	//��ȡ�����Ϣ
	mouse = getmessage(EX_MOUSE);
	//��ȡ�������x��y����
	int x = mouse.x, y = mouse.y, condition = 4;
	//�����ȡ�����������µ���Ϣ
	if (mouse.message == WM_LBUTTONDOWN) {
		//�ж����x�����ڿ�ѡ��ģʽ���水ť����
		if (x >= 80 && x <= 260) {
			//����ڡ�����������
			if (y >= 30 && y <= 100) {
				*rowsPtr = 9;              //��������������������������,�ı�conditionֵʹѡ��������м����߼�ͬ��
				*columnsPtr = 9;
				*minesPtr = 10;
				condition = 3;
			}
			//����ڡ��м�������
			if (y >= 130 && y <= 200) {
				*rowsPtr = 16;
				*columnsPtr = 16;
				*minesPtr = 40;
				condition = 3;
			}
			//����ڡ��߼�������
			if (y >= 230 && y <= 300) {
				*rowsPtr = 16;
				*columnsPtr = 30;
				*minesPtr = 99;
				condition = 3;
			}
			//����ڡ��Զ��塱����
			if (y >= 330 && y <= 400) {
				int i, j, k, judge;
				//input���鴢������Ի�����ַ�����record���鴢���input�����л�ȡ����Ч�ַ�����������������������
				wchar_t  record[3][6];
				char input[30];
				//��ʼ�������ַ���
				input[0] = '\0', record[0][0] = '\0';
				//�ԶԻ�����ʽ��ȡ�û�����
				InputBox(input, 30, "������߶ȣ�9-24������ȣ�9-30����������10-648��[�Կո�ָ�]", "�Զ���", NULL, 250, 2, false);
				for (i = 0, j = 0, k = 0, judge = 0; j < 3 && input[i] != '\0'; i++) {	//ѭ������Ϊ����record�ģ����߶���input�������
					//�������input���������־���record�����д�
					if (input[i] >= '0' && input[i] <= '9') {
						record[j][k] = input[i];
						k++;
						//judge����һ˵��input��һ�������Ļ�������
						judge = 1;
					}
					else {
						//��input�����Ĳ�Ϊ��������һ�������Ļ�������
						if (judge == 1) {
							//������ǰ��record�Ĵ���
							record[j][k] = '\0';
							//����record����һ��
							j++;
							//record����һ�д�ͷ��ʼ
							k = 0;
							//judge����0˵��input��һ�������Ѳ������֣������ٶ������ַ���������record���
							judge = 0;
						}
					}
				}
				//��record�д�Ĵ���߶ȡ���ȡ��������ַ���һ��ת��Ϊ����
				*rowsPtr = _wtoi(record[0]);
				*columnsPtr = _wtoi(record[1]);
				*minesPtr = _wtoi(record[2]);
				//�ж� �����ȡ���ĸ߶ȡ���ȡ������Ϸ�����˵��ѡ��ɹ�������ѡ��ʧ������ѡ��ģʽҳ��
				if (*rowsPtr >= 9 && *rowsPtr <= 24 && *columnsPtr >= 9 && *columnsPtr <= 30 && *minesPtr >= 10 && *minesPtr <= (*rowsPtr) * (*columnsPtr) * 0.9) {
					condition = 3;
				}
			}
		}
	}
	return condition;
}
