#pragma once


class Vector3
{
public:
	float X, Y, Z;

	Vector3();

	Vector3(float x, float y, float z);

	const float& GetX() const { return X; }
	const float& GetY() const { return Y; }
	const float& GetZ() const { return Z; }

	float GetDistanceToMe();

	float GetDistance(Vector3 target);

	float Get2DDistance(Vector3 target);

	bool Equals(Vector3 target);

	~Vector3();
};
