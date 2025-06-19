#include "ProceduralGeneration/UCommandPlayer.h"
#include "ProceduralGeneration/UCommandQueue.h"
#include "ProceduralGeneration/Commands/FCollapseTileCommand.h"

void UCommandPlayer::Init()
{
	commandQueue = NewObject<UCommandQueue>();
}

void UCommandPlayer::Clear()
{
	commandQueue->Clear();
}

void UCommandPlayer::StepForward(UGenerationModel* model, UWorld* world, FVector gridSize)
{
	bool success = false;
	//This might crash us, TODO, implement a timeout after a max number of attempts
	while (!success && !commandQueue->IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Executing first command in queue"))
		success = commandQueue->Execute(model, world);
	}
	if(!success && commandQueue->IsEmpty())
	{
		commandQueue->PushBack(new FCollapseTileCommand());
		UE_LOG(LogTemp, Warning, TEXT("Pushing back a collapse command!"))
		commandQueue->Execute(model, world);
	}
}
