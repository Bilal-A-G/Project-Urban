#pragma once
#include "FAbstractCommand.h"

//Command to collapse a block, preparing for the collapse tile and propagate commands
struct FCollapseBlockCommand : FAbstractCommand
{
public:
	FCollapseBlockCommand(FVector newBlockBottomLeft, FVector newBlockSize); 
	virtual ~FCollapseBlockCommand() override = default;
	virtual bool Execute(UGenerationModel* model, UWorld* world, UCommandQueue* commandQueue) override;
private:
	FVector blockBottomLeft;
	FVector blockSize;
};
