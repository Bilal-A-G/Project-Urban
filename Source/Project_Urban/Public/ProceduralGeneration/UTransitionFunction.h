#pragma once
#include "CoreMinimal.h"
#include "PUrbanUtils.h"
#include "UObject/Object.h"
#include "UTransitionFunction.generated.h"

struct FLabel;
struct FGenerationRuleset;

UCLASS()
class PROJECT_URBAN_API UTransitionFunction : public UObject
{
	GENERATED_BODY()
public:
	bool CheckConsistency(FLabel current, FLabel other, PUrban::Adjacency adjacency);
	void RemoveInconsistentLabels(FLabel current, TArray<FLabel>& array, PUrban::Adjacency adjacency);
public:
	TArray<FGenerationRuleset*> RuleSets;
};