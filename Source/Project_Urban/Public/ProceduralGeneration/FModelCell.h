#pragma once
#include "FLabel.h"
#include "FModelCell.generated.h"

USTRUCT(Blueprintable)
struct FModelCell
{
	GENERATED_BODY()
public:
	FModelCell(TArray<FLabel> allPossibleLabels) : CandidateLabels(allPossibleLabels)
	{}
	FModelCell(){}
public:
	TArray<FLabel> CandidateLabels;
};
