#ifndef __CENTER_3D_H__
#define __CENTER_3D_H__
#include"center.h"

template<class T>
struct center_3d : public center<T, 3>
{
	typedef center<T, 3> center_type;
	typedef typename center_type::value_type value_type;
	center_3d()
	{
		for(int i = 0; i < 3; ++i) this->r[i] = T(0);
	}
	center_3d(center_3d<T> const & v)
	{
		for(int i = 0; i < 3; ++i) this->r[i] = v[i];
	}
	center_3d(T const & r0, T const & r1, T const & r2)
	{
		this->r[0] = r0; this->r[1] = r1; this->r[2] = r2;
	}
	void operator()(T const & r0, T const & r1, T const & r2)
	{
		this->r[0] = r0; this->r[1] = r1; this->r[2] = r2;
	}
	center_3d<T> & operator=(center_3d<T> const & v)
	{
		this->center_type::operator=( v );
		return *this;
	}
};
#endif//__CENTER_3D_H__
