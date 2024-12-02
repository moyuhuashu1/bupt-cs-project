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


//初始化加载图片
void init_IMG() {
	//按照图片按照其格子的尺寸存进格子的数组
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
	loadimage(&num[10], "img/负号.png", NUM_WIDTH, NUM_HEIGHT);
	loadimage(&num[11], "img/空.png", NUM_WIDTH, NUM_HEIGHT);
	loadimage(&emoji[0], "img/emoji1.jpg", EMOJI_SIZE, EMOJI_SIZE);
	loadimage(&emoji[1], "img/emoji2.jpg", EMOJI_SIZE, EMOJI_SIZE);
	loadimage(&emoji[2], "img/emoji3.jpg", EMOJI_SIZE, EMOJI_SIZE);
}

//显示游戏界面
void printBoard(int height, int  width, int rows, int columns, int condition, int mines, int* overPtr) {
	int i = 0, j = 0;//计算剩余雷数的个位，十位，个位
	//在顶上写个return
	settextcolor(DARKGRAY);
	outtextxy(width / 2 - 35, TOP_BOTTOM_MARGIN / 10, "重选等级");
	//显示剩余雷数
	printMinesCount(mines);

	//根据游戏状态和棋盘状态放置对应棋盘砖块图片（condition为0：游戏失败；为1：游戏继续；为2：游戏胜利）
	printChessBoard(rows, columns, condition);
	//显示游戏状态
	printCondition(height, width, condition, overPtr);
	//表情图右边以蓝色文本提示“点此重开”
	settextcolor(GREEN);
	outtextxy((width + EMOJI_SIZE) / 2 + 10, TOP_BOTTOM_MARGIN - EMOJI_SIZE / 2 - 3, "<--点我重开");

}

//显示剩余雷数
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
		//在左上角根据剩余雷数的百位，十位，各位依次贴代表相应数字的图片
		putimage(LEFT_RIGHT_MARGIN, TOP_BOTTOM_MARGIN - NUM_HEIGHT - 8, &num[mines100]);
		putimage(LEFT_RIGHT_MARGIN + NUM_WIDTH, TOP_BOTTOM_MARGIN - NUM_HEIGHT - 8, &num[mines10]);
		putimage(LEFT_RIGHT_MARGIN + 2 * NUM_WIDTH, TOP_BOTTOM_MARGIN - NUM_HEIGHT - 8, &num[mines1]);
	}
}

//显示棋盘
void printChessBoard(int rows, int columns, int condition) {
	int i, j;
	for (i = 0; i < rows; i++) {
		for (j = 0; j < columns; j++) {
			//如果游戏未结束则操作棋盘为雷的地方也显示为未翻开
			if (condition == 1 && chessBoard[i][j] == '*') {
				//图片x坐标为左边界宽度+在棋盘的列数*棋盘贴图尺寸，y坐标为上边界宽度+在棋盘的行数*棋盘贴图尺寸（贴图坐标都如此，后不做解释）
				putimage(j * BLOCK_SIZE + LEFT_RIGHT_MARGIN, i * BLOCK_SIZE + TOP_BOTTOM_MARGIN, &block[9]);
			}
			//如果游戏结束则所有雷都要显示
			else if ((condition == 0 || condition == 2) && originalBoard[i][j] == '*') {

				//如果是左键点开的雷显示红雷（操作棋盘中用@表示）
				if (chessBoard[i][j] == '@')/*红雷*/ {
					putimage(j * BLOCK_SIZE + LEFT_RIGHT_MARGIN, i * BLOCK_SIZE + TOP_BOTTOM_MARGIN, &block[11]);
				}
				//如果是插旗错误双击开错雷，则没被标到的雷以打叉的雷显示（操作棋盘中用&表示）
				else if (chessBoard[i][j] == '&')/*叉雷*/ {
					putimage(j * BLOCK_SIZE + LEFT_RIGHT_MARGIN, i * BLOCK_SIZE + TOP_BOTTOM_MARGIN, &block[12]);
				}
				//其余的雷都用普通雷显示
				else {
					putimage(j * BLOCK_SIZE + LEFT_RIGHT_MARGIN, i * BLOCK_SIZE + TOP_BOTTOM_MARGIN, &block[10]);
				}
			}
			//其余该以什么图显示用什么图显示
			else {
				//操作棋盘中的“ . ”表示为点开的格子
				if (chessBoard[i][j] == '.') {
					putimage(j * BLOCK_SIZE + LEFT_RIGHT_MARGIN, i * BLOCK_SIZE + TOP_BOTTOM_MARGIN, &block[9]);
				}
				if (chessBoard[i][j] == '!') {
					putimage(j * BLOCK_SIZE + LEFT_RIGHT_MARGIN, i * BLOCK_SIZE + TOP_BOTTOM_MARGIN, &block[13]);
				}
				if (chessBoard[i][j] == '?') {
					putimage(j * BLOCK_SIZE + LEFT_RIGHT_MARGIN, i * BLOCK_SIZE + TOP_BOTTOM_MARGIN, &block[14]);
				}
				//棋盘数字按block数组顺序存的，所以周围雷数多少对应的数组顺序就是该数字的图片
				if ((chessBoard[i][j] - '0' >= 0) && (chessBoard[i][j] - '0') <= 9) {
					putimage(j * BLOCK_SIZE + LEFT_RIGHT_MARGIN, i * BLOCK_SIZE + TOP_BOTTOM_MARGIN, &block[chessBoard[i][j] - '0']);
				}
			}
		}
	}
}

//显示游戏状态
void printCondition(int height, int  width, int condition, int* overPtr) {
	//如果游戏失败，上方显示哭哭图，下方文本提示游戏失败
	if (condition == 0) {
		putimage((width - EMOJI_SIZE) / 2, TOP_BOTTOM_MARGIN - EMOJI_SIZE - 3, &emoji[1]);
		settextcolor(BLACK);
		outtextxy(width / 2 - 70, height - 50, "Hit mine, you lose");
		if (*overPtr == 1) {
			mciSendString("open  music/雷爆炸游戏失败.mp3 alias 炸", NULL, 0, NULL);
			mciSendString("play 炸", NULL, 0, NULL);
			MessageBox(GetHWnd(), "输咯  ,  可惜", "提示", MB_OK);
			*overPtr = 0;
		}
	}
	//如果游戏继续，上方显示加油图，下方文本提示游戏继续
	if (condition == 1) {
		putimage((width - EMOJI_SIZE) / 2, TOP_BOTTOM_MARGIN - EMOJI_SIZE - 3, &emoji[0]);
		settextcolor(BLACK);
		outtextxy(width / 2 - 70, height - 50, "Game in progress");
		if (first ==0) {
			endTime = clock();
		}
	}
	//如果游戏胜利，上方显示高兴图，下方文本提示游戏胜利
	if (condition == 2) {
		putimage((width - EMOJI_SIZE) / 2, TOP_BOTTOM_MARGIN - EMOJI_SIZE - 3, &emoji[2]);
		settextcolor(BLACK);
		outtextxy(width / 2 - 70, height - 50, "Game over, you win");
		if (*overPtr == 1) {
			mciSendString("open  music/游戏胜利.mp3 alias 赢", NULL, 0, NULL);
			mciSendString("play 赢", NULL, 0, NULL);
			MessageBox(GetHWnd(), "赢咯  ,  恭喜", "提示", MB_OK);
			*overPtr = 0;
		}
	}
}
