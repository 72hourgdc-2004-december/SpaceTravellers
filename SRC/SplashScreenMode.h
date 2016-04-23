
#ifndef _SPLASHSCREENMODE_H_
#define _SPLASHSCREENMODE_H_

class SplashScreenMode : public Mode
{
private:
	SDL_Surface *		mSurface;
	float				mTimeRemaining;

public:
	bool Enter();
	void Exit();

	void Update();
};

#endif
