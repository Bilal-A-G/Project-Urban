#pragma once
#include "FAbstractCommand.h"

//Command to collapse a tile at a specific index
struct FCollapseTileCommand : FAbstractCommand
{
public:
	FCollapseTileCommand() = default;
	virtual ~FCollapseTileCommand() override = default;
	virtual bool Execute(UGenerationModel* model, UWorld* world, UCommandQueue* commandQueue) override;
};
