#ifndef _VECTOR3_H
#define	_VECTOR3_H

#include <iostream>

class Vector3
{
public:
    Vector3();
    Vector3(const Vector3& v);
    Vector3(float x, float y, float z);
    Vector3(const float v[3]);
    virtual ~Vector3();

    float length() const;
    float lengthSquared() const;
	const Vector3 unit() const;
    const Vector3 crossProduct(const Vector3& v) const;
    float dotProduct(const Vector3& v) const;
    float dotProductSpecial(const Vector3& v) const;
    void inverseMe();

    operator float* ();
    operator const float* () const;

    const Vector3& operator = (const Vector3& v);

    const Vector3& operator = (const float v[3]);
    bool operator == (const Vector3& v) const;
    bool operator != (const Vector3& v) const;
    bool operator == (const float v[3]) const;
    bool operator != (const float v[3]) const;
    Vector3 operator * (const Vector3& v) const;
    const Vector3& operator *= (const Vector3& v);
    Vector3 operator + (const Vector3& v) const;
    const Vector3& operator += (const Vector3& v);
    Vector3 operator - (const Vector3& v) const;
    const Vector3& operator -= (const Vector3& v);
    Vector3 operator * (float s) const;
    const Vector3& operator *= (float s);
    Vector3 operator + (float s) const;
    const Vector3& operator += (float s);

    float x() const { return mVector[0]; }
    void x(float x) { mVector[0] = x; }
    float y() const { return mVector[1]; }
    void y(float y) { mVector[1] = y; }
    float z() const { return mVector[2]; }
    void z(float z) { mVector[2] = z; }

    float* copyTo(float f[3]) const;
    const char* toString(char string[]) const;
	void print() const;

//private:
    float mVector[3];

    friend Vector3 operator * (float s, const Vector3& v);
    friend Vector3 operator + (float s, const Vector3& v);
    friend bool operator == (const float f[3], const Vector3& v);
    friend bool operator != (const float f[3], const Vector3& v);
	
};

Vector3 operator * (float s, const Vector3& v);
Vector3 operator + (float s, const Vector3& v);
bool operator == (const float f[3], const Vector3& v);
bool operator != (const float f[3], const Vector3& v);

std::ostream& operator << (std::ostream& stream, const Vector3& v);

#endif	/* _VECTOR3_H */

