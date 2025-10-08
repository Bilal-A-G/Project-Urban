#include "RoadGeneration/BoxManipulator.h"
#include "Kismet/KismetMathLibrary.h"

FVector UBoxManipulator::CalculateMajorEigenVector(FVector point_location, FTransform manipulator_transform)
{
	FVector facing_direction = UKismetMathLibrary::Quat_VectorForward(manipulator_transform.GetRotation());
	float theta = atan2(facing_direction.Z, facing_direction.X);
	return FVector(cos(theta), sin(theta), 0) * LAMBDA;
}

FVector UBoxManipulator::CalculateMinorEigenVector(FVector point_location, FTransform manipulator_transform)
{
	FVector facing_direction = UKismetMathLibrary::Quat_VectorForward(manipulator_transform.GetRotation());
	float theta = atan2(facing_direction.Z, facing_direction.X);
	return FVector(cos(theta + PI/2), sin(theta + PI/2), 0) * LAMBDA;
}
