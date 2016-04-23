#ifndef __BUTTON_H
#define __BUTTON_H

#include "guibase.h"

class button:public guiBase
{
public:
	button();
	
	void setStatus(bool _value)		{ btnDown = _value;		}
	bool getStatus()				{ return(btnDown);		}
	void setToggle(bool _value)		{ isToggle = _value;	}
	virtual int update(SDL_Event &_event,int handled = UNHANDLED);
	virtual void render();

protected:
	bool btnDown,isToggle;
};

#endif