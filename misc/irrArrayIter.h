#include <irrArray.h>

namespace irr {
namespace core {

template<class T>
class aItr
{
	array<T>* a;
	u32 i;

public:
	aItr( array<T>& pArray )
	{
		a = pArray;
	}
	
	T& operator++ ()
	{
		if ( i < a.Size()-1 )
			i++;
		return (*a)[i];
	}
	
	T& operator-- ()
	{
		if ( i > 0 )
			i--;
		return (*a)[i];
	}
	
	T& start()
	{
		i = 0;
		return (*a)[i];
	}
	
	T& end()
	{
		i = a.size() - 1;
		return (*a)[i];
	}
	
	T& goTo( u32 pIndex )
	{
		i = clamp(pIndex,0,a.Size()-1);
		return (*a)[i];
	}
	
	T& operator* ()
	{
		return (*a)[i];
	}
}

}}