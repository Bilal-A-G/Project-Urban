#include "..\..\..\Public\ProceduralGeneration\Ruleset\F_Generation_Ruleset_Holder.h"

void FGenerationRulesetHolder::AddRuleset(UGenerationRuleset* ruleset)
{
	rulesets_.Add(ruleset);
}

void FGenerationRulesetHolder::AddAirRuleset(UGenerationRuleset* air_ruleset)
{
	air_ruleset_ = air_ruleset;
}

bool FGenerationRulesetHolder::TryCollapseRandom()
{
	bool has_air_ruleset = air_ruleset_ != nullptr;
	if(rulesets_.Num() == 0 && !has_air_ruleset)
		return false;
	int random_index = rand() % (rulesets_.Num() + has_air_ruleset ? 1 : 0);
	UGenerationRuleset* chosen_ruleset;
	if(random_index >= rulesets_.Num())
		chosen_ruleset = air_ruleset_;
	else
		chosen_ruleset = rulesets_[random_index];

	rulesets_.Empty();
	if(chosen_ruleset != air_ruleset_)
		rulesets_.Add(chosen_ruleset);
	
	return true;
}

bool FGenerationRulesetHolder::TryMakeConsistentWith(FGenerationRulesetHolder& other, EAdjacency to_other_adjacency)
{
	return UGenerationRuleset::RemoveInconsistentLabels(rulesets_, other.rulesets_, to_other_adjacency);
}

bool FGenerationRulesetHolder::IsEqualTo(FGenerationRulesetHolder& other)
{
	TArray<UGenerationRuleset*> other_rulesets = other.rulesets_;
	if(other_rulesets.Num() != rulesets_.Num())
		return false;
	if(other.air_ruleset_ != air_ruleset_)
		return false;
	bool elements_match = true;
	for(int i = 0; i < other_rulesets.Num(); i++)
	{
		bool element_found = false;
		UGenerationRuleset* other_ruleset = other_rulesets[i];
		//Need second for loop in case the elements are not in the exact same order
		for(int v = 0; v < rulesets_.Num(); v++)
		{
			UGenerationRuleset* current_ruleset = rulesets_[v];
			if(current_ruleset == other_ruleset)
			{
				element_found = true;
				break;
			}
		}
		elements_match = elements_match && element_found;
	}
	return elements_match;
}

bool FGenerationRulesetHolder::IsSubSetOf(FGenerationRulesetHolder& other)
{
	if(air_ruleset_ != other.air_ruleset_)
		return false;
	
	bool all_elements_match = true;
	TArray<UGenerationRuleset*> other_rulesets = other.rulesets_;
	for(int i = 0; i < rulesets_.Num(); i++)
	{
		UGenerationRuleset* current_ruleset = rulesets_[i];
		bool elements_match = false;
		for(int v = 0; v < other_rulesets.Num())
		{
			UGenerationRuleset* current_other_ruleset = other_rulesets[v];
			if(current_ruleset == current_other_ruleset)
			{
				elements_match = true;
				break;
			}
		}
		all_elements_match = all_elements_match && elements_match;	
	}

	return all_elements_match;
}

bool FGenerationRulesetHolder::IsEmpty()
{
	return air_ruleset_ == nullptr && rulesets_.Num() == 0;
}

UGenerationRuleset* FGenerationRulesetHolder::GetAirRuleset()
{
	return air_ruleset_;
}


