/*
 * test.cpp
 *
 *  Created on: Jul 5, 2016
 *      Author: Siva
 */

#include <iostream>
#include <vector>

using namespace std;

#if 0
class Test
{
private:
	float a;

public:
	Test(int a1)
	{
		this->a = a1;
	}
	float get_a () const
	{
		return this->a;
	}

};

template <typename T>
class TemplateTest
{
private:
	T a;
public:
	TemplateTest(T a1)
	{
		this->a = a1;
	}
	T get_a () const
	{
		return this->a;
	}
};

template <typename T>
class VectorTemplateTest
{
private:
	vector<T> a;
public:
	VectorTemplateTest(vector<T> a1)
	{
		this->a = a1;
	}
	vector<T> get_a () const
	{
		return this->a;
	}
};

ostream& operator<< (ostream &os, const Test &t)
{
	os << "a = " << t.get_a() << endl;
	return os;
}

template<typename T>
ostream& operator<< (ostream &os, const TemplateTest<T>& t)
{
	os << "a = " << t.get_a() << endl;
	return os;
}

template<typename T>
ostream& operator<< (ostream &os, const VectorTemplateTest<T>& t)
{
	os << "a = ";
	for (typename vector<T>::iterator it = t.get_a().begin(); it != t.get_a().end(); it++)
	{
		os << *it << " ";
	}
	os << endl;
	return os;
}


int main() {

	cout << "Operator overloading" << endl; // prints !!!Hello World!!!
	/*
	 * read a query points file and output the indices of nearest points to
	 * each query point
	 *
	 * while file not empty
	 * 	read_point()
	 * 	nearest_point = kdtree->find_nearest_point(query_point)
	 * 	print the index of nearest_point.
	 */
	Test t(1);
	cout << t << endl;
	TemplateTest<float> template_t(2);
	cout << template_t << endl;

	vector<float> f;
	f.push_back(1);
	f.push_back(2);
	VectorTemplateTest<float> vectortemplate_t(f);
	cout << vectortemplate_t << endl;

	return 0;
}
#endif
