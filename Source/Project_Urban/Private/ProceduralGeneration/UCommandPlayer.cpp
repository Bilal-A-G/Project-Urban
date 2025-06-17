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
	if(commandQueue->IsEmpty())
	{
		commandQueue->PushBack(new FCollapseTileCommand(FVector(rand() % static_cast<int>(gridSize.X),
		rand() % static_cast<int>(gridSize.Y), rand() % static_cast<int>(gridSize.Z))));
		UE_LOG(LogTemp, Warning, TEXT("Pushing back a collapse command!"))
	}

	UE_LOG(LogTemp, Warning, TEXT("Executing first command in queue"))
	commandQueue->Execute(model, world);
}
