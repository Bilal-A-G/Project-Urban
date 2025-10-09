#include "RoadGeneration/BoxManipulator.h"
#include "Kismet/KismetMathLibrary.h"

FVector UBoxManipulator::CalculateMajorEigenVector(FVector point_location)
{
	FVector facing_direction = UKismetMathLibrary::Quat_VectorForward(manipulator_transform_.GetRotation());
	float theta = atan2(facing_direction.Y, facing_direction.X);
	return FVector(cos(theta), sin(theta), 0) * LAMBDA;
}

FVector UBoxManipulator::CalculateMinorEigenVector(FVector point_location)
{
	FVector facing_direction = UKismetMathLibrary::Quat_VectorForward(manipulator_transform_.GetRotation());
	float theta = atan2(facing_direction.Y, facing_direction.X);
	return FVector(cos(theta + PI/2), sin(theta + PI/2), 0) * LAMBDA;
}

void UBoxManipulator::UpdateTransform(FTransform new_transform)
{
	manipulator_transform_ = new_transform;	
}
