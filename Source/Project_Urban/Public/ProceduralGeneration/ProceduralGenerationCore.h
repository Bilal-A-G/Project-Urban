#pragma once

#include "CoreMinimal.h"
#include "UGenerationModel.h"
#include "UObject/Object.h"
#include "ProceduralGenerationCore.generated.h"

class UCommandPlayer;
class UTileEntryDTO;

/**
 * This class acts as a layer of abstraction between the BP and the C++ implementation of procedural generation tasks
 */
UCLASS(Blueprintable)
class PROJECT_URBAN_API UProceduralGenerationCore : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void Init();
	UFUNCTION(BlueprintCallable)
	void DrawGrid(FVector gridSize, FVector centerPosition,
		int cellSize, float lineThickness);
	UFUNCTION(BlueprintCallable)
	void DrawVisualizations(FVector gridSize, FVector centerPosition,
		int cellSize, float lineThickness, TArray<UTileEntryDTO*> tiles);
	UFUNCTION(BlueprintCallable)
	TArray<AStaticMeshActor*> GetTilesVisualization(FVector visualScale,
		FVector offset, float spacing, UMaterial* material);
	UFUNCTION(BlueprintCallable)
	void StepForwards();
	UFUNCTION(BlueprintCallable)
	void ClearDebugGizmos();
	UFUNCTION(BlueprintCallable)
	void ClearAll();
private:
	UFUNCTION()
	void OnGridChanged();
public:
	UPROPERTY(BlueprintAssignable)
	FOnGridUpdatedSignature OnGridUpdated;
private:
	FVector gridDimensions;
	TArray<UGenerationRuleset*> lastUsedAllPossibleRuleSets;
	int cellDimension;
	UPROPERTY()
	UGenerationModel* model;
	UPROPERTY()
	UCommandPlayer* commandPlayer;
};
