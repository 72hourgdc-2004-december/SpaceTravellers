#include "checkbox.h"

checkBox::checkBox():button()
{
	setToggle(true);
	selColor.r = selColor.g = selColor.b = 0;
}

void checkBox::setSelColor(SDL_Color &_color)
{
	selColor = _color;
	reDraw = true;
}

void checkBox::render()
{
	if(hidden)
		return;

	if(reDraw)
	{
		SDL_Rect temp = {1,6,9,9};
		SDL_FillRect(surface,NULL,SDL_MapRGB(surface->format,255,0,255));
		SDL_FillRect(surface,&temp,SDL_MapRGB(surface->format,color.r,color.g,color.b));
		hLine(0,5,10,SDL_MapRGB(surface->format,0,0,0));
		vLine(0,5,10,SDL_MapRGB(surface->format,0,0,0));
		hLine(0,15,10,SDL_MapRGB(surface->format,255,255,255));
		vLine(10,5,11,SDL_MapRGB(surface->format,255,255,255));

		if(btnDown)
		{
			temp.x = 3;
			temp.y = 8;
			temp.h = temp.w = 5;
			SDL_FillRect(surface,&temp,SDL_MapRGB(surface->format,selColor.r,selColor.g,selColor.b));
		}

		if(foreground != NULL)
		{
			SDL_Rect rcDest = {15,0,0,0};
			SDL_BlitSurface(foreground,NULL,surface,&rcDest);
		}
		reDraw = false;
	}

	SDL_BlitSurface(surface,NULL,screen,&rect);
}