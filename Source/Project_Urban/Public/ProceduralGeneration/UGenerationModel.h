#pragma once

#include "CoreMinimal.h"
#include "FModelCell.h"
#include "UObject/Object.h"
#include "UGenerationModel.generated.h"

class UGenerationRuleset;

//This class owns all of our model cells and manages operations on them related to the model synthesis algorithm
UCLASS()
class PROJECT_URBAN_API UGenerationModel : public UObject
{
	GENERATED_BODY()
public:
	void Initialize(FVector gridSize, int cellSize, TArray<UGenerationRuleset*>& allPossibleRuleSets);
	void CollapseTile(FVector tileIndex, UWorld* world);
	virtual void BeginDestroy() override;
private:
	TArray<TArray<TArray<FModelCell>>> _grid;
	UPROPERTY()
	TArray<AStaticMeshActor*> _spawnedActors;
	FVector _gridSize;
	int _cellSize;
};
