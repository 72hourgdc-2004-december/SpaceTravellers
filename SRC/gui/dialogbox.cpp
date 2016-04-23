#include "dialogbox.h"

dialogBox::dialogBox(string _text):window()
{
	SDL_Rect tmpRect = {300,200,150,50};
	setRect(tmpRect);
	setText(_text);
}

int dialogBox::update(SDL_Event &_event,int handled)
{
	window::update(_event,handled);
	return(UNHANDLED);
}

void dialogBox::render()
{
	window::render();
}