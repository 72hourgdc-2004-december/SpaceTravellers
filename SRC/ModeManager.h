
#ifndef _MODEMANAGER_H_
#define _MODEMANAGER_H_

class Mode
{
public:
	virtual ~Mode() { }

	virtual bool Enter() = 0;
	virtual void Exit() = 0;

	virtual void Update() = 0;

	virtual void InputEvent(SDL_Event * event) { }
};

class ModeManager : public Singleton < ModeManager >
{
public:
	std::map < std::string, Mode * > mModes;

	Mode *		mCurrentMode;
	Mode *		mWantedMode;

	bool		mQuitWanted;

public:
	ModeManager();

	void RegisterMode(const std::string & name, Mode * mode);

	void Init(const std::string & name);
	void Destroy();

	void RequestMode(const std::string & name);

	bool Update();

	void InputEvent(SDL_Event * event);

	void QuitNow();

	Mode * GetCurrentMode()		{ return mCurrentMode; }
};

#define Modes ModeManager::GetInstance()

#endif
