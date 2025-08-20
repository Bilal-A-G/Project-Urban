#pragma once

#include "CoreMinimal.h"
#include "FModelCell.h"
#include "UObject/Object.h"
#include "UGenerationModel.generated.h"

class ULabel;
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
	bool CollapseTile(FVector tileIndex, UWorld* world, bool update);
	TTuple<bool, FVector> CollapseRandomValidTile(UWorld* world, bool update);
	//Used to visualize the grid
	FLinearColor GetColourAtIndex(FVector index);
	//Called by something to aid visualization
	void SetColourAtIndex(FVector index, FLinearColor colour);
	void ResetColours();
	void ResetVisited();
	//Collapses a single chunk of the model as specified by the block size and the bottom left index, only collapses edges, ensures everything in
	//The model is consistent with each other and their neighbours outside the block
	//Returns true if collapsed, false if air tile is null
	bool CollapseBlock(FVector bottomLeftIndex, FVector blockSize, bool updateGrid);
	//Only propagates 1 neighbour at a time
	bool PropagateToNeighbours(FVector tileIndex, int neighbourIndex, bool updateGrid);
	//Propagates changes to the tile index and neighbour combo across the entire grid
	void RecursivePropagateToNeighbours(FVector tileIndex, int neighbourIndex);
	void RecursivePropagateToAllNeighbours(FVector tileIndex);
	void DestroySpawnedActors();
	virtual void BeginDestroy() override;
private:
	FVector TileIndexToCoordinates(FVector index);
public:
	FOnGridUpdatedSignature OnGridUpdated;
	FOnGridUpdatedSignature OnOnlyColoursUpdated;
private:
	TArray<TArray<TArray<FModelCell>>> _grid;
	TArray<UGenerationRuleset*> _allPossibleRulesets;
	UGenerationRuleset* _airRuleset;
	TArray<FVector> _validCollapseIndices;
	UPROPERTY()
	TArray<AStaticMeshActor*> _spawnedActors;
	FVector _gridSize;
	int _cellSize;
};
