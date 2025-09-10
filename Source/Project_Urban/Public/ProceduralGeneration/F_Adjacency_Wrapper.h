#pragma once
#include "INC_Adjacency.h"
#include "U_Label.h"
#include "F_Adjacency_Wrapper.generated.h"

//This is just a wrapper so we can have an array of tuples, where the key is an enum, and the values are
//an array of Label*
USTRUCT(Blueprintable)
struct FAdjacencyWrapper
{
	GENERATED_BODY()
public:
	FAdjacencyWrapper(){}
public:
	TArray<ULabel*> values_;
	EAdjacency key_;
};
