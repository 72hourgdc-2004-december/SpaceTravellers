#include "scrollbar.h"
#include <SDL_Image.h>

#define UPSURF			"Images\\uparrow.bmp"
#define DOWNSURF		"Images\\downarrow.bmp"
#define LEFTSURF		"Images\\leftarrow.bmp"
#define RIGHTSURF		"Images\\rightarrow.bmp"


void __stdcall btnHandle(guiBase *_object,SDL_Event &_event,guiBase::eventData _highEvent)
{
	if(_highEvent.eventNum == EVT_CLICK)
	{
		if(_object->getID() == "INC")
			((scrollBar *)_object->getParent())->meter->setPos(((scrollBar *)_object->getParent())->meter->getPos() + 1);
		else if(_object->getID() == "DEC")
			((scrollBar *)_object->getParent())->meter->setPos(((scrollBar *)_object->getParent())->meter->getPos() - 1);
	}
}

scrollBar::scrollBar():guiBase()
{
	decSurf = incSurf = NULL;
	btnDec = new button;
	meter = new guiMeter;
	btnInc = new button;
	
	btnDec->setParent(this);
	meter->setParent(this);
	btnInc->setParent(this);
	
	btnDec->setID("DEC");
	btnDec->setHandler((eventHandle)&btnHandle);
	btnInc->setID("INC");
	btnInc->setHandler((eventHandle)&btnHandle);

	setHoriz(false);
}

scrollBar::~scrollBar()
{
	SDL_FreeSurface(decSurf);
	SDL_FreeSurface(incSurf);
	delete meter;
	delete btnDec;
	delete btnInc;
}

void scrollBar::setHoriz(bool _value)
{
	meter->setHoriz(isHoriz = _value);
	setRect(rect);
	SDL_FreeSurface(decSurf);
	SDL_FreeSurface(incSurf);
	if(isHoriz)
	{
		decSurf = IMG_Load(LEFTSURF);
		incSurf = IMG_Load(RIGHTSURF);
	}
	else
	{
		decSurf = IMG_Load(UPSURF);
		incSurf = IMG_Load(DOWNSURF);
	}
	btnDec->setPic(decSurf);
	btnInc->setPic(incSurf);
}

void scrollBar::setRect(SDL_Rect &_rect)
{
	SDL_Rect tmpRect;
	guiBase::setRect(_rect);
	if(isHoriz)
	{
		tmpRect.x = tmpRect.y = 0;
		tmpRect.w = BTN_HEIGHT;
		tmpRect.h = rect.h;
		btnDec->setRect(tmpRect);
		tmpRect.x += tmpRect.w;
		tmpRect.w = rect.w - tmpRect.w * 2;
		meter->setRect(tmpRect);
		tmpRect.x += tmpRect.w;
		tmpRect.w = rect.w - tmpRect.x;
		btnInc->setRect(tmpRect);
	}
	else
	{
		tmpRect.x = tmpRect.y = 0;
		tmpRect.w = rect.w;
		tmpRect.h = BTN_HEIGHT;
		btnDec->setRect(tmpRect);
		tmpRect.y += tmpRect.h;
		tmpRect.h = rect.h - tmpRect.h * 2;
		meter->setRect(tmpRect);
		tmpRect.y += tmpRect.h;
		tmpRect.h = rect.h - tmpRect.y;
		btnInc->setRect(tmpRect);
	}
	btnDec->setSurface(surface);
	meter->setSurface(surface);
	btnInc->setSurface(surface);
}

int scrollBar::update(SDL_Event &_event,int handled)
{
	if(hidden || handled == HANDLED)
		return(UNHANDLED);

	btnDec->update(_event,handled);
	meter->update(_event,handled);
	btnInc->update(_event,handled);

	return(UNHANDLED);
}

void scrollBar::render()
{
	if(hidden)
		return;

	btnDec->render();
	meter->render();
	btnInc->render();

	SDL_BlitSurface(surface,NULL,screen,&rect);
}