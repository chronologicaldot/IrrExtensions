/*
(c) 2013 Nicolaus Anderson
*/

#include <irrlicht.h>

#ifndef _CIRCLE2D_
#define _CIRCLE2D_

namespace irr
{
namespace core
{

//! Class circle 2D
/*
A 2D circle.
*/

template<class T>
class circle2d
{
public:
	vector2d<T> center;
	T radius;

	circle2d()
		: radius( (T)0 )
	{}

	circle2d( T x, T y, T r )
		: center( vector2d<T>(x,y) )
		, radius( r )
	{}

	circle2d( vector2d<T> origin, T r )
		: center( origin )
		, radius( r )
	{}

	//! Set
	void set( T x, T y, T r )
	{
		center = vector2d<T>(x,y);
		radius = r;
	}

	//! Set
	void set( vector2d<T> origin, T r )
	{
		center = origin;
		radius = r;
	}

	//! Top point
	/* Returns a vector that gives the top-most point on the circle. */
	vector2d<T> topPoint()
	{
		return vector2d<T>( center.X, center.Y + radius);
	}

	//! Get area
	T getArea()
	{
		return PI * radius * radius;
	}

	//! Circumference
	inline T getCircumference()
	{
		return 2 * PI * radius;
	}

	//! Point inside?
	bool isPointInside( vector2d<T> point )
	{
		return ( point - center ).getLengthSQ() <= radius * radius;
	}

	//! Get horizon
	/* Returns the horizontal secant line of the circle at the given
	distance from the top.
	The line goes from the left side of the circle to the right side. */
	line2d<T> getHorizon( T distDown )
	{
		line2d<T> ret;

		ret.start.Y = center.Y + radius - distDown;
		ret.end.Y = ret.start.Y;

		ret.end.X = (T) squareroot( (irr::f64)
							( radius * radius ) - ret.start.Y
							);

		ret.start.X = - ret.end.X; // get the negative of the sqrt

		// correct origin offset
		ret.start.X += center.X;
		ret.end.X += center.X;

		return ret;
	}
};

typedef circle2d<irr::s32>		circle2di;
typedef circle2d<irr::f32>		circle2df;

} // end namespace core
} // end namespace irr

#endif // #ifndef _CIRCLE2D_
