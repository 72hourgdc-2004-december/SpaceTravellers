#ifndef __SCROLLBAR_H
#define __SCROLLBAR_H

#include "meter.h"
#include "button.h"

class scrollBar:public guiBase
{
public:
	scrollBar();
	virtual ~scrollBar();

	void setHoriz(bool _value);
	virtual void setRect(SDL_Rect &_rect);
	virtual int update(SDL_Event &_event,int handled = UNHANDLED);
	virtual void render();

	guiMeter *meter;
	button *btnDec;
	button *btnInc;
protected:
	bool isHoriz;
	SDL_Surface *decSurf,*incSurf;
};

#endif