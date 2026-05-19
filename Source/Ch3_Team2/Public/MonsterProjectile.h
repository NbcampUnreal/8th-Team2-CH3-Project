// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Poolable.h"
#include "MonsterProjectile.generated.h"

UCLASS()
class CH3_TEAM2_API AMonsterProjectile : public AActor ,public IPoolable
{
	GENERATED_BODY()
	
public:	
	AMonsterProjectile();
	
	// IPoolable 인터페이스 구현
	virtual FOnReadyToReturn& GetOnReadyToReturn() override { return OnReadyToReturn; }
	virtual void OnSpawnFromPool(const FTransform& Transform) override;
	virtual void OnReturnToPool() override;
	UPROPERTY(BlueprintAssignable)
	FOnReadyToReturn OnReadyToReturn;
	
	float Damage; 
protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* ProjectileMesh;

	//발사체 움직임 담당 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	class UProjectileMovementComponent* ProjectileMovement;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
