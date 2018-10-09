#include <SDL.h>		// Always needs to be included for an SDL app
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <time.h>
#include <stdlib.h>
#include <exception>
#include <iostream>
#include <string>

//Game general information
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FPS 60

enum class GameState { PLAY, MENU, EXIT };

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
		//Background Menu
		SDL_Texture* bgTexture{ IMG_LoadTexture(m_renderer, "../../res/img/bg.jpg") };
		if (bgTexture == nullptr) throw "Error: bgTexture init";
		SDL_Rect bgRect{ 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };
		//Background Game
		SDL_Texture *bgGameTexture{ IMG_LoadTexture(m_renderer, "../../res/img/bgCastle.jpg") };
		if (bgGameTexture == nullptr) throw "Error: bgGameTexture init";
		SDL_Rect bgGameRect{ 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };
		//Cursor
		SDL_Texture *playerTexture{ IMG_LoadTexture(m_renderer, "../../res/img/kintoun.png") };
		if (playerTexture == nullptr) throw "Error: Cursos Image can't be loaded";
		SDL_Rect playerRect{ 0,0,160,80 };
		SDL_Rect playerTarget{0,0,100,100};
		//Coin Bags
		SDL_Texture *goldTexture{ IMG_LoadTexture(m_renderer, "../../res/img/gold.png") };
		if (goldTexture == nullptr) throw "Error: Gold Image can't be loaded";
		SDL_Rect goldRect1{ rand() % 500 + 10, rand() % 550 + 150, 50, 37 };
		SDL_Rect goldRect2{ 0,0,50,37 };
		SDL_Rect goldRect3{ 0,0,50,37 };
		SDL_Rect goldRect4{ 0,0,50,37 };
		SDL_Rect goldRect5{ 0,0,50,37 };
	//-->Animated Sprite ---
		//Sprite Player 1
		SDL_Texture *player1Texture{ IMG_LoadTexture(m_renderer, "../../res/img/spCastle.png") };
		SDL_Rect player1Position;
		int animWidth, animHeight, frameWidth, frameHeight;
		SDL_QueryTexture(player1Texture, NULL, NULL, &animWidth, &animHeight);
		frameWidth = animWidth / 12;
		frameHeight = animHeight / 8;
		player1Position.x = 300;
		player1Position.y = 300;
		SDL_Rect player1Rect{ 4 * frameWidth, frameHeight, 0, 0 };
		player1Position.w = frameWidth * 2;
		player1Position.h = frameHeight * 2;
		player1Rect.w = frameWidth;
		player1Rect.h = frameHeight;
		int frameCounter = 0;
		//Sprite Player 2
		SDL_Texture *player2Texture{ IMG_LoadTexture(m_renderer, "../../res/img/spCastle.png") };
		SDL_Rect player2Position;
		int animWidth2, animHeight2, frameWidth2, frameHeight2;
		SDL_QueryTexture(player2Texture, NULL, NULL, &animWidth, &animHeight);
		frameWidth = animWidth / 12;
		frameHeight = animHeight / 8;
		player2Position.x = 500;
		player2Position.y = 300;
		SDL_Rect player2Rect{ 0 * frameWidth, frameHeight, 0, 0 };
		player2Position.w = frameWidth * 2;
		player2Position.h = frameHeight * 2;
		player2Rect.w = frameWidth;
		player2Rect.h = frameHeight;
		int frameCounter2 = 0;
		//COUNTDOWN SPRITES
		/*
		SDL_Texture *countdownTexture{ IMG_LoadTexture(m_renderer, "../../res/img/num.png") };
		SDL_Rect countdownPosition;
		int countWidth, countHeight, fcountWidth, fcountHeight;
		SDL_QueryTexture(countdownTexture, NULL, NULL, &countWidth, &countHeight);
		fcountWidth = countWidth / 10;
		fcountHeight = countHeight;
		countdownPosition.x = 600;
		countdownPosition.y = 50;
		SDL_Rect countdownRect{ 0 * frameWidth, frameHeight, 0, 0 };
		countdownPosition.w = fcountWidth;
		countdownPosition.h = fcountHeight;
		countdownRect.w = fcountWidth;
		countdownRect.h = fcountHeight;
		int frameCountdown = 0;
		*/

	// --- TEXT ---

		TTF_Font *font{ TTF_OpenFont("../../res/ttf/saiyan.ttf", 80) };
		if (font == nullptr) throw "No es pot inicialitzar TTF_Font";
		SDL_Surface *tmpSurf{ TTF_RenderText_Blended(font, "PLAY", SDL_Color{0,255,0,255}) };
		if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		SDL_Texture *textTexture{ SDL_CreateTextureFromSurface(m_renderer,tmpSurf) };
		SDL_Rect textRect{ SCREEN_WIDTH / 2 - tmpSurf->w / 2, 50, tmpSurf->w,tmpSurf->h };
		SDL_FreeSurface(tmpSurf);

		tmpSurf = { TTF_RenderText_Blended(font, "Exit!", SDL_Color{255,0,0,255}) };
		SDL_Texture *textExit{ SDL_CreateTextureFromSurface(m_renderer,tmpSurf) };
		if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		SDL_Rect textRectExit{ SCREEN_WIDTH / 2 - tmpSurf->w / 2 + 10, 50 + tmpSurf->h, tmpSurf->w,tmpSurf->h }; // Necesita millora
		SDL_FreeSurface( tmpSurf );
/*
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
*/
		TTF_CloseFont(font);


	// --- AUDIO ---
		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) 
		{ 
			throw "Error: SDL_Mixer Init"; 
		};
		Mix_Music *menuSound = { Mix_LoadMUS("../../res/au/mainTheme.mp3") };
		if (!menuSound) throw "Error: Can't load Soundtrack";
		Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
		Mix_PlayMusic(menuSound, -1);


	// --- TIMER ---
	
		clock_t lastTime = clock();
		float countDown = 0;
		float frameTime = 0;

	// --- GAME LOOP ---
	SDL_Event event; //SDL_Event es un struct (Recordar [Go to Definition] para ver como funciona internamente)
	bool isRunning = true;
	// --- PLAYER 1 KEYS ---
	bool player1Down = false;
	bool player1Up = false;
	bool player1Right = false;
	bool player1Left = false;
	// --- PLAYER 2 KEYS ---
	bool player2Down = false;
	bool player2Up = false;
	bool player2Right = false;
	bool player2Left = false;

	GameState gamestate = GameState::MENU;

	while (gamestate != GameState::EXIT) {
		// HANDLE EVENTS
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_MOUSEMOTION:
				playerTarget.x = event.motion.x - 80;
				playerTarget.y = event.motion.y - 40;
				break;
			case SDL_QUIT:		
				gamestate = GameState::EXIT; 
				break;
			case SDL_KEYDOWN:	
				//PLAYR 1
				if (event.key.keysym.sym == SDLK_UP) player1Up = true;
				if (event.key.keysym.sym == SDLK_DOWN) player1Down = true;
				if (event.key.keysym.sym == SDLK_RIGHT) player1Right = true;
				if (event.key.keysym.sym == SDLK_LEFT) player1Left = true;
				//PLAYER 2
				if (event.key.keysym.sym == SDLK_w) player2Up = true;
				if (event.key.keysym.sym == SDLK_s) player2Down = true;
				if (event.key.keysym.sym == SDLK_d) player2Right = true;
				if (event.key.keysym.sym == SDLK_a) player2Left = true;
				break;
			case SDL_KEYUP:
				//PLAYER 1
				if (event.key.keysym.sym == SDLK_UP) player1Up = false;
				if (event.key.keysym.sym == SDLK_DOWN) player1Down = false;
				if (event.key.keysym.sym == SDLK_RIGHT) player1Right = false;
				if (event.key.keysym.sym == SDLK_LEFT) player1Left = false;
				//PLAYER 2
				if (event.key.keysym.sym == SDLK_w) player2Up = false;
				if (event.key.keysym.sym == SDLK_s) player2Down = false;
				if (event.key.keysym.sym == SDLK_d) player2Right = false;
				if (event.key.keysym.sym == SDLK_a) player2Left = false;
			default:;
			}
		}
		switch (gamestate) {
		case GameState::PLAY: //Case per a fer el Handle del Joc
			//PLAYER 1
			if (player1Up) {
				if (player1Position.y - 5 + frameHeight / 2 >= 150) player1Position.y -= 5;
				player1Rect.y = 3 * frameHeight;
			}
			if (player1Down) {
				if (player1Position.y + 5 + frameHeight * 2 <= 600) player1Position.y += 5;
				player1Rect.y = 0 * frameHeight;
			}
			if (player1Right) {
				if (player1Position.x + frameWidth * 2 <= 800) player1Position.x += 5;
				player1Rect.y = 2 * frameHeight;
			}
			if (player1Left) {
				if (player1Position.x - 5 + frameWidth / 2 >= 0) player1Position.x -= 5;
				player1Rect.y = 1 * frameHeight;
			}
			//PLAYER 2

			if (player2Up) {
				if (player2Position.y - 5 + frameHeight / 2 >= 150) player2Position.y -= 5;
				player2Rect.y = 3 * frameHeight;
			}
			if (player2Down) {
				if (player2Position.y + 5 + frameHeight * 2 <= 600) player2Position.y += 5;
				player2Rect.y = 0 * frameHeight;
			}
			if (player2Right) {
				if (player2Position.x + frameWidth * 2 <= 800) player2Position.x += 5;
				player2Rect.y = 2 * frameHeight;
			}
			if (player2Left) {
				if (player2Position.x - 5 + frameWidth / 2 >= 0) player2Position.x -= 5;
				player2Rect.y = 1 * frameHeight;
			}
			//UPDATE PLAY

			countDown++;
			if (FPS / countDown <= 5) {
				countDown = 0;
				player1Rect.x += frameWidth;
				if (player1Rect.x >= 6 * frameWidth) player1Rect.x = 4 * frameWidth; //Update sprite position Player 1
				player2Rect.x += frameWidth;
				if (player2Rect.x >= 3 * frameWidth) player2Rect.x = 0 * frameWidth; //Update sprite position player 2
			}

			frameTime = (clock() - lastTime);
			lastTime = clock();
			frameTime /= CLOCKS_PER_SEC;
			countDown -= frameTime;
			std::cout << countDown << std::endl;

			//DRAW PLAY

			SDL_RenderClear(m_renderer);
			SDL_RenderCopy(m_renderer, bgGameTexture, nullptr, &bgGameRect);
			SDL_RenderCopy(m_renderer, player1Texture, &player1Rect, &player1Position); //Draws Player 1
			SDL_RenderCopy(m_renderer, player2Texture, &player2Rect, &player2Position); //Draws Player 2
			SDL_RenderCopy(m_renderer, goldTexture, nullptr, &goldRect1); // Draws Gold Bag 1
			//SDL_RenderCopy(m_renderer, countdownTexture, &countdownRect, &countdownPosition);
			SDL_RenderPresent(m_renderer);

		case GameState::EXIT:
			break;
		case GameState::MENU:
			if (event.button.button == SDL_BUTTON_LEFT) {
				if ((event.button.x > textRect.x) && (event.button.x < textRect.x + textRect.w) && (event.button.y > textRect.y) && (event.button.y < textRect.y + textRect.h)) {
					gamestate = GameState::PLAY;
				}
				if ((event.button.x > textRectExit.x) && (event.button.x < textRectExit.x + textRectExit.w) && (event.button.y > textRectExit.y) && (event.button.y < textRectExit.y + textRectExit.h)) {
					gamestate = GameState::EXIT;
				}
			}
			//UPDATE MENU
			playerRect.x += (playerTarget.x - playerRect.x) / 10;
			playerRect.y += (playerTarget.y - playerRect.y) / 10;

			// DRAW
			SDL_RenderClear(m_renderer); //Neteja el buffer
			// DRAW Background
			SDL_RenderCopy(m_renderer, bgTexture, nullptr, &bgRect); //Introdueix a el buffer
			// DRAW Cursor
			SDL_RenderCopy(m_renderer, playerTexture, nullptr, &playerRect);
			// DRAW Text
			SDL_RenderCopy(m_renderer, textTexture, nullptr, &textRect); //Play Button
			SDL_RenderCopy(m_renderer, textExit, nullptr, &textRectExit); //Exit Button
			SDL_RenderPresent(m_renderer);
		}


		




		

		
	}
	


	// --- DESTROY ---
	SDL_DestroyTexture(bgTexture);
	SDL_DestroyTexture(playerTexture);
	SDL_DestroyTexture(textTexture);
	SDL_DestroyTexture(player1Texture);
	SDL_DestroyTexture(goldTexture);
	SDL_DestroyTexture(bgGameTexture);
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	Mix_CloseAudio();
	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();



	// --- QUIT ---
	
	SDL_Quit();

	return 0;
}