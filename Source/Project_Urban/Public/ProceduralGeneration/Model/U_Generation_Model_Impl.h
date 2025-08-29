#pragma once

#include "CoreMinimal.h"
#include "ProceduralGeneration/FModelCell.h"
#include "ProceduralGeneration/UGenerationRuleset.h"
#include "ProceduralGeneration/CoordinateSpaces/F_Grid_Array_Index_Coordinate.h"
#include "ProceduralGeneration/CoordinateSpaces/Sizing/F_Grid_Cell_Count_Size.h"
#include "ProceduralGeneration/CoordinateSpaces/Sizing/F_Grid_Extents_Size.h"
#include "UObject/Object.h"
#include "U_Generation_Model_Impl.generated.h"

UCLASS()
class PROJECT_URBAN_API UGenerationModelImpl: public UObject
{
	GENERATED_BODY()
public:
	void Initialize(FGridExtentsSize grid_size, TArray<UGenerationRuleset*> all_possible_rulesets);
	bool CollapseTile(FGridArrayIndexCoordinate tile_index);
	bool PropagateToNeighbour(FGridArrayIndexCoordinate tile_index, int neighbour_index);
public:
	TArray<FModelCell>& D_GetGrid(){return grid_;}
private:
	TArray<FModelCell> grid_;
	FGridCellCountSize grid_size_;
};