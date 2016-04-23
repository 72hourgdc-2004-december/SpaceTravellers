#ifndef __LIST_H
#define __LIST_H

#include "scrollbar.h"
#include <vector>

class guiList:public guiBase
{
public:
	guiList();
	~guiList();

	void setMaxLen(int _value)	{ maxLen = _value;		}

	void setResize(bool _value);
	void addItem(string item);
	void setSelColor(SDL_Color &_color);
	virtual void setRect(SDL_Rect &_rect);
	virtual int update(SDL_Event &_event,int handled = UNHANDLED);
	virtual void render();

	scrollBar *scroll;
protected:
	vector<string> items;
	vector<SDL_Surface *> itemSurf;
	SDL_Color selColor;
	int maxLen,selected,topList;
	bool isResize,inRect;
};

#endif