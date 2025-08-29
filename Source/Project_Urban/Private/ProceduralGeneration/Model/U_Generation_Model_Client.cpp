#include "ProceduralGeneration/Model/U_Generation_Model_Client.h"

void UGenerationModelClient::Initialize(FGridExtentsSize grid_size, int cell_size, TArray<UGenerationRuleset*> all_possible_rulesets, UWorld* world)
{
	implementation_ = NewObject<UGenerationModelImpl>();
	//implementation_->Initialize(grid_size.vector, cell_size, all_possible_rulesets);
	world_ = world;
	is_silent_mode_ = false;
}

TTuple<bool, FGridArrayIndexCoordinate> UGenerationModelClient::CollapseRandomValidTile()
{
	return TTuple<bool, FGridArrayIndexCoordinate>(false, FGridArrayIndexCoordinate(FVector(0,0,0)));	
}

bool UGenerationModelClient::CollapseTile(FGridArrayIndexCoordinate tile_index)
{
	return false;
}

bool UGenerationModelClient::MakeNeighbourPossibilitiesConsistent(FGridArrayIndexCoordinate starting_index,
	EAdjacency to_neighbour_adjacency)
{
	return false;
}

bool UGenerationModelClient::MakeAllNeighboursPossibilitiesConsistent(FGridArrayIndexCoordinate starting_index)
{
	return false;
}

void UGenerationModelClient::MakeGridPossibilitiesConsistent(FGridArrayIndexCoordinate starting_index)
{
}

bool UGenerationModelClient::CollapseBlock(FGridArrayIndexCoordinate bottom_left_index,
	FGridArrayIndexCoordinate block_size)
{
	return false;
}
