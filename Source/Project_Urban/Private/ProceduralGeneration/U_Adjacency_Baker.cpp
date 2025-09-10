
#include "ProceduralGeneration/U_Adjacency_Baker.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProceduralGeneration/ProceduralGenerationCore.h"

TArray<UTileEntryDTO*> UAdjacencyBaker::BakeAdjacencies(UWorld* world, FGridExtentsSize grid_dimensions, int cell_size)
{
	TArray<UTileEntryDTO*> array;
	return array;
}

UTileEntryDTO* UAdjacencyBaker::GetExistingEntry(TArray<UTileEntryDTO*>& array, ULabel* label)
{
	for (int i = 0; i < array.Num(); i++)
	{
		UTileEntryDTO* entry = array[i];
		//if(entry->tileRuleset->Current->Compare(label))
			//return entry;
	}
	return nullptr;
}

TTuple<ULabel*, FString> UAdjacencyBaker::GetLabelAtPosition(UWorld* world, FWorldCoordinate worldPosition, int halfCellSize)
{
	TTuple<ULabel*, FString> toReturn;
	ULabel* airTile = NewObject<ULabel>();
	airTile->Initialize(nullptr, FQuat::Identity, FVector3d::Zero());
	toReturn.Key = airTile;
	toReturn.Value = FString("Air");
	
	TArray<AActor*> actors;
	EObjectTypeQuery staticObjects = UEngineTypes::ConvertToObjectType(ECC_WorldStatic);
	FVector world_position_vector = worldPosition.vector;
	bool foundActorsAtCell = UKismetSystemLibrary::BoxOverlapActors(
		world, world_position_vector, FVector(halfCellSize, halfCellSize, halfCellSize),
		{staticObjects}, nullptr,{}, actors
	);
	
	if(!foundActorsAtCell || actors.Num() == 0)
		return toReturn;
	
	if(actors.Num() > 1)
		UE_LOG(LogTemp, Warning, TEXT("Found multiple actors in cell (%f, %f, %f), picking first one found"),
			world_position_vector.X, world_position_vector.Y, world_position_vector.Z)
				
	AActor* toConsider = actors[0];
	AStaticMeshActor* asStaticMesh = Cast<AStaticMeshActor>(toConsider);
	if(asStaticMesh == nullptr)
		return toReturn;
				
	ULabel* label = NewObject<ULabel>();
	FTransform actorTransform = asStaticMesh->GetActorTransform();
	//label->Scale = actorTransform.GetScale3D();
	//label->Rotation = actorTransform.GetRotation();
	//label->Mesh = asStaticMesh->GetStaticMeshComponent()->GetStaticMesh();
	
	toReturn.Key = label;
	toReturn.Value = asStaticMesh->GetActorLabel();
	return toReturn;
}
