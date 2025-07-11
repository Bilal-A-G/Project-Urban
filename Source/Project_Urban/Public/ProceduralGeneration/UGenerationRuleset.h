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
	virtual void PostInitProperties() override;
	//Modifies the array to conform to the supplied current ruleset at the adjacency provided
	static void RemoveInconsistentLabels(const UGenerationRuleset* current, TArray<UGenerationRuleset*>& array, EAdjacency adjacency);
	//Get a read only copy of our adjacency list, excluding any nullptrs, this does copy, but it's not too bad
	UFUNCTION(BlueprintCallable)
	const TArray<ULabel*> GetAdjacencyValuesFromKey(EAdjacency adjacency) const;
	//Write to a adjacency list at an index, if it exists, if not create a new entry at that index, if you pass nullptr
	//as the label parameter, then it will delete the entry at the provided index, shrinking the array 
	UFUNCTION(BlueprintCallable)
	void UpdateAdjacencyValue(EAdjacency adjacency, int index, ULabel* label);
	//Adds a new entry to the corresponding adjacency array, specified by the adjacency enum, does not allow duplicates,
	//returns whether it succeeded or not
	bool AddAdjacencyEntry(EAdjacency adjacency, ULabel* label);
	bool ArrayContains(TArray<ULabel*> array, ULabel* val) const;
	//Each ruleset must have a unique label tied to it
	bool operator==(const UGenerationRuleset& Other) const
	{
		return Current == Other.Current;
	}
private:
	//Helper function, just tells us if this ruleset is inconsistent with the supplied ruleset at the adjacency
	bool CheckConsistency(const UGenerationRuleset* other, EAdjacency adjacency) const;
public:
	UPROPERTY(BlueprintReadWrite)
	ULabel* Current;
	UPROPERTY(BlueprintReadWrite)
	TArray<FAdjacencyWrapper> Adjacencies;
	UPROPERTY()
	TArray<ULabel*> NullList = TArray<ULabel*>{nullptr};
};