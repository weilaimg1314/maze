//
//  main.cpp
//  maze
//
//  Created by WeiLai’s Macbook Pro on 2016/11/22.
//  Copyright © 2016年 WeiLai’s Macbook Pro. All rights reserved.
//
//  迷宫问题求解...............
//
//
//
//  若不支持UTF-8编码
//  可在GCC中添加以下配置：
//  -fexec-charset=GBK
//  -finput-charset=UTF-8
//
//

#include <iostream>
#include <stdlib.h>
using namespace std;

#define OK 1                                    //-------常用宏定义----------
#define ERROR 0
#define OVERFLOW -2
#define MAXSIZE 3000
#define MAX_MAZE_SIZE 50

typedef int Status;                             //--------数据类型自定义---------
typedef short SElemType;

char maze[MAX_MAZE_SIZE][MAX_MAZE_SIZE];                  //-------迷宫的储存结构------------

SElemType first_point=0,last_point=0,maze_size;           //-------迷宫起终点设定标志---------
bool is_set_maze = 0;                           //------迷宫设置标志-------
#include "Stack.h"                              //---------栈的引入----------
#include "statement.h"
SqStack S;                                      //--------设置全局栈------

void any_press(){
    char p;
    //p = getch();                              //如果编译系统为windows，用此句作为输入
    cin >> p;                                   //如果编译系统非windows，尽量用此句输入
}


void cls(){
    //system("cls");                              //如果编译系统为windows，用此句作为清屏
    for(int i = 0 ; i < 50 ; i++)               //如果编译系统非windows，尽量用此句清屏
            cout << '\n';
}

void all_wall(){
    for(int i = 0 ; i < MAX_MAZE_SIZE ; i++)
        for (int j = 0 ; j < MAX_MAZE_SIZE ; j++)
            maze[i][j] = '1';
}

Status Init_Maze(){                             //---------迷宫初始化函数------
    all_wall();
    int m,n;
    char charset='0';
    cout << "               ==============              \n";
    cout << "                迷宫游戏初始化               \n";
    cout << "               ==============              \n";
    cout << "请输入迷宫的高度：";
    cin >> n ;
    cout << "请输入迷宫的宽度：";
    cin >> m;
    cout << "请输入一个高为" << n << "宽为" << m << "的，由0与1构成的迷宫"<< endl;
    cout << "其中，0为迷宫中的通路，1为迷宫中的障碍"<< endl;
    for (int i = 1 ; i <= n ; i++){             //---------迷宫输入--------
        for (int j = 1 ; j <= m ; j++){
            cin >> maze[i][j];
            if(maze[i][j] != '0' && maze [i][j] != '1'){
                charset = '1';
            }
        }
    }
    if(charset == '1'){
        cout << "含有非法字符，迷宫初始化失败，键入任意键退出...";
        cin >> charset;
        clear_maze();
        return ERROR;
    }
    maze_size =Make_SType(n, m, 0);
    is_set_maze = 1;
    cout << "迷宫初始化完毕！"<< endl;
    return OK;
}

SElemType Make_SType(int x , int y ,int dir){                           //------制作栈存类型----------
    return x*100+y*10+dir;
}

Status ReMake_SType (SElemType t, int &x,int &y ,int &dir){             //---------解码栈存类型------
    x  = t / 100;
    y = t / 10 % 10;
    dir  = t % 10;
    return OK;
}

Status Set_Point (){                    //-----------设定起点与终点--------
    int n,m;
    cout << "请输入起点的坐标(例：左上角：1 1)：";
    cin >> n >> m;
    if(maze[n][m] == '1'){
        cout << "输入坐标无效,输入任意键返回...";
        any_press();
        return ERROR;
    }
    first_point = Make_SType(n, m, 0);              //------设置全局起点--------
    cout << "请输入终点的坐标(中间用空格分开)：";
    cin >> n >> m;
    if(maze[n][m] == '1'){
        cout << "输入坐标无效,输入任意键返回...";
        any_press();
        return ERROR;
    }
    last_point = Make_SType(n, m, 0);               //-------设置全局终点---------
    cout << "起点与终点坐标设定完成" << endl;
    return OK;
}

Status is_set_point (){                             //------检查是否设置起终点-------
    return (first_point && last_point);
}



Status Find_Way(){                                  //------查找出路算法-------
    if(!is_set_point()||is_set_maze == 0)
        return ERROR;
    int x,y,dir,f_x,f_y,f_dir;
    SElemType t;
    InitStack(S);
    ReMake_SType(first_point, x, y, dir);
    ReMake_SType(last_point, f_x, f_y, f_dir);
    while(x != f_x || y != f_y){                        //-----四个方向查找路径非递归算法
        if(dir==0){
            if(maze[x+1][y] == '0'){                    //-----若此方向是路-------
                Push (S,Make_SType(x, y, dir));         //-----将所在点入栈-------
                maze[x+1][y] = '1';                     //-----将所在点改为墙------
                x = x+1;                                //-----修改所在点位置------
                dir = 0;                                //-----下一方向初始化------
            } else {                                    //-----若该方向为墙-----
                dir ++;                                 //-----修改方向为下一方向------
            }
        }
        if(dir==1){
            if(maze[x][y+1] == '0'){
                Push(S, Make_SType(x, y, dir));
                maze[x][y+1] = '1';
                y = y+1 ;
                dir = 0;
            } else {
                dir++;
            }
        }
        if(dir==2){
            if(maze[x][y-1] == '0'){
                Push(S, Make_SType(x, y, dir));
                maze[x][y-1] = '1';
                y = y-1 ;
                dir = 0;
            } else {
                dir++;
            }
        }
        if(dir==3){
            if(maze[x-1][y] == '0'){
                Push(S, Make_SType(x, y, dir));
                maze[x-1][y] = '1';
                x = x-1 ;
                dir = 0;
            } else {
                dir++;
            }
        }
        if(dir > 3){                                        //------若四个方向都为墙-----
            t = Pop(S)+1;                                   //------从栈中弹出上一点，并将方向改为下一方向
            if(t != 1){                                     //------若栈非空-----
                ReMake_SType(t, x, y, dir);                 //------解析栈存元素------
            } else {                                        //------若栈空-------
                return ERROR;                               //------返回无出路状态-------
            }
        }
    }
    return OK;                                              //-----若找到出口，返回成功------
}

Status clear_maze(){
    all_wall();
    first_point=0,last_point=0;
    is_set_maze = 0;
    return OK;
}

char menu(){
    char select;
    cout << "               ==============              \n";
    cout << "               迷宫游戏求解方案               \n";
    cout << "               ==============              \n";
    cout << "1.创建新迷宫              迷宫状态：";is_set_maze?cout << "存在迷宫":cout << "无迷宫";cout <<'\n';
    cout << "2.设置起点与终点           起终点状态：";is_set_point()?cout << "已设置":cout << "未设置";cout <<'\n';
    cout << "3.查看迷宫                                   \n";
    cout << "4.查找通路                                   \n";
    cout << "5.清空迷宫                                   \n";
    cout << "6.退出程序                                   \n";
    cout << "请输入你的选择：";
    cin >> select;
    return select;
}

Status scan_maze(){
    if(is_set_maze == 1){
        int n,m,dir;
        ReMake_SType(maze_size, n, m, dir);
        cout << "当前迷宫为：\n";
        for(int i = 1 ; i <= n ; i++){
            for(int j = 1 ; j<= m ; j++){
                cout << maze[i][j];
            }
            cout << '\n';
        }
        if(is_set_point()){
            ReMake_SType(first_point, n, m, dir);
            cout << "迷宫的起点为：("<<n<<','<<m<<')'<<endl;
            ReMake_SType(last_point, n, m, dir);
            cout << "迷宫的终点为：("<<n<<','<<m<<')'<<endl;
        } else {
            cout << "目前未设置迷宫起点与终点...\n";
        }
    } else {
        return ERROR;
    }
    return OK;
}

void Traval(SqStack S){
    int x,y,dir;
    SElemType *p = S.base;
    while(p != S.top){
        ReMake_SType(*p, x, y, dir);
        cout << "在点(" << x << ',' << y << "),向";
        if(dir == 0)
            cout << "下";
        else if(dir == 1)
            cout << "右";
        else if(dir == 2)
            cout << "左";
        else if(dir == 3)
            cout << "上";
        cout << "走\n";
        p ++;
    }
}





int main(){
    while (1){
        char select;
        cls();
        select = menu();
        switch(select){
            case '1':cls();Init_Maze();break;
            case '2':cls();scan_maze();Set_Point();break;
            case '3':cls();
                if(scan_maze() == 0){
                    cout << "未设置迷宫，键入任意键继续...\n";
                    any_press();
                } else {
                    cout << "键入任意键继续...\n";
                    any_press();
                }
                break;
            case '4':cls();
                scan_maze();
                if(Find_Way()){
                    cout << "已找到走出迷宫的路径，路径为：\n";
                    Traval(S);
                } else {
                    cout << "未找到走出迷宫的路径\n";
                }
                cout << "键入任意键重新开始...\n";
                any_press();
                clear_maze();
                break;
            case '5':cls();clear_maze();break;
            case '6':exit(0);
            default :cls();continue;
        }
    }
    return 0;
}