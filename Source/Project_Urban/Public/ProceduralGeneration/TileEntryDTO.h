#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TileEntryDTO.generated.h"

UCLASS(Blueprintable)
class PROJECT_URBAN_API UTileEntryDTO : public UObject
{
	GENERATED_BODY()
public:
	UTileEntryDTO(){}
public:
	UPROPERTY(BlueprintReadWrite)
	FString name;
	UPROPERTY(BlueprintReadWrite)
	UStaticMesh* mesh;
	UPROPERTY(BlueprintReadWrite)
	FVector rotation;
	UPROPERTY(BlueprintReadWrite)
	TArray<FString> upAdjacencies;
	UPROPERTY(BlueprintReadWrite)
	TArray<FString> downAdjacencies;
	UPROPERTY(BlueprintReadWrite)
	TArray<FString> leftAdjacencies;
	UPROPERTY(BlueprintReadWrite)
	TArray<FString> rightAdjacencies;

	UPROPERTY(BlueprintReadWrite)
	TArray<FString> allKnownTiles;
};
