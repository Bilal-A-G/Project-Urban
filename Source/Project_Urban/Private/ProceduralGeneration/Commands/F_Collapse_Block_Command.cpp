#include "ProceduralGeneration/Commands/F_Collapse_Block_Command.h"
#include "ProceduralGeneration/UCommandQueue.h"
#include "..\..\..\Public\ProceduralGeneration\Commands\F_Collapse_Tile_Command.h"
#include "ProceduralGeneration/Model/U_Generation_Model_Impl.h"

FCollapseBlockCommand::FCollapseBlockCommand(FGridArrayIndexCoordinate new_block_bottom_left, FGridArrayIndexCoordinate new_block_top_right)
{
	block_bottom_left_ = new_block_bottom_left;
	block_top_right_ = new_block_top_right;
}

bool FCollapseBlockCommand::Execute(UGenerationModelImpl* model, UWorld* world, UCommandQueue* command_queue)
{
	bool collapse_block_result = model->CollapseBlock(block_bottom_left_, block_top_right_);
	if(!collapse_block_result)
		return false;
	FVector block_size_vector = block_top_right_.vector - block_bottom_left_.vector;
	int num_block_tiles = block_size_vector.X * block_size_vector.Y * block_size_vector.Z;
	for(int i = 0; i < num_block_tiles; i++)
	{
		command_queue->PushBack(new FCollapseTileCommand());	
	}
	return true;
}
