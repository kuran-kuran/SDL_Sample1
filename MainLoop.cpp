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

// �v���O���������������A�N����1��Ă΂�܂�
void MainLoop_Setup(void)
{
	// SDL������
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
	// Window�T�C�Y�ݒ�
	SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);
	// �t���X�N���[���\���ݒ� �R�����g�A�E�g���O���Ɖ�ʑS�̕\���ɂȂ�܂�
	//	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	// ���ۂ̃Q�[���̉𑜓x�ݒ�
	screenSurface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, 0, 0, 0, 0);
	if(screenSurface == NULL)
	{
		throw "Error: SDL_CreateRenderer";
	}
	if(SDL_InitSubSystem(SDL_INIT_JOYSTICK) != 0)
	{
		throw "Error: SDL_Init(SDL_INIT_JOYSTICK)";
	}
	// 1P�̃W���C�p�b�h������
	int numJoysticks = SDL_NumJoysticks();
	if((numJoysticks > 0) && SDL_IsGameController(0))
	{
		gameController = SDL_GameControllerOpen(0);
	}
	// ���@png�摜�ǂݍ���
	SDL_Surface* surface = IMG_Load("fighter1.png");
	// �ǂݍ��񂾎��@png�摜����e�N�X�`���쐬�AGPU�������ɓ]������
	fighterTexture = SDL_CreateTextureFromSurface(renderer, surface);
	// ���@png�摜��GPU�������ɓ]�����ꂽ�̂Ŕj������
	SDL_FreeSurface(surface);

	// �w�ipng�摜�ǂݍ���
	surface = IMG_Load("bg103.png");
	// �ǂݍ��񂾔w�ipng�摜����e�N�X�`���쐬�AGPU�������ɓ]������
	bgTexture = SDL_CreateTextureFromSurface(renderer, surface);
	// �w�ipng�摜��GPU�������ɓ]�����ꂽ�̂Ŕj������
	SDL_FreeSurface(surface);
}

// �t���[�������A���t���[���Ă΂�܂�
bool MainLoop_Loop(void)
{
	// �W���C�p�b�h
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
	// �����Ă���{�^�� (�L�[�{�[�h�ƃW���C�p�b�h�̉���������or����)
	unsigned int button = joypad | keyboard;
	// ���@�ړ�
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
	// ��ʏ���
	SDL_RenderClear(renderer);
	// �w�i����ʑS�̂ɕ\��
	SDL_RenderCopy(renderer, bgTexture, NULL, NULL);
	// ���@��\�� (48x48�A���������͉摜���̃A���t�@�`���l�����L���ɂȂ�)
	SDL_Rect source = {
		48 * 2, 0, 48, 48
	};
	SDL_Rect destination = {
		x, y, 48, 48
	};
	SDL_RenderCopy(renderer, fighterTexture, &source, &destination);
	// ��ʍX�V
	SDL_RenderPresent(renderer);
	return false;
}

// �S�͏���
void MainLoop_NoWaitLoop(void)
{
	// �T�E���h������
}

// �I������
void MainLoop_Finalize(void)
{
	// �v���O�����I��
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

// �����ꂽ�L�[��ݒ�
void MainLoop_SetKey(unsigned int key)
{
	keyboard |= key;
}

// �����ꂽ�L�[��ݒ�
void MainLoop_ResetKey(unsigned int key)
{
	keyboard &= ~key;
}
