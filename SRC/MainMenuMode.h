
#ifndef _MAINMENUMODE_H_
#define _MAINMENUMODE_H_


class MainMenuMode : public Mode
{
private:

	SDL_Surface *	mBackground;

	guiManager *	mGUI;

	button *		mButtonGame;
	button *		mButtonLoadLevel;
	button *		mButtonEditor;
	button *		mButtonQuit;

public:
	bool Enter();
	void Exit();

	void Update();

	void InputEvent(SDL_Event * event);
};

#endif
