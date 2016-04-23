#ifndef __CHECKBOX_H
#define __CHECKBOX_H

#include "button.h"

class checkBox:public button
{
public:
	checkBox();
	void setSelColor(SDL_Color &_color);
	virtual void render();
	
protected:
	SDL_Color selColor;
};

#endif