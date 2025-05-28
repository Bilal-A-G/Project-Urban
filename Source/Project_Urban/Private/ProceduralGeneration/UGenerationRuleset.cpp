#include "ProceduralGeneration/UGenerationRuleset.h"

void UGenerationRuleset::PostInitProperties()
{
	Super::PostInitProperties();
	for (uint8 i = 0; i < static_cast<uint8>(EAdjacency::LAST) - 2; i++)
	{
		FAdjacencyWrapper wrapper = FAdjacencyWrapper();
		wrapper.key = static_cast<EAdjacency>(i);
		wrapper.values = TArray<ULabel*>();
		Adjacencies.Add(wrapper);
	}
}

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

const TArray<ULabel*> UGenerationRuleset::GetAdjacencyValuesFromKey(EAdjacency adjacency) const
{
	TArray<ULabel*> toReturn = TArray<ULabel*>();
	
	for (const FAdjacencyWrapper& wrapper : Adjacencies)
	{
		if(wrapper.key != adjacency)
			continue;

		for (ULabel* label : wrapper.values)
		{
			if(label == nullptr)
				continue;
			toReturn.Add(label);
		}
	}	
	
	return toReturn;
}

void UGenerationRuleset::UpdateAdjacencyValue(EAdjacency adjacency, int index, ULabel* label)
{
	for (FAdjacencyWrapper& wrapper : Adjacencies)
	{
		if(wrapper.key != adjacency)
			continue;

		int numValues = wrapper.values.Num(); 
		if (numValues <= index)
		{
			int difference = index - numValues;
			for (int i = 0; i < difference; i ++)
			{
				UE_LOG(LogTemp, Warning, TEXT("Added null value at index %i to pad array"), i)
				wrapper.values.Add(nullptr);
			}
			
			UE_LOG(LogTemp, Warning, TEXT("Added value at index %i"), index)
			wrapper.values.Add(label);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Updated value at index %i"), index)
			wrapper.values[index] = label;
		}
		return;
	}
}
