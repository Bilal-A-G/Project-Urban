#pragma once
#include "PUrbanUtils.h"
#include "FGenerationRuleset.generated.h"

struct FLabel;

USTRUCT(Blueprintable)
struct FGenerationRuleset
{
	GENERATED_BODY()
public:
	FLabel* Current;
	TMap<PUrban::Adjacency, TArray<FLabel*>> Adjacencies;
};