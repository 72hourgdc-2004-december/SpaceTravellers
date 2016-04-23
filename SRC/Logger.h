
#ifndef _LOGGER_H_
#define _LOGGER_H_

#include "Singleton.h"
#include <fstream>
#include <string>
#include <stdarg.h>

class Logger : public Singleton < Logger >
{
public:
	Logger()
	{
		std::ofstream OutFile("log.txt");
		OutFile.close();
	}

	void Logger::Print(const std::string msg)
	{
		std::ofstream OutFile("log.txt", std::ios::app);
		OutFile << msg << std::endl;
		OutFile.close();
	}

	void Logger::Print(const char * fmt, ...)
	{
		const char *p;
		va_list argp;
		int i;
		double f;
		char *s;

		std::stringstream OutputString;

		va_start(argp, fmt);

		for(p = fmt; *p != '\0'; p++)
		{
			if(*p != '%')
			{
				char c = *p;
				OutputString << c;
				continue;
			}

			switch(*++p)
			{
			case 'c':
				i = va_arg(argp, int);
				OutputString << i;
				break;

			case 'f':
				f = va_arg(argp, double);
				OutputString << f;
				break;

			case 'd':
				i = va_arg(argp, int);
				OutputString << i;
				break;

			case 's':
				s = va_arg(argp, char *);
				OutputString << s;
				break;

			case 'x':
				i = va_arg(argp, int);
				OutputString << i;
				break;

			case '%':
				OutputString << "%";
				break;
			}
		}

		va_end(argp);
		OutputString << std::ends;

		std::ofstream OutFile("log.txt", std::ios::app);
		OutFile << OutputString.str() << std::endl;
		OutFile.close();
	};
};

#define LOG Logger::GetInstance()

#endif
