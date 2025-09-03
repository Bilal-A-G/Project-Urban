#pragma once

struct FGridArrayIndexCoordinate
{
public:
	FGridArrayIndexCoordinate(FVector input)
	{
		vector = input;
	}
	FGridArrayIndexCoordinate()
	{
		vector = FVector(0,0,0);
	}
public:
	FVector vector;
};
