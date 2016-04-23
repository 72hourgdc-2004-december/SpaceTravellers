
#include "Engine.h"
#include <windows.h>

// GUI Event handlers
void __stdcall ToggleLayersButtonHandler(guiBase *_object, SDL_Event &_event, guiBase::eventData _highEvent)
{
	if(_highEvent.eventNum == EVT_CLICK)
	{
		EditorMode * editor = reinterpret_cast < EditorMode * > (Modes.GetCurrentMode());

		if (editor->GetActiveLayer() == EditorMode::LAYER_GRAPHICS)
		{
			editor->SetActiveLayer(EditorMode::LAYER_AI);
			_object->setText("Layer: AI");
		}
		else
		{
			editor->SetActiveLayer(EditorMode::LAYER_GRAPHICS);
			_object->setText("Layer: Graphics");
		}

		editor->SetActiveTile(0);
		_object->setRedraw(true);
	}
}

void __stdcall MainMenuButtonHandler(guiBase *_object, SDL_Event &_event, guiBase::eventData _highEvent)
{
	if(_highEvent.eventNum == EVT_CLICK)
	{
		if (MessageBox(Engine.GetWin32Handle(), "Quit to main menu without saving?", "", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
		{
			Modes.RequestMode("MainMenu");
		}
	}
}

void __stdcall SelectTileButtonsHandler(guiBase *_object, SDL_Event &_event, guiBase::eventData _highEvent)
{
	if(_highEvent.eventNum == EVT_CLICK)
	{
		EditorMode * editor = reinterpret_cast < EditorMode * > (Modes.GetCurrentMode());

		stringstream str(_object->getID());
		int tile;
		str >> tile;
		editor->SetActiveTile(tile);
	}
}

void __stdcall NewLevelButtonHandler(guiBase *_object, SDL_Event &_event, guiBase::eventData _highEvent)
{
	if(_highEvent.eventNum == EVT_CLICK)
	{
		EditorMode * editor = reinterpret_cast < EditorMode * > (Modes.GetCurrentMode());

		string filename;
		string tilesurface;

		{
			OPENFILENAME ofn;
			char szFileName[MAX_PATH] = "";

			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
			ofn.hwndOwner = Engine.GetWin32Handle();
			ofn.lpstrFilter = "Map Files (*.map)\0";
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
			ofn.lpstrDefExt = ".map";
			ofn.lpstrTitle = "Select filename for new map";

			if(!GetOpenFileName(&ofn))
			{
				return;
			}

			filename.assign(ofn.lpstrFile);
		}

		{
			OPENFILENAME ofn;
			char szFileName[MAX_PATH] = "";

			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
			ofn.hwndOwner = Engine.GetWin32Handle();
			ofn.lpstrFilter = "Tilesets (*.tiles.bmp)\0";
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
			ofn.lpstrDefExt = "tiles.bmp";
			ofn.lpstrTitle = "Select tileset";

			if(!GetOpenFileName(&ofn))
			{
				return;
			}

			tilesurface.assign(ofn.lpstrFile);
		}

		if (filename.length() > 0 && tilesurface.length() > 0)
		{
			editor->StartNewLevel(filename, tilesurface);
		}
	}
}

void __stdcall LoadLevelButtonHandler(guiBase *_object, SDL_Event &_event, guiBase::eventData _highEvent)
{
	if(_highEvent.eventNum == EVT_CLICK)
	{
		EditorMode * editor = reinterpret_cast < EditorMode * > (Modes.GetCurrentMode());

		{
			OPENFILENAME ofn;
			char szFileName[MAX_PATH] = "";

			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
			ofn.hwndOwner = Engine.GetWin32Handle();
			ofn.lpstrFilter = "Map Files (*.map)\0";
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
			ofn.lpstrDefExt = "map";
			ofn.lpstrTitle = "Select filename for new map";

			if(!GetOpenFileName(&ofn))
			{
				return;
			}

			editor->LoadLevel(ofn.lpstrFile);
		}
	}
}

void __stdcall SaveLevelButtonHandler(guiBase *_object, SDL_Event &_event, guiBase::eventData _highEvent)
{
	if(_highEvent.eventNum == EVT_CLICK)
	{
		EditorMode * editor = reinterpret_cast < EditorMode * > (Modes.GetCurrentMode());

		editor->SaveLevel();
	}
}

bool EditorMode::Enter()
{
	mMap = NULL;

	mActiveLayer = LAYER_GRAPHICS;
	mActiveTile = 0;

	mTilesWindow = NULL;
	mAITilesWindow = NULL;

	mGUI = new guiManager(Engine.GetBackBuffer());

	int MapAreaHeight = 480;//mMap->GetHeight() * mMap->GetTileSize();

	SDL_Rect DesktopRect = { 0, 0, Engine.GetWidth(), Engine.GetHeight()};
	mGUI->setRect(DesktopRect);

	mEditorToolsWindow = new window();
	SDL_Rect ToolsWindowRect = { 0, MapAreaHeight, Engine.GetWidth(), Engine.GetHeight() - MapAreaHeight };
	mEditorToolsWindow->setRect(ToolsWindowRect);
	mEditorToolsWindow->setText("Editor Tools");

	mGUI->addChild(mEditorToolsWindow);

	// NEW LEVEL BUTTON
	SDL_Rect ButtonRect = { 5, 5, 150, 25 };

	mButtonNewLevel = new button();
	mButtonNewLevel->setRect(ButtonRect);
	mButtonNewLevel->setText("New Level");
	mButtonNewLevel->setID("new_level");
	mButtonNewLevel->setHandler(NewLevelButtonHandler);

	mEditorToolsWindow->view->addChild(mButtonNewLevel);

	// LOAD LEVEL BUTTON
	ButtonRect.y += 30;

	mButtonLoadLevel = new button();
	mButtonLoadLevel->setRect(ButtonRect);
	mButtonLoadLevel->setText("Load Level");
	mButtonLoadLevel->setID("load_level");
	mButtonLoadLevel->setHandler(LoadLevelButtonHandler);

	mEditorToolsWindow->view->addChild(mButtonLoadLevel);

	// SAVE LEVEL BUTTON
	ButtonRect.y += 30;

	mButtonSaveLevel = new button();
	mButtonSaveLevel->setRect(ButtonRect);
	mButtonSaveLevel->setText("Save Level");
	mButtonSaveLevel->setID("save_level");
	mButtonSaveLevel->setHandler(SaveLevelButtonHandler);

	mEditorToolsWindow->view->addChild(mButtonSaveLevel);

	// TOGGLE LAYERS BUTTON
	ButtonRect.x = 645;
	ButtonRect.y = 5;

	mButtonToggleLayers = new button();
	mButtonToggleLayers->setRect(ButtonRect);
	mButtonToggleLayers->setText("Layer: Graphics");
	mButtonToggleLayers->setID("toggle_layers");
	mButtonToggleLayers->setHandler(ToggleLayersButtonHandler);

	mEditorToolsWindow->view->addChild(mButtonToggleLayers);

	mButtonMainMenu = new button();
	ButtonRect.y += 30 + 30;
	mButtonMainMenu->setRect(ButtonRect);
	mButtonMainMenu->setText("Main Menu");
	mButtonMainMenu->setHandler(MainMenuButtonHandler);

	mEditorToolsWindow->view->addChild(mButtonMainMenu);

	return true;
}

void EditorMode::Exit()
{
	if (mMap)
	{
		mMap->Destroy();
		delete mMap;
		mMap = NULL;
	}

	delete mGUI;
	mGUI = NULL;

	delete mButtonToggleLayers;
	mButtonToggleLayers = NULL;

	delete mButtonMainMenu;
	mButtonMainMenu = NULL;

	delete mEditorToolsWindow;
	mEditorToolsWindow = NULL;

	delete mButtonNewLevel;
	mButtonNewLevel = NULL;

	delete mButtonLoadLevel;
	mButtonLoadLevel = NULL;

	delete mButtonSaveLevel;
	mButtonSaveLevel = NULL;

	delete mTilesWindow;
	mTilesWindow = NULL;

	delete mAITilesWindow;
	mAITilesWindow = NULL;

	for (int i = 0; i < MAX_TILE_BUTTONS; i++)
	{
		delete mTiles[i];
		delete mAITiles[i];
	}
}

void EditorMode::Update()
{
	if (mMap)
	{
		mMap->Draw();

		if (mActiveLayer == LAYER_AI)
		{
			mMap->DrawAI(0);
		}

		const Point & MousePos = Engine.GetMousePos();
		int TileSize = mMap->GetTileSize();

		if (MousePos.x >= 0 && MousePos.x < mMap->GetWidth() * TileSize &&
			MousePos.y >= 0 && MousePos.y < mMap->GetHeight() * TileSize)
		{
			SDL_Surface * surface;
			
			if (mActiveLayer == LAYER_GRAPHICS)
				surface = mMap->GetTileSurface();
			else if (mActiveLayer == LAYER_AI)
				surface = mMap->GetAITileSurface();

			SDL_Rect SrcRect = { mActiveTile * TileSize, 0, TileSize, TileSize };
			SDL_Rect DestRect = { (MousePos.x / TileSize) * TileSize, (MousePos.y / TileSize) * TileSize, 0, 0};

			SDL_BlitSurface(surface, &SrcRect, Engine.GetBackBuffer(), &DestRect);

			if (Engine.IsRightMouseHeld())
			{
				Tile & tile = mMap->TileAt(MousePos.x / TileSize, MousePos.y / TileSize);

				if (mActiveLayer == LAYER_GRAPHICS)
					tile.mGfxID = mActiveTile;
				else if (mActiveLayer == LAYER_AI)
					tile.mAI = mActiveTile;
			}
		}
	}

	if (mGUI)
	{
		mGUI->render();
	}
}

void EditorMode::InputEvent(SDL_Event * event)
{
	if (mGUI)
	{
		mGUI->update(*event);
	}
}

void EditorMode::StartNewLevel(const std::string & filename, const std::string & tilesSurface)
{
	ofstream mapfile(filename.c_str());

	mapfile << 25 << " " << 15 << std::endl;			// Map width and height
	mapfile << 5 << std::endl;							// Max tiles allowed to be placed
	mapfile << tilesSurface << std::endl << std::endl;

	// Graphics tiles
	for (int y = 0; y < 15; y++)
	{
		for (int x = 0; x < 25; x++)
		{
			mapfile << 0 << " ";
		}

		mapfile << std::endl;
	}

	// AI Tiles
	for (int y = 0; y < 15; y++)
	{
		for (int x = 0; x < 25; x++)
		{
			mapfile << 0 << " ";
		}

		mapfile << std::endl;
	}

	mapfile.close();

	LoadLevel(filename);
}

void EditorMode::LoadLevel(const std::string & filename)
{
	mCurrentLevelFilename = filename;

	if (mMap != NULL)
	{
		mMap->Destroy();
		delete mMap;
	}

	mMap = new Map();

	if (mMap->Load(filename) == false)
	{
		LOG.Print("Map load failed");
		delete mMap;
		mMap = NULL;
		return;
	}

	for (int i = 0; i < MAX_TILE_BUTTONS; i++)
	{
		if (mTiles[i])
		{
		}

		mTiles[i] = NULL;
		mAITiles[i] = NULL;
	}

	if (mTilesWindow)
	{
		mGUI->removeChild(mTilesWindow);
		delete mTilesWindow;
	}

	mTilesWindow = new window();
	SDL_Rect TilesWindowRect = { 200, 200, 300, 150 };
	mTilesWindow->setRect(TilesWindowRect);
	mTilesWindow->setText("Graphics Tiles");
	mGUI->addChild(mTilesWindow);

	mActiveLayer = LAYER_GRAPHICS;
	mActiveTile = 0;

	{
		int NumTiles = mMap->GetNumTiles();
		SDL_Surface * surface = mMap->GetTileSurface();

		int TilesDrawX = 5;

		int DrawX = TilesDrawX;
		int DrawY = 5;
		int Count = 0;

		for (int i = 0; i < NumTiles; i++)
		{
			mTiles[i] = new button();

			SDL_Rect DestRect = { DrawX, DrawY, mMap->GetTileSize() + 2, mMap->GetTileSize() + 2 };
			mTiles[i]->setRect(DestRect);

			mTiles[i]->setPic(surface);
			mTiles[i]->setSourceRect(i * mMap->GetTileSize(), 0, mMap->GetTileSize(), mMap->GetTileSize());

			char buf[8];
			sprintf(buf, "%d", i);
			mTiles[i]->setID(buf);

			mTiles[i]->setHandler(SelectTileButtonsHandler);

			mTilesWindow->view->addChild(mTiles[i]);

			DrawX += mMap->GetTileSize() + 4;
			Count++;

			if (Count == 8)
			{
				DrawX = TilesDrawX;
				DrawY += mMap->GetTileSize() + 4;
				Count = 0;
			}

			// HACK: Jump over the animated acid pool tiles
			if (i == 12)
			{
				i = 15;
			}
		}
	}

	if (mAITilesWindow)
	{
		mGUI->removeChild(mAITilesWindow);
		delete mAITilesWindow;
	}

	mAITilesWindow = new window();
	SDL_Rect AITilesWindowRect = { 200, 200, 300, 150 };
	mAITilesWindow->setRect(AITilesWindowRect);
	mAITilesWindow->setText("AI/Collision Tiles");
	mGUI->addChild(mAITilesWindow);
	mAITilesWindow->setHidden(true);

	{
		int NumTiles = mMap->GetNumAITiles();
		SDL_Surface * surface = mMap->GetAITileSurface();

		int TilesDrawX = 5;

		int DrawX = TilesDrawX;
		int DrawY = 5;
		int Count = 0;

		for (int i = 0; i < NumTiles; i++)
		{
			mTiles[i] = new button();

			SDL_Rect DestRect = { DrawX, DrawY, mMap->GetTileSize() + 2, mMap->GetTileSize() + 2 };
			mTiles[i]->setRect(DestRect);

			mTiles[i]->setPic(surface);
			mTiles[i]->setSourceRect(i * mMap->GetTileSize(), 0, mMap->GetTileSize(), mMap->GetTileSize());

			char buf[8];
			sprintf(buf, "%d", i);
			mTiles[i]->setID(buf);

			mTiles[i]->setHandler(SelectTileButtonsHandler);

			mAITilesWindow->view->addChild(mTiles[i]);

			DrawX += mMap->GetTileSize() + 4;
			Count++;

			if (Count == 8)
			{
				DrawX = TilesDrawX;
				DrawY += mMap->GetTileSize() + 4;
				Count = 0;
			}

			// HACK: Jump over the animated acid pool tiles
			if (i == 12)
			{
				i = 15;
			}
		}
	}

	SetActiveLayer(LAYER_GRAPHICS);
}

void EditorMode::SaveLevel()
{
	std::ofstream mapfile(mCurrentLevelFilename.c_str());

	if (mapfile.is_open() == false)
	{
		LOG.Print(string("Couldnt save to file: ") + mCurrentLevelFilename);
		return;
	}

	mapfile << 25 << " " << 15 << std::endl;
	mapfile << 5 << std::endl;
	mapfile << mMap->GetSurfaceFilename() << std::endl << std::endl;

	for (int y = 0; y < 15; y++)
	{
		for (int x = 0; x < 25; x++)
		{
			Tile & tile = mMap->TileAt(x, y);
			mapfile << tile.mGfxID << " ";
		}

		mapfile << std::endl;
	}

	mapfile << std::endl;

	for (int y = 0; y < 15; y++)
	{
		for (int x = 0; x < 25; x++)
		{
			Tile & tile = mMap->TileAt(x, y);
			mapfile << tile.mAI << " ";
		}

		mapfile << std::endl;
	}

	MessageBox(NULL, (string("Level saved: ") + mCurrentLevelFilename).c_str(), "Map Saved", MB_OK | MB_ICONINFORMATION);
}
