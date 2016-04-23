
#ifndef _EDITORMODE_H_
#define _EDITORMODE_H_

class EditorMode : public Mode
{
public:
	enum LAYERS
	{
		LAYER_GRAPHICS,
		LAYER_AI,
	};

	enum
	{
		MAX_TILE_BUTTONS	= 26,
	};

private:
	Map	*			mMap;

	LAYERS			mActiveLayer;
	int				mActiveTile;

	std::string		mCurrentLevelFilename;

	guiManager *	mGUI;

	window *		mEditorToolsWindow;

	button *		mButtonToggleLayers;
	button *		mButtonMainMenu;

	button *		mButtonNewLevel;
	button *		mButtonLoadLevel;
	button *		mButtonSaveLevel;

	window *		mTilesWindow;
	button *		mTiles[MAX_TILE_BUTTONS];

	window *		mAITilesWindow;
	button *		mAITiles[MAX_TILE_BUTTONS];

public:

	bool Enter();
	void Exit();

	void Update();

	void InputEvent(SDL_Event * event);

	void StartNewLevel(const std::string & filename, const std::string & tilesSurface);
	void LoadLevel(const std::string & filename);
	void SaveLevel();

	LAYERS GetActiveLayer()				{ return mActiveLayer; }
	void SetActiveLayer(LAYERS layer)
	{ 
		mActiveLayer = layer;
		if (mActiveLayer == LAYER_GRAPHICS)
		{
			mTilesWindow->setHidden(false);
			mAITilesWindow->setHidden(true);
		}
		if (mActiveLayer == LAYER_AI)
		{
			mTilesWindow->setHidden(true);
			mAITilesWindow->setHidden(false);
		}
	}

	int GetActiveTile()					{ return mActiveTile; }
	void SetActiveTile(int tile)		{ mActiveTile = tile; }

};

#endif
