#pragma once
#include "FModelCell.generated.h"

class UGenerationRuleset;

//This represents a single cell in the model
USTRUCT(Blueprintable)
struct FModelCell
{
	GENERATED_BODY()
public:
	FModelCell() : Collapsed(false){}
	FModelCell(TArray<UGenerationRuleset*> allPossibleRuleSets) : CandidateRuleSets(allPossibleRuleSets),
	Collapsed(false), Colour(FLinearColor::White)
	{
		VisualizationColour = FLinearColor(FMath::FRand(), FMath::FRand(), FMath::FRand());
	}
public:
	UPROPERTY()
	TArray<UGenerationRuleset*> CandidateRuleSets;
	bool Collapsed;
	UPROPERTY()
	FLinearColor Colour = FLinearColor::White;
	UPROPERTY()
	FLinearColor VisualizationColour = FLinearColor::White;
};
