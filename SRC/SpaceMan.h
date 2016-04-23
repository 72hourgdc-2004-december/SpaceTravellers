
#ifndef _SPACEMAN_H_
#define _SPACEMAN_H_


class SpaceMan
{
public:
	enum STATE
	{
		NORMAL,
		HOME,
		DYING,
		DEAD,
	};

private:
	Map *				mMap;

	STATE				mState;

	int					mTileX;		// These are the tile you are moving onto (not the tile you are on)
	int					mTileY;

	Point				mSourcePos;
	Point				mDestinationPos;
	float				mMovementDelta;

	int					mFacing;

	SDL_Surface *		mSurface;

	float				mDyingAnimationTimer;
	int					mDyingAnimationFrame;

public:

	void Init(int x, int y, Map * map);
	void Destroy();

	int Update();
	void Draw();

};

#endif
