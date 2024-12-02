#include "myLib1.h"

//创建两个全局变量二维字符数组作为棋盘
char** chessBoard;	//操作棋盘
char** originalBoard;	//初始棋盘	
int click;
int first;
int leftDown = 0, rightDown = 0;
//图像对象 用数组分别储存棋盘图片，计数数字图片，表情图片 
IMAGE  block[15], num[12], emoji[3];
clock_t startTime;//设置初始的时间为0
clock_t endTime;
/*———————全局常量说明—————————*/
//全局常量 页面上下边框的长度
extern const int TOP_BOTTOM_MARGIN = 75;
//全局常量 页面左右边框的长度
extern const int LEFT_RIGHT_MARGIN = 20;
//全局常量 每块棋盘贴图的尺寸
extern const int BLOCK_SIZE = 30;
//全局常量 表情图的尺寸
extern const int EMOJI_SIZE = 45;
//全局常量 计数图的宽度
extern const int NUM_WIDTH = 30;
//全局常量 计数图的高度
extern const int NUM_HEIGHT = 40;

int main(void) {
	//first代表第一次操作是否执行（0：已执行，1：未执行），mines代表剩余雷数，allMines代表雷数
	//condition为0：游戏失败；为1：游戏继续；为2：游戏胜利；为3：更新数据状态；为4：未选择状态
	//rows:棋盘行数，columns：棋盘列数，height：游戏界面高度，width：游戏界面宽度
	int rows, columns, mines, height, width, allMInes, condition, safe, all, over;
	//加载图片只需要进行一次
	init_IMG();
	//得到更新数据状态
	condition = chooseMode(&rows, &columns, &mines);
	allMInes = mines;
	//游戏界面高度等于棋盘行数*每张贴图的尺寸+上下边界
	height = rows * BLOCK_SIZE + 2 * TOP_BOTTOM_MARGIN;
	//游戏界面宽度等于棋盘列数*每张贴图的尺寸+左右边界
	width = columns * BLOCK_SIZE + 2 * LEFT_RIGHT_MARGIN;
	//初始化生成游戏界面
	creatChessBoard(rows, columns);
	initgraph(width, height, EW_DBLCLKS);
	setbkcolor(RGB(230, 236, 202));
	cleardevice();
	/*第一次生成游戏界面是便于以后每次return出去再生成游戏界面时可以先free掉上次分配给棋盘的内存*/
	while (true) {
		//更新数据状态
		if (condition == 3) {
			//重置棋盘和相关数据，重置第一次点击状态，进入游戏状态
			updateChessBoard(rows, columns, allMInes);
			safe = 0;
			all = rows * columns - allMInes;
			mines = allMInes;
			condition = 1;
			first = 1;
			over = 1;
			endTime = 0;
			startTime = 0;
			mciSendString("close 炸", NULL, 0, NULL);
			mciSendString("close 赢", NULL, 0, NULL);
			mciSendString("open  music/生成初始游戏界面.mp3 alias 初", NULL, 0, NULL);
			mciSendString("play 初", NULL, 0, NULL);
			mciSendString("close 旗", NULL, 0, NULL);
			mciSendString("close 问", NULL, 0, NULL);
			mciSendString("close 取", NULL, 0, NULL);
			mciSendString("close 单", NULL, 0, NULL);
			mciSendString("close 多", NULL, 0, NULL);
			mciSendString("open  music/标记旗子.mp3 alias 旗", NULL, 0, NULL);
			mciSendString("open  music/标记问号.mp3 alias 问", NULL, 0, NULL);
			mciSendString("open  music/取消标记.mp3 alias 取", NULL, 0, NULL);
			mciSendString("open  music/左键单击只打开一个格子.mp3 alias 单", NULL, 0, NULL);
			mciSendString("open  music/左键单击递归打开.mp3 alias 多", NULL, 0, NULL);
		}
		//未选择状态
		if (condition == 4) {
			//关掉游戏界面窗口并释放棋盘数组的内存
			closegraph();
			freeChessBoard(rows);
			//进入更新数据状态，并重新绘制游戏界面
			condition = chooseMode(&rows, &columns, &mines);
			allMInes = mines;
			height = rows * BLOCK_SIZE + 2 * TOP_BOTTOM_MARGIN;
			width = columns * BLOCK_SIZE + 2 * LEFT_RIGHT_MARGIN;
			creatChessBoard(rows, columns);
			initgraph(width, height, EW_DBLCLKS);
			setbkcolor(RGB(230, 236, 202));
			cleardevice();
		}
		//打印游戏界面
		//显示计时器
		//游戏操作
		condition = playgame(rows, columns, &safe, all, condition, &mines);
		if ((condition == 0 || condition == 1 || condition == 2)) {
			printBoard(height, width, rows, columns, condition, mines, &over);
			timeCount(width);//每次运行都更新时间
		}
	}
	return 0;
}

