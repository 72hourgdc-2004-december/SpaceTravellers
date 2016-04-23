
#include "Engine.h"

void SpaceMan::Init(int x, int y, Map * map)
{
	mState = NORMAL;
	mMovementDelta = 0.0f;

	mDyingAnimationTimer = 0.0f;
	mDyingAnimationFrame = 0;

	mMap = map;

	mFacing = EAST;
	
	mSourcePos.x = x * mMap->GetTileSize();
	mSourcePos.y = y * mMap->GetTileSize();

	mDestinationPos.x = (x+1) * mMap->GetTileSize();
	mDestinationPos.y = y * mMap->GetTileSize();

	mTileX = x + 1;
	mTileY = y;

	mSurface = IMG_Load("spaceman.bmp");

	SDL_SetColorKey(mSurface, SDL_SRCCOLORKEY, SDL_MapRGB(mSurface->format, 255, 0, 255));
}

void SpaceMan::Destroy()
{
	SDL_FreeSurface(mSurface);
	mSurface = NULL;
}

int SpaceMan::Update()
{
	if (mState == DYING)
	{
		mDyingAnimationTimer += Engine.GetElapsedTime();
		if (mDyingAnimationTimer > 0.2f)
		{
			mDyingAnimationTimer = 0.0f;
			mDyingAnimationFrame++;
			if (mDyingAnimationFrame == 8)
			{
				return DEAD;
			}
		}

		return NORMAL;
	}

	if (mState != NORMAL)
	{
		// only the "normal" state needs processing
		return mState;
	}

	mMovementDelta += Engine.GetElapsedTime();

	if (mMovementDelta >= 1.0f)
	{
		mMovementDelta = 0.0f;

		// we have arrived at the destination square, now what?
		Tile & tile = mMap->TileAt(mTileX, mTileY);

		if (tile.mAI == 0)
		{
			// blank tile
		}
		else if (tile.mAI == 1)
		{
			// blocked tile, this cant happen
		}
		else if (tile.mAI == 3)
		{
			// Yay! home
			mState = HOME;
			return HOME;
		}
		else if (tile.mAI == 4)
		{
			// Uh oh, acid...
			mState = DYING;
			mFacing = 4;		// Nasty

			mSourcePos = mDestinationPos;
			mMovementDelta = 0.0f;

			return NORMAL;
		}
		else if (tile.mAI == 5)
		{
			mFacing = NORTH;
		}
		else if (tile.mAI == 6)
		{
			mFacing = EAST;
		}
		else if (tile.mAI == 7)
		{
			mFacing = SOUTH;
		}
		else if (tile.mAI == 8)
		{
			mFacing = WEST;
		}

		int tx = mTileX;
		int ty = mTileY;

		if (mFacing == NORTH)		ty--;
		if (mFacing == EAST)		tx++;
		if (mFacing == SOUTH)		ty++;
		if (mFacing == WEST)		tx--;

		Tile & destTile = mMap->TileAt(tx, ty);
		if (destTile.mAI == 1)
		{
			if (mFacing == NORTH)		mFacing = EAST;
			else if (mFacing == EAST)	mFacing = SOUTH;
			else if (mFacing == SOUTH)	mFacing = WEST;
			else if (mFacing == WEST)	mFacing = NORTH;

			// Recalulate our target tile
			tx = mTileX;
			ty = mTileY;

			if (mFacing == NORTH)		ty--;
			if (mFacing == EAST)		tx++;
			if (mFacing == SOUTH)		ty++;
			if (mFacing == WEST)		tx--;

			Tile & destTile = mMap->TileAt(tx, ty);
			if (destTile.mAI == 1)
			{
				if (mFacing == NORTH)		mFacing = EAST;
				else if (mFacing == EAST)	mFacing = SOUTH;
				else if (mFacing == SOUTH)	mFacing = WEST;
				else if (mFacing == WEST)	mFacing = NORTH;

				// Recalulate our target tile
				tx = mTileX;
				ty = mTileY;

				if (mFacing == NORTH)		ty--;
				if (mFacing == EAST)		tx++;
				if (mFacing == SOUTH)		ty++;
				if (mFacing == WEST)		tx--;
			}
		}

		mTileX = tx;
		mTileY = ty;

		mSourcePos = mDestinationPos;
		mDestinationPos = Point(mTileX * mMap->GetTileSize(), mTileY * mMap->GetTileSize());
	}

	return NORMAL;
}

void SpaceMan::Draw()
{
	if (mState == HOME)
	{
		return;
	}

	SDL_Rect SrcRect = { 0, mFacing * 48, 32, 48 };

	if (mState == DYING)
	{
		SrcRect.x += (32 * mDyingAnimationFrame);
	}

	int x = (int)((mDestinationPos.x - mSourcePos.x) * mMovementDelta) + mSourcePos.x;
	int y = (int)((mDestinationPos.y - mSourcePos.y) * mMovementDelta) + mSourcePos.y;
	SDL_Rect DestRect = { x, y-16, 0, 0 };

	SDL_BlitSurface(mSurface, &SrcRect, Engine.GetBackBuffer(), &DestRect);
}
