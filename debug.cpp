#include "debug.hpp"

void	ft_assert(int expression, const char* str)
{
	if (expression == 0)
		std::cout << str << std::endl;
	assert(expression);
}
