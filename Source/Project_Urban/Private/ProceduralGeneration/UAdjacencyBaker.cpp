#include "ProceduralGeneration/UAdjacencyBaker.h"

#include "Engine/StaticMeshActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProceduralGeneration/UGenerationModel.h"
#include "ProceduralGeneration/UGenerationRuleset.h"
#include "ProceduralGeneration/ULabel.h"
#include "ProceduralGeneration/UTileEntryDTO.h"

TArray<UTileEntryDTO*> UAdjacencyBaker::BakeAdjacencies(UWorld* world, FVector gridDimensions, int cellSize)
{
	TArray<UTileEntryDTO*> tileEntries;
	UE_LOG(LogTemp, Warning, TEXT("Grid dimensions = (%f, %f, %f)"),
		gridDimensions.X, gridDimensions.Y, gridDimensions.Z);

	int xLowerBounds = -gridDimensions.X - 1;
	int xUpperBounds = gridDimensions.X + 2;
	int yLowerBounds = -gridDimensions.Y - 1;
	int yUpperBounds = gridDimensions.Y + 2;
	int zLowerBounds = 0;
	int zUpperBounds = gridDimensions.Z + 1;
	
	for (int x = xLowerBounds; x < xUpperBounds; x++)
	{
		for (int y = yLowerBounds; y < yUpperBounds; y++)
		{
			for (int z = zLowerBounds; z < zUpperBounds; z++)
			{
				FVector worldCoordinates = FVector(x,y,z) * cellSize * 2;
				float halfSize = cellSize/2.0f;
				TTuple<ULabel*, FString> labelAtPosition = GetLabelAtPosition(world,worldCoordinates, halfSize);
				UTileEntryDTO* exitingEntry = GetExistingEntry(tileEntries, labelAtPosition.Key);
				bool doesNotExist = exitingEntry == nullptr;
				if(doesNotExist)
				{
					UGenerationRuleset* newRuleset = NewObject<UGenerationRuleset>();
					newRuleset->Current = labelAtPosition.Key;
					UTileEntryDTO* newEntry = NewObject<UTileEntryDTO>();
					newEntry->tileRuleset = newRuleset;
					newEntry->name = labelAtPosition.Value;

					tileEntries.Add(newEntry);
					UE_LOG(LogTemp, Warning, TEXT("Found unique actor %s, adding to list"), *labelAtPosition.Value)
					exitingEntry = newEntry;
				}
				UGenerationRuleset* ruleset = exitingEntry->tileRuleset;
				for (int i = 0; i < static_cast<int>(EAdjacency::LAST); i++)
				{
					EAdjacency currentAdjacency = static_cast<EAdjacency>(i);
					FVector direction = PUrban::ToVector(currentAdjacency);
					FVector movedIndex = FVector(x, y, z) + direction;
					FVector movedWorldPos = movedIndex * cellSize * 2;
					TTuple<ULabel*, FString> neighbour = GetLabelAtPosition(world, movedWorldPos, halfSize);
					bool success = ruleset->AddAdjacencyEntry(currentAdjacency, neighbour.Key);
					if(success)
					{
						UE_LOG(LogTemp, Warning, TEXT("Successfully added %s to the ruleset of %s at adjacency %i"),
							*neighbour.Value, *labelAtPosition.Value, i)
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Failed to add %s to the ruleset of %s at adjacency %i, due to duplicate"),
							*neighbour.Value, *labelAtPosition.Value, i)
					}
				}
			}
		}
	}

	return tileEntries;
}

FVector UAdjacencyBaker::IndexToCoordinates(FVector index, FVector gridSize, int cellSize)
{
	FVector xyGridSize = FVector(gridSize.X, gridSize.Y, 0);
	return (index - (xyGridSize - FVector(1, 1, 0))/2) * (cellSize * 2);
}

UTileEntryDTO* UAdjacencyBaker::GetExistingEntry(TArray<UTileEntryDTO*>& array, ULabel* label)
{
	for (int i = 0; i < array.Num(); i++)
	{
		UTileEntryDTO* entry = array[i];
		if(entry->tileRuleset->Current->Compare(label))
			return entry;
	}
	return nullptr;
}

TTuple<ULabel*, FString> UAdjacencyBaker::GetLabelAtPosition(UWorld* world, FVector worldPosition, int halfCellSize)
{
	TTuple<ULabel*, FString> toReturn;
	ULabel* airTile = NewObject<ULabel>();
	airTile->Initialize(nullptr, FQuat::Identity, FVector3d::Zero());
	toReturn.Key = airTile;
	toReturn.Value = FString("Air");
	
	TArray<AActor*> actors;
	EObjectTypeQuery staticObjects = UEngineTypes::ConvertToObjectType(ECC_WorldStatic);
	bool foundActorsAtCell = UKismetSystemLibrary::BoxOverlapActors(
		world, worldPosition, FVector(halfCellSize, halfCellSize, halfCellSize),
		{staticObjects}, nullptr,{}, actors
	);
	
	if(!foundActorsAtCell || actors.Num() == 0)
		return toReturn;
	
	if(actors.Num() > 1)
		UE_LOG(LogTemp, Warning, TEXT("Found multiple actors in cell (%f, %f, %f), picking first one found"),
			worldPosition.X, worldPosition.Y, worldPosition.Z)
				
	AActor* toConsider = actors[0];
	AStaticMeshActor* asStaticMesh = Cast<AStaticMeshActor>(toConsider);
	if(asStaticMesh == nullptr)
		return toReturn;
				
	ULabel* label = NewObject<ULabel>();
	FTransform actorTransform = asStaticMesh->GetActorTransform();
	label->Scale = actorTransform.GetScale3D();
	label->Rotation = actorTransform.GetRotation();
	label->Mesh = asStaticMesh->GetStaticMeshComponent()->GetStaticMesh();
	
	toReturn.Key = label;
	toReturn.Value = asStaticMesh->GetActorLabel();
	return toReturn;
}
