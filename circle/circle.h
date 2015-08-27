#ifndef __CIRCLE_H__
#define __CIRCLE_H__
#include"../center/center.3d.h"

template<class T>
struct circle
{
	T _r;
	center_3d<T> c;
	circle():_r(1), c(0, 0, 0){}
	circle(T const & __r, center_3d<T> const & __c) : _r( __r ), c( __c ) {}
	circle(T const & __r, T const & __x, T const & __y, T const & __z): _r( __r ), c( __x, __y, __z ) {}
	T & r(){return _r;}
	T & x(){return c[0];}
	T & y(){return c[1];}
	T & z(){return c[2];}
	T const & r()const{return _r;}
	T const & x()const{return c[0];}
	T const & y()const{return c[1];}
	T const & z()const{return c[2];}
};

#endif//__CIRCLE_H__
