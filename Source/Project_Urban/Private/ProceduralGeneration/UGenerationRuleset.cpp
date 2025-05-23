#include "ProceduralGeneration/UGenerationRuleset.h"

bool UGenerationRuleset::CheckConsistency(const UGenerationRuleset* other, EAdjacency adjacency) const
{
	TArray<ULabel*> currentAdjacencyLabels = GetAdjacencyValuesFromKey(adjacency);
	TArray<ULabel*> otherOppositeAdjacencyLabels = other->GetAdjacencyValuesFromKey(PUrban::Opposite(adjacency));
	
	return currentAdjacencyLabels.Contains(other->Current) &&
		otherOppositeAdjacencyLabels.Contains(Current);
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

const TArray<ULabel*>& UGenerationRuleset::GetAdjacencyValuesFromKey(EAdjacency adjacency) const
{
	for (const FAdjacencyWrapper& wrapper : Adjacencies)
	{
		if(wrapper.key == adjacency)
			return wrapper.values;
	}
	
	return NullList;
}

void UGenerationRuleset::UpdateAdjacencyValue(EAdjacency adjacency, int index, ULabel* label)
{
	for (FAdjacencyWrapper& wrapper : Adjacencies)
	{
		if(wrapper.key != adjacency)
			continue;

		wrapper.values[index] = label;
		return;
	}
}
