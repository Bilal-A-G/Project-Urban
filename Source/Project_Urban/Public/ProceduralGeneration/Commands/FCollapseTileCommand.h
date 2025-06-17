#pragma once
#include "FAbstractCommand.h"

//Command to collapse a tile at a specific index
struct FCollapseTileCommand : FAbstractCommand
{
public:
	FCollapseTileCommand(FVector newTileIndex);
	virtual ~FCollapseTileCommand() override = default;
	virtual void Execute(UGenerationModel* model, UWorld* world, UCommandQueue* commandQueue) override;
public:
	FVector tileIndex;
};
