
#include "Engine.h"

void SpaceShip::Init(int x, int y)
{
	mPosition = Point(x, y);

	mShipSurface = IMG_Load("spaceship.bmp");
	SDL_SetColorKey(mShipSurface, SDL_SRCCOLORKEY, SDL_MapRGB(mShipSurface->format, 255, 0, 255));
	
	mFlamesSurface = IMG_Load("flames.bmp");
	SDL_SetColorKey(mFlamesSurface, SDL_SRCCOLORKEY, SDL_MapRGB(mFlamesSurface->format, 255, 0, 255));

	mFlamesAnimationTimer = 0.0f;
	mFlamesFrame = 0;
}

void SpaceShip::Destroy()
{
	SDL_FreeSurface(mShipSurface);
	mShipSurface = NULL;

	SDL_FreeSurface(mFlamesSurface);
	mFlamesSurface = NULL;
}

bool SpaceShip::Update()
{
	int speed = 200;

	int dy = (int)(speed * Engine.GetElapsedTime());

	mPosition.y -= dy;

	mFlamesAnimationTimer += Engine.GetElapsedTime();
	if (mFlamesAnimationTimer >= 0.25f)
	{
		mFlamesAnimationTimer = 0.0f;
		mFlamesFrame++;
		if (mFlamesFrame == 2) mFlamesFrame = 0;
	}

	if (mPosition.y + 48 < 0) return true;
	return false;
}

void SpaceShip::Draw(bool flames)
{
	SDL_Rect DestRect = { mPosition.x, mPosition.y, 0, 0 };

	SDL_BlitSurface(mShipSurface, NULL, Engine.GetBackBuffer(), &DestRect);

	if (flames)
	{
		SDL_Rect DestRect = { mPosition.x, mPosition.y+32, 0, 0 };

		SDL_Rect SrcRect = { mFlamesFrame * 32, 0, 32, 16 };

		SDL_BlitSurface(mFlamesSurface, &SrcRect, Engine.GetBackBuffer(), &DestRect);
	}
}
