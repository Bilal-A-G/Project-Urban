#include "Misc/AutomationTest.h"
#include "ProceduralGeneration/CoordinateSpaces/F_Grid_Array_Index_Coordinate.h"
#include "ProceduralGeneration/CoordinateSpaces/F_Grid_Local_Coordinate.h"
#include "ProceduralGeneration/CoordinateSpaces/INC_Coordinate_Space_Converter.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GridArrayIndexToLocal, "Project_Urban.CoordinateSpaceConverter.GridArrayIndexToLocal",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::CriticalPriority | EAutomationTestFlags::ProductFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GridLocalToArrayIndex, "Project_Urban.CoordinateSpaceConverter.GridLocalToArrayIndex",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::CriticalPriority | EAutomationTestFlags::ProductFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GridArrayIndexFlatten, "Project_Urban.CoordinateSpaceConverter.GridArrayIndexFlatten",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::CriticalPriority | EAutomationTestFlags::ProductFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GridArrayIndexUnFlatten, "Project_Urban.CoordinateSpaceConverter.GridArrayIndexUnFlatten",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::CriticalPriority | EAutomationTestFlags::ProductFilter)

bool GridArrayIndexFlatten::RunTest(const FString& parameters)
{
	FGridCellCountSize grid_size = FGridCellCountSize(FVector(2,1,4));
	auto CheckConversion = [&](const FGridArrayIndexCoordinate& coordinate, const FGridCellCountSize& grid_size, const int& expected) -> bool
	{
		int result = INCCoordinateSpaceConverter::FlattenGridArrayIndexCoordinate(coordinate, grid_size);
		if(result != expected)
		{
			UE_LOG(LogTemp, Error, TEXT("Conversion mismatch! Input=(%s), Expected=(%i), Got=(%i)"), *coordinate.vector.ToString(), expected, result);
			return false;
		}
		
		return true;
	};

	if(!CheckConversion(FGridArrayIndexCoordinate(FVector(0,0,0)), grid_size, 0))
		return false;
	if(!CheckConversion(FGridArrayIndexCoordinate(FVector(1,0,3)), grid_size, 7))
		return false;
	if(!CheckConversion(FGridArrayIndexCoordinate(FVector(1,0,1)), grid_size, 3))
		return false;

	return true;
}
bool GridArrayIndexUnFlatten::RunTest(const FString& parameters)
{
	auto CheckConversion = [&](const int& flattened_index, const FGridCellCountSize& grid_size, const FVector& expected) -> bool
	{
		FGridArrayIndexCoordinate result = INCCoordinateSpaceConverter::UnFlattenToGridArrayIndexCoordinate(flattened_index, grid_size);
		if(!result.vector.Equals(expected, KINDA_SMALL_NUMBER))
		{
			UE_LOG(LogTemp, Error, TEXT("Conversion mismatch! Input=(%i), Expected=(%s), Got=(%s)"),flattened_index, *expected.ToString(), *result.vector.ToString());
			return false;
		}
		
		return true;
	};

	FGridCellCountSize grid_size = FGridCellCountSize(FVector(3,2,6));
	
	if(!CheckConversion(0, grid_size, FVector(0,0,0)))
		return false;
	if(!CheckConversion(35, grid_size, FVector(2,1,5)))
		return false;
	if(!CheckConversion(22, grid_size, FVector(1,1,3)))
		return false;

	return true;
}

bool GridArrayIndexToLocal::RunTest(const FString& parameters)
{
	// Grid extents (max local values: -x->x, -y->y, 0->z)
	FGridExtentsSize grid_extents(FVector(10, 20, 30));

	auto CheckConversion = [&](const FGridArrayIndexCoordinate& array_index, const FVector& expected) -> bool
	{
		FGridLocalCoordinate converted = INCCoordinateSpaceConverter::GridArrayIndexToLocal(array_index, grid_extents);
		FVector converted_vector = converted.vector;

		if (!converted_vector.Equals(expected, KINDA_SMALL_NUMBER))
		{
			UE_LOG(LogTemp, Error, TEXT("Conversion mismatch! Input=(%s), Expected=(%s), Got=(%s)"),
				*array_index.vector.ToString(),
				*expected.ToString(),
				*converted_vector.ToString());
			return false;
		}
		return true;
	};

	// --- Corner cases ---
	// Lower bounds (array 0,0,0 → local -x,-y,0)
	if (!CheckConversion(FGridArrayIndexCoordinate(FVector(0, 0, 0)), FVector(-10, -20, 0)))
		return false;

	// Upper bounds (array 2x,2y,z → local +x,+y,z)
	if (!CheckConversion(FGridArrayIndexCoordinate(FVector(20, 40, 30)), FVector(10, 20, 30)))
		return false;

	// Center point (array x,y,z → local 0,0,z)
	if (!CheckConversion(FGridArrayIndexCoordinate(FVector(10, 20, 15)), FVector(0, 0, 15)))
		return false;

	// Mixed extremes
	if (!CheckConversion(FGridArrayIndexCoordinate(FVector(0, 40, 10)), FVector(-10, 20, 10)))
		return false;
	if (!CheckConversion(FGridArrayIndexCoordinate(FVector(20, 0, 30)), FVector(10, -20, 30)))
		return false;

	// Random interior
	if (!CheckConversion(FGridArrayIndexCoordinate(FVector(13, 7, 12)), FVector(3, -13, 12)))
		return false;

	return true;	
}

bool GridLocalToArrayIndex::RunTest(const FString& parameters)
{
	// Grid extents (max local values: -x->x, -y->y, 0->z)
	FGridExtentsSize grid_extents(FVector(10, 20, 30));

	auto CheckConversion = [&](const FGridLocalCoordinate& local_coordinate, const FVector& expected) -> bool
	{
		FGridArrayIndexCoordinate converted = INCCoordinateSpaceConverter::GridLocalToArrayIndex(local_coordinate, grid_extents);
		FVector converted_vector = converted.vector;

		if (!converted_vector.Equals(expected, KINDA_SMALL_NUMBER))
		{
			UE_LOG(LogTemp, Error, TEXT("Conversion mismatch! Input=(%s), Expected=(%s), Got=(%s)"),
				*local_coordinate.vector.ToString(),
				*expected.ToString(),
				*converted_vector.ToString());
			return false;
		}
		return true;
	};

	// --- Corner cases ---
	// Lower bounds (local -x,-y,0 → array 0,0,0)
	if (!CheckConversion(FGridLocalCoordinate(FVector(-10, -20, 0)), FVector(0, 0, 0)))
		return false;

	// Upper bounds (local +x,+y,z → array 2x,2y,z)
	if (!CheckConversion(FGridLocalCoordinate(FVector(10, 20, 30)), FVector(20, 40, 30)))
		return false;

	// Center point (local 0,0,z → array x,y,z)
	if (!CheckConversion(FGridLocalCoordinate(FVector(0, 0, 15)), FVector(10, 20, 15)))
		return false;

	// Mixed extremes
	if (!CheckConversion(FGridLocalCoordinate(FVector(-10, 20, 10)), FVector(0, 40, 10)))
		return false;
	if (!CheckConversion(FGridLocalCoordinate(FVector(10, -20, 30)), FVector(20, 0, 30)))
		return false;

	// Random interior
	if (!CheckConversion(FGridLocalCoordinate(FVector(3, -13, 12)), FVector(13, 7, 12)))
		return false;

	return true;	
}
