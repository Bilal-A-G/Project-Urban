#include "Misc/AutomationTest.h"
#include "ProceduralGeneration/CoordinateSpaces/Sizing/F_Grid_Cell_Count_Size.h"
#include "ProceduralGeneration/CoordinateSpaces/Sizing/F_Grid_Extents_Size.h"
#include "ProceduralGeneration/CoordinateSpaces/Sizing/INC_Size_Type_Converter.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GridExtentsToCellCount, "Project_Urban.SizeTypeConverter.GridExtentsToCellCount",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GridCellCountToExtents, "Project_Urban.SizeTypeConverter.GridCellCountToExtents",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority)

bool GridCellCountToExtents::RunTest(const FString& Parameters)
{
	auto CheckConversion = [&](const FGridCellCountSize& cell_count_size, const FGridExtentsSize& expected)
	{
		FGridExtentsSize result = INCSizeTypeConverter::GridCellCountToExtents(cell_count_size);
		if(!result.size.Equals(expected.size, KINDA_SMALL_NUMBER))
		{
			UE_LOG(LogTemp, Error, TEXT("Conversion mismatch! Input=(%s), Expected=(%s), Got=(%s)"), *cell_count_size.size.ToString(),
				*expected.size.ToString(), *result.size.ToString())
			return false;
		}
		return true;
	};

	if(!CheckConversion(FGridCellCountSize(FVector(1,5,3)), FGridExtentsSize(FVector(0,2,2))))
		return false;
	if(!CheckConversion(FGridCellCountSize(FVector(1,1,1)), FGridExtentsSize(FVector(0,0,0))))
		return false;
	if(!CheckConversion(FGridCellCountSize(FVector(5,7,1)), FGridExtentsSize(FVector(2,3,0))))
		return false;
	if(!CheckConversion(FGridCellCountSize(FVector(7,1,5)), FGridExtentsSize(FVector(3,0,4))))
		return false;
	
	return true;
}

bool GridExtentsToCellCount::RunTest(const FString& parameters)
{
	auto CheckConversion = [&](const FGridExtentsSize& extents_size, const FGridCellCountSize& expected)
	{
		FGridCellCountSize result = INCSizeTypeConverter::GridExtentsToCellCount(extents_size);
		if(!result.size.Equals(expected.size, KINDA_SMALL_NUMBER))
		{
			UE_LOG(LogTemp, Error, TEXT("Conversion mismatch! Input=(%s), Expected=(%s), Got=(%s)"), *extents_size.size.ToString(),
				*expected.size.ToString(), *result.size.ToString())
			return false;
		}
		return true;
	};
	
	if(!CheckConversion(FGridExtentsSize(FVector(0,2,2)), FGridCellCountSize(FVector(1,5,3))))
		return false;
	if(!CheckConversion(FGridExtentsSize(FVector(0,0,0)), FGridCellCountSize(FVector(1,1,1))))
		return false;
	if(!CheckConversion(FGridExtentsSize(FVector(2,3,0)), FGridCellCountSize(FVector(5,7,1))))
		return false;
	if(!CheckConversion(FGridExtentsSize(FVector(3,0,4)), FGridCellCountSize(FVector(7,1,5))))
		return false;
	
	return true;
}
