#include "guibase.h"

const SDL_Color black = { 0, 0, 0, 0 };

guiBase::guiBase()
{
	rect.x = rect.y = 0;
	rect.w = rect.h = 5;
	surface = foreground = NULL;
	parent = NULL;
	ttfFont = NULL;
	handler = NULL;
	reDraw = true;
	hidden = lastStatus = focusFlag = isPic = useSrcRect = false;
	text = "";
	setFont("comic.ttf");
	setRect(rect);
}

guiBase::~guiBase()
{
	SDL_FreeSurface(surface);
	if(ttfFont != NULL)
		TTF_CloseFont(ttfFont);
	if(!isPic)
		SDL_FreeSurface(foreground);
}


void guiBase::setColor(SDL_Color &_color)
{
	color = _color;
	reDraw = true;
	if(parent!= NULL)
		parent->setRedraw(true);
}

void guiBase::setPic(SDL_Surface *_pic)
{
	if(!isPic)
		SDL_FreeSurface(foreground);
	foreground = _pic;
//	SDL_SetColorKey(foreground,SDL_SRCCOLORKEY,SDL_MapRGB(foreground->format,255,0,255));
	isPic = reDraw = true;
}

void guiBase::setFocus(bool _value)
{
	if(focusFlag != _value)	// Never set reDraw to false here.
		reDraw = true;
	focusFlag = _value;
}

void guiBase::setHidden(bool _value)
{
	hidden = _value;
	if(parent != NULL)
		parent->setRedraw(true);
}

void guiBase::setFontColor(SDL_Color &_color)
{
	reDraw = true;
	fontColor = black;
	if(parent != NULL)
		parent->setRedraw(true);
	if(ttfFont != NULL)
	{
		if(!isPic)
			SDL_FreeSurface(foreground);
		foreground = TTF_RenderText_Solid(ttfFont,text.c_str(),fontColor);
		isPic = false;
	}
}


guiBase::coord guiBase::getParentDims()
{
	coord temp = {0,0};
	if(parent != NULL)
	{
		temp = parent->getParentDims();
		temp.x += rect.x;
		temp.y += rect.y;
	}
	else
	{
		temp.x = temp.y = 0;
	}
	return(temp);
}


// Determines if the coordinate is within the dimensions of the object.
bool guiBase::inDims(int x,int y)
{
	coord temp;
	if(parent != NULL)
		temp = parent->getParentDims();
	else
		temp.x = temp.y = 0;

	return(x >= temp.x + rect.x && x < temp.x + rect.x + rect.w && y >= temp.y + rect.y && y < temp.y + rect.y + rect.h);
}


// Maps absolute coordinates to local object coordinates.
guiBase::coord guiBase::mapToLocal(int x,int y)
{
	coord temp;
	if(parent != NULL)
		temp = parent->getParentDims();
	else
		temp.x = temp.y = 0;
	temp.x = x - temp.x - rect.x;
	temp.y = y - temp.y - rect.y;
	return(temp);
}


void guiBase::setText(string _text)
{
	reDraw = true;
	if(parent != NULL)
		parent->setRedraw(true);
	text = _text;
	if(ttfFont != NULL)
	{
		if(!isPic)
			SDL_FreeSurface(foreground);
		foreground = TTF_RenderText_Solid(ttfFont,text.c_str(),black);
		isPic = false;
	}
}


void guiBase::setFont(string _filename,int _point)
{
	reDraw = true;
	if(parent != NULL)
		parent->setRedraw(true);
	if(ttfFont != NULL)
		TTF_CloseFont(ttfFont);
	if((ttfFont = TTF_OpenFont(_filename.c_str(),_point)) == NULL)
		return;
	if(!isPic)
		SDL_FreeSurface(foreground);
	foreground = TTF_RenderText_Solid(ttfFont,text.c_str(),black);
	isPic = false;
}


void guiBase::setRect(SDL_Rect &_rect)
{
	rect = _rect;
	reDraw = true;
	if(parent != NULL)
		parent->setRedraw(true);
	SDL_FreeSurface(surface);
	surface = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA,rect.w,rect.h,32,0,0,0,0);
	SDL_SetColorKey(surface,SDL_SRCCOLORKEY,SDL_MapRGB(surface->format,255,0,255));
}


void guiBase::setSourceRect(int x, int y, int w, int h)
{
	useSrcRect = true;
	srcRect.x = x;
	srcRect.y = y;
	srcRect.w = w;
	srcRect.h = h;
}

// Put Pixel
// Author: From the SDL tutorials.
void guiBase::putPixel(int x,int y,Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}


// Draws a horizontal line.
void guiBase::hLine(int x,int y,int len,Uint32 color)
{
	for(int i = x;i < x + len;i++)
		putPixel(i,y,color);
}

// Draws a vertical line.
void guiBase::vLine(int x,int y,int len,Uint32 color)
{
	for(int i = y;i < y + len;i++)
		putPixel(x,i,color);
}