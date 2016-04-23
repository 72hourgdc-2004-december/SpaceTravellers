#include "manager.h"

guiManager::guiManager(SDL_Surface * _screen) : desktop()
{
	cursor = new guiCursor();
	setSurface(_screen);
	color.r = 0;
	color.g = 100;
	color.b = 200;
	TTF_Init();
	atexit(TTF_Quit);
	isManager(true);
}

guiManager::~guiManager()
{
	delete cursor;
}

void guiManager::setSurface(SDL_Surface *_screen)
{
	guiBase::setSurface(_screen);
	cursor->setSurface(surface);
}

void guiManager::setRect(SDL_Rect &_rect)
{
	rect = _rect;
	surface = screen;
	cursor->setSurface(surface);
}


int guiManager::update(SDL_Event &_event,int handled)
{
	desktop::update(_event);
	cursor->update(_event);
	_event.type = 0;
	return(0);
}

void guiManager::render()
{
	desktop::render();
	cursor->render();
}