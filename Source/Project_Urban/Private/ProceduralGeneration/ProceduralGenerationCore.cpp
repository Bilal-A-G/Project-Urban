#include "Project_Urban/Public/ProceduralGeneration/ProceduralGenerationCore.h"

#include "Components/LineBatchComponent.h"
#include "Editor.h"
#include "Engine/StaticMeshActor.h"
#include "ProceduralGeneration/UCommandPlayer.h"
#include "ProceduralGeneration\UGenerationRuleset.h"
#include "ProceduralGeneration\UTileEntryDTO.h"
#include "ProceduralGeneration/UGenerationModel.h"

void UProceduralGenerationCore::Init()
{
	this->model = NewObject<UGenerationModel>(this);
	this->commandPlayer = NewObject<UCommandPlayer>(this);
	commandPlayer->Init();
	model->OnGridUpdated.AddUniqueDynamic(this, &UProceduralGenerationCore::OnGridChanged);
}

void UProceduralGenerationCore::DrawGrid(FVector gridSize,
	FVector centerPosition, int cellSize, float lineThickness)
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
	for (int x = -this->gridDimensions.X; x < this->gridDimensions.X + 1; x++)
	{
		for (int y = -this->gridDimensions.Y; y < this->gridDimensions.Y + 1; y++)
		{
			for (int z = 0; z < this->gridDimensions.Z + 1; z++)
			{
				float thickness = lineThickness;
				int depthPriority = 0;
				FVector wholeIndex = FVector(x + gridDimensions.X, y + gridDimensions.Y, z);
				FLinearColor colour = FLinearColor::White;
				if(model != nullptr)
					colour = model->GetColourAtIndex(wholeIndex);
				
				if(colour != FLinearColor::White)
				{
					thickness = lineThickness * 1.25f;
					depthPriority = 1;
				}
				
				FVector offset = FVector(x, y, z) * cellSize * 2;
				lineBatcher->DrawBox(centerPosition + offset, FVector(cellSize, cellSize, cellSize),
					colour, -1.0f, depthPriority, thickness);
			}
		}
	}

	for (FEditorViewportClient* client : GEditor->GetAllViewportClients())
	{
		client->Invalidate();
	}
}

void UProceduralGenerationCore::DrawVisualizations(FVector gridSize, FVector centerPosition, int cellSize,
	float lineThickness, TArray<UTileEntryDTO*> tiles)
{
	this->gridDimensions = gridSize;
	this->cellDimension = cellSize;
	
	TArray<UGenerationRuleset*> allRuleSets = TArray<UGenerationRuleset*>();
	
	for (const UTileEntryDTO* tileEntry : tiles)
	{
		allRuleSets.Add(tileEntry->tileRuleset);
	}
	FVector convertedGridSize = FVector(gridDimensions.X * 2 + 1,
	gridDimensions.Y * 2 + 1, gridDimensions.Z + 1);
	model->Initialize(convertedGridSize, this->cellDimension,
	allRuleSets);
	DrawGrid(gridSize, centerPosition, cellSize, lineThickness);
}

TArray<AStaticMeshActor*> UProceduralGenerationCore::GetTilesVisualization(FVector visualScale,
                                                                           FVector offset, float spacing, UMaterial* material)
{
	if(model == nullptr)
		return TArray<AStaticMeshActor*>();
	
	return model->GetPossibleTileVisualization(visualScale, GetWorld(), offset, spacing, material);
}

void UProceduralGenerationCore::StepForwards()
{
	FVector convertedGridSize = FVector(gridDimensions.X * 2 + 1,
		gridDimensions.Y * 2 + 1, gridDimensions.Z + 1);
	commandPlayer->StepForward(model, GetWorld(), convertedGridSize);
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

void UProceduralGenerationCore::ClearAll()
{
	if(model == nullptr)
		return;
	
	model->DestroySpawnedActors();
	for (int x = -this->gridDimensions.X; x < this->gridDimensions.X + 1; x++)
	{
		for (int y = -this->gridDimensions.Y; y < this->gridDimensions.Y + 1; y++)
		{
			for (int z = 0; z < this->gridDimensions.Z + 1; z++)
			{
				FVector wholeIndex = FVector(x + gridDimensions.X, y + gridDimensions.Y, z);
				model->SetColourAtIndex(wholeIndex, FLinearColor::White);
			}
		}
	}

	commandPlayer->Clear();
}

void UProceduralGenerationCore::OnGridChanged()
{
	OnGridUpdated.Broadcast();
}
