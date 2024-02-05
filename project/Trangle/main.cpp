#include <bits/stdc++.h>
// #include <graphics.h>
// #include <gdiplus/gdiplusgraphics.h>
// #include <windows.h>
#include <ctime>
#include "three_D.h"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#undef main
threeD_object obj[5000];
using namespace std;
double tx,ty,tz;
double si=90;

SDL_Window* window;
SDL_Renderer* renderer;
int initSDL()
{
	// 初始化SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL初始化失败: %s\n", SDL_GetError());
		return 1;
	}

	// 创建窗口
	window = SDL_CreateWindow("SDL Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1600, 1000, SDL_WINDOW_SHOWN);
	if (!window) {
		printf("窗口创建失败: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}
	// 创建渲染器
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		printf("渲染器创建失败: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}
	// 清空屏幕为白色
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
	int i;
	for(i = 0;i<1000;i++){
		SDL_RenderDrawPoint(renderer, i/0.01f, sin(i/0.1f)*10+50);
	}
	SDL_RenderDrawLine(renderer, 0, 0, 100, 100);
	SDL_RenderDrawLine(renderer, 0, 0, 100, 200);
	// 更新屏幕
	SDL_RenderPresent(renderer);
	return 0;
}
int main(){
	initSDL();
	srand((unsigned int)time(NULL));
	// initwindow(1600,1000);
	// ege_enable_aa(true);
	setcamerasize(si);
	//double xp=rand()%3+2,yp=xp=rand()%3+2,zp=xp=rand()%3+2;
	/*for(double x=-10;x<=10;x+=1){
		obj[2]=newpoint(obj[2],x,sqrt(10*10-x*x),0);
		obj[2]=newpoint(obj[2],x,-sqrt(10*10-x*x),0);
	}
	obj[2]=newpoint(obj[2],0,0,30);*/
	int g0=1;
	for(int x=-10;x<=10;x++){
		for(int y=-10;y<=10;y++){
			obj[g0]=newpoint(obj[g0],x,y,-1);
			obj[g0]=newpoint(obj[g0],x,y+1,-1);
			obj[g0]=newpoint(obj[g0],x+1,y+1,-1);
			obj[g0]=newpoint(obj[g0],x+1,y,-1);
			g0++;
		}
	}
	printf("g0 %d\n", g0);
	int key_code;
	int s=0;
	int mr=rand(),mg=rand(),mb=rand(),r=rand(),g=rand(),b=rand();
	// 主循环处理事件
	SDL_Event event;
	bool quit = false;
	SDL_MouseButtonEvent *e0;
	while(!quit){
		// while (!quit) {
		// printf("loop\n");
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) {
				quit = true;
			}
			// printf("event %d\n", event.type);
			switch (event.type) {
				case SDL_MOUSEMOTION:
					// 处理鼠标移动事件
					break;
				case SDL_MOUSEBUTTONDOWN:
					// 处理鼠标按下事件
					printf("mouse down %d %d\n", event.motion.x, event.motion.y);
					break;
				case SDL_MOUSEBUTTONUP:
					printf("mouse up\n");
					// g_touch_button = 0;
					// 处理鼠标释放事件
					break;
				case SDL_KEYDOWN:
					// 按下键盘按键
					printf("Key pressed: %d\n", event.key.keysym.sym);
					key_code = event.key.keysym.sym;
					break;
				case SDL_KEYUP:
					// 松开键盘按键
					printf("Key released: %d\n", event.key.keysym.sym);
					break;
				default:
					break;
			}
		}
		// }
		if(quit)
			break;
		SDL_RenderPresent(renderer);






		setcamerasize(si);
		srand((unsigned int)time(NULL));
		s++;
		//t+=0.001;
		// delay_fps(60);
		if(s%10==0){
			if(r==mr)
				mr=rand();
			if(mr>r) r++;
			if(mr<r) r--;
			if(g==mg)
				mg=rand();
			if(mg>g) g++;
			if(mg<g) g--;
			if(b==mb)
				mb=rand();
			if(mb>b) b++;
			if(mb<b) b--;
		}
		obj[0]=getcube(1,0,0,0);
		// if(kbhit()){
		// 	key_code=getch();
		// }
		
		tx=0;ty=0;tz=0;
		double x,y,z;
		if(key_code=='w') ty=0.1;
		if(key_code=='s') ty=-0.1;
		if(key_code=='q') tz=-0.1;
		if(key_code=='e') tz=0.1;
		if(key_code=='a') tx=0.1;
		if(key_code=='d') tx=-0.1;
		if(key_code=='h') txy-=asin(1)*2/200;
		if(key_code=='k') txy+=asin(1)*2/200;
		if(key_code=='u') tyz-=asin(1)*2/200;
		if(key_code=='j') tyz+=asin(1)*2/200;
		if(key_code=='y') txz-=asin(1)*2/200;
		if(key_code=='i') txz+=asin(1)*2/200;
		if(key_code=='x') si++;
		if(key_code=='c') si--;
		key_code='p';
		
		x=tx;
		y=ty;
		tx=x*cos(txy)-y*sin(txy);ty=x*sin(txy)+y*cos(txy);
		xp+=tx;
		yp+=ty;
		zp+=tz;
		// delay(3);
		// setfillcolor(BLACK);
		// bar(0,0,1600,1000);
		// setcolor(WHITE);
		// 清空屏幕为白色
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
		for(int m=1;m<g0;m++){
			putobj(obj[m]);
		}
		// setcolor(RED);
		//setcolor(EGEARGB(0x50, 0x00, 0x20, 0xFF));
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
		putobj(obj[0]);
	}

	// 释放资源
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

}
