
#ifndef _SDL_ENGINE_H_
#define _SDL_ENGINE_H_

#include <string>
#include <vector>
#include <map>
#include <sstream>

#include <SDL.h>
#include <SDL_image.h>

#include "Logger.h"
#include "Singleton.h"
#include "Sprite.h"
#include "Point.h"
#include "Map.h"

#include "GUI/manager.h"
#include "GUI/checkbox.h"
#include "GUI/window.h"
#include "GUI/console.h"
#include "GUI/list.h"
#include "GUI/label.h"
#include "GUI/textbox.h"

#include "SpaceMan.h"
#include "SpaceShip.h"

#include "ModeManager.h"
#include "SplashScreenMode.h"
#include "EditorMode.h"
#include "MainMenuMode.h"
#include "GameMode.h"

#include "SDL_gfxPrimitives.h"
#include "SDL_rotozoom.h"

#ifdef WIN32
#	include "SDL_syswm.h"
#	include <windows.h>
#	include <commdlg.h>
#	pragma comment (lib, "comdlg32.lib")
#endif

const SDL_Color ColourRed		= {150, 0, 0, 0};
const SDL_Color ColourGreen		= {0, 255, 0, 0};
const SDL_Color ColourBlue		= {0, 0, 255, 0};
const SDL_Color ColourYellow	= {255, 255, 0, 0};
const SDL_Color ColourBlack		= {0, 0, 0, 0};
const SDL_Color ColourWhite		= {255, 255, 255, 0};

const int NORTH = 0;
const int EAST = 1;
const int SOUTH = 2;
const int WEST = 3;

class SDL_Engine : public Singleton < SDL_Engine >
{
private:
	SDL_Surface *		mScreenSurface;

	Point				mMousePos;
	Point				mMouseDelta;
	bool				mLeftMouseDown;
	bool				mLeftMouseDownThisFrame;
	bool				mRightMouseDown;
	bool				mRightMouseDownThisFrame;

	bool				mKeysPressedThisFrame[512];
	bool				mKeysPressed[512];

	unsigned int		mLastTime;
	unsigned int		mCurrentTime;
	unsigned int		mElapsedFrameTime;
	unsigned int		mFrameCounter;
	float				mElapsedTime;
	int					mFPS;

#ifdef WIN32
	HWND				mHandle;
#endif

public:
	SDL_Engine();

	bool Create(int width, int height, bool fullscreen);
	bool Destroy();

	void BeginFrame();
	void EndFrame();

	bool ProcessEvents();

	int GetWidth()					{ return mScreenSurface->w; }
	int GetHeight()					{ return mScreenSurface->h; }

	SDL_Surface * GetBackBuffer()	{ return mScreenSurface; }

	float GetElapsedTime()			{ return mElapsedTime; }

	bool IsKeyPressed(int keyCode)	{ return mKeysPressedThisFrame[keyCode]; }
	bool IsKeyHeld(int keyCode)		{ return mKeysPressed[keyCode]; }

	bool IsLeftMouseDown()			{ return mLeftMouseDownThisFrame; }
	bool IsLeftMouseHeld()			{ return mLeftMouseDown; }

	bool IsRightMouseDown()			{ return mRightMouseDownThisFrame; }
	bool IsRightMouseHeld()			{ return mRightMouseDown; }

	Point GetMousePos()				{ return mMousePos; }
	Point GetMouseDelta()			{ return mMouseDelta; }

	HWND GetWin32Handle()			{ return mHandle; }

};

#define Engine SDL_Engine::GetInstance()

#endif
