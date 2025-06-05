#include "ProceduralGeneration/Commands/FCollapseTileCommand.h"
#include "ProceduralGeneration/UGenerationModel.h"

FCollapseTileCommand::FCollapseTileCommand(FVector newTileIndex)
{
	this->tileIndex = newTileIndex;
}

void FCollapseTileCommand::Execute(UGenerationModel* model, UWorld* world)
{
	model->CollapseTile(this->tileIndex, world);
}
