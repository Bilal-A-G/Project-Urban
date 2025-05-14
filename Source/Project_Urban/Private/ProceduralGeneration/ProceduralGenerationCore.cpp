#include "Project_Urban/Public/ProceduralGeneration/ProceduralGenerationCore.h"

#include "Components/LineBatchComponent.h"
#include "Editor.h"
#include "ProceduralGeneration/FGenerationRuleset.h"
#include "ProceduralGeneration/FLabel.h"
#include "ProceduralGeneration/TileEntryDTO.h"
#include "ProceduralGeneration/UGenerationModel.h"

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

TArray<FLabel*> GetAdjacencyArray(TArray<FLabel*>& allLabels, TArray<int>& adjacencies)
{
	TArray<FLabel*> directionalAdjacencies;
	for (int v = 0; v < adjacencies.Num(); v++)
	{
		int currentAdjacency = adjacencies[v];
		if (currentAdjacency - 1 < 0)
		{
			directionalAdjacencies.Add(FLabel::Null);
			continue;
		}
		directionalAdjacencies.Add(allLabels[currentAdjacency - 1]);
	}

	return directionalAdjacencies;
}

void UProceduralGenerationCore::Generate(TArray<UTileEntryDTO*> tiles)
{
	if(this->model == nullptr)
		this->model = NewObject<UGenerationModel>();

	TArray<FLabel*> allLabels;
	TArray<FGenerationRuleset*> allRulesets;
	
	for (int i = 0; i < tiles.Num(); i++)
	{
		UTileEntryDTO* tileEntry = tiles[i];
		UE_LOG(LogTemp, Warning, TEXT("Got tile with (%f, %f, %f))"),
			tileEntry->scale.X, tileEntry->scale.Y, tileEntry->scale.Z)
		
		FLabel* newLabel = new FLabel(tileEntry->mesh,
			FQuat::MakeFromEuler(tileEntry->rotation), tileEntry->scale);
		allLabels.Add(newLabel);
	}

	for (int i = 0; i < tiles.Num(); i++)
	{
		FGenerationRuleset* ruleset = new FGenerationRuleset();
		ruleset->Current = allLabels[i];
		TArray<FLabel*> upAdjacencies = GetAdjacencyArray(allLabels, tiles[i]->upAdjacencies);
		TArray<FLabel*> downAdjacencies = GetAdjacencyArray(allLabels, tiles[i]->downAdjacencies);
		TArray<FLabel*> leftAdjacencies = GetAdjacencyArray(allLabels, tiles[i]->leftAdjacencies);
		TArray<FLabel*> rightAdjacencies = GetAdjacencyArray(allLabels, tiles[i]->rightAdjacencies);

		UE_LOG(LogTemp, Warning, TEXT("Tile %s has %i up, %i down, %i left, %i right"), *tiles[i]->name,
			upAdjacencies.Num(), downAdjacencies.Num(), leftAdjacencies.Num(), rightAdjacencies.Num())

		ruleset->Adjacencies.Add(PUrban::Adjacency::UP, upAdjacencies);
		ruleset->Adjacencies.Add(PUrban::Adjacency::DOWN, downAdjacencies);
		ruleset->Adjacencies.Add(PUrban::Adjacency::LEFT, leftAdjacencies);
		ruleset->Adjacencies.Add(PUrban::Adjacency::RIGHT, rightAdjacencies);
		allRulesets.Add(ruleset);
	}
	
	model->Init(FVector(10, 10, 10), allLabels);
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
