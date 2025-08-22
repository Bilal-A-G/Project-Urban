#pragma once

#include "CoreMinimal.h"
#include "Commands/FAbstractCommand.h"
#include "UObject/Object.h"
#include "UCommandQueue.generated.h"

/**
 * This class simply encapsulates a queue with commands in it
 * Used as an intermediary between the core and the model, so we can defer execution of
 * operations until a later date and track what was changed
 */
UCLASS()
class PROJECT_URBAN_API UCommandQueue : public UObject
{
	GENERATED_BODY()
public:
	//TODO, use move semantics here so avoid copying, these added commands belong to the queue after they have been added
	void PushBack(FAbstractCommand* command);
	void PushFront(FAbstractCommand* command);
	//Pop the first command off the queue, and execute it, if execution fails, it will return false
	bool Execute(UGenerationModel* model, UWorld* world);
	bool IsEmpty();
	void Clear();
private:
	TArray<FAbstractCommand*> queue;
};
