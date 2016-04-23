
#include "Engine.h"

bool SplashScreenMode::Enter()
{
	mSurface = IMG_Load("STtitle.jpg");

	mTimeRemaining = 2.0f;

	return true;
}

void SplashScreenMode::Exit()
{
	if (mSurface)
	{
		SDL_FreeSurface(mSurface);
		mSurface = NULL;
	}
}

void SplashScreenMode::Update()
{
	mTimeRemaining -= Engine.GetElapsedTime();

	if (mTimeRemaining <= 0.0f)
	{
		Modes.RequestMode("MainMenu");
	}

	SDL_BlitSurface(mSurface, NULL, Engine.GetBackBuffer(), NULL);
}
