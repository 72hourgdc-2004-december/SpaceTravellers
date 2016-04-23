#include "console.h"

guiConsole::guiConsole():guiBase()
{
	memRecall = new scrollBar;
	memRecall->setSurface(surface);
	memRecall->setParent(this);
	memRecall->meter->setScale(1);
	fontColor.r = 0;
	fontColor.g = fontColor.b = 100;
}

guiConsole::~guiConsole()
{
	delete memRecall;
}

void guiConsole::setRect(SDL_Rect &_rect)
{
	SDL_Rect tmpRect;
	guiBase::setRect(_rect);
	tmpRect.x = rect.w - BTN_HEIGHT;
	tmpRect.y = 0;
	tmpRect.w = BTN_HEIGHT;
	tmpRect.h = rect.h;
	memRecall->setRect(tmpRect);
	memRecall->setSurface(surface);
}

int guiConsole::update(SDL_Event &_event,int handled)
{
	if(handled == HANDLED)
		return(UNHANDLED);

	memRecall->update(_event,handled);
	if(SDL_GetMouseState(NULL,NULL) & SDL_BUTTON(1) || _event.type == SDL_MOUSEBUTTONUP)
		reDraw = true;
	if(_event.type == SDL_KEYDOWN)
	{
		reDraw = true;
		if(_event.key.keysym.sym >= SDLK_SPACE && _event.key.keysym.sym <= SDLK_z)
		{
			if(_event.key.keysym.mod & (KMOD_LSHIFT | KMOD_RSHIFT | KMOD_CAPS) && _event.key.keysym.sym >= SDLK_a)
			{
				text += _event.key.keysym.sym - 32;
			}
			else
			{
				if(_event.key.keysym.mod & (KMOD_LSHIFT | KMOD_RSHIFT))
				{
					switch(_event.key.keysym.sym)
					{
					case SDLK_1:
						text += '!';
						break;
					case SDLK_2:
						text += '@';
						break;
					case SDLK_3:
						text += '#';
						break;
					case SDLK_4:
						text += '$';
						break;
					case SDLK_5:
						text += '%';
						break;
					case SDLK_6:
						text += '^';
						break;
					case SDLK_7:
						text += '&';
						break;
					case SDLK_8:
						text += '*';
						break;
					case SDLK_9:
						text += '(';
						break;
					case SDLK_0:
						text += ')';
						break;
					case SDLK_BACKQUOTE:
						text += '~';
						break;
					case SDLK_MINUS:
						text += '_';
						break;
					case SDLK_EQUALS:
						text += '+';
						break;
					case SDLK_BACKSLASH:
						text += '|';
						break;
					case SDLK_LEFTBRACKET:
						text += '{';
						break;
					case SDLK_RIGHTBRACKET:
						text += '}';
						break;
					case SDLK_SEMICOLON:
						text += ':';
						break;
					case SDLK_QUOTE:
						text += "\"";
						break;
					case SDLK_COMMA:
						text += '<';
						break;
					case SDLK_PERIOD:
						text += '>';
						break;
					case SDLK_SLASH:
						text += '?';
						break;
					};
				}
				else
					text += _event.key.keysym.sym;
			}

			SDL_FreeSurface(foreground);
			foreground = TTF_RenderText_Solid(ttfFont,text.c_str(),fontColor);
			if(foreground != NULL && foreground->w > rect.w - memRecall->getRect().w)
			{
				text = text.substr(0,text.length() - 1);
				SDL_FreeSurface(foreground);
				foreground = TTF_RenderText_Solid(ttfFont,text.c_str(),fontColor);
			}
		}
		else if(_event.key.keysym.sym == SDLK_RETURN && text != "")
		{
			memory.push_back(text);
			text = "";
			SDL_FreeSurface(foreground);
			foreground = TTF_RenderText_Solid(ttfFont,text.c_str(),fontColor);
			memRecall->meter->setScale((int)memory.size());
			memRecall->meter->setPos((int)memory.size()-1);
		}
		else if(_event.key.keysym.sym == SDLK_BACKSPACE && text.length() > 0)
		{
			text = text.substr(0,text.length() - 1);
			SDL_FreeSurface(foreground);
			foreground = TTF_RenderText_Solid(ttfFont,text.c_str(),fontColor);
		}
		return(HANDLED);
	}
	return(UNHANDLED);
}

void guiConsole::render()
{
	if(hidden)
		return;

	if(reDraw)
	{
		SDL_FillRect(surface,NULL,SDL_MapRGB(surface->format,255,0,255));
		SDL_Rect rcDest = {1,0,0,0};
		if(foreground != NULL)
			rcDest.y = rect.h - foreground->h;
		else
		{
			SDL_FreeSurface(foreground);
			foreground = TTF_RenderText_Solid(ttfFont," ",fontColor);
			rcDest.y = rect.h - foreground->h;
			SDL_FreeSurface(foreground);
			foreground = TTF_RenderText_Solid(ttfFont,text.c_str(),fontColor);
		}
		SDL_BlitSurface(foreground,NULL,surface,&rcDest);
		SDL_Surface *tmpSurf;
		int temp = rcDest.y;
		if(memory.size() > 0)
			for(int i = memRecall->meter->getPos();i >= 0 && temp >= 0;i--)
			{
				tmpSurf = TTF_RenderText_Solid(ttfFont,memory[i].c_str(),fontColor);
				if(tmpSurf != NULL)
					rcDest.y = temp -= tmpSurf->h;
				if(temp >= 0) // Must use temp b/c rcDest is unsigned.
					SDL_BlitSurface(tmpSurf,NULL,surface,&rcDest);
				SDL_FreeSurface(tmpSurf);
			}
		reDraw = false;
	}
	memRecall->render();
	SDL_BlitSurface(surface,NULL,screen,&rect);
}


guiConsole &guiConsole::operator<<(const string &_str)
{
	if(_str == cendl)
	{
		memory.push_back(text);
		text = "";
		memRecall->meter->setScale((int)memory.size());
		memRecall->meter->setPos((int)memory.size() - 1);
	}
	else
		text += _str;
	SDL_FreeSurface(foreground);
	foreground = TTF_RenderText_Solid(ttfFont,text.c_str(),fontColor);
	reDraw = true;
	return(*this);
}

guiConsole &guiConsole::operator<<(const int &_val)
{
	char temp[20];
	itoa(_val,temp,10);
	text += temp;
	SDL_FreeSurface(foreground);
	foreground = TTF_RenderText_Solid(ttfFont,text.c_str(),fontColor);
	reDraw = true;
	return(*this);
}

guiConsole &guiConsole::operator<<(const double &_val)
{
	char temp[80];
	_gcvt(_val,10,temp);
	text += temp;
	SDL_FreeSurface(foreground);
	foreground = TTF_RenderText_Solid(ttfFont,text.c_str(),fontColor);
	reDraw = true;
	return(*this);
}