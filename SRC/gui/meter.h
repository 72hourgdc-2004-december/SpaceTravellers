#ifndef __METER_H
#define __METER_H

#include "button.h"
#define BTN_HEIGHT	10

class guiMeter:public guiBase
{
public:
	guiMeter();
	virtual ~guiMeter();

	int getPos()						{ return(value); }
	void setHoriz(bool _value);
	void setPos(int _value);
	void setScale(int _value);
	virtual void setRect(SDL_Rect &_rect);
	virtual int update(SDL_Event &_event,int handled = UNHANDLED);
	virtual void render();

protected:
	SDL_Color sliderColor;
	bool isHoriz,status;
	double sliderWidth;
	int scale,value;
};

#endif