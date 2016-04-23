#ifndef __GUIBASE_H
#define __GUIBASE_H

#include <SDL.h>
#include <SDL_TTF.h>
#include <string>

using namespace std;

#define EVT_NONE	0
#define EVT_OVER	1
#define EVT_OUT		2
#define EVT_CLICK	3

#define UNHANDLED	0
#define HANDLED		1
#define KEEPFOCUS	2

class guiBase
{
public:
	struct coord
	{
		int x;
		int y;
	};
	struct eventData
	{
		int eventNum;
		string data;
	};
	typedef void (__stdcall *eventHandle)(guiBase *,SDL_Event &,eventData);

public:
	guiBase();
	virtual ~guiBase();

	string getID()									{ return(id);			}
	void setID(string _value)						{ id = _value;			}
	bool getHidden()								{ return(hidden);		}
	SDL_Rect &getRect()								{ return(rect);			}
	guiBase *getParent()							{ return(parent);		}
	void setParent(guiBase *_obj)					{ parent = _obj;		}
	void setRedraw(bool _value)						{ reDraw = _value;		}
	void setColor(SDL_Color &_color);
	void setHandler(eventHandle _handle)			{ handler = _handle;	}
	virtual void setSurface(SDL_Surface *_screen)	{ screen = _screen;		}

	void setPic(SDL_Surface *_pic);
	void setFocus(bool _value);
	void setHidden(bool _value);
	void setFontColor(SDL_Color &_color);
	coord getParentDims();
	bool inDims(int x,int y);
	coord mapToLocal(int x,int y);
	virtual void setText(string _text);
	virtual void setFont(string _filename,int _point = 12);
	virtual void setRect(SDL_Rect &_rect);

	virtual int update(SDL_Event &_event,int handled = UNHANDLED) = 0;
	virtual void render() = 0;


	void setSourceRect(int x, int y, int w, int h);

protected:
	guiBase *parent;
	SDL_Surface *screen,*surface,*foreground;
	SDL_Rect rect;
	SDL_Color color,fontColor;
	bool reDraw,hidden,lastStatus,focusFlag,isPic;
	string text,id;
	TTF_Font *ttfFont;
	eventHandle handler;

	bool useSrcRect;
	SDL_Rect srcRect;

protected:
	void putPixel(int x,int y,Uint32 pixel);
	void hLine(int x,int y,int len,Uint32 color);
	void vLine(int x,int y,int len,Uint32 color);
};

#endif