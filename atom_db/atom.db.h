#ifndef __ATOM_DATA_BASE_H__
#define __ATOM_DATA_BASE_H__
#include<cstdlib>// exit
#include<string>
#include<vector>
#include"/home/Ivan/cpp/lib/aux.stdlib.h"
#include<iostream>
#include<iomanip>
#include<fstream>
#include"../color/color.h"

struct atom_name : public std::vector<std::string>
{
	typedef std::string                                      value_type;
	typedef std::vector<std::string>::allocator_type         allocator_type;
	typedef std::vector<std::string>::const_reference        const_reference;
	typedef std::vector<std::string>::reference              reference;
	typedef std::vector<std::string>::pointer                pointer;
	typedef std::vector<std::string>::const_pointer          const_pointer;
	typedef std::vector<std::string>::iterator               iterator;
	typedef std::vector<std::string>::const_iterator         const_iterator;
	typedef std::vector<std::string>::reverse_iterator       reverse_iterator;
	typedef std::vector<std::string>::const_reverse_iterator const_reverse_iterator;
	typedef std::vector<std::string>::size_type              size_type;
	//
	explicit atom_name(const allocator_type& alloc = allocator_type()):std::vector<value_type>(alloc){}
	explicit atom_name(size_type n, const value_type & val = value_type(), const allocator_type & alloc = allocator_type()):
		std::vector<value_type>(n, val, alloc){}
	template<class InputIterator>
		atom_name( InputIterator first, InputIterator last, const allocator_type & alloc = allocator_type() ):
			std::vector<value_type>(first, last, alloc){}
	atom_name(atom_name const & v): std::vector<value_type>( v ){}
	//
	atom_name & operator=(atom_name const & v)
	{
		this->std::vector<value_type>::operator=( v );
		return *this;
	}
	bool is_equal(std::string const & name)const
	{
		if( !this->size() ) return false;
		std::string s = astd::tolower_s( name );
		std::string const * ps = &(this->operator[](0));
		for(int i = 0; i < this->size(); ++i)
			if( s == astd::tolower_s( *ps++ ) ) return true;
		return false;
	}
	void add(const_reference val)
	{
		if( this->is_equal(val) ) return;
		this->push_back( val ); return;
	}
	void tolower()
	{
		std::string * ps = this->data();
		for(int i = 0; i < this->size(); ++i)
		{
			*ps = astd::tolower_s( *ps );
			++ps;
		}
		ps = this->data();
		std::string * ps2 = 0;
		for(int i = 0; i < this->size(); ++i)
		{
			ps2 = ps+1;
			for(int j = i+1; j < this->size(); ++j)
			{
				if( *ps == *ps2 && ps2->size() > 0 ) ps2->clear();
				++ps2;
			}
			++ps;
		}
		ps = this->data();
		for(int i = 0; i < this->size(); ++i)
		{
			if( ps->size() > 0 ) continue;
			this->erase( this->begin() + i );
			--i;
		}
	}
};

template<class T>
struct radius_name_color
{
	typedef T                     float_type;
	typedef atom_name::value_type str_type;
	typedef atom_name::size_type  size_type;
	typedef color_rgb             color_type;
	T _radius;
	atom_name _name;
	color_rgb _colorRGB;
	// radius
	float_type & radius(){return _radius;}
	float_type const & radius()const{return _radius;}
	float_type radius(float_type const & radius_value){return _radius = radius_value;}
	// size
	size_type size()const{return _name.size();}
	void resize( size_type _size){_name.resize( _size );}
	// name
	atom_name & name(){return _name;}
	atom_name const & name()const{return _name;}
	void name(str_type const & name_value){_name.add(name_value);}
	// str
	str_type & name(size_type i){return _name[i];}
	str_type const & name(size_type i)const{return _name[i];}
	// color
	color_type & colorRGB(){return _colorRGB;}
	color_type const & colorRGB()const{return _colorRGB;}
	void colorRGB(color_type const & color_value){ _colorRGB = color_value;}
};

template<class T>
struct atom_db
{
	//std::vector<std::pair<T, atom_name> > m;
	std::vector<radius_name_color<T> > m;
	typedef radius_name_color<T>                                     value_type;
	typedef typename std::vector<value_type>::allocator_type         allocator_type;
	typedef typename std::vector<value_type>::const_reference        const_reference;
	typedef typename std::vector<value_type>::reference              reference;
	typedef typename std::vector<value_type>::pointer                pointer;
	typedef typename std::vector<value_type>::const_pointer          const_pointer;
	typedef typename std::vector<value_type>::iterator               iterator;
	typedef typename std::vector<value_type>::const_iterator         const_iterator;
	typedef typename std::vector<value_type>::reverse_iterator       reverse_iterator;
	typedef typename std::vector<value_type>::const_reverse_iterator const_reverse_iterator;
	typedef typename std::vector<value_type>::size_type              size_type;
	atom_db():m(){}
	reference operator[](atom_name::const_reference name)
	{
		int n = this->find( name );
		if( n == -1 )
		{
			err("operator[](atom_name::const_reference )") << " atom \"" << name << "\" not found in data base" << std::endl;
			std::exit(EXIT_FAILURE);
		}
		return m[n];
	}
	const_reference operator[](atom_name::const_reference name)const
	{
		int n = this->find( name );
		if( n == -1 )
		{
			err("operator[](atom_name::const_reference )") << " atom \"" << name << "\" not found in data base" << std::endl;
			std::exit(EXIT_FAILURE);
		}
		return m[n];
	}
	reference operator[](size_type i){return m[i];}
	const_reference operator[](size_type i)const{return m[i];}
	size_type size()const{return m.size();}
	pointer data(){return m.data();}
	//
	int find(atom_name::const_reference name)const
	{
		const_pointer p = this->data();
		for(int i = 0; i < this->size(); ++i)
			if( p++->name().is_equal( name ) ) return i;
		return -1;
	}
	//
	const_pointer data()const{return m.data();}
	void resize(size_type _size){m.resize( _size );}
	int import_data(char const * file)
	{
		std::ifstream inp( file );
		if( !inp.is_open() )
		{
			err("import(char const *)") << " can't open file \"" << file << '\"' << std::endl;
			return 1;
		}
		size_type sz = 0;
		inp >> sz;
		this->resize( sz );
		pointer p = this->data();
		atom_name * ap = &p->name();
		for(int i = 0; i < this->size(); ++i)
		{
			inp >> p->radius();
			inp >> sz;
			ap = &p->name();
			ap->resize( sz );
			for(int j = 0; j < ap->size(); ++j) inp >> (*ap)[j];
			if( inp == 0 )
			{
				err("import(char const *)") << "unexpected end of input, file \"" << file << '\"' << std::endl;
				return 2;
			}
			++p;
		}
		return 0;
	}
	int import_name(char const * file)
	{
		std::ifstream inp( file );
		if( !inp.is_open() )
		{
			//std::cerr << "Error : atom_db::import(char const *) can't open file \"" << file << '\"' << std::endl;
			err("import_name(char const *)") << " can't open file \"" << file << '\"' << std::endl;
			return 1;
		}
		size_type sz = 0;
		inp >> sz;
		this->resize( sz );
		pointer p = this->data();
		//atom_name::value_type * avp = 0;
		atom_name * ap = 0;
		for(int i = 0; i < this->size(); ++i)
		{
			inp >> sz;
			ap = &p->name();
			ap->resize( sz );
			//avp = ap->data();
			//for(int j = 0; j < ap->size(); ++j) inp >> avp[j];
			for(int j = 0; j < ap->size(); ++j) inp >> (*ap)[j];
			//inp >> p->first;
			if( inp == 0 )
			{
				//std::cerr << "Error : atom_db::import(char const *) unexpected end of input, file \"" << file << '\"' << std::endl;
				err("import_name(char const *)") << "unexpected end of input, file \"" << file << '\"' << std::endl;
				return 2;
			}
			++p;
		}
		return 0;
	}
	int import_radius(char const * file)
	{
		std::ifstream inp( file );
		if( !inp.is_open() )
		{
			err("import_radius(char const *)") << " can't open file \"" << file << '\"' << std::endl;
			return 1;
		}
		pointer p = this->data();
		atom_name::value_type s;
		T rad = 0;
		int n = 0;
		for(int i = 0; i < this->size(); ++i)
		{
			inp >> s >> rad;
			if( inp == 0 )
			{
				err("import_radius(char const *)") << "unexpected end of input, file \"" << file << '\"' << std::endl;
				return 2;
			}
			n = this->find(s);
			if( n == -1 )
			{
				err("import_radius(char const *)") << "atom \"" << s << "\" not found in data base" << std::endl;
				std::cerr << "size : " << this->size() << std::endl;
				std::cerr << "i    : " << i << std::endl;
				std::cerr << "s    : " << s << std::endl;
				std::cerr << "rad  : " << rad << std::endl;
				return 3;
			}
			m[n].radius( rad );
			++p;
		}
		return 0;
	}
	int export_data(char const * file)const
	{
		std::ofstream out( file );
		out.setf( std::ios::scientific );
		int prec = 8, w = prec + 8;
		out.precision( prec );
		out << this->size() << std::endl;
		const_pointer p = this->data();
		for(int i = 0; i < this->size(); ++i)
		{
			out << std::setw( w ) << p->radius() << ' ';
			out << p->size() << ' ';
			for(int j = 0; j < p->size(); ++j)
				out << p->name(j) << ' ';
			out << std::endl;
			++p;
		}
		return 0;
	}
private:
	std::ostream & err(std::string s)const
	{
		std::cerr << "[" << this << "] Error : atom_db::" << s; return std::cerr;
	}
	std::ostream & log(std::string s)const
	{
		std::cout << "[" << this << "] atom_db::" << s; return std::cout;
	}
};

#endif//__ATOM_DATA_BASE_H__
