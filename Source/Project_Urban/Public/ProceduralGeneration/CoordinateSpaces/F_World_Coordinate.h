#pragma once

struct FWorldCoordinate
{
public:
	FWorldCoordinate(FVector input)
	{
		vector = input;
	}
public:
	FVector vector;
};
