#pragma once
#include "FModelCell.generated.h"

class UGenerationRuleset;

//This represents a single cell in the model
USTRUCT(Blueprintable)
struct FModelCell
{
	GENERATED_BODY()
public:
	FModelCell() : Visited(false), Collapsed(false){}
	FModelCell(TArray<UGenerationRuleset*> allPossibleRuleSets) : CandidateRuleSets(allPossibleRuleSets),
	Visited(false), Collapsed(false), Colour(FLinearColor::White)
	{
		VisualizationColour = FLinearColor(FMath::FRand(), FMath::FRand(), FMath::FRand());
	}
public:
	UPROPERTY()
	TArray<UGenerationRuleset*> CandidateRuleSets;
	bool Visited;
	bool Collapsed;
	UPROPERTY()
	FLinearColor Colour = FLinearColor::White;
	UPROPERTY()
	FLinearColor VisualizationColour = FLinearColor::White;
};
