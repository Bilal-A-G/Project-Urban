#include "ProceduralGeneration/UCommandPlayer.h"

#include "ProceduralGeneration/UCommandQueue.h"
#include "ProceduralGeneration/Commands/FCollapseTileCommand.h"

void UCommandPlayer::Init(UWorld* worldInstance, FVector gridDimensions, UGenerationModel* modelInstance)
{
	commandQueue = NewObject<UCommandQueue>();
	gridSize = gridDimensions;
	world = worldInstance;
	model = modelInstance;
	timeSinceLastPlay = worldInstance->TimeSince(0);
}

void UCommandPlayer::Clear()
{
	commandQueue->Clear();
}

void UCommandPlayer::Tick(float deltaTime)
{
	if(!isPlaying)
		return;

	if(world->TimeSince(0) < timeSinceLastPlay + 1.0f/playbackSpeed)
		return;

	UE_LOG(LogTemp, Warning, TEXT("Running a command!"));
	timeSinceLastPlay = world->TimeSince(0);
	this->StepForward();
}

void UCommandPlayer::StepForward()
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
