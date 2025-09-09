#pragma once
#include "Ruleset\F_Generation_Ruleset_Holder.h"
#include "F_Model_Cell.generated.h"

//This represents a single cell in the model
USTRUCT(Blueprintable)
struct FModelCell
{
	GENERATED_BODY()
public:
	FModelCell() = default; 
	FModelCell(FGenerationRulesetHolder rulesets) : rulesets_(rulesets){}
public:
	UPROPERTY()
	FGenerationRulesetHolder rulesets_;
};
