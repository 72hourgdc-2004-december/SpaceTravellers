
#ifndef _POINT_H_
#define _POINT_H_

class Point
{
public:
	int x;
	int y;

public:
	Point() : x(0), y(0)
	{
	}

	Point(int x, int y) : x(x), y(y)
	{
	}

	Point operator+= (const Point & rhs)
	{
		x += rhs.x;
		y += rhs.y;

		return *this;
	}

	bool operator== (const Point & rhs)
	{
		return (x == rhs.x && y == rhs.y);
	}
};

#endif
