#pragma once
#include "IGridManipulator.h"

class BoxManipulator : IGridManipulator
{
public:
	virtual FVector GetMajorEigenVector(FVector point_location) override;
	virtual FVector GetMinorEigenVector(FVector point_location) override;
};
