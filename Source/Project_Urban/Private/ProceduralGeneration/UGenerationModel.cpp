#include "ProceduralGeneration/UGenerationModel.h"

#include "Engine/StaticMeshActor.h"
#include "ProceduralGeneration/FModelCell.h"
#include "ProceduralGeneration/UGenerationRuleset.h"
#include "ProceduralGeneration/ULabel.h"

void UGenerationModel::Initialize(FVector gridSize, int cellSize, TArray<UGenerationRuleset*>& allPossibleRuleSets)
{
	_grid.SetNum(gridSize.X);

	for (int x = 0; x < gridSize.X; x++)
	{
		_grid[x].SetNum(gridSize.Y);
		for (int y = 0; y < gridSize.Y; y++)
		{
			_grid[x][y].SetNum(gridSize.Z);
			for (int z = 0; z < gridSize.Z; z++)
			{
				_grid[x][y].Add(FModelCell(allPossibleRuleSets));
			}
		}
	}

	this->_gridSize = gridSize;
	this->_cellSize = cellSize;
}

void UGenerationModel::CollapseTile(FVector tileIndex, UWorld* world)
{
	FModelCell modelCell = _grid[tileIndex.X][tileIndex.Y][tileIndex.Z];
	TArray<UGenerationRuleset*>& candidateRuleSets = modelCell.CandidateRuleSets;
	UGenerationRuleset* chosenRuleset = candidateRuleSets[rand() % candidateRuleSets.Num()];
	candidateRuleSets.Empty();
	candidateRuleSets.Add(chosenRuleset);

	if (world == nullptr)
		return;

	FVector spawnLocation = tileIndex;
	ULabel* chosenLabel = chosenRuleset->Current;
	FTransform transform = FTransform(chosenLabel->Rotation, spawnLocation, chosenLabel->Scale);
	
	AStaticMeshActor* levelMeshActor =
		world->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), transform);

	if(levelMeshActor == nullptr)
		return;

	UStaticMeshComponent* meshComponent = levelMeshActor->GetStaticMeshComponent();
	meshComponent->SetStaticMesh(chosenLabel->Mesh);
	meshComponent->SetMobility(EComponentMobility::Static);
	meshComponent->SetSimulatePhysics(false);

	levelMeshActor->SetMobility(EComponentMobility::Static);
	UE_LOG(LogTemp, Warning, TEXT("Successfully spawned a static mesh actor! "
						   "at (%f, %f, %f)"), spawnLocation.X, spawnLocation.Y, spawnLocation.Z)
	_spawnedActors.Add(levelMeshActor);
}

void UGenerationModel::BeginDestroy()
{
	Super::BeginDestroy();
	for(int i = 0; i < _spawnedActors.Num(); i++)
	{
		if(!_spawnedActors[i]->IsValidLowLevel())
			continue;
		_spawnedActors[i]->Destroy();
	}

	_spawnedActors.Empty();
}
