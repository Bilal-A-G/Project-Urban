
#include "ProceduralGeneration/Ruleset/U_Generation_Ruleset.h"
#include "..\..\..\Public\ProceduralGeneration\U_Label.h"

void UGenerationRuleset::PostInitProperties()
{
	Super::PostInitProperties();
	for (uint8 i = 0; i < static_cast<uint8>(EAdjacency::LAST); i++)
	{
		FAdjacencyWrapper wrapper = FAdjacencyWrapper();
		wrapper.key_ = static_cast<EAdjacency>(i);
		wrapper.values_ = TArray<ULabel*>();
		adjacencies_.Add(wrapper);
	}
}

bool UGenerationRuleset::CheckConsistency(const UGenerationRuleset* other, EAdjacency adjacency) const
{
	TArray<ULabel*> current_adjacency_lables = GetAdjacencyValuesFromKey(adjacency);
	TArray<ULabel*> other_opposite_labels = other->GetAdjacencyValuesFromKey(PUrban::Opposite(adjacency));

	FString name = "Air";
	if (current_->mesh_ != nullptr)
		name = current_->mesh_->GetName();
	
	UE_LOG(LogTemp, Warning, TEXT("Current adjacencies = %i, other opposite adjacencies = %i %s"),
		current_adjacency_lables.Num(), other_opposite_labels.Num(), *name)
	
	return ArrayContains(current_adjacency_lables, other->current_) &&
		ArrayContains(other_opposite_labels, current_);
}

bool UGenerationRuleset::RemoveInconsistentLabels(const UGenerationRuleset* current, TArray<UGenerationRuleset*>& array,
	EAdjacency adjacency)
{
	TArray<UGenerationRuleset*> consistent_rulesets;
	for (UGenerationRuleset* current_array_ruleset: array)
	{
		if(!current->CheckConsistency(current_array_ruleset, adjacency))
			continue;
		consistent_rulesets.Add(current_array_ruleset);
	}

	if (array.Num() == consistent_rulesets.Num())
		return false;
	
	array = consistent_rulesets;
	return true;
}	

bool UGenerationRuleset::RemoveInconsistentLabels(TArray<UGenerationRuleset*>& current,
	TArray<UGenerationRuleset*>& array, EAdjacency adjacency)
{
	TArray<UGenerationRuleset*> consistent_rulesets;
	for (UGenerationRuleset* current_array_ruleset: array)
	{
		bool consistent = false;
		for (UGenerationRuleset* current_compare : current)
		{
			if(current_compare->CheckConsistency(current_array_ruleset, adjacency))
				consistent = true;
		}
		if(!consistent)
			continue;
		
		consistent_rulesets.Add(current_array_ruleset);
	}
	if(array.Num() == consistent_rulesets.Num())
		return false;
	
	array = consistent_rulesets;
	return true;
}

const TArray<ULabel*> UGenerationRuleset::GetAdjacencyValuesFromKey(EAdjacency adjacency) const
{
	TArray<ULabel*> to_return = TArray<ULabel*>();
	
	for (const FAdjacencyWrapper& wrapper : adjacencies_)
	{
		if(wrapper.key_ != adjacency)
			continue;

		for (ULabel* label : wrapper.values_)
		{
			if(label == nullptr)
				continue;
			to_return.Add(label);
		}
	}	
	
	return to_return;
}

void UGenerationRuleset::UpdateAdjacencyValue(EAdjacency adjacency, int index, ULabel* label)
{
	for (FAdjacencyWrapper& wrapper : adjacencies_)
	{
		if(wrapper.key_ != adjacency)
			continue;

		int num_values = wrapper.values_.Num(); 
		if (num_values <= index)
		{
			int difference = index - num_values;
			for (int i = 0; i < difference; i ++)
			{
				UE_LOG(LogTemp, Warning, TEXT("Added null value at index %i to pad array"), i)
				wrapper.values_.Add(nullptr);
			}
			
			UE_LOG(LogTemp, Warning, TEXT("Added value at index %i"), index)
			wrapper.values_.Add(label);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Updated value at index %i"), index)
			wrapper.values_[index] = label;
		}
		return;
	}
}

bool UGenerationRuleset::AddAdjacencyEntry(EAdjacency adjacency, ULabel* label)
{
	for (FAdjacencyWrapper& wrapper : adjacencies_)
	{
		if(wrapper.key_ != adjacency || ArrayContains(wrapper.values_, label))
			continue;
		
		wrapper.values_.Add(label);
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
