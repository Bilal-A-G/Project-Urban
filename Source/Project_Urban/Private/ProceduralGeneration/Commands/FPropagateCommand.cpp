#include "ProceduralGeneration/Commands/FPropagateCommand.h"

#include "ProceduralGeneration/EAdjacency.h"
#include "ProceduralGeneration/UCommandQueue.h"
#include "ProceduralGeneration/UGenerationModel.h"

FPropagateCommand::FPropagateCommand(FVector newTileIndex, int newNeighbourIndex)
{
	this->tileIndex = newTileIndex;
	this->neighbourIndex = newNeighbourIndex;
}

bool FPropagateCommand::Execute(UGenerationModel* model, UWorld* world, UCommandQueue* commandQueue)
{
	UE_LOG(LogTemp, Warning, TEXT("Trying to execute a propagate command"))
	bool success = model->PropagateToNeighbours(this->tileIndex, neighbourIndex, true);
	if(!success)
		return false;

	UE_LOG(LogTemp, Warning, TEXT("Propagate command success!"))
	FVector adjacentIndex = tileIndex + PUrban::ToVector(static_cast<EAdjacency>(neighbourIndex));
	model->SetColourAtIndex(adjacentIndex, FLinearColor::Yellow);
	for (int i = 0; i < static_cast<int>(EAdjacency::LAST); i++)
	{
		if(static_cast<EAdjacency>(i) == PUrban::Opposite(static_cast<EAdjacency>(neighbourIndex)))
			continue;
		UE_LOG(LogTemp, Warning, TEXT("Pushed front a propagate command"))
		commandQueue->PushFront(new FPropagateCommand(adjacentIndex, i));
	}
	return true;
}

