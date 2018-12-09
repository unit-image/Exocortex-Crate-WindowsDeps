// Copyright (c) 2017 Autodesk, Inc.
// All rights reserved.
// 
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.

#pragma once

#include "GeomExport.h"
#include "maxheap.h"
#include "point2.h"
#include "assert1.h"
#include <iosfwd>

/**
Description:
This class describes a 2D point using double precision x and y coordinates.
Methods are provided to add and subtract points, multiply and divide by
scalars, and element by element multiply and divide two points. All methods are
implemented by the system.
*/
class DPoint2 : public MaxHeapOperators {
public:
	double x, y;

	/** Constructor. Initializes all vector components to zero.
	*/
	DPoint2() : x(0.0), y(0.0) {}

	/** Constructor. x and y are initialized to the values
	specified. */
	DPoint2(double X, double Y) { x = X; y = Y; }

	/**  Constructor. x and y are initialized to the DPoint2
	specified.
	\param a value used to initialize x and y components */
	DPoint2(const DPoint2& a) { x = a.x; y = a.y; }

	/** Constructor. x and y are initialized to the Point2
	specified.
	\param a value used to initialize x and y components */
	DPoint2(const Point2& a) { x = a.x; y = a.y; }

	/*!  Constructor. x and y are initialized to. af[0]
	and af[1] respectively.
	\param af two-element array to initialize x and y components */
	DPoint2(const double af[2]) { x = af[0]; y = af[1]; }

	// Access operators

	/**  Allows access to x and y using the subscript operator.
	\param i index of component to return (0 or 1)
	\return  An index of 0 will return x, 1 will return y. */
	double& operator[](int i) { DbgAssert((i >= 0) && (i <= 1)); return (i == 0) ? x : y; }

	/*!  Allows access to x and y using the subscript operator.
	\param i index of component to return (0 or 1)
	\return  An index of 0 will return x, 1 will return y. */
	const double& operator[](int i) const { DbgAssert((i >= 0) && (i <= 1)); return (i == 0) ? x : y; }

	// Conversion function

	/**  Conversion function. Returns the address of the DPoint2.x
	\return a pointer to the x component. */
	operator double*() { return(&x); }

	/** Convert DPoint2 to Point2
	\return the value as a Point2 */
	operator Point2() { return Point2(x, y); }

	// Unary operators

	/** Unary - operator. Negates both x and y
	\return the resulting DPoint2 */
	DPoint2 operator-() const { return(DPoint2(-x, -y)); }

	/** Unary +. Returns the point unaltered.
	\return the resulting DPoint2 */
	DPoint2 operator+() const { return *this; }

	/** Assignment operator
	\param a the Point2 to assign to this DPoint2
	\return the resulting DPoint2 */
	DPoint2& operator=(const Point2& a) { x = a.x; y = a.y;	return *this; }

	/**  Subtracts a DPoint2 from this DPoint2.
	\param a the value to subtract from this DPoint2
	\return the resulting DPoint2 */
	DPoint2& operator-=(const DPoint2& a);

	/**  Adds a DPoint2 to this DPoint2.
	\param a the value to add to this DPoint2
	\return the resulting DPoint2 */
	DPoint2& operator+=(const DPoint2& a);

	/** Member-wise multiplication of two vectors: (x*x, y*y)
	\param a the multiplier
	\return the resulting DPoint2 */
	DPoint2& operator*=(const DPoint2& a);

	/** Each element of this DPoint2 is multiplied by the specified
	double.
	\param a the multiplier
	\return the resulting DPoint2 */
	DPoint2& operator*=(double a);

	/** Each element of this DPoint2 is divided by the specified
	double.
	\param a the divisor
	\return the resulting DPoint2 */
	DPoint2& operator/=(double a);

	// Binary operators

	/*!  Subtracts a DPoint2 from a DPoint2.
	\param a the value to subtract from this DPoint2
	\return the resulting DPoint2 */
	DPoint2 operator-(const DPoint2& a) const;

	/*!  Adds a DPoint2 to a DPoint2.
	\param a the value to add to this DPoint2
	\return the resulting DPoint2 */
	DPoint2 operator+(const DPoint2& a) const;

	/** Member-wise multiplication of two vectors: (x*x, y*y)
	\param a the multiplier
	\return the resulting DPoint2 */
	DPoint2 operator*(const DPoint2& a) const;

	/*!  Returns the 'Length' of this point (vector). This is:\n\n
	sqrt(v.x*v.x+v.y*v.y).
	\return the length value */
	double Length() const;

	/** The 'Length' squared of this point. This is
	v.x*v.x+v.y*v.y.
	\return the length value */
	GEOMEXPORT double LengthSquared() const;

	/** In place normalize
	\return the normalized value */
	GEOMEXPORT DPoint2& Unify();

	/** The largest axis
	\return the largest axis (0 = x, 1 = y) */
	GEOMEXPORT int MaxComponent() const;

	/** The smallest axis
	\return the smallest axis (0 = x, 1 = y) */
	GEOMEXPORT int MinComponent() const;

	// Test for equality

	/** Equality operator. Test for equality between two DPoint2's.
	\param p the value to compare
	\return  Nonzero if the DPoint2's are equal; otherwise 0. */
	int operator==(const DPoint2& p) const {
		return ((p.x == x) && (p.y == y));
	}

	/** Equality operator. Test for nonequality between two DPoint2's.
	\param p the value to compare
	\return  Nonzero if the DPoint2's are not equal; otherwise 0. */
	int operator!=(const DPoint2& p) const {
		return ((p.x != x) || (p.y != y));
	}

	// Property functions

	/**  const for (0,0) */
	GEOMEXPORT static const DPoint2 Origin;

	/**  const for (1,0) */
	GEOMEXPORT static const DPoint2 XAxis;

	/**  const for (0,1) */
	GEOMEXPORT static const DPoint2 YAxis;
};

/*!  Returns the 'Length' of the point. This is
sqrt(v.x*v.x+v.y*v.y)
\param p the DPoint2 to test
\return the length value */
double Length(const DPoint2& p);

/** Returns the component with the maximum absolute value.
\param p the DPoint2 to test
\return the maximum component (0=x, 1=y). */
GEOMEXPORT int MaxComponent(const DPoint2& p);

/** Returns the component with the minimum absolute value. 0=x, 1=y.
\param p the DPoint2 to test
\return the minimum component (0=x, 1=y). */
GEOMEXPORT int MinComponent(const DPoint2& p);

/** Returns a unit vector. This is a DPoint2 with each component
divided by the point Length().
\param p the DPoint2 to test
\return the unit vector for the specified DPoint2*/
GEOMEXPORT DPoint2 Normalize(const DPoint2& p);

/** Multiply a DPoint2 by a scalar
\param m the multiplier
\param p the DPoint2
\return the resulting DPoint2 */
GEOMEXPORT DPoint2 operator*(double m, const DPoint2& p);

/** Multiply a DPoint2 by a scalar
\param p the DPoint2
\param m the multiplier
\return the resulting DPoint2 */
GEOMEXPORT DPoint2 operator*(const DPoint2& p, double m);

/** Divide a DPoint2 by a scalar
\param p the DPoint2
\param d the divisor
\return the resulting DPoint2 */
GEOMEXPORT DPoint2 operator/(const DPoint2& p, double d);

/** Output a DPoint2 to a std::ostream as text
\param s the std::ostream
\param p the DPoint2
\return the DPoint2 text value */
GEOMEXPORT std::ostream &operator<<(std::ostream& s, const DPoint2& p);

// Inlines:

inline double Length(const DPoint2& v) {
	return (double)sqrt(v.x*v.x + v.y*v.y);
}

inline DPoint2& DPoint2::operator-=(const DPoint2& a) {
	x -= a.x;	y -= a.y;
	return *this;
}

inline DPoint2& DPoint2::operator+=(const DPoint2& a) {
	x += a.x;	y += a.y;
	return *this;
}

inline DPoint2& DPoint2::operator*=(const DPoint2& rhs)
{
	x *= rhs.x;
	y *= rhs.y;
	return *this;
}

inline DPoint2& DPoint2::operator*=(double f) {
	x *= f;   y *= f;
	return *this;
}

inline DPoint2& DPoint2::operator/=(double f) {
	x /= f;	y /= f;
	return *this;
}

inline DPoint2 DPoint2::operator-(const DPoint2& b) const {
	return(DPoint2(x - b.x, y - b.y));
}

inline DPoint2 DPoint2::operator+(const DPoint2& b) const {
	return(DPoint2(x + b.x, y + b.y));
}

inline DPoint2 DPoint2::operator*(const DPoint2& b) const {
	return DPoint2(x * b.x, y * b.y);
}

inline DPoint2 operator*(double f, const DPoint2& a) {
	return(DPoint2(a.x*f, a.y*f));
}

inline DPoint2 operator*(const DPoint2& a, double f) {
	return(DPoint2(a.x*f, a.y*f));
}

inline DPoint2 operator/(const DPoint2& a, double f) {
	return(DPoint2(a.x / f, a.y / f));
}

// Helper for converting DPoint2 to Point2
inline Point2 Point2FromDPoint2(const DPoint2& from)
{
	return Point2(from.x, from.y);
}

/*!  Returns the dot product of two DPoint2s. */
GEOMEXPORT double DotProd(const DPoint2& a, const DPoint2& b);

// Handy DPoint2 uses...

/** This class describes a vector in space using an origin point p, and a
unit direction vector in double precision. */
class DRay2 : public MaxHeapOperators {
	// Warning - instances of this class are saved as a binary blob to scene file
	// Adding/removing members will break file i/o
public:
	// point of origin
	DPoint2 p;
	// unit vector
	DPoint2 dir;
};

/** Double-Precision Line Segment Intersection test.
Determines if two line segments intersect.
\param seg1Start endpoint 1 for the first line segment
\param seg1End endpoint 2 for the first line segment
\param seg2Start endpoint 1 for the second line segment
\param seg2End endpoint 2 for the second line segment
\param intersectPoint if the lines intersect, this will return the intersection point.
\return  Intersection determination (0: No intersection, 1: Lines intersect (intersection location in 'intersectPoint'), 2: Lines parallel (no intersection)) */
GEOMEXPORT int DoublePrecisionLineSegmentIntersection(const DPoint2& seg1Start, const DPoint2& seg1End, const DPoint2& seg2Start, const DPoint2& seg2End, DPoint2& intersectPoint);

/** Double-Precision Line Intersection test.
Determines if two infinite lines cross, and if so, where.
\param line1PointA point 1 on the first line
\param line1PointB point 2 on the first line
\param line2PointA point 1 on the second line
\param line2PointB point 2 on the second line
\param intersectPoint if the lines intersect, this will return the intersection point.
\return Intersection determination (0: Lines parallel, 1: Lines intersect (intersection location in 'intersectPoint')) */
GEOMEXPORT int DoublePrecisionLineIntersection(const DPoint2& line1PointA, const DPoint2& line1PointB, const DPoint2& line2PointA, const DPoint2& line2PointB, DPoint2& intersectPoint);
