
#include "Engine.h"

Sprite::Sprite()
{
	m_Surface = NULL;
}

bool Sprite::Create(std::string FileName, bool WithKey, int r, int g, int b)
{
	Logger::GetInstance().Print("Sprite::Create(%s)", FileName.c_str());

	m_FileName = FileName;

	if (m_Surface != NULL)
	{
		Destroy();
	}

	m_Surface = IMG_Load(m_FileName.c_str());

	if (WithKey == true)
	{
		unsigned int ColourKey = SDL_MapRGB(m_Surface->format, r, g, b);
		SDL_SetColorKey(m_Surface, SDL_SRCCOLORKEY, ColourKey);
	}

	SDL_Surface * temp = SDL_DisplayFormat(m_Surface);

	if (temp != NULL)
	{
		SDL_FreeSurface(m_Surface);
		m_Surface = temp;
	}

	return true;
}

void Sprite::Destroy()
{
	if (m_Surface)
	{
		Logger::GetInstance().Print("Sprite::Destroy(): %s", m_FileName.c_str());

		SDL_FreeSurface(m_Surface);
		m_Surface = NULL;
	}
}

void Sprite::Blt(int x, int y)
{
	SDL_Rect SrcRect;
	SDL_Rect DestRect;

	SrcRect.x = 0;
	SrcRect.y = 0;
	SrcRect.w = m_Surface->w;
	SrcRect.h = m_Surface->h;
	DestRect.x = x;
	DestRect.y = y;
	DestRect.w = 0;
	DestRect.h = 0;

	SDL_BlitSurface(m_Surface, &SrcRect, Engine.GetBackBuffer(), &DestRect);
}

void Sprite::Blt(int DestX, int DestY, int SrcX, int SrcY, int SrcWidth, int SrcHeight)
{
	SDL_Rect SrcRect;
	SDL_Rect DestRect;

	SrcRect.x = SrcX;
	SrcRect.y = SrcY;
	SrcRect.w = SrcWidth;
	SrcRect.h = SrcHeight;
	DestRect.x = DestX;
	DestRect.y = DestY;
	DestRect.w = SrcWidth;
	DestRect.h = SrcHeight;

	SDL_BlitSurface(m_Surface, &SrcRect, Engine.GetBackBuffer(), &DestRect);
}