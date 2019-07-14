#ifndef __data_type_h
#define __data_type_h
#include<iostream>
using namespace std;
enum{
	FALSE=0,
	TRUE
};
template <class T>
class Node{
    public:
        T data;
        Node<T> *next;
};
template <class T>
class BiNode{
    public:
        T data;
        BiNode<T> *left;
		BiNode<T> *right;
		int bf;
};
class mystring{
	private:
		char *data;
	public:
		mystring();
		mystring(const char *);
		mystring(const mystring &);
		~mystring();
		mystring & operator=(const char *);
		int operator==(const char *);
		int operator!=(const char *);
		int operator>(const char *);
		int operator<(const char *);

		int operator==(const mystring &);
		int operator!=(const mystring &);
		int operator>(const mystring &);
		int operator<(const mystring &);
		friend ostream & operator <<(ostream &,mystring &);
};
#endif
