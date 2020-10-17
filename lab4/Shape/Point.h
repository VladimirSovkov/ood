#pragma once

struct Point
{
	double x;
	double y;

	bool operator ==(Point const& point) const
	{
		return (*this).x == point.x && (*this).y == point.y;
	}
 };