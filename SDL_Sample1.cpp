#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include "MainLoop.h"

// 右のソリューションエクスプローラーのSDL_Sample1を右クリックしてプロパティを選択
// 「構成プロパティ」の「C++」を選択
// 「追加インクルード ディレクトリ」の
// 構成: 「Debug」「Release」の両方
// プラットフォーム: 「すべてのプラットフォーム」
// に以下の設定が必要です
// ".\lib\sdl2\sdl2\include; .\lib\sdl2\sdl2_image\include;"
// 上記の設定でSDL2関連のincludeが使えるようになる
// 
// また
// 「構成プロパティ」→「リンカー」→「システム」を選択
// 構成:「Release」のみ
// プラットフォーム: 「すべてのプラットフォーム」
// に以下の設定が必要です
// 「サブシステム」を Windows (/SUBSYSTEM:WINDOWS) に変更。
// するとReleaseバージョンのみデバッグ表示が無くなる

// SDL2ライブラリのリンク指定
#if defined(_WIN64)
// 64ビット版
#pragma comment(lib, "./lib/sdl2/sdl2/lib/x64/SDL2.lib")
#pragma comment(lib, "./lib/sdl2/sdl2/lib/x64/SDL2main.lib")
#pragma comment(lib, "./lib/sdl2/sdl2_image/lib/x64/SDL2_image.lib")
#else
// 32ビット版
#pragma comment(lib, "./lib/sdl2/sdl2/lib/x86/SDL2.lib")
#pragma comment(lib, "./lib/sdl2/sdl2/lib/x86/SDL2main.lib")
#pragma comment(lib, "./lib/sdl2/sdl2_image/lib/x86/SDL2_image.lib")
#endif

// メインプログラム
int main(int argc, char* argv[])
{
	try
	{
		// プログラム初期化処理
		MainLoop_Setup();
		// 前の時刻
		Uint64 beforeTime= SDL_GetPerformanceCounter();
		// 60FPSの時間間隔取得
		Uint64 intervalTime = SDL_GetPerformanceFrequency() / 60;
		bool exit = false;
		while(exit == false)
		{
			// 今の時刻
			Uint64 nowTime = SDL_GetPerformanceCounter();
			// 前の時刻から今の時刻までどれくらいの時間が経ったか
			Uint64 elapsed = nowTime - beforeTime;
			// intervalTime以上の時間が経っていたら処理を実行する(だいたい60FPSになるはず)
			if(elapsed > intervalTime)
			{
				// メインループ実行、exitがtrueならプログラム終了、falseならプログラム継続
				exit = MainLoop_Loop();
				// SDLのイベント処理
				SDL_Event event;
				while(SDL_PollEvent(&event))
				{
					switch(event.type)
					{
					// Windowの×ボタンを押したら終了する
					case SDL_QUIT:
						exit = true;
						break;
					// キーを押した
					case SDL_KEYDOWN:
						switch(event.key.keysym.sym)
						{
						case SDLK_UP:
							MainLoop_SetKey(BUTTON_UP);
							break;
						case SDLK_DOWN:
							MainLoop_SetKey(BUTTON_DOWN);
							break;
						case SDLK_LEFT:
							MainLoop_SetKey(BUTTON_LEFT);
							break;
						case SDLK_RIGHT:
							MainLoop_SetKey(BUTTON_RIGHT);
							break;
						// ESCボタンを押したら終了する
						case SDLK_ESCAPE:
							exit = true;
							break;
						}
						break;
					// キーを離した
					case SDL_KEYUP:
						switch(event.key.keysym.sym)
						{
						case SDLK_UP:
							MainLoop_ResetKey(BUTTON_UP);
							break;
						case SDLK_DOWN:
							MainLoop_ResetKey(BUTTON_DOWN);
							break;
						case SDLK_LEFT:
							MainLoop_ResetKey(BUTTON_LEFT);
							break;
						case SDLK_RIGHT:
							MainLoop_ResetKey(BUTTON_RIGHT);
							break;
						}
						break;
					}
				}
				beforeTime = nowTime;
			}
			// サウンドなどはFPS関係なく全力で処理する(今回は未実装)
			MainLoop_NoWaitLoop();
		}
		// 終了処理
		MainLoop_Finalize();
	}
	catch (const char* errorMessage)
	{
		printf(errorMessage);
	}
	return 0;
}
