#pragma once
#include "F_Abstract_Command.h"

//Command to collapse a tile at a specific index
struct FCollapseTileCommand : FAbstractCommand
{
public:
	FCollapseTileCommand() = default;
	virtual ~FCollapseTileCommand() override = default;
	virtual bool Execute(UGenerationModelImpl* model, UWorld* world, UCommandQueue* command_queue) override;
};
