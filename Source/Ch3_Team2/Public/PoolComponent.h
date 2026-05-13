// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PoolComponent.generated.h"

USTRUCT(BlueprintType)
struct FActorPool
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<AActor*> InactiveActors;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CH3_TEAM2_API UPoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPoolComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	AActor* GetActorFromPool(TSubclassOf<AActor> Class,const FTransform& Transform);
	UFUNCTION()
	void ReturnActorToPool(AActor* Actor);
	
protected:
	virtual void BeginPlay() override;
	
	
private:
	
	UPROPERTY()
	TMap<TSubclassOf<AActor>,FActorPool> PoolMap;
};
