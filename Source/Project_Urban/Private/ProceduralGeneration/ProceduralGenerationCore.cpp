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
	model->OnGridUpdated.AddUniqueDynamic(this, &UProceduralGenerationCore::OnGridChanged);
	model->OnOnlyColoursUpdated.AddUniqueDynamic(this, &UProceduralGenerationCore::OnOnlyColoursChanged);
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
	TArray<UGenerationRuleset*> allRuleSets = TArray<UGenerationRuleset*>();
	
	this->gridDimensions = gridSize;
	this->cellDimension = cellSize;
	for (const UTileEntryDTO* tileEntry : tiles)
	{
		allRuleSets.Add(tileEntry->tileRuleset);
	}
	
	this->lastUsedAllPossibleRuleSets = allRuleSets;
	FVector convertedGridSize = FVector(gridDimensions.X * 2 + 1,
	gridDimensions.Y * 2 + 1, gridDimensions.Z + 1);
	model->Initialize(convertedGridSize, this->cellDimension,
	allRuleSets);
	commandPlayer->Init(GetWorld(), convertedGridSize, model);
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
	commandPlayer->StepForward();
}

void UProceduralGenerationCore::TogglePlaying()
{
	commandPlayer->TogglePlay();
}

void UProceduralGenerationCore::SetPlaybackSpeed(float speed)
{
	commandPlayer->SetPlaybackSpeed(speed);
}

void UProceduralGenerationCore::Tick(float deltaTime)
{
	commandPlayer->Tick(deltaTime);
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

	FVector convertedGridSize = FVector(gridDimensions.X * 2 + 1,
	gridDimensions.Y * 2 + 1, gridDimensions.Z + 1);
	model->Initialize(convertedGridSize, cellDimension, lastUsedAllPossibleRuleSets);
	commandPlayer->Clear();
}

void UProceduralGenerationCore::OnGridChanged()
{
	OnGridUpdated.Broadcast();
}

void UProceduralGenerationCore::OnOnlyColoursChanged()
{
	OnOnlyColoursUpdated.Broadcast();
}
