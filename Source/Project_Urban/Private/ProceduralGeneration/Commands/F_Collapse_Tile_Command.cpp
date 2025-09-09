#include "ProceduralGeneration/Commands/F_Collapse_Tile_Command.h"

#include "ProceduralGeneration/EAdjacency.h"
#include "ProceduralGeneration/UCommandQueue.h"
#include "ProceduralGeneration/Commands/F_Propagate_Command.h"

bool FCollapseTileCommand::Execute(UGenerationModelImpl* model, UWorld* world, UCommandQueue* command_queue)
{
	TTuple<bool, FGridArrayIndexCoordinate> collapse_data = model->CollapseRandomValidTile();
	if(!collapse_data.Key)
		return false;

	//TODO, visualize results
	//model->SetColourAtIndex(collapseData.Value, FLinearColor::Black);
	for (int i = 0; i < static_cast<int>(EAdjacency::LAST); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pushed front a propagate command"))
		command_queue->PushFront(new FPropagateCommand(collapse_data.Value, i));
	}
	
	return true;
}
