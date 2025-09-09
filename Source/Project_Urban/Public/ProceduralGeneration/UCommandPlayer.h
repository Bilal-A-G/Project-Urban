#pragma once

#include "CoreMinimal.h"
#include "UCommandQueue.h"
#include "Model/U_Generation_Model_Impl.h"
#include "UObject/Object.h"
#include "UCommandPlayer.generated.h"

/**
 * This class acts as an interface between the core and the command queue
 * This handles higher level operations like playing, pausing, stepping, etc.
 */
UCLASS()
class PROJECT_URBAN_API UCommandPlayer : public UObject
{
	GENERATED_BODY()
public:
	void Init(UWorld* worldInstance, FVector gridDimensions, UGenerationModelImpl* modelInstance);
	void Clear();
	void Tick(float deltaTime);
	void TogglePlay(){isPlaying = !isPlaying;}
	//Speed defined in how many commands to execute per second, when play is true
	void SetPlaybackSpeed(float speed){playbackSpeed = speed;}
	void StepForward();
private:
	bool isPlaying;
	float playbackSpeed = 1.0f;
	int timesCollapsed;
	int maxTimesCollapsedX;
	int maxTimesCollapsedY;
	FVector blockSize;
	float timeSinceLastPlay;
	UPROPERTY()
	UWorld* world;
	UPROPERTY()
	UGenerationModelImpl* model;
	FVector gridSize;
	UPROPERTY()
	UCommandQueue* commandQueue;
};
