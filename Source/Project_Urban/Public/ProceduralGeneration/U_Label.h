#pragma once
#include "CoreMinimal.h"
#include "U_Label.generated.h"

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
		mesh_ = mesh;
		rotation_ = rotation;
		scale_ = scale;
	}
	UFUNCTION(BlueprintCallable)
	bool Compare(const ULabel* label) const
	{
		return label->mesh_ == this->mesh_ && label->rotation_.Equals(this->rotation_, 0.01f) && label->scale_.Equals(this->scale_, 0.01f);
	}
	
	bool operator==(const ULabel& Label) const
	{
		return Label.mesh_ == this->mesh_ && Label.rotation_.Equals(this->rotation_, 0.01f) && Label.scale_.Equals(this->scale_, 0.01f);
	}
public:
	UPROPERTY(BlueprintReadWrite)
	UStaticMesh* mesh_;
	UPROPERTY(BlueprintReadWrite)
	FQuat rotation_;
	UPROPERTY(BlueprintReadWrite)
	FVector scale_;
};