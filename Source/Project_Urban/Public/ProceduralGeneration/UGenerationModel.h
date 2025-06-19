#pragma once

#include "CoreMinimal.h"
#include "FModelCell.h"
#include "UObject/Object.h"
#include "UGenerationModel.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGridUpdatedSignature);
class UGenerationRuleset;

//This class owns all of our model cells and manages operations on them related to the model synthesis algorithm
UCLASS()
class PROJECT_URBAN_API UGenerationModel : public UObject
{
	GENERATED_BODY()
public:
	void Initialize(FVector gridSize, int cellSize, TArray<UGenerationRuleset*> allPossibleRuleSets);
	TArray<AStaticMeshActor*> GetPossibleTileVisualization(FVector visualScale,
		UWorld* world, FVector offset, float spacing, UMaterial* material);
	//Returns true if tile is un-collapsed, false if it has already been collapsed
	bool CollapseTile(FVector tileIndex, UWorld* world);
	TTuple<bool, FVector> CollapseRandomValidTile(UWorld* world);
	//Used to visualize the grid
	FLinearColor GetColourAtIndex(FVector index);
	//Called by something to aid visualization
	void SetColourAtIndex(FVector index, FLinearColor colour);
	void ResetColours();
	void ResetVisited();
	//Only call after we have collapsed the tile at this index, the function assumes there's only 1
	//candidate ruleset in that cell, also only collapses 1 neighbour at a time
	bool PropagateToNeighbours(FVector tileIndex, int neighbourIndex);
	void DestroySpawnedActors();
	virtual void BeginDestroy() override;
private:
	FVector TileIndexToCoordinates(FVector index);
public:
	FOnGridUpdatedSignature OnGridUpdated;
private:
	TArray<TArray<TArray<FModelCell>>> _grid;
	UPROPERTY()
	TArray<AStaticMeshActor*> _spawnedActors;
	FVector _gridSize;
	int _cellSize;
};
