#pragma once
#include "FAbstractCommand.h"

struct FCollapseTileCommand : FAbstractCommand
{
public:
	FCollapseTileCommand(FVector newTileIndex);
	virtual ~FCollapseTileCommand() override = default;
	virtual void Execute(UGenerationModel* model, UWorld* world) override;
public:
	FVector tileIndex;
};
