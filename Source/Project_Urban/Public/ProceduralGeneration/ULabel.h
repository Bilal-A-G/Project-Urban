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
	UFUNCTION(BlueprintCallable)
	bool Compare(const ULabel* label) const
	{
		return label->Mesh == this->Mesh && label->Rotation.Equals(this->Rotation, 0.01f) && label->Scale.Equals(this->Scale, 0.01f);
	}
	
	bool operator==(const ULabel& Label) const
	{
		return Label.Mesh == this->Mesh && Label.Rotation.Equals(this->Rotation, 0.01f) && Label.Scale.Equals(this->Scale, 0.01f);
	}
public:
	UPROPERTY(BlueprintReadWrite)
	UStaticMesh* Mesh;
	UPROPERTY(BlueprintReadWrite)
	FQuat Rotation;
	UPROPERTY(BlueprintReadWrite)
	FVector Scale;
};