#include "MainLoop.h"

static SDL_Window* window;
static SDL_Renderer* renderer;
static SDL_Surface* screenSurface;
static SDL_Texture* fighterTexture;
static SDL_Texture* bgTexture;
static SDL_GameController* gameController;
static int x = 296;
static int y = 320;
static unsigned int keyboard = 0;

// プログラム初期化処理、起動時1回呼ばれます
void MainLoop_Setup(void)
{
	// SDL初期化
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		throw "Error: SDL_Init(SDL_INIT_VIDEO)";
	}
	atexit(SDL_Quit);
	window = SDL_CreateWindow("SDL_Sample1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if(window == NULL)
	{
		throw "Error: SDL_CreateWindow";
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(renderer == NULL)
	{
		throw "Error: SDL_CreateRenderer";
	}
	// Windowサイズ設定
	SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);
	// フルスクリーン表示設定 コメントアウトを外すと画面全体表示になります
	//	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	// 実際のゲームの解像度設定
	screenSurface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, 0, 0, 0, 0);
	if(screenSurface == NULL)
	{
		throw "Error: SDL_CreateRenderer";
	}
	if(SDL_InitSubSystem(SDL_INIT_JOYSTICK) != 0)
	{
		throw "Error: SDL_Init(SDL_INIT_JOYSTICK)";
	}
	// 1Pのジョイパッド初期化
	int numJoysticks = SDL_NumJoysticks();
	if((numJoysticks > 0) && SDL_IsGameController(0))
	{
		gameController = SDL_GameControllerOpen(0);
	}
	// 自機png画像読み込み
	SDL_Surface* surface = IMG_Load("fighter1.png");
	// 読み込んだ自機png画像からテクスチャ作成、GPUメモリに転送する
	fighterTexture = SDL_CreateTextureFromSurface(renderer, surface);
	// 自機png画像はGPUメモリに転送されたので破棄する
	SDL_FreeSurface(surface);

	// 背景png画像読み込み
	surface = IMG_Load("bg103.png");
	// 読み込んだ背景png画像からテクスチャ作成、GPUメモリに転送する
	bgTexture = SDL_CreateTextureFromSurface(renderer, surface);
	// 背景png画像はGPUメモリに転送されたので破棄する
	SDL_FreeSurface(surface);
}

// フレーム処理、毎フレーム呼ばれます
bool MainLoop_Loop(void)
{
	// ジョイパッド
	unsigned int joypad = 0;
	if(SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_DPAD_UP) == 1)
	{
		joypad |= BUTTON_UP;
	}
	if(SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_DPAD_DOWN) == 1)
	{
		joypad |= BUTTON_DOWN;
	}
	if(SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_DPAD_LEFT) == 1)
	{
		joypad |= BUTTON_LEFT;
	}
	if(SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == 1)
	{
		joypad |= BUTTON_RIGHT;
	}
	// 押しているボタン (キーボードとジョイパッドの押した結果or合成)
	unsigned int button = joypad | keyboard;
	// 自機移動
	if(button & BUTTON_UP)
	{
		y -= 4;
		if(y < 0)
		{
			y = 0;
		}
	}
	if(button & BUTTON_DOWN)
	{
		y += 4;
		if(y > 480 - 48)
		{
			y = 480 - 48;
		}
	}
	if(button & BUTTON_LEFT)
	{
		x -= 4;
		if(x < 0)
		{
			x = 0;
		}
	}
	if(button & BUTTON_RIGHT)
	{
		x += 4;
		if(x > 640 - 48)
		{
			x = 640 - 48;
		}
	}
	// 画面消去
	SDL_RenderClear(renderer);
	// 背景を画面全体に表示
	SDL_RenderCopy(renderer, bgTexture, NULL, NULL);
	// 自機を表示 (48x48、透明部分は画像側のアルファチャネルが有効になる)
	SDL_Rect source = {
		48 * 2, 0, 48, 48
	};
	SDL_Rect destination = {
		x, y, 48, 48
	};
	SDL_RenderCopy(renderer, fighterTexture, &source, &destination);
	// 画面更新
	SDL_RenderPresent(renderer);
	return false;
}

// 全力処理
void MainLoop_NoWaitLoop(void)
{
	// サウンド未実装
}

// 終了処理
void MainLoop_Finalize(void)
{
	// プログラム終了
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

// 押されたキーを設定
void MainLoop_SetKey(unsigned int key)
{
	keyboard |= key;
}

// 離されたキーを設定
void MainLoop_ResetKey(unsigned int key)
{
	keyboard &= ~key;
}
