#ifndef __GUICURSOR_H
#define __GUICURSOR_H

#include "guibase.h"
#include <vector>

using namespace std;

#define CUR_CUSTOM		-1
#define CUR_DEFAULT		0
#define CUR_UP			1
#define CUR_DOWN		2
#define CUR_LEFT		3
#define CUR_RIGHT		4
#define CUR_WAIT		5


class guiCursor:public guiBase
{
public:
	guiCursor();
	virtual ~guiCursor();

	void setRange(int x,int y);
	bool loadCursor(int _type,string _filename);
	void setCursor(int _value)					{ currentCursor = _value;	}
	int getCursor()								{ return(currentCursor);	}
	virtual int update(SDL_Event &_event,int handled = UNHANDLED);
	virtual void render();

protected:
	int currentCursor,xMax,yMax;
	vector<SDL_Surface *> cursors;
	SDL_Surface *customCursor;
};

#endif