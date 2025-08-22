#include "ProceduralGeneration/UCommandPlayer.h"

#include "ProceduralGeneration/UCommandQueue.h"
#include "ProceduralGeneration/Commands/FCollapseBlockCommand.h"

void UCommandPlayer::Init(UWorld* worldInstance, FVector gridDimensions, UGenerationModel* modelInstance)
{
	commandQueue = NewObject<UCommandQueue>();
	gridSize = gridDimensions;
	world = worldInstance;
	model = modelInstance;
	timeSinceLastPlay = worldInstance->TimeSince(0);

	//TODO, make this configurable
	blockSize = FVector(3, 3, 1);
	maxTimesCollapsedX = gridDimensions.X - 2;
	maxTimesCollapsedY = gridDimensions.Y - 2;
}

void UCommandPlayer::Clear()
{
	commandQueue->Clear();
	timesCollapsed = 0;	
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
	if(commandQueue->IsEmpty())
	{
		int xOffset = timesCollapsed % maxTimesCollapsedX;
		int yOffset = timesCollapsed / maxTimesCollapsedX;
		if(timesCollapsed >= maxTimesCollapsedX * maxTimesCollapsedY)
		{
			commandQueue->Clear();
			UE_LOG(LogTemp, Warning, TEXT("Generation is over, collapsed max times!"));
			return;
		}
		UE_LOG(LogTemp, Warning, TEXT("Bottom left index = (%i, %i, %i)"), xOffset, yOffset, 0);
		commandQueue->PushBack(new FCollapseBlockCommand(FVector(xOffset, yOffset, 0), blockSize));
		timesCollapsed++;
		commandQueue->Execute(model, world);
	}
	else
	{
		commandQueue->Execute(model, world);
	}
}
