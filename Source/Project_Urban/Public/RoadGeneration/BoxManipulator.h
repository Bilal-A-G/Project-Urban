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
	virtual FVector CalculateMajorEigenVector(FVector point_location) override;
	virtual FVector CalculateMinorEigenVector(FVector point_location) override;
	virtual void UpdateTransform(FTransform new_transform) override;
private:
	FTransform manipulator_transform_;
};
