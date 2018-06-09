#pragma once

namespace game
{
	struct Point
	{
		float x;
		float y;
	};

	struct Size
	{
		float width;
		float height;
		
		Size()
		: width(0), height(0)
		{}
		
		Size(float width, float height)
		: width(width), height(height)
		{}
	};

	struct Rect
	{
		Point origin;
		Size size;

		Rect()
			: origin({ 0, 0 }), size({ 0, 0 })
		{}

		Rect(float x, float y, float width, float height)
		{
			origin.x = x;
			origin.y = y;
			size.width = width;
			size.height = height;
		}
	};
}

template<typename T>
struct Vector2
{
	T X;
	T Y;

	Vector2()
		: X(0), Y(0)
	{}

	Vector2(T x, T y)
		: X(x), Y(y)
	{}

	Vector2<T>& operator=(const Vector2<T>& v)
	{
		X = v.X;
		Y = v.Y;

		return *this;
	}

	Vector2<T>& operator+=(const Vector2<T>& v)
	{
		X += v.X;
		Y += v.Y;

		return *this;
	}

	Vector2<T>& operator-=(const Vector2<T>& v)
	{
		X -= v.X;
		Y -= v.Y;

		return *this;
	}

	Vector2<T>& operator*=(const T v)
	{
		X *= v;
		Y *= v;

		return *this;
	}

	Vector2<T>& operator/=(const T v)
	{
		X /= v;
		Y /= v;

		return *this;
	}
	
	T Length() const
	{
		if (X == 0 && Y == 0) return 0;
		return std::sqrt(X * X + Y * Y);
	}
	
	Vector2<T> Normal() const
	{
		T length = Length();
		if (length == 0)
		{
			return Vector2<T>(0, 0);
		}
		
		T mag = 1 / length;
		return (*this) * mag;
	}
};

template<typename T>
Vector2<T> operator+(const Vector2<T>& u, const Vector2<T>& v)
{
	Vector2<T> t;
	t.X = u.X + v.X;
	t.Y = u.Y + v.Y;

	return t;
}

template<typename T>
Vector2<T> operator-(const Vector2<T>& u, const Vector2<T>& v)
{
	Vector2<T> t;
	t.X = u.X - v.X;
	t.Y = u.Y - v.Y;

	return t;
}

template<typename T>
Vector2<T> operator*(const T u, const Vector2<T>& v)
{
	Vector2<T> t;
	t.X = v.X * u;
	t.Y = v.Y * u;

	return t;
}

template<typename T>
Vector2<T> operator*(const Vector2<T>& v, const T u)
{
	Vector2<T> t;
	t.X = v.X * u;
	t.Y = v.Y * u;

	return t;
}

template<typename T>
struct Vector3
{
	T X;
	T Y;
	T Z;
	
	Vector3()
	: X(0), Y(0), Z(0)
	{}
	
	Vector3(T x, T y, T z)
	: X(x), Y(y), Z(z)
	{}

	Vector2<T>& toVector2()
	{
		Vector2<T> t;
		t.X = X;
		t.Y = Y;

		return t;
	}
	
	Vector3<T>& operator=(const Vector3<T>& v)
	{
		X = v.X;
		Y = v.Y;
		Z = v.Z;
		
		return *this;
	}
	
	Vector3<T>& operator+=(const Vector3<T>& v)
	{
		X += v.X;
		Y += v.Y;
		Z += v.Z;
		
		return *this;
	}
	
	Vector3<T>& operator-=(const Vector3<T>& v)
	{
		X -= v.X;
		Y -= v.Y;
		Z -= v.Z;
		
		return *this;
	}
	
	Vector3<T>& operator*=(const T v)
	{
		X *= v;
		Y *= v;
		Z *= v;
		
		return *this;
	}
	
	Vector3<T>& operator/=(const T v)
	{
		X /= v;
		Y /= v;
		Z /= v;
		
		return *this;
	}
};

template<typename T>
Vector3<T> operator+(const Vector3<T>& u, const Vector3<T>& v)
{
	Vector3<T> t;
	t.X = u.X + v.X;
	t.Y = u.Y + v.Y;
	t.Z = u.Z + v.Z;
	
	return t;
}

template<typename T>
Vector3<T> operator-(const Vector3<T>& u, const Vector3<T>& v)
{
	Vector3<T> t;
	t.X = u.X - v.X;
	t.Y = u.Y - v.Y;
	t.Z = u.Z - v.Z;
	
	return t;
}

template<typename T>
Vector3<T> operator*(const T u, const Vector3<T>& v)
{
	Vector3<T> t;
	t.X = v.X * u;
	t.Y = v.Y * u;
	t.Z = v.Z * u;
	
	return t;
}

template<typename T>
Vector3<T> operator*(const Vector3<T>& v, const T u)
{
	Vector3<T> t;
	t.X = v.X * u;
	t.Y = v.Y * u;
	t.Z = v.Z * u;
	
	return t;
}


using Vector2f = Vector2<float>;
using Vector3f = Vector3<float>;

template<typename T>
struct MaxValue
{
	T Current;
	T Max;
	
	MaxValue()
	: Current(0), Max(0)
	{}
	
	MaxValue(T current, T max)
	: Current(current), Max(max)
	{}
};

