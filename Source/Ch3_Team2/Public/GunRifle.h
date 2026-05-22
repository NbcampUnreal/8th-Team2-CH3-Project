// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GunBase.h"
#include "GunRifle.generated.h"

/**
 * 
 */
UCLASS()
class CH3_TEAM2_API AGunRifle : public AGunBase
{
	GENERATED_BODY()
public:
	virtual void Fire_Gun(FVector Location, FVector Direction) override;
};
