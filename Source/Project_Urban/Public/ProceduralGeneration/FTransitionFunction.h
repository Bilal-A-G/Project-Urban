#pragma once
#include "PUrbanUtils.h"
#include "FTransitionFunction.generated.h"
struct FLabel;
struct FGenerationRuleset;


USTRUCT(Blueprintable)
struct FTransitionFunction
{
	GENERATED_BODY()
public:
	bool CheckConsistency(FLabel* current, FLabel* other, PUrban::Adjacency adjacency);
	void RemoveInconsistentLabels(FLabel* current, TArray<FLabel*>& array, PUrban::Adjacency adjacency);
public:
	TArray<FGenerationRuleset*> RuleSets;
};