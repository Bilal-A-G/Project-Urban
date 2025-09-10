#pragma once

#include "CoreMinimal.h"
#include "U_Tile_Entry_DTO.h"
#include "CoordinateSpaces/F_World_Coordinate.h"
#include "CoordinateSpaces/Sizing/F_Grid_Extents_Size.h"
#include "UObject/Object.h"
#include "U_Adjacency_Baker.generated.h"

/**
 * This class handles baking adjacency data given certain grid dimensions
 */
UCLASS()
class PROJECT_URBAN_API UAdjacencyBaker : public UObject
{
	GENERATED_BODY()

public:
	TArray<UTileEntryDTO*> BakeAdjacencies(UWorld* world, FGridExtentsSize grid_dimensions, int cell_size);
private:
	//TODO, refactor this probably
	//If we found the label, return the DTO else nullptr
	UTileEntryDTO* GetExistingEntry(TArray<UTileEntryDTO*>& array, ULabel* label);
	TTuple<ULabel*, FString> GetLabelAtPosition(UWorld* world, FWorldCoordinate world_position, int half_cell_size);
};
