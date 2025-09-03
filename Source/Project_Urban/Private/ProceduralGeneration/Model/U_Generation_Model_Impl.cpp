#include "ProceduralGeneration/Model/U_Generation_Model_Impl.h"
#include "ProceduralGeneration/CoordinateSpaces/INC_Coordinate_Space_Converter.h"
#include "ProceduralGeneration/CoordinateSpaces/Sizing/INC_Size_Type_Converter.h"
#include "ProceduralGeneration/Model/Utility/INC_Model_Utilities.h"

void UGenerationModelImpl::Initialize(FGridExtentsSize grid_size, TArray<UGenerationRuleset*>& all_possible_rulesets, bool add_to_valid_collapse)
{
	grid_.Empty();
	FGridCellCountSize to_cell_count = INCSizeTypeConverter::GridExtentsToCellCount(grid_size);
	grid_size_ = to_cell_count;
	FVector grid_size_vector = to_cell_count.size;
	int grid_array_total_cells = grid_size_vector.X * grid_size_vector.Y * grid_size_vector.Z;
	
	for(int i = 0; i <  grid_array_total_cells; i ++)
	{
		FModelCell cell = FModelCell(all_possible_rulesets);
		FGridArrayIndexCoordinate un_flattened = INCCoordinateSpaceConverter::UnFlattenToGridArrayIndexCoordinate(i, to_cell_count);
		valid_collapse_indices_.Add(un_flattened);
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

bool UGenerationModelImpl::MakeNeighbourPossibilitiesConsistent(FGridArrayIndexCoordinate starting_index, EAdjacency to_neighbour_adjacency)
{
	int flattened_tile_index = INCCoordinateSpaceConverter::FlattenGridArrayIndexCoordinate(starting_index, grid_size_);
	TArray<UGenerationRuleset*>& rulesets = grid_[flattened_tile_index].CandidateRuleSets;
	if(to_neighbour_adjacency == EAdjacency::LAST || to_neighbour_adjacency == EAdjacency::INVALID)
		return false;
	
	FString string_adjacency = UEnum::GetValueAsString(to_neighbour_adjacency);
	FGridArrayIndexCoordinate adjacent_index = starting_index.vector + PUrban::ToVector(to_neighbour_adjacency);
	if(INCModelUtilities::OutOfBounds(adjacent_index, grid_size_))
		return false;
	int flattened_adjacency_index = INCCoordinateSpaceConverter::FlattenGridArrayIndexCoordinate(adjacent_index, grid_size_);
	TArray<UGenerationRuleset*>& rulesets_at_adjacency = grid_[flattened_adjacency_index].CandidateRuleSets;

	if(!UGenerationRuleset::RemoveInconsistentLabels(rulesets, rulesets_at_adjacency, to_neighbour_adjacency))
		return false;

	return true;
}

bool UGenerationModelImpl::MakeAllNeighboursPossibilitiesConsistent(FGridArrayIndexCoordinate starting_index)
{
	int last_adjacency = static_cast<int>(EAdjacency::LAST);
	bool success = false;
	for(int i = 0; i < last_adjacency; i++)
	{
		EAdjacency current_adjacency = static_cast<EAdjacency>(i);
		bool neighbour_success = MakeNeighbourPossibilitiesConsistent(starting_index, current_adjacency);
		if(neighbour_success)
			success = true;
	}
	return success;
}

void UGenerationModelImpl::MakeGridPossibilitiesConsistent(FGridArrayIndexCoordinate starting_index)
{
	bool propagate_success = MakeAllNeighboursPossibilitiesConsistent(starting_index);
	if (!propagate_success)
		return;

	int last_adjacency = static_cast<int>(EAdjacency::LAST);
	for (int i = 0; i < last_adjacency; i++)
	{
		EAdjacency adjacency = static_cast<EAdjacency>(i);
		FVector raw_adjacency_vector = FVector(starting_index.vector + PUrban::ToVector(adjacency));
		FGridArrayIndexCoordinate adjacent_index = FGridArrayIndexCoordinate(raw_adjacency_vector);
		if (INCModelUtilities::OutOfBounds(adjacent_index,grid_size_))
			continue;
		
		MakeGridPossibilitiesConsistent(adjacent_index);	
	}
}

bool UGenerationModelImpl::CollapseBlock(FGridArrayIndexCoordinate bottom_left_index, FGridArrayIndexCoordinate block_size)
{
	bool reset_success = ResetGridPossibilitiesInRegion(bottom_left_index, block_size);
	if(!reset_success)
		return false;
	
	return true;
}

bool UGenerationModelImpl::ResetGridPossibilitiesInRegion(FGridArrayIndexCoordinate bottom_left_index, FGridArrayIndexCoordinate region_size)
{
	int bottom_left_index_flattened = INCCoordinateSpaceConverter::FlattenGridArrayIndexCoordinate(bottom_left_index, grid_size_);
	FGridArrayIndexCoordinate top_right_index = FGridArrayIndexCoordinate(FVector(bottom_left_index.vector + region_size.vector));
	if(INCModelUtilities::OutOfBounds(top_right_index, grid_size_))
		return false;
	int top_right_index_flattened = INCCoordinateSpaceConverter::FlattenGridArrayIndexCoordinate(top_right_index, grid_size_);
	
	for(int i = bottom_left_index_flattened; i < top_right_index_flattened; i++)
	{
		grid_[i].CandidateRuleSets = all_possible_rulesets_;
	}
	
	return true;
}

TTuple<bool, FGridArrayIndexCoordinate> UGenerationModelImpl::CollapseRandomValidTile()
{
	TTuple<bool, FGridArrayIndexCoordinate> collapsed_information = TTuple<bool, FGridArrayIndexCoordinate>();
	if(valid_collapse_indices_.Num() == 0)
		return collapsed_information;
	int random_index = rand() % valid_collapse_indices_.Num();
	FGridArrayIndexCoordinate random_array_index = valid_collapse_indices_[random_index];
	collapsed_information.Value = random_array_index;
	bool success = CollapseTile(random_array_index);
	if(success)
		valid_collapse_indices_.RemoveAt(random_index);
	collapsed_information.Key = success;
	return collapsed_information;
}
