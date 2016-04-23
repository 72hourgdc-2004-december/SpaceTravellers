
#include "Engine.h"

bool gSingleLevel = false;
string gSingleLevelFileName;

// GUI Event handlers
void __stdcall GameButtonHandler(guiBase *_object, SDL_Event &_event, guiBase::eventData _highEvent)
{
	if(_highEvent.eventNum == EVT_CLICK)
	{
		gSingleLevel = false;
		Modes.RequestMode("Game");
	}
}

void __stdcall MainMenuLoadLevelButtonHandler(guiBase *_object, SDL_Event &_event, guiBase::eventData _highEvent)
{
	if(_highEvent.eventNum == EVT_CLICK)
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
		ofn.lpstrDefExt = "map.txt";
		ofn.lpstrTitle = "Select map";

		if(!GetOpenFileName(&ofn))
		{
			return;
		}

		gSingleLevel = true;
		gSingleLevelFileName = ofn.lpstrFile;

		Modes.RequestMode("Game");
	}
}

void __stdcall EditorButtonHandler(guiBase *_object, SDL_Event &_event, guiBase::eventData _highEvent)
{
	if(_highEvent.eventNum == EVT_CLICK)
	{
		Modes.RequestMode("Editor");
	}
}

void __stdcall QuitButtonHandler(guiBase *_object, SDL_Event &_event, guiBase::eventData _highEvent)
{
	if(_highEvent.eventNum == EVT_CLICK)
	{
		Modes.QuitNow();
	}
}

bool MainMenuMode::Enter()
{
	mGUI = new guiManager(Engine.GetBackBuffer());

	SDL_Rect DesktopRect = { 0, 0, Engine.GetWidth(), Engine.GetHeight() };
	mGUI->setRect(DesktopRect);

	SDL_Rect ButtonRect = { 580, 400, 200, 40 };

	mButtonGame = new button();
	mButtonGame->setRect(ButtonRect);
	mButtonGame->setText("Play!");
	mButtonGame->setID("play_game");
	mButtonGame->setHandler(GameButtonHandler);

	mGUI->addChild(mButtonGame);

	ButtonRect.y += 50;

	mButtonLoadLevel = new button();
	mButtonLoadLevel->setRect(ButtonRect);
	mButtonLoadLevel->setText("Load Custom Level");
	mButtonLoadLevel->setID("load_level");
	mButtonLoadLevel->setHandler(MainMenuLoadLevelButtonHandler);

	mGUI->addChild(mButtonLoadLevel);

	ButtonRect.y += 50;

	mButtonEditor = new button();
	mButtonEditor->setRect(ButtonRect);
	mButtonEditor->setText("Editor");
	mButtonEditor->setID("editor");
	mButtonEditor->setHandler(EditorButtonHandler);

	mGUI->addChild(mButtonEditor);

	ButtonRect.y += 50;

	mButtonQuit = new button();
	mButtonQuit->setRect(ButtonRect);
	mButtonQuit->setText("Quit");
	mButtonQuit->setID("quit");
	mButtonQuit->setHandler(QuitButtonHandler);

	mGUI->addChild(mButtonQuit);

	mBackground = IMG_Load("STtitle.jpg");

	return true;
}

void MainMenuMode::Exit()
{
	SDL_FreeSurface(mBackground);

	delete mGUI;
	mGUI = NULL;

	delete mButtonEditor;
	mButtonEditor = NULL;

	delete mButtonGame;
	mButtonGame = NULL;

	delete mButtonQuit;
	mButtonQuit = NULL;
}

void MainMenuMode::Update()
{
	SDL_BlitSurface(mBackground, NULL, Engine.GetBackBuffer(), NULL);

	if (mGUI)
	{
		mGUI->render();
	}
}

void MainMenuMode::InputEvent(SDL_Event * event)
{
	if (mGUI)
	{
		mGUI->update(*event);
	}
}
