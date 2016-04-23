
#ifndef _MAP_H_
#define _MAP_H_

class Tile
{
public:
	int		mAI;
	int		mGfxID;
};

class Map
{
private:
	enum
	{
		TILE_SIZE = 32,
	};

	Tile *			mData;
	SDL_Surface *	mSurface;
	SDL_Surface *	mAISurface;

	std::string		mSurfaceFilename;

	int				mWidth;
	int				mHeight;

	int				mMaxTilesToBePlaced;

	float			mAnimationFrameTime;
	int				mAcidPoolsAnimationFrame;

public:
	Map();

	bool Load(const std::string & filename);
	void Destroy();

	void Draw();
	void DrawAI(int minTileID);

	Tile & TileAt(int x, int y)
	{
		return mData[(y * mWidth) + x];
	}

	Tile & TileAt(const Point & pixelCoord)
	{
		TileAt(pixelCoord.x / TILE_SIZE, pixelCoord.y / TILE_SIZE);
	}

	int GetWidth()						{ return mWidth; }
	int GetHeight()						{ return mHeight; }
	int GetTileSize()					{ return TILE_SIZE; }
	SDL_Surface * GetTileSurface()		{ return mSurface; }
	int GetNumTiles()					{ return mSurface->w / TILE_SIZE; }
	int GetNumAITiles()					{ return mAISurface->w / TILE_SIZE; }

	SDL_Surface * GetAITileSurface()	{ return mAISurface; }

	std::string GetSurfaceFilename()	{ return mSurfaceFilename; }
	int GetMaxTilesToBePlaced()			{ return mMaxTilesToBePlaced; }

	int GetNumSpaceMen()				
	{
		int spacemen = 0;

		for (int i = 0; i < mWidth * mHeight; i++)
		{
			if (mData[i].mAI == 2) spacemen ++;
		}

		return spacemen;
	}
};

#endif
