#include "object.h"
#include <cassert>

namespace Base
{
	Object::~Object()
	{
		assert( m_ReferenceCount == 0 );
	}

	void Object::AddRef() const
	{
		++m_ReferenceCount;
	}

	void Object::RemoveRef() const
	{
		--m_ReferenceCount;

		if( m_ReferenceCount == 0 )
		{
			delete this;
		}
	}
}