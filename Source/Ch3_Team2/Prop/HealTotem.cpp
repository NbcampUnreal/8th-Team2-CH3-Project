#include "HealTotem.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "APlayer.h"

AHealTotem::AHealTotem()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("DummyRoot"));
	RootComponent = RootComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);

	CollisionBox->SetCollisionProfileName(TEXT("Trigger"));
}

void AHealTotem::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AHealTotem::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AHealTotem::OnOverlapEnd);
}

void AHealTotem::OnSpawnFromPool(const FTransform& Transform)
{
	SetActorLocationAndRotation(Transform.GetLocation(), Transform.GetRotation());
	
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	
	if (CollisionBox)
	{
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void AHealTotem::OnReturnToPool()
{
	if (CollisionBox)
	{
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void AHealTotem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AAPlayer* Player = Cast<AAPlayer>(OtherActor);
		if (Player)
		{
			// 플레이어 범위 안에 들어왔으므로, 플레이어에게 내 주소(this)를 전달
			Player->SetCurrentStructure(this);
			UE_LOG(LogTemp, Log, TEXT("[HealTotem] 플레이어가 치유 범위에 들어옴"));
		}
	}
}

void AHealTotem::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AAPlayer* Player = Cast<AAPlayer>(OtherActor);
		if (Player && Player->GetCurrentStructure() == this)
		{
			// 플레이어가 범위를 나갔으므로 대상을 비워줌
			Player->SetCurrentStructure(nullptr);
			UE_LOG(LogTemp, Log, TEXT("[HealTotem] 플레이어가 치유 범위를 벗어남"));
		}
	}
}

void AHealTotem::Interact(AActor* Interactor)
{
	if (!Interactor)
	{
		return;
	}

	AAPlayer* Player = Cast<AAPlayer>(Interactor);
	if (Player)
	{
		Player->AddCurrentHp(HealAmount);
        
		UE_LOG(LogTemp, Warning, TEXT("[HealTotem] 상호작용 완료! 플레이어에게 %d 힐 전송 후 토템 파괴"), HealAmount);
        
		// 사용된 토템을 브로드캐스트로 보내 줌
		ReadyToReturn.Broadcast(this);
	}
}
