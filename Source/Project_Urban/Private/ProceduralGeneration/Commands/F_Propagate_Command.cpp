#include "ProceduralGeneration/Commands/F_Propagate_Command.h"

#include "ProceduralGeneration/EAdjacency.h"
#include "ProceduralGeneration/UCommandQueue.h"

FPropagateCommand::FPropagateCommand(FGridArrayIndexCoordinate new_tile_index, EAdjacency new_neighbour_adjacency)
{
	tile_index_ = new_tile_index;
	neighbour_adjacency_ = new_neighbour_adjacency;
}

bool FPropagateCommand::Execute(UGenerationModelImpl* model, UWorld* world, UCommandQueue* command_queue)
{
	UE_LOG(LogTemp, Warning, TEXT("Trying to execute a propagate command"))
	bool success = model->MakeNeighbourPossibilitiesConsistent(tile_index_, neighbour_adjacency_);
	if(!success)
		return false;

	UE_LOG(LogTemp, Warning, TEXT("Propagate command success!"))
	FVector adjacency_index_vector = tile_index_.vector + PUrban::ToVector(neighbour_adjacency_);
	FGridArrayIndexCoordinate adjacency_grid_index = FGridArrayIndexCoordinate(adjacency_index_vector);
	//TODO, make it so we can have a visualization
	//model->SetColourAtIndex(adjacentIndex, FLinearColor::Yellow);
	for (int i = 0; i < static_cast<int>(EAdjacency::LAST); i++)
	{
		EAdjacency asAdjacency = static_cast<EAdjacency>(i);
		if(asAdjacency == PUrban::Opposite(neighbour_adjacency_))
			continue;
		UE_LOG(LogTemp, Warning, TEXT("Pushed front a propagate command"))
		command_queue->PushFront(new FPropagateCommand(adjacency_grid_index, asAdjacency));
	}
	return true;
}

