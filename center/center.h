#ifndef __CENTER_H__
#define __CENTER_H__
#include<cstddef>// size_t

#define __center_print__
#ifdef  __center_print__
#include<iostream>
#include<iomanip>
#endif

template<class T, std::size_t sz = std::size_t(3)>
struct center
{
	typedef T value_type;
	typedef center<T, sz> center_type;
	T r[sz];
	center(){}
	center(center_type const & v)
	{
		for(int i = 0; i < this->size(); ++i) r[i] = v[i];
	}
	~center(){}
	center_type & operator=(center_type const & v)
	{
		if( this == &v ) return *this;
		for(int i = 0; i < this->size(); ++i) r[i] = v[i];
		return *this;
	}
	std::size_t size()const{return sz;}
	value_type & operator[](int i){return r[i];}
	value_type const & operator[](int i)const{return r[i];}
	void pub_print()const
	{
#ifdef  __center_print__
		this->print(std::cout);
#endif
	}
private:
#ifdef  __center_print__
	void print(std::ostream & out)const
	{
		int prec = 10, w = prec + 8;
		out.setf( std::ios::scientific );
		for(int i = 0; i < this->size(); ++i)
			out << std::setw( w ) << std::setprecision( prec ) << r[i];
		out.unsetf( std::ios::scientific );
	}
#endif
};

#endif//__CENTER_H__
