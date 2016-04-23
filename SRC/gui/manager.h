#ifndef __MANAGER_H
#define __MANAGER_H

#include "desktop.h"
#include "guicursor.h"

class guiManager:public desktop
{
public:
	guiManager(SDL_Surface *_screen = NULL);
	virtual ~guiManager();

	guiCursor *getCursor() { return(cursor); }
	virtual void setSurface(SDL_Surface *_screen);
	virtual void setRect(SDL_Rect &_rect);

	virtual int update(SDL_Event &_event,int handled = UNHANDLED);
	virtual void render();

protected:
	guiCursor *cursor;
};

#endif