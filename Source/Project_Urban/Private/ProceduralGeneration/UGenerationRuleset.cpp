#include "ProceduralGeneration/UGenerationRuleset.h"

bool UGenerationRuleset::CheckConsistency(const UGenerationRuleset* other, EAdjacency adjacency) const
{
	return Adjacencies[adjacency].Contains(other->Current) &&
		other->Adjacencies[PUrban::Opposite(adjacency)].Contains(Current);
}

void UGenerationRuleset::RemoveInconsistentLabels(const UGenerationRuleset* current, TArray<UGenerationRuleset*>& array,
	EAdjacency adjacency)
{
	for (UGenerationRuleset* currentArrayRuleset: array)
	{
		if(!current->CheckConsistency(currentArrayRuleset, adjacency))
			array.Remove(currentArrayRuleset);
	}
}
