#pragma once

#include "CoreMinimal.h"
#include "Ruleset/U_Generation_Ruleset.h"
#include "UObject/Object.h"
#include "U_Tile_Entry_DTO.generated.h"


UCLASS(Blueprintable)
//Basically just encapsulates a name and the ruleset
//Name is there so in the UI we can display it to the user in some form
class PROJECT_URBAN_API UTileEntryDTO : public UObject
{
	GENERATED_BODY()
public:
	virtual void PostInitProperties() override;
public:
	UPROPERTY(BlueprintReadWrite)
	FString name_;
	UPROPERTY(BlueprintReadWrite)
	UGenerationRuleset* tileRuleset_;
};
