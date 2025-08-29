#include "ProceduralGeneration/Model/U_Generation_Model_Visualizer.h"

void UGenerationModelVisualizer::Initialize(UWorld* world)
{
}

void UGenerationModelVisualizer::DrawGridPossibilities(FWorldCoordinate visual_scale, FWorldCoordinate offset,
	float spacing, UMaterial* material, TArray<FModelCell*> grid, FString directory_parent)
{
}

void UGenerationModelVisualizer::DrawGrid(FWorldCoordinate center, FGridLocalCoordinate extents, float line_thickness,
	int cell_size)
{
}

void UGenerationModelVisualizer::SetCellColour(FGridArrayIndexCoordinate coordinate, FColor colour)
{
}

void UGenerationModelVisualizer::SetAllCellColours(FColor colour)
{
}

void UGenerationModelVisualizer::CleanUpPossibilitiesVisualization()
{
}
