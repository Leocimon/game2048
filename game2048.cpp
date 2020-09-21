#include<string>
#include<vector>
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include"curses.h"
using namespace std;

#define N 4             //NxN个格子
#define WIDTH 5         //每个格子宽度

#define S_FAIL 0
#define S_WIN 1
#define S_NORMAL 2
#define S_QUIT 3
#define S_RECORD 4

class game2048{
public:
    game2048():status(S_NORMAL){         //测试函数
        setTestData();
    }

    int getstatus(){ return status; }

    void processInput(){
        char ch = getch();
        if(ch>='a' && ch<='z') ch -= 32;
        if(ch=='Q') status = S_QUIT;
        else if(ch=='R') restart();
        else status = (status + 1)%3;
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
        //显示提示
        mvprintw(2*N+2, (5*(N-4)-1)/2,"W(UP),S(DOWN),A(LEFT),D(RIGHT),R(RESTART),Q(QUIT)");

        if(status == S_WIN) mvprintw(N, 5*N/2-1, " YOU WIN,PRESS R TO CONTINUE ");
        else if(status == S_FAIL) mvprintw(N, 5*N/2-1, " YOU LOSE,PRESS R TO CONTINUE ");

    }

    void setTestData(){
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                data[i][j] = 16 << i << j;
            }
        }
    }

private:
    void moveleft(){
        for(int i=0;i<N;i++){
            int curWritePos = 0;
            int lastVal = 0;
            for(int j=0;j<N;j++){
                if(!data[i][j]) continue;
                if(!lastVal) lastVal = data[i][j];
                else{
                    if(lastVal == data[i][j]){
                        data[i][curWritePos] = lastVal;
                        lastVal = 0;
                    }else{
                        data[i][curWritePos] = lastVal;
                        lastVal = data[i][j];
                    }
                curWritePos++;
                }
            data[i][j] = 0;
            }
            if(lastVal) data[i][curWritePos] = lastVal;
        }
    }

    void restart(){         //重开
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                data[i][j] = 0;
            }
        }
        randnew();
        randnew();
        status = S_NORMAL;
    }

    bool randnew(){
        vector<int> emptyPos;
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                if(!data[i][j]) emptyPos.push_back(i*N+j);
            }
        }

        if(!emptyPos.size()) return false;

        int val = emptyPos[rand()%emptyPos.size()];
        data[val/N][val%N] = rand()%10 == 1 ? 4 : 2;
        return true;
    }

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
    int status;
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
        game.processInput();
    }while(game.getstatus() != S_QUIT);

    shutdown();
    return 0;
}
