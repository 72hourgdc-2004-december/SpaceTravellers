#ifndef __LABEL_H
#define __LABEL_H

#include "guibase.h"


class guiLabel:public guiBase
{
public:
	guiLabel(string _text,int x,int y);

	virtual int update(SDL_Event &_event,int handled = UNHANDLED);
	virtual void render();
};

#endif