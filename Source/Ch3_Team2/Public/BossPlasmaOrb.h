// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Poolable.h"
#include "BossPlasmaOrb.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class CH3_TEAM2_API ABossPlasmaOrb : public AActor,public IPoolable
{
	GENERATED_BODY()
    
public:    
	ABossPlasmaOrb();

	// --- IPoolable 인터페이스 구현 ---
	virtual FOnReadyToReturn& GetOnReadyToReturn() override { return OnReadyToReturn; }
	virtual void OnSpawnFromPool(const FTransform& Transform) override;
	virtual void OnReturnToPool() override;
	
	UPROPERTY(BlueprintAssignable)
	FOnReadyToReturn OnReadyToReturn;
	// ------------------------------

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement;
	
	// 플레이어 몸이나 벽에 부딪혔을 때의 처리
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb Stats", meta = (AllowPrivateAccess = "true"))
	int32 MaxHitCount = 3;

	int32 CurrentHitCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb Stats", meta = (AllowPrivateAccess = "true"))
	float OrbDamage = 30.0f;
};