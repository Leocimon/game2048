#include<string>
#include<vector>
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include"curses.h"
using namespace std;

#define N 4             //NxN个格子
#define WIDTH 5         //每个格子宽度

class game2048{
public:
    game2048(){         //测试函数
        setTestData();
    }

    void draw(){        //绘制函数
        clear();
        const int offset = 12;
        for(int i=0;i<=N;i++){
            for(int j=0;j<=N;j++){
                drawItem(i*2, 1+j*WIDTH+offset, '+');
                
                if(i!=N) drawItem(i*2+1, 1+j*WIDTH+offset, '|');

                for(int k=1;j!=N && k<WIDTH;k++) drawItem(i*2, 1+j*WIDTH+k+offset, '-');

                if(i!=N && j!=N) drawNum(i*2+1, (j+1)*WIDTH+offset, data[i][j]);
            }
        }
        mvprintw(2*N+2, (5*(N-4)-1)/2,"W(UP),S(DOWN),A(LEFT),D(RIGHT),R(RESTART),Q(QUIT)");
    }

    void setTestData(){
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                data[i][j] = 16 << i << j;
            }
        }
    }

private:
    void drawItem(int row, int col, char c){
        move(row, col);
        addch(c);
    }

    void drawNum(int row, int col, int num){
        while(num>0){
            drawItem(row, col, num%10 + '0');
            num /= 10;
            col--;
        }
    }

private:
    int data[N][N];
};

void initialize(){
    initscr();          //  按键不需要enter进行交互
    cbreak();           //  不显示按键
    noecho();           //  不显示光标
    curs_set(0);        //  随机数
    srand(time(NULL));
}

void shutdown(){
    endwin();
}

int main(){
    initialize();       //  初始化

    game2048 game;
    char ch = 'n';
    do{
        game.draw();
        ch = getch();
    }while(ch != 'Q' || ch != 'q');

    shutdown();
    return 0;
}
