#include "ProceduralGeneration/Commands/FCollapseTileCommand.h"

#include "ProceduralGeneration/EAdjacency.h"
#include "ProceduralGeneration/UCommandQueue.h"
#include "ProceduralGeneration/UGenerationModel.h"
#include "ProceduralGeneration/Commands/FPropagateCommand.h"

FCollapseTileCommand::FCollapseTileCommand(FVector newTileIndex)
{
	this->tileIndex = newTileIndex;
}

void FCollapseTileCommand::Execute(UGenerationModel* model, UWorld* world, UCommandQueue* commandQueue)
{
	model->CollapseTile(this->tileIndex, world);
	model->SetColourAtIndex(this->tileIndex, FLinearColor::Black);
	for (int i = 0; i < static_cast<int>(EAdjacency::LAST); i++)
	{
		commandQueue->PushBack(new FPropagateCommand(this->tileIndex));
	}
}
