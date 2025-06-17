#include "ProceduralGeneration/UCommandQueue.h"

void UCommandQueue::PushBack(FAbstractCommand* command)
{
	queue.Push(command);
}

void UCommandQueue::Execute(UGenerationModel* model, UWorld* world)
{
	FAbstractCommand* command = queue.Pop(EAllowShrinking::No);
	command->Execute(model, world, this);
	delete command;
	command = nullptr;
}

bool UCommandQueue::IsEmpty()
{
	return queue.IsEmpty();
}

void UCommandQueue::Clear()
{
	for (int i = 0; i < queue.Num(); i++)
	{
		delete queue[i];
		queue[i] = nullptr;
	}

	queue.Empty();
}
