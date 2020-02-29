// Copyright 2016-2018 Nicolaus Anderson

namespace math {

class Fraction
{
	//const char INFINITY = 0x01;
	//char flags;
	int num;	// Numerator
	int denom;	// Denominator

public:
	Fraction() : num(0), denom(1) {}

	Fraction(int pNumerator, int pDenominator) : /*flags(0),*/ num(pNumerator), denom(pDenominator)
	{
		//flags &= (!denom & INFINITY); // Set infinity bit if denominator is zero
	}

	Fraction(const Fraction& p)
	{
		*this = p;
	}

	Fraction& operator = (const Fraction& p)
	{
		num = p.num;
		denom = p.denom;
	}

	int numerator() const { return num; }

	void numerator( int value ) { num = value; }

	int denominator() const { return denom; }

	void denominator( int value ) { denom = value; }

	void reduce()
	{
		if ( num < 0 && denom < 0 )
		{
			num = -num;
			denom = -denom;
		}
		if ( denom != 0 ) {
			const int factor = commonFactor(num,denom);
			num /= factor;
			denom /= factor;
		} else {
			if ( num != 0 )
				num = 1;
		}
	}

	Fraction& operator += (const Fraction& p)
	{
		if ( denom == p.denom )
		{
			num += p.num;
			return *this;
		}
		num = (num * p.denom) + (p.num * denom);
		denom *= p.denom;
		reduce();
		return *this;
	}

	Fraction& operator + (const Fraction& p)
	{
		return (Fraction(*this) += p);
	}

	Fraction& operator -= (const Fraction& p)
	{
		return (*this += Fraction( -p.num, p.denom ));
	}

	Fraction& operator - (const Fraction& p)
	{
		return (Fraction(*this) -= p);
	}

	Fraction& operator *= (const Fraction& p)
	{
		num *= p.num;
		denom *= p.denom;
		//flags |= p.flags & INFINITY; // Faster? Better?
		//flags &= (!denom & INFINITY);
		reduce();
		return *this;
	}

	Fraction& operator * (const Fraction& p)
	{
		return (Fraction(*this) -= p);
	}

	Fraction& operator /= (const Fraction& p)
	{
		num *= p.denom;
		denom *= p.num;
		//flags &= (!denom & INFINITY);
		reduce();
		return *this;
	}

	Fraction& operator / (const Fraction& p)
	{
		return (Fraction(*this) /= p);
	}

protected:
	int commonFactor(int p, int q)
	{
		int big, small;
		if ( p >= q ) {
			big = p;
			small = q;
		} else {
			big = q;
			small = p;
		}
		int mod_bg = big % small;
		if ( mod_bg == 1 )
			return 1;
		if ( mod_bg == 0 )
			return small;
		return commonFactor(small,mod_bg);
	}
};


}
