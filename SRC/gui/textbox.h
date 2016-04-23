#ifndef __TEXTBOX_H
#define __TEXTBOX_H

#include "guibase.h"

class textBox:public guiBase
{
public:
	textBox();

	virtual int update(SDL_Event &_event,int handled = UNHANDLED);
	virtual void render();

	string getText() { return text; }
protected:

};

#endif