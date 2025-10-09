#pragma once

#include "CoreMinimal.h"
#include "GridManipulator.h"
#include "RoadGeneration/RoadGridCell.h"
#include "UObject/Object.h"
#include "GridManager.generated.h"

UCLASS(BlueprintType)
class PROJECT_URBAN_API UGridDrawer : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void DrawGrid();
	UFUNCTION(BlueprintCallable)
	void InitGrid(int x_cell_count, int y_cell_count, int cell_size);
	UFUNCTION(BlueprintCallable)
	void StopDrawing();

	UFUNCTION(BlueprintCallable)
	void AddManipulator(FString name, TScriptInterface<IGridManipulator> manipulator, FTransform transform);
	UFUNCTION(BlueprintCallable)
	void UpdateManipulatorTransform(FString name, FTransform new_transform);
	
	UFUNCTION(BlueprintCallable)
	void OnGridXCellCountUpdated(int new_x_cells)
	{
		road_grid_cells_.Empty();
		UE_LOG(LogTemp, Warning, TEXT("X cell count updated"))
		InitGrid(new_x_cells, grid_y_cells_, grid_cell_size_);
	}
	UFUNCTION(BlueprintCallable)
	void OnGridYCellCountUpdated(int new_y_cells)
	{
		road_grid_cells_.Empty();
		UE_LOG(LogTemp, Warning, TEXT("Y cell count updated"))
		InitGrid(grid_x_cells_, new_y_cells, grid_cell_size_);
	}
	UFUNCTION(BlueprintCallable)
	void OnGridCellSizeUpdated(int new_cell_size)
	{
		grid_cell_size_ = new_cell_size;
	}
private:
	TArray<URoadGridCell> road_grid_cells_;
	TObjectPtr<ULineBatchComponent> persistent_line_batcher_;
	UPROPERTY()
	TMap<FString, TScriptInterface<IGridManipulator>> manipulators_;
	
	int grid_x_cells_;
	int grid_y_cells_;
	int grid_cell_size_;
};
