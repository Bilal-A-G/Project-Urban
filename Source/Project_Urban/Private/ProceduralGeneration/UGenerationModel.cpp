#include "ProceduralGeneration/UGenerationModel.h"

#include "Engine/StaticMeshActor.h"
#include "ProceduralGeneration/FModelCell.h"
#include "ProceduralGeneration/UGenerationRuleset.h"
#include "ProceduralGeneration/ULabel.h"

void UGenerationModel::Initialize(FVector gridSize, int cellSize, TArray<UGenerationRuleset*> allPossibleRuleSets)
{
	DestroySpawnedActors();
	_grid.Empty();
	
	_grid.SetNum(gridSize.X);

	for (int x = 0; x < gridSize.X; x++)
	{
		_grid[x].SetNum(gridSize.Y);
		for (int y = 0; y < gridSize.Y; y++)
		{
			for (int z = 0; z < gridSize.Z; z++)
			{
				FModelCell createdCell = FModelCell(allPossibleRuleSets);
				UE_LOG(LogTemp, Warning, TEXT("Created new cell with candidates %i at index (%i, %i, %i)"),
					createdCell.CandidateRuleSets.Num(), x, y, z);
				_grid[x][y].Add(MoveTemp(createdCell));
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Cell at (0,0,0) has %i candidates"), _grid[0][0][0].CandidateRuleSets.Num());
	
	this->_gridSize = gridSize;
	this->_cellSize = cellSize;
}

TArray<AStaticMeshActor*> UGenerationModel::GetPossibleTileVisualization(FVector visualScale,
	UWorld* world, FVector offset, float spacing, UMaterial* material)
{
	TArray<AStaticMeshActor*> visualizations;

	for (int x = 0; x < _gridSize.X; x++)
	{
		for (int y = 0; y < _gridSize.Y; y++)
		{
			for (int z = 0; z < _gridSize.Z; z++)
			{
				FModelCell cell = _grid[x][y][z];
				FVector4 randCellColour = FVector4(rand()%255, rand()%255, rand()%255);
				for (int i = 0; i < cell.CandidateRuleSets.Num(); i++)
				{
					UGenerationRuleset* ruleset = cell.CandidateRuleSets[i];
					FQuat rotation = ruleset->Current->Rotation;
					int maxTiles = (int)((_cellSize * 2)/spacing) - 1;
					FVector position = TileIndexToCoordinates(FVector(x,y,z)) -
						FVector(_cellSize - (i % maxTiles + 1) * spacing,
							_cellSize - (i) / maxTiles * spacing, 0) + offset;
					FTransform spawnTransform = FTransform(rotation, position,
						visualScale * ruleset->Current->Scale);
					AStaticMeshActor* levelMeshActor =
						world->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), spawnTransform);
					UStaticMeshComponent* meshComponent = levelMeshActor->GetStaticMeshComponent();
					meshComponent->SetStaticMesh(ruleset->Current->Mesh);
					meshComponent->SetMobility(EComponentMobility::Static);
					meshComponent->SetSimulatePhysics(false);
					meshComponent->SetMaterial(0, material);
					meshComponent->SetCustomPrimitiveDataVector4(0, randCellColour/255);
	
					levelMeshActor->SetMobility(EComponentMobility::Static);
					visualizations.Add(levelMeshActor);
				}
			}
		}
	}
	
	return visualizations;
}

void UGenerationModel::CollapseTile(FVector tileIndex, UWorld* world)
{
	FModelCell modelCell = _grid[(int)tileIndex.X][(int)tileIndex.Y][(int)tileIndex.Z];
	TArray<UGenerationRuleset*> candidateRuleSets = modelCell.CandidateRuleSets;
	if(candidateRuleSets.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Error, no candidates found in ruleset "
							  "in cell at (%f, %f, %f)"), tileIndex.X, tileIndex.Y, tileIndex.Z);
		return;
	}
	UGenerationRuleset* chosenRuleset = candidateRuleSets[rand() % candidateRuleSets.Num()];
	candidateRuleSets.Empty();
	candidateRuleSets.Add(chosenRuleset);

	//Have to reassign since we're modifying a copy, need to fix this
	modelCell.CandidateRuleSets = candidateRuleSets;
	_grid[tileIndex.X][tileIndex.Y][tileIndex.Z] = modelCell;
	
	if (world == nullptr)
		return;
	
	ULabel* chosenLabel = chosenRuleset->Current;
	FVector spawnLocation = TileIndexToCoordinates(tileIndex);
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
	UE_LOG(LogTemp, Warning, TEXT("Successfully spawned a static mesh actor, with mesh name %s"
						   "at (%f, %f, %f)"), *chosenLabel->Mesh->GetName(),
						   spawnLocation.X, spawnLocation.Y, spawnLocation.Z)
	_spawnedActors.Add(levelMeshActor);
}

void UGenerationModel::PropagateToNeighbours(FVector tileIndex)
{
	UGenerationRuleset* rulesetAtIndex = _grid[tileIndex.X][tileIndex.Y][tileIndex.Z].CandidateRuleSets[0];
	
	for (int i = 0; i < static_cast<int>(EAdjacency::LAST); i++)
	{
		EAdjacency currentAdjacency = static_cast<EAdjacency>(i);
		FString stringAdjacency = UEnum::GetValueAsString(currentAdjacency);

		FVector adjacencyIndex = tileIndex + PUrban::ToVector(currentAdjacency);
		if(adjacencyIndex.X >= _gridSize.X || adjacencyIndex.X < 0 ||
			adjacencyIndex.Y >= _gridSize.Y || adjacencyIndex.Y < 0 ||
			adjacencyIndex.Z >= _gridSize.Z || adjacencyIndex.Z < 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid index at adjacency %s "
								 "for cell at coordinates (%f, %f, %f)"),
								 *stringAdjacency, tileIndex.X, tileIndex.Y, tileIndex.Z);
			continue;
		}
		TArray<UGenerationRuleset*> rulesetsAtAdjacency =
			_grid[adjacencyIndex.X][adjacencyIndex.Y][adjacencyIndex.Z].CandidateRuleSets;
		UE_LOG(LogTemp, Warning, TEXT("Cell at index (%f, %f, %f) at adjacency %s currently contains %i candidates"),
		adjacencyIndex.X, adjacencyIndex.Y, adjacencyIndex.Z, *stringAdjacency, rulesetsAtAdjacency.Num());
		//Modifying by ref
		UGenerationRuleset::RemoveInconsistentLabels(rulesetAtIndex, rulesetsAtAdjacency, currentAdjacency);
		//We can now reset the value once we have modified the array, we should probably figure out a way to avoid
		//copying the array and reassigning it like this
		UE_LOG(LogTemp, Warning, TEXT("Cell at index (%f, %f, %f) now contains %i candidates"),
			adjacencyIndex.X, adjacencyIndex.Y, adjacencyIndex.Z, rulesetsAtAdjacency.Num());
		_grid[adjacencyIndex.X][adjacencyIndex.Y][adjacencyIndex.Z].CandidateRuleSets = rulesetsAtAdjacency;
	}
}

void UGenerationModel::DestroySpawnedActors()
{
	for(int i = 0; i < _spawnedActors.Num(); i++)
	{
		if(!_spawnedActors[i]->IsValidLowLevel())
			continue;
		_spawnedActors[i]->Destroy();
	}

	_spawnedActors.Empty();
}

void UGenerationModel::BeginDestroy()
{
	Super::BeginDestroy();
	DestroySpawnedActors();
}

FVector UGenerationModel::TileIndexToCoordinates(FVector index)
{
	return (index - (_gridSize - FVector(1, 1, 1))/2) * (_cellSize * 2);
}
