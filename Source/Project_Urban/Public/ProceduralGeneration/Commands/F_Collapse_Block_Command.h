#pragma once
#include "F_Abstract_Command.h"
#include "ProceduralGeneration/UCommandQueue.h"
#include "ProceduralGeneration/Model/U_Generation_Model_Impl.h"

//Command to collapse a block, preparing for the collapse tile and propagate commands
struct FCollapseBlockCommand : FAbstractCommand
{
public:
	FCollapseBlockCommand(FGridArrayIndexCoordinate new_block_bottom_left, FGridArrayIndexCoordinate new_block_top_right); 
	virtual ~FCollapseBlockCommand() override = default;
	virtual bool Execute(UGenerationModelImpl* model, UWorld* world, UCommandQueue* command_queue) override;
private:
	FGridArrayIndexCoordinate block_bottom_left_;
	FGridArrayIndexCoordinate block_top_right_;
};
