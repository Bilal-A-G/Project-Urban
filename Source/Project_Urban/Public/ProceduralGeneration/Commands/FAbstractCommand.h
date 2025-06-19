#pragma once

class UCommandQueue;
class UGenerationModel;

/**
 * This class that represents a command in the command queue, needs to have a concrete implementation defined.
 * Implemented in pure C++ so I can have pure virtual functions without a heap allocation.
 * Is a struct because it is essentially just a data structure
 */
struct PROJECT_URBAN_API FAbstractCommand
{
public:
	virtual ~FAbstractCommand() = default;
	//Impossible to call, just there so we don't have to use pointers
	virtual bool Execute(UGenerationModel* model, UWorld* world, UCommandQueue* commandQueue){ return false;};
protected:
	//You can't instantiate this
	FAbstractCommand() = default;
};
