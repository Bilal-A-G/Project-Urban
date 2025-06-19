#include "ProceduralGeneration/UCommandQueue.h"
#include "ProceduralGeneration/UGenerationModel.h"

void UCommandQueue::PushBack(FAbstractCommand* command)
{
	queue.Push(command);
}

bool UCommandQueue::Execute(UGenerationModel* model, UWorld* world)
{
	model->ResetColours();
	FAbstractCommand* command = queue.Pop(EAllowShrinking::No);
	bool success = command->Execute(model, world, this);
	delete command;
	command = nullptr;

	return success;
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
