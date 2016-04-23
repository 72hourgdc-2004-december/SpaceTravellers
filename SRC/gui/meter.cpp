#include "meter.h"

guiMeter::guiMeter():guiBase()
{
	isHoriz = status = false;
	scale = BTN_HEIGHT;
	setScale(scale);
	setPos(0);
	sliderColor.r = 0;
	sliderColor.g = sliderColor.b = color.r = color.g = color.b = 100;
}

guiMeter::~guiMeter()
{

}

void guiMeter::setHoriz(bool _value)
{
	isHoriz = _value;
	setScale(scale);
	reDraw = true;
}

void guiMeter::setPos(int _value)
{
	if(_value >= 0 && _value < scale)
		value = _value;
	reDraw = true;
}

void guiMeter::setScale(int _value)
{
	scale = _value;
	if(isHoriz)
		sliderWidth = (double)rect.w / (double)scale;
	else
		sliderWidth = (double)rect.h / (double)scale;
}

void guiMeter::setRect(SDL_Rect &_rect)
{
	guiBase::setRect(_rect);
	setScale(scale);
}

int guiMeter::update(SDL_Event &_event,int handled)
{
	if(_event.type == SDL_MOUSEBUTTONUP && _event.button.button == SDL_BUTTON_LEFT)
		status = false;

	if(hidden || handled == HANDLED)
		return(UNHANDLED);

	bool inRect = inDims(_event.motion.x,_event.motion.y) && handled != KEEPFOCUS;
	if(_event.type == SDL_MOUSEBUTTONDOWN && _event.button.button == SDL_BUTTON_LEFT && inRect)
		status = true;
	else if(_event.type == SDL_MOUSEBUTTONUP && _event.button.button == SDL_BUTTON_LEFT)
		status = false;

	if(status)
	{
		if(isHoriz)
			setPos((int)(mapToLocal(_event.motion.x,_event.motion.y).x / sliderWidth));
		else
			setPos((int)(mapToLocal(_event.motion.x,_event.motion.y).y / sliderWidth));
		reDraw = true;
	}

	return(UNHANDLED);
}

void guiMeter::render()
{
	if(hidden)
		return;

	if(reDraw)
	{
		SDL_FillRect(surface,NULL,SDL_MapRGB(surface->format,color.r,color.g,color.b));

		SDL_Rect temp;
		if(isHoriz)
		{
			temp.x = (int)(value * sliderWidth);
			temp.y = 0;
			if(sliderWidth < 0.5)
				temp.w = 1;
			else
				temp.w = (Uint16)sliderWidth;

			temp.h = rect.h;
			if(value == scale)
				temp.x = (Sint16)(rect.x - sliderWidth + 1);
		}
		else
		{
			temp.x = 0;
			temp.y = (int)(value * sliderWidth);
			if(sliderWidth < 0.5)
				temp.h = 1;
			else
				temp.h = (Uint16)sliderWidth;

			temp.w = rect.w;
			if(value == scale - 1)
				temp.y = (Sint16)(rect.h - sliderWidth + 1);
		}
		SDL_FillRect(surface,&temp,SDL_MapRGB(surface->format,sliderColor.r,sliderColor.g,sliderColor.b));
		reDraw = false;
	}
	SDL_BlitSurface(surface,NULL,screen,&rect);
}