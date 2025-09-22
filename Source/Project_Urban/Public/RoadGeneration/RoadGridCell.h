#pragma once
#include "CoreMinimal.h"

class PROJECT_URBAN_API URoadGridCell 
{
public:
	URoadGridCell()
	{
		colour = FColor(1.0f, 1.0f, 1.0f);
		vector = FVector2d(rand(), rand());
		vector.Normalize();
	}
public:
	FVector2d vector;
	FColor colour;	
};
