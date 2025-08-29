#pragma once

#include "CoreMinimal.h"
#include "ProceduralGeneration/FModelCell.h"
#include "ProceduralGeneration/CoordinateSpaces/F_Grid_Array_Index_Coordinate.h"
#include "ProceduralGeneration/CoordinateSpaces/F_Grid_Local_Coordinate.h"
#include "ProceduralGeneration/CoordinateSpaces/F_World_Coordinate.h"
#include "UObject/Object.h"
#include "U_Generation_Model_Visualizer.generated.h"

UCLASS()
class PROJECT_URBAN_API UGenerationModelVisualizer : public UObject
{
	GENERATED_BODY()
public:
	void Initialize(UWorld* world);
	void DrawGridPossibilities(FWorldCoordinate visual_scale, FWorldCoordinate offset, float spacing,
		UMaterial* material, TArray<FModelCell*> grid, FString directory_parent);
	void DrawGrid(FWorldCoordinate center, FGridLocalCoordinate extents, float line_thickness, int cell_size);
	void SetCellColour(FGridArrayIndexCoordinate coordinate, FColor colour);
	void SetAllCellColours(FColor colour);
	void CleanUpPossibilitiesVisualization();
private:
	UPROPERTY()
	UWorld* world_;
	UPROPERTY()
	TArray<AStaticMeshActor*> possibilities_visualization_actors_;
};
