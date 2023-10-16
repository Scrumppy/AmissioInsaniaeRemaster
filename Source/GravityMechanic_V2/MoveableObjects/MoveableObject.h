// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoveableObject.generated.h"

UCLASS()
class GRAVITYMECHANIC_V2_API AMoveableObject : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMoveableObject();

	void CustomTickPhysics(FVector TargetVelocity);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* ObjectMesh;

	UPROPERTY(EditAnywhere, Category = "Variables")
	FVector Velocity;

	UPROPERTY(EditAnywhere, Category = "Variables")
	float MaxVelocity;

	UPROPERTY(EditAnywhere, Category = "Variables")
	float MaxVelocitySquared;

	UPROPERTY(EditAnywhere, Category = "Variables")
	bool bBeingGrabbed;

	UPROPERTY(EditAnywhere, Category = "Variables")
	float MassInKg;
};
