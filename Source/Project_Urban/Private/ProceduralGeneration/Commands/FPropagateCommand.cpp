#include "ProceduralGeneration/Commands/FPropagateCommand.h"

#include "ProceduralGeneration/UGenerationModel.h"

FPropagateCommand::FPropagateCommand(FVector newTileIndex)
{
	this->tileIndex = newTileIndex;
}

void FPropagateCommand::Execute(UGenerationModel* model, UWorld* world, UCommandQueue* commandQueue)
{
	model->PropagateToNeighbours(this->tileIndex);
	model->SetColourAtIndex(this->tileIndex, FLinearColor::Yellow);
}

