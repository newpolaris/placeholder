#include <vector>
#include <list>
#include <iostream>
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>

int main()
{
	using std::cout;
	using std::endl;

	std::list<int> li;
	li.push_back(3);
	li.push_back(0);
	li.push_back(1);
	li.push_back(4);
	li.push_back(2);

	for_each(li.begin(), li.end(), cout << boost::lambda::_1 << " ");

	typedef std::list<int>::iterator iter_type;

	std::vector<iter_type> vec;
	for (iter_type i=li.begin(); i != li.end(); ++i) 
		vec.push_back(i);

	std::sort(vec.begin(), vec.end(), boost::bind(std::less<int>(), _1, _2));

	return 0;
}