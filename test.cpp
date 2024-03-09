// #include "iterator.hpp"
// #include "type_traits.hpp"
// #include "rbtree.hpp"
// #include "pair.hpp"
// #include "stl_function.hpp"
// #include "lexicographical_compare.hpp"
#include "vector.hpp"
#include "set.hpp"
#include "map.hpp"
#include "stack.hpp"
#include <map>
#include <vector>
#include <set>
#include <deque>
#include <stack>
#include <string>

#include <sstream>
#include <stdlib.h>
#include <time.h>


#define BUFFER_SIZE 4096
struct Buffer
{
	int idx;
	char buff[BUFFER_SIZE];
};

// #define	size_ 100

// int main(void)
// {
// 	ft::vector<ft::pair<int, int> >	vector;
// 	ft::_Rb_tree<int, ft::pair<int, int>, ft::_Select1st<ft::pair<int, int> >, ft::__less<int> >	rbTree;
// 	int l = 0;

// 	for (int k = 0 ; l < size_ ; l = 2 * k)
// 	{
// 		ft::pair<int, int> tmp(l, l);
// 		vector.push_back(tmp);
// 		ft::pair<int, int> tmp1(size_ - 1 - l, size_ - 1 - l);
// 		vector.push_back(tmp1);
// 		++k;
// 	}
// 	l = 0;
// 	for (ft::vector<ft::pair<int, int> >::iterator iter = vector.begin() ; iter != vector.end() ; iter++)
// 	{
// 		std::cout << l++ << " - key : " << (*iter).first << " value : " << (*iter).second << std::endl;
// 	}
// 	for (ft::vector<ft::pair<int, int> >::iterator iter = vector.begin() ; iter != vector.end() ; iter++)
// 	{
// 		rbTree.insert_unique((*iter));
// 	}
// 	if (rbTree.__rb_verify() != 0)
// 		std::cout << "verify : " << rbTree.__rb_verify() << std::endl;
// 	else
// 	{
// 		std::cerr << "error" << std::endl;
// 		exit (EXIT_FAILURE);
// 	}

// 	// std::istringstream str("1 2 3 4 5 6 7");
// 	// std::istreambuf_iterator<char> it(str), end;

	
// 	// ft::vector<char> v;
// 	// v.assign(it, end);


// 	// for (ft::vector<char>::iterator iter = v.begin() ; iter != v.end() ; iter++)
// 	// {
// 	// 	std::cout << l++ << " - key : " << (*iter) << std::endl;
// 	// }



// 	int tmp;


// 	// int k = 0;
// 	// if (k < size_ / 2)
// 	// 	tmp = (size_ / 2) + k;
// 	// else
// 	// 	tmp = size_ - 1 - k;
// 	// ft::_Rb_tree<int, ft::pair<int, int>, ft::_Select1st<ft::pair<int, int> >, ft::__less<int> >::iterator	it = rbTree.find(tmp);
// 	// ft::_Rb_tree<int, ft::pair<int, int>, ft::_Select1st<ft::pair<int, int> >, ft::__less<int> >::iterator	it2 = it;
// 	// // vector.pop_back();
// 	// ft::advance(it2, 2);
// 	// std::cout << "1 : " << (*it).first << " 2 : " << (*it2).first << std::endl;
// 	// rbTree.erase(it, it2);
// 	// std::cout << "root : " << rbTree.root_value() << std::endl;
// 	// for (ft::_Rb_tree<int, ft::pair<int, int>, ft::_Select1st<ft::pair<int, int> >, ft::__less<int> >::iterator iter = rbTree.begin() ; iter != rbTree.end() ; iter++)
// 	// {
// 	// 	std::cout << "key : " << rbTree.color(iter)  << " value : " << (*iter).second << std::endl;
// 	// 	std::cout << "\x1b[44mchild\x1b[0m : LeftChile " << rbTree.LeftChildcolor(iter)  << " RightChile " << rbTree.RightChildcolor(iter) << std::endl;
// 	// 	std::cout << "\x1b[42mchild\x1b[0m : LeftColor " << rbTree.Leftcolor(iter)  << " RightColor " << rbTree.Rightcolor(iter) << std::endl;
// 	// }
// 	// if (rbTree.__rb_verify() != 0)
// 	// 	std::cout << "\x1b[43mverify\x1b[0m : " << rbTree.__rb_verify() << std::endl;
// 	// else
// 	// {
// 	// 	std::cerr << "error" << std::endl;
// 	// 	exit (EXIT_FAILURE);
// 	// }

// 	for (int k = 0 ; k < size_ ; k++)
// 	{
// 		if (k < size_ / 2)
// 			tmp = (size_ / 2) + k;
// 		else
// 			tmp = size_ - 1 - k;
// 		ft::_Rb_tree<int, ft::pair<int, int>, ft::_Select1st<ft::pair<int, int> >, ft::__less<int> >::iterator	it = rbTree.find(tmp);
// 		// vector.pop_back();
// 		// ft::advance(it2, 2);
// 		// std::cout << "1 : " << (*it).first << " 2 : " << (*it2).first << std::endl;
// 		rbTree.erase(it);
// 		std::cout << "root : " << rbTree.root_value() << std::endl;
// 		for (ft::_Rb_tree<int, ft::pair<int, int>, ft::_Select1st<ft::pair<int, int> >, ft::__less<int> >::iterator iter = rbTree.begin() ; iter != rbTree.end() ; iter++)
// 		{
// 			std::cout << "key : " << rbTree.color(iter)  << " value : " << (*iter).second << std::endl;
// 			std::cout << "\x1b[44mchild\x1b[0m : LeftChile " << rbTree.LeftChildcolor(iter)  << " RightChile " << rbTree.RightChildcolor(iter) << std::endl;
// 			std::cout << "\x1b[42mchild\x1b[0m : LeftColor " << rbTree.Leftcolor(iter)  << " RightColor " << rbTree.Rightcolor(iter) << std::endl;
// 		}
// 		if (rbTree.__rb_verify() != 0)
// 			std::cout << "\x1b[43mverify\x1b[0m : " << rbTree.__rb_verify() << std::endl;
// 		else
// 		{
// 			std::cerr << "error" << std::endl;
// 			exit (EXIT_FAILURE);
// 		}
// 	}
// 	return (0);
// }

int main(void)
{
	srand((unsigned)time(NULL));
	{
		ft::vector<int> ft_vector_int;
		ft::stack<int> ft_stack_int;
		ft::map<int, int> ft_map_int;
		ft::set<int, int> ft_set_int;

		std::vector<int> std_vector_int;
		std::stack<int> std_stack_int;
		std::map<int, int> std_map_int;
		std::set<int, int> std_set_int;

		for (int i = 0 ; i < 10 ; i++)
		{
			int tmp = rand();
		}
	}
}