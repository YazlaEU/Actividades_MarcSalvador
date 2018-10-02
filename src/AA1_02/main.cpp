#include <SDL.h>		// Always needs to be included for an SDL app
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <exception>
#include <iostream>
#include <string>

//Game general information
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600



int main(int, char*[]) 
{
	// --- INIT SDL ---
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
		throw "No es pot inicialitzar SDL subsystems";

	// --- WINDOW ---
	SDL_Window *m_window{ SDL_CreateWindow("SDL...", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN) };
	if (m_window == nullptr) 
		throw "No es pot inicialitzar SDL_Window";

	// --- RENDERER ---
	SDL_Renderer *m_renderer{ SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) };
	if (m_renderer == nullptr) 
		throw "No es pot inicialitzar SDL_Renderer";

	//-->SDL_Image 
	const Uint8 imgFlags{ IMG_INIT_PNG | IMG_INIT_JPG };
	if (!(IMG_Init(imgFlags) & imgFlags)) throw "Error: SDL_image init";

	//-->SDL_TTF
	if (TTF_Init() != 0) throw "No es pot inicialitzar SDL_ttf";
	//-->SDL_Mix
	const Uint8 mixFlags{ MIX_INIT_MP3 };
	// --- SPRITES ---
		//Background
		SDL_Texture* bgTexture{ IMG_LoadTexture(m_renderer, "../../res/img/bg.jpg") };
		if (bgTexture == nullptr) throw "Error: bgTexture init";
		SDL_Rect bgRect{ 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };
		//Cursor
		SDL_Texture *playerTexture{ IMG_LoadTexture(m_renderer, "../../res/img/gokukintoun.png") };
		if (playerTexture == nullptr) throw "No s'ha pogut carregar la textura del cursor";
		SDL_Rect playerRect{ 0,0,190,270 };
		SDL_Rect playerTarget{0,0,190,270};
	//-->Animated Sprite ---

	// --- TEXT ---
		TTF_Font *font{ TTF_OpenFont("../../res/ttf/saiyan.ttf", 80) };
		if (font == nullptr) throw "No es pot inicialitzar TTF_Font";
		SDL_Surface *tmpSurf{ TTF_RenderText_Blended(font, "Bagon Drall Seta", SDL_Color{255,100,0,255}) };
		if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		SDL_Texture *textTexture{ SDL_CreateTextureFromSurface(m_renderer,tmpSurf) };
		SDL_Rect textRect{ 100,50,tmpSurf->w,tmpSurf->h };
		SDL_FreeSurface(tmpSurf);

		tmpSurf = { TTF_RenderText_Blended(font, "Exit!", SDL_Color{255,100,0,255}) };
		SDL_Texture *textExit{ SDL_CreateTextureFromSurface(m_renderer,tmpSurf) };
		if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		SDL_Rect textRectExit{ 650,500,tmpSurf->w,tmpSurf->h };
		SDL_FreeSurface( tmpSurf );

		tmpSurf = { TTF_RenderText_Blended(font, "Play!", SDL_Color{255,100,0,255}) };
		SDL_Texture *textPlay{ SDL_CreateTextureFromSurface(m_renderer,tmpSurf) };
		if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		SDL_Rect textRectPlay{ 50,500,tmpSurf->w,tmpSurf->h };
		SDL_FreeSurface( tmpSurf );

		tmpSurf = { TTF_RenderText_Blended(font, "Sound Off!", SDL_Color{255,100,0,255}) };
		SDL_Texture *textSound{ SDL_CreateTextureFromSurface(m_renderer,tmpSurf) };
		if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		SDL_Rect textRectSound{ 250,500,tmpSurf->w,tmpSurf->h };
		SDL_FreeSurface(tmpSurf);

		TTF_CloseFont(font);


	// --- AUDIO ---
		Mix_Music *menuSound = { Mix_LoadMUS("../../res/au/mainTheme.mp3") };
		Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
		Mix_PlayMusic(menuSound, -1);
	// --- GAME LOOP ---
	SDL_Event event; //SDL_Event es un struct (Recordar [Go to Definition] para ver como funciona internamente)
	bool isRunning = true;
	while (isRunning) {
		// HANDLE EVENTS
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_MOUSEMOTION:
				//playerRect.x = event.motion.x - 95;
				//playerRect.y = event.motion.y - 120;
				playerTarget.x = event.motion.x - 95;
				playerTarget.y = event.motion.y - 120;
				break;
			case SDL_QUIT:		
				isRunning = false; 
				break;
			case SDL_KEYDOWN:	
				if (event.key.keysym.sym == SDLK_ESCAPE) isRunning = false; //Agafa cada cop que s'apreta un boto, pero nomes reacciona si es ESC.
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.motion.x > 650 && event.motion.y > 500) {
					//EXIT
					isRunning = false;
					break;
				}
				if (event.motion.x < 100 && event.motion.y > 500) {
					//PLAY
				}
				if (event.motion.x > 250 && event.motion.x < 500 && event.motion.y > 500) {
					//SOUNDOFF
					Mix_Pause(-1);
				}
			default:;
			}
		}

		// UPDATE
		playerRect.x += (playerTarget.x - playerRect.x) / 10;
		playerRect.y += (playerTarget.y - playerRect.y) / 10;

		// DRAW
		SDL_RenderClear(m_renderer); //Neteja el buffer
			//Background
			SDL_RenderCopy(m_renderer, bgTexture, nullptr, &bgRect); //Introdueix a el buffer
			//Cursor
			SDL_RenderCopy(m_renderer, playerTexture, nullptr, &playerRect);
			//Text
			SDL_RenderCopy(m_renderer, textTexture, nullptr, &textRect);
			SDL_RenderCopy(m_renderer, textExit, nullptr, &textRectExit);
			SDL_RenderCopy(m_renderer, textPlay, nullptr, &textRectPlay);
			SDL_RenderCopy(m_renderer, textSound, nullptr, &textRectSound);
		SDL_RenderPresent(m_renderer);

	}

	// --- DESTROY ---
	SDL_DestroyTexture(bgTexture);
	SDL_DestroyTexture(playerTexture);
	SDL_DestroyTexture(textTexture);
	Mix_CloseAudio();
	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);

	// --- QUIT ---
	
	SDL_Quit();

	return 0;
}