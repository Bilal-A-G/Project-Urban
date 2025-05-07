#pragma once
#include "CoreMinimal.h"
#include "FLabel.generated.h"

//This struct represents a state that a grid cell can be in, it can be user defined or extracted
//from an example model
USTRUCT(Blueprintable)
struct FLabel
{
	GENERATED_BODY()
public:
	FLabel(UStaticMesh* mesh, const FQuat& rotation, const FVector3d& scale)
	{
		this->Mesh = mesh;
		this->Rotation = rotation;
		this->Scale = scale;
	}

	FLabel()
	{
		this->Mesh = nullptr;
		this->Rotation = FQuat::Identity;
		this->Scale = FVector3d::Zero();
	}
public:
	UPROPERTY()
	UStaticMesh* Mesh;
	FQuat Rotation;
	FVector3d Scale;
		
	static FLabel* Null;
};