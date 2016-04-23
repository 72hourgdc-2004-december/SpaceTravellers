#ifndef __DESKTOP_H
#define __DESKTOP_H

#include "guibase.h"
#include <list>


class desktop:public guiBase
{
public:
	desktop();

	void addChild(guiBase *_obj);
	void removeChild(guiBase * _obj);

	virtual void setRect(SDL_Rect &_rect);
	virtual int update(SDL_Event &_event,int handled = UNHANDLED);
	virtual void render();

protected:
	list<guiBase *> children;
	bool isManage;

protected:
	void isManager(bool _value)				{ isManage = _value; }
};

#endif