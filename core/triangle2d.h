/*
(c) 2013 Nicolaus Anderson
*/

#include <irrlicht.h>

#ifndef _TRIANGLE2D_
#define _TRIANGLE2D_

namespace irr
{
namespace core
{

//! Class triangle 2D
/*
A 2D triangle.
*/

template<class T>
class triangle2d
{
public:

	vector2d<T> pt1;
	vector2d<T> pt2;
	vector2d<T> pt3;

	triangle2d<T>()
	{}

	triangle2d<T>(
		vector2d<T> point1,
		vector2d<T> point2,
		vector2d<T> point3
		)
		: pt1( point1 )
		, pt2( point2 )
		, pt3( point3 )
	{}

	triangle2d<T> (
		T p1x, T p1y,
		T p2x, T p2y,
		T p3x, T p3y
		)
		: pt1( vector2d<T>( p1x, p1y ) )
		, pt2( vector2d<T>( p2x, p2y ) )
		, pt3( vector2d<T>( p3x, p3y ) )
	{}

	//! Set
	/* Sets the triangle. */
	void set(
		vector2d<T> point1,
		vector2d<T> point2,
		vector2d<T> point3
		)
	{
		pt1 = point1;
		pt2 = point2;
		pt3 = point3;
	}

	//! Set
	/* Sets the triangle. */
	void set(
		T p1x, T p1y,
		T p2x, T p2y,
		T p3x, T p3y
		)
	{
		pt1 = vector2d<T>(p1x,p1y);
		pt2 = vector2d<T>(p2x,p2y);
		pt3 = vector2d<T>(p3x,p3y);
	}

	//! Top point
	/* Returns the address of the top point.
	Default return is point 1. */
	vector2d<T>& topPoint()
	{
		if ( pt3.Y > pt2.Y )
		{
			if ( pt3.Y > pt1.Y )
				return pt3;
		} else {
			if ( pt2.Y > pt1.Y )
				return pt2;
		}

		return pt1;
	}

	//! Top point index
	/* Returns which of the 3 points is the highest
	(default return is 1). */
	irr::u32 topPointIndex()
	{
		if ( pt3.Y > pt2.Y )
		{
			if ( pt3.Y > pt1.Y )
				return 3;
		} else {
			if ( pt2.Y > pt1.Y )
				return 2;
		}

		return 1;
	}

	//! Set highest point to first
	/* Sets the highest point to be the first of the three points. */
	void setHighestToFirst()
	{
		vector2d<T> temp;

		if ( pt3.Y > pt2.Y )
		{
			if ( pt3.Y > pt1.Y )
				swap( pt1, pt3 );
		} else {
			if ( pt2.Y > pt1.Y )
				swap( pt1, pt2 );
		}

		// Else: no change
	}

	//! Make triangle clockwise
	/* Restore the triangle to a clockwise triangle. */
	void makeCW()
	{
		//irr::core::vector2d<T> v12;
		//irr::core::vector2d<T> v13;
		//irr::core::vector2d<t> v13d;

		// Check for equality
		if ( pt1 == pt2 || pt1 == pt3 || pt2 == pt3
			|| pt1.isBetweenPoints(pt2,pt3)
			|| pt2.isBetweenPoints(pt1,pt3)
			|| pt3.isBetweenPoints(pt1,pt2)
			)
			return;

		// Set the highest vector to the first one
		setHighestToFirst();

		// This means only points 2 and 3 are variables
		/* For clockwise rotation, 2 must be right of 3.
		Switch them if this is not the case. */

		if (
			(pt3.Y - pt1.Y)*(pt2.X - pt1.X) - (pt2.Y - pt1.Y)*(pt3.X - pt1.X)
			< 0
			)
		{
			swap( pt2, pt3 );
		}

		//v12 = pt2 - pt1;
		//v13 = pt3 - pt1;

		//// Vertical line - easy comparison
		//if ( v13.X == 0 )
		//{
		//	if ( v12.X < 0 )
		//		swap( pt2, pt3 );

		//	return;
		//}

		///* Construct a vector the length of v13 along the x axis. */
		//v13d = irr::core::vector2d<T>(
		//				v13.getLength(),
		//				0
		//			);

		///* Subtract this vector from the vector between points 1 and 2.
		//This will give us the same situation as above. */

		//if ( (v12 - v13d).X < 0 )
		//{
		//	swap( pt2, pt3 );
		//}
	}

	//! Make triangle counter clockwise
	/* Restore the triangle to a counter-clockwise triangle. */
	void makeCCW()
	{
		makeCW();

		vector2d<T> temp = pt3;
		pt3 = pt2;
		pt2 = temp;
	}

	//! Get altitude
	/* */
	// To do

	//! Get area
	/* Returns the area of the triangle. */
	T getArea()
	{
		// Can't do this with 2D vectors:
		//return
		//	irr::core::abs_(
		//		( (pt2 - pt1).crossProduct(pt3 - pt2) ).getLength()
		//	) /(T)2;

		return
			(
				(pt2 - pt1).getLength() * (pt3 - pt2).getLength()
			) / (T)2;
	}

	//! Point inside?
	/* Indicates if the point is within the triangle.
	Assumes a clockwise triangle.
	If you want to keep the original triangle, make a copy of it
	and call makeCW() on the copy. */
	bool isPointInside( vector2d<T> point )
	{
		/* Points are automatically disqualified if they are
		higher than all of the points or lower than all of them. */
		if (
			// higher
			( point.Y > pt1.Y && point.Y > pt2.Y && point.Y > pt3.Y )
			|| // lower
			( point.Y < pt1.Y && point.Y < pt2.Y && point.Y < pt3.Y )
			)
		{
			return false;
		}

		/* If the triangle is flat, then the point need merely be
		between any two of the points. */
		if ( pt1 == pt2 || pt1 == pt3 || pt2 == pt3 )
		{
			if (
				point.isBetweenPoints(pt2,pt3)
				|| point.isBetweenPoints(pt1,pt3)
				|| point.isBetweenPoints(pt1,pt2)
				)
			{
				return true;
			}
			return false;
		}

		/* For the point to be inside, it must be right of the
		vector between pt1 and pt3 and left of the one between
		pt1 and pt2. */

		// What is commented out is too time-consuming

		//line2d<T> line12( pt1, pt2 );
		//line2d<T> line13( pt2, pt3 );

		//vector2d<T> near12
		//	= line12.getClosestPoint( point );

		//if ( near12.X < point.X )
		//	return false;

		//vector2d<T> near13
		//	= line13.getClosestPoint( point );

		//if ( near13.X > point.X )
		//	return false;

		// Must be inside
		//return true;

		/* Point must be between the ends of the horizon. */
		line2d<T> horiz = getHorizon( point.Y );

		if ( horiz.start.X =< point.X
			&& point.X <= horiz.end.X )
		{
			return true;
		}

		return false;
	}

	//! Get horizon
	/* Returns the horizontal line between the vectors that go between
	points 1 and 2 and the points 1 and 3. The vector always starts from
	the left side of the triangle and goes to the right side.
	Assumes the triangle has been made clockwise.
	\param distDown - The distance from the top to form the line. */
	line2d<T> getHorizon( T distDown )
	{
		line2d<T> ret( pt1, pt1 );

		/* Don't bother if the distance down is out of the triagle. */
		if ( pt1.Y - distDown < (pt2.Y < pt3.Y)? pt2.Y : pt3.Y )
			return ret;

		/* Situations:
		1) Horizon is above point 2 and 3.
		2) Horizon is below point 2 which is above point 3.
		3) Horizon is below point 3 which is above point 2.
		*/

		ret.start.Y = pt1.Y - distDown;
		ret.end.Y = pt1.Y - distDown;

		// Situation 1?
		if ( pt1.Y - distDown >= pt2.Y && pt1.Y - distDown >= pt3.Y )
		{
			if ( pt3.Y == pt1.Y )
			{
				ret.start = (pt1.X < pt3.X)? pt1 : pt3;
			} else {
				ret.start.X =
					// delta y * 1/m
					(pt1.Y - distDown) * ( pt3.X - pt1.X ) / ( pt3.Y - pt1.Y )
					+ pt1.X; // offset to location
			}

			if ( pt2.Y == pt1.Y )
			{
				ret.end = (pt1.X > pt2.X)? pt1 : pt2;
			} else {
				ret.end.X =
					// delta y * 1/m
					(pt1.Y - distDown) * ( pt2.X - pt1.X ) / ( pt2.Y - pt1.Y )
					+ pt1.X; // offset to location
			}

			return ret;
		}

		// Situation 2?
		if ( pt2.Y >= pt3.Y )
		{
			if ( pt2.Y == pt3.Y )
			{
				ret.start = pt3;
				ret.end = pt2;
				return ret;
			}

			// pt2.Y != pt3.Y

			ret.start.X =
					// delta y * 1/m
					(pt1.Y - distDown) * ( pt3.X - pt1.X ) / ( pt3.Y - pt1.Y )
					+ pt1.X; // offset to location

			ret.end.X =
					// delta y * 1/m
					(pt1.Y - distDown) * ( pt3.X - pt2.X ) / ( pt3.Y - pt2.Y )
					+ pt2.X; // offset to location

			return ret;
		}

		// Situation 3

		ret.start.X =
				// delta y * 1/m
				(pt1.Y - distDown) * ( pt2.X - pt3.X ) / ( pt2.Y - pt3.Y )
				+ pt3.X; // offset to location

		ret.end.X =
				// delta y * 1/m
				(pt1.Y - distDown) * ( pt2.X - pt1.X ) / ( pt2.Y - pt1.Y )
				+ pt1.X; // offset to location

		return ret;
	}

	//! Swap points
	/* Swaps the values of the two points. */
	inline void swap(
		vector2d<T>& point1;
		vector2d<T>& point2;
		)
	{
		vector2d<T> temp = point1;
		point1 = point2;
		point2 = temp;
	}

};

typedef triangle2d<irr::s32>		triangle2di;
typedef triangle2d<irr::f32>		triangle2df;

} // end namespace core
} // end namespace irr

#endif // #ifndef _TRIANGLE2D_