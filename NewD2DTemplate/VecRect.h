#pragma once
#include "Vec2.h"
#include "Rect.h"

template<typename VecType>
class Extents
{
public:
	typedef VecType type;
private:
	_Vec2<type> m_min;
	_Vec2<type> m_max;
public:
	Extents() {}
	
	Extents(type w, type h)
	{
		m_min.x = -(w / (type)2);
		m_min.y = -(w / (type)2);
		m_max.x = (w / (type)2);
		m_max.y = (h / (type)2);

	};
	
	_Vec2<type> GetMin()
	{
		return m_min;
	}
	_Vec2<type> GetMax()
	{
		return m_max;
	}
	Rect<type> GetRect(_Vec2<type> pos)
	{
		_Vec2<type> pt1 = pos + m_min;
		_Vec2<type> pt2 = pos + m_max;
		return Rect<type>(pt1.x, pt1.y, pt2.x, pt2.y);

	}

	
};
typedef Extents<float> ExtentsF;
typedef Extents<int> ExtentsI;