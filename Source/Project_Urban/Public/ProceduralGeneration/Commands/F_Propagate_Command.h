#pragma once
#include "F_Abstract_Command.h"

//Command to propagate ruleset constraints at a tile index to its neighbours
struct FPropagateCommand : FAbstractCommand
{
public:
	FPropagateCommand(FGridArrayIndexCoordinate new_tile_index, EAdjacency new_neighbour_adjacency);
	virtual ~FPropagateCommand() override = default;
	virtual bool Execute(UGenerationModelImpl* model, UWorld* world, UCommandQueue* command_queue) override;
public:
	FGridArrayIndexCoordinate tile_index_;
	EAdjacency neighbour_adjacency_;
};
