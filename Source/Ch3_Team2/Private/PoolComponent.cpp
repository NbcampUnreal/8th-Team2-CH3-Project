// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolComponent.h"

#include "Poolable.h"

UPoolComponent::UPoolComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UPoolComponent::BeginPlay()
{
	Super::BeginPlay();
}
void UPoolComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


AActor* UPoolComponent::GetActorFromPool(TSubclassOf<AActor> Class, const FTransform& Transform)
{
	FActorPool& ActorPool = PoolMap.FindOrAdd(Class);
	AActor* SelectedActor= nullptr;
	
	//풀에 액터 있으면 가져오고 아니면 새로 만듬
	if (ActorPool.InactiveActors.Num() > 0)
	{
		SelectedActor= ActorPool.InactiveActors.Pop();
	}
	else
	{
		SelectedActor = GetWorld()->SpawnActor<AActor>(Class,Transform);
		if (IPoolable* Poolable = Cast<IPoolable>(SelectedActor))
		{
			//신호를 받으면 신호 보낸 액터 풀로 다시 넣음
			Poolable->GetOnReadyToReturn().AddUniqueDynamic(this,&UPoolComponent::ReturnActorToPool);
		}
	}
	
	//Poolable 상속받은 액터만 pool사용가능
	if (IPoolable* Poolable = Cast<IPoolable>(SelectedActor))
	{
		Poolable->OnSpawnFromPool(Transform);
	}
	return SelectedActor;
}

void UPoolComponent::ReturnActorToPool(AActor* Actor)
{
	if (!Actor) return;
	
	FActorPool& ActorPool = PoolMap.FindOrAdd(Actor->GetClass());
	//actor 안에 있는지 검사
	if (ActorPool.InactiveActors.Contains(Actor)) return;
	
	//Poolable 상속받은 액터만 pool사용가능
	if (IPoolable* Poolable = Cast<IPoolable>(Actor))
	{
		Poolable->OnReturnToPool();
	}
	ActorPool.InactiveActors.Add(Actor);
}



