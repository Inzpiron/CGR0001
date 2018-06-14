#ifndef __UTILITIES__
#define __UTILITIES__

#include <string>

void ToLowerString(std::string &str)
{
	for (auto &i : str)
		if (i >= 'A' && i <= 'Z') i += 32;
}

#endif
