#ifndef __CONSOLE_H
#define __CONSOLE_H

#include "scrollbar.h"
#include <vector>

const string cendl = "\n";

class guiConsole:public guiBase
{
public:
	guiConsole();
	~guiConsole();

	virtual void setRect(SDL_Rect &_rect);
	virtual int update(SDL_Event &_event,int handled = UNHANDLED);
	virtual void render();

	scrollBar *memRecall;
	guiConsole &operator<<(const string &_str);
	guiConsole &operator<<(const int &_val);
	guiConsole &operator<<(const double &_val);
protected:
	vector<string> memory;
};

#endif