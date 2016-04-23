
#include "Engine.h"

ModeManager::ModeManager()
{
	mCurrentMode = NULL;
	mWantedMode = NULL;
	mQuitWanted = false;
}

void ModeManager::RegisterMode(const std::string & name, Mode * mode)
{
	if (mModes.find(name) != mModes.end())
	{
		LOG.Print(std::string("Mode already registered: ") + name);
		return;
	}

	mModes[name] = mode;
}

void ModeManager::Init(const std::string & name)
{
	std::map < std::string, Mode * > ::iterator it = mModes.find(name);

	if (it == mModes.end())
	{
		LOG.Print(std::string("Requested an invalid mode: ") + name);
		return;
	}

	mCurrentMode = it->second;
	mWantedMode = it->second;

	mCurrentMode->Enter();
}

void ModeManager::Destroy()
{
	mCurrentMode->Exit();
}

void ModeManager::RequestMode(const std::string & name)
{
	std::map < std::string, Mode * > ::iterator it = mModes.find(name);

	if (it == mModes.end())
	{
		LOG.Print(std::string("Requested an invalid mode: ") + name);
		return;
	}

	mWantedMode = it->second;
}

bool ModeManager::Update()
{
	if (mCurrentMode == NULL || mWantedMode == NULL)
	{
		LOG.Print("FATAL: Current or wanted mode is NULL");
		return false;
	}

	if (mCurrentMode != mWantedMode)
	{
		mCurrentMode->Exit();
		mCurrentMode = mWantedMode;
		mCurrentMode->Enter();
	}

	mCurrentMode->Update();

	return !mQuitWanted;
}

void ModeManager::InputEvent(SDL_Event * event)
{
	mCurrentMode->InputEvent(event);
}

void ModeManager::QuitNow()
{
	mQuitWanted = true;
}


