// Copyright 2014-2016 Nicolaus Anderson

#ifndef SPACEMATRIX_H
#define SPACEMATRIX_H

#include <irrMath.h>
#include <vector3d.h>
#include <matrix4.h>
#include <quaternion.h>

using irr::f32;
using irr::core::abs_;
using irr::core::radToDeg;
using irr::core::degToRad;
using irr::core::vector3df;
using irr::core::matrix4;
using irr::core::quaternion;

class SpaceMatrix
{
	vector3df translation;
	vector3df rotation;
	vector3df scale;
	bool dirty;

	// Rotation matrix
	f32 RM[3][3];

public:

	SpaceMatrix()
		: translation(0.f,0.f,0.f)
		, rotation(0.f,0.f,0.f)
		, scale(1.f,1.f,1.f)
		, dirty(false)
	{
		makeRotationIdentity();
	}

	SpaceMatrix( const SpaceMatrix& pOther )
		: translation( pOther.translation )
		, rotation( pOther.rotation ) // radians
		, scale( pOther.scale )
		, dirty( pOther.dirty )
	{
		if ( ! dirty )
		{
			RM[0][0] = pOther.RM[0][0];
			RM[0][1] = pOther.RM[0][1];
			RM[0][2] = pOther.RM[0][2];
			RM[1][0] = pOther.RM[1][0];
			RM[1][1] = pOther.RM[1][1];
			RM[1][2] = pOther.RM[1][2];
			RM[2][0] = pOther.RM[2][0];
			RM[2][1] = pOther.RM[2][1];
			RM[2][2] = pOther.RM[2][2];
		}
	}

	SpaceMatrix( const vector3df pTranslation, const vector3df pRotation, const vector3df pScale )
		: translation( pTranslation )
		, rotation( pRotation )
		, scale( pScale )
		, dirty(true)
	{
		makeRotationIdentity();
	}

	SpaceMatrix& operator= ( const SpaceMatrix& pOther )
	{
		translation = pOther.translation;
		rotation = pOther.rotation;
		scale = pOther.scale;
		dirty = pOther.dirty;
		if ( ! dirty )
		{
			RM[0][0] = pOther.RM[0][0];
			RM[0][1] = pOther.RM[0][1];
			RM[0][2] = pOther.RM[0][2];
			RM[1][0] = pOther.RM[1][0];
			RM[1][1] = pOther.RM[1][1];
			RM[1][2] = pOther.RM[1][2];
			RM[2][0] = pOther.RM[2][0];
			RM[2][1] = pOther.RM[2][1];
			RM[2][2] = pOther.RM[2][2];
		}
		return *this;
	}

	SpaceMatrix& operator+= ( const SpaceMatrix& pOther )
	{
		translation += pOther.translation;
		rotation += pOther.rotation;
		scale *= pOther.scale;
		dirty = true;
		return *this;
	}

	SpaceMatrix& operator+ ( const SpaceMatrix& pOther )
	{
		return SpaceMatrix(*this) += pOther;
	}

	SpaceMatrix& operator-= ( const SpaceMatrix& pOther )
	{
		translation -= pOther.translation;
		rotation -= pOther.rotation;
		scale /= pOther.scale; // Causes failure if any value of pOther.scale is zero
		dirty = true;
		return *this;
	}

	SpaceMatrix& operator- ( const SpaceMatrix& pOther )
	{
		return SpaceMatrix(*this) -= pOther;
	}

	SpaceMatrix& operator+= ( const matrix4 pMatrix )
	{
		translation += pMatrix.getTranslation();
		setRotationDegrees( getRotationDegrees() + pMatrix.getRotationDegrees() );
		scale *= pMatrix.getScale();
		dirty = true;
		return *this;
	}

	SpaceMatrix operator+ ( const matrix4 pMatrix )
	{
		return SpaceMatrix(*this) += pMatrix;
	}

	void reset()
	{
		translation.set(0.f,0.f,0.f);
		rotation.set(0.f,0.f,0.f);
		scale.set(1.f,1.f,1.f);
		makeRotationIdentity();
		dirty = false;
	}

	void makeRotationIdentity()
	{
		RM[0][0] = 1; RM[0][1] = 0; RM[0][2] = 0;
		RM[1][0] = 0; RM[1][1] = 1; RM[1][2] = 0;
		RM[2][0] = 0; RM[2][1] = 0; RM[2][2] = 1;
	}

	void setTranslation( vector3df pTranslation )
	{
		translation = pTranslation;
	}

	void setRotationRadians( vector3df pRotation )
	{
		rotation = pRotation;
		dirty = true;
	}

	void setRotationDegrees( vector3df pRotation )
	{
		rotation.X = degToRad( pRotation.X );
		rotation.Y = degToRad( pRotation.Y );
		rotation.Z = degToRad( pRotation.Z );
		dirty = true;
	}

	void setScale( vector3df pScale )
	{
		scale = pScale;
#ifdef PREVENT_NEGATIVE_MATRIX_SCALE
		// Prevent scale sizes < 0
		scale.X = abs_( scale.X );
		scale.Y = abs_( scale.Y );
		scale.Z = abs_( scale.Z );
#endif
	}

	void addTranslation( vector3df pTranslation )
	{
		translation += pTranslation;
	}

	void addRotationRadians( vector3df pRotationRadians )
	{
		rotation += pRotationRadians;
		dirty = true;
	}

	void amplifyScale( vector3df pScale )
	{
		scale *= pScale;
#ifdef PREVENT_NEGATIVE_MATRIX_SCALE
		// Prevent scale sizes < 0
		scale.X = abs_( scale.X );
		scale.Y = abs_( scale.Y );
		scale.Z = abs_( scale.Z );
#endif
	}

	vector3df getTranslation() const
	{
		return translation;
	}

	vector3df getRotationRadians() const
	{
		return rotation;
	}

	vector3df getRotationDegrees()
	{
		return vector3df( radToDeg(rotation.X), radToDeg(rotation.Y), radToDeg(rotation.Z) );
	}

	vector3df getScale() const
	{
		return scale;
	}

	SpaceMatrix getInverse()
	{
		return SpaceMatrix( - translation, - rotation, vector3df(1) / scale );
	}

	SpaceMatrix& invert()
	{
		translation *= -1;
		rotation *= -1;
		scale = vector3df(1) / scale;
		dirty = true;
		return *this;
	}

	// Needed function for MeshUnit::applyLocalizedSpaceTransform()
	SpaceMatrix& localizeWith( const SpaceMatrix& pMatrix )
	{
		pMatrix.constScaleRotateVect( translation );
		return *this;
	}

	void scaleTranslation( const vector3df& vec )
	{
		translation *= vec;
	}

	void translateVect( vector3df& vec ) const
	{
		vec += translation;
	}

	inline void rotateVect( vector3df& vec )
	{
		if ( dirty )
			calcRotation();
		vector3df v = vec;
		vec.X = v.X*RM[0][0] + v.Y*RM[0][1] + v.Z*RM[0][2];
		vec.Y = v.X*RM[1][0] + v.Y*RM[1][1] + v.Z*RM[1][2];
		vec.Z = v.X*RM[2][0] + v.Y*RM[2][1] + v.Z*RM[2][2];
	}

	inline void inverseRotateVect( vector3df& vec )
	{
		if ( dirty )
			calcRotation();
		vector3df v = vec;
		vec.X = v.X*RM[0][0] + v.Y*RM[1][0] + v.Z*RM[2][0];
		vec.Y = v.X*RM[0][1] + v.Y*RM[1][1] + v.Z*RM[2][1];
		vec.Z = v.X*RM[0][2] + v.Y*RM[1][2] + v.Z*RM[2][2];
	}

	// Warning: Must call calcRotation() in non-const zone for this to be correct
	void constRotateVect( vector3df& vec ) const
	{
		vector3df v = vec;
		vec.X = v.X*RM[0][0] + v.Y*RM[0][1] + v.Z*RM[0][2];
		vec.Y = v.X*RM[1][0] + v.Y*RM[1][1] + v.Z*RM[1][2];
		vec.Z = v.X*RM[2][0] + v.Y*RM[2][1] + v.Z*RM[2][2];
	}

	// Warning: Must call calcRotation() in non-const zone for this to be correct
	void constInverseRotateVect( vector3df& vec ) const
	{
		vector3df v = vec;
		vec.X = v.X*RM[0][0] + v.Y*RM[1][0] + v.Z*RM[2][0];
		vec.Y = v.X*RM[0][1] + v.Y*RM[1][1] + v.Z*RM[2][1];
		vec.Z = v.X*RM[0][2] + v.Y*RM[1][2] + v.Z*RM[2][2];
	}
	
	inline void scaleVect( vector3df& vec ) const
	{
		vec *= scale;
	}


	void scaleRotateVect( vector3df& vec )
	{
		vec *= scale;
		rotateVect(vec);
	}

	void inverseScaleRotateVect( vector3df& vec )
	{
		inverseRotateVect(vec);
		vec /= scale;
	}

	// Warning: Must call calcRotation() in non-const zone for this to be correct
	void constScaleRotateVect( vector3df& vec ) const
	{
		vec *= scale;
		constRotateVect(vec);
	}

	// Warning: Must call calcRotation() in non-const zone for this to be correct
	void constInverseScaleRotateVect( vector3df& vec ) const
	{
		constInverseRotateVect(vec);
		vec /= scale;
	}

	void transformVect( vector3df& vec )
	{
		if ( dirty )
		{
			calcRotation();
		}

		vec *= scale;

		// Apply rotation
		vector3df v = vec;
		vec.X = v.X*RM[0][0] + v.Y*RM[0][1] + v.Z*RM[0][2];
		vec.Y = v.X*RM[1][0] + v.Y*RM[1][1] + v.Z*RM[1][2];
		vec.Z = v.X*RM[2][0] + v.Y*RM[2][1] + v.Z*RM[2][2];

		vec += translation;
	}

	void inverseTransformVect( vector3df& vec )
	{
		vec -= translation;

		if ( dirty )
		{
			calcRotation();
		}

		// Apply rotation
		vector3df v = vec;
		vec.X = v.X*RM[0][0] + v.Y*RM[1][0] + v.Z*RM[2][0];
		vec.Y = v.X*RM[0][1] + v.Y*RM[1][1] + v.Z*RM[2][1];
		vec.Z = v.X*RM[0][2] + v.Y*RM[1][2] + v.Z*RM[2][2];

		vec /= scale;
	}

	void calcRotation()
	{
		quaternion q;
		matrix4 rm = q.set( rotation ).getMatrix();

		RM[0][0] = rm[0];
		RM[1][0] = rm[1];
		RM[2][0] = rm[2];

		RM[0][1] = rm[4];
		RM[1][1] = rm[5];
		RM[2][1] = rm[6];

		RM[0][2] = rm[8];
		RM[1][2] = rm[9];
		RM[2][2] = rm[10];

		dirty = false;
	}
};

#endif // #ifndef SPACEMATRIX_H
