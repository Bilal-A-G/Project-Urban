#pragma once
#include "FModelCell.generated.h"

struct FLabel;

USTRUCT(Blueprintable)
struct FModelCell
{
	GENERATED_BODY()
public:
	FModelCell(TArray<FLabel*> allPossibleLabels) : CandidateLabels(allPossibleLabels)
	{}
	FModelCell(){}
public:
	TArray<FLabel*> CandidateLabels;
};
