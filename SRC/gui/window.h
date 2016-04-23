#ifndef __WINDOW_H
#define __WINDOW_H

#include "desktop.h"

class window:public guiBase
{
public:
	window();
	virtual ~window();

	void setBorderColor(SDL_Color &_color);
	virtual void setText(string _text);
	virtual void setFont(string _filename,int _point = 12);
	virtual void setRect(SDL_Rect &_rect);
	virtual int update(SDL_Event &_event,int handled = UNHANDLED);
	virtual void render();

	desktop *view;
protected:
	SDL_Color borderColor;
	int height;
	bool isDrag;
};

#endif