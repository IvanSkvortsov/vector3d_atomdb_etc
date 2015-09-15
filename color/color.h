#ifndef __COLOR_H__
#define __COLOR_H__
#include"/home/Ivan/cpp/lib/stoi/stoi.h"
#include"/home/Ivan/cpp/lib/itos/itos.h"

#define __color_rgb_print__
#ifdef  __color_rgb_print__
#include<iostream>
#include<iomanip>
#endif

#define __color_rgb_err_msg__
#ifdef  __color_rgb_err_msg__
#include<iostream>
#include<cstdlib>// exit
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
	//color_rgb(value_type const & v):_rgb( v ){}
	//color_rgb(std::string const & s):_rgb( stoi( s, 16 ) ){}
	color_rgb(value_type __r, value_type __g, value_type __b):_rgb( (__b&0xff) | ((__g&0xff)<<8u) | ((__r&0xff)<<16u) ){}
	// operator=
	color_rgb & operator=(color_rgb const & v)
	{
		if( this == &v ) return *this;
		_rgb = v.rgb();
		return *this;
	}
	color_rgb & operator=(value_type const & v)
	{
		_rgb = v;
		return *this;
	}
	color_rgb & operator=(std::string const & s)
	{
		_rgb = stoi( s, 16 );
		return *this;
	}
	// operator+=
	color_rgb & operator+=(color_rgb const & v)
	{
		value_type __red = this->red() + v.red(), __green = this->green() + v.green(), __blue = this->blue() + v.blue();
		if( __red > 0xff ) __red = 0xff;
		if( __green > 0xff ) __green = 0xff;
		if( __blue > 0xff ) __blue = 0xff;
		this->init( __red, __green, __blue );
		return *this;
	}
	// operator-=
	color_rgb & operator-=(color_rgb const & v)
	{
		value_type __red = this->red() - v.red(), __green = this->green() - v.green(), __blue = this->blue() - v.blue();
		if( __red > 0xff ) __red = 0;
		if( __green > 0xff ) __green = 0;
		if( __blue > 0xff ) __blue = 0;
		this->init( __red, __green, __blue );
		return *this;
	}
	/*
	// operator*=
	color_rgb & operator*=(value_type const & d)
	{
		value_type __red = this->red() * d, __green = this->green() * d, __blue = this->blue() * d;
		if( __red > 0xff ) __red = 0xff;
		if( __green > 0xff ) __green = 0xff;
		if( __blue > 0xff ) __blue = 0xff;
		this->init( __red, __green, __blue );
		return *this;
	}
	// operator/=
	color_rgb & operator/=(value_type const & d)
	{
		if( d == 0 )
		{
#ifdef  __color_rgb_err_msg__
			std::cerr << "Error : color_rgb::operator/=(value_type const & )" << std::endl;
			std::cerr << "d : " << d << std::endl;
			std::exit(1);
#endif
			return *this;
		}
		value_type __red = this->red() / d, __green = this->green() / d, __blue = this->blue() / d;
		if( __red > 0xff ) __red = 0xff;
		if( __green > 0xff ) __green = 0xff;
		if( __blue > 0xff ) __blue = 0xff;
		this->init( __red, __green, __blue );
		return *this;
	}
	*/
	// operator*=
	color_rgb & operator*=(double const & d)
	{
		if( d < double(0) )
		{
#ifdef  __color_rgb_err_msg__
			std::cerr << "Error : color_rgb::operator*=(double const & )" << std::endl;
			std::cerr << "d : " << d << std::endl;
			std::exit(1);
#endif
			return *this;
		}
		value_type __red = double(this->red()) * d, __green = double(this->green()) * d, __blue = double(this->blue()) * d;
		if( __red > 0xff ) __red = 0xff;
		if( __green > 0xff ) __green = 0xff;
		if( __blue > 0xff ) __blue = 0xff;
		this->init( __red, __green, __blue );
		return *this;
	}
	// operator/=
	color_rgb & operator/=(double const & d)
	{
		if( d <= double(0) )
		{
#ifdef  __color_rgb_err_msg__
			std::cerr << "Error : color_rgb::operator/=(double const & )" << std::endl;
			std::cerr << "d : " << d << std::endl;
			std::exit(1);
#endif
			return *this;
		}
		value_type __red = double(this->red()) / d, __green = double(this->green()) / d, __blue = double(this->blue()) / d;
		if( __red > 0xff ) __red = 0xff;
		if( __green > 0xff ) __green = 0xff;
		if( __blue > 0xff ) __blue = 0xff;
		this->init( __red, __green, __blue );
		return *this;
	}
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
		itos( this->_rgb, s, 16 );
		std::string buf;
		if( s.size() < 6 )
			buf.assign( 6-s.size(), '0');
		//return "0x" + buf + s;
		return buf + s;
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

color_rgb operator+(color_rgb const & a, color_rgb const & b)
{
	color_rgb c( a );
	c += b;
	return c;
}

color_rgb operator-(color_rgb const & a, color_rgb const & b)
{
	color_rgb c( a );
	c -= b;
	return c;
}

color_rgb operator*(color_rgb const & a, double const & b)
{
	color_rgb c( a );
	c *= b;
	return c;
}

color_rgb operator*(double const & a, color_rgb const & b)
{
	color_rgb c( b );
	c *= a;
	return c;
}

color_rgb operator/(color_rgb const & a, double const & b)
{
	color_rgb c( a );
	c /= b;
	return c;
}

#endif//__COLOR_H__
