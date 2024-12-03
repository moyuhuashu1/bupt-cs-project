#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_LEN 2048

//LL分析表，表中数字表示对应的产生式，0表示error
int ll_action[5][8] = {
    {1,0,0,0,0,0,1,0},
    {0,4,0,2,3,0,0,4},
    {5,0,0,0,0,0,5,0},
    {0,8,7,8,8,6,0,8},
    {9,0,0,0,0,0,10,0}
};
//分析栈
char stack[BUFFER_LEN] = { 0 };
int top = 0//栈顶;

void push(char i) {
    stack[top++] = i;
}


int pop() {
    top--;
    return 0;
}

//选择产生式
int SelctExp(char topChar, char inputChar) {
    char end[8] = { '(',  ')', '/' , '+' ,  '-',  '*' ,'n'	,'\0' };
    char noend[5] = { 'E', 'A', 'T', 'B', 'F' };
    int i = 0, j = 0;
    for (; i < 5 && topChar != noend[i]; i++);
    for (; j < 8 && inputChar != end[j]; j++);
    return ll_action[i][j];
}

//将对应的右产生式反序入栈
void pusharr(int exp) {
    char pushed[4];
    switch (exp) {
    case 1:strcpy(pushed, "TA\0"); break;//E->TA
    case 2:strcpy(pushed , "+TA\0"); break;//E->+TA
    case 3:strcpy(pushed , "-TA\0"); break;//E->-TA
    case 4:strcpy(pushed , "\0"); break;//A->ε
    case 5:strcpy(pushed , "FB\0"); break;//T->FB
    case 6:strcpy(pushed ,"*FB\0"); break;//T->*FB
    case 7:strcpy(pushed , "/FB\0"); break;//T->/FB
    case 8:strcpy(pushed , "\0"); break;//T->ε
    case 9:strcpy(pushed , "(E)\0"); break;//F->(E)
    case 10:strcpy(pushed , "n\0"); break;//F->num  
    default:strcpy(pushed , "\0");
    }
    pop();
    for (int i = strlen(pushed) - 1; i != -1; i--)
        push(pushed[i]);
}


//打印分析栈
void pintfAnaStack() {
    for (int i = 0; i < top; i++) {
        //\0表示$
        if (stack[i] == '\0') {
            printf("$");
        }
        else {
            printf("%c", stack[i]);
        }
    }
    printf("\t");
}

//打印输入串
void pintInptStack(char* input, int now_w) {
    printf("%s$\t", &input[now_w]);
}

//判断栈顶符号是否是终结符号
int isEndSymbol(char a) {
    //\0表示$
    char end[8] = { '(',  ')',  '/' , '+' ,  '-',  '*' ,'n'	,'\0' };
    char noend[5] = { 'E', 'A', 'T', 'B', 'F' };
    for (int i = 0; i < 8; i++) {
        if (a == end[i]) {
            return 1;
        }
    }
    return 0;
}


int ll(char* w) {
    //由于实际输入串的结束符为‘\0’，本实验都以‘\0’表示‘$’
    push('\0');
    //压入开始符号
    push('E');
    int now_w = 0;
    do {       
        char inputChar = w[now_w];//当前输入符号
        char topChar = stack[top - 1];//当前栈顶符号
        pintfAnaStack();//打印分析栈
        pintInptStack(w,now_w);//打印输入串
        //如果栈顶符号是终结符号
        if (isEndSymbol(topChar) == 1) {
            if (topChar == inputChar) {
                //栈顶符号和当前输入符号都是$，分析成功
                if (inputChar == '\0') {
                    printf("accept\n");
                    break;
                }
                else {
                    //从栈顶弹出，输入指针前移一个位置
                    printf("match\n");
                    pop();
                    now_w++;
                }
            }
            else {
                printf("error");
                break;
            }
        }
        //如果栈顶符号是非终结符
        else {
            int exp = SelctExp(topChar, inputChar);
            if (exp == 0) {//查找到分析表中空的位置（0）,说明分析出错了
                printf("error\n");
                break;
            }
            else {
                //根据对应的产生式反序入栈
                pusharr(exp);
                printf("%d\n", exp);
            }
        }
    } while (1);
    return 0;
}


int main(void) {
    char w[BUFFER_LEN];//存储需要分析的字符串
    scanf("%s", &w);
    ll(w);
    system("Pause");
    return 0;
}



