// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/Actor.h"
#include "CeilingLight.generated.h"

UCLASS()
class GRAVITYMECHANIC_V2_API ACeilingLight : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACeilingLight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = "Components")
	UPointLightComponent* PointLightComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* LightMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	USoundBase* LightCue;
};
