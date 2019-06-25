#ifndef __data_type_h
#define __data_type_h
#include<iostream>
using namespace std;
template <class T>
class Node{
    public:
        T data;
        Node<T> *next;
};

class mystring{
	private:
		char *data;
	public:
		mystring();
		mystring(const char *);
		~mystring();
		mystring & operator=(const char *);
		int operator==(const char *);
		int operator!=(const char *);
		friend ostream & operator <<(ostream &,mystring &);
};
#endif
