#include "ProceduralGeneration/Model/U_Generation_Model_Impl.h"
#include "ProceduralGeneration/CoordinateSpaces/INC_Coordinate_Space_Converter.h"
#include "ProceduralGeneration/CoordinateSpaces/Sizing/INC_Size_Type_Converter.h"
#include "ProceduralGeneration/Model/Utility/INC_Model_Utilities.h"

void UGenerationModelImpl::Initialize(FGridExtentsSize grid_size, TArray<UGenerationRuleset*> all_possible_rulesets)
{
	grid_.Empty();
	FGridCellCountSize to_cell_count = INCSizeTypeConverter::GridExtentsToCellCount(grid_size);
	grid_size_ = to_cell_count;
	FVector grid_size_vector = to_cell_count.size;
	int grid_array_total_cells = grid_size_vector.X * grid_size_vector.Y * grid_size_vector.Z;
	
	for(int i = 0; i <  grid_array_total_cells; i ++)
	{
		FModelCell cell = FModelCell(all_possible_rulesets);
		grid_.Add(cell);
	}
}

bool UGenerationModelImpl::CollapseTile(FGridArrayIndexCoordinate tile_index)
{
	int flattened_tile_index = INCCoordinateSpaceConverter::FlattenGridArrayIndexCoordinate(tile_index, grid_size_);
	FModelCell& cell = grid_[flattened_tile_index];
	TArray<UGenerationRuleset*>& candidates = cell.CandidateRuleSets;

	if(candidates.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No candidates found in cell, cannot collapse"))
		return false;
	}
	int random_index = rand() % candidates.Num();
	UGenerationRuleset* selected_ruleset = candidates[random_index];
	candidates.Empty();
	candidates.Add(selected_ruleset);
	
	return true;
}

bool UGenerationModelImpl::PropagateToNeighbour(FGridArrayIndexCoordinate tile_index, int neighbour_index)
{
	int flattened_tile_index = INCCoordinateSpaceConverter::FlattenGridArrayIndexCoordinate(tile_index, grid_size_);
	TArray<UGenerationRuleset*>& rulesets = grid_[flattened_tile_index].CandidateRuleSets;
	if(neighbour_index >= static_cast<int>(EAdjacency::LAST))
		return false;
	
	EAdjacency current_adjacency = static_cast<EAdjacency>(neighbour_index);
	FString string_adjacency = UEnum::GetValueAsString(current_adjacency);
	FGridArrayIndexCoordinate adjacent_index = tile_index.vector + PUrban::ToVector(current_adjacency);
	if(INCModelUtilities::OutOfBounds(adjacent_index, grid_size_))
		return false;
	int flattened_adjacency_index = INCCoordinateSpaceConverter::FlattenGridArrayIndexCoordinate(adjacent_index, grid_size_);
	TArray<UGenerationRuleset*>& rulesets_at_adjacency = grid_[flattened_adjacency_index].CandidateRuleSets;

	if(!UGenerationRuleset::RemoveInconsistentLabels(rulesets, rulesets_at_adjacency, current_adjacency))
		return false;

	return true;
}