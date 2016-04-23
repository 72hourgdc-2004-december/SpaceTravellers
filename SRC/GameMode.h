
#ifndef _GAMEMODE_H_
#define _GAMEMODE_H_


class GameMode : public Mode
{
public:

	enum STATE
	{
		LOAD_LEVEL,
		SHOW_LEVEL_INTRO,
		SETTING_UP,
		RUNNING,
		BLASTOFF,
		LEVEL_WON,
		LEVEL_LOST,
		GAME_COMPLETE,
	};

private:
	STATE					mState;
	float					mStateTimer;

	guiManager *			mGUI;

	window *				mHudWindow;

	guiLabel *				mArrowsRemainingLabel;

	button *				mArrowButtons[4];
	button *				mGoButton;

	button *				mQuitButton;

	Map *					mMap;
	int						mCurrentLevel;

	vector < SpaceMan * >	mSpaceMen;
	SpaceShip *				mSpaceShip;

	int						mTilePlacementsRemaining;

	int						mSelectedTile;

	SDL_Surface *			mIntroSurface;

	string					mLevelName;
	bool					mSingleLevel;


public:
	bool Enter();
	void Exit();

	void Update();

	void InputEvent(SDL_Event * event);

	void LoadLevel(const std::string & filename, bool singleLevel = false);
	void DestroyLevel();

	void CreateHUDArrowButtons();


	STATE GetState()				{ return mState; }
	void SetState(STATE state)		{ mState = state; mStateTimer = 0.0f; }

	void SelectArrow(int arrow)		{ mSelectedTile = arrow; }
	void PlaceArrow(int x, int y);

	void StartRunning();
	void ResetLevel();
};

#endif
