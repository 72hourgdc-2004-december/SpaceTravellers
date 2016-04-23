
#pragma comment (lib, "SDL.lib")
#pragma comment (lib, "SDLmain.lib")
#pragma comment (lib, "SDL_image.lib")
#pragma comment (lib, "SDL_ttf.lib")

#include "Engine.h"

SDL_Engine::SDL_Engine()
{
	mScreenSurface = NULL;

	mLeftMouseDown = false;
	mLeftMouseDownThisFrame = false;
	mRightMouseDown = false;
	mRightMouseDownThisFrame = false;

	mLastTime = 0;
	mCurrentTime = 0;
	mElapsedFrameTime = 0;
	mFrameCounter = 0;
	mFPS = 0;

	for (int i = 0 ; i < 512; i++)
	{
		mKeysPressedThisFrame[i] = false;
		mKeysPressed[i] = false;
	}
}

bool SDL_Engine::Create(int width, int height, bool fullscreen)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		return false;
	}

	if (fullscreen)
	{
		mScreenSurface = SDL_SetVideoMode(width, height, 16, SDL_SWSURFACE);
	}
	else
	{
		mScreenSurface = SDL_SetVideoMode(width, height, 0, SDL_ANYFORMAT);
	}

	mLastTime = SDL_GetTicks();

	SDL_SysWMinfo info;
	SDL_VERSION(&info.version);

	if (SDL_GetWMInfo (&info))
	{
		mHandle = info.window;
	}

	return true;
}

bool SDL_Engine::Destroy()
{
	SDL_Quit();

	return true;
}

void SDL_Engine::BeginFrame()
{
	mCurrentTime = SDL_GetTicks();
	unsigned int ElapsedTime = mCurrentTime - mLastTime;
	mElapsedTime = (float)((double)ElapsedTime/1000.0f);
	mElapsedFrameTime += ElapsedTime;
	if (mElapsedFrameTime > 1000)
	{
		mFPS = (int)(((double)mFrameCounter)/1.0f);
		mElapsedFrameTime = 0;
		mFrameCounter = 0;

#ifdef WIN32
		char buf[64];
		sprintf(buf, "FPS: %d", mFPS);

		SetWindowText(mHandle, buf);
#endif
	}
	mFrameCounter++;
	mLastTime = mCurrentTime;

	// Mouse not clicked yet this frame
	mLeftMouseDownThisFrame = false;
	mRightMouseDownThisFrame = false;

	// No keys pressed yet this frame
	for (int i = 0; i < 512; i++)
	{
		mKeysPressedThisFrame[i] = false;
	}

	// Clear the screen
	SDL_FillRect(mScreenSurface, NULL, 0);
}

void SDL_Engine::EndFrame()
{
#ifdef _DEBUG
#endif
	while (SDL_Flip(mScreenSurface) == -1);
}

bool SDL_Engine::ProcessEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			Modes.QuitNow();
			return false;

		case SDL_MOUSEMOTION:
			mMousePos = Point(event.motion.x, event.motion.y);
			mMouseDelta = Point(event.motion.xrel, event.motion.yrel);
			break;

		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				if (mLeftMouseDown == false)
				{
					mLeftMouseDown = true;
					mLeftMouseDownThisFrame = true;
				}
			}
			if (event.button.button == SDL_BUTTON_RIGHT)
			{
				if (mRightMouseDown == false)
				{
					mRightMouseDown = true;
					mRightMouseDownThisFrame = true;
				}
			}
			break;

		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				mLeftMouseDown = false;
			}
			if (event.button.button == SDL_BUTTON_RIGHT)
			{
				mRightMouseDown = false;
			}
			break;

		case SDL_KEYDOWN:
			mKeysPressedThisFrame[event.key.keysym.sym] = true;
			mKeysPressed[event.key.keysym.sym] = true;
			break;

		case SDL_KEYUP:
			mKeysPressed[event.key.keysym.sym] = false;
			break;
		}

		Modes.InputEvent(&event);
	}

	return true;
}
