#include "ProceduralGeneration/Commands/FCollapseBlockCommand.h"

#include "ProceduralGeneration/UCommandQueue.h"
#include "ProceduralGeneration/UGenerationModel.h"
#include "ProceduralGeneration/Commands/FCollapseTileCommand.h"

FCollapseBlockCommand::FCollapseBlockCommand(FVector newBlockBottomLeft, FVector newBlockSize)
{
	blockBottomLeft = newBlockBottomLeft;
	blockSize = newBlockSize;
}

bool FCollapseBlockCommand::Execute(UGenerationModel* model, UWorld* world, UCommandQueue* commandQueue)
{
	bool collapseBlockResult = model->CollapseBlock(blockBottomLeft, blockSize, true);
	if(!collapseBlockResult)
		return false;

	int numBlockTiles = blockSize.X * blockSize.Y * blockSize.Z;
	for(int i = 0; i < numBlockTiles; i++)
	{
		commandQueue->PushBack(new FCollapseTileCommand());	
	}
	return true;
}
