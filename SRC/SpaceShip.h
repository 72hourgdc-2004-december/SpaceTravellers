
#ifndef _SPACESHIP_H_
#define _SPACESHIP_H_

class SpaceShip
{
private:
	Point				mPosition;

	SDL_Surface *		mShipSurface;
	SDL_Surface *		mFlamesSurface;

	float				mFlamesAnimationTimer;
	int					mFlamesFrame;

public:
	void Init(int x, int y);
	void Destroy();

	bool Update();
	void Draw(bool flames = false);

};

#endif
