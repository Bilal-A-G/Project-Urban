#pragma once

#include "CoreMinimal.h"
#include "..\F_Model_Cell.h"
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
	void Initialize(FGridExtentsSize grid_size, FGenerationRulesetHolder all_possible_rulesets, bool add_to_valid_collapse = false);
	bool CollapseTile(FGridArrayIndexCoordinate tile_index);
	bool MakeNeighbourPossibilitiesConsistent(FGridArrayIndexCoordinate starting_index, EAdjacency to_neighbour_adjacency);
	
	bool MakeAllNeighboursPossibilitiesConsistent(FGridArrayIndexCoordinate starting_index);
	void MakeGridPossibilitiesConsistent(FGridArrayIndexCoordinate starting_index);
	bool CollapseBlock(FGridArrayIndexCoordinate bottom_left_index, FGridArrayIndexCoordinate block_size);
	TTuple<bool, FGridArrayIndexCoordinate> CollapseRandomValidTile();
private:
	bool ResetGridPossibilitiesInRegion(FGridArrayIndexCoordinate bottom_left_index, FGridArrayIndexCoordinate region_size);
public:
	TArray<FModelCell>& D_GetGrid(){return grid_;}
private:
	TArray<FModelCell> grid_;
	UPROPERTY()
	FGenerationRulesetHolder all_possible_rulesets_;
	FGridCellCountSize grid_size_;
	TArray<FGridArrayIndexCoordinate> valid_collapse_indices_;
};