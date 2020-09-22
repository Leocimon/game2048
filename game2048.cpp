#include<string>
#include<vector>
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include"curses.h"
using namespace std;

#define N 4             //NxN个格子
#define WIDTH 5         //每个格子宽度
#define TARGET 2048     //胜利条件

#define S_FAIL 0
#define S_WIN 1
#define S_NORMAL 2
#define S_QUIT 3

class game2048{
public:
    game2048():status(S_NORMAL),score(0),maxscore(0){
        setTestData();  //测试函数
    }

    int getstatus(){ return status; }

    void processInput(){
        char ch = getch();
        if(ch>='a' && ch<='z') ch -= 32;
        if(status==S_NORMAL){
            bool updated = false;
            if(ch=='A') updated = moveleft();
            else if(ch=='W'){
                rotate();
                updated = moveleft();
                rotate();
                rotate();
                rotate();
            }else if(ch=='D'){
                rotate();
                rotate();
                updated = moveleft();
                rotate();
                rotate();
            }else if(ch=='S'){
                rotate();
                rotate();
                rotate();
                updated = moveleft();
                rotate();
            }

            if(updated){
                randnew();
                if(isOver()) status = S_FAIL;
            }
        }
        if(ch=='Q') status = S_QUIT;
        else if(ch=='R') restart();
        
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
        //  显示提示
        mvprintw(2*N+2, (5*(N-4)-1)/2,"W(UP),S(DOWN),A(LEFT),D(RIGHT),R(RESTART),Q(QUIT)");
        
        //  显示分数
        mvprintw(0, 0, "score:");
        drawNum(0, 10, score);
        mvprintw(1, 0, "best:");
        drawNum(1, 10, maxscore);

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
    bool isOver(){
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                if((j+1<N) && (data[i][j]*data[i][j+1]==0 || data[i][j]==data[i][j+1])) return false;
                if((i+1<N) && (data[i][j]*data[i+1][j]==0 || data[i][j]==data[i+1][j])) return false;
            }
        }
        return true;
    }

    bool moveleft(){
        int tmp[N][N];
        for(int i=0;i<N;i++){
            int curWritePos = 0;
            int lastVal = 0;
            for(int j=0;j<N;j++){
                tmp[i][j] = data[i][j];         //记录当前游戏状况
                if(!data[i][j]) continue;
                if(!lastVal) lastVal = data[i][j];
                else{
                    if(lastVal == data[i][j]){
                        data[i][curWritePos] = lastVal*2;
                        score += lastVal*2;                         //记录当前分数
                        if(score>maxscore) maxscore = score;        //记录最高分
                        lastVal = 0;
                        if(data[i][curWritePos]==TARGET) status = S_WIN;
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

        // 检查游戏状况是否改变
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                if(data[i][j]!=tmp[i][j]) return true;
            }
        }
        return false;
    }

    void rotate(){          //矩阵旋转函数
        int tmp[N][N] = {0};
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                tmp[i][j] = data[j][N-1-i];
            }
        }
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                data[i][j] = tmp[i][j];
            }
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
        score = 0;
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
    int score;
    int maxscore;
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
    do{
        game.draw();
        game.processInput();
    }while(game.getstatus() != S_QUIT);

    shutdown();
    return 0;
}
