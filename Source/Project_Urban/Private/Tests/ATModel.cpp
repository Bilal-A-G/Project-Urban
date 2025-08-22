
#include "Misc/AutomationTest.h"
#include "ProceduralGeneration/UGenerationModel.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GridSize, "Project_Urban.Model.GridSize",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::CriticalPriority | EAutomationTestFlags::ProductFilter)

//Check if grid is created with the correct dimensions 
//Given a size of (1,1,0), we have to create a (3,3,1) grid
bool GridSize::RunTest(const FString& Parameters)
{
	UGenerationModel* model = NewObject<UGenerationModel>();
	TArray<UGenerationRuleset*> allRulesets;
	FVector givenSize = FVector(1,1,0);
	FVector expectedSize = FVector(givenSize.X * 2 + 1, givenSize.Y * 2 + 1, givenSize.Z + 1);
	model->Initialize(givenSize, 0, allRulesets);
	TArray<TArray<TArray<FModelCell>>>& generatedGrid = model->D_GetGrid();
	int gridXDimension = generatedGrid.Num();
	if(gridXDimension != expectedSize.X)
	{
		UE_LOG(LogTemp, Error, TEXT("Grid x dimension is %i, expected %f"), gridXDimension, expectedSize.X)
		return false;
	}
	int gridYDimension = generatedGrid[0].Num();
	if(gridYDimension != expectedSize.Y)
	{
		UE_LOG(LogTemp, Error, TEXT("Grid y dimension is %i, expected %f"), gridYDimension, expectedSize.Y)
		return false;
	}
	int gridZDimension = generatedGrid[0][0].Num();
	if(gridZDimension != expectedSize.Z)
	{
		UE_LOG(LogTemp, Error, TEXT("Grid z dimension is %i, expected %f"), gridYDimension, expectedSize.Z)
		return false;
	}

	return true;
}