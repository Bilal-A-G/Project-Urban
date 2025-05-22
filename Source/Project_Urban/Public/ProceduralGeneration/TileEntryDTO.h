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
	UFUNCTION(BlueprintCallable)
	void Reset()
	{
		name = "";
		mesh = nullptr;

		upAdjacencies.Empty();
		downAdjacencies.Empty();
		leftAdjacencies.Empty();
		rightAdjacencies.Empty();

		allKnownTiles.Empty();
	}
public:
	UPROPERTY(BlueprintReadWrite)
	FString name;
	UPROPERTY(BlueprintReadWrite)
	UStaticMesh* mesh;
	UPROPERTY(BlueprintReadWrite)
	FVector rotation;
	UPROPERTY(BlueprintReadWrite)
	FVector scale = FVector(1.0f, 1.0f, 1.0f);
	
	UPROPERTY(BlueprintReadWrite)
	TArray<int> upAdjacencies;
	UPROPERTY(BlueprintReadWrite)
	TArray<int> downAdjacencies;
	UPROPERTY(BlueprintReadWrite)
	TArray<int> leftAdjacencies;
	UPROPERTY(BlueprintReadWrite)
	TArray<int> rightAdjacencies;

	UPROPERTY(BlueprintReadWrite)
	TArray<int> allKnownTiles;
};
