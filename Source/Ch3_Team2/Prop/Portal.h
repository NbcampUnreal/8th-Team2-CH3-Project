#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portal.generated.h"

class UBoxComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class CH3_TEAM2_API APortal : public AActor
{
	GENERATED_BODY()
	
public:	
	APortal();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, Category = "Portal")
	UBoxComponent* TriggerBox;
	
	UPROPERTY(VisibleAnywhere, Category = "Portal")
	UNiagaraComponent* PortalNS;
	
	bool bAlreadyTriggered = false;
	
	UFUNCTION()
	void OnPortalOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
};
