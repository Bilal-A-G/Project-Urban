#include "Project_Urban/Public/ProceduralGeneration/ProceduralGenerationCore.h"

#include "Components/LineBatchComponent.h"
#include "Editor.h"

void UProceduralGenerationCore::DrawGrid(FVector gridSize, FVector centerPosition, int cellSize, float lineThickness)
{
	this->gridDimensions = gridSize;
	this->cellDimension = cellSize;

	UWorld* world = GetWorld();
	if(world == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get UWorld, can't draw stuff!"))
		return;
	}
	ULineBatchComponent* lineBatcher = world->PersistentLineBatcher;
	if(lineBatcher == nullptr)
		return;
	
	lineBatcher->Flush();
	for (int x = -gridDimensions.X; x < gridDimensions.X + 1; x++)
	{
		for (int y = -gridDimensions.Y; y < gridDimensions.Y + 1; y++)
		{
			for (int z = 0; z < gridDimensions.Z + 1; z++)
			{
				FVector offset = FVector(x, y, z) * cellSize * 2;
				lineBatcher->DrawBox(centerPosition + offset, FVector(cellSize, cellSize, cellSize),
					FLinearColor::White, -1.0f, 0, lineThickness);
			}
		}
	}

	for (FEditorViewportClient* client : GEditor->GetAllViewportClients())
	{
		client->Invalidate();
	}
}

void UProceduralGenerationCore::ClearDebugGizmos()
{
	UWorld* world = GetWorld();
	if(world == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get UWorld, can't draw stuff!"))
		return;
	}
	ULineBatchComponent* lineBatcher = world->PersistentLineBatcher;
	if(lineBatcher == nullptr)
		return;

	lineBatcher->Flush();

	for (FEditorViewportClient* client : GEditor->GetAllViewportClients())
	{
		client->Invalidate();
	}
}
