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
	void Init(FVector gridSize, int cellSize, TArray<FLabel> allPossibleLabels, UWorld* world);
	//Choose a random label in the model cell and collapse the cell to it
	void CollapseTile(FVector tileIndex, UWorld* world);
	void DestroyAllSpawnedActors(UWorld* world);
private:
	TArray<TArray<TArray<FModelCell*>>> _grid;
	TArray<AStaticMeshActor*> _spawnedActors;
	FVector _gridSize;
	int _cellSize;
};
