/*
(c) 2012-2013 Nicolaus Anderson
Created: Dec 24, 2012

Incrementor - A simple incrementing class for keeping track of changes in a value
with fewer keystrokes.
*/

#include "stdtypes.h"

#ifndef __INCREMENTOR__
#define __INCREMENTOR__


enum ECycleType
{
	/* This incrementor is to act like a boolean value, switching back and forth
	between 0 and 1. */
	CYC_BOOLEAN = 0,
	
	/* This incrementor must be set to the min value after reaching the max value
	or must be set to the max value if decremented from the min value. */
	CYC_REPEAT,
	
	/* This incrementor counts up and down like an ordinary integer. */
	CYC_DEFAULT
};

class Inc
{
	stds32 value;
	stds32 step;
	stds32 min;
	stds32 max;

	ECycleType cycle;

public:

	//! Constructor
	Inc(
		ECycleType type = CYC_DEFAULT,
		stds32 start_val=0, stds32 start_step=1, stds32 start_min=0, stds32 start_max=1
		)
		: value( start_val ), step( start_step ), min(start_min), max( start_max ), cycle( type )
	{}
	
	
	//! Deconstructor
	~Inc() {}
	
	
	//! Get type of cycle
	ECycleType getCycleType()
	{
		return cycle;
	}
	
	//! Set the cycle type directly
	void setCycleType( ECycleType type )
	{
		cycle = type;
	}
	
	//! Set the cycle type to the default
	void ctDef()
	{
		cycle = CYC_DEFAULT;
	}
	
	//! Set the cycle type to boolean
	void ctBool()
	{
		cycle = CYC_BOOLEAN;
	}
	
	//! Set the cycle type to repeat
	void ctRpt()
	{
		cycle = CYC_REPEAT;
	}

	//! Checks if out of bounds - fixes if necessary
	bool outOfBounds()
	{
		switch ( cycle )
		{
		case CYC_BOOLEAN:
			if ( value > 1 )
			{
				value = 0;
				return true;
			} else if ( value < 0 )
			{
				value = 1;
				return true;
			}
			break;

		case CYC_REPEAT:
			if ( value > max )
			{
				// Wrap around if greater than the max
				value = (value - max) + min;
				return true;
			} else if ( value < min )
			{
				// Wrap around if greater than the max
				value = (value - max) + min;
				return true;
			}

		default:
			if ( value < min || value > max )
				return true;
			break;
		}

		return false;
	}


//******* Getters and setters **********

	stds32& Val()
	{
		return value;
	}

	stdu32 uInt()
	{
		return (stdu32)value;
	}

	stds32& getStep()
	{
		return step;
	}
	
	stds32& getMin()
	{
		return min;
	}
	
	stds32& getMax()
	{
		return max;
	}

	void setVal( stds32 new_val )
	{
		value = new_val;
	}
	
	void setStep( stds32 new_step )
	{
		step = new_step;
	}
	
	void setMin( stds32 new_min )
	{
		min = new_min;
		clampVal();
	}
	
	void setMax( stds32 new_max )
	{
		max = new_max;
		clampVal();
	}

	void setMinShiftMax( stds32 new_min )
	{
		max += new_min - min;
		min = new_min;
		clampVal();
	}

	void setMaxShiftMin( stds32 new_max )
	{
		min += new_max - max;
		max = new_max;
		clampVal();
	}

	void setRange( stds32 new_min, stds32 new_max )
	{
		min = new_min;
		max = new_max;
		clampVal();
	}

	// Range modifiers
#ifdef __RANGE_CLASS__
	Range<stds32> getRange()
	{
		return Range<stds32>( min, max );
	}
	
	void setRange ( Range<stds32>& range )
	{
		min = range.start;
		max = range.end;
		clampVal();
	}

	Range<stds32>& operator= ( Range<std32>& range )
	{
		min = range.start;
		max = range.end;
		clampVal();
		return *this;
	}
#endif

	void restart()
	{
		value = min;
	}

	void clampVal()
	{
		if ( min > max )
		{
			stds32 m = min;
			min = max;
			max = m;
		}

		if ( value < min )
			value = min;

		else if ( max < value )
			value = max;
	}


// ********* Shortcut operators ***********
	
	bool operator++ ()
	{
		switch( cycle )
		{
		case CYC_BOOLEAN:
			value++;
			break;
		
		case CYC_REPEAT:
			if ( value == max )
			{
				value = min;
				return true;
			} else {
				value += step;
			}
			break;
		
		default:
			value += step;
		}

		return outOfBounds();
	}

	bool operator++ ( stds32 )
	{
		return ++(*this);
	}
	
	bool operator-- ()
	{
		switch( cycle )
		{
		case CYC_BOOLEAN:
			value--;
			break;
			
		case CYC_REPEAT:
			if ( value == min )
			{
				value = max;
				return true;
			} else {
				value -= step;
			}
			break;
			
		default:
			value -= step;
		}

		return outOfBounds()
	}

	bool operator-- ( stds32 )
	{
		return --(*this);
	}
	
	bool operator==( Inc& other )
	{
		return value == other.value;
	}
	
	bool operator>( Inc& other )
	{
		return value > other.value;
	}
	
	bool operator>=( Inc& other )
	{
		return value >= other.value;
	}
	
	bool operator<( Inc& other )
	{
		return value < other.value;
	}
	
	bool operator<=( Inc& other )
	{
		return value <= other.value;
	}
	
	Inc& operator=( Inc& other )
	{
		return copy(other);
	}
	
	Inc& copy( Inc& other )
	{
		value = other.value;
		step = other.step;
		min = other.min;
		max = other.max;
		cycle = other.cycle;
		
		return *this;
	}

	/* REMOVED
	Reason: Working with operator=( stds32 val ) it overrides
	the role of operator=( Inc& other )

	template<class T>
	operator T ()
	{
		return (T)value;
	}
	*/

//****** assignment operators *******

	Inc& operator+=( Inc& other )
	{
		value = other.value;

		return *this;
	}

	Inc& operator-=( Inc& other )
	{
		value -= other.value;

		return *this;
	}

	Inc& operator=( stds32 val )
	{
		value = val;

		return *this;
	}

	Inc& operator+=( stds32 val )
	{
		value += val;

		return *this;
	}
	Inc& operator-=( stds32 val )
	{
		value -= val;

		return *this;
	}

	Inc& operator=( stdu32 val )
	{
		value = (stds32)val;

		return *this;
	}

	Inc& operator+=( stdu32 val )
	{
		value += (stds32)val;

		return *this;
	}

	Inc& operator-=( stdu32 val )
	{
		value -= (stds32)val;

		return *this;
	}
};

#endif // define __INCREMENTOR__