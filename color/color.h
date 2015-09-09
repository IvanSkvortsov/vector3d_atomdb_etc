#ifndef __COLOR_H__
#define __COLOR_H__
#include"/home/Ivan/cpp/lib/stoi/stoi.h"
#include"/home/Ivan/cpp/lib/itos/itos.h"

#define __color_rgb_print__
#ifdef  __color_rgb_print__
#include<iostream>
#include<iomanip>
#endif

struct color_rgb
{
	typedef unsigned int value_type;
	typedef unsigned char byte_type;
	value_type _rgb;
	// color_rgb
	color_rgb():_rgb(0u){}
	color_rgb(color_rgb const & v):_rgb( v._rgb ){}
	explicit color_rgb(value_type const & v):_rgb( v ){}
	explicit color_rgb(std::string const & s):_rgb( stoi( s, 16 ) ){}
	color_rgb(value_type __r, value_type __g, value_type __b):_rgb( (__b&0xff) | ((__g&0xff)<<8u) | ((__r&0xff)<<16u) ){}
	// rgb
	value_type & rgb(){return _rgb;}
	value_type const & rgb()const{return _rgb;}
	// operator value_type()
	operator value_type()const{return _rgb;}
	// red, green, blue
	value_type red()const{return (_rgb>>16u)&0xff;}
	value_type green()const{return (_rgb>>8u)&0xff;}
	value_type blue()const{return _rgb&0xff;}
	void red(value_type const & __r)
	{
		byte_type * p = (byte_type *)&_rgb+2;
		*p = byte_type(__r&0xff);
	}
	void green(value_type const & __g)
	{
		byte_type * p = (byte_type *)&_rgb+1;
		*p = byte_type(__g&0xff);
	}
	void blue(value_type const & __b)
	{
		byte_type * p = (byte_type *)&_rgb;
		*p = byte_type(__b&0xff);
	}
	// init
	void init(value_type __r, value_type __g, value_type __b)
	{
		_rgb = (__b&0xff) | ((__g&0xff)<<8u) | ((__r&0xff)<<16u);
	}
	void init(std::string const & __rgb)
	{
		_rgb = stoi( __rgb, 16 );
	}
	void operator()(value_type __r, value_type __g, value_type __b)
	{
		_rgb = (__b&0xff) | ((__g&0xff)<<8u) | ((__r&0xff)<<16u);
	}
	void operator()(std::string const & __rgb)
	{
		_rgb = stoi( __rgb, 16 );
	}
	// toString
	std::string toString()const
	{
		std::string s;
		itos( _rgb, s, 16 );
		std::string buf;
		if( s.size() < 6 )
			buf.assign( 6-s.size(), '0');
		return "0x" + buf + s;
	}
	void pub_print()const
	{
#ifdef  __color_rgb_print__
		this->print(std::cout);
#endif
	}
private:
#ifdef  __color_rgb_print__
	void print(std::ostream & out)const
	{
		int w = 10;
		out << std::setw( w ) << this->toString();
	}
#endif
};

#endif//__COLOR_H__
