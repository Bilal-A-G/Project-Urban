#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ProceduralGenerationCore.generated.h"

class UTileEntryDTO;
class UGenerationModel;

/**
 * This class acts as a layer of abstraction between the BP and the C++ implementation of procedural generation tasks
 */
UCLASS(Blueprintable)
class PROJECT_URBAN_API UProceduralGenerationCore : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void DrawGrid(FVector gridSize, FVector centerPosition, int cellSize, float lineThickness);

	UFUNCTION(BlueprintCallable)
	TArray<AStaticMeshActor*> GetTilesVisualization(FVector visualScale,
		FVector offset, float spacing, UMaterial* material);

	UFUNCTION(BlueprintCallable)
	void Generate(TArray<UTileEntryDTO*> tiles);
	
	UFUNCTION(BlueprintCallable)
	void ClearDebugGizmos();
private:
	FVector gridDimensions;
	int cellDimension;
	UPROPERTY()
	UGenerationModel* model;
};
