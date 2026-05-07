#include "LevelFlowTestPawn.h"
#include "Data/LevelFlowSubsystem.h"
#include "Kismet/GameplayStatics.h"

ALevelFlowTestPawn::ALevelFlowTestPawn()
{
	PrimaryActorTick.bCanEverTick = false;
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ALevelFlowTestPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (PlayerInputComponent)
		PlayerInputComponent->BindKey(EKeys::E, IE_Pressed, this, &ALevelFlowTestPawn::OnPressNext);
}

void ALevelFlowTestPawn::OnPressNext()
{
	UE_LOG(LogTemp, Log, TEXT("[LevelFlowTest] E 키 입력 → 다음 레벨"));

	if (UGameInstance* GI = UGameplayStatics::GetGameInstance(this))
		if (ULevelFlowSubsystem* Flow = GI->GetSubsystem<ULevelFlowSubsystem>())
			Flow->TravelToNextLevel();
}