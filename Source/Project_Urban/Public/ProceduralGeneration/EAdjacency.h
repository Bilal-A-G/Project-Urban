#pragma once

UENUM(BlueprintType)
enum class EAdjacency : uint8
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	FORWARD,
	BACKWARD,
	//Just here so we know how many valid entries are in here
	LAST,
	INVALID
};

namespace PUrban
{
	static EAdjacency Opposite(EAdjacency current)
	{
		switch (current)
		{
		case EAdjacency::UP:
			return EAdjacency::DOWN;
		case EAdjacency::DOWN:
			return EAdjacency::UP;
		case EAdjacency::LEFT:
			return EAdjacency::RIGHT;
		case EAdjacency::RIGHT:
			return EAdjacency::LEFT;
		case EAdjacency::FORWARD:
			return EAdjacency::BACKWARD;
		case EAdjacency::BACKWARD:
			return EAdjacency::FORWARD;
		default:
			return EAdjacency::INVALID;
		}
	}

	static FVector ToVector(EAdjacency adjacency)
	{
		switch (adjacency) {
		case EAdjacency::UP:
			return FVector(0, 0, 1);
		case EAdjacency::DOWN:
			return FVector(0, 0, -1);
		case EAdjacency::LEFT:
			return FVector(-1, 0, 0);
		case EAdjacency::RIGHT:
			return FVector(1, 0, 0);
		case EAdjacency::FORWARD:
			return FVector(0, -1, 0);
		case EAdjacency::BACKWARD:
			return FVector(0, 1, 0);
		default:
			UE_LOG(LogTemp, Error, TEXT("Error, adjacency enum provided is invalid!"))
			return FVector(0, 0, 0);
		}
	}
}
