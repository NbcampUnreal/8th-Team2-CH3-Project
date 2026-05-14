
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Ch3_Team2/APlayer.h"
#include "ExpItemBase.generated.h"

UCLASS()
class CH3_TEAM2_API AExpItemBase : public AActor
{
	GENERATED_BODY()
	

public:	
	AExpItemBase();
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
	//오버랩
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
	TObjectPtr<USphereComponent> SphereComponent;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	
	UPROPERTY()
	TObjectPtr<AAPlayer> TargetPlayer;

};
