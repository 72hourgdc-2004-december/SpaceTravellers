#include "desktop.h"

desktop::desktop():guiBase()
{
	isManage = false;
	color.r = 133;
	color.g = 135;
	color.b = 134;
}

void desktop::addChild(guiBase *_obj)
{
	_obj->setSurface(surface);
	_obj->setParent(this);
	children.push_back(_obj);
}

void desktop::removeChild(guiBase * _obj)
{
	list<guiBase *> ::iterator it;
	for (it = children.begin(); it != children.end(); it++)
	{
		if ((*it) == _obj)
		{
			children.erase(it);
			return;
		}
	}
}

void desktop::setRect(SDL_Rect &_rect)
{
	guiBase::setRect(_rect);
	for(list<guiBase *>::iterator p = children.begin();p != children.end();p++)
		(*p)->setSurface(surface);
}


int desktop::update(SDL_Event &_event,int handled)
{
	int evtResult;
	if(_event.type == SDL_KEYDOWN)
	{
		(*(children.begin()))->update(_event,handled);
	}
	else
	{
		list<guiBase *>::iterator temp = children.end();
		for(list<guiBase *>::iterator p = children.begin();p != children.end();p++)
		{
			if((evtResult = (*p)->update(_event,handled)) == HANDLED)
			{
				handled = HANDLED;
				temp = p;
			}
			else if(evtResult == KEEPFOCUS && handled != HANDLED)
				handled = KEEPFOCUS;
		}

		if(temp != children.end())
		{
			children.push_front(*temp);
			children.erase(temp);
		}
	}
	return(UNHANDLED);
}


void desktop::render()
{
	if (!isManage)	SDL_FillRect(surface,NULL,SDL_MapRGB(surface->format,color.r,color.g,color.b));
	for(list<guiBase *>::reverse_iterator p = children.rbegin();p != children.rend();p++)
		(*p)->render();
	if(!isManage)
		SDL_BlitSurface(surface,NULL,screen,&rect);
}