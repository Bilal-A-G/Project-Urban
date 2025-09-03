#include "Misc/AutomationTest.h"
#include "ProceduralGeneration/CoordinateSpaces/F_Grid_Array_Index_Coordinate.h"
#include "ProceduralGeneration/CoordinateSpaces/Sizing/F_Grid_Cell_Count_Size.h"
#include "ProceduralGeneration/Model/Utility/INC_Model_Utilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GridOutOfBounds, "Project_Urban.ModelUtilities.GridOutOfBounds",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::CriticalPriority | EAutomationTestFlags::ProductFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GridGetBorderTileIndices, "Project_Urban.ModelUtilities.GridGetBorderTileIndices",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::CriticalPriority | EAutomationTestFlags::ProductFilter)

bool CheckCase(FGridArrayIndexCoordinate index, bool expected, const FGridCellCountSize& grid_size)
{
	bool result = INCModelUtilities::OutOfBounds(index, grid_size);
	if (result != expected)
	{
		UE_LOG(LogTemp, Error,
			TEXT("OutOfBounds mismatch! Index = %s, Size = %s, Expected = %s, Got = %s"),
			*index.vector.ToString(),
			*grid_size.size.ToString(),
			expected ? TEXT("true") : TEXT("false"),
			result ? TEXT("true") : TEXT("false"));
		return false;
	}
	return true;
}

bool GridOutOfBounds::RunTest(const FString& parameters)
{
	FGridCellCountSize grid_size = FGridCellCountSize(FVector(2,2,1));

	if (!CheckCase(FGridArrayIndexCoordinate(FVector(0, 0, 0)), false, grid_size)) return false;

	if (!CheckCase(FGridArrayIndexCoordinate(FVector(-1, 0, 0)), true, grid_size)) return false;
	if (!CheckCase(FGridArrayIndexCoordinate(FVector(3, 0, 0)), true, grid_size)) return false;

	if (!CheckCase(FGridArrayIndexCoordinate(FVector(0, -1, 0)), true, grid_size)) return false;
	if (!CheckCase(FGridArrayIndexCoordinate(FVector(0, 2, 0)), true, grid_size)) return false;

	if (!CheckCase(FGridArrayIndexCoordinate(FVector(0, 0, -1)), true, grid_size)) return false;
	if (!CheckCase(FGridArrayIndexCoordinate(FVector(0, 0, 1)), true, grid_size)) return false;
	
	return true;
}

bool GridGetBorderTileIndices::RunTest(const FString& parameters)
{
	FGridCellCountSize grid_size = FGridCellCountSize(FVector(3,3,1));
	TArray<FGridArrayIndexCoordinate> expected_border_cells =
	{
		FGridArrayIndexCoordinate(FVector(0,0,0)),
		FGridArrayIndexCoordinate(FVector(1,0,0)),
		FGridArrayIndexCoordinate(FVector(2,0,0)),

		FGridArrayIndexCoordinate(FVector(0,1,0)),
		FGridArrayIndexCoordinate(FVector(0,2,0)),
		
		FGridArrayIndexCoordinate(FVector(2,1,0)),
		FGridArrayIndexCoordinate(FVector(2,2,0)),
		FGridArrayIndexCoordinate(FVector(1,2,0)),
	};
	TArray<FGridArrayIndexCoordinate> border_cells = INCModelUtilities::GetBorderTileIndices(grid_size, true);
	if(border_cells.Num() != expected_border_cells.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Expected border tiles count mismatch! Expected = {%i}, got = {%i}"), expected_border_cells.Num(), border_cells.Num());
		return false;
	}
	for (int i = 0; i < border_cells.Num(); i++)
	{
		FGridArrayIndexCoordinate current_cell = border_cells[i];
		bool found = false;
		for(int v = 0; v < expected_border_cells.Num(); v++)
		{
			FGridArrayIndexCoordinate expected_border_current = expected_border_cells[v];
			if(expected_border_current.vector.Equals(current_cell.vector, KINDA_SMALL_NUMBER))
			{
				found = true;
				break;
			}
		}
		if(!found)
		{
			UE_LOG(LogTemp, Error, TEXT("Expected border tiles mismatch! tile with index = {%s}, cannot be found in the expected result"),
				*current_cell.vector.ToString())
			return false;
		}
	}

	return true;
}
