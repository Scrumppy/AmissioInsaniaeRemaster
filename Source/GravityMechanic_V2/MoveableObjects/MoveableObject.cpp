// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveableObject.h"

#include "GravityMechanic_V2/Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AMoveableObject::AMoveableObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object Mesh"));
}

// Called when the game starts or when spawned
void AMoveableObject::BeginPlay()
{
	Super::BeginPlay();

	ObjectMesh->SetMassOverrideInKg(NAME_None, MassInKg, true);

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (PlayerCharacter)
	{
		Velocity.Z = PlayerCharacter->GetDefaultGravity() * (-1.f);

		CustomTickPhysics(Velocity);

		MaxVelocitySquared = UKismetMathLibrary::Square(MaxVelocity);
	}
}

void AMoveableObject::CustomTickPhysics(FVector TargetVelocity)
{
	Velocity = TargetVelocity;

	if (true)
	{
		FLatentActionInfo LatentActionInfo;
		UKismetSystemLibrary::RetriggerableDelay(this, 0.01f, LatentActionInfo);
	}

	if (ObjectMesh->Mobility == EComponentMobility::Movable)
	{
		if (bBeingGrabbed)
		{
			float VectorLengthSquared = UKismetMathLibrary::VSizeSquared(GetVelocity());
			bool bCondition = UKismetMathLibrary::Greater_FloatFloat(VectorLengthSquared, MaxVelocitySquared);
			
			if (!bCondition)
			{
				ObjectMesh->SetAllPhysicsLinearVelocity(Velocity, true);
			}
			else
			{
				FVector ClampedVelocity = UKismetMathLibrary::ClampVectorSize(GetVelocity(), 0.f, MaxVelocity);
				ObjectMesh->SetAllPhysicsLinearVelocity(ClampedVelocity, false);
			}
		}
		else
		{
			ObjectMesh->SetAllPhysicsLinearVelocity(FVector::ZeroVector, true);
		}
	}
}

