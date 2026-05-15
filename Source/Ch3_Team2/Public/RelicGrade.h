#pragma once

#include "CoreMinimal.h"
#include "RelicGrade.generated.h"

UENUM(BlueprintType)
enum class ERelicGrade : uint8
{
	Common,
	Rare,
	Epic,
	Legendary,
};

UENUM(BlueprintType)
enum class ERelicStatType : uint8
{
	MaxHP,
	SkillCooldown,
	MoveSpeed,
	AmmoDamage,
	critical,
	
};
