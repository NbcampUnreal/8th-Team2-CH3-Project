// Fill out your copyright notice in the Description page of Project Settings.


#include "SBW/SkillRadialExplosion.h"
#include "Engine/OverlapResult.h"
#include "public/MonsterBase.h"

void USkillRadialExplosion::ActiveSkill()
{
	AActor* Owner = GetOwner();
	if (Owner && GetWorld())
	{
		FVector StartLocation = Owner->GetActorLocation();

		// 1. 다중 충돌 결과를 담을 배열
		TArray<FOverlapResult> OverlapResults;
    
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Owner); // 시전자(나)는 제외

		// 2. 주변의 액터들을 감지 (Overlap 사용)
		bool bHasHit = GetWorld()->OverlapMultiByChannel(
			OverlapResults,
			StartLocation,
			FQuat::Identity,
			ECC_Pawn, // 캐릭터/몬스터들이 보통 사용하는 채널
			FCollisionShape::MakeSphere(DetectionRadius),
			Params
		);

		// 디버깅용 구체 그리기
		DrawDebugSphere(GetWorld(), StartLocation, DetectionRadius, 32, FColor::Cyan, false, 1.0f);

		if (bHasHit)
		{
			for (const FOverlapResult& Result : OverlapResults)
			{
				AActor* HitActor = Result.GetActor();
            
				// 3. 캐릭터 타입인지 확인 (넉백을 위해 Character로 형변환)
				ACharacter* TargetCharacter = Cast<ACharacter>(HitActor);
				if (TargetCharacter)
				{
					// 4. 넉백 방향 계산 (타겟 위치 - 내 위치)
					FVector LaunchDirection = TargetCharacter->GetActorLocation() - StartLocation;

					// 살짝 위로 뜨게 하고 싶다면 (타격감 상승)
					LaunchDirection.Z = LaunchZDirection; 
					LaunchDirection.Normalize();
					TargetCharacter->LaunchCharacter(LaunchDirection * KnockbackStrength, true, true);
					
					// 데미지 계산 여기서 호출하면 됩니다.
				}
			}
		}
	}
	Super::ActiveSkill();
	
}