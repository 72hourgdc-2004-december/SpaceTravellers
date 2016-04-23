#include "list.h"

guiList::guiList():guiBase()
{
	maxLen = topList = 0;
	selected = -1;
	isResize = true;
	inRect = false;
	color.r = color.g = color.b = 50;
	fontColor.r = fontColor.g = fontColor.b = 0;
	selColor.r = 0;
	selColor.g = selColor.b = 100;
	scroll = new scrollBar;
	scroll->setParent(this);
	scroll->setSurface(surface);
	scroll->meter->setPos(0);
	scroll->meter->setScale(1);
	scroll->setHidden(true);
}

guiList::~guiList()
{
	for(int i = 0;i < (int)itemSurf.size();i++)
		SDL_FreeSurface(itemSurf[i]);
	delete scroll;
}

void guiList::setResize(bool _value)
{
	if(isResize = _value)
		scroll->setHidden(true);
	else
		scroll->setHidden(false);
}

void guiList::addItem(string item)
{
	items.push_back(item);
	itemSurf.push_back(TTF_RenderText_Solid(ttfFont,item.c_str(),fontColor));
	if(isResize)
	{
		maxLen = (int)items.size() - 1;
		rect.h = itemSurf[itemSurf.size() - 1]->h * maxLen;
		setRect(rect);
	}
	else if((int)items.size() > maxLen)
	{
		scroll->meter->setScale((int)items.size() - maxLen);
	}
}

void guiList::setSelColor(SDL_Color &_color)
{
	selColor = _color;
	reDraw = true;
}

void guiList::setRect(SDL_Rect &_rect)
{
	SDL_Rect tmpRect;
	guiBase::setRect(_rect);
	tmpRect.x = rect.w - BTN_HEIGHT;
	tmpRect.y = 0;
	tmpRect.h = rect.h;
	tmpRect.w = BTN_HEIGHT;
	scroll->setRect(tmpRect);
	scroll->setSurface(surface);
}

int guiList::update(SDL_Event &_event,int handled)
{
	if(hidden || handled == HANDLED)
		return(UNHANDLED);

	eventData highEvent;
	highEvent.eventNum = EVT_NONE;
	scroll->update(_event,handled);
	if(inDims(_event.motion.x,_event.motion.y) && handled != KEEPFOCUS && itemSurf.size() > 0)
	{
		selected = mapToLocal(_event.motion.x,_event.motion.y).y / itemSurf[0]->h + scroll->meter->getPos();
		reDraw = inRect = true;
	}
	else if(inRect && (!inDims(_event.motion.x,_event.motion.y) || handled == KEEPFOCUS))
	{
		reDraw = true;
		inRect = false;
		selected = -1;
	}


	if(handler != NULL)
	{
		if(lastStatus != inRect && inRect)
			highEvent.eventNum = EVT_OVER;
		else if(lastStatus != inRect && !inRect)
			highEvent.eventNum = EVT_OUT;
		if(selected != -1 && _event.type == SDL_MOUSEBUTTONDOWN)
			highEvent.data = items[selected];
		else
			highEvent.data = "";
		handler(this,_event,highEvent);
	}

	return(UNHANDLED);
}

void guiList::render()
{
	if(hidden)
		return;

	if(reDraw)
	{
		SDL_FillRect(surface,NULL,SDL_MapRGB(surface->format,color.r,color.g,color.b));
		SDL_Rect rcDest = {0,0,0,0};
		for(int i = scroll->meter->getPos();i <= scroll->meter->getPos() + maxLen && itemSurf[i] != NULL;i++)
		{
			if(i == selected)
			{
				SDL_Rect tmpRect;
				tmpRect.x = 0;
				tmpRect.y = rcDest.y;
				tmpRect.w = rect.w;
				tmpRect.h = itemSurf[i]->h;
				SDL_FillRect(surface,&tmpRect,SDL_MapRGB(surface->format,selColor.r,selColor.g,selColor.b));
			}
            SDL_BlitSurface(itemSurf[i],NULL,surface,&rcDest);
			rcDest.y += itemSurf[i]->h;
		}
		reDraw = false;
	}
	scroll->render();
	SDL_BlitSurface(surface,NULL,screen,&rect);
}