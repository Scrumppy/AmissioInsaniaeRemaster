// Fill out your copyright notice in the Description page of Project Settings.


#include "CeilingLightFlicker.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
ACeilingLightFlicker::ACeilingLightFlicker()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpotLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("Spotlight Component"));

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow Component"));
	ArrowComponent->SetupAttachment(SpotLightComponent);

	LightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Light Mesh"));
	LightMesh->SetupAttachment(SpotLightComponent);
}

// Called when the game starts or when spawned
void ACeilingLightFlicker::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::PlaySoundAtLocation(this, LightCue, GetActorLocation(), FRotator::ZeroRotator, 1.f,
		1.f, 0.f, nullptr, nullptr, nullptr);
}

