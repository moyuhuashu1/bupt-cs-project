#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define KEYSIZE 32 //�ؼ�������
#define BUFFERSIZE 1000 //�����������С

//���ּǺ�  
#define KEYWORD 0
#define	IDENTIFIER 1
#define	OPERATOR 2
#define	DELIMITER 3
#define	CHARCON 4 
#define	STRING 5
#define	NUMBER 6
#define ERROR 7

int token_num[8]; //��Ÿ��ּǺŵ��ʵ�����

char keyTable[KEYSIZE][10] = { "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else",
                         "enum", "extern", "float", "for", "goto", "if", "int", "long", "register",
                          "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef",
                         "union", "unsigned", "void", "volatile","while"}; //��Źؼ����ַ�
int state; //��ǰ״ָ̬ʾ
char C; //��ŵ�ǰ������ַ�
int isKey; //ֵΪ-1��ʾʶ����ĵ������û��Զ���ı�ʶ�������򣬱�ʾʶ����ĵ����ǹؼ��֣���ֵΪ�ؼ��ֵļǺ�
char now_string[1000]; //��ŵ�ǰ����ʶ��ĵ����ַ���
int pos; //token�е��ַ���β����
int forward; //��ǰָ��
int line; //��ǰ����
char buffer[BUFFERSIZE * 2]; //���뻺����
FILE* fp; //�ļ�ָ��
int return_flag; //��ǰָ�����ʱ�ı�ǣ������ظ����ػ�����

void get_char(); //������ǰָ��forward��ָʾ�����뻺�����ж�һ���ַ��������C�У�Ȼ���ƶ�forward��ʹָ֮����һ���ַ�
void get_nbc(void); //���C�е��ַ��Ƿ�Ϊ�ո����ǣ��򷴸�����get_char��ֱ��C�н���һ���ǿ��ַ�Ϊֹ
void link(void); //��C���ַ�������token�е��ַ�������
void retract(void); //��ǰָ��forward����һ���ַ�
int letter(void); //�ж�C�е��ַ��Ƿ�Ϊ��ĸ�������򷵻�1�����򷵻�0
int digit(void); //�ж�C�е��ַ��Ƿ�Ϊ���֣������򷵻�1�����򷵻�0
int reserve(void); //����token�еĵ��ʲ�ؼ��ֱ���token�еĵ����ǹؼ��֣��򷵻ظùؼ��ֵļǺţ����򣬷���ֵ��-1��
void buffer_fill(int start); //���һ��Ļ�����
void outcome_print(void); //��ӡͳ�ƽ��



int letter(void) {
    return ((C >= 'a' && C <= 'z') || (C >= 'A' && C <= 'Z')) ? 1 : 0;
}

int digit(void) {
    return (C >= '0' && C <= '9') ? 1 : 0;
}

void get_char() {
    C = buffer[forward];
    if (C == '\n') {
        line++;
    }
    forward++;
    if (forward == BUFFERSIZE && return_flag == 0) {
        buffer_fill(BUFFERSIZE);
    }
    else if (forward == BUFFERSIZE * 2 && return_flag == 0) {
        buffer_fill(0);
    }
    return_flag = 0;
    forward = (forward + BUFFERSIZE * 2) % (BUFFERSIZE * 2);
}

void get_nbc(void) {
    while (C == ' ' || C == '\n') {
        get_char();
    }
}

void link(void) {
    now_string[pos++] = C;
}

void retract(void) {
    return_flag = 1;
    forward = (forward + BUFFERSIZE * 2 - 1) % (BUFFERSIZE * 2);
    if (C == '\n') {
        line--;
    }
}


void result_print(int type) {
    now_string[pos] = '\0';
    token_num[type]++;  //��¼�������
    int eline = line;
    if (type == 7 && C == '\n') {
        eline--;
    }
    printf("%d ", eline);
    switch (type)
    {
    case 0:printf("<KEYWORD");break;
    case 1:printf("<IDENTIFIER");break;
    case 2:printf("<OPERATOR");break;
    case 3:printf("<DELIMITER");break;
    case 4:printf("<CHARCON");break;
    case 5:printf("<STRING");break;
    case 6:printf("<NUMBER");break;
    case 7:printf("<ERROR");break;
    default:break;
    }
    printf(",%s>\n",now_string);
    pos = 0;
}


int reserve(void) {
    now_string[pos++] = '\0';
    for (int i = 0; i < KEYSIZE; i++) {
        if (strcmp(keyTable[i], now_string) == 0) {
            return i;
        }
    }
    return -1;
}


void buffer_fill(int start) {
    int i = 0;
    char c;
    while (i < BUFFERSIZE && (c = fgetc(fp)) != EOF) {
        buffer[start + i] = c;
        i++;
    }
    if (c == EOF) {
        buffer[start + i] = '\0';
    }
}

void lexicalAnalyzer(void) {
    state = 0;
    pos = 0;
    do {
        switch (state) {
        case 0: //��ʼ״̬
            get_char();
            get_nbc();
            if (letter() || C == '_') {
                if (C == 'u' || C == 'U') {
                    link();
                    get_char();
                    switch (C) {
                    case '\'':state = 24; break;
                    case '"':state = 25; break;
                    case '8':
                        link();
                        get_char();
                        if (C == '"') {
                            state = 25;
                        }
                        else {
                            retract();//����ǰ�ַ����˵���u��������ʶ���ʶ����״̬state1
                            retract();
                            retract();
                            getchar();
                            pos=pos-2;
                            state = 1;
                        }
                        break;
                    default:retract(); retract();get_char(); pos--; state = 1; break;
                    }
                }
                else if (C == 'L') {
                    link();
                    get_char();
                    switch (C) {
                    case '\'':state = 24; break;
                    case '"':state = 25; break;
                    default:retract(); retract();get_char; pos--; state = 1; break;
                    }

                }
                else {
                state = 1; //���ñ�ʶ��״̬
                }
            }
            else if (digit()) {
                if (C == '0') {
                    link();
                    get_char();
                    if (C == 'x'||C=='X') {
                        link();
                        get_char();
                        state = 26;
                    }
                    else {
                        retract();
                        retract();
                        get_char();
                        pos--;
                        state = 2;
                    }
                }
                else {
                    state = 2; //���ó�����״̬
                }
            }
            else {
                switch (C) {
                case '<': state = 8; break;
                case '>': state = 10; break;
                case '=': state = 12; break;
                case '!': state = 13; break;
                case '+': state = 14; break;
                case '-': state = 15; break;
                case '&': state = 16; break;
                case '|': state = 17; break;
                case '*': state = 18; break;
                case '/': state = 19; break;
                case '%': state = 22; break;
                case '^': state = 23; break;
                case '\'': state = 24; break;
                case '"': state = 25; break;
                case '{':
                    link(); state = 0; result_print(DELIMITER); break;
                case '}':
                    link(); state = 0; result_print(DELIMITER); break;
                case '(':
                    link(); state = 0; result_print(DELIMITER); break;
                case ')':
                    link(); state = 0; result_print(DELIMITER); break;
                case '[':
                    link(); state = 0; result_print(DELIMITER); break;
                case ']':
                    link(); state = 0; result_print(DELIMITER); break;
                case ':':
                    link(); state = 0; result_print(DELIMITER); break;
                case ';':
                    link(); state = 0; result_print(DELIMITER); break;
                case '?':
                    link(); state = 0; result_print(DELIMITER); break;
                case ',':
                    link(); state = 0; result_print(DELIMITER); break;
                case '.':
                    link();
                    get_char();
                    if (digit()) {
                        state = 3;
                    }
                    else {
                        retract();
                        state = 0;
                        result_print(OPERATOR);
                    }
                    break;
                case '~':
                    link(); state = 0; result_print(OPERATOR); break;
                case '@':
                    link(); state = 0; result_print(ERROR); break;
                default:  break;
                }
            }
            break;

        case 1: //��ʶ��״̬
            link();
            get_char();
            if (letter() || digit() || C == '_') {
                state = 1;
            }
            else {
                retract();
                state = 0;
                isKey = reserve();
                if (isKey != -1) {
                    result_print(KEYWORD);
                }
                else {
                    result_print(IDENTIFIER);
                }
            }
            break;

        case 2: //����״̬
            link();
            get_char();
            switch (C) {
            case '0':
            case '1':       
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9': state = 2; break;
            case '.': state = 3; break;
            case 'e':
            case 'E': state = 5; break;
            case 'u':
            case 'U':
            case 'l':
            case 'L':state=27; break;
            default: //ʶ���������
                if (letter()) {//������ֺ������ĸ˵���ǲ��Ϸ���ID
                    link(); state = 0; result_print(ERROR);//��Ϊ��ѯ���ǷǷ���ʶ������.����Ͳ��ܳ����ڱ�ʶ���У��ʲ�������ĸ������С�����Ĵ������
                }
                else {
                    retract(); state = 0; result_print(NUMBER); break;
                }
            }
            break;

        case 3: //С����״̬
            link();
            get_char();
            if (digit()) {
                state = 4;
            }
            else {//С�������û����Ҳ���
                retract(); state = 0; result_print(ERROR);
            }
            break;

        case 4: //С��״̬
            link();
            get_char();
            switch (C) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9': state = 4; break;
            case 'l':
            case 'L':
            case 'F':
            case 'f':
                link(); state = 0; result_print(NUMBER); break;
            case 'e':
            case 'E': state = 5; break;
            default: //ʶ���С��
                retract(); state = 0; result_print(NUMBER); break;
        
            }
            break;

        case 5: //ָ��״̬
            link();
            get_char();
            switch (C) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9': state = 7; break;
            case '+':
            case '-':state = 6; break;
            default:
               retract(); state = 0; result_print(ERROR);break;
            }
            break;

        case 6://E���������ŵ�״̬  
            link();
            get_char();
            if (digit()) {
                state = 7;
            }
            else if (C == 'l' || C == 'F' || C == 'f' || C == 'L') {
                link(); state = 0; result_print(NUMBER);
            }
            else if (letter()) {
                link(); state = 0; result_print(ERROR);
            }
            else {
                retract(); state = 0;
            }
            break;

        case 7://E��û�������ŵ�״̬
            link();
            get_char();
            if (digit()) {
                state = 7;
            }
            else if (C == 'l' || C == 'F' || C == 'f' || C == 'L') {
                link(); state = 0; result_print(NUMBER);
            }
            else if(letter()) {
                link(); state = 0; result_print(ERROR);
            }
            else{
                retract(); state = 0; result_print(NUMBER);
            }
            break;

        case 8: // <
            link();
            get_char();
            switch (C) {
            case '@':
                link(); state = 0; result_print(ERROR); break;
            case '=':
                link(); state = 0; result_print(OPERATOR); break; // <=
            case '<':
                state = 9; break; // <<?
            default:
                retract(); state = 0; result_print(OPERATOR); break; // <
            }
            break;

        case 9: // <<
            link();
            get_char();
            if (C == '@') {
                link(); state = 0; result_print(ERROR);
            }
            else if (C == '=') {
                link(); state = 0; result_print(OPERATOR); // <<=
            }
            else {
                retract(); state = 0; result_print(OPERATOR); // <<
            }
            break;

        case 10: // >
            link();
            get_char();
            switch (C) {
            case '@':
                link(); state = 0; result_print(ERROR); break;
            case '=':
                link(); state = 0; result_print(OPERATOR); break; // >=
            case '>':
                state = 11; break; // >>?
            default:
                retract(); state = 0; result_print(OPERATOR); break; // >
            }
            break;

        case 11: // >>
            link();
            get_char();
            if (C == '@') {//�������     
                link(); state = 0; result_print(ERROR);
            }
            else if (C == '=') {
                link(); state = 0; result_print(OPERATOR); // >>=
            }
            else {
                retract(); state = 0; result_print(OPERATOR); // >>
            }
            break;

        case 12: // =
            link();
            get_char();
            if (C == '@') {//�������     
                link(); state = 0; result_print(ERROR);
            }
            else if (C == '=') {
                link(); state = 0; result_print(OPERATOR); // ==
            }
            else {
                retract(); state = 0; result_print(OPERATOR); // =
            }
            break;

        case 13: // !
            link();
            get_char();
            if (C == '@') {//�������     
                link(); state = 0; result_print(ERROR);
            }
            else if (C == '=') {
                link(); state = 0; result_print(OPERATOR); // !=
            }
            else {
                retract(); state = 0; result_print(OPERATOR); // !
            }
            break;

        case 14: // +
            link();
            get_char();
            switch (C) {
            case '@':
                link(); state = 0; result_print(ERROR); break;
            case '+':
                link(); state = 0; result_print(OPERATOR); break; // ++
            case '=':
                link(); state = 0; result_print(OPERATOR); break; // +=
            default:
                retract(); state = 0; result_print(OPERATOR); break; // +
            }
            break;

        case 15: // -
            link();
            get_char();
            switch (C) {
            case '@':
                link(); state = 0; result_print(ERROR); break;
            case '-':
                link(); state = 0; result_print(OPERATOR); break; // --
            case '=':
                link(); state = 0; result_print(OPERATOR); break; // -=
            case '>':
                link(); state = 0; result_print(OPERATOR);  break; // ->
            default:
                retract(); state = 0; result_print(OPERATOR); break; // -
            }
            break;

        case 16: // &
            link();
            get_char();
            switch (C) {
            case '@':
                link(); state = 0; result_print(ERROR); break;
            case '&':
                link(); state = 0; result_print(OPERATOR); break;// &&
            case '=':
                link(); state = 0; result_print(OPERATOR); break;// &=
            default:
                retract(); state = 0; result_print(OPERATOR); break;// &
            }
            break;

        case 17: // |
            link();
            get_char();
            switch (C) {
            case '@':
                link(); state = 0; result_print(ERROR); break;
            case '|':
                link(); state = 0; result_print(OPERATOR); break;// ||
            case '=':
                link(); state = 0; result_print(OPERATOR); break;// |=
            default:
                retract(); state = 0; result_print(OPERATOR); break;// |
            }
            break;

        case 18: // *
            link();
            get_char();
            if (C == '@') {//�������     
                link(); state = 0; result_print(ERROR);
            }
            else if (C == '=') {
                link(); state = 0; result_print(OPERATOR); // *=
            }
            else {
                retract(); state = 0; result_print(OPERATOR); // *
            }
            break;

        case 19: // /
            link();
            get_char();
            switch (C) {
            case '@':
                link(); state = 0; result_print(ERROR); break;
            case '=':
                link(); state = 0; result_print(OPERATOR); break;// /=
            case '*':
                link(); state = 20; break;// /*   ע��
            case '/':
                link(); state = 21; break;// //
            default:
                retract(); state = 0; result_print(OPERATOR); break;// /
            }
            break;

        case 20: // /* */
            get_char();
            while (C != '*') get_char();
            get_char();
            if (C == '/') {
                state = 0;
                pos = 0;
            }
            else {
                state = 20;
            }
            break;

        case 21: // //
            get_char();
            while (C != '\n' && C != '\0') get_char();
            if (C == '\n') {
                state = 0;
                pos = 0;
            }
            break;

        case 22: // %
            link();
            get_char();
            if (C == '@') {//�������     
                link(); state = 0; result_print(ERROR);
            }
            else if (C == '=') {
                link(); state = 0; result_print(OPERATOR); // %=
            }
            else {
                retract(); state = 0; result_print(OPERATOR); // %
            }
            break;

        case 23: // ^
            link();
            get_char();
            if (C == '@') {//�������     
                link(); state = 0; result_print(ERROR);
            }
            else if (C == '=') {
                link(); state = 0; result_print(OPERATOR); // ^=
            }
            else {
                retract(); state = 0; result_print(OPERATOR); // ^
            }
            break;
        case 24:// '
            link();
            get_char();
            while (C != '\'' && C != '\n' && C != '\0' && C != '\\') {
                link();
                get_char();
            }
            switch (C)
            {
            case '\\'://�����ַ�������ת���ַ�\�����
                link();
                get_char();
                if (C == '\n') {
                    state = 0; result_print(ERROR);
                }
                else {
                    state = 24;
                }
                break;
            case '\'':
                link(); state = 0; result_print(CHARCON); break;
            case '\n':
                state = 0; result_print(ERROR); break;
            default:
                break;
            }
            break;
        case 25:// "
            link();
            get_char();
            while (C != '"' && C != '\n' && C != '\0'&&C!='\\') {
                link(); 
                get_char();
            } 
            switch (C)
            {
            case '\\'://�����ַ�������ת���ַ�\�����
                link();
                get_char();
                if (C == '\n') {
                    state = 0; result_print(ERROR);
                }
                else {
                    state = 25;
                }
                break;
            case '"':
                link();state = 0; result_print(STRING); break;
            case '\n':
                state = 0; result_print(ERROR); break;
            default:
                break;
            }
            break;
        case 26://ʮ������
            link();
            get_char();
            switch (C) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9': state = 26; break;
            default: //ʶ���������
                if ((C>='a'&&C<='f')||(C>='A'&&C<='F')){ 
                    state = 26;
                }
                else {
                    retract(); state = 0; result_print(NUMBER);
                }
                break;  
            }
            break;

        case 27://u/l ��׺
            link();
            get_char();
            switch (C) {
            case 'u':
            case 'U':
            case 'l':
            case 'L':state = 27; break;
            default:retract(); state = 0; result_print(NUMBER); break;
            }
        }
    } while (C != '\0');//���ļ�����ʱC=/0
}

void outcome_print(void) {
    printf("%d\n", line);
    printf("%d %d %d %d %d %d %d\n", token_num[0], token_num[1], token_num[2], token_num[3], token_num[4], token_num[5], token_num[6]);
    printf("%d\n    ", token_num[7]);
}


int main(void) {

    //char s[100]="1.txt";
    char s[100];
    printf("�������ļ�����");
    scanf("%s", s);

    if ((fp = fopen(s, "r")) == NULL) {
        printf("File doesn't exit.");
        return 0;
    }

    //��ʼ������
    forward = 0;
    line = 1;


    buffer_fill(0); //�ȶ������߻�����

    lexicalAnalyzer(); //�ʷ�����

    outcome_print(); //��ӡ���

    fclose(fp);
    system("Pause");
    return 0;
}