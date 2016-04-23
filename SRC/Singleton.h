
#ifndef _SINGLETON_H_
#define _SINGLETON_H_

template <typename T> class Singleton
{
public:
	static T& GetInstance( void )
	{
		static T singleton;
		return singleton;
	}
};

#endif