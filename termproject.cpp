#include <iostream>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfx.h>
#include <string>
#include <math.h>

using namespace std;

//functions ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void initial_setup();
void window_color(SDL_Renderer *Renderer, int R, int G, int B);
void rect(SDL_Renderer *Renderer, int x,int y,int w,int h,int R, int G, int B, int fill_boolian );
void ellipse(SDL_Renderer *Renderer, int x, int y, int Radius1, int Radius2, int R, int G, int B, int fill_boolian);
void my_line(SDL_Renderer *Renderer, int x_1, int y_1, int L,double theta,int widht, int R, int G, int B );
void update();
void input();
void render();
void endcheck();
void drawplayer(struct player);

// declarations ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
SDL_Window * m_window;
SDL_Renderer *m_renderer;
bool running=true;
int scorep1=0, scporep2=0;
#define WIDTH 1120
#define HIGHT 700
struct player{
    int x;
    int y;
    string power;
};
player p1, p2;

int lastFrame , lastTime = 0 , frameCount, timerFPS;

//main :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
int main( int argc, char * argv[] ){

    //cout<<"setup window and renderer"<<endl;
    initial_setup();

    //cout<<"run the game"<<endl;
    while(running){

        cout<<"set fps\n";
        lastFrame = SDL_GetTicks();
        if(lastFrame >= lastTime + 1000){
            lastTime = lastFrame;
            frameCount = 0;
        }

        //cout<<"logic of the game\n";
        update();

        //cout<<"actions taken by the players\n";
        input();

        //cout<<"show the new frame\n";
        render();

        //cout<<"check if the game has ended\n";
        endcheck();
    }
    //cout<<"game over!\n";
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
    return 0;
}

//functions ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void initial_setup(){
    //cout<<"entered initial setup!"<<endl;
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(WIDTH, HIGHT, 0, &m_window, &m_renderer);
    p1.x=560;
    p1.y=450;
    p2.x=760;
    p2.y=450;

}

void input(){
    //cout<<"entered input!"<<endl;
}

void update(){
    //cout<<"entered update!"<<endl;
}

void render(){
    //cout<<"entered render!"<<endl;
    //clear renderer
    SDL_RenderClear(m_renderer);

    //set fps
    frameCount++;
    timerFPS = SDL_GetTicks() - lastFrame;
    if(timerFPS < (1000/60))
    {
        SDL_Delay((1000/60) - timerFPS);
    }
    //draw background
    window_color(m_renderer, 0, 255 ,0);

    //draw players
    drawplayer(p1);
    drawplayer(p2);

    //show renderer
    SDL_RenderPresent(m_renderer);
}

void endcheck(){
    //cout<<"entered endcheck!\n";
}


//shapes ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
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

void drawplayer(struct player player){
    //body
    rect(m_renderer, player.x, player.y, 100, 100,  255, 50, 50, 1);
    //feet
    rect(m_renderer, player.x, player.y+100, 100, 70,  255, 195, 170, 1);
    rect(m_renderer, player.x, player.y+170, 100, 30,  101, 67, 33, 1);
    thickLineRGBA(m_renderer, player.x+50, player.y+101, player.x+50, player.y+198, 2, 0, 0, 0, 200);
    //head
    ellipse(m_renderer, player.x+50, player.y-50, 50, 50, 255, 195, 170, 1);
    //face


}
