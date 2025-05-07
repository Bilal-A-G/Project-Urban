#include "ProceduralGeneration/FTransitionFunction.h"
#include "ProceduralGeneration/FGenerationRuleset.h"

bool FTransitionFunction::CheckConsistency(FLabel* current, FLabel* other, PUrban::Adjacency adjacency)
{
	FGenerationRuleset* currentRuleset = nullptr;
	FGenerationRuleset* otherRuleset = nullptr;

	for (FGenerationRuleset* ruleSet : RuleSets)
	{
		if(ruleSet->Current == current)
			currentRuleset = ruleSet;
		if(ruleSet->Current == other)
			otherRuleset = ruleSet;

		if(currentRuleset != nullptr && otherRuleset != nullptr)
			break;
	}

	
	return currentRuleset->Adjacencies[adjacency].Contains(other) &&
		otherRuleset->Adjacencies[PUrban::Opposite(adjacency)].Contains(current);
}

void FTransitionFunction::RemoveInconsistentLabels(FLabel* current, TArray<FLabel*>& array, PUrban::Adjacency adjacency)
{
	for (int i = 0; i < array.Num(); i++)
	{
		if(!CheckConsistency(current, array[i], adjacency))
			array.RemoveAt(i);
	}
}
