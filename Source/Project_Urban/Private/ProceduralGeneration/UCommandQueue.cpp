#include "ProceduralGeneration/UCommandQueue.h"
#include "ProceduralGeneration/UGenerationModel.h"

void UCommandQueue::PushBack(FAbstractCommand* command)
{
	queue.Push(command);
}

void UCommandQueue::PushFront(FAbstractCommand* command)
{
	queue.Insert(command, 0);
}

bool UCommandQueue::Execute(UGenerationModel* model, UWorld* world)
{
	model->ResetColours();
	FAbstractCommand* command = queue[0];
	bool success = command->Execute(model, world, this);
	queue.RemoveAt(0, EAllowShrinking::No);

	UE_LOG(LogTemp, Warning, TEXT("The queue contains %i commands"), queue.Num());
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
