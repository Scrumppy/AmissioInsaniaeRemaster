// Fill out your copyright notice in the Description page of Project Settings.


#include "Gate.h"

#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AGate::AGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	GateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GateMesh"));
	GateMesh->SetupAttachment(RootComponent);
	
	ArrivalScenePoint = CreateDefaultSubobject<USceneComponent>(TEXT("ArrivalScenePoint"));
	ArrivalScenePoint->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGate::BeginPlay()
{
	Super::BeginPlay();

	DefaultRelativeLocation = GateMesh->GetRelativeLocation();
	
}

void AGate::MoveGate(bool bIsOpen)
{
	const FLatentActionInfo LatentInfo;
	
	if (bIsOpen)
	{
		UKismetSystemLibrary::MoveComponentTo(GateMesh, ArrivalScenePoint->GetRelativeLocation(), FRotator::ZeroRotator, true, true, 1.0f,
			false, EMoveComponentAction::Move, LatentInfo);
		UE_LOG(LogTemp, Warning, TEXT("OPEN"))
	}
	else
	{
		UKismetSystemLibrary::MoveComponentTo(GateMesh, DefaultRelativeLocation, FRotator::ZeroRotator, true, true, 1.0f,
		false, EMoveComponentAction::Move, LatentInfo);
	}
}