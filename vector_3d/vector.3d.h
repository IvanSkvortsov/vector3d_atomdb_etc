#ifndef __VECTOR_H__
#define __VECTOR_H__
#include"../center/center.3d.h"
#include<cmath>// sqrt

template<class T>
struct vector_3d : center_3d<T>
{
	vector_3d(): center_3d<T>(){}
	vector_3d(T const & r0, T const & r1, T const & r2): center_3d<T>( r0, r1, r2 ) {}
	vector_3d(vector_3d<T> const & v): center_3d<T>( v ) {}
	~vector_3d(){}
	T sqr_len()const{ return this->r[0] * this->r[0] + this->r[1] * this->r[1] + this->r[2] * this->r[2]; }
	T len()const{ return std::sqrt( this->sqr_len() ); }
	vector_3d<T> & operator=(vector_3d<T> const & v)
	{
		this->center_3d<T>::operator=( v );
		return *this;
	}
	vector_3d<T> & operator+=( vector_3d<T> const & v )
	{
		for(int i = 0; i < 3; ++i) this->r[i] += v[i];
		return *this;
	}
	vector_3d<T> & operator-=( vector_3d<T> const & v )
	{
		for(int i = 0; i < 3; ++i) this->r[i] -= v[i];
		return *this;
	}
	vector_3d<T> & operator*=( T const & v )
	{
		for(int i = 0; i < 3; ++i) this->r[i] *= v;
		return *this;
	}
	vector_3d<T> & operator/=( T const & v )
	{
		T tmp = 1/v;
		for(int i = 0; i < 3; ++i) this->r[i] *= tmp;
		return *this;
	}
	T scalar( vector_3d<T> const & v)const
	{
		return this->r[0] * v[0] + this->r[1] * v[1] + this->r[2] * v[2];
	}
	static T scalar( vector_3d<T> const & a, vector_3d<T> const & b)
	{
		return a.scalar( b );
	}
	vector_3d<T> vecProd( vector_3d<T> const & v )const
	{
		vector_3d<T> tmp;
		tmp[0] = this->r[1] * v[2] - this->r[2] * v[1];
		tmp[1] = this->r[2] * v[0] - this->r[0] * v[2];
		tmp[2] = this->r[0] * v[1] - this->r[1] * v[0];
		return tmp;
	}
	static vector_3d<T> vecProd( vector_3d<T> const & a, vector_3d<T> const & b )
	{
		return a.vecProd( b );
	}
};

template<class T>
vector_3d<T> operator+( vector_3d<T> const & a, vector_3d<T> const & b )
{
	vector_3d<T> c( a );
	c += b;
	return c;
}

template<class T>
vector_3d<T> operator-( vector_3d<T> const & a, vector_3d<T> const & b )
{
	vector_3d<T> c( a );
	c -= b;
	return c;
}

template<class T>
vector_3d<T> operator*( vector_3d<T> const & a, T const & b )
{
	vector_3d<T> c( a );
	c *= b;
	return c;
}

template<class T>
vector_3d<T> operator+( T const & a, vector_3d<T> const & b )
{
	vector_3d<T> c( b );
	c *= a;
	return c;
}

template<class T>
vector_3d<T> operator/( vector_3d<T> const & a, T const & b )
{
	vector_3d<T> c( a );
	c /= b;
	return c;
}

#endif//__VECTOR_H__
