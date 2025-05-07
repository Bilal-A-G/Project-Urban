#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UGenerationModel.generated.h"

struct FLabel;
struct FModelCell;

UCLASS()
class PROJECT_URBAN_API UGenerationModel : public UObject
{
	GENERATED_BODY()
public:
	UGenerationModel(){}
	void Init(FVector gridSize, TArray<FLabel*> allPossibleLabels);
private:
	TArray<TArray<TArray<FModelCell*>>> grid;
};
