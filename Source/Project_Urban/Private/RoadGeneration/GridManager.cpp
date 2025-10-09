#include "RoadGeneration/GridManager.h"

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
	manipulators_ = TMap<FString,  TScriptInterface<IGridManipulator>>();
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
		FVector2d majorVector = FVector2d(0, 0);
		FVector2d minorVector = FVector2d(0, 0);
		for (auto kvp : manipulators_)
		{
			TScriptInterface<IGridManipulator> value = kvp.Value;
			FVector majorEigenVector = value->CalculateMajorEigenVector(center);
			FVector minorEigenVector = value->CalculateMinorEigenVector(center);
			majorVector += FVector2d(majorEigenVector.X, majorEigenVector.Y);
			minorVector += FVector2d(minorEigenVector.X, minorEigenVector.Y);
		}
		if(manipulators_.Num() > 0)
		{
			majorVector /= manipulators_.Num();
			minorVector /= manipulators_.Num();
		}
		majorVector.Normalize();
		minorVector.Normalize();
		
		FVector size = FVector(grid_cell_size_, grid_cell_size_, grid_cell_size_);
		persistent_line_batcher_->DrawBox(center, size, cell.colour, -1.0f, 0, 0);
		DrawDebugDirectionalArrow(GetWorld(), center, center + FVector(majorVector.X, majorVector.Y, 0) * grid_cell_size_
			, 100, FColor(255,255,255), true, -1, 100, 10);
		DrawDebugDirectionalArrow(GetWorld(), center, center + FVector(minorVector.X, minorVector.Y, 0) * grid_cell_size_ / 2.0f
			, 100, FColor(255,0,0), true, -1, 100, 10);
	}	
}

void UGridDrawer::StopDrawing()
{
	persistent_line_batcher_->Flush();
}

void UGridDrawer::AddManipulator(FString name, TScriptInterface<IGridManipulator> manipulator, FTransform transform)
{
	manipulator->UpdateTransform(transform);
	manipulators_.Add(name, manipulator);
	
	UE_LOG(LogTemp, Warning, TEXT("Created a new manipulator"));
}

void UGridDrawer::UpdateManipulatorTransform(FString name, FTransform new_transform)
{
	TScriptInterface<IGridManipulator>* manipulator = manipulators_.Find(name);
	manipulator->GetInterface()->UpdateTransform(new_transform);

	UE_LOG(LogTemp, Warning, TEXT("Updating manipulator transform"));
}
