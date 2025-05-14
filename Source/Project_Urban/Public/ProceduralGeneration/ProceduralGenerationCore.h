#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ProceduralGenerationCore.generated.h"

class UTileEntryDTO;
class UGenerationModel;

/**
 * This class handles rules based procedural generation tasks,
 * Ie Wave Function Collapse style generation
 */
UCLASS(Blueprintable)
class PROJECT_URBAN_API UProceduralGenerationCore : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void DrawGrid(FVector gridSize, FVector centerPosition, int cellSize, float lineThickness);

	UFUNCTION(BlueprintCallable)
	void Generate(TArray<UTileEntryDTO*> tiles);
	
	UFUNCTION(BlueprintCallable)
	void ClearDebugGizmos();
private:
	FVector gridDimensions;
	int cellDimension;
	UGenerationModel* model;
};
