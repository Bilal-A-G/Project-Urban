#pragma once

class IGridManipulator
{
public:
	virtual ~IGridManipulator() = default;
	virtual FVector GetMajorEigenVector(FVector point_location) = 0;
	virtual FVector GetMinorEigenVector(FVector point_location) = 0;
	void UpdateTransform(FTransform new_transform);
	//TODO: Implement later
	//virtual float GetTensorValueAt(FTransform gizmo_transform, FVector point_location) = 0;
protected:
	const float LAMBDA = 1;
	FTransform current_transform;
};
