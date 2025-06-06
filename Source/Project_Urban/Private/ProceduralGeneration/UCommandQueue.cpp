#include "ProceduralGeneration/UCommandQueue.h"

void UCommandQueue::PushBack(FAbstractCommand command)
{
	queue.Push(command);
}

void UCommandQueue::Execute(UGenerationModel* model, UWorld* world)
{
	FAbstractCommand command = queue.Pop(EAllowShrinking::No);
	command.Execute(model, world);
}
