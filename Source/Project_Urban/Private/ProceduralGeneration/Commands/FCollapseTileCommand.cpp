#include "ProceduralGeneration/Commands/FCollapseTileCommand.h"

#include "ProceduralGeneration/EAdjacency.h"
#include "ProceduralGeneration/UCommandQueue.h"
#include "ProceduralGeneration/UGenerationModel.h"
#include "ProceduralGeneration/Commands/FPropagateCommand.h"

bool FCollapseTileCommand::Execute(UGenerationModel* model, UWorld* world, UCommandQueue* commandQueue)
{
	TTuple<bool, FVector> collapseData = model->CollapseRandomValidTile(world, true);
	if(!collapseData.Key)
		return false;
	
	model->SetColourAtIndex(collapseData.Value, FLinearColor::Black);
	for (int i = 0; i < static_cast<int>(EAdjacency::LAST); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pushed front a propagate command"))
		commandQueue->PushFront(new FPropagateCommand(collapseData.Value, i));
	}
	
	return true;
}
