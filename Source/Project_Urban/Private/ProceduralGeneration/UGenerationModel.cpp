#include "ProceduralGeneration/UGenerationModel.h"

#include "Engine/StaticMeshActor.h"
#include "ProceduralGeneration/FModelCell.h"
#include "ProceduralGeneration/UGenerationRuleset.h"
#include "ProceduralGeneration/ULabel.h"

void UGenerationModel::Initialize(FVector gridSize, int cellSize, TArray<UGenerationRuleset*> allPossibleRuleSets)
{
	DestroySpawnedActors();
	_grid.Empty();
	_validCollapseIndices.Empty();

	_grid.SetNum(gridSize.X);
	TArray<EAdjacency> adjacencies;
	TArray<FVector> borderCells;

	UGenerationRuleset* airRuleset = nullptr;
	for (int i = 0; i < allPossibleRuleSets.Num(); i++)
	{
		if(allPossibleRuleSets[i]->Current->Mesh == nullptr)
			airRuleset = allPossibleRuleSets[i];
	}

	for (int x = 0; x < gridSize.X; x++)
	{
		_grid[x].SetNum(gridSize.Y);
		for (int y = 0; y < gridSize.Y; y++)
		{
			for (int z = 0; z < gridSize.Z; z++)
			{
				adjacencies.Empty();
				bool minX = x == 0;
				bool maxX = x == gridSize.X - 1;
				bool minY = y == 0;
				bool maxY = y == gridSize.Y - 1;
				if(minX || minY || maxX || maxY)
					borderCells.Add(FVector(x, y, z));

				if(minY)
					adjacencies.Add(EAdjacency::BACKWARD);
				else if(maxY)
					adjacencies.Add(EAdjacency::FORWARD);
				if(minX)
					adjacencies.Add(EAdjacency::RIGHT);
				else if (maxX)
					adjacencies.Add(EAdjacency::LEFT);

				TArray<UGenerationRuleset*> rulesets = allPossibleRuleSets;
				if(airRuleset != nullptr)
				{
					for (int i = 0; i < adjacencies.Num(); i++)
					{
						UGenerationRuleset::RemoveInconsistentLabels(airRuleset, rulesets, adjacencies[i]);
						UE_LOG(LogTemp, Warning, TEXT("Current num rulesets = %i"), rulesets.Num())
					}
				}
				
				FModelCell createdCell = FModelCell(rulesets);
				UE_LOG(LogTemp, Warning, TEXT("Created new cell with candidates %i at index (%i, %i, %i)"),
				       createdCell.CandidateRuleSets.Num(), x, y, z);
				_grid[x][y].Add(MoveTemp(createdCell));
				_validCollapseIndices.Add(FVector(x, y, z));
			}
		}
	}

	FDateTime startTime = FDateTime::Now();
	this->_gridSize = gridSize;
	this->_cellSize = cellSize;

	//Propagate all border cells to rest of grid, this basically makes sure
	//the edges can only border air, and the rest of the model respects that
	for (int i = 0; i < borderCells.Num(); i++)
	{
		for (int v = 0; v < static_cast<int>(EAdjacency::LAST); v++)
		{
			RecursivePropagateToNeighbours(borderCells[i], v);
		}
	}

	ResetVisited();
	FTimespan elapsed = FDateTime::Now() - startTime;
	UE_LOG(LogTemp, Warning, TEXT("Time elapsed since function %f"), elapsed.GetTotalMilliseconds());
}

TArray<AStaticMeshActor*> UGenerationModel::GetPossibleTileVisualization(FVector visualScale,
                                                                         UWorld* world, FVector offset, float spacing,
                                                                         UMaterial* material)
{
	TArray<AStaticMeshActor*> visualizations;

	for (int x = 0; x < _gridSize.X; x++)
	{
		for (int y = 0; y < _gridSize.Y; y++)
		{
			for (int z = 0; z < _gridSize.Z; z++)
			{
				FModelCell cell = _grid[x][y][z];
				FVector4 cellVisualizationColour = cell.VisualizationColour;
				for (int i = 0; i < cell.CandidateRuleSets.Num(); i++)
				{
					UGenerationRuleset* ruleset = cell.CandidateRuleSets[i];
					FQuat rotation = ruleset->Current->Rotation;
					int maxTiles = (int)(_cellSize * 2 / spacing) - 1;
					maxTiles = FMath::Max(maxTiles, 1);
					UE_LOG(LogTemp, Warning, TEXT("Max tiles = %i"), maxTiles)
					FVector position = TileIndexToCoordinates(FVector(x, y, z)) -
						FVector(_cellSize - (i % maxTiles + 1) * spacing,
						        _cellSize - i / maxTiles * spacing, 0) + offset;
					FTransform spawnTransform = FTransform(rotation, position,
					                                       visualScale * ruleset->Current->Scale);
					AStaticMeshActor* levelMeshActor =
						world->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), spawnTransform);
					UStaticMeshComponent* meshComponent = levelMeshActor->GetStaticMeshComponent();
					meshComponent->SetStaticMesh(ruleset->Current->Mesh);
					meshComponent->SetMobility(EComponentMobility::Static);
					meshComponent->SetSimulatePhysics(false);
					meshComponent->SetMaterial(0, material);
					meshComponent->SetCustomPrimitiveDataVector4(0, cellVisualizationColour);

					levelMeshActor->SetMobility(EComponentMobility::Static);
					levelMeshActor->SetFlags(RF_Transient);
					visualizations.Add(levelMeshActor);
				}
			}
		}
	}

	return visualizations;
}

bool UGenerationModel::CollapseTile(FVector tileIndex, UWorld* world, bool update)
{
	FModelCell modelCell = _grid[(int)tileIndex.X][(int)tileIndex.Y][(int)tileIndex.Z];
	TArray<UGenerationRuleset*> candidateRuleSets = modelCell.CandidateRuleSets;
	if(modelCell.Collapsed)
		return false;
	if (candidateRuleSets.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Error, no candidates found in ruleset "
			       "in cell at (%f, %f, %f)"), tileIndex.X, tileIndex.Y, tileIndex.Z);
		return false;
	}
	UGenerationRuleset* chosenRuleset = candidateRuleSets[rand() % candidateRuleSets.Num()];
	candidateRuleSets.Empty();
	candidateRuleSets.Add(chosenRuleset);
	//Have to reassign since we're modifying a copy, need to fix this
	modelCell.CandidateRuleSets = candidateRuleSets;
	modelCell.Collapsed = true;
	_grid[tileIndex.X][tileIndex.Y][tileIndex.Z] = modelCell;

	if (world == nullptr)
		return false;
	
	ULabel* chosenLabel = chosenRuleset->Current;
	if (chosenLabel->Mesh != nullptr)
	{
		FVector spawnLocation = TileIndexToCoordinates(tileIndex);
		FTransform transform = FTransform(chosenLabel->Rotation, spawnLocation, chosenLabel->Scale);

		AStaticMeshActor* levelMeshActor =
			world->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), transform);

		if (levelMeshActor == nullptr)
			return false;

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
	if(update)
		OnGridUpdated.Broadcast();
	return true;
}

TTuple<bool, FVector> UGenerationModel::CollapseRandomValidTile(UWorld* world, bool update)
{
	if(_validCollapseIndices.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ran out of tiles to collapse! Generation is over"))
		return MakeTuple(false, FVector(-1,-1,-1));
	}
	int randomChoice = rand() % _validCollapseIndices.Num();
	FVector randomChoiceIndex = _validCollapseIndices[randomChoice];
	bool collapsed = CollapseTile(randomChoiceIndex, world, update);
	if (collapsed)
		_validCollapseIndices.RemoveAt(randomChoice);
	return MakeTuple(collapsed, randomChoiceIndex);
}

FLinearColor UGenerationModel::GetColourAtIndex(FVector index)
{
	if(_grid.Num() <= index.X || _grid[index.X].Num() <= index.Y || _grid[index.X][index.Y].Num() <= index.Z)
		return FLinearColor::White;
	return _grid[index.X][index.Y][index.Z].Colour;
}

void UGenerationModel::SetColourAtIndex(FVector index, FLinearColor colour)
{
	_grid[index.X][index.Y][index.Z].Colour = colour;
	OnOnlyColoursUpdated.Broadcast();
}

void UGenerationModel::ResetColours()
{
	for (int x = 0; x < _gridSize.X; x++)
	{
		for (int y = 0; y < _gridSize.Y; y++)
		{
			for (int z = 0; z < _gridSize.Z; z++)
			{
				_grid[x][y][z].Colour = FLinearColor::White;
			}
		}
	}
	OnOnlyColoursUpdated.Broadcast();
}

void UGenerationModel::ResetVisited()
{
	for (int x = 0; x < _gridSize.X; x++)
	{
		for (int y = 0; y < _gridSize.Y; y++)
		{
			for (int z = 0; z < _gridSize.Z; z++)
			{
				_grid[x][y][z].Visited = false;
			}
		}
	}
}

bool UGenerationModel::PropagateToNeighbours(FVector tileIndex, int neighbourIndex, bool updateGrid)
{
	TArray<UGenerationRuleset*>& rulesetsAtIndex = _grid[tileIndex.X][tileIndex.Y][tileIndex.Z].CandidateRuleSets;
	if (neighbourIndex >= static_cast<int>(EAdjacency::LAST))
		return false;

	EAdjacency currentAdjacency = static_cast<EAdjacency>(neighbourIndex);
	FString stringAdjacency = UEnum::GetValueAsString(currentAdjacency);
	
	FVector adjacencyIndex = tileIndex + PUrban::ToVector(currentAdjacency);
	if (adjacencyIndex.X >= _gridSize.X || adjacencyIndex.X < 0 ||
		adjacencyIndex.Y >= _gridSize.Y || adjacencyIndex.Y < 0 ||
		adjacencyIndex.Z >= _gridSize.Z || adjacencyIndex.Z < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid index at adjacency %s "
			       "for cell at coordinates (%f, %f, %f)"),
		       *stringAdjacency, tileIndex.X, tileIndex.Y, tileIndex.Z);
		return false;
	}
	FDateTime startTime = FDateTime::Now();

	UE_LOG(LogTemp, Warning, TEXT("Trying to get ruleset at array index (%f, %f, %f)"),
		adjacencyIndex.X, adjacencyIndex.Y, adjacencyIndex.Z);
	TArray<UGenerationRuleset*>& rulesetsAtAdjacency =
		_grid[adjacencyIndex.X][adjacencyIndex.Y][adjacencyIndex.Z].CandidateRuleSets;
	UE_LOG(LogTemp, Warning, TEXT("Cell at index (%f, %f, %f) at adjacency %s currently contains %i candidates"),
	       adjacencyIndex.X, adjacencyIndex.Y, adjacencyIndex.Z, *stringAdjacency, rulesetsAtAdjacency.Num());
	//Modifying by ref
	FDateTime removeTime = FDateTime::Now();
	if(!UGenerationRuleset::RemoveInconsistentLabels(rulesetsAtIndex,rulesetsAtAdjacency, currentAdjacency))
		return false;
	FTimespan elapsedSinceRem = FDateTime::Now() - removeTime;
	UE_LOG(LogTemp, Warning, TEXT("Time elapsed since removed %f"), elapsedSinceRem.GetTotalMilliseconds());

	if(updateGrid)
		OnGridUpdated.Broadcast();

	UE_LOG(LogTemp, Warning, TEXT("Cell at index (%f, %f, %f) now contains %i candidates"),
	       adjacencyIndex.X, adjacencyIndex.Y, adjacencyIndex.Z, rulesetsAtAdjacency.Num());

	FTimespan elapsed = FDateTime::Now() - startTime;
	UE_LOG(LogTemp, Warning, TEXT("Time elapsed since adjacencies updated %f"), elapsed.GetTotalMilliseconds());

	return true;
}

void UGenerationModel::RecursivePropagateToNeighbours(FVector tileIndex, int neighbourIndex)
{
	if (!PropagateToNeighbours(tileIndex, neighbourIndex, false))
		return;
	EAdjacency adjacencyEnum = static_cast<EAdjacency>(neighbourIndex);
	EAdjacency opposite = PUrban::Opposite(adjacencyEnum);
	FVector adjacentTileIndex = tileIndex + PUrban::ToVector(adjacencyEnum);
	//If out of bounds
	if (adjacentTileIndex.X >= _gridSize.X || adjacentTileIndex.X < 0 ||
	adjacentTileIndex.Y >= _gridSize.Y || adjacentTileIndex.Y < 0 ||
	adjacentTileIndex.Z >= _gridSize.Z || adjacentTileIndex.Z < 0)
	{
		return;
	}
	for (int i = 0; i < static_cast<int>(EAdjacency::LAST); i++)
	{
		EAdjacency asAdjacency = static_cast<EAdjacency>(i);
		if (asAdjacency == opposite)
			continue;

		UE_LOG(LogTemp, Warning, TEXT("Recursed"))
		RecursivePropagateToNeighbours(adjacentTileIndex, i);
	}
}

void UGenerationModel::RecursivePropagateToAllNeighbours(FVector tileIndex)
{
	for (int i = 0; i < static_cast<int>(EAdjacency::LAST); i++)
	{
		RecursivePropagateToNeighbours(tileIndex, i);
	}
}

void UGenerationModel::DestroySpawnedActors()
{
	for (int i = 0; i < _spawnedActors.Num(); i++)
	{
		if (!_spawnedActors[i]->IsValidLowLevel())
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
	FVector xyGridSize = FVector(_gridSize.X, _gridSize.Y, 0);
	return (index - (xyGridSize - FVector(1, 1, 0))/2) * (_cellSize * 2);
}
