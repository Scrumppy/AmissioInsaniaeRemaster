// Fill out your copyright notice in the Description page of Project Settings.


#include "CeilingLight.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
ACeilingLight::ACeilingLight()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PointLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("Pointlight Component"));

	LightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Light Mesh"));
	LightMesh->SetupAttachment(PointLightComponent);
}

// Called when the game starts or when spawned
void ACeilingLight::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::PlaySoundAtLocation(this, LightCue, GetActorLocation(), FRotator::ZeroRotator, 0.1f,
		1.f, 0.f, nullptr, nullptr, nullptr);
}
