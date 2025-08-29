#pragma once

#include "CoreMinimal.h"
#include "U_Generation_Model_Impl.h"
#include "ProceduralGeneration/CoordinateSpaces/F_Grid_Array_Index_Coordinate.h"
#include "ProceduralGeneration/CoordinateSpaces/Sizing/F_Grid_Extents_Size.h"
#include "UObject/Object.h"
#include "U_Generation_Model_Client.generated.h"

UCLASS()
class PROJECT_URBAN_API UGenerationModelClient : public UObject
{
	GENERATED_BODY()
public:
	void Initialize(FGridExtentsSize grid_size, int cell_size, TArray<UGenerationRuleset*> all_possible_rulesets, UWorld* world);
	void DisableSilentMode() {is_silent_mode_ = false;}
	void EnableSilentMode() {is_silent_mode_ = true;}
	
	TTuple<bool, FGridArrayIndexCoordinate> CollapseRandomValidTile();
	bool CollapseTile(FGridArrayIndexCoordinate tile_index);
	bool MakeNeighbourPossibilitiesConsistent(FGridArrayIndexCoordinate starting_index, EAdjacency to_neighbour_adjacency);
	
	bool MakeAllNeighboursPossibilitiesConsistent(FGridArrayIndexCoordinate starting_index);
	void MakeGridPossibilitiesConsistent(FGridArrayIndexCoordinate starting_index);

	bool CollapseBlock(FGridArrayIndexCoordinate bottom_left_index, FGridArrayIndexCoordinate block_size);
private:
	UPROPERTY()
	UGenerationModelImpl* implementation_;
	UPROPERTY()	
	UWorld* world_;
	bool is_silent_mode_;
};
