#pragma once

#include "CoreMinimal.h"
#include "U_Generation_Ruleset.h"
#include "UObject/Object.h"
#include "F_Generation_Ruleset_Holder.generated.h"

USTRUCT()
struct PROJECT_URBAN_API FGenerationRulesetHolder 
{
	GENERATED_BODY()
public:
	void AddRuleset(UGenerationRuleset* ruleset);
	void AddAirRuleset(UGenerationRuleset* air_ruleset);
	bool TryCollapseRandom();
	bool TryMakeConsistentWith(FGenerationRulesetHolder& other, EAdjacency to_other_adjacency);
	bool IsEqualTo(FGenerationRulesetHolder& other);
	bool IsSubSetOf(FGenerationRulesetHolder& other);
	bool IsEmpty();

	UGenerationRuleset* GetAirRuleset();
private:
	UPROPERTY()
	TArray<UGenerationRuleset*> rulesets_;
	UPROPERTY()
	UGenerationRuleset* air_ruleset_;
};
