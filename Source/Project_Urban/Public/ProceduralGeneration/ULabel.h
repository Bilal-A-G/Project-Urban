#pragma once
#include "CoreMinimal.h"
#include "ULabel.generated.h"

//This represents a state that a model cell can be in
//Is a class because we want to share them for the purposes of adjacency, would waste a bit of memory duplicating the
//rotation and scale otherwise
UCLASS(Blueprintable)
class PROJECT_URBAN_API ULabel : public UObject
{
	GENERATED_BODY()
public:
	void Initialize(UStaticMesh* mesh, const FQuat& rotation, const FVector3d& scale)
	{
		this->Mesh = mesh;
		this->Rotation = rotation;
		this->Scale = scale;
	}
	
	bool operator==(const ULabel& Label) const
	{
		return Label.Mesh == this->Mesh && Label.Rotation == this->Rotation && Label.Scale == this->Scale;
	}
public:
	UPROPERTY()
	UStaticMesh* Mesh;
	FQuat Rotation;
	FVector3d Scale;
};