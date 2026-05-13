// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Poolable.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReadyToReturn,AActor*,ActorToReturn);
// This class does not need to be modified.

UINTERFACE(MinimalAPI)
class UPoolable : public UInterface
{
	GENERATED_BODY()
};



class CH3_TEAM2_API IPoolable
{
	GENERATED_BODY()

public:
	//상속받은 객체의 델리게이트 얻어옴
	virtual FOnReadyToReturn& GetOnReadyToReturn()=0;
	
	//풀에서 나올 때
	virtual void OnSpawnFromPool(const FTransform& Transform) = 0;
	//풀로 돌아갈 때
	virtual void OnReturnToPool() = 0;
};

