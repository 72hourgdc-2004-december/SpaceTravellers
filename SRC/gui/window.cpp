#include "window.h"

#define DEF_HEIGHT		10

window::window():guiBase()
{
	view = new desktop;
	view->setParent(this);
	height = DEF_HEIGHT;
	isDrag = false;
	color.r = 0;
	color.g = color.b = 100;
}

window::~window()
{
	delete view;
}


void window::setBorderColor(SDL_Color &_color)
{
	borderColor = _color;
	reDraw = true;
}


void window::setText(string _text)
{
	guiBase::setText(_text);
	if(foreground == NULL)
		height = DEF_HEIGHT;
	else
		height = foreground->h;
	setRect(rect);
}
void window::setFont(string _filename,int _point)
{
	guiBase::setFont(_filename,_point);
	if(foreground == NULL)
		height = DEF_HEIGHT;
	else
		height = foreground->h;
	setRect(rect);
}


void window::setRect(SDL_Rect &_rect)
{
	guiBase::setRect(_rect);
	SDL_Rect tmpRect = rect;
	tmpRect.x = 1;
	tmpRect.y = height + 1;
	tmpRect.w -= 2;
	tmpRect.h -= height + 2;
	view->setRect(tmpRect);
	view->setSurface(surface);
}


int window::update(SDL_Event &_event,int handled)
{
	int retVal = UNHANDLED;
	if(hidden)
		return(UNHANDLED);

	view->update(_event,handled);
	if(handled == HANDLED)
		return(UNHANDLED);

	eventData highEvent;
	highEvent.eventNum = EVT_NONE;
	if(handler != NULL)
	{
		handler(this,_event,highEvent);
	}

	if(inDims(_event.motion.x,_event.motion.y))
	{
		retVal = KEEPFOCUS;
		coord tmpPos = mapToLocal(_event.motion.x,_event.motion.y);
		if(_event.type == SDL_MOUSEBUTTONDOWN && tmpPos.y < height)
		{
			retVal = HANDLED;
			isDrag = true;
		}
		else if(_event.type == SDL_MOUSEBUTTONUP)
		{
			retVal = HANDLED;
			isDrag = false;
		}
	}

	if(isDrag && _event.type == SDL_MOUSEMOTION)
	{
		retVal = HANDLED;
		rect.x += _event.motion.xrel;
		rect.y += _event.motion.yrel;
		reDraw = true;
	}

	return(retVal);
}

void window::render()
{
	if(hidden)
		return;
	view->render();
	if(reDraw)
	{
		hLine(0,0,rect.w,SDL_MapRGB(surface->format,borderColor.r,borderColor.g,borderColor.b));
		vLine(0,1,rect.h - 2,SDL_MapRGB(surface->format,borderColor.r,borderColor.g,borderColor.b));
		hLine(0,rect.h - 1,rect.w,SDL_MapRGB(surface->format,borderColor.r,borderColor.g,borderColor.b));
		vLine(rect.w - 1,0,rect.h - 1,SDL_MapRGB(surface->format,borderColor.r,borderColor.g,borderColor.b));
		SDL_Rect tmpRect;
		tmpRect.x = tmpRect.y = 1;
		tmpRect.h = height;
		tmpRect.w = rect.w - 2;
		SDL_FillRect(surface,&tmpRect,SDL_MapRGB(surface->format,color.r,color.g,color.b));
		if(foreground != NULL)
		{
			tmpRect.x = 3;
			tmpRect.y = 1;
			tmpRect.w = tmpRect.h = 0;
			SDL_BlitSurface(foreground,NULL,surface,&tmpRect);
		}
		reDraw = false;
	}
	SDL_BlitSurface(surface,NULL,screen,&rect);
}