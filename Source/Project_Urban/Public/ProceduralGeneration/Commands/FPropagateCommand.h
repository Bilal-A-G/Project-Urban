#pragma once
#include "FAbstractCommand.h"

//Command to propagate ruleset constraints at a tile index to its neighbours
struct FPropagateCommand : FAbstractCommand
{
public:
	FPropagateCommand(FVector newTileIndex);
	virtual ~FPropagateCommand() override = default;
	virtual void Execute(UGenerationModel* model, UWorld* world) override;
public:
	FVector tileIndex;
};
