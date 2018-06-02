#include "Vector3.h"
#include <cmath>

Vector3::Vector3()
{
	X = 0;
	Y = 0;
	Z = 0;
}

Vector3::Vector3(float x, float y, float z)
{
	X = x;
	Y = y;
	Z = z;
}

Vector3::~Vector3()
{
	
}

float Vector3::GetDistance(Vector3  target)
{	
	float xD = X - target.X;
	float yD = Y - target.Y;
	float zD = Z - target.Z;

	return sqrt(xD * xD + yD * yD + zD * zD);
}

float Vector3::Get2DDistance(Vector3  target)
{
	float xD = X - target.X;	
	float zD = Z - target.Z;

	return sqrt(xD * xD + zD * zD);
}

bool Vector3::Equals(Vector3 target)
{
	if (X == target.X && Y == target.Y && Z == target.Z)
		return true;

	return false;
}
