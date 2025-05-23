#pragma once
#include "EAdjacency.h"
#include "FAdjacencyWrapper.generated.h"

class ULabel;
//This is just a wrapper so we can have an array of tuples, where the key is an enum, and the values are
//an array of Label*
USTRUCT(Blueprintable)
struct FAdjacencyWrapper
{
	GENERATED_BODY()
public:
	FAdjacencyWrapper(){}
public:
	TArray<ULabel*> values;
	EAdjacency key;
};
