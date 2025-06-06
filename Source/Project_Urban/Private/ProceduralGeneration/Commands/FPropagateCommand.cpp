#include "ProceduralGeneration/Commands/FPropagateCommand.h"

#include "ProceduralGeneration/UGenerationModel.h"

FPropagateCommand::FPropagateCommand(FVector newTileIndex)
{
	this->tileIndex = newTileIndex;
}

void FPropagateCommand::Execute(UGenerationModel* model, UWorld* world)
{
	model->PropagateToNeighbours(this->tileIndex);
}

