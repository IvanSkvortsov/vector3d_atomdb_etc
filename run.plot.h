#include"all.h"

using namespace std;

void print_color(color_rgb const & c, string s, ostream & out = cout, int w = 4)
{
	out << setw(14) << s << " : " << setw( w ) << c.red() << setw( w ) << c.green() << setw( w ) << c.blue() << endl;
}


template<class T>
T incr_grad(T const & d, int const & size, int const & i, T const & max_grad = T(0.125))
{
	T d0 = d * (T(1) - max_grad), grad = d * max_grad / T(size-1);
	return d0 + grad * i;
}

template<class T>
T decr_grad(T const & d, int const & size, int const & i, T const & max_grad = T(0.125))
{
	T d0 = d * (T(1) + max_grad), grad = d * max_grad / T(size-1);
	return d0 - grad * i;
}

template<class T>
int run_radius(vector<T> & vr, int size, T const & r)
{
	if( size < 0 ) return 1;
	if( size%5 ) size += (5-size%5);
	vr.resize( size );
	int sub_size = (2 * size) / 5;
	vr[0] = r;
	T d = (r - (9 * r) / 14) / T(sub_size-1);
	for(int i = 0; i < sub_size; ++i)
	{
		vr[i] = r - decr_grad<T>(d, sub_size, i) * i;
		//vr[i] = r - d * i;
	}
	int sub_size2 = size - sub_size;
	vr[sub_size] = vr[sub_size-1] - decr_grad<T>(d, sub_size, sub_size-1);//(r * 42) / 73;
	d = (vr[sub_size] - (r * 6)/65) / T(sub_size2-1);
	for(int i = 0; i < sub_size2; ++i)
	{
		vr[i+sub_size] = vr[sub_size] - decr_grad<T>(d, sub_size2, i) * i;
		//vr[i+sub_size] = vr[sub_size] - d * i;
	}
	return 0;
}

template<class T>
int run_center(vector<vector_3d<T> > & vc, int size, vector_3d<T> const & c, vector_3d<T> v3, vector<T> const & vr)
{
	if( size < 0 ) return 1;
	if( size%5 ) size += (5-size%5);
	vc.resize( size );
	vc[0] = c;
	vector_3d<T> tmp_c;
	v3.normalize();
	int sub_size = (2 * size) / 5;
	T calibr;
	for(int i = 1; i < sub_size; ++i)
	{
		//tmp_c = v3 * (vr[i-1] - vr[i]) * T(1) - T(i) / (T(8 * sub_size));
		calibr = T(1) - T(i) / (T(8 * sub_size));
		tmp_c = v3 * (vr[i-1] - vr[i]) * calibr;
		tmp_c += vc[i-1];
		vc[i] = tmp_c;
	}
	int sub_size2 = size - sub_size;
	T calibr2;
	for(int i = sub_size; i < vc.size(); ++i)
	{
		//tmp_c = v3 * (vr[i-1] - vr[i]) * (T(1) - T(i-sub_size) / (T(1.1) * T(sub_size2)) ) * calibr;
		calibr2 = (T(1) - T(i-sub_size) / (T(1.1) * T(sub_size2)) ) * calibr;
		tmp_c = v3 * (vr[i-1] - vr[i]) * calibr2;
		tmp_c += vc[i-1];
		vc[i] = tmp_c;
	}
}

int run_color(vector<color_rgb> & vcolor, int size, color_rgb b, color_rgb e)
{
	if( size < 0 ) return 1;
	if( size%5 ) size += (5-size%5);
	vcolor.resize( size );
	vcolor[0] = b;
	int sub_size1 = (2 * size) / 5;
	color_rgb c = color_rgb( (e - b) / double(5) ), d = color_rgb( c / double(sub_size1-1) );
	for(int i = 1; i < sub_size1; ++i)
		vcolor[i] = b + color_rgb( d * double(i) );
	int sub_size2 = size - sub_size1;
	b = vcolor[sub_size1-1];
	d = (e - b) / double(sub_size2);
	for(int i = 0; i < sub_size2; ++i)
		vcolor[sub_size1 + i] = b + color_rgb(d * double(i+1) );
	return 0;
}

template<class T>
void demo_gnuplot(char const * file1)
{
	int prec = 6, w = prec + 8;
	ifstream inp( file1 );
 	//-- start of input --//
	vector_3d<T> v3;// direction of 'light reflection' effect simulation
	inp >> v3.x() >> v3.y() >> v3.z();
	cout.setf( ios::scientific );
	cout.precision( prec );
	int size = 0;
	inp >> size;
	int w14 = 14;
	vector_3d<T> c;// cartesian coordinates of point
	inp >> c.x() >> c.y() >> c.z();
	cout << setw( w14 ) << "" <<
		setw( w ) << "x" <<
		setw( w ) << "y" <<
		setw( w ) << "z" <<
		endl;
	cout << setw( w14 ) << "direction :";
	for(int i = 0; i < 3; ++i) cout << setw( w ) << v3[i]; cout << endl;
	cout << setw( w14 ) << "coordinate :";
	for(int i = 0; i < 3; ++i) cout << setw( w ) << c[i]; cout << endl;
	T r;// radius of point
	inp >> r;
	cout << setw( w14 ) << "radius :" << setw( w ) << r << endl;
	cout << setw( w14 ) << "size :" << setw( w ) << size << endl;
	cout.unsetf( ios::scientific );
	vector<T> vsqr;// visible parts of subpoints using in 'light reflection' effect simulation
	vsqr.resize( size );
	for(int i = 0; i < vsqr.size(); ++i)
		inp >> vsqr[i];
	inp.close();
	//--- end of input ---//
	vector<T> vr;// radia of subpoints using in 'light ...
	run_radius<T>( vr, size, r );
	vector<vector_3d<T> > vc;// cartesian coordinates of subpoints using in 'light ...
	run_center<T>( vc, size, c, v3, vr );
	vector<color_rgb> vcolor;
	run_color( vcolor, size, color_rgb("0xdd0000"), color_rgb("0xff8989") );
	char file2[] = "file2";
	ofstream  out( file2 );
 	//-- start of output --//
	out << setw( 4 ) << vr.size() << endl;
	out.setf( ios::scientific );
	out <<  setw( w ) << "radius" << setw( w ) << "x" << setw( w ) << "y" << setw( w ) << "z" <<
		setw( 4 ) << "r" << setw( 4 ) << "g" << setw( 4 ) << "b" <<
		setw( w ) << "sqr(r / r0)" <<
		setw( w ) << "sqr_visible" <<
		setw( w ) << "sqr_part" <<
		endl << endl;
	for(int i = 0; i < vr.size(); ++i)
		out <<  setw( w ) << vr[i] << setw( w ) << vc[i].x() << setw( w ) << vc[i].y() << setw( w ) << vc[i].z() <<
			setw( 4 ) << vcolor[i].red() <<
			setw( 4 ) << vcolor[i].green() <<
			setw( 4 ) << vcolor[i].blue() <<
			setw( w ) << vr[i] * vr[i] / (vr[0] * vr[0]) <<
			setw( w ) << vr[i] * vr[i] - (i == vr.size()-1 ? 0 : vr[i+1] * vr[i+1]) <<
			setw( w ) << ( i == vr.size()-1 ? T(1) : T(1) - vr[i+1] * vr[i+1] / ( vr[i] * vr[i]) ) <<
			setw( w ) << (i == vr.size()-1 ? T(1) : vsqr[i+1]) <<
			endl;
	out.close();
 	//--- end of output ---//
	cout << setw( 4 ) << "i" << setw( w ) << "sqr(r / r0)" << setw( w ) << "r / r0" << endl;
	for(int i = 0; i < vr.size(); ++i)
		cout << setw(4) << i << setw( w ) << vr[i] * vr[i] / (vr[0] * vr[0]) << setw( w ) << vr[i]/vr[0] << endl;
	cout << endl;
	char file3[] = "file3";
	out.open( file3 );
	out << "set terminal postscript eps" << endl;
	out << "set output \"plot.eps\"" << endl;
	out << "set xrange[-10.0 : 10.0]" << endl;
	out << "set yrange[-10.0 : 10.0]" << endl;
	out << "set size square" << endl;
	for(int i = 0; i < vr.size(); ++i)
	{
		out << "set object circle at first "<<vc[i].x()<<','<<vc[i].y()<<" radius " << vr[i] <<
			" fillcolor rgb \"#" << vcolor[i].toString() << "\" fillstyle solid noborder" << endl;
		cout << "i : " << setw(3) << i << " 0x" << vcolor[i].toString() << endl;
	}
	out << "set key off" << endl;
	out << "plot 20 w l" << endl;
	out.close();
}

template<class T>
struct pt_cnfg
{
	vector_3d<T> v3;
	vector<T> vr;
	vector<vector_3d<T> > vc;
	vector<color_rgb> vcolor;
	void run_v3(T const & x, T const & y, T const & z)
	{
		v3.x() = x;
		v3.y() = y;
		v3.z() = z;
	}
	int run_r(T const & r, int size = 15);
	int run_c(vector_3d<T> const & c, int size = 15);
	int run_color(color_rgb b, color_rgb e, int size = 15);
};

template<class T>
int pt_cnfg<T>::run_r(T const & r, int size)
{
	if( size < 0 ) return 1;
	if( size%5 ) size += (5-size%5);
	vr.resize( size );
	int sub_size = (2 * size) / 5;
	vr[0] = r;
	T d = (r - (9 * r) / 14) / T(sub_size-1);
	for(int i = 0; i < sub_size; ++i)
	{
		vr[i] = r - decr_grad<T>(d, sub_size, i) * i;
		//vr[i] = r - d * i;
	}
	int sub_size2 = size - sub_size;
	vr[sub_size] = vr[sub_size-1] - decr_grad<T>(d, sub_size, sub_size-1);//(r * 42) / 73;
	d = (vr[sub_size] - (r * 6)/65) / T(sub_size2-1);
	for(int i = 0; i < sub_size2; ++i)
	{
		vr[i+sub_size] = vr[sub_size] - decr_grad<T>(d, sub_size2, i) * i;
		//vr[i+sub_size] = vr[sub_size] - d * i;
	}
	return 0;
}

template<class T>
int pt_cnfg<T>::run_c(vector_3d<T> const & c, int size)
{
	if( size < 0 ) return 1;
	if( size%5 ) size += (5-size%5);
	vc.resize( size );
	vc[0] = c;
	vector_3d<T> tmp_c;
	v3.normalize();
	int sub_size = (2 * size) / 5;
	T calibr;
	for(int i = 1; i < sub_size; ++i)
	{
		//tmp_c = v3 * (vr[i-1] - vr[i]) * T(1) - T(i) / (T(8 * sub_size));
		calibr = T(1) - T(i) / (T(8 * sub_size));
		tmp_c = v3 * (vr[i-1] - vr[i]) * calibr;
		tmp_c += vc[i-1];
		vc[i] = tmp_c;
	}
	int sub_size2 = size - sub_size;
	T calibr2;
	for(int i = sub_size; i < vc.size(); ++i)
	{
		//tmp_c = v3 * (vr[i-1] - vr[i]) * (T(1) - T(i-sub_size) / (T(1.1) * T(sub_size2)) ) * calibr;
		calibr2 = (T(1) - T(i-sub_size) / (T(1.1) * T(sub_size2)) ) * calibr;
		tmp_c = v3 * (vr[i-1] - vr[i]) * calibr2;
		tmp_c += vc[i-1];
		vc[i] = tmp_c;
	}
}

template<class T>
int pt_cnfg<T>::run_color(color_rgb b, color_rgb e, int size)
{
	if( size < 0 ) return 1;
	if( size%5 ) size += (5-size%5);
	vcolor.resize( size );
	vcolor[0] = b;
	int sub_size1 = (2 * size) / 5;
	color_rgb c = color_rgb( (e - b) / double(5) ), d = color_rgb( c / double(sub_size1-1) );
	for(int i = 1; i < sub_size1; ++i)
		vcolor[i] = b + color_rgb( d * double(i) );
	int sub_size2 = size - sub_size1;
	b = vcolor[sub_size1-1];
	d = (e - b) / double(sub_size2);
	for(int i = 0; i < sub_size2; ++i)
		vcolor[sub_size1 + i] = b + color_rgb(d * double(i+1) );
	return 0;
}

template<class T>
struct matrix_sqr : public vector<T>
{
	int _size;
	matrix_sqr():_size(0), vector<T>(){}
	matrix_sqr(int __size):_size(__size), vector<T>(__size * __size){}
	T & operator()(int i, int j){return this->operator[](i * _size + j);}
	T const & operator()(int i, int j)const{return this->operator[](i * _size + j);}
	int sz()const{return _size;}
	void sz(int __size){_size = __size; this->resize( _size * _size );}
};

template<class T>
void make_graph(matrix_sqr<int> & mx, geometry<T> const & geom, T const & min_v)
{
	mx.sz( geom.size() );
	vector_3d<T> v;
	for(int i = 0; i < geom.size(); ++i)
	{
		for(int j = i + 1; j < geom.size(); ++j)
		{
			v = geom[i].coord - geom[j].coord;
			mx(i, j) = (v.len() > min_v ? 0 : 1);
		}
	}
}

template<class T>
void make_vgraph(vector<matrix_sqr<int> > & v_mx, vector<geometry<T> > const & v_geom, T const & min_v = T(2.8))
{
	v_mx.resize( v_geom.size() );
	for(int i = 0; i < v_geom.size(); ++i)
		make_graph<T>( v_mx[i], v_geom[i], min_v );
}

template<class T>
T max_radius(T & max_ri, int & ni, geometry<T> const & geom)
{
	max_ri = T(0);
	ni = 0;
	for(int i = 0; i < geom.size(); ++i)
		if( max_ri < geom[i].radius )
		{
			max_ri = geom[i].radius;
			ni = i;
		}
	return max_ri;
}


template<class T>
T calibr_value( geometry<T> const & geom, T const & min_v = T(2.8) )
{
	vector_3d<T> v;
	T calibr_i = 1e6;
	for(int i = 0; i < geom.size(); ++i)
	{
		for(int j = i + 1; j < geom.size(); ++j)
		{
			v = geom[i].coord - geom[j].coord;
			if( v.len() > min_v ) continue;
			if( calibr_i * T(1.8) > v.len() )
				calibr_i = v.len() / T(1.8);
		}
	}
	return calibr_i;
}

template<class T>
void calibrate_radius( vector<geometry<T> > & v_geom )
{
	T max_r = T(0), max_ri;
	int n = 0, ni;
	for(int i = 0; i < v_geom.size(); ++i)
	{
		max_radius<T>( max_ri, ni, v_geom[i] );
		if( max_r < max_ri )
		{
			max_r = max_ri;
			n = i;
		}
	}
	for(int i = 0; i < v_geom.size(); ++i)
	{
		for(int j = 0; j < v_geom[i].size(); ++j)
			v_geom[i][j].radius /= max_r;
	}
	T calibr = 1e6, calibr_i;
	for(int i = 0; i < v_geom.size(); ++i)
	{
		calibr_i = calibr_value<T>( v_geom[i] );
		if( calibr > calibr_i )
			calibr = calibr_i;
	}
	for(int i = 0; i < v_geom.size(); ++i)
		for(int j = 0; j < v_geom[i].size(); ++j)
		{
			v_geom[i][j].radius *= calibr;
			//v_geom[i][j].radius *= 10;
			//v_geom[i][j].coord *= 10;
		}
	return;
}

template<class T>
T maximum_xyz(geometry<T> const & geom, int it = 0)
{
	T v = T(0);
	int n = 0;
	for(int i = 0; i < geom.size(); ++i)
		if( v < geom[i].coord[it] )
		{
			v = geom[i].coord[it];
			n = i;
		}
	return v + geom[n].radius;
}

template<class T>
T minimum_xyz(geometry<T> const & geom, int it = 0)
{
	T v = 1e6;
	int n = 0;
	for(int i = 0; i < geom.size(); ++i)
		if( v > geom[i].coord[it] )
		{
			v = geom[i].coord[it];
			n = i;
		}
	return v - geom[n].radius;
}

template<class T>
T maximum_y(geometry<T> const & geom)
{
	return maximum_xyz<T>( geom, 1 );
}

template<class T>
T minimum_y(geometry<T> const & geom)
{
	return minimum_xyz<T>( geom, 1 );
}


template<class T>
T minimum_x(geometry<T> const & geom)
{
	return minimum_xyz<T>( geom, 0 );
}

template<class T>
T maximum_x(geometry<T> const & geom)
{
	return maximum_xyz<T>( geom, 0 );
}


template<class T>
struct max_min
{
	T max, min;
	max_min():max(), min(){}
	T len()const{this->check();return max - min;}
	int check()
	{
		if( max < min )
		{
			T tmp = max;
			max = min;
			min = tmp;
			return 1;
		}
		return 0;
	}
};

template<class T>
bool is_between(T const & a, T const & b, T const & c)
{
	return a <= b && b <= c;
}

template<class T>
bool is_intersected(max_min<T> const & a, max_min<T> const & b)
{
	if( is_between<T>( a.min, b.max, a.max ) ) return true;
	if( is_between<T>( a.min, b.min, a.max ) ) return true;
	if( is_between<T>( b.min, a.max, b.max ) ) return true;
	if( is_between<T>( b.min, a.min, b.max ) ) return true;
	return false;
}

template<class T>
void shift_y(geometry<T> & geom, max_min<T> & a, max_min<T> & b)
{
	if( !is_intersected<T>( a, b ) ) return;
	vector_3d<T> v3(0, 0, 0);
	T d = b.max - a.min + geom[0].radius * T(0.5);// > 0
	b.max -= d;
	b.min -= d;
	v3.y() = d;
	for(int i = 0; i < geom.size(); ++i)
		geom[i].coord -= v3;
}

template<class T>
void y_shift_geometry(vector<geometry<T> > & v_geom)
{
	vector<max_min<T> > vmm;
	vmm.resize( v_geom.size() );
	for(int i = 0; i < v_geom.size(); ++i)
	{
		vmm[i].max = maximum_y<T>( v_geom[i] );
		vmm[i].min = minimum_y<T>( v_geom[i] );
	}
	for(int i = 0; i < v_geom.size(); ++i)
	{
		for(int j = 0; j < v_geom.size(); ++j)
		{
			if( i == j ) continue;
			shift_y<T>( v_geom[j], vmm[i], vmm[j] );
		}
	}
}

template<class T>
void max_min_y_vgeom(T & max_y_, T & min_y_, vector<geometry<T> > const & v_geom)
{
	max_y_ = T(0);
	min_y_ = T(1e6);
	T tmp;
	for(int i = 0; i < v_geom.size(); ++i)
	{
		tmp = maximum_y<T>( v_geom[i] );
		if( tmp > max_y_ ) max_y_ = tmp;
		tmp = minimum_y<T>( v_geom[i] );
		if( tmp < min_y_ ) min_y_ = tmp;
	}
}

template<class T>
void max_min_x_vgeom(T & max_x_, T & min_x_, vector<geometry<T> > const & v_geom)
{
	max_x_ = T(0);
	min_x_ = T(1e6);
	T tmp;
	for(int i = 0; i < v_geom.size(); ++i)
	{
		tmp = maximum_x<T>( v_geom[i] );
		if( tmp > max_x_ ) max_x_ = tmp;
		tmp = minimum_x<T>( v_geom[i] );
		if( tmp < min_x_ ) min_x_ = tmp;
	}
}

template<class T>
void run_vpt(vector<pt_cnfg<T> > & v_pt, geometry<T> const & geom)
{
	v_pt.resize( geom.size() );
	for(int i = 0; i < v_pt.size(); ++i)
	{
		v_pt[i].run_v3(1., 1.3, 0.);
		v_pt[i].run_r( geom[i].radius );
		v_pt[i].run_c( geom[i].coord );
		v_pt[i].run_color(geom[i].color, geom[i].color_light);
	}
}

template<class T>
void run_v2pt(vector<vector<pt_cnfg<T> > > & v2_pt, vector<geometry<T> > const & v_geom)
{
	v2_pt.resize( v_geom.size() );
	for(int i = 0; i < v_geom.size(); ++i)
		run_vpt<T>( v2_pt[i], v_geom[i] );
}

template<class T>
void set_object(ostream & out, pt_cnfg<T> const & pt)
{
	for(int i = 0; i < pt.vr.size(); ++i)
	{
		out << "set object circle at first "<<pt.vc[i].x()<<','<<pt.vc[i].y()<<" radius " << pt.vr[i] <<
			" fillcolor rgb \"#" << pt.vcolor[i].toString() << "\" fillstyle solid noborder" << endl;
		//out << "set label at first "<<pt.vc[i].x()<<','<<pt.vc[i].y()<<" \" \" point pointtype 7 ps " << pt.vr[i] * 9.6 <<
		//	" lc rgb \"#" << pt.vcolor[i].toString() << "\"" << endl;
	}
}

template<class T>
void set_vobject(ostream & out, vector<pt_cnfg<T> > const & v_pt)
{
	for(int i = 0; i < v_pt.size(); ++i)
		set_object<T>( out, v_pt[i] );
}

template<class T>
void set_v2object(ostream & out, vector<vector<pt_cnfg<T> > > const & v2_pt)
{
	for(int i = 0; i < v2_pt.size(); ++i)
		set_vobject<T>( out, v2_pt[i] );
}

template<class T>
void set_arrow(ostream & out, geometry<T> const & geom, matrix_sqr<int> const & mx, int & iter)
{
	vector_3d<T> v0, v1, v, b, e;
	T r0, r1;
	for(int i = 0; i < geom.size(); ++i)
	{
		v0 = geom[i].coord;
		r0 = geom[i].radius;
		for(int j = i+1; j < geom.size(); ++j)
		{
			if( !mx(i, j) ) continue;
			v1 = geom[j].coord;
			r1 = geom[j].radius;
			v = v1 - v0;
			v.normalize();
			b = (v0 + v * r0);
			e = (v1 - v * r1);
			out << "set arrow " << setw( 3 ) << ++iter <<
				//" from " << setprecision(4) << geom[i].coord.x() << ", "<< setprecision( 4 ) << geom[i].coord.y() <<
				//" to "   << setprecision(4) << geom[j].coord.x() << ", "<< setprecision( 4 ) << geom[j].coord.y() <<
				" from " << setprecision(4) << b.x() << ", "<< setprecision( 4 ) << b.y() <<
				" to "   << setprecision(4) << e.x() << ", "<< setprecision( 4 ) << e.y() <<
				" nohead lw 3 lc rgb \"#000000\"" << endl;
		}
	}
}

template<class T>
void set_varrow(ostream & out, vector<geometry<T> > const & v_geom, vector<matrix_sqr<int> > const & v_mx)
{
	int iter = 0;
	for(int i = 0; i < v_geom.size(); ++i)
		set_arrow<T>(out, v_geom[i], v_mx[i], iter);
}

template<class T>
void set_xy_range(ostream & out, vector<geometry<T> > const & v_geom)
{
	T max_y_, min_y_;
	max_min_y_vgeom<T>( max_y_, min_y_, v_geom );
	min_y_ -= v_geom[0][0].radius;
	max_y_ += v_geom[0][0].radius;
	//
	T _y_len = int((max_y_ - min_y_)* 10) / T(10);
	max_y_ = int(max_y_ * 10) / T(10);
	min_y_ = max_y_ - _y_len;
	//
	T max_x_, min_x_;
	max_min_x_vgeom<T>( max_x_, min_x_, v_geom );
	min_x_ -= v_geom[0][0].radius;
	max_x_ += v_geom[0][0].radius;
	//
	T _x0 = (max_x_ + min_x_)/2 + _y_len/2;
	max_x_ = int(_x0 * 10) / T(10);
	min_x_ = max_x_ - _y_len;
	//
	out << "set xrange[" << min_x_ << " : " << max_x_ << "]" << endl;
	out << "set yrange[" << min_y_ << " : " << max_y_ << "]" << endl;
}

template<class T>
void run_gnuplot(vector<vector<pt_cnfg<T> > > const & v2_pt, vector<geometry<T> > const & v_geom, vector<matrix_sqr<int> > const & v_mx, char const file[] = "plot.gnp")
{
	ofstream out( file );
	out << "#!" << endl;
	out << "set terminal postscript eps size 7,7" << endl;
	out << "set output \"plot.eps\"" << endl;
	set_xy_range<T>( out, v_geom );
	//out << "set grid" << endl;
	//out << "set xtics format \"\"" << endl;
	//out << "set ytics format \"\"" << endl;
	out << "unset xtics" << endl;
	out << "unset ytics" << endl;
	out << "set size square" << endl;
	set_varrow<T>(out, v_geom, v_mx);
	set_v2object<T>( out, v2_pt );
	out << "set key off" << endl;
	out << "plot 20 w l" << endl;
	out.close();
}


template<class T>
int init_atom_db(atom_db<T> & at_db, char const file[] = "atom.db.src0")
{
	return at_db.import_data( file );
}

template<class T>
int init_geometry(vector<geometry<T> > & v_geom, atom_db<T> const & at_db, int argc, char ** argv)
{
	v_geom.resize( argc - 1 );
	int term_key = 0;
	for(int i = 0; i < v_geom.size(); ++i)
		term_key += v_geom[i].import( argv[i+1], at_db );
	return term_key;
}

template<class T>
void demo_run(int argc, char ** argv)
{
	atom_db<T> at_db;
	if( init_atom_db<T>( at_db ) ) return;
	vector<geometry<T> > v_geom;
	if( init_geometry<T>( v_geom, at_db, argc, argv) ) return;
	for(int i = 0; i < v_geom.size(); ++i) v_geom[i].print();
	calibrate_radius<T>( v_geom );
	y_shift_geometry<T>( v_geom );
	for(int i = 0; i < v_geom.size(); ++i) v_geom[i].print();
	vector<matrix_sqr<int> > v_mx;
	make_vgraph<T>( v_mx, v_geom );
	vector<vector<pt_cnfg<T> > > v2_pt;
	run_v2pt<T>( v2_pt, v_geom );
	run_gnuplot<T>( v2_pt, v_geom, v_mx );
}

void demo_plot_d(int argc, char ** argv)
{
	demo_run<double>( argc, argv );
	//demo_gnuplot<double>( file1);
}
