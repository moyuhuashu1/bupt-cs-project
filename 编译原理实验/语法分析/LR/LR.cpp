#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_LEN 2048
typedef struct states_stack {
    int State;
    struct states_stack* next;
}States_stack;

int lr(const char w[]);
States_stack* push(States_stack* stack, int a);
States_stack* pop(States_stack* stack);
int for_line(char a);
int pop_len(int a);
int reduce(int a);

int main()
{
    char w[BUFFER_LEN];//存储需要分析的字符串
    scanf("%s", &w);
    lr(w);
    return 0;
}

int lr(const char w[])
{
    //    保存分析表，acc用100代替，正数为状态数，负数为规约产生式号数
    int lr_action[30][11] = { 
                           {0,0,0,0,4,0,5,0,1,2,3},
                           {6,7,0,0,0,0,0,100,0,0,0},
                           {-3,-3,8,9,0,0,0,-3,0,0,0},
                           {-6,-6,-6,-6,0,0,0,-6,0,0,0},
                           {0,0,0,0,13,0,14,0,10,11,12},
                           {-8,-8,-8,-8,0,0,0,-8,0,0,0},
                           {0,0,0,0,4,0,5,0,0,15    ,3},
                           {0,0,0,0,4,0,5,0,0,16,3},
                           {0,0,0,0,4,0,5,0,0,0,17},
                           {0,0,0,0,4,0,5,0,0,0,18},
                           {20,21,0,0,0,19,0,0,0,0,0},
                           {-3,-3,22,23,0,-3,0,0,0,0,0},
                           {-6,-6,-6,-6,0,-6,0,0,0,0,0},
                           {0,0,0,0,13,0,14,0,24,11,12},
                           {-8,-8,-8,-8,0,-8,0,0,0,0,0},
                           {-1,-1,8,9,0,0,0,-1,0,0,0} ,
                           {-2,-2,8,9,0,0,0,-2,0,0,0},
                           {-4,-4,-4,-4,0,0,0,-4,0,0,0},
                           {-5,-5,-5,-5,0,0,0,-5,0,0,0},
                           {-7,-7,-7,-7,0,0,0,-7,0,0,0},
                           {0,0,0,0,13,0,14,0,0,25,12},
                           {0,0,0,0,13,0,14,0,0,26,12},
                           {0,0,0,0,13,0,14,0,0,0,27},
                           {0,0,0,0,13,0,14,0,0,0,28},
                           {20,21,0,0,0,29,0,0,0,0,0},
                           {-1,-1,21,22,0,-1,0,0,0,0,0},
                           {-2,-2,21,22,0,-2,0,0,0,0,0},
                           {-4,-4,-4,-4,0,-4,0,0,0,0,0},
                           {-5,-5,-5,-5,0,-5,0,0,0,0,0},
                           {-7,-7,-7,-7,0,-7,0,0,0,0,0}
                    };
    //    初始状态入栈
    States_stack* states = (States_stack*)malloc(sizeof(States_stack));//创建状态栈
    states->next = NULL;
    states = push(states, 0);
    States_stack* symbol = (States_stack*)malloc(sizeof(States_stack));//创建符号栈
    symbol->next = NULL;
    symbol = push(symbol, 1);
    //    输入字符串w所在的位置(用字符串代替指针)
    int now_w = 0;
    do {
        int a = for_line(w[now_w]);//当前输入符号在分析表中第几列
        int now_state = lr_action[states->State][a];//结合当前栈顶状态查询分析表中对应下一步动作，得到下一步状态（移进/规约）
        //接受
        if (now_state == 100)
        {
            printf("accept\n");
            break;
        }
        //移进
        if (now_state > 0)
        {
            printf("shift\n");
            states = push(states, now_state);
            symbol = push(symbol, a);
            now_w++;
        }
        else
        {
            //规约
            if (now_state < 0)
            {
                printf("%d\n", -now_state);
                a = reduce(now_state);
                for (int i = 0; i < pop_len(now_state); i++)
                {
                    states = pop(states);
                    symbol = pop(symbol);
                }
                symbol = push(symbol, a);
                states = push(states, lr_action[states->State][a]);
            }
            else
            {
                printf("error");
                break;
            }
        }
    } while (1);
    system("Pause");
    return 0;
}

States_stack* push(States_stack* stack, int a) {
    States_stack* line = (States_stack*)malloc(sizeof(States_stack));
    line->State = a;
    line->next = (States_stack*)stack;
    stack = line;
    return stack;
}
States_stack* pop(States_stack* stack) {
    States_stack* p = stack;
    stack = (States_stack*)stack->next;
    free(p);
    return stack;
}

//判断字符的在分析表中的列数
int for_line(char a) {
    //实际输入字符串中的$为‘\0’
    char line[12] = { '+','-','*','/','(',')','n','\0','E','F','T','\0' };
    int r = 0;
    for (int i = 0; i < 12; i++)
    {
        if (line[i] == a)
        {
            r = i;
            break;
        }
    }
    return r;
}

int pop_len(int a) {
    //    判断要弹出多少个栈顶元素
    int r = 3;
    if (a == -3 || a == -6 || a == -8)
    {
        r = 1;
    }
    return r;
}

int reduce(int a) {
    int r;
    //返回规约后的左式符号的数组在分析表中的列数
    switch (a) {
    case -1://E->E+T
        r = 8; break;
    case -2://E->E-T
        r = 8;break;
    case -3://E->T
        r = 8; break;
    case -4://T->T*F
        r = 9;break;
    case -5://T->T/F
        r = 9;break;
    case -6://T->F
        r = 9;break;
    case -7://F->(E)
        r = 10;break;
    case -8://F->num
        r = 10;break;
    default:r = 0;break;
    }
    return r;
}
