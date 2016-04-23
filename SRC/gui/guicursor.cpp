#include <SDL_Image.h>
#include "guicursor.h"

#define NUM_CURSORS		6
#define PATH_DEFAULT	"Images\\arrowcur.bmp"
#define PATH_UP			"Images\\upcur.bmp"
#define PATH_DOWN		"Images\\downcur.bmp"
#define PATH_LEFT		"Images\\leftcur.bmp"
#define PATH_RIGHT		"Images\\rightcur.bmp"
#define PATH_WAIT		"Images\\waitcur.bmp"

guiCursor::guiCursor():guiBase()
{
	rect.x = rect.y = rect.w = rect.h = xMax = yMax = 0;
	currentCursor = CUR_DEFAULT;
	customCursor = NULL;
	cursors.resize(NUM_CURSORS);
	loadCursor(CUR_DEFAULT,PATH_DEFAULT);
	loadCursor(CUR_UP,PATH_UP);
	loadCursor(CUR_DOWN,PATH_DOWN);
	loadCursor(CUR_LEFT,PATH_LEFT);
	loadCursor(CUR_RIGHT,PATH_RIGHT);
	loadCursor(CUR_WAIT,PATH_WAIT);
}

guiCursor::~guiCursor()
{
	for(int i = 0;i < NUM_CURSORS;i++)
		SDL_FreeSurface(cursors[i]);
}

void guiCursor::setRange(int x,int y)
{
	xMax = x;
	yMax = y;
}

bool guiCursor::loadCursor(int _type,string _filename)
{
	// Load a customized cursor.
	if(_type == CUR_CUSTOM)
	{
		SDL_FreeSurface(customCursor);
		if((customCursor = IMG_Load(_filename.c_str())) == NULL)
			return(false);
		SDL_SetColorKey(customCursor,SDL_SRCCOLORKEY,SDL_MapRGB(customCursor->format,255,0,255));
	}
	// Load a standard cursor.
	else
	{
		SDL_FreeSurface(cursors[_type]);
		if((cursors[_type] = IMG_Load(_filename.c_str())) == NULL)
			return(false);
		SDL_SetColorKey(cursors[_type],SDL_SRCCOLORKEY,SDL_MapRGB(cursors[_type]->format,255,0,255));
	}
	return(true);
}


int guiCursor::update(SDL_Event &_event,int handled)
{
	if(_event.type == SDL_MOUSEMOTION)
	{
		rect.x = _event.motion.x;
		rect.y = _event.motion.y;
		if(xMax != 0)
		{
			if(rect.x > xMax)
				rect.x = xMax;
			if(rect.y > yMax)
				rect.y = yMax;
		}
	}
	return(0);
}

void guiCursor::render()
{
	// Draw a custom cursor.
	if(currentCursor == CUR_CUSTOM)
		SDL_BlitSurface(customCursor,NULL,screen,&rect);
	// Draw a standard cursor.
	else
		SDL_BlitSurface(cursors[currentCursor],NULL,screen,&rect);
}