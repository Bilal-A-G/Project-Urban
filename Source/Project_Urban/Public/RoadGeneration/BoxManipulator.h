#pragma once

#include "CoreMinimal.h"
#include "GridManipulator.h"
#include "UObject/Object.h"
#include "BoxManipulator.generated.h"

UCLASS(Blueprintable)
class PROJECT_URBAN_API UBoxManipulator : public UObject, public IGridManipulator
{
	GENERATED_BODY()
public:
	virtual FVector CalculateMajorEigenVector(FVector point_location, FTransform manipulator_transform) override;
	virtual FVector CalculateMinorEigenVector(FVector point_location, FTransform manipulator_transform) override;
};
