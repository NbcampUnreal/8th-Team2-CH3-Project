#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealTotem.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class USceneComponent;

UCLASS()
class CH3_TEAM2_API AHealTotem : public AActor
{
	GENERATED_BODY()
	
public:	
	AHealTotem();

	void Interact(AActor* InteractorCharacter);

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
						bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
					  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* RootComp;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* CollisionBox;
	
	UPROPERTY(EditAnywhere, Category = "DesignSettings")
	int32 HealAmount = 50;
};
