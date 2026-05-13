
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Ch3_Team2/APlayer.h"
#include "Exp_ItemBase.generated.h"

UCLASS()
class CH3_TEAM2_API AExp_ItemBase : public AActor
{
	GENERATED_BODY()
	

public:	
	AExp_ItemBase();
	//경험치 양
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category = "Exp")
	int32 ExpAmount;
	//나올 확률
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Exp")
	float DropWeight;
	
	bool bIsAttracted;
	float MoveSpeed;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult);

public:	
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere)
	USphereComponent* SphereComponent;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

};
