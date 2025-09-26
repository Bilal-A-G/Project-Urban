#include "RoadGeneration/GridDrawer.h"

#include "Components/LineBatchComponent.h"

void UGridDrawer::InitGrid(int x_cell_count, int y_cell_count, int cell_size)
{
	int grid_size = x_cell_count * y_cell_count;
	UE_LOG(LogTemp, Warning, TEXT("Initialized grid with size %i, x = %i, y = %i"), grid_size, x_cell_count, y_cell_count)
	for(int i = 0; i < grid_size; i++)
	{
		road_grid_cells_.Add(URoadGridCell());
	}
	grid_cell_size_ = cell_size;
	grid_x_cells_ = x_cell_count;
	grid_y_cells_ = y_cell_count;

	persistent_line_batcher_ = GetWorld()->PersistentLineBatcher;
}

void UGridDrawer::DrawGrid()
{
	persistent_line_batcher_->Flush();
	for(int i = 0; i < road_grid_cells_.Num(); i++)
	{
		URoadGridCell& cell = road_grid_cells_[i];
		int yIndex = i / grid_x_cells_;
		int xIndex = i % grid_x_cells_;
		FVector center = (FVector(-(grid_x_cells_ - 1)/2.0f, -(grid_y_cells_ - 1)/2.0f, 0) + FVector(xIndex, yIndex, 0)) * (grid_cell_size_ * 2);
		FVector2d vector = cell.vector;
		FVector size = FVector(grid_cell_size_, grid_cell_size_, grid_cell_size_);
		persistent_line_batcher_->DrawBox(center, size, cell.colour, -1.0f, 0, 0);
		DrawDebugDirectionalArrow(GetWorld(), center, center + FVector(vector.X, vector.Y, 0) * grid_cell_size_
			, 100, FColor(255,255,255), true, -1, 100, 10);
	}	
}

void UGridDrawer::StopDrawing()
{
	persistent_line_batcher_->Flush();
}


