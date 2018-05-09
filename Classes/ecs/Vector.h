#pragma once

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

