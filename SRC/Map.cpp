
#include "Engine.h"

Map::Map()
{
	mData = NULL;
	mSurface = NULL;
}

bool Map::Load(const std::string & filename)
{
	std::ifstream mapfile(filename.c_str());

	if (mapfile.is_open() == false)
	{
		LOG.Print(std::string("Couldn't open map file: ") + filename);
		return false;
	}

	mapfile >> mWidth >> mHeight;

	mapfile >> mMaxTilesToBePlaced;

	mapfile >> mSurfaceFilename;

	mSurface = IMG_Load(mSurfaceFilename.c_str());

	if (mSurface == NULL)
	{
		LOG.Print(std::string("Couldn't load tiles: ") + mSurfaceFilename);
		return false;
	}

	LOG.Print(std::string("Loaded tiles: ") + mSurfaceFilename);

	mData = new Tile[mWidth * mHeight];

	// Load graphics tile info
	for (int y = 0; y < mHeight; y++)
	{
		for (int x = 0; x < mWidth; x++)
		{
			int temp;
			mapfile >> temp;

			mData[(y * mWidth) + x].mGfxID = temp;
		}
	}

	// Load AI tile info
	for (int y = 0; y < mHeight; y++)
	{
		for (int x = 0; x < mWidth; x++)
		{
			int temp;
			mapfile >> temp;

			mData[(y * mWidth) + x].mAI = temp;
		}
	}

	string AITilesFileName = "ai_tiles.bmp";
	mAISurface = IMG_Load(AITilesFileName.c_str());
	unsigned int ColourKey = SDL_MapRGB(mAISurface->format, 255, 0, 255);
	SDL_SetColorKey(mAISurface, SDL_SRCCOLORKEY, ColourKey);

	if (mAISurface == NULL)
	{
		LOG.Print(std::string("Could not load AI tiles: ") + AITilesFileName);
		return false;
	}

	LOG.Print(std::string("Loaded AI tiles: ") + AITilesFileName);

	mAnimationFrameTime = 0.0f;
	mAcidPoolsAnimationFrame = 0;

	return true;
}

void Map::Destroy()
{

}

void Map::Draw()
{
	mAnimationFrameTime += Engine.GetElapsedTime();
	if (mAnimationFrameTime >= 0.25f)
	{
		mAnimationFrameTime = 0.0f;
		mAcidPoolsAnimationFrame++;
		if (mAcidPoolsAnimationFrame == 4)
		{
			mAcidPoolsAnimationFrame = 0;
		}
	}

	int DrawX = 0;
	int DrawY = 0;

	for (int y = 0; y < mHeight; y++)
	{
		for (int x = 0; x < mWidth; x++)
		{
			const Tile & tile = TileAt(x, y);

			int gfx = tile.mGfxID;

			if (gfx == 12)
			{
				gfx += mAcidPoolsAnimationFrame;
			}

			SDL_Rect src = { gfx * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE };
			SDL_Rect dest = { DrawX, DrawY, 0, 0 };

			DrawX += TILE_SIZE;

			SDL_BlitSurface(mSurface, &src, Engine.GetBackBuffer(), &dest);
		}

		DrawX = 0;
		DrawY += TILE_SIZE;
	}
}

void Map::DrawAI(int minTileID)
{
	int DrawX = 0;
	int DrawY = 0;

	for (int y = 0; y < mHeight; y++)
	{
		for (int x = 0; x < mWidth; x++)
		{
			const Tile & tile = TileAt(x, y);
			
			if (tile.mAI >= minTileID)
			{
				SDL_Rect src = { tile.mAI * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE };
				SDL_Rect dest = { DrawX, DrawY, 0, 0 };

				SDL_BlitSurface(mAISurface, &src, Engine.GetBackBuffer(), &dest);
			}

			DrawX += TILE_SIZE;
		}

		DrawX = 0;
		DrawY += TILE_SIZE;
	}
}

