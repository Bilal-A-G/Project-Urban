#pragma once
#include "FModelCell.generated.h"

class UGenerationRuleset;

//This represents a single cell in the model
USTRUCT(Blueprintable)
struct FModelCell
{
	GENERATED_BODY()
public:
	FModelCell(){}
	FModelCell(TArray<UGenerationRuleset*> allPossibleRuleSets) : CandidateRuleSets(allPossibleRuleSets)
	{}
public:
	UPROPERTY()
	TArray<UGenerationRuleset*> CandidateRuleSets;
};
