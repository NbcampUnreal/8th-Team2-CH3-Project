#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "LevelFlowTestPawn.generated.h"

UCLASS()
class CH3_TEAM2_API ALevelFlowTestPawn : public APawn
{
	GENERATED_BODY()

public:
	ALevelFlowTestPawn();
	void OnPressNext();

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};