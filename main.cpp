#include <math.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfx.h>
#include <string>
#include <math.h>
#include <time.h>
#include <complex>
#include <string>
#include <sstream>

using namespace std;

//functions ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void initial_setup();
void window_color(SDL_Renderer *Renderer, int R, int G, int B);
void rect(SDL_Renderer *Renderer, int x,int y,int w,int h,int R, int G, int B, int fill_boolian );
void rect2(SDL_Renderer *Renderer, int x,int y,int w,int h,int R, int G, int B, int fill_boolian , int a );
void ellipse(SDL_Renderer *Renderer, int x, int y, int Radius1, int Radius2, int R, int G, int B, int fill_boolian);
void my_line(SDL_Renderer *Renderer, int x_1, int y_1, int L,double theta,int widht, int R, int G, int B );
void update(int);
void input();
void render();
void endcheck();
void drawplayer(struct player &, int);
void drawplayerg(struct playerg &, int, int);
void drawfan(struct playerg &, int, int);
void drawball(struct toop &);
void drawballg(struct toopg &);
void movetomiddle(player &, player &, toop &);
void movetodefault(player &, player &, toop &);
void movetofield(player &, player &, toop &);
void drawpowerbar(playerg &, playerg &);
double rand_gen();
double normalRandom();
double maximum(double n[10]);
void dopower1();
void dopower2();
void drawclones(playerg &);
void resetgame(int);
void drawtrap(struct trap &, int);

// declarations ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
SDL_Window * m_window;
SDL_Renderer *m_renderer;
bool running=true;
int scorep1=0, scorep2=0, winner=0;
#define WIDTH 1120
#define HIGHT 700
#define M_PI_2 1.57079632679489661923
struct player{
    int x;
    int y;
    int speedx=3;
    int speedy;
    int power;
    int team=0;
    SDL_Rect teamphoto;
    SDL_Texture *teamimg;
};
struct playerg{
    double x;
    double y;
    double speedx=0;
    //bool speedxu;
    double speedy;
    double health;
    int power;
    int team;
    SDL_Rect teamphoto;
    SDL_Texture *teamimg;
    bool poweru=false;
    bool awareness=true;
    bool poison;
};
struct toop{
    int x;
    int y;
    double speedx;
    double speedy;
    int photo=0;
    SDL_Rect ballphoto;
    SDL_Texture *ballimg;
};
struct toopg{
    double x;
    double y;
    double speedx;
    double speedy;
    int photo;
    SDL_Rect ballphoto;
    SDL_Texture *ballimg;
};
struct trap{
    double x;
    double y=500;
};
player p1, p2;
playerg pg1, pg2;
toop ball;
toopg ballg;
trap trap1, trap2;
int lastFrame , lastTime = 0 , frameCount, timerFPS;
double gametimer=100;
unsigned long long int gamestarttime;
string gamestat;
SDL_Event *e = new SDL_Event();
const Uint8 *state = SDL_GetKeyboardState(NULL);
//images
SDL_Rect menubackground;
SDL_Rect gamebackground;
SDL_Rect choosebackground1;
SDL_Rect choosebackground2;
SDL_Rect howtoplaybackground;
SDL_Rect powerphoto;
SDL_Rect powerphoto2;
SDL_Rect enteranceback;
SDL_Texture *menubackimg;
SDL_Texture *gamebackimg;
SDL_Texture *choosebackimg1;
SDL_Texture *choosebackimg2;
SDL_Texture *howtoplaybackimg;
SDL_Texture *powerimg;
SDL_Texture *powerimg2;
SDL_Texture *enteranceimg;
bool goalscore=false;
int latency;
playerg fan11, fan12, fan13, fan14;
playerg fan22 , fan23, fan24;


//main #####################################################################################################################
int main( int argc, char * argv[] ){

    srand(time(NULL));
    initial_setup();

    while(running){

        int n;
        double sigma = 82.0;
        double Mi = 40.0;
        double x = normalRandom()*sigma+Mi;
        n=x;
        n%=3;
        lastFrame = SDL_GetTicks();
        if(lastFrame >= lastTime + 1000){
            lastTime = lastFrame;
            frameCount = 0;
        }

        update(n);
        input();
        render();
        endcheck();

        if (SDL_PollEvent(e)) {
            if (e->type==SDL_KEYDOWN) {
                if (e->key.keysym.sym==SDLK_ESCAPE) break;
            }
        }

    }

    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
    return 0;
}

//functions ################################################################################################################
void initial_setup(){

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(WIDTH, HIGHT, 0, &m_window, &m_renderer);
    SDL_SetWindowSize(m_window, 1120, 768);
    SDL_SetWindowPosition(m_window, 130, 28);
    //Uint32 WND_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP;//SDL_WINDOW_BORDERLESS ;
    //int SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
    p1.x=780;
    p1.y=450;
    p2.x=970;
    p2.y=450;
    gamestat="menu";
    menubackimg = IMG_LoadTexture(m_renderer, "menubackimg.jpg");
    choosebackimg1 = IMG_LoadTexture(m_renderer, "choosebackimg1.jpg");
    choosebackimg2 = IMG_LoadTexture(m_renderer, "choosebackimg2.jpg");
    howtoplaybackimg = IMG_LoadTexture(m_renderer, "howtoplaybackimg.jpg");
    gamebackimg = IMG_LoadTexture(m_renderer, "gameback.jpg");
    ball.ballimg = IMG_LoadTexture(m_renderer, "ball_classic.png");
    enteranceimg = IMG_LoadTexture(m_renderer, "enterance.jpg");
    menubackground.x=0;
    menubackground.y=0;
    menubackground.h=1120;
    menubackground.w=700;
    choosebackground1.x=0;
    choosebackground1.y=0;
    choosebackground1.h=1120;
    choosebackground1.w=700;
    choosebackground2.x=0;
    choosebackground2.y=0;
    choosebackground2.h=1120;
    choosebackground2.w=700;
    howtoplaybackground.x=0;
    howtoplaybackground.y=0;
    howtoplaybackground.h=1120;
    howtoplaybackground.w=700;
    enteranceback.x=0;
    enteranceback.y=0;
    powerphoto.x=40;
    powerphoto.y=452;
    powerphoto2.x=936;
    powerphoto2.y=452;
    ball.x=910;
    ball.y=575;
    p1.power=0;
    p2.power=0;
    int n;
    double sigma = 82.0;
    double Mi = 40.0;
    double x = normalRandom()*sigma+Mi;
    n=x;
    n%=3;
    if(n==-2)
        n*=-1;
    pg1.x=200+2*n;
    pg1.y=500;
    pg2.x=870;
    pg2.y=500;
    ballg.x=560;
    ballg.y=35;
    ballg.speedx=0;
    ballg.speedy=1;
    pg1.health=1;
    pg2.health=1;

    fan11.y=200;
    fan12.y=200;
    fan13.y=200;
    fan14.y=200;
    fan22.y=280;
    fan23.y=280;
    fan24.y=280;

    fan11.x=110;
    fan12.x=200;
    fan13.x=280;
    fan14.x=360;
    fan22.x=150;
    fan23.x=240;
    fan24.x=320;
}

void input(){

    if(gamestat=="menu"){
        SDL_PollEvent(e);
        if(e -> type == SDL_MOUSEBUTTONDOWN){
            SDL_Event k=*e;
            if(k.button.x>173 && k.button.x<425 && k.button.y >310 && k.button.y <370){
                gamestat="choose1";
            }
            if(k.button.x>173 && k.button.x<445 && k.button.y >396 && k.button.y <455){
                gamestat="how to play";
            }
            if(k.button.x>175 && k.button.x<380 && k.button.y >575 && k.button.y <634){
                SDL_DestroyRenderer(m_renderer);
                SDL_DestroyWindow(m_window);
                SDL_Quit();
            }
        }
    }

    else if(gamestat=="choose1"){
        SDL_PollEvent(e);
        if(e -> type == SDL_MOUSEBUTTONDOWN){
            SDL_Event k=*e;
            if(k.button.x>113 && k.button.x<240 && k.button.y >610 && k.button.y <665){
                gamestat="menu";
                SDL_Delay(200);
            }
        }
    }

    else if(gamestat=="choose2"){
        SDL_PollEvent(e);
        if(e -> type == SDL_MOUSEBUTTONDOWN){
            SDL_Event k=*e;
/*back*/    if(k.button.x>113 && k.button.x<240 && k.button.y >610 && k.button.y <665){
                p1.team=0;
                p2.team=0;
                ball.photo=0;
                gamestat="menu";
                SDL_Delay(200);
            }

/*p1team+*/ if(k.button.x>345 && k.button.x<380 && k.button.y >432 && k.button.y <468){
                if(p1.team<8){
                    p1.team++;
                }
                else if(p1.team==8){
                    p1.team=0;
                }
                SDL_Delay(200);
            }

/*p1team-*/  if(k.button.x>345 && k.button.x<380 && k.button.y >535 && k.button.y <572){
                if(p1.team>0){
                    p1.team--;
                }
                else if(p1.team==0){
                    p1.team=8;
                }
                SDL_Delay(200);
            }

/*p2team+*/ if(k.button.x>727 && k.button.x<762 && k.button.y >432 && k.button.y <468){
                if(p2.team<8){
                    p2.team++;
                }
                else if(p2.team==8){
                    p2.team=0;
                }
                SDL_Delay(200);
            }

/*p2team-*/ if(k.button.x>727 && k.button.x<762 && k.button.y >535 && k.button.y <572){
                if(p2.team>0){
                    p2.team--;
                }
                else if(p2.team==0){
                    p2.team=8;
                }
                SDL_Delay(200);
            }

/*p1pwer+*/ if(k.button.x>238 && k.button.x<274 && k.button.y >432 && k.button.y <468){
                if(p1.power<7){
                    p1.power++;
                }
                else if(p1.power==7){
                    p1.power=0;
                }
                SDL_Delay(200);
            }

/*p1pwer-*/ if(k.button.x>238 && k.button.x<274 && k.button.y >535 && k.button.y <572){
                if(p1.power>0){
                    p1.power--;
                }
                else if(p1.power==0){
                    p1.power=7;
                }
                SDL_Delay(200);
            }

/*p2pwer+*/ if(k.button.x>834 && k.button.x<868 && k.button.y >432 && k.button.y <468){
                if(p2.power<7){
                    p2.power++;
                }
                else if(p2.power==7){
                    p2.power=0;
                }
                SDL_Delay(200);
            }

/*p2pwer-*/ if(k.button.x>834 && k.button.x<868 && k.button.y >535 && k.button.y <572){
                if(p2.power>0){
                    p2.power--;
                }
                else if(p2.power==0){
                    p2.power=7;
                }
                SDL_Delay(200);
            }

/*ballph+*/ if(k.button.x>530 && k.button.x<565 && k.button.y >502 && k.button.y <537){
                if(ball.photo<4){
                    ball.photo++;
                }
                if(ball.photo==4){
                    ball.photo=0;
                }
                SDL_Delay(200);
            }

/*ballph-*/ if(k.button.x>530 && k.button.x<565 && k.button.y >617 && k.button.y <652){
                if(ball.photo>0){
                    ball.photo--;
                }
                if(ball.photo==0){
                    ball.photo=4;
                }
                SDL_Delay(200);
            }

/*continue*/if(k.button.x>848 && k.button.x<1025 && k.button.y >617 && k.button.y <672){
                gamestat="enterance";
                SDL_Delay(200);
            }

        }
    }

    else if(gamestat=="how to play"){
        SDL_PollEvent(e);
        if(e -> type == SDL_MOUSEBUTTONDOWN){
            SDL_Event k=*e;
            if(k.button.x>110 && k.button.x<228 && k.button.y >615 && k.button.y <670){
                gamestat="menu";
                SDL_Delay(200);
            }
        }
    }

//keybinds of game ##########################################################################################################
    else if(gamestat=="game"){
            delete state;
        if (state[SDL_SCANCODE_D] && pg1.awareness) {
            if(pg2.x > pg1.x+50 || pg1.y +35 < pg2.y || pg1.x> pg2.x+50)
                pg1.speedx=5.15;
        }
        if (state[SDL_SCANCODE_A] && pg1.awareness) {
            pg1.speedx=-5.15;
        }
        if (state[SDL_SCANCODE_RIGHT] && pg2.awareness) {
            pg2.speedx=5.15;
        }
        if (state[SDL_SCANCODE_LEFT] && pg2.awareness) {
            if(pg2.x > pg1.x+50 || pg1.y > pg2.y +35 || pg2.x> pg1.x+50)
                pg2.speedx= -5.15;
        }
        if (state[SDL_SCANCODE_W] && pg1.awareness) {
            if( pg1.y==500){
                pg1.speedy=-6.2;
            }
        }
        if (state[SDL_SCANCODE_UP] && pg2.awareness) {
            if(pg2.y==500){
                pg2.speedy=-6.2;
            }
        }

        if (state[SDL_SCANCODE_E] && pg1.health >=150 && (pg1.power < 3 || pg1.power==5) && pg1.awareness) {
            pg1.poweru=true;
        }
        if (state[SDL_SCANCODE_RSHIFT] && pg2.health >= 150 && (pg2.power < 3 || pg1.power==5) && pg2.awareness) {
            pg2.poweru=true;
        }
        if (state[SDL_SCANCODE_E] && pg1.health >=150 && pg1.power == 3 && abs(pg1.x-pg2.x)<100 && pg1.awareness) {
            pg1.poweru=true;
        }
        if (state[SDL_SCANCODE_RSHIFT] && pg2.health >= 150&& pg2.power == 3 && abs(pg1.x-pg2.x)<100 && pg2.awareness) {
            pg2.poweru=true;
        }
        if (state[SDL_SCANCODE_E] && pg1.health >=150 && pg1.power == 4 && pg1.awareness) {
            pg1.poweru=true;
            trap1.x = pg1.x;
        }
        if (state[SDL_SCANCODE_RSHIFT] && pg2.health >= 150&& pg2.power == 4 && pg2.awareness) {
            pg2.poweru=true;
            trap2.x = pg2.x;
        }
        if (state[SDL_SCANCODE_E] && pg1.health >=150 && pg1.power == 6 && abs(pg1.x-ballg.x)<80 && pg1.awareness) {
            pg1.poweru=true;
        }
        if (state[SDL_SCANCODE_RSHIFT] && pg2.health >= 150&& pg2.power == 6 && abs(pg1.x-ballg.x)<80 && pg2.awareness) {
            pg2.poweru=true;
        }
    }
}

void update(int n){
    if(gamestat=="game"){
        //ball movement ##################################
        ballg.x+=ballg.speedx;
        ballg.y+=ballg.speedy;
        //player movement ################################
        if(pg1.poison){
            pg1.speedx/=2;
        }
        if(pg2.poison){
            pg2.speedx/=2;
        }
        pg1.y+=pg1.speedy;
        pg1.x+=pg1.speedx;
        pg2.y+=pg2.speedy;
        pg2.x+=pg2.speedx;
        pg1.speedx=0;
        pg2.speedx=0;
        //ball acceleration y ############################
        if(ballg.speedy > 2 || ballg.speedy < -2){
            ballg.speedy+=0.3;
        }
        else if(ballg.speedy < 2 && ballg.speedy > -2 && ballg.y < 580 &&  ballg.y > 552){
            ballg.speedy+=0.2;
        }
        else{
            ballg.speedy+=0.3;
        }
        //ball acceleration x ############################
        double u;
        u = pow(ballg.speedx, 2);
        if(ballg.speedx>0){
            ballg.speedx = sqrt(u *0.975);
        }

        if(ballg.speedx<0){
            ballg.speedx = -1 * sqrt(u*0.975);
        }
        if(fabs(ballg.speedx) < 0.5 ){
            ballg.speedx=0;
        }

        //ball reflection ###############################
        if(ballg.x > 992 && ballg.speedx > 0 && ballg.y < 400){
            ballg.speedx*=-1;
        }

        if(ballg.x > 1060 && ballg.speedx > 0 && ballg.y >= 400){
            scorep1++;
            ballg.speedx*=-1;
            pg1.awareness=false;
            pg2.awareness=false;
            goalscore=true;
            latency=lastTime;
        }

        if(ballg.x > 992 && ballg.speedx > 0 && ballg.y >= 400 && pg2.poweru && pg2.power==5){
            ballg.speedx*=-1;
        }

        if(ballg.x < 128 && ballg.speedx < 0 && ballg.y < 400){
            ballg.speedx*=-1;
        }

        if(ballg.x < 60 && ballg.speedx < 0 && ballg.y >= 400){
            scorep2++;
            ballg.speedx*=-1;
            pg1.awareness=false;
            pg2.awareness=false;
            goalscore=true;
            latency=lastTime;
        }

        if(ballg.x < 128 && ballg.speedx < 0 && ballg.y >= 400 && pg1.poweru && pg1.power==5){
            ballg.speedx*=-1;
        }

        if(ballg.speedy < 0.41 && ballg.speedy >0  && ballg.y > 580 && ballg.y < 584){
               ballg.speedy=0;
               ballg.y=582;
        }

        if(ballg.y > 582 && ballg.speedy > 0){
            double k = pow(ballg.speedy, 2);
            ballg.speedy = sqrt(k*0.4);
            ballg.speedy*=-1;
        }

        if(ballg.y < 0 && ballg.speedy < 0){
            ballg.speedy*=-1;
        }

        //player acceleration##############################
        if(pg1.speedy != 0 && pg1.speedy < 20 && pg1.y < 500){
            pg1.speedy += 0.25;
        }

        if(pg2.speedy != 0 && pg2.speedy < 20 && pg2.y < 500){
            pg2.speedy += 0.25;
        }

        if(pg1.y > 500){
            pg1.y = 500;
            pg1.speedy=0;
        }

        if(pg2.y > 500){
            pg2.y = 500;
            pg2.speedy=0;
        }

        //players strike foot #############################
        if(pg1.x-ballg.x <= 22 && pg1.x-ballg.x >= 10 && ballg.y - pg1.y < 100 && ballg.y - pg1.y  >= 65 && pg1.awareness){
            ballg.speedx=9;
            ballg.speedy=-2.8;
            //cout<<"f1R "<<ballg.x<<" "<<pg1.x<<endl;
        }

        if(ballg.x - pg1.x <= 72 && ballg.x - pg1.x >= 60 && ballg.y - pg1.y < 100 && ballg.y - pg1.y  >= 65 && pg1.awareness){
            ballg.speedx=13.5;
            ballg.speedy=-4.8 + pg1.speedy;
            //cout<<"f1R "<<ballg.x<<" "<<pg1.x<<endl;
        }

        if(pg2.x-ballg.x <= 22 && pg2.x-ballg.x >= 10 && ballg.y - pg2.y < 100 && ballg.y - pg2.y  >= 65 && pg2.awareness){
            ballg.speedx=-13.5;
            ballg.speedy=-4.8 + pg2.speedy;
            //cout<<"f2L "<<ballg.x<<" "<<pg2.x<<endl;
        }

        if(ballg.x - pg2.x <= 72 && ballg.x - pg2.x >= 60 && ballg.y - pg2.y < 100 && ballg.y - pg2.y  >= 65 && pg2.awareness){
            ballg.speedx=-9;
            ballg.speedy=-2.8;
            //cout<<"f2R "<<ballg.x<<" "<<pg2.x<<endl;
        }

        //players strike CLONE foot #############################

        if((pg1.x+120)-ballg.x <= 22 && (pg1.x+120)-ballg.x >= 10 && ballg.y - pg1.y < 100 && ballg.y - pg1.y  >= 65 && pg1.power== 1 && pg1.poweru==true && pg1.awareness){
            ballg.speedx=9;
            ballg.speedy=-2.8;
            //cout<<"f1R "<<ballg.x<<" "<<pg1.x<<endl;
        }

        if(ballg.x - (pg1.x+120) <= 72 && ballg.x - (pg1.x+120) >= 60 && ballg.y - pg1.y < 100 && ballg.y - pg1.y  >= 65 && pg1.power== 1 && pg1.poweru==true && pg1.awareness){
            ballg.speedx=13.5;
            ballg.speedy=-4.8 + pg1.speedy;
            //cout<<"f1R "<<ballg.x<<" "<<pg1.x<<endl;
        }

        if((pg2.x+120)-ballg.x <= 22 && (pg2.x+120)-ballg.x >= 10 && ballg.y - pg2.y < 100 && ballg.y - pg2.y  >= 65 && pg2.power== 1 && pg2.poweru==true && pg2.awareness){
            ballg.speedx=-13.5;
            ballg.speedy=-4.8 + pg2.speedy;
            //cout<<"f2L "<<ballg.x<<" "<<pg2.x<<endl;
        }

        if(ballg.x - (pg2.x+120) <= 72 && ballg.x - (pg2.x+120) >= 60 && ballg.y - pg2.y < 100 && ballg.y - pg2.y  >= 65 && pg2.power== 1 && pg2.poweru==true && pg2.awareness){
            ballg.speedx=-9;
            ballg.speedy=-2.8;
            //cout<<"f2R "<<ballg.x<<" "<<pg2.x<<endl;
        }

        //players strike CLONE foot #############################

        if((pg1.x-120)-ballg.x <= 22 && (pg1.x-120)-ballg.x >= 10 && ballg.y - pg1.y < 100 && ballg.y - pg1.y  >= 65 && pg1.power== 1 && pg1.poweru==true && pg1.awareness){
            ballg.speedx=9;
            ballg.speedy=-2.8;
            //cout<<"f1R "<<ballg.x<<" "<<pg1.x<<endl;
        }

        if(ballg.x - (pg1.x-120) <= 74 && ballg.x - (pg1.x-120) >= 58 && ballg.y - pg1.y < 100 && ballg.y - pg1.y  >= 65 && pg1.power== 1 && pg1.poweru==true && pg1.awareness){
            ballg.speedx=13.5;
            ballg.speedy=-4.8 + pg1.speedy;
            //cout<<"f1R "<<ballg.x<<" "<<pg1.x<<endl;
        }

        if((pg2.x-120)-ballg.x <= 22 && (pg2.x-120)-ballg.x >= 10 && ballg.y - pg2.y < 100 && ballg.y - pg2.y  >= 65 && pg2.power== 1 && pg2.poweru==true && pg2.awareness){
            ballg.speedx=-13.5;
            ballg.speedy=-4.8 + pg2.speedy;
            //cout<<"f2L "<<ballg.x<<" "<<pg2.x<<endl;
        }

        if(ballg.x - (pg2.x-120) <= 74 && ballg.x - (pg2.x-120) >= 58 && ballg.y - pg2.y < 100 && ballg.y - pg2.y  >= 65 && pg2.power== 1 && pg2.poweru==true && pg2.awareness){
            ballg.speedx=-9;
            ballg.speedy=-2.8;
            //cout<<"f2R "<<ballg.x<<" "<<pg2.x<<endl;
        }

        //players strike body #############################
        if(pg1.x - ballg.x <= 24 && pg1.x-ballg.x >= 10 && ballg.y - pg1.y < 65 && ballg.y - pg1.y  > 0 && ballg.speedx >0 && pg1.power!= 1 && pg1.poweru==false && pg1.awareness){
            ballg.speedx*=-0.3;
            ballg.speedy-=4.5;
            //cout<<"b1R "<<ballg.x<<" "<<pg1.x<<endl;
        }

        if(ballg.x - pg1.x <= 74 && ballg.x - pg1.x >= 58 && ballg.y - pg1.y < 65 && ballg.y - pg1.y  > 0 && ballg.speedx <0 && pg1.awareness){
            ballg.speedx*=-1;
            ballg.speedy-=5.5;
            //cout<<"b1R "<<ballg.x<<" "<<pg1.x<<endl;
        }

        if(pg2.x-ballg.x <= 24 && pg2.x-ballg.x >= 10 && ballg.y - pg2.y < 65 && ballg.y - pg2.y  > 0 && ballg.speedx >0 && pg2.awareness){
            ballg.speedx*=-1;
            ballg.speedy-=5.5;
            //cout<<"b2L "<<ballg.x<<" "<<pg2.x<<endl;
        }

        if(ballg.x - pg2.x <= 74 && ballg.x - pg2.x >= 58 && ballg.y - pg2.y < 65 && ballg.y - pg2.y  > 0 && ballg.speedx <0 && pg2.power!= 1 && pg2.poweru==false && pg2.awareness){
            ballg.speedx*=-0.3;
            ballg.speedy-=4.5;
            //cout<<"b2R "<<ballg.x<<" "<<pg2.x<<endl;
        }

        //players clone strike body #############################
        if((pg1.x-120) - ballg.x <= 24 && (pg1.x-120)-ballg.x >= 10 && ballg.y - pg1.y < 65 && ballg.y - pg1.y  > 0 && ballg.speedx >0 && pg1.power== 1 && pg1.poweru==true && pg1.awareness){
            //clone p1left left
            ballg.speedx*=-1;
            ballg.speedy-=5.5;
            //cout<<"b1R "<<ballg.x<<" "<<pg1.x<<endl;
        }

        if(ballg.x - (pg1.x-120) <= 72 && ballg.x - (pg1.x-120) >= 60 && ballg.y - pg1.y < 65 && ballg.y - pg1.y  > 0 && ballg.speedx <0 && pg1.power== 1 && pg1.poweru==true && pg1.awareness){
            //clone p1left right
            ballg.speedx*=-1;
            ballg.speedy-=5.5;
            //cout<<"b1R "<<ballg.x<<" "<<pg1.x<<endl;
        }

        if(ballg.x - (pg1.x+120) <= 72 && ballg.x - (pg1.x+120) >= 60 && ballg.y - pg1.y < 65 && ballg.y - pg1.y  > 0 && ballg.speedx <0 && pg1.power== 1 && pg1.poweru==true && pg1.awareness){
            ballg.speedx*=-1;
            ballg.speedy-=5.5;
            //cout<<"b1R "<<ballg.x<<" "<<pg1.x<<endl;
        }

        if((pg2.x-120)-ballg.x <= 24 && (pg2.x-120)-ballg.x >= 10 && ballg.y - pg2.y < 65 && ballg.y - pg2.y  > 0 && ballg.speedx >0 && pg2.power== 1 && pg2.poweru==true && pg2.awareness){
            ballg.speedx*=-1;
            ballg.speedy-=5.5;
            //cout<<"b2L "<<ballg.x<<" "<<pg2.x<<endl;
        }

        if(ballg.x - (pg2.x+120) <= 72 && ballg.x - (pg2.x+120) >= 60 && ballg.y - pg2.y < 65 && ballg.y - pg2.y  > 0 && ballg.speedx <0 && pg2.power== 1 && pg2.poweru==true && pg2.awareness){
            ballg.speedx*=-1;
            ballg.speedy-=5.5;
            //cout<<"b2R "<<ballg.x<<" "<<pg2.x<<endl;
        }

        if((pg2.x+120)-ballg.x <= 24 && (pg2.x+120)-ballg.x >= 10 && ballg.y - pg2.y < 65 && ballg.y - pg2.y  > 0 && ballg.speedx >0 && pg2.power== 1 && pg2.poweru==true && pg2.awareness){
            ballg.speedx*=-1;
            ballg.speedy-=5.5;
            //cout<<"b2L "<<ballg.x<<" "<<pg2.x<<endl;
        }

        //players strike head #############################

        /*
        if( sqrt(pow(ballg.x +ballg.speedx - (pg1.x + pg1.speedx +25),2) + pow(ballg.y + ballg.speedy - (pg1.y + pg1.speedy-25),2)) < 43 ){
            complex <double> speedball(ballg.speedx, -1*  ballg.speedy);
            if(arg(speedball) > 1 || arg(speedball) < 2){
                complex <double> symmetricline  (ballg.x - (pg1.x+25), (pg1.y-25) - ballg.y);
                double theta = arg(speedball) + 2* ( arg(symmetricline)-arg(speedball) );
                if(ballg.speedx<0){
                    ballg.speedx = sqrt(pow(ballg.speedx, 2) + pow(ballg.speedy,2)) * cos(theta) + 6 - ballg.speedx;
                }
                else{
                    ballg.speedx = sqrt(pow(ballg.speedx, 2) + pow(ballg.speedy,2)) * cos(theta);
                }
                ballg.speedy = -1 * ( sqrt(pow(ballg.speedx, 2) + pow(ballg.speedy,2)) * sin(theta) - pg1.speedy);
                cout<<"arg speed & arg sym " <<arg(speedball)<<" "<<arg(symmetricline)<<endl;
            }
        }
        if( sqrt(pow(ballg.x + ballg.speedx - (pg2.x + pg2.speedx +25),2) + pow(ballg.y + ballg.speedy - (pg2.y + pg2.speedy-25),2)) < 43){
            complex <double> speedball(ballg.speedx,  ballg.speedy);
            if(arg(speedball) > 1.3 || arg(speedball) < 1.7){
                complex <double> symmetricline  (ballg.x - (pg2.x+25), (pg2.y-25) - ballg.y);
                double theta = arg(speedball) + 2* ( arg(symmetricline)-arg(speedball) );
                ballg.speedx = sqrt(pow(ballg.speedx, 2) + pow(ballg.speedy,2)) * cos(theta) - 6 ;
                ballg.speedy =  ( sqrt(pow(ballg.speedx, 2) + pow(ballg.speedy,2)) * sin(theta) + pg2.speedy);
                cout<<"arg speed & arg sym " <<arg(speedball)<<" "<<arg(symmetricline)<<endl;
            }
        }
        */

        //health ##########################################

        if(n<0){
            n*=-1;
        }
        if(pg1.health<150 && pg1.poweru==false){
            if(n==1){
                if(pg1.power != 4)
                    pg1.health+=1;
                else
                    pg1.health+=2;
            }
        }
        if(pg2.health<150 && pg2.poweru==false){
            if(n==2){
                if(pg2.power != 4){
                    pg2.health+=1;
                }
                else{
                    pg2.health+=2;
                }
            }
        }

        //GOAL SCORE ######################################
        if(goalscore){
            resetgame(n);
        }

    }
}

void render(){

    //clear renderer
    SDL_RenderClear(m_renderer);

    //set fps
    frameCount++;
    timerFPS = SDL_GetTicks() - lastFrame;
    if(timerFPS < (1000/70)){
        SDL_Delay((1000/70) - timerFPS);
    }

    //draw background
    if(gamestat=="menu"){
        SDL_QueryTexture(menubackimg,NULL,NULL,&menubackground.w,&menubackground.h);
        SDL_RenderCopy(m_renderer,menubackimg,NULL,&menubackground);
        movetodefault(p1, p2, ball);
    }
    if(gamestat=="choose1"){
        SDL_QueryTexture(choosebackimg1,NULL,NULL,&choosebackground1.w,&choosebackground1.h);
        SDL_RenderCopy(m_renderer,choosebackimg1,NULL,&choosebackground1);
        movetomiddle(p1, p2, ball);
    }
    if(gamestat=="choose2"){
        SDL_QueryTexture(choosebackimg2,NULL,NULL,&choosebackground2.w,&choosebackground2.h);
        SDL_RenderCopy(m_renderer,choosebackimg2,NULL,&choosebackground2);
        SDL_DestroyTexture(powerimg);
        switch (p1.power){
            case 0:
                powerimg=IMG_LoadTexture(m_renderer, "");
                break;
            case 1:
                powerimg=IMG_LoadTexture(m_renderer, "power1.jpg");
                break;
            case 2:
                powerimg=IMG_LoadTexture(m_renderer, "power2.jpg");
                break;
            case 3:
                powerimg=IMG_LoadTexture(m_renderer, "power3.jpg");
                break;
            case 4:
                powerimg=IMG_LoadTexture(m_renderer, "power4.jpg");
                break;
            case 5:
                powerimg=IMG_LoadTexture(m_renderer, "power5.jpg");
                break;
            case 6:
                powerimg=IMG_LoadTexture(m_renderer, "power6.jpg");
                break;
            case 7:
                SDL_DestroyTexture(powerimg);
                powerimg=IMG_LoadTexture(m_renderer, "power7.jpg");
                break;
        }
        SDL_DestroyTexture(powerimg2);
        switch (p2.power){
            case 0:
                powerimg2=IMG_LoadTexture(m_renderer, "");
                break;
            case 1:
                powerimg2=IMG_LoadTexture(m_renderer, "power1.jpg");
                break;
            case 2:
                powerimg2=IMG_LoadTexture(m_renderer, "power2.jpg");
                break;
            case 3:
                powerimg2=IMG_LoadTexture(m_renderer, "power3.jpg");
                break;
            case 4:
                powerimg2=IMG_LoadTexture(m_renderer, "power4.jpg");
                break;
            case 5:
                powerimg2=IMG_LoadTexture(m_renderer, "power5.jpg");
                break;
            case 6:
                powerimg2=IMG_LoadTexture(m_renderer, "power6.jpg");
                break;
            case 7:
                SDL_DestroyTexture(powerimg);
                powerimg2=IMG_LoadTexture(m_renderer, "power7.jpg");
                break;
        }
        SDL_QueryTexture(powerimg,NULL,NULL,&powerphoto.w,&powerphoto.h);
        SDL_RenderCopy(m_renderer,powerimg,NULL,&powerphoto);
        SDL_QueryTexture(powerimg2,NULL,NULL,&powerphoto2.w,&powerphoto2.h);
        SDL_RenderCopy(m_renderer,powerimg2,NULL,&powerphoto2);
    }
    if(gamestat=="how to play"){
        SDL_QueryTexture(howtoplaybackimg,NULL,NULL,&howtoplaybackground.w,&howtoplaybackground.h);
        SDL_RenderCopy(m_renderer,howtoplaybackimg,NULL,&howtoplaybackground);
    }
    if(gamestat=="enterance"){
        SDL_QueryTexture(enteranceimg,NULL,NULL,&enteranceback.w,&enteranceback.h);
        SDL_RenderCopy(m_renderer,enteranceimg,NULL,&enteranceback);
        movetofield(p1, p2, ball);
    }
    if(gamestat=="game" || gamestat=="gameover"){
        SDL_QueryTexture(gamebackimg,NULL,NULL,&gamebackground.w,&gamebackground.h);
        rect(m_renderer, 0, 0, 1120, 700, 200, 200, 200, 1);
        //SDL_RenderCopy(m_renderer,gamebackimg,NULL,&gamebackground);
        drawpowerbar(pg1, pg2);
    }


    if(gamestat=="menu" || gamestat=="choose1" || gamestat=="choose2" || gamestat=="enterance"){
        //draw players
        drawplayer(p1, 1);
        drawplayer(p2, 2);
        //draw ball
        drawball(ball);
    }
    if(gamestat=="game" || gamestat=="gameover"){
        //fans
        fan14.team=pg1.team;
        fan22.team=pg1.team;
        fan23.team=pg1.team;

        drawfan(fan11, 1, 0);
        drawfan(fan12, 2, 0);
        drawfan(fan13, 1,0);
        drawplayerg(fan14, 1, 0);
        drawplayerg(fan22, 1,0);
        drawplayerg(fan23, 1, 0);
        drawfan(fan24, 2, 0);


        //scoreboard
        /*
        rect(m_renderer, 393, 5, 15, 134, 255, 50, 50, 1);
        rect(m_renderer, 712, 5, 15, 134, 0, 255, 128, 1);
        rect(m_renderer, 528, 5, 64, 134, 75,0,130, 1);
        rect(m_renderer, 408, 5, 120, 134, 255, 255, 255, 1);
        rect(m_renderer, 592, 5, 120, 134, 255, 255, 255, 1);
        gametimer = 99 - (SDL_GetTicks()-gamestarttime)/1000 ;
        if(gametimer<1)
            gametimer=0;
        int gametimerint= int(gametimer);
        stringstream ss;
        ss<<gametimerint;
        char gametimerstr[2];
        ss>>gametimerstr;
        if(gamestat=="game")
        textRGBA(m_renderer, 538, 50, gametimerstr , 5, 40, 255, 255, 255,255);
        */

        //scores
        switch (scorep1){
        case 0:
            textRGBA(m_renderer, 428, -8, "0", 5, 140, 0, 0, 0,255);
            break;
        case 1:
            textRGBA(m_renderer, 428, -8, "1", 5, 140, 0, 0, 0,255);
            break;
        case 2:
            textRGBA(m_renderer, 428, -8, "2", 5, 140, 0, 0, 0,255);
            break;
        case 3:
            textRGBA(m_renderer, 428, -8, "3", 5, 140, 0, 0, 0,255);
            break;
        case 4:
            textRGBA(m_renderer, 428, -8, "4", 5, 140, 0, 0, 0,255);
            break;
        case 5:
            textRGBA(m_renderer, 428, -8, "5", 5, 140, 0, 0, 0,255);
            break;
        }
        switch (scorep2){
        case 0:
            textRGBA(m_renderer, 612, -8, "0", 5, 140, 0, 0, 0,255);
            break;
        case 1:
            textRGBA(m_renderer, 612, -8, "1", 5, 140, 0, 0, 0,255);
            break;
        case 2:
            textRGBA(m_renderer, 612, -8, "2", 5, 140, 0, 0, 0,255);
            break;
        case 3:
            textRGBA(m_renderer, 612, -8, "3", 5, 140, 0, 0, 0,255);
            break;
        case 4:
            textRGBA(m_renderer, 612, -8, "4", 5, 140, 0, 0, 0,255);
            break;
        case 5:
            textRGBA(m_renderer, 612, -8, "5", 5, 140, 0, 0, 0,255);
            break;
        }

        //margins

        my_line(m_renderer, 0, 145, 1120,0, 2, 0, 0, 0);
        my_line(m_renderer, 0, 358, 1120,0, 2, 0, 0, 0);
        /*
        my_line(m_renderer, 100, 370, 330,-1*M_PI_2, 2, 0, 255, 0);
        my_line(m_renderer, 100, 370, 370,M_PI_2, 2, 0, 255, 255);
        my_line(m_renderer, 1020, 370, 370,M_PI_2, 2, 0, 255, 255);
        my_line(m_renderer, 1020, 370, 330,-1*M_PI_2, 2, 0, 255, 0);
        */

        //draw players
        drawplayerg(pg1, 1, 1);
        drawplayerg(pg2, 2, 1);
        //draw ball
        drawballg(ballg);
        //draw power
        if(pg1.poweru){
            dopower1();
        }
        if(pg2.poweru){
            dopower2();
        }
        if(gamestat=="gameover"){
            rect2(m_renderer, 0, 0, 1120, 700, 255, 255, 255, 1, 100);
            if(winner==1){
                textRGBA(m_renderer, 200, 300, "PLAYER 1 WON!" , 5, 100, 255, 50, 50,255);
            }
            if(winner==2){
                textRGBA(m_renderer, 200, 300, "PLAYER 2 WON!" , 5, 100, 0, 255, 128,255);
            }
            if(winner==0){
                textRGBA(m_renderer, 408, 300, "DRAW!" , 5, 100, 255, 255, 255,255);
            }
        }
    }


    //show renderer
    SDL_RenderPresent(m_renderer);
}

void endcheck(){
    if(gametimer<=1){
        if(scorep1>scorep2)
            winner=1;
        if(scorep1<scorep2)
            winner=2;
        if(scorep1==scorep2)
            winner=0;
        gamestat="gameover";
    }
    if(scorep1==5){
        winner=1;
        gamestat="gameover";
    }
    if(scorep2==5){
        winner=2;
        gamestat="gameover";
    }
    if(scorep1==scorep2 && scorep1==4){
        winner=0;
        gamestat="gameover";
    }
}


//game actions##############################################################################################################

void drawplayer(struct player &player, int i){
    player.teamphoto.x=player.x;
    player.teamphoto.y=player.y;
    player.teamphoto.w=70;
    player.teamphoto.h=70;
    //body
    if(i==1)
    rect(m_renderer, player.x, player.y, 70, 70,  255, 50, 50, 1);
    if(i==2)
    rect(m_renderer, player.x, player.y, 70, 70,  0, 255, 128, 1);
    //feet
    rect(m_renderer, player.x, player.y+70, 70, 70,  255, 195, 170, 1);     //sagh
    rect(m_renderer, player.x, player.y+110, 70, 30,  101, 67, 33, 1);      //kafsh
    //head
    ellipse(m_renderer, player.x+35, player.y-35, 35, 35, 255, 195, 170, 1);
    //face
    if(ball.x >= player.x){
        my_line(m_renderer,player.x+28, player.y-45, 10, 0, 2, 0, 0, 0);
        my_line(m_renderer,player.x+48, player.y-45, 10, 0, 2, 0, 0, 0);
        my_line(m_renderer,player.x+30, player.y-25, 25, 0, 2, 0, 0, 0);
    }
    if(ball.x < player.x){
        my_line(m_renderer,player.x+14, player.y-45, 10, 0, 2, 0, 0, 0);
        my_line(m_renderer,player.x+34, player.y-45, 10, 0, 2, 0, 0, 0);
        my_line(m_renderer,player.x+19, player.y-25, 25, 0, 2, 0, 0, 0);
    }
    //team
    SDL_DestroyTexture(player.teamimg);
    switch (player.team){
        case 0:
            //short
            rect(m_renderer, player.x, player.y+70, 70, 20, 0, 0, 255, 1);
            break;
        case 1:
            player.teamimg = IMG_LoadTexture(m_renderer, "team1_barca.jpg");
            //short
            rect(m_renderer, player.x, player.y+70, 70, 20, 0, 0, 120, 1);
            break;
        case 2:
            player.teamimg = IMG_LoadTexture(m_renderer, "team2_tractor.jpg");
            //short
            rect(m_renderer, player.x, player.y+70, 70, 20, 200, 0, 0, 1);
            break;
        case 3:
            player.teamimg = IMG_LoadTexture(m_renderer, "team3_realmadrid.png");
            //short
            rect(m_renderer, player.x, player.y+70, 70, 20, 255, 255, 255, 1);
            break;
        case 4:
            player.teamimg = IMG_LoadTexture(m_renderer, "team4_perspolis.jpg");
            //short
            rect(m_renderer, player.x, player.y+70, 70, 20, 255, 0, 0, 1);
            break;
        case 5:
            player.teamimg = IMG_LoadTexture(m_renderer, "team5_sepahan.jpg");
            //short
            rect(m_renderer, player.x, player.y+70, 70, 20, 30, 30, 30, 1);
            break;
        case 6:
            player.teamimg = IMG_LoadTexture(m_renderer, "team6_esteghlal.jpg");
            //short
            rect(m_renderer, player.x, player.y+70, 70, 20, 0, 0, 255, 1);
            break;
        case 7:
            player.teamimg = IMG_LoadTexture(m_renderer, "team7_bayernmunich.jpg");
            //short
            rect(m_renderer, player.x, player.y+70, 70, 20, 0, 0, 255, 1);
            break;
        case 8:
            player.teamimg = IMG_LoadTexture(m_renderer, "team8_juventus.jpg");
            //short
            rect(m_renderer, player.x, player.y+70, 70, 20, 0, 0, 0, 1);
            break;
    }
    SDL_QueryTexture(player.teamimg,NULL,NULL,&player.teamphoto.w,&player.teamphoto.h);
    SDL_RenderCopy(m_renderer,player.teamimg,NULL,&player.teamphoto);
    //hands
    if(ball.x >= player.x){
        my_line(m_renderer,player.x+15, player.y+15, 35, -1 *M_PI_2, 2, 0, 0, 0);
        rect(m_renderer, player.x, player.y+40, 15, 11, 255, 195, 170, 1);
    }
    if(ball.x < player.x){
        my_line(m_renderer,player.x+55, player.y+15, 35, -1 *M_PI_2, 2, 0, 0, 0);
        rect(m_renderer, player.x+55, player.y+40, 15, 11, 255, 195, 170, 1);
    }
    my_line(m_renderer, player.x, player.y+70, 70, 0, 2, 0, 0, 0);
    thickLineRGBA(m_renderer, player.x+35, player.y+80, player.x+35, player.y+139, 2, 0, 0, 0, 200);
}

void drawplayerg(struct playerg &player, int i, int j){
    player.teamphoto.x=player.x;
    player.teamphoto.y=player.y;
    player.teamphoto.w=50;
    player.teamphoto.h=50;
    //body
    if(i==2)
    rect(m_renderer, player.x, player.y, 50, 50,  255, 50, 50, 1);
    if(i==1)
    rect(m_renderer, player.x, player.y, 50, 50,  0, 255, 128, 1);
    //feet
    rect(m_renderer, player.x, player.y+50, 50, 50,  255, 195, 170, 1);     //sagh
    rect(m_renderer, player.x, player.y+85, 50, 15,  101, 67, 33, 1);      //kafsh
    //head
    ellipse(m_renderer, player.x+25, player.y-25, 25, 25, 255, 195, 170, 1);
    //face
    if(ballg.x >= player.x && player.awareness){
        my_line(m_renderer,player.x+14, player.y-30, 7, 0, 2, 0, 0, 0);
        my_line(m_renderer,player.x+29, player.y-30, 7, 0, 2, 0, 0, 0);
        my_line(m_renderer,player.x+18, player.y-18, 15, 0, 2, 0, 0, 0);
    }


    if(ballg.x >= player.x && player.awareness==false){
        my_line(m_renderer,player.x+18, player.y-30, 8, M_PI_4, 2, 0, 0, 0);
        my_line(m_renderer,player.x+33, player.y-30, 8, M_PI_4, 2, 0, 0, 0);
        my_line(m_renderer,player.x+18, player.y-35, 8, -1*M_PI_4, 2, 0, 0, 0);
        my_line(m_renderer,player.x+33, player.y-35, 8, -1*M_PI_4, 2, 0, 0, 0);
        my_line(m_renderer,player.x+22, player.y-18, 15, 0, 2, 0, 0, 0);
    }
    if(ballg.x < player.x && player.awareness){
        my_line(m_renderer,player.x+10, player.y-30, 7, 0, 2, 0, 0, 0);
        my_line(m_renderer,player.x+25, player.y-30, 7, 0, 2, 0, 0, 0);
        my_line(m_renderer,player.x+15, player.y-18, 15, 0, 2, 0, 0, 0);
    }
    if(ballg.x < player.x && player.awareness==false){
        my_line(m_renderer,player.x+10, player.y-30, 8, M_PI_4, 2, 0, 0, 0);
        my_line(m_renderer,player.x+25, player.y-30, 8, M_PI_4, 2, 0, 0, 0);
        my_line(m_renderer,player.x+10, player.y-35, 8, -1*M_PI_4, 2, 0, 0, 0);
        my_line(m_renderer,player.x+25, player.y-35, 8, -1*M_PI_4, 2, 0, 0, 0);
        my_line(m_renderer,player.x+15, player.y-18, 15, 0, 2, 0, 0, 0);
    }
    //team
    SDL_DestroyTexture(player.teamimg);
    switch (player.team){
        case 0:
            //short
            rect(m_renderer, player.x, player.y+50, 50, 15, 0, 0, 255, 1);
            break;
        case 1:
            player.teamimg = IMG_LoadTexture(m_renderer, "team1_barcag.jpg");
            //short
            rect(m_renderer, player.x, player.y+50, 50, 15, 0, 0, 120, 1);
            break;
        case 2:
            player.teamimg = IMG_LoadTexture(m_renderer, "team2_tractorg.jpg");
            //short
            rect(m_renderer, player.x, player.y+50, 50, 15, 200, 0, 0, 1);
            break;
        case 3:
            player.teamimg = IMG_LoadTexture(m_renderer, "team3_realmadridg.png");
            //short
            rect(m_renderer, player.x, player.y+50, 50, 15, 255, 255, 255, 1);
            break;
        case 4:
            player.teamimg = IMG_LoadTexture(m_renderer, "team4_perspolisg.jpg");
            //short
            rect(m_renderer, player.x, player.y+50, 50, 15, 255, 0, 0, 1);
            break;
        case 5:
            player.teamimg = IMG_LoadTexture(m_renderer, "team5_sepahang.jpg");
            //short
            rect(m_renderer, player.x, player.y+50, 50, 15, 30, 30, 30, 1);
            break;
        case 6:
            player.teamimg = IMG_LoadTexture(m_renderer, "team6_esteghlalg.jpg");
            //short
            rect(m_renderer, player.x, player.y+50, 50, 15, 0, 0, 255, 1);
            break;
        case 7:
            player.teamimg = IMG_LoadTexture(m_renderer, "team7_bayernmunichg.jpg");
            //short
            rect(m_renderer, player.x, player.y+50, 50, 15, 0, 0, 255, 1);
            break;
        case 8:
            player.teamimg = IMG_LoadTexture(m_renderer, "team8_juventusg.jpg");
            //short
            rect(m_renderer, player.x, player.y+50, 50, 15, 0, 0, 0, 1);
            break;
    }
    SDL_QueryTexture(player.teamimg,NULL,NULL,&player.teamphoto.w,&player.teamphoto.h);
    SDL_RenderCopy(m_renderer,player.teamimg,NULL,&player.teamphoto);

    //hands

    if(ballg.x >= player.x && j==0){
        my_line(m_renderer,player.x+11, player.y+11, 25, -1 *M_PI_2, 2, 0, 0, 0);
        rect(m_renderer, player.x, player.y+28, 11, 9, 255, 195, 170, 1);
    }
    if((ballg.x < player.x || true) && j==0 ){
        my_line(m_renderer,player.x+39, player.y+11, 25, -1 *M_PI_2, 2, 0, 0, 0);
        rect(m_renderer, player.x+39, player.y+28, 11, 9, 255, 195, 170, 1);
    }


    if(ballg.x >= player.x && j==1){
        rect(m_renderer, player.x, player.y-55, 11, 55, 240, 180, 155, 1);
        rect(m_renderer, player.x, player.y-55, 11, 10, 0,0,0, 1);
        my_line(m_renderer,player.x+11, player.y, 55, M_PI_2, 1, 0, 0, 0);
        my_line(m_renderer,player.x, player.y, 55, M_PI_2, 1, 0, 0, 0);
        my_line(m_renderer,player.x, player.y-55, 11, 0, 1, 0, 0, 0);
        my_line(m_renderer,player.x, player.y-46, 11, 0, 1, 0, 0, 0);
    }
    if((ballg.x < player.x || true) && j==1){
        rect(m_renderer, player.x+40, player.y-55, 10, 55, 240, 180, 155, 1);
        rect(m_renderer, player.x+40, player.y-55, 10, 10, 0, 0 , 0, 1);
        my_line(m_renderer,player.x+39, player.y, 55, M_PI_2, 1, 0, 0, 0);
        my_line(m_renderer,player.x+49, player.y, 55, M_PI_2, 1, 0, 0, 0);
        my_line(m_renderer,player.x+39, player.y-46, 10, 0, 1, 0, 0, 0);
        my_line(m_renderer,player.x+39, player.y-55, 10, 0, 1, 0, 0, 0);
    }

    my_line(m_renderer, player.x, player.y+50, 49, 0, 2, 0, 0, 0);
    thickLineRGBA(m_renderer, player.x+25, player.y+60, player.x+25, player.y+99, 2, 0, 0, 0, 200);
}

void drawfan(struct playerg &player, int i, int j){
    player.teamphoto.x=player.x;
    player.teamphoto.y=player.y;
    player.teamphoto.w=50;
    player.teamphoto.h=50;
    //body
    if(i==1)
    rect(m_renderer, player.x, player.y, 50, 50,  0, 255, 200, 1);
    if(i==2)
    rect(m_renderer, player.x, player.y, 50, 50,  240, 240 ,240 , 1);
    //feet
    rect(m_renderer, player.x, player.y+50, 50, 50,  255, 195, 170, 1);     //sagh
    rect(m_renderer, player.x, player.y+85, 50, 15,  101, 67, 33, 1);      //kafsh
    //head
    ellipse(m_renderer, player.x+25, player.y-25, 25, 25, 255, 195, 170, 1);
    //face
    if(ballg.x >= player.x && player.awareness){
        my_line(m_renderer,player.x+14, player.y-30, 7, 0, 2, 0, 0, 0);
        my_line(m_renderer,player.x+29, player.y-30, 7, 0, 2, 0, 0, 0);
        my_line(m_renderer,player.x+18, player.y-18, 15, 0, 2, 0, 0, 0);
    }


    if(ballg.x >= player.x && player.awareness==false){
        my_line(m_renderer,player.x+18, player.y-30, 8, M_PI_4, 2, 0, 0, 0);
        my_line(m_renderer,player.x+33, player.y-30, 8, M_PI_4, 2, 0, 0, 0);
        my_line(m_renderer,player.x+18, player.y-35, 8, -1*M_PI_4, 2, 0, 0, 0);
        my_line(m_renderer,player.x+33, player.y-35, 8, -1*M_PI_4, 2, 0, 0, 0);
        my_line(m_renderer,player.x+22, player.y-18, 15, 0, 2, 0, 0, 0);
    }
    if(ballg.x < player.x && player.awareness){
        my_line(m_renderer,player.x+10, player.y-30, 7, 0, 2, 0, 0, 0);
        my_line(m_renderer,player.x+25, player.y-30, 7, 0, 2, 0, 0, 0);
        my_line(m_renderer,player.x+15, player.y-18, 15, 0, 2, 0, 0, 0);
    }
    if(ballg.x < player.x && player.awareness==false){
        my_line(m_renderer,player.x+10, player.y-30, 8, M_PI_4, 2, 0, 0, 0);
        my_line(m_renderer,player.x+25, player.y-30, 8, M_PI_4, 2, 0, 0, 0);
        my_line(m_renderer,player.x+10, player.y-35, 8, -1*M_PI_4, 2, 0, 0, 0);
        my_line(m_renderer,player.x+25, player.y-35, 8, -1*M_PI_4, 2, 0, 0, 0);
        my_line(m_renderer,player.x+15, player.y-18, 15, 0, 2, 0, 0, 0);
    }
    //team
    SDL_DestroyTexture(player.teamimg);
    switch (player.team){
        case 0:
            //short
            rect(m_renderer, player.x, player.y+50, 50, 15, 255,255, 255, 1);
            break;
        case 1:
            player.teamimg = IMG_LoadTexture(m_renderer, "team1_barcag.jpg");
            //short
            rect(m_renderer, player.x, player.y+50, 50, 15, 0, 0, 120, 1);
            break;
        case 2:
            player.teamimg = IMG_LoadTexture(m_renderer, "team2_tractorg.jpg");
            //short
            rect(m_renderer, player.x, player.y+50, 50, 15, 200, 0, 0, 1);
            break;
        case 3:
            player.teamimg = IMG_LoadTexture(m_renderer, "team3_realmadridg.png");
            //short
            rect(m_renderer, player.x, player.y+50, 50, 15, 255, 255, 255, 1);
            break;
        case 4:
            player.teamimg = IMG_LoadTexture(m_renderer, "team4_perspolisg.jpg");
            //short
            rect(m_renderer, player.x, player.y+50, 50, 15, 255, 0, 0, 1);
            break;
        case 5:
            player.teamimg = IMG_LoadTexture(m_renderer, "team5_sepahang.jpg");
            //short
            rect(m_renderer, player.x, player.y+50, 50, 15, 30, 30, 30, 1);
            break;
        case 6:
            player.teamimg = IMG_LoadTexture(m_renderer, "team6_esteghlalg.jpg");
            //short
            rect(m_renderer, player.x, player.y+50, 50, 15, 0, 0, 255, 1);
            break;
        case 7:
            player.teamimg = IMG_LoadTexture(m_renderer, "team7_bayernmunichg.jpg");
            //short
            rect(m_renderer, player.x, player.y+50, 50, 15, 0, 0, 255, 1);
            break;
        case 8:
            player.teamimg = IMG_LoadTexture(m_renderer, "team8_juventusg.jpg");
            //short
            rect(m_renderer, player.x, player.y+50, 50, 15, 0, 0, 0, 1);
            break;
    }
    SDL_QueryTexture(player.teamimg,NULL,NULL,&player.teamphoto.w,&player.teamphoto.h);
    SDL_RenderCopy(m_renderer,player.teamimg,NULL,&player.teamphoto);

    //hands

    if(ballg.x >= player.x && j==0){
        my_line(m_renderer,player.x+11, player.y+11, 25, -1 *M_PI_2, 2, 0, 0, 0);
        rect(m_renderer, player.x, player.y+28, 11, 9, 255, 195, 170, 1);
    }
    if((ballg.x < player.x || true) && j==0){
        my_line(m_renderer,player.x+39, player.y+11, 25, -1 *M_PI_2, 2, 0, 0, 0);
        rect(m_renderer, player.x+39, player.y+28, 11, 9, 255, 195, 170, 1);
    }


    if(ballg.x >= player.x && j==1){
        rect(m_renderer, player.x, player.y-55, 11, 55, 240, 180, 155, 1);
        rect(m_renderer, player.x, player.y-55, 11, 10, 0,0,0, 1);
        my_line(m_renderer,player.x+11, player.y, 55, M_PI_2, 1, 0, 0, 0);
        my_line(m_renderer,player.x, player.y, 55, M_PI_2, 1, 0, 0, 0);
        my_line(m_renderer,player.x, player.y-55, 11, 0, 1, 0, 0, 0);
        my_line(m_renderer,player.x, player.y-46, 11, 0, 1, 0, 0, 0);
    }
    if((ballg.x < player.x || true) && j==1){
        rect(m_renderer, player.x+40, player.y-55, 10, 55, 240, 180, 155, 1);
        rect(m_renderer, player.x+40, player.y-55, 10, 10, 0, 0 , 0, 1);
        my_line(m_renderer,player.x+39, player.y, 55, M_PI_2, 1, 0, 0, 0);
        my_line(m_renderer,player.x+49, player.y, 55, M_PI_2, 1, 0, 0, 0);
        my_line(m_renderer,player.x+39, player.y-46, 10, 0, 1, 0, 0, 0);
        my_line(m_renderer,player.x+39, player.y-55, 10, 0, 1, 0, 0, 0);
    }

    my_line(m_renderer, player.x, player.y+50, 49, 0, 2, 0, 0, 0);
    thickLineRGBA(m_renderer, player.x+25, player.y+60, player.x+25, player.y+99, 2, 0, 0, 0, 200);
}

void drawclones(playerg &player){
    player.teamphoto.x=player.x;
    player.teamphoto.y=player.y;
    player.teamphoto.w=50;
    player.teamphoto.h=50;
    //body
    rect(m_renderer, player.x, player.y, 50, 50,  255, 50, 50, 1);
    //feet
    rect(m_renderer, player.x, player.y+50, 50, 50,  255, 235, 235, 1);     //sagh
    rect(m_renderer, player.x, player.y+85, 50, 15,  101, 67, 33, 1);      //kafsh
    //head
    ellipse(m_renderer, player.x+25, player.y-25, 25, 25, 255, 235, 235, 1);
    //face
    if(ballg.x >= player.x){
        my_line(m_renderer,player.x+18, player.y-30, 7, 0, 2, 0, 0, 0);
        my_line(m_renderer,player.x+33, player.y-30, 7, 0, 2, 0, 0, 0);
        my_line(m_renderer,player.x+22, player.y-18, 15, 0, 2, 0, 0, 0);
    }
    if(ballg.x < player.x){
        my_line(m_renderer,player.x+10, player.y-30, 7, 0, 2, 0, 0, 0);
        my_line(m_renderer,player.x+25, player.y-30, 7, 0, 2, 0, 0, 0);
        my_line(m_renderer,player.x+15, player.y-18, 15, 0, 2, 0, 0, 0);
    }
    //team
    SDL_DestroyTexture(player.teamimg);
    switch (player.team){
        case 0:
            //short
            rect(m_renderer, player.x-1, player.y+50, 51, 15, 0, 0, 255, 1);
            break;
        case 1:
            player.teamimg = IMG_LoadTexture(m_renderer, "team1_barcag.jpg");
            //short
            rect(m_renderer, player.x-1, player.y+50, 51, 15, 0, 0, 120, 1);
            break;
        case 2:
            player.teamimg = IMG_LoadTexture(m_renderer, "team2_tractorg.jpg");
            //short
            rect(m_renderer, player.x-1, player.y+50, 51, 15, 200, 0, 0, 1);
            break;
        case 3:
            player.teamimg = IMG_LoadTexture(m_renderer, "team3_realmadridg.png");
            //short
            rect(m_renderer, player.x-1, player.y+50, 51, 15, 255, 255, 255, 1);
            break;
        case 4:
            player.teamimg = IMG_LoadTexture(m_renderer, "team4_perspolisg.jpg");
            //short
            rect(m_renderer, player.x-1, player.y+50, 51, 15, 255, 0, 0, 1);
            break;
        case 5:
            player.teamimg = IMG_LoadTexture(m_renderer, "team5_sepahang.jpg");
            //short
            rect(m_renderer, player.x-1, player.y+50, 51, 15, 30, 30, 30, 1);
            break;
        case 6:
            player.teamimg = IMG_LoadTexture(m_renderer, "team6_esteghlalg.jpg");
            //short
            rect(m_renderer, player.x-1, player.y+50, 51, 15, 0, 0, 255, 1);
            break;
        case 7:
            player.teamimg = IMG_LoadTexture(m_renderer, "team7_bayernmunichg.jpg");
            //short
            rect(m_renderer, player.x-1, player.y+50, 51, 15, 0, 0, 255, 1);
            break;
        case 8:
            player.teamimg = IMG_LoadTexture(m_renderer, "team8_juventusg.jpg");
            //short
            rect(m_renderer, player.x-1, player.y+50, 51, 15, 0, 0, 0, 1);
            break;
    }
    SDL_QueryTexture(player.teamimg,NULL,NULL,&player.teamphoto.w,&player.teamphoto.h);
    SDL_RenderCopy(m_renderer,player.teamimg,NULL,&player.teamphoto);
    //hands
    if(ballg.x >= player.x){
        my_line(m_renderer,player.x+11, player.y+11, 25, -1 *M_PI_2, 2, 0, 0, 0);
        rect(m_renderer, player.x, player.y+28, 11, 9, 255, 195, 170, 1);
    }
    if(ballg.x < player.x){
        my_line(m_renderer,player.x+39, player.y+11, 25, -1 *M_PI_2, 2, 0, 0, 0);
        rect(m_renderer, player.x+39, player.y+28, 11, 9, 255, 195, 170, 1);
    }
    my_line(m_renderer, player.x, player.y+50, 49, 0, 2, 0, 0, 0);
    thickLineRGBA(m_renderer, player.x+25, player.y+60, player.x+25, player.y+99, 2, 0, 0, 0, 200);
}

void drawball(struct toop &ball){
    SDL_DestroyTexture(ball.ballimg);
    switch(ball.photo){
        case 0:
            ball.ballimg = IMG_LoadTexture(m_renderer, "ball_classic.png");
            break;
        case 1:
            ball.ballimg = IMG_LoadTexture(m_renderer, "ball_volleyball.png");
            break;
        case 2:
            ball.ballimg = IMG_LoadTexture(m_renderer, "ball_alien.png");
            break;
        case 3:
            ball.ballimg = IMG_LoadTexture(m_renderer, "ball_messi.png");
            break;
        case 4:
            ball.ballimg = IMG_LoadTexture(m_renderer, "ball_minion.png");
            break;
    }
    ball.ballphoto.x=ball.x-25;
    ball.ballphoto.y=ball.y-25;
    SDL_QueryTexture(ball.ballimg,NULL,NULL,&ball.ballphoto.w,&ball.ballphoto.h);
    SDL_RenderCopy(m_renderer,ball.ballimg,NULL,&ball.ballphoto);
}

void drawballg(struct toopg &ballg){
    SDL_DestroyTexture(ballg.ballimg);
    switch(ballg.photo){
        case 0:
            ballg.ballimg = IMG_LoadTexture(m_renderer, "ball_classicg.png");
            break;
        case 1:
            ballg.ballimg = IMG_LoadTexture(m_renderer, "ball_volleyballg.png");
            break;
        case 2:
            ballg.ballimg = IMG_LoadTexture(m_renderer, "ball_alieng.png");
            break;
        case 3:
            ballg.ballimg = IMG_LoadTexture(m_renderer, "ball_messig.png");
            break;
        case 4:
            ballg.ballimg = IMG_LoadTexture(m_renderer, "ball_miniong.png");
            break;
    }
    ballg.ballphoto.x=ballg.x-18;
    ballg.ballphoto.y=ballg.y-18;
    SDL_QueryTexture(ballg.ballimg,NULL,NULL,&ballg.ballphoto.w,&ballg.ballphoto.h);
    SDL_RenderCopy(m_renderer,ballg.ballimg,NULL,&ballg.ballphoto);
}

void movetomiddle(player &p1, player &p2, toop &ball){
    if(p1.x>420 && p2.x >600 && ball.x>550){
        p1.x   -= p1.speedx;
        p2.x   -= p2.speedx;
        ball.x -= p1.speedx;
    }
    else{
        gamestat="choose2";
    }
}

void movetodefault(player &p1, player &p2, toop &ball){
    if(p1.x<780 && p2.x <960 && ball.x<910){
        p1.x   += 2*p1.speedx;
        p2.x   += 2*p2.speedx;
        ball.x += 2*p1.speedx;
    }
}

void movetofield(player &p1, player &p2, toop &ball){
    if(p1.x<450 && p2.x >570){
        p1.x   += 1;
        p2.x   -= 1;
        p1.y   -= 1;
        p2.y   -= 1;
        ball.y -= 1;
    }
    else{
        gamestat="game";
        pg1.team=p1.team;
        pg2.team=p2.team;
        pg1.power=p1.power;
        pg2.power=p2.power;
        ballg.photo= ball.photo;
        SDL_Delay(3000);
        gamestarttime=SDL_GetTicks();
    }
}

void drawpowerbar(playerg &pg1, playerg &pg2){

    switch (pg1.power){
            case 0:
                SDL_DestroyTexture(powerimg);
                powerimg=IMG_LoadTexture(m_renderer, "");
                break;
            case 1:
                SDL_DestroyTexture(powerimg);
                powerimg=IMG_LoadTexture(m_renderer, "power1.jpg");
                break;
            case 2:
                SDL_DestroyTexture(powerimg);
                powerimg=IMG_LoadTexture(m_renderer, "power2.jpg");
                break;
            case 3:
                SDL_DestroyTexture(powerimg);
                powerimg=IMG_LoadTexture(m_renderer, "power3.jpg");
                break;
            case 4:
                SDL_DestroyTexture(powerimg);
                powerimg=IMG_LoadTexture(m_renderer, "power4.jpg");
                break;
            case 5:
                SDL_DestroyTexture(powerimg);
                powerimg=IMG_LoadTexture(m_renderer, "power5.jpg");
                break;
            case 6:
                SDL_DestroyTexture(powerimg);
                powerimg=IMG_LoadTexture(m_renderer, "power6.jpg");
                break;
            case 7:
                SDL_DestroyTexture(powerimg);
                powerimg=IMG_LoadTexture(m_renderer, "power7.jpg");
                break;
        }
        switch (pg2.power){
            case 0:
                SDL_DestroyTexture(powerimg2);
                powerimg2=IMG_LoadTexture(m_renderer, "");
                break;
            case 1:
                SDL_DestroyTexture(powerimg2);
                powerimg2=IMG_LoadTexture(m_renderer, "power1.jpg");
                break;
            case 2:
                SDL_DestroyTexture(powerimg2);
                powerimg2=IMG_LoadTexture(m_renderer, "power2.jpg");
                break;
            case 3:
                SDL_DestroyTexture(powerimg2);
                powerimg2=IMG_LoadTexture(m_renderer, "power3.jpg");
                break;
            case 4:
                SDL_DestroyTexture(powerimg2);
                powerimg2=IMG_LoadTexture(m_renderer, "power4.jpg");
                break;
            case 5:
                SDL_DestroyTexture(powerimg2);
                powerimg2=IMG_LoadTexture(m_renderer, "power5.jpg");
                break;
            case 6:
                SDL_DestroyTexture(powerimg2);
                powerimg2=IMG_LoadTexture(m_renderer, "power6.jpg");
                break;
            case 7:
                SDL_DestroyTexture(powerimg);
                powerimg2=IMG_LoadTexture(m_renderer, "power7.jpg");
                break;
        }
        powerphoto.x=40;
        powerphoto.y=32;
        powerphoto2.x=936;
        powerphoto2.y=32;
        SDL_QueryTexture(powerimg,NULL,NULL,&powerphoto.w,&powerphoto.h);
        SDL_RenderCopy(m_renderer,powerimg,NULL,&powerphoto);
        SDL_QueryTexture(powerimg2,NULL,NULL,&powerphoto2.w,&powerphoto2.h);
        SDL_RenderCopy(m_renderer,powerimg2,NULL,&powerphoto2);
        if(pg1.power!=0 ){
            rect(m_renderer, 190, 75, 150, 10, 255, 255, 255, 0);
            if(pg1.health>=1)
            my_line( m_renderer, 190, 80, pg1.health, 0, 10, 255, 255, 255);
        }
        if(pg2.power!=0){
            rect(m_renderer, 780, 75, 150, 10, 255, 255, 255, 0);
            if(pg2.health>=1)
            my_line( m_renderer, 930, 80, pg2.health, -1* M_PI, 10, 255, 255, 255);
        }
}

void dopower1(){
    switch (pg1.power){
        case 0:
            return;
        case 1:{
            playerg pg1cr=pg1, pg1cl=pg1;
            pg1cr.x+=120;
            pg1cl.x-=120;
            drawclones(pg1cr);
            drawclones(pg1cl);
            break;
            }
        case 2:
            pg1.speedx*=1.8;
            break;
        case 3:
            pg2.awareness=false;
            break;
        case 4:
            drawtrap(trap1, 1);
            break;
        case 5:
            rect2(m_renderer, 100, 340, 20, 270, 255, 50, 50, 1, 150);
            break;
    }
    if(pg1.health>1 && pg1.power!=3 ){
        pg1.health-=0.4;
    }
    if(pg1.health>1 && pg1.power==3){
        pg1.health-=1;
    }
    if(pg1.health>1 && pg1.power==4){
        pg1.health-=0.4;
    }
    if(pg1.health<=1 && goalscore==false){
        pg1.poweru=false;
        pg1.speedx=5.150;
        pg2.awareness=true;
        pg2.poison=false;
    }
}

void dopower2(){
    switch (pg2.power){
        case 0:
            return;
        case 1:{
            playerg pg2cr=pg2, pg2cl=pg2;
            pg2cr.x+=120;
            pg2cl.x-=120;
            drawclones(pg2cr);
            drawclones(pg2cl);
            break;
        }
        case 2:
            pg2.speedx*=1.8;
            break;
        case 3:
            pg1.awareness=false;
        case 4:
            drawtrap(trap2 ,2);
            break;
        case 5:
            rect2(m_renderer, 1000, 340, 20, 270, 0, 255, 125, 1, 150);
            break;
    }
    if(pg2.health>1 ){
        pg2.health-=0.4;
    }
    if(pg2.health>1 && pg2.power==3){
        pg2.health-=1;
    }
    if(pg2.health>1 && pg2.power==4){
        pg2.health-=0.4;
    }
    if(pg2.health<=1 && goalscore==false){
        pg2.poweru=false;
        pg2.speedx=5.150;
        pg1.awareness=true;
        pg1.poison=false;
    }
}

void drawtrap(trap &trapg, int i){
    if(i==1){
        if(trapg.x<pg2.x && pg2.poison==false){
                cout<<trapg.x;
            filledEllipseRGBA(m_renderer, trapg.x, trapg.y, 15, 15, 0, 255 ,0, 100);
            trapg.x+=8;
        }
        if(trapg.x >= pg2.x){
            pg2.poison=true;
        }
    }
    if(i==2){
        if(trapg.x > pg1.x && pg1.poison==false){
                cout<<trapg.x;
            filledEllipseRGBA(m_renderer, trapg.x, trapg.y, 15, 15, 0, 255 ,0, 100);
            trapg.x-=8;
        }
        if(trapg.x <= pg1.x){
            pg1.poison=true;
        }
    }
}

void resetgame(int n){
    if(latency < lastTime - 4000){
        pg1.x=200;
        pg1.y=500;
        pg2.x=870+5*n;
        pg2.y=500;
        ballg.x=560;
        ballg.y=35;
        ballg.speedx=0;
        ballg.speedy=1;
        pg1.health=1;
        pg2.health=1;
        pg1.awareness=true;
        pg2.awareness=true;
        goalscore=false;
    }
}

//random number produce##############################################################################################################
double rand_gen() {
   // return a uniformly distributed random value
   return ( (double)(rand()) + 1. )/( (double)(RAND_MAX) + 1. );
}
double normalRandom() {
   // return a normally distributed random value
   double v1=rand_gen();
   double v2=rand_gen();
   return cos(2*3.14*v2)*sqrt(-2.*log(v1));
}
double maximum(double n[10]){
    int max=n[0];
    for(int i=0; i< 10; i++){
        if(n[i]>=max){
            max=n[i];
        }
    }
    return max;
}


//shapes ############################################################################################################################
void my_line(SDL_Renderer *Renderer, int x_1, int y_1, int L, double theta,int widht, int R, int G, int B ){

    int x_2 = x_1 + L*cos(theta);

    int y_2 = y_1 - L*sin(theta);

    thickLineRGBA(Renderer,x_1,y_1,x_2,y_2,widht,R,G,B,255);
  //  SDL_RenderPresent(Renderer);
}

void rect(SDL_Renderer *Renderer, int x,int y,int w,int h,int R, int G, int B, int fill_boolian ){

    SDL_Rect rectangle ;

    rectangle.x = x;

    rectangle.y = y;

    rectangle.w = w;

    rectangle.h = h;

    SDL_SetRenderDrawColor(Renderer, R, G, B, 255);

    if (fill_boolian==1)
        SDL_RenderFillRect(Renderer, &rectangle);

    SDL_RenderDrawRect(Renderer, &rectangle);
   // SDL_RenderPresent(Renderer);
}

void rect2(SDL_Renderer *Renderer, int x,int y,int w,int h,int R, int G, int B, int fill_boolian , int a ){

    SDL_Rect rectangle ;

    rectangle.x = x;

    rectangle.y = y;

    rectangle.w = w;

    rectangle.h = h;

    SDL_SetRenderDrawColor(Renderer, R, G, B, a);

    if (fill_boolian==1)
        SDL_RenderFillRect(Renderer, &rectangle);

    SDL_RenderDrawRect(Renderer, &rectangle);
   // SDL_RenderPresent(Renderer);
}

void ellipse(SDL_Renderer *Renderer, int x, int y, int Radius1, int Radius2, int R, int G, int B, int fill_boolian){
    if(fill_boolian==1)
        filledEllipseRGBA(Renderer,x,y,Radius1,Radius2,R,G,B,255);

    if(fill_boolian==0)
        ellipseRGBA(Renderer,x,y,Radius1,Radius2,R,G,B,255);
   // SDL_RenderPresent(Renderer);
}

void window_color(SDL_Renderer *Renderer, int R, int G, int B){
    SDL_SetRenderDrawColor( Renderer, R, G, B, 255 );
    SDL_RenderClear( Renderer );
  //SDL_RenderPresent( Renderer );
}


