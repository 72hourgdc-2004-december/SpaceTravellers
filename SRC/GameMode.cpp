
#include "Engine.h"

void __stdcall ArrowButtonsHandler(guiBase *_object, SDL_Event &_event, guiBase::eventData _highEvent)
{
	if(_highEvent.eventNum == EVT_CLICK)
	{
		GameMode * game = reinterpret_cast < GameMode * > (Modes.GetCurrentMode());

		// You can only chose tiles during the setup stage
		if (game->GetState() == GameMode::SETTING_UP)
		{
			stringstream str(_object->getID());
			int tile;
			str >> tile;
			game->SelectArrow(tile);
		}
	}
}

void __stdcall GoButtonHandler(guiBase *_object, SDL_Event &_event, guiBase::eventData _highEvent)
{
	if(_highEvent.eventNum == EVT_CLICK)
	{
		GameMode * game = reinterpret_cast < GameMode * > (Modes.GetCurrentMode());

		if (game->GetState() == GameMode::SETTING_UP)
		{
			game->StartRunning();
		}
		else if (game->GetState() == GameMode::RUNNING)
		{
			game->ResetLevel();
		}
	}
}

void __stdcall InGameQuitButtonHandler(guiBase *_object, SDL_Event &_event, guiBase::eventData _highEvent)
{
	if(_highEvent.eventNum == EVT_CLICK)
	{
		Modes.RequestMode("MainMenu");
	}
}

Point CenterText(string str, int w, int h)
{
	int strw = (int)(str.length() * 8);

	return Point((w-strw)/2, (h-8)/2);
}

bool GameMode::Enter()
{
	mCurrentLevel = 1;
	mState = LOAD_LEVEL;

	mStateTimer = 0.0f;

	mMap = NULL;
	mSelectedTile = -1;

	mIntroSurface = NULL;

	mSpaceShip = NULL;

	mGUI = new guiManager(Engine.GetBackBuffer());
	SDL_Rect DesktopRect = { 0, 0, Engine.GetWidth(), Engine.GetHeight()};
	mGUI->setRect(DesktopRect);

	int MapAreaHeight = 480;//mMap->GetHeight() * mMap->GetTileSize();

	mHudWindow = new window();
	SDL_Rect HUDWindowRect = { 0, MapAreaHeight, Engine.GetWidth(), Engine.GetHeight() - MapAreaHeight };
	mHudWindow->setRect(HUDWindowRect);

	mGUI->addChild(mHudWindow);

	mArrowsRemainingLabel = new guiLabel("Arrows: x", 20, 20);
	mHudWindow->view->addChild(mArrowsRemainingLabel);

	SDL_Rect GoButtonRect = { 300, 20, 100, 34 };

	mGoButton = new button();
	mGoButton->setRect(GoButtonRect);
	mGoButton->setText("GO!");
	mGoButton->setHandler(GoButtonHandler);

	mHudWindow->view->addChild(mGoButton);

	mQuitButton = new button();
	GoButtonRect.x += 120;
	mQuitButton->setRect(GoButtonRect);
	mQuitButton->setText("Exit");
	mQuitButton->setHandler(InGameQuitButtonHandler);

	mHudWindow->view->addChild(mQuitButton);

	for (int i = 0; i < 4; i++)
	{
		mArrowButtons[i] = NULL;
	}

	return true;
}

void GameMode::Exit()
{
	DestroyLevel();

	delete mGUI;
	mGUI = NULL;

	delete mHudWindow;
	mHudWindow = NULL;

	delete mArrowsRemainingLabel;
	mArrowsRemainingLabel = NULL;

	for (int i = 0; i < 4; i++)
	{
		delete mArrowButtons[i];
		mArrowButtons[i] = NULL;
	}

	delete mGoButton;
	mGoButton = NULL;

	delete mQuitButton;
	mQuitButton = NULL;
}

extern bool gSingleLevel;
extern string gSingleLevelFileName;

void GameMode::Update()
{
	mStateTimer += Engine.GetElapsedTime();

	switch (mState)
	{
	case LOAD_LEVEL:
		{
			mSingleLevel = gSingleLevel;

			if (gSingleLevel)
			{
				LoadLevel(gSingleLevelFileName, true);
				mLevelName = gSingleLevelFileName;
			}
			else
			{
				stringstream filename;
				filename << "level" << mCurrentLevel << ".map";
				LoadLevel(filename.str());
			}
		}
		break;

	case SHOW_LEVEL_INTRO:
		{
			if (mIntroSurface == NULL)
			{
				mIntroSurface = IMG_Load("intro.png");
				SDL_SetColorKey(mIntroSurface, SDL_SRCCOLORKEY, SDL_MapRGB(mIntroSurface->format, 255, 0, 255));
				
				Point pos = CenterText(mLevelName, mIntroSurface->w, mIntroSurface->h);
				stringRGBA(mIntroSurface, pos.x, pos.y, mLevelName.c_str(), 0, 0, 0, 255); 
			}

			SDL_Rect IntroRect = { (Engine.GetWidth() - mIntroSurface->w) / 2, (Engine.GetHeight() - mIntroSurface->h) / 2, mIntroSurface->w, mIntroSurface->h };
			SDL_BlitSurface(mIntroSurface, NULL, Engine.GetBackBuffer(), &IntroRect);

			if (mStateTimer > 2.0f)
			{
				SDL_FreeSurface(mIntroSurface);
				mIntroSurface = NULL;
				SetState(SETTING_UP);
			}
		}

		break;

	case SETTING_UP:
		if (mMap)
		{
			mMap->Draw();
			mMap->DrawAI(5);

			if (mTilePlacementsRemaining >  0 && mSelectedTile != -1)
			{
				const Point & MousePos = Engine.GetMousePos();
				int TileSize = mMap->GetTileSize();

				if (MousePos.x >= 0 && MousePos.x < mMap->GetWidth() * TileSize &&
					MousePos.y >= 0 && MousePos.y < mMap->GetHeight() * TileSize)
				{
					SDL_Surface * surface = mMap->GetAITileSurface();

					SDL_Rect SrcRect = { mSelectedTile * TileSize, 0, TileSize, TileSize };
					SDL_Rect DestRect = { (MousePos.x / TileSize) * TileSize, (MousePos.y / TileSize) * TileSize, 0, 0};

					Tile & tile = mMap->TileAt(MousePos.x / TileSize, MousePos.y / TileSize);

					if (tile.mAI == 0 || tile.mAI == 2)
					{
						SDL_BlitSurface(surface, &SrcRect, Engine.GetBackBuffer(), &DestRect);

						if (Engine.IsLeftMouseHeld())
						{
							tile.mAI = mSelectedTile;
							mTilePlacementsRemaining--;
							mSelectedTile = -1;

							stringstream str;
							str << "Arrows: " << mTilePlacementsRemaining;
							mArrowsRemainingLabel->setText(str.str());
							mHudWindow->setRedraw(true);
							mArrowsRemainingLabel->setRedraw(true);
						}
					}
				}
			}

			for (size_t i = 0; i < mSpaceMen.size(); i++)
			{
				mSpaceMen[i]->Draw();
			}

			if (mSpaceShip)
			{
				mSpaceShip->Draw();
			}
		}

		if (mGUI)
		{
			mGUI->render();
		}

		break;

	case RUNNING:
		if (mMap)
		{
			mMap->Draw();
			mMap->DrawAI(5);

			int NumSpaceMenHome = 0;

			for (size_t i = 0; i < mSpaceMen.size(); i++)
			{
				int state = mSpaceMen[i]->Update();
				if (state == SpaceMan::HOME) NumSpaceMenHome++;
				if (state == SpaceMan::DEAD) SetState(LEVEL_LOST);
				mSpaceMen[i]->Draw();
			}

			if (NumSpaceMenHome == (int)mSpaceMen.size())
			{
				// Woo! All rescued
				SetState(BLASTOFF);
			}

			if (mSpaceShip)
			{
				mSpaceShip->Draw();
			}
		}

		if (mGUI)
		{
			mGUI->render();
		}
		break;

	case BLASTOFF:
		mMap->Draw();

		if (mSpaceShip)
		{
			if (mSpaceShip->Update() == true)
			{
				SetState(LEVEL_WON);
			}
			mSpaceShip->Draw(true);
		}

		break;

	case LEVEL_LOST:
		{
			if (mIntroSurface == NULL)
			{
				mIntroSurface = IMG_Load("intro.png");
				SDL_SetColorKey(mIntroSurface, SDL_SRCCOLORKEY, SDL_MapRGB(mIntroSurface->format, 255, 0, 255));
				
				Point pos = CenterText("Level Failed", mIntroSurface->w, mIntroSurface->h);
				stringRGBA(mIntroSurface, pos.x, pos.y-5, "Level Failed", 0, 0, 0, 255); 
				
				pos = CenterText("Click To Retry", mIntroSurface->w, mIntroSurface->h);
				stringRGBA(mIntroSurface, pos.x, pos.y+5, "Click To Retry", 0, 0, 0, 255); 
			}

			SDL_Rect IntroRect = { (Engine.GetWidth() - mIntroSurface->w) / 2, (Engine.GetHeight() - mIntroSurface->h) / 2, mIntroSurface->w, mIntroSurface->h };
			SDL_BlitSurface(mIntroSurface, NULL, Engine.GetBackBuffer(), &IntroRect);

			if (mStateTimer > 0.5f && Engine.IsLeftMouseHeld())
			{
				SDL_FreeSurface(mIntroSurface);
				mIntroSurface = NULL;
				SetState(LOAD_LEVEL);
			}
		}
		break;

	case LEVEL_WON:
		{
			stringRGBA(Engine.GetBackBuffer(), 20, 20, "LEVEL WON", 0, 0, 255, 255);

			if (mSingleLevel)
			{
				Modes.RequestMode("MainMenu");
			}
			else
			{
				DestroyLevel();
				mCurrentLevel++;

				stringstream filename;
				filename << "level" << mCurrentLevel << ".map";
				LoadLevel(filename.str());

				SetState(LOAD_LEVEL);
			}
			break;
		}

	case GAME_COMPLETE:
		{
			if (mIntroSurface == NULL)
			{
				mIntroSurface = IMG_Load("intro.png");
				SDL_SetColorKey(mIntroSurface, SDL_SRCCOLORKEY, SDL_MapRGB(mIntroSurface->format, 255, 0, 255));

				Point pos = CenterText("Game Complete!", mIntroSurface->w, mIntroSurface->h);
				stringRGBA(mIntroSurface, pos.x, pos.y, "Game Complete!", 0, 0, 0, 255); 
			}

			SDL_Rect IntroRect = { (Engine.GetWidth() - mIntroSurface->w) / 2, (Engine.GetHeight() - mIntroSurface->h) / 2, mIntroSurface->w, mIntroSurface->h };
			SDL_BlitSurface(mIntroSurface, NULL, Engine.GetBackBuffer(), &IntroRect);

			if ((mStateTimer > 0.5f && Engine.IsLeftMouseHeld()) || mStateTimer > 5.0f)
			{
				SDL_FreeSurface(mIntroSurface);
				mIntroSurface = NULL;
				Modes.RequestMode("MainMenu");
			}
		}
		break;
	}
}

void GameMode::InputEvent(SDL_Event * event)
{
	if (mGUI)
	{
		mGUI->update(*event);
	}
}

void GameMode::LoadLevel(const std::string & filename, bool singleLevel)
{
	DestroyLevel();


	mMap = new Map();
	if (mMap->Load(filename) == false)
	{
		if (mSingleLevel)
		{
			Modes.RequestMode("MainMenu");
		}
		else
		{
			SetState(GAME_COMPLETE);
		}

		return;
	}

	mTilePlacementsRemaining = mMap->GetMaxTilesToBePlaced();

	stringstream str;
	str << "Arrows: " << mTilePlacementsRemaining;
	mArrowsRemainingLabel->setText(str.str());

	mGoButton->setText("GO!");

	// Go through the AI layer and spawn spacemen

	for (int y = 0; y < mMap->GetHeight(); y++)
	{
		for (int x = 0; x < mMap->GetWidth(); x++)
		{
			Tile & tile = mMap->TileAt(x, y);
			if (tile.mAI == 2)
			{
				// Spawn a spaceman
				SpaceMan * man = new SpaceMan();
				man->Init(x, y, mMap);
				mSpaceMen.push_back(man);
			}
			else if (tile.mAI == 3)
			{
				mSpaceShip = new SpaceShip();
				mSpaceShip->Init(x * mMap->GetTileSize(), y * mMap->GetTileSize());
			}
		}
	}

	CreateHUDArrowButtons();

	mLevelName = filename;
	SetState(SHOW_LEVEL_INTRO);
}

void GameMode::DestroyLevel()
{
	if (mMap)
	{
		mMap->Destroy();
		delete mMap;
		mMap = NULL;
	}

	for (size_t i = 0; i < mSpaceMen.size(); i++)
	{
		mSpaceMen[i]->Destroy();
		delete mSpaceMen[i];
	}

	mSpaceMen.clear();

	if (mSpaceShip)
	{
		mSpaceShip->Destroy();
		delete mSpaceShip;
		mSpaceShip = NULL;
	}
}

void GameMode::CreateHUDArrowButtons()
{
	// ARROW BUTTONS
	SDL_Rect ArrowButtonRect = { 100, 20, 34, 34 };

	for (int i = 0; i < 4; i++)
	{
		mArrowButtons[i] = new button();
		mArrowButtons[i]->setRect(ArrowButtonRect);

		mArrowButtons[i]->setPic(mMap->GetAITileSurface());
		mArrowButtons[i]->setSourceRect((i+5)*32, 0, 32, 32);

		char buf[8];
		sprintf(buf, "%d", i+5);
		mArrowButtons[i]->setID(buf);

		mArrowButtons[i]->setHandler(ArrowButtonsHandler);

		mHudWindow->view->addChild(mArrowButtons[i]);

		ArrowButtonRect.x += 40;
	}
}

void GameMode::StartRunning()
{
	SetState(RUNNING);
	mGoButton->setText("Restart");
}

void GameMode::ResetLevel()
{
	DestroyLevel();
	LoadLevel(mLevelName);
}
