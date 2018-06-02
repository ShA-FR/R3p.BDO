#pragma once

class Vector2 {
public:
	int X, Y;

	Vector2()
	{
		X = 0;
		Y = 0;
	}

	Vector2(int x, int y)
	{
		X = x;
		Y = y;
	}

	const int& GetX() const { return X; }
	const int& GetY() const { return Y; }

	~Vector2()
	{

	}
};