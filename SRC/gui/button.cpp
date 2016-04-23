#include "button.h"

button::button():guiBase()
{
	btnDown = isToggle = false;
	color.r = 0;
	color.g = color.b = 100;
}


int button::update(SDL_Event &_event,int handled)
{
	if(hidden || handled == HANDLED)
		return(UNHANDLED);

	eventData highEvent;
	highEvent.eventNum = EVT_NONE;
	bool inRect = inDims(_event.motion.x,_event.motion.y) && handled != KEEPFOCUS;
	if(isToggle)
	{
		if(_event.type == SDL_MOUSEBUTTONDOWN && inRect)
		{
			setStatus(!getStatus());
			reDraw = true;
		}
	}
	else
	{
		if(_event.type == SDL_MOUSEBUTTONDOWN && inRect)
		{
			setStatus(true);
			reDraw = true;
		}
		else if(btnDown && (_event.type == SDL_MOUSEBUTTONUP || !inRect))
		{
			setStatus(false);
			reDraw = true;
			highEvent.eventNum = EVT_CLICK;
		}
	}

	if(handler != NULL)
	{
		if(lastStatus != inRect && inRect)
			highEvent.eventNum = EVT_OVER;
		else if(lastStatus != inRect && !inRect)
			highEvent.eventNum = EVT_OUT;
		handler(this,_event,highEvent);
	}

	lastStatus = inRect;
	return(UNHANDLED);	// This is not a window type of object, so it will always
}						// return UNHANDLED to stay at the back of the list.

void button::render()
{
	if(hidden)
		return;

	if(reDraw)
	{
		int tmpX = 0,tmpY = 0;
		if(foreground != NULL)
		{
			if(rect.w != foreground->w)
				tmpX = (rect.w - foreground->w) / 2;
			if(rect.h != foreground->h)
				tmpY = (rect.h - foreground->h) / 2;
		}
		SDL_FillRect(surface,NULL,SDL_MapRGB(surface->format,color.r,color.g,color.b));

		if(btnDown)
		{
			tmpX++;
			tmpY++;
			hLine(0,0,rect.w,SDL_MapRGB(surface->format,0,0,0));
			vLine(0,1,rect.h - 2,SDL_MapRGB(surface->format,0,0,0));
			hLine(0,rect.h - 1,rect.w,SDL_MapRGB(surface->format,255,255,255));
			vLine(rect.w - 1,1,rect.h - 2,SDL_MapRGB(surface->format,255,255,255));
		}
		else
		{
			hLine(0,0,rect.w,SDL_MapRGB(surface->format,255,255,255));
			vLine(0,1,rect.h - 2,SDL_MapRGB(surface->format,255,255,255));
			hLine(0,rect.h - 1,rect.w,SDL_MapRGB(surface->format,0,0,0));
			vLine(rect.w - 1,0,rect.h - 1,SDL_MapRGB(surface->format,0,0,0));
		}

		if(foreground != NULL)
		{
			if (useSrcRect)
			{
				SDL_Rect tmpRect = {1,1,0,0};
				SDL_BlitSurface(foreground,&srcRect,surface,&tmpRect);
			}
			else
			{
				SDL_Rect tmpRect = {tmpX,tmpY,0,0};
				SDL_BlitSurface(foreground,NULL,surface,&tmpRect);
			}
		}

		reDraw = false;
	}
	SDL_BlitSurface(surface,NULL,screen,&rect);
}