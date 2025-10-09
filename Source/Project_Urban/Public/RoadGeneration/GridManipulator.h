// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GridManipulator.generated.h"

UINTERFACE(Blueprintable)
class UGridManipulator : public UInterface
{
	GENERATED_BODY()
};

class PROJECT_URBAN_API IGridManipulator
{
	GENERATED_BODY()
public:
	virtual FVector CalculateMajorEigenVector(FVector point_location) = 0;
	virtual FVector CalculateMinorEigenVector(FVector point_location) = 0;
	virtual void UpdateTransform(FTransform new_transform) = 0;
	//Calculate the tensor when we actually need it
protected:
	const float LAMBDA = 1;
};
