#pragma once
#include "FAbstractCommand.h"

//Command to propagate ruleset constraints at a tile index to its neighbours
struct FPropagateCommand : FAbstractCommand
{
public:
	FPropagateCommand(FVector newTileIndex, int newNeighbourIndex);
	virtual ~FPropagateCommand() override = default;
	virtual bool Execute(UGenerationModel* model, UWorld* world, UCommandQueue* commandQueue) override;
public:
	FVector tileIndex;
	int neighbourIndex;
};
