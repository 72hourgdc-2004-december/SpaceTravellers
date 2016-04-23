#ifndef __DIALOG_H
#define __DIALOG_H

#include "window.h"

class dialogBox:public window
{
public:
	dialogBox(string _text);

	virtual int update(SDL_Event &_event,int handled = UNHANDLED);
	virtual void render();
};

#endif