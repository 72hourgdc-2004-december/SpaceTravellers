#include "label.h"

guiLabel::guiLabel(string _text,int x,int y):guiBase()
{
	setText(_text);
	rect.x = x;
	rect.y = y;
	rect.w = rect.h = 1;
	setRect(rect);
}

int guiLabel::update(SDL_Event &_event,int handled)
{
	return(UNHANDLED);
}

void guiLabel::render()
{
	if(hidden)
		return;

	if(foreground != NULL)
	{
		SDL_Rect tmpRect = {rect.x,rect.y,0,0};
		SDL_BlitSurface(foreground,NULL,screen,&tmpRect);
	}
}