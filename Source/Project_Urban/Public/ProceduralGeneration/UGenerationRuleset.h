#pragma once
#include "FAdjacencyWrapper.h"
#include "UGenerationRuleset.generated.h"

class ULabel;

//This represents a state that model cells can be in + all the states it can be adjacent to
//It is a class because it would be a waste of memory to copy everything in it everytime we create a new model cell
UCLASS(Blueprintable)
class UGenerationRuleset : public UObject
{
	GENERATED_BODY()
public:
	//Helper function, just tells us if this ruleset is inconsistent with the supplied ruleset at the adjacency
	bool CheckConsistency(const UGenerationRuleset* other, EAdjacency adjacency) const;
	//Modifies the array to conform to the supplied current ruleset at the adjacency provided
	static void RemoveInconsistentLabels(const UGenerationRuleset* current, TArray<UGenerationRuleset*>& array, EAdjacency adjacency);
	//Get a read only copy of our adjacency list
	UFUNCTION(BlueprintCallable)
	const TArray<ULabel*>& GetAdjacencyValuesFromKey(EAdjacency adjacency) const;
	//Write to a adjacency list at an index
	UFUNCTION(BlueprintCallable)
	void UpdateAdjacencyValue(EAdjacency adjacency, int index, ULabel* label);
	
	//Each ruleset must have a unique label tied to it
	bool operator==(const UGenerationRuleset& Other) const
	{
		return Current == Other.Current;
	}
public:
	UPROPERTY(BlueprintReadWrite)
	ULabel* Current;
	UPROPERTY(BlueprintReadWrite)
	TArray<FAdjacencyWrapper> Adjacencies;
	UPROPERTY()
	TArray<ULabel*> NullList = TArray<ULabel*>{nullptr};
};