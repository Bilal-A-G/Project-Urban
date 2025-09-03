#include "Misc/AutomationTest.h"
#include "ProceduralGeneration/ULabel.h"
#include "ProceduralGeneration/CoordinateSpaces/INC_Coordinate_Space_Converter.h"
#include "ProceduralGeneration/CoordinateSpaces/Sizing/F_Grid_Extents_Size.h"
#include "ProceduralGeneration/CoordinateSpaces/Sizing/INC_Size_Type_Converter.h"
#include "ProceduralGeneration/Model/U_Generation_Model_Impl.h"
#include "ProceduralGeneration/Model/Utility/INC_Model_Utilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GridSize, "Project_Urban.ModelImpl.GridSize",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::CriticalPriority | EAutomationTestFlags::ProductFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GridInitialState, "Project_Urban.ModelImpl.GridInitialState",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::CriticalPriority | EAutomationTestFlags::ProductFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GridCollapseTile, "Project_Urban.ModelImpl.GridCollapseTile",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::CriticalPriority | EAutomationTestFlags::ProductFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GridMakeNeighbourPossibilitiesConsistent, "Project_Urban.ModelImpl.GridMakeNeighbourPossibilitiesConsistent",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::CriticalPriority | EAutomationTestFlags::ProductFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GridMakeAllNeighboursPossibilitiesConsistent, "Project_Urban.ModelImpl.GridMakeAllNeighbourPossibilitesConsistent",
									EAutomationTestFlags::EditorContext | EAutomationTestFlags::CriticalPriority | EAutomationTestFlags::ProductFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GridMakeGridPossibilitiesConsistent, "Project_Urban.ModelImpl.GridMakeGridPossibilitiesConsistent",
									EAutomationTestFlags::EditorContext | EAutomationTestFlags::CriticalPriority | EAutomationTestFlags::ProductFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GridCollapseRandomTile, "Project_Urban.ModelImpl.GridCollapseRandomTile",
									EAutomationTestFlags::EditorContext | EAutomationTestFlags::CriticalPriority | EAutomationTestFlags::ProductFilter)

UGenerationModelImpl* SetupTestModel(FGridExtentsSize& size, TArray<UGenerationRuleset*> all_possible_rulesets)
{
	UGenerationModelImpl* created_model = NewObject<UGenerationModelImpl>();
	created_model->Initialize(size, all_possible_rulesets);
	return created_model;
}
TArray<UGenerationRuleset*> SetupMockRulesetData()
{
	TArray<UGenerationRuleset*> mock_data = TArray<UGenerationRuleset*>();
	
	ULabel* square_label = NewObject<ULabel>();
	UStaticMesh* square_mesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(),nullptr,
		TEXT("/Game/LevelPrototyping/Meshes/SM_Cube.SM_Cube")));
	square_label->Mesh = square_mesh;
	square_label->Rotation = FQuat::Identity;
	square_label->Scale = FVector(1,1,1);
	
	ULabel* cylinder_label = NewObject<ULabel>();
	UStaticMesh* cylinder_mesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(),nullptr,
		TEXT("/Game/LevelPrototyping/Meshes/SM_Cylinder.SM_Cylinder")));
	cylinder_label->Mesh = cylinder_mesh;
	cylinder_label->Rotation = FQuat::Identity;
	cylinder_label->Scale = FVector(1,1,1);
	
	UGenerationRuleset* square = NewObject<UGenerationRuleset>();
	square->Current = square_label;
	square->AddAdjacencyEntry(EAdjacency::RIGHT, cylinder_label);
	square->AddAdjacencyEntry(EAdjacency::LEFT, cylinder_label);
	square->AddAdjacencyEntry(EAdjacency::FORWARD, square_label);
	square->AddAdjacencyEntry(EAdjacency::BACKWARD, square_label);
	
	UGenerationRuleset* cylinder = NewObject<UGenerationRuleset>();
	cylinder->Current = cylinder_label;
	cylinder->AddAdjacencyEntry(EAdjacency::FORWARD, cylinder_label);
	cylinder->AddAdjacencyEntry(EAdjacency::BACKWARD, cylinder_label);
	cylinder->AddAdjacencyEntry(EAdjacency::LEFT, square_label);
	cylinder->AddAdjacencyEntry(EAdjacency::RIGHT, square_label);

	mock_data.Append({square, cylinder});
	return mock_data;
}

bool GridSize::RunTest(const FString& Parameters)
{
	FGridExtentsSize given_size = FVector(1,1,0);
	int expected_size = 9;
	UGenerationModelImpl* model_impl = SetupTestModel(given_size, {});
	TArray<FModelCell> generated_grid = model_impl->D_GetGrid();
	if(generated_grid.Num() != expected_size)
	{
		UE_LOG(LogTemp, Error, TEXT("Grid size mismatch! Expected={%i}, Got={%i}"), expected_size, generated_grid.Num())
		return false;
	}
	return true;
}

bool GridInitialState::RunTest(const FString& Parameters)
{
	FGridExtentsSize given_size = FVector(1,1,0);
	TArray<UGenerationRuleset*> given_rulesets = SetupMockRulesetData();
	UGenerationModelImpl* model_impl = SetupTestModel(given_size,given_rulesets);
	TArray<FModelCell> generated_grid = model_impl->D_GetGrid();
	for(int i = 0; i < generated_grid.Num(); i++)
	{
		FModelCell current_cell = generated_grid[i];
		TArray<UGenerationRuleset*> candidate_rulesets = current_cell.CandidateRuleSets;
		if(candidate_rulesets.Num() != given_rulesets.Num())
		{
			UE_LOG(LogTemp, Error, TEXT("Grid candidate ruleset count mismatch! Expected={%i}, Got={%i}"), given_rulesets.Num(), candidate_rulesets.Num())
			return false;
		}
		int random_index = rand() % candidate_rulesets.Num();
		if(candidate_rulesets[random_index] != given_rulesets[random_index])
		{
			UE_LOG(LogTemp, Error,TEXT("Grid element {%i} mismatch! No additional data available"), random_index)
			return false;
		}
	}
	
	return true;
}

bool GridCollapseTile::RunTest(const FString& Parameters)
{
	TArray<UGenerationRuleset*> given_ruleset = SetupMockRulesetData();
	FGridExtentsSize given_size = FGridExtentsSize(FVector(1,1,0));
	UGenerationModelImpl* model_impl = NewObject<UGenerationModelImpl>();
	model_impl->Initialize(given_size, given_ruleset);
	FGridArrayIndexCoordinate collapse_index = FGridArrayIndexCoordinate(FVector(0,0,0));
	bool success = model_impl->CollapseTile(collapse_index);
	if(!success)
	{
		UE_LOG(LogTemp, Error, TEXT("Grid collapse error, could not collapse cell at index {%s}"), *collapse_index.vector.ToString());
		return false;
	}
	TArray<FModelCell> grid = model_impl->D_GetGrid();
	FGridCellCountSize size_to_cell_count = INCSizeTypeConverter::GridExtentsToCellCount(given_size);
	int flattened_index = INCCoordinateSpaceConverter::FlattenGridArrayIndexCoordinate(collapse_index, size_to_cell_count);
	FModelCell collapsed_cell = grid[flattened_index];
	TArray<UGenerationRuleset*> candidate_rulesets = collapsed_cell.CandidateRuleSets;
	if(candidate_rulesets.Num() > 1)
	{
		UE_LOG(LogTemp, Error, TEXT("Model collapse failed! Expected number of rulesets after collapse = {%i}, got {%i}"), 1, candidate_rulesets.Num())
		return false;
	}
	for(int i = 0; i < candidate_rulesets.Num(); i++)
	{
		UGenerationRuleset* current_candidate = candidate_rulesets[i];
		bool contains = false;
		for(int v = 0; v < given_ruleset.Num(); v++)
		{
			UGenerationRuleset* current_given = given_ruleset[v];
			if(current_candidate == current_given)
			{
				contains = true;
				break;
			}
		}
		if(!contains)
		{
			UE_LOG(LogTemp, Error, TEXT("Model collapse error! Candidate in ruleset after collapse is not found in given ruleset"))
			return false;
		}
	}
	return true;
}

bool VerifyCandidateRulesetsConsistentAroundIndex(FGridExtentsSize given_size, FGridArrayIndexCoordinate check_at_index,
	UGenerationModelImpl* model_impl, EAdjacency neighbour_adjacency = EAdjacency::INVALID)
{
	FGridCellCountSize extents_to_cell_count = INCSizeTypeConverter::GridExtentsToCellCount(given_size);
	int flattened_check_at_index = INCCoordinateSpaceConverter::FlattenGridArrayIndexCoordinate(check_at_index, extents_to_cell_count);
	int last_adjacency = static_cast<int>(EAdjacency::LAST);
	for(int i = 0; i < last_adjacency; i++)
	{
		EAdjacency current_neighbour_adjacency = static_cast<EAdjacency>(i);
		if((neighbour_adjacency != EAdjacency::INVALID && neighbour_adjacency != EAdjacency::LAST) && current_neighbour_adjacency != neighbour_adjacency)
			continue;
		FGridArrayIndexCoordinate neighbour_index = FGridArrayIndexCoordinate(
			FVector(check_at_index.vector + PUrban::ToVector(current_neighbour_adjacency)));
		if(INCModelUtilities::OutOfBounds(neighbour_index, extents_to_cell_count))
			continue;
		int flattened_neighbour_index = INCCoordinateSpaceConverter::FlattenGridArrayIndexCoordinate(
			neighbour_index, extents_to_cell_count);
		FModelCell collapsed_cell = model_impl->D_GetGrid()[flattened_check_at_index];
		FModelCell neighbour_cell = model_impl->D_GetGrid()[flattened_neighbour_index];
		UGenerationRuleset* ruleset_after_collapse = collapsed_cell.CandidateRuleSets[0];
		TArray<UGenerationRuleset*> rulesets_at_neighbour_after_collapse = neighbour_cell.CandidateRuleSets;

		TArray<ULabel*> rulesets_at_adjacency = ruleset_after_collapse->GetAdjacencyValuesFromKey(current_neighbour_adjacency);
		if (rulesets_at_neighbour_after_collapse.Num() != rulesets_at_adjacency.Num())
		{
			UE_LOG(LogTemp, Error,
			       TEXT("Model propagate error! Expected number of rulesets at adjacency = {%i} at neighbour = {%s}, Got = {%i}"),
			       rulesets_at_adjacency.Num(), *UEnum::GetValueAsString(current_neighbour_adjacency), rulesets_at_neighbour_after_collapse.Num())
			return false;
		}
		for (int v = 0; v < rulesets_at_neighbour_after_collapse.Num(); v++)
		{
			UGenerationRuleset* current_neighbour_ruleset = rulesets_at_neighbour_after_collapse[v];
			bool contains = false;
			for (int k = 0; k < rulesets_at_adjacency.Num(); k++)
			{
				ULabel* current_desired_label = rulesets_at_adjacency[k];
				if (current_desired_label == current_neighbour_ruleset->Current)
				{
					contains = true;
					break;
				}
			}
			if (!contains)
			{
				UE_LOG(LogTemp, Error,
				       TEXT("Model propagate error! Ruleset found at adjacency at neighbour = {%s} that does not match allowed rulesets"),
				       *UEnum::GetValueAsString(current_neighbour_adjacency))
				return false;
			}
		}		
	}
		
	return true;	
}

bool GridMakeNeighbourPossibilitiesConsistent::RunTest(const FString& Parameters)
{
	TArray<UGenerationRuleset*> given_ruleset = SetupMockRulesetData();
	FGridExtentsSize given_size = FGridExtentsSize(FVector(1,1,0));
	UGenerationModelImpl* model_impl = SetupTestModel(given_size, given_ruleset);
	FGridArrayIndexCoordinate propagate_index = FGridArrayIndexCoordinate(FVector(0,0,0));
	model_impl->CollapseTile(propagate_index);
	EAdjacency neighbour_adjacency = EAdjacency::RIGHT;
	bool success = model_impl->MakeNeighbourPossibilitiesConsistent(propagate_index, neighbour_adjacency);
	if(!success)
	{
		UE_LOG(LogTemp, Error, TEXT("Model propagate error! Failed to propagate to neighbour {%s} at starting index {%s}"),
			*UEnum::GetValueAsString(neighbour_adjacency), *propagate_index.vector.ToString())
		return false;
	}
	if(!VerifyCandidateRulesetsConsistentAroundIndex(given_size, propagate_index, model_impl, neighbour_adjacency))
		return false;
	
	return true;
}

bool GridMakeAllNeighboursPossibilitiesConsistent::RunTest(const FString& parameters)
{
	TArray<UGenerationRuleset*> given_ruleset = SetupMockRulesetData();
	FGridExtentsSize given_size = FGridExtentsSize(FVector(1, 1, 0));
	UGenerationModelImpl* model_impl = SetupTestModel(given_size, given_ruleset);
	FGridArrayIndexCoordinate propagate_index = FGridArrayIndexCoordinate(FVector(0, 0, 0));
	model_impl->CollapseTile(propagate_index);
	bool success = model_impl->MakeAllNeighboursPossibilitiesConsistent(propagate_index);
	if (!success)
	{
		UE_LOG(LogTemp, Error, TEXT("Model propagate error! Failed to propagate to all neighbours at starting index {%s}"),
			*propagate_index.vector.ToString())
		return false;
	}
	if (!VerifyCandidateRulesetsConsistentAroundIndex(given_size, propagate_index, model_impl))
		return false;
	
	return true;	
}

bool GridMakeGridPossibilitiesConsistent::RunTest(const FString& parameters)
{
	TArray<UGenerationRuleset*> given_ruleset = SetupMockRulesetData();
	FGridExtentsSize given_size = FGridExtentsSize(FVector(1, 1, 0));
	UGenerationModelImpl* model_impl = SetupTestModel(given_size, given_ruleset);
	FGridArrayIndexCoordinate propagate_index = FGridArrayIndexCoordinate(FVector(0, 0, 0));
	model_impl->CollapseTile(propagate_index);
	model_impl->MakeGridPossibilitiesConsistent(propagate_index);
	FGridCellCountSize to_cell_count_size = INCSizeTypeConverter::GridExtentsToCellCount(given_size);
	for(int i = 0; i < model_impl->D_GetGrid().Num(); i++)
	{
		FGridArrayIndexCoordinate current_index = INCCoordinateSpaceConverter::UnFlattenToGridArrayIndexCoordinate(i, to_cell_count_size);
		if(!VerifyCandidateRulesetsConsistentAroundIndex(given_size, current_index, model_impl))
			return false;
	}	

	return true;	
}

bool GridCollapseRandomTile::RunTest(const FString& parameters)
{
	TArray<UGenerationRuleset*> given_ruleset = SetupMockRulesetData();
	FGridExtentsSize given_size = FGridExtentsSize(FVector(1, 1, 0));
	UGenerationModelImpl* model_impl = NewObject<UGenerationModelImpl>();
	model_impl->Initialize(given_size, given_ruleset, true);
	TTuple<bool, FGridArrayIndexCoordinate> result = model_impl->CollapseRandomValidTile();
	if (!result.Key)
	{
		UE_LOG(LogTemp, Error, TEXT("Grid collapse error, could not collapse cell at index {%s}"),
		       *result.Value.vector.ToString());
		return false;
	}
	TArray<FModelCell> grid = model_impl->D_GetGrid();
	FGridCellCountSize size_to_cell_count = INCSizeTypeConverter::GridExtentsToCellCount(given_size);
	int flattened_index = INCCoordinateSpaceConverter::FlattenGridArrayIndexCoordinate(
		result.Value, size_to_cell_count);
	FModelCell collapsed_cell = grid[flattened_index];
	TArray<UGenerationRuleset*> candidate_rulesets = collapsed_cell.CandidateRuleSets;
	if (candidate_rulesets.Num() > 1)
	{
		UE_LOG(LogTemp, Error,
		       TEXT("Model collapse failed! Expected number of rulesets after collapse = {%i}, got {%i}"), 1,
		       candidate_rulesets.Num())
		return false;
	}
	for (int i = 0; i < candidate_rulesets.Num(); i++)
	{
		UGenerationRuleset* current_candidate = candidate_rulesets[i];
		bool contains = false;
		for (int v = 0; v < given_ruleset.Num(); v++)
		{
			UGenerationRuleset* current_given = given_ruleset[v];
			if (current_candidate == current_given)
			{
				contains = true;
				break;
			}
		}
		if (!contains)
		{
			UE_LOG(LogTemp, Error,
			       TEXT("Model collapse error! Candidate in ruleset after collapse is not found in given ruleset"))
			return false;
		}
	}
	return true;
	
}
