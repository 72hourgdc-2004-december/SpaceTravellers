
#ifndef _SPRITE_H_
#define _SPRITE_H_

class Sprite
{
public:
	std::string m_FileName;
	SDL_Surface * m_Surface;

	Sprite();

	bool Create(std::string FileName, bool WithKey = false, int r = 0, int g = 0, int b = 0);
	void Destroy();

	void Blt(int x, int y);

	void Blt(int DestX, int DestY, int SrcX, int SrcY, int SrcWidth, int SrcHeight);
};

#endif
