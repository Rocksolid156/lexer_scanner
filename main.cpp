#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;
const int num_of_keyword = 23;
const int num_of_delimiter = 20;
const int num_of_Moperator = 15;  //单目运算符
const int num_of_Boperator = 20;  //双目运算符
/******************************************/
//关键字    1
//标识符    2
//常数		3
//运算符	4
//界符		5
/******************************************/
const char Keyword[num_of_keyword][12] = {"include", "int", "char", "if", "else", "void", "while", "return", "float",
                                          "double", "break", "case", "switch", "default", "continue", "long", "typedef",
                                          "iostream", "do", "define", "for", "const", "struct"};    //关键字
const char Delimiter[num_of_delimiter] = {',', '(', ')', '{', '}', ';', '<', '>', '#', '\'', '\"'};   //界符
const char Monocular_Operator[num_of_Moperator] = {'+', '-', '*', '/', '%', '!', '<', '>', '=', '~', '&', '|',
                                                   '=', '^'};    //单目运算符
const char Binocular_Operator[num_of_Boperator][5] = {"<=", ">=", "==", "!=", "&&", "||", "+=", "-=", "*=", "/=", "++",
                                                      "--"};  //双目运算符
ifstream input;
ofstream output;
FILE *file_source = nullptr; //文件指针默认值

/*Top of //TODO */

struct NFA_set{
    char set[100];
    int len=0;
};
NFA_set Move[100][100];

int dfa[100][100];  //DFA状态转换函数实现：δ(当前状态，输入字母)
bool isFinal[100]; //某状态是否为终态，例如isFinal[1]
int len_state,len_final;

/*Bottom of //TODO */
/***************************************
 * include -> l 0
 * int -> I 1
 * char -> C 2
 * if -> i 3
 * else -> e 4
 * void -> V 5
 * while -> w 6
 * return -> r 7
 * float -> F 8
 * double -> D 9
 * break -> b 10
 * case -> c 11
 * switch -> s 12
 * default -> d 13
 * continue -> G    //GO! 14
 * long -> L 15
 * typedef -> t 16
 * iostream -> k 17
 * do -> g  //干(g)活 18
 * define -> _  //#define _CRT_SECURE_NO_WARNINGS 19
 * for -> f 20
 * const -> S   //solid 21
 * struct -> B  //Build a struct 22
 /***************************************/
char GetToken(char str[]) {
    if (strcmp(str, Keyword[0]) == 0)
        return 'l';
    if (strcmp(str, Keyword[1]) == 0)
        return 'I';
    if (strcmp(str, Keyword[2]) == 0)
        return 'C';
    if (strcmp(str, Keyword[3]) == 0)
        return 'i';
    if (strcmp(str, Keyword[4]) == 0)
        return 'e';
    if (strcmp(str, Keyword[5]) == 0)
        return 'V';
    if (strcmp(str, Keyword[6]) == 0)
        return 'w';
    if (strcmp(str, Keyword[7]) == 0)
        return 'r';
    if (strcmp(str, Keyword[8]) == 0)
        return 'F';
    if (strcmp(str, Keyword[9]) == 0)
        return 'D';
    if (strcmp(str, Keyword[10]) == 0)
        return 'b';
    if (strcmp(str, Keyword[11]) == 0)
        return 'c';
    if (strcmp(str, Keyword[12]) == 0)
        return 's';
    if (strcmp(str, Keyword[13]) == 0)
        return 'd';
    if (strcmp(str, Keyword[14]) == 0)
        return 'G';
    if (strcmp(str, Keyword[15]) == 0)
        return 'L';
    if (strcmp(str, Keyword[16]) == 0)
        return 't';
    if (strcmp(str, Keyword[17]) == 0)
        return 'k';
    if (strcmp(str, Keyword[18]) == 0)
        return 'g';
    if (strcmp(str, Keyword[19]) == 0)
        return '_';
    if (strcmp(str, Keyword[20]) == 0)
        return 'f';
    if (strcmp(str, Keyword[21]) == 0)
        return 'S';
    if (strcmp(str, Keyword[22]) == 0)
        return 'B';
}   //符号化

bool isLetter(char a) {
    if (a >= 'a' && a <= 'z')
        return true;
    if (a >= 'A' && a <= 'Z')
        return true;
    return false;
}

bool isInteger(char a) {
    if (a >= '0' && a <= '9')
        return true;
    return false;
}

bool isMO(char a) {
    for (int i = 0; i < num_of_Moperator; ++i) {
        if (Monocular_Operator[i] == a)
            return true;
    }
    return false;
}   //是否为单目运算符

bool isBO(char a[]) {
    for (int i = 0; i < num_of_Boperator; ++i) {
        if (strcmp(Binocular_Operator[i], a) == 0)
            return true;
    }
    return false;
}   //是否为双目运算符

bool isDelimiter(char a) {
    for (int i = 0; i < num_of_delimiter; ++i) {
        if (Delimiter[i] == a)
            return true;
    }
    return false;
}   //是否为界符

bool isKeyword(char a[]) {
    int len = strlen(a);
    for (int i = 0; i < num_of_keyword; ++i) {
        if (strlen(Keyword[i]) == len)
            if (strcmp(Keyword[i], a) == 0)
                return true;
    }
    return false;
}   //是否为关键字

bool isWs(char a) {
    return false;
} //是否为空符号    //TODO

bool DFA(char str[]) {   //自动机
    char now_state = 0;   //初态
    for (int i = 0; i < strlen(str); ++i) {
        now_state = dfa[now_state][str[i]];
        if (now_state == 1)
            return false;
    }
    if (isFinal[now_state] == true)
        return true;
    return false;
}

void init() {
    len_final = 0;
    len_state = 0;
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            for (int k = 0; k < 100; ++k) {
                Move[i][j].set[k] = '#';
            }
        }
    }
}

void scan() {
    char str[100];
    char ch;
    int i, j;
    int point, flag;

    ch = fgetc(file_source);
    /*读取文件,每成功读取一个字符，指针自动跳到下一个字符
     * 先调用函数给if，来判断开头的字母是什么，然后判断下一步走向
     * fgetc将被看待为书中的GetNextChar()函数
     */
    bool isFinish = false;   //default
    while (!isFinish) {
        flag = -1;    //default
        point = 0;  //default指针
        if (isLetter(ch)) {
            flag = 2;
            str[point++] = ch;
            ch = fgetc(file_source);
            while (isLetter(ch) || isInteger(ch) || ch == '\\') {
                flag = 2;
                str[point++] = ch;
                ch = fgetc(file_source);
            }
            str[point] = '\0';
        }   //P49（3）的实现
        if (flag == 2) {
            if (isKeyword(str)) {
                cout << str << " " << "关键字" << endl;
                output << GetToken(str);
            } else {
                if (DFA(str)) { //TODO
                    cout << str << " " << "标识符" << endl;
                    output << 2;
                } else
                    cout << str << " " << "不是标识符，错误" << endl;
            }
        }
        if (isInteger(ch)) { //判断当前单词是否为整数，若不是就判断为其他种类单词
            flag = 1;
            str[point++] = ch;    //先赋值再自加
            ch = fgetc(file_source); //指针指向代码下一个字符
            /*
             * 假如输入的常量是1，就不会执行下面的while语句
             */
            while (isLetter(ch) || isInteger(ch) || ch == '.' || ch == '+' || ch == '-') {    //判断是否为浮点数，有真则真
                flag = 1;
                str[point++] = ch;
                ch = fgetc(file_source);
            }
            str[point] = '\0';    //字符串结尾，分割出一个已经判断出的词语
        }   //P49（4）的实现
        if (flag == 1) {
            if (DFA(str)) { //TODO
                cout << str << " " << "为常量" << endl;
                output << 3;
            } else
                cout << str << " " << "不是常量，错误" << endl;
            point = 0;    //重置指针
            flag = -1;    //重置浮标
        }
        if (isDelimiter(ch)) {
            cout << ch << " " << "界符" << endl;
            if (ch =='#')
                output << '*';
            else
                output<<ch;
            if ((ch = fgetc(file_source)) == EOF) {
                isFinish= true;
            }
        }
        if (flag == 3) {}  //TODO
        if(isMO(ch)){
            str[point++]=ch;
            if((ch= fgetc(file_source))==EOF){
                isFinish= true;
            }
            str[point++]=ch;
            str[point]='\0';
            if (isFinish== false&& isBO(str)){
                cout<<str<<" "<<"双目运算符"<<endl;
                //output<<  //TODO
                ch= fgetc(file_source);
            } else{
                cout<<str[0]<<" "<<"单目运算符"<<endl;
                output<<str[0];
            }
            point=0;
        }   //P49(8)的实现
        if (ch==' '||ch=='\n'||ch=='\t'){
            if ((ch= fgetc(file_source))==EOF){
                isFinish= true;
            }
            continue;
        }   //P48（1）的实现
    }
    output<<'#';
}

int main() {
    file_source = fopen(".\\input\\source.txt", "r+");
    output.open("output.txt");
    scan();
    fclose(file_source);
    output.close();
    return 0;
}
