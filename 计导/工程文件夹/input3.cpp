#include "myLib1.h"

extern char** chessBoard;
extern char** originalBoard;
extern const int TOP_BOTTOM_MARGIN;
extern const int LEFT_RIGHT_MARGIN;
extern const int BLOCK_SIZE;
extern const int EMOJI_SIZE;
extern const int NUM_WIDTH;
extern const int NUM_HEIGHT;

//选择模式的页面
int chooseMode(int* rowsPtr, int* columnsPtr, int* minesPtr) {
	int condition = 4;
	//初始化绘图窗口
	initgraph(340, 430, EW_DBLCLKS);
	setbkcolor(RGB(230, 236, 202));
	cleardevice();
	//如果处于未选择状态（condition等于4），则一直处于选择模式的界面
	for (; condition == 4;)
	{
		//选择模式界面布局
		settextcolor(BLACK);
		setfillcolor(BLUE);
		rectangle(80, 30, 260, 100);
		outtextxy(155, 60, "初级");
		rectangle(80, 130, 260, 200);
		outtextxy(155, 160, "中级");
		rectangle(80, 230, 260, 300);
		outtextxy(155, 260, "高级");
		rectangle(80, 330, 260, 400);
		outtextxy(147.5, 360, "自定义");
		//进行选择模式内部操作（选择成功则改变choice以跳出选择模式界面）
		condition = chooseMode_Opra(rowsPtr, columnsPtr, minesPtr);
	}
	//选择完毕就关掉窗口
	closegraph();
	return condition;
}

//选择模式的内部操作
int chooseMode_Opra(int* rowsPtr, int* columnsPtr, int* minesPtr) {
	//保存鼠标信息的结构体
	ExMessage mouse;
	//获取鼠标信息
	mouse = getmessage(EX_MOUSE);
	//获取鼠标点击的x，y坐标
	int x = mouse.x, y = mouse.y, condition = 4;
	//如果获取到鼠标左键按下的信息
	if (mouse.message == WM_LBUTTONDOWN) {
		//判断左键x坐标在可选择模式界面按钮区域
		if (x >= 80 && x <= 260) {
			//点击在“初级”区域
			if (y >= 30 && y <= 100) {
				*rowsPtr = 9;              //设置棋盘行数、列数、总雷数,改变condition值使选择结束（中级，高级同）
				*columnsPtr = 9;
				*minesPtr = 10;
				condition = 3;
			}
			//点击在“中级”区域
			if (y >= 130 && y <= 200) {
				*rowsPtr = 16;
				*columnsPtr = 16;
				*minesPtr = 40;
				condition = 3;
			}
			//点击在“高级”区域
			if (y >= 230 && y <= 300) {
				*rowsPtr = 16;
				*columnsPtr = 30;
				*minesPtr = 99;
				condition = 3;
			}
			//点击在“自定义”区域
			if (y >= 330 && y <= 400) {
				int i, j, k, judge;
				//input数组储存输入对话框的字符串，record数组储存从input数组中获取的有效字符串（行数，列数，雷数）
				wchar_t  record[3][6];
				char input[30];
				//初始化两个字符串
				input[0] = '\0', record[0][0] = '\0';
				//以对话框形式获取用户输入
				InputBox(input, 30, "请输入高度（9-24）、宽度（9-30）、雷数（10-648）[以空格分隔]", "自定义", NULL, 250, 2, false);
				for (i = 0, j = 0, k = 0, judge = 0; j < 3 && input[i] != '\0'; i++) {	//循环条件为存完record的，或者读到input数组结束
					//如果读到input数组是数字就往record数组中存
					if (input[i] >= '0' && input[i] <= '9') {
						record[j][k] = input[i];
						k++;
						//judge等于一说明input上一个读到的还是数字
						judge = 1;
					}
					else {
						//当input读到的不为数字且上一个读到的还是数字
						if (judge == 1) {
							//结束当前行record的储存
							record[j][k] = '\0';
							//换到record的下一行
							j++;
							//record的下一行从头开始
							k = 0;
							//judge等于0说明input上一个读的已不是数字，除非再读到数字否则不用再往record里存
							judge = 0;
						}
					}
				}
				//将record中存的代表高度、宽度、雷数的字符串一次转换为整型
				*rowsPtr = _wtoi(record[0]);
				*columnsPtr = _wtoi(record[1]);
				*minesPtr = _wtoi(record[2]);
				//判断 如果获取到的高度、宽度、雷数合法，才说明选择成功，否则选择失败仍在选择模式页面
				if (*rowsPtr >= 9 && *rowsPtr <= 24 && *columnsPtr >= 9 && *columnsPtr <= 30 && *minesPtr >= 10 && *minesPtr <= (*rowsPtr) * (*columnsPtr) * 0.9) {
					condition = 3;
				}
			}
		}
	}
	return condition;
}
