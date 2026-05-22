#include "Prop/Portal.h"

#include "AGameState.h"
#include "APlayer.h"
#include "NiagaraComponent.h"
#include "Battle/BattleSubsystem.h"
#include "Components/BoxComponent.h"
#include "Data/LevelFlowSubsystem.h"
#include "Kismet/GameplayStatics.h"

APortal::APortal()
{
	PrimaryActorTick.bCanEverTick = false;
	
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	TriggerBox->SetGenerateOverlapEvents(true);
	
	PortalNS = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PortalFX"));
	PortalNS->SetupAttachment(RootComponent);
	PortalNS->SetAutoActivate(true);
}

void APortal::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnPortalOverlap);
}

void APortal::OnPortalOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep,const FHitResult& SweepResult)
{
	if (bAlreadyTriggered)
	{
		return;
	}
	
	AAPlayer* Player = Cast<AAPlayer>(OtherActor);
	if (!Player || Player != UGameplayStatics::GetPlayerCharacter(this, 0))
	{
		return;
	}

	bAlreadyTriggered = true;

	if (ULevelFlowSubsystem* LevelFlow = GetGameInstance()->GetSubsystem<ULevelFlowSubsystem>())
	{
		LevelFlow->TravelToNextLevel();
	}
}
