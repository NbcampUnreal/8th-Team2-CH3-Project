// Fill out your copyright notice in the Description page of Project Settings.


#include "Exp_ItemBase.h"

// Sets default values
AExp_ItemBase::AExp_ItemBase()
{
	//SphereCollision은 플레이어 감지용
 	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = SphereComponent;
	
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	//pawn민 오버랩 허용
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Overlap);
	
	
	//Mesh는 보이는 용도
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(SphereComponent);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetSimulatePhysics(false);
	
	
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	MoveSpeed = 900.0f;

}

// Called when the game starts or when spawned
void AExp_ItemBase::BeginPlay()
{
	
	Super::BeginPlay();
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this,&AExp_ItemBase::BeginOverlap);
	
}

// Called every frame
void AExp_ItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
	if (!TargetPlayer) return;
	
	
	FVector Dir =
		(TargetPlayer->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	
	FVector NewLocation =
		GetActorLocation() + Dir * MoveSpeed * DeltaTime;
	
	SetActorLocation(NewLocation);
	
	float Dist = FVector::Distance(
		GetActorLocation(),
		TargetPlayer->GetActorLocation());
	
	if (Dist < 40.0f)
	{
		
		TargetPlayer->AddExp(ExpAmount);
		SetActorTickEnabled(false);
		Destroy();
	}
}

void AExp_ItemBase::BeginOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	AAPlayer* Player = Cast<AAPlayer>(OtherActor);
	
	if (Player)
	{
		TargetPlayer = Player;
		
		SetActorTickEnabled(true);
	}
}

