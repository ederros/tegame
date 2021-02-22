#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <sstream>
#include <SDL.h>
#include <SDL_image.h>
#include <ctime>

using namespace std;

SDL_Texture* create_textureFromPng(SDL_Renderer* ren, const char *name) {
	SDL_Surface* peps = IMG_Load(name);
	SDL_Texture* peps_tex = SDL_CreateTextureFromSurface(ren, peps);
	SDL_FreeSurface(peps);
	return peps_tex;
}

class parts
{
private:
	int i;
public:
	bool *lethal;
	int kol;
	float* hp, *max_hp;
	string* names;
	void init(int k) {
		kol = k;
		lethal = new bool[kol];
		hp = new float[kol];
		max_hp = new float[kol];
		names = new string[kol];
	};

};

class humanoid
{
private:
	int i;
public:
	SDL_Texture* armor= NULL, *weapon= NULL;
	int body, head, l_arm, r_arm, l_leg, r_leg;
};

int main(int argc,char *argv[]) {
	int i,i2,fps=1000/60;
	string gamemode;
	stringstream out;
	bool game = 1;
	//float scaledscreen=1.25;
	//cout << "input scren scale(default 1):"; cin >> scaledscreen;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	int window_width = GetSystemMetrics(SM_CXSCREEN), window_height = GetSystemMetrics(SM_CYSCREEN);//параметры окна
	SDL_Window* win = SDL_CreateWindow("game", 0, 0, window_width, window_height, SDL_WINDOW_OPENGL/*||SDL_WINDOW_FULLSCREEN_DESKTOP*/);
	SDL_GetWindowSize(win, &window_width, &window_height);
	SDL_GLContext glContext = SDL_GL_CreateContext(win);

	SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	float center[] = { window_width / 2, window_height / 2 }, mouse[2];
	SDL_SetRenderDrawColor(ren, 100, 0, 0, 255);
	SDL_RenderClear(ren);

	SDL_Surface* peps = IMG_Load("grass.png");
	SDL_Texture * peps_tex = SDL_CreateTextureFromSurface(ren, peps);
	SDL_FreeSurface(peps);


	SDL_Event event;
	bool key[340];
	int newtime=time(0),lasttime,e=0;
	float pos[] = {0,0},speed = 2;
	for (i = 0; i < 340; i++)
	{
		key[i] = false;
	}
	i = 1;

	SDL_Texture *grass = create_textureFromPng(ren,"grass.png");
	SDL_Texture* black = create_textureFromPng(ren, "blackbody.png");
	SDL_Rect grass_block;
	SDL_Rect block32;
	block32.w = 32;
	block32.h = 32;
	block32.x = 0;
	block32.y = 0;
	
	SDL_Rect map;
	map.h = 320;
	map.w = 320;
	map.x = map.w / 2;
	map.y = map.h / 2;

	SDL_Rect rect;
	rect.w = 160;
	rect.h = 160;
	rect.x = window_width/2-rect.w/2;
	rect.y = window_height/2-rect.h/2;

	int tick = 0;

	int trigy = 0;
	while (game) 
	{
		SDL_RenderClear(ren);
		while (SDL_PollEvent(&event) != 0)
		{
			if(event.type == SDL_KEYDOWN)
			{
				key[event.key.keysym.sym%340] = true;
				
			}
			if (event.type == SDL_KEYUP)
			{
				key[event.key.keysym.sym%340] = false;

			}
			if (event.type == SDL_MOUSEMOTION)
			{
				mouse[0] = event.motion.x;
				mouse[1] = event.motion.y;
			}
		}
		

		if (key[SDLK_w])
		{
			pos[1]+= speed;
			block32.x = 32;
		}
		if (key[SDLK_s])
		{
			pos[1] -=speed;
			block32.x = 0;
		}
		if (key[SDLK_a])
		{
			pos[0]+=speed;
			block32.x = 96;
		}
		if (key[SDLK_d])
		{
			pos[0]-=speed;
			block32.x = 64;
		}
		
		if (key[SDLK_ESCAPE]) {
			game = 0;
		}
		if (key[SDLK_LSHIFT%340])
		{
			speed = 5;
		}
		else {
			speed = 2;
		}
		SDL_Delay(fps);

		if (!(key[SDLK_a] || key[SDLK_s] || key[SDLK_d] || key[SDLK_w])) {
			if ((window_width / 2 - mouse[0] < 0) && (-window_width / 2 + mouse[0] > abs(window_height / 2 - mouse[1]))) {
				block32.x = 64;
			}
			else if ((window_height / 2 - mouse[1] < 0) && (abs(-window_width / 2 + mouse[0]) < -window_height / 2 + mouse[1])) {
				block32.x = 0;
			}
			else if ((window_width / 2 - mouse[0] > 0) && (window_width / 2 - mouse[0] > abs(-window_height / 2 + mouse[1]))) {
				block32.x = 96;
			}
			else {
				block32.x = 32;
			}
		}
		tick++;
		
		if (tick % 5 == 0) {
			if (tick%150 < 75) { rect.h -= 1; trigy+=1; }
			if (tick%150 >= 75) { rect.h += 1; trigy-=1; }
		}

		if (tick >= 1200)
		{
			rect.h = 160;
			tick = 0;
		}


		
		rect.y = window_height * 3 / 4 - mouse[1] / 2 - 80+trigy;
		rect.x = window_width * 3 / 4 - mouse[0] / 2 - 80;
		map.y = pos[1]+window_height / 2 - mouse[1]/2;
		for (i = 0; i < 10; i++) {
			map.x = pos[0]+window_width / 2 - mouse[0]/2;
			for (i2 = 0; i2 < 10; i2++) {

				SDL_RenderCopy(ren, peps_tex, NULL, &map);
				map.x += 320;

			}
			map.y+=320;
		}
		
		SDL_RenderCopy(ren, black, &block32, &rect);
		SDL_RenderPresent(ren);
	}

	SDL_GL_DeleteContext(glContext);
	return(0);
};