#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UCommandPlayer.generated.h"

class UGenerationModel;
class UCommandQueue;
/**
 * This class acts as an interface between the core and the command queue
 * This handles higher level operations like playing, pausing, stepping, etc.
 */
UCLASS()
class PROJECT_URBAN_API UCommandPlayer : public UObject
{
	GENERATED_BODY()
public:
	void Init();
	void Clear();
	void StepForward(UGenerationModel* model, UWorld* world, FVector gridSize);
private:
	UPROPERTY()
	UCommandQueue* commandQueue;
};
