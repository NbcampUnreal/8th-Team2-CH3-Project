// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "APlayer.h"
#include "PickUpComp.generated.h"

// Declaration of the delegate that will be called when someone picks this up
// The character picking this up is the parameter sent with the notification
// FOn_PickUp 형태 이름 선언이기 떄문에 다른 파일에 있는 이름과 중복되지 않도록 설정할것
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOn_PickUp, AAPlayer*, PickUpCharacter);

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CH3_TEAM2_API UPickUpComp : public USphereComponent
{
	GENERATED_BODY()
public:
	/** Delegate to whom anyone can subscribe to receive this event */
	UPROPERTY(BlueprintAssignable, Category = Interaction)
	FOn_PickUp OnPickUp;
	UPickUpComp();
protected:
	
	/** Called when the game starts */
	virtual void BeginPlay() override;

	/** Code for when something overlaps this component */
	UFUNCTION()
	void OnSphereBeginOverlap(
		UPrimitiveComponent* OverlappedComponent
		, AActor* OtherActor
		, UPrimitiveComponent* OtherComp
		, int32 OtherBodyIndex
		, bool bFromSweep
		, const FHitResult& SweepResult);
};
