
#include "Engine.h"


int main(int argc, char* argv[])
{
	if (Engine.Create(800, 600, false) == false)
	{
		LOG.Print("Could not create engine");
		return false;
	}

	Modes.RegisterMode("SplashScreen", new SplashScreenMode);
	Modes.RegisterMode("MainMenu", new MainMenuMode);
	Modes.RegisterMode("Editor", new EditorMode);
	Modes.RegisterMode("Game", new GameMode);

	Modes.Init("SplashScreen");

	bool KeepRunning = true;
	while(KeepRunning)
	{
		Engine.BeginFrame();

		Engine.ProcessEvents();

		if (Modes.Update() == false)
		{
			break;
		}

		Engine.EndFrame();
	}

	Modes.Destroy();
	Engine.Destroy();

	return 0;
}
