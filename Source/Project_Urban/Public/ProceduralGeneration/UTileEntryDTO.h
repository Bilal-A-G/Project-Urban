#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UTileEntryDTO.generated.h"

class UGenerationRuleset;

UCLASS(Blueprintable)
//Basically just encapsulates a name and the ruleset
//Name is there so in the UI we can display it to the user in some form
class PROJECT_URBAN_API UTileEntryDTO : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	FString name;
	UPROPERTY(BlueprintReadWrite)
	UGenerationRuleset* tileRuleset;
};
