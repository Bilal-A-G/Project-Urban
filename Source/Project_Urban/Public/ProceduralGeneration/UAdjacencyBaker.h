#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UAdjacencyBaker.generated.h"

class ULabel;
class UTileEntryDTO;
/**
 * This class handles baking adjacency data given certain grid dimensions
 */
UCLASS()
class PROJECT_URBAN_API UAdjacencyBaker : public UObject
{
	GENERATED_BODY()

public:
	TArray<UTileEntryDTO*> BakeAdjacencies(UWorld* world, FVector gridDimensions, int cellSize);
private:
	//TODO, move this to a utility class, duplicated code in UGenerationModel as well
	FVector IndexToCoordinates(FVector index, FVector gridSize, int cellSize);
	//TODO, refactor this probably
	//If we found the label, return the DTO else nullptr
	UTileEntryDTO* GetExistingEntry(TArray<UTileEntryDTO*>& array, ULabel* label);
	TTuple<ULabel*, FString> GetLabelAtPosition(UWorld* world, FVector worldPosition, int halfCellSize);
};
