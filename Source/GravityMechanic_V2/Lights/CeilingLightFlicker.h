// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/Actor.h"
#include "CeilingLightFlicker.generated.h"

UCLASS()
class GRAVITYMECHANIC_V2_API ACeilingLightFlicker : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACeilingLightFlicker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = "Components")
	USpotLightComponent* SpotLightComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	UArrowComponent* ArrowComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* LightMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	USoundBase* LightCue;
};
