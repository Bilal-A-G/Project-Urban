#include "RoadGeneration/BoxManipulator.h"
#include "Kismet/KismetMathLibrary.h"

FVector BoxManipulator::GetMajorEigenVector(FVector point_location)
{
	FVector facing_direction = UKismetMathLibrary::Quat_VectorForward(current_transform.GetRotation());
	float theta = atan2(facing_direction.Z, facing_direction.X);
	return FVector(cos(theta), sin(theta), 0) * LAMBDA;
}

FVector BoxManipulator::GetMinorEigenVector(FVector point_location)
{
	FVector facing_direction = UKismetMathLibrary::Quat_VectorForward(current_transform.GetRotation());
	float theta = atan2(facing_direction.Z, facing_direction.X);
	return FVector(cos(theta + PI/2), sin(theta + PI/2), 0) * LAMBDA;
}