#include "ProceduralGeneration/UGenerationRuleset.h"

#include "ProceduralGeneration/ULabel.h"

void UGenerationRuleset::PostInitProperties()
{
	Super::PostInitProperties();
	for (uint8 i = 0; i < static_cast<uint8>(EAdjacency::LAST); i++)
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

	UE_LOG(LogTemp, Warning, TEXT("Current adjacencies = %i, other opposite adjacencies = %i %s"),
		currentAdjacencyLabels.Num(), otherOppositeAdjacencyLabels.Num(), *Current->Mesh->GetName())
	
	return ArrayContains(currentAdjacencyLabels, other->Current) &&
		ArrayContains(otherOppositeAdjacencyLabels, Current);
}

void UGenerationRuleset::RemoveInconsistentLabels(const UGenerationRuleset* current, TArray<UGenerationRuleset*>& array,
	EAdjacency adjacency)
{
	TArray<UGenerationRuleset*> consistentRulesets;
	for (UGenerationRuleset* currentArrayRuleset: array)
	{
		if(!current->CheckConsistency(currentArrayRuleset, adjacency))
			continue;
		consistentRulesets.Add(currentArrayRuleset);
	}

	array = consistentRulesets;
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

bool UGenerationRuleset::AddAdjacencyEntry(EAdjacency adjacency, ULabel* label)
{
	for (FAdjacencyWrapper& wrapper : Adjacencies)
	{
		if(wrapper.key != adjacency || wrapper.values.Contains(label))
			continue;
		
		wrapper.values.Add(label);
		return true;
	}

	return false;
}

bool UGenerationRuleset::ArrayContains(TArray<ULabel*> array, ULabel* val) const
{
	for (int i = 0; i < array.Num(); i++)
	{
		if(array[i]->Compare(val))
			return true;
	}
	return false;
}
