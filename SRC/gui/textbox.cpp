#include "textbox.h"

textBox::textBox():guiBase()
{
	color.r = 0;
	color.g = color.b = 100;
}

int textBox::update(SDL_Event &_event,int handled)
{	
	if(handled == HANDLED)
		return(UNHANDLED);

	if(_event.type == SDL_MOUSEBUTTONDOWN)
		return(HANDLED);
	else if(_event.type == SDL_KEYDOWN)
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
			if(foreground != NULL && foreground->w > rect.w)
			{
				text = text.substr(0,text.length() - 1);
				SDL_FreeSurface(foreground);
				foreground = TTF_RenderText_Solid(ttfFont,text.c_str(),fontColor);
			}
		}
		else if(_event.key.keysym.sym == SDLK_BACKSPACE && text.length() > 0)
		{
			text = text.substr(0,text.length() - 1);
			SDL_FreeSurface(foreground);
			foreground = TTF_RenderText_Solid(ttfFont,text.c_str(),fontColor);
		}
	}
	return(UNHANDLED);
}

void textBox::render()
{
	if(hidden)
		return;

	if(reDraw)
	{
		SDL_FillRect(surface,NULL,SDL_MapRGB(surface->format,color.r,color.g,color.b));
		hLine(0,0,rect.w,SDL_MapRGB(surface->format,0,0,0));
		vLine(0,1,rect.h - 2,SDL_MapRGB(surface->format,0,0,0));
		hLine(0,rect.h - 1,rect.w,SDL_MapRGB(surface->format,255,255,255));
		vLine(rect.w - 1,1,rect.h - 2,SDL_MapRGB(surface->format,255,255,255));
		reDraw = false;
		if(foreground != NULL)
		{
			SDL_Rect tmpRect = {2,2,0,0};
			SDL_BlitSurface(foreground,NULL,surface,&tmpRect);
		}
	}
	SDL_BlitSurface(surface,NULL,screen,&rect);
}