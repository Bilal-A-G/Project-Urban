#include "Project_Urban/Public/ProceduralGeneration/ProceduralGenerationCore.h"

#include "Components/LineBatchComponent.h"
#include "Editor.h"
#include "ProceduralGeneration\UGenerationRuleset.h"
#include "ProceduralGeneration\UTileEntryDTO.h"
#include "ProceduralGeneration/UGenerationModel.h"
#include "ProceduralGeneration/ULabel.h"

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
	for (int x = -this->gridDimensions.X; x < this->gridDimensions.X + 1; x++)
	{
		for (int y = -this->gridDimensions.Y; y < this->gridDimensions.Y + 1; y++)
		{
			for (int z = 0; z < this->gridDimensions.Z + 1; z++)
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

void UProceduralGenerationCore::Generate(TArray<UTileEntryDTO*> tiles)
{
	this->model = NewObject<UGenerationModel>(this);
	TArray<UGenerationRuleset*> allRuleSets;
	
	for (const UTileEntryDTO* tileEntry : tiles)
	{
		const TArray<ULabel*> upAdjacencies = tileEntry->tileRuleset->GetAdjacencyValuesFromKey(EAdjacency::UP);
		for (const ULabel* label : upAdjacencies)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found %s adjacency in Up %s"),
				*label->Mesh->GetName(), *tileEntry->name)
		}
		allRuleSets.Add(tileEntry->tileRuleset);
	}
	
	model->Initialize(FVector(2, 2, 1), 100, allRuleSets);
	//model->CollapseTile(FVector(0, 0, 0), GetWorld());
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
