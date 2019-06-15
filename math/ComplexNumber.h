// Copyright 2014-2016 Nicolaus Anderson

template<class T>
struct ComplexNumber
{
	T real;
	T imag; // imaginary component


	ComplexNumber()
		: real( 0 )
		, imag( 0 )
	{}

	ComplexNumber( const ComplexNumber& other )
		: real( other.real )
		, imag( other.imag )
	{}

	ComplexNumber( T realT, T imagT )
		: real( realT )
		, imag( imagT )
	{}

	void set( T realT, T imagT )
	{
		real = realT;
		imag = imagT;
	}

	ComplexNumber& operator = ( const ComplexNumber<T>& other )
	{
		real = other.real;
		imag = other.imag;
		return *this;
	}

	ComplexNumber& operator = ( T& value )
	{
		real = value;
		imag = T(0);
		return *this;
	}

	bool operator == ( ComplexNumber& other )
	{
		return ( real == other.real && imag == other.imag );
	}

	bool operator == ( T& value )
	{
		return ( real == value && imag == T(0) );
	}

	// Basic operators for other == ComplexNumber

	ComplexNumber& operator += ( ComplexNumber& other )
	{
		real += other.real;
		imag += other.imag;
		return *this;
	}

	ComplexNumber operator + ( ComplexNumber& other )
	{
		ComplexNumber temp(*this);
		return (temp += other);
	}

	ComplexNumber& operator -= ( ComplexNumber& other )
	{
		real -= other.real;
		imag -= other.imag;
		return *this;
	}

	ComplexNumber operator - ( ComplexNumber& other )
	{
		ComplexNumber temp(*this);
		return (temp -= other);
	}

	ComplexNumber& operator *= ( ComplexNumber& other )
	{
		T tempReal = real;
		real = tempReal * other.real - imag * other.imag;
		imag = tempReal * other.imag + imag * other.real;
		return *this;
	}

	ComplexNumber operator * ( ComplexNumber& other )
	{
		ComplexNumber temp(*this);
		return (temp *= other);
	}

	ComplexNumber& operator /= ( ComplexNumber& other )
	{
		/* Technically what's happening... (Multiply top and bottom by the complex conjugate of the divisor)*/
		// ComplexNumber cconj = other; // complex conjugate
		// cconj.imag *= -1;
		// ComplexNumber oc = other*cconj; // eliminate imag from divisor
		// real *= cconj.real / oc.real;
		// imag *= cconj.imag / oc.real;

		// Shorthand
		T tReal = other.real * other.real + other.imag * other.imag;
		real *= other.real / tReal;
		imag *= -other.imag / tReal;
		return *this;
	}

	ComplexNumber operator / ( ComplexNumber& other )
	{
		ComplexNumber temp(*this);
		return (temp /= other);
	}

	// Basic operators for other == T

	ComplexNumber& operator += ( T& other )
	{
		real += other;
		return *this;
	}

	ComplexNumber operator + ( T& other )
	{
		ComplexNumber temp(*this);
		return (temp += other);
	}

	ComplexNumber& operator -= ( T& other )
	{
		real -= other;
		return *this;
	}

	ComplexNumber operator - ( T& other )
	{
		ComplexNumber temp(*this);
		return (temp -= other);
	}

	ComplexNumber& operator *= ( T& other )
	{
		real *= other;
		imag *= other;
		return *this;
	}

	ComplexNumber operator * ( T& other )
	{
		ComplexNumber temp(*this);
		return (temp *= other);
	}

	ComplexNumber& operator /= ( T& other )
	{
		real /= other;
		imag /= other;
		return *this;
	}

	ComplexNumber operator / ( T& other )
	{
		ComplexNumber temp(*this);
		return (temp /= other);
	}
};
