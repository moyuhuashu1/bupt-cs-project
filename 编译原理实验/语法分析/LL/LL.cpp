#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_LEN 2048

//LL�������������ֱ�ʾ��Ӧ�Ĳ���ʽ��0��ʾerror
int ll_action[5][8] = {
    {1,0,0,0,0,0,1,0},
    {0,4,0,2,3,0,0,4},
    {5,0,0,0,0,0,5,0},
    {0,8,7,8,8,6,0,8},
    {9,0,0,0,0,0,10,0}
};
//����ջ
char stack[BUFFER_LEN] = { 0 };
int top = 0//ջ��;

void push(char i) {
    stack[top++] = i;
}


int pop() {
    top--;
    return 0;
}

//ѡ�����ʽ
int SelctExp(char topChar, char inputChar) {
    char end[8] = { '(',  ')', '/' , '+' ,  '-',  '*' ,'n'	,'\0' };
    char noend[5] = { 'E', 'A', 'T', 'B', 'F' };
    int i = 0, j = 0;
    for (; i < 5 && topChar != noend[i]; i++);
    for (; j < 8 && inputChar != end[j]; j++);
    return ll_action[i][j];
}

//����Ӧ���Ҳ���ʽ������ջ
void pusharr(int exp) {
    char pushed[4];
    switch (exp) {
    case 1:strcpy(pushed, "TA\0"); break;//E->TA
    case 2:strcpy(pushed , "+TA\0"); break;//E->+TA
    case 3:strcpy(pushed , "-TA\0"); break;//E->-TA
    case 4:strcpy(pushed , "\0"); break;//A->��
    case 5:strcpy(pushed , "FB\0"); break;//T->FB
    case 6:strcpy(pushed ,"*FB\0"); break;//T->*FB
    case 7:strcpy(pushed , "/FB\0"); break;//T->/FB
    case 8:strcpy(pushed , "\0"); break;//T->��
    case 9:strcpy(pushed , "(E)\0"); break;//F->(E)
    case 10:strcpy(pushed , "n\0"); break;//F->num  
    default:strcpy(pushed , "\0");
    }
    pop();
    for (int i = strlen(pushed) - 1; i != -1; i--)
        push(pushed[i]);
}


//��ӡ����ջ
void pintfAnaStack() {
    for (int i = 0; i < top; i++) {
        //\0��ʾ$
        if (stack[i] == '\0') {
            printf("$");
        }
        else {
            printf("%c", stack[i]);
        }
    }
    printf("\t");
}

//��ӡ���봮
void pintInptStack(char* input, int now_w) {
    printf("%s$\t", &input[now_w]);
}

//�ж�ջ�������Ƿ����ս����
int isEndSymbol(char a) {
    //\0��ʾ$
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
    //����ʵ�����봮�Ľ�����Ϊ��\0������ʵ�鶼�ԡ�\0����ʾ��$��
    push('\0');
    //ѹ�뿪ʼ����
    push('E');
    int now_w = 0;
    do {       
        char inputChar = w[now_w];//��ǰ�������
        char topChar = stack[top - 1];//��ǰջ������
        pintfAnaStack();//��ӡ����ջ
        pintInptStack(w,now_w);//��ӡ���봮
        //���ջ���������ս����
        if (isEndSymbol(topChar) == 1) {
            if (topChar == inputChar) {
                //ջ�����ź͵�ǰ������Ŷ���$�������ɹ�
                if (inputChar == '\0') {
                    printf("accept\n");
                    break;
                }
                else {
                    //��ջ������������ָ��ǰ��һ��λ��
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
        //���ջ�������Ƿ��ս��
        else {
            int exp = SelctExp(topChar, inputChar);
            if (exp == 0) {//���ҵ��������пյ�λ�ã�0��,˵������������
                printf("error\n");
                break;
            }
            else {
                //���ݶ�Ӧ�Ĳ���ʽ������ջ
                pusharr(exp);
                printf("%d\n", exp);
            }
        }
    } while (1);
    return 0;
}


int main(void) {
    char w[BUFFER_LEN];//�洢��Ҫ�������ַ���
    scanf("%s", &w);
    ll(w);
    system("Pause");
    return 0;
}



