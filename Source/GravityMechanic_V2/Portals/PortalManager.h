// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PlayerController/MainPlayerController.h"
#include "GameFramework/Actor.h"
#include "PortalManager.generated.h"

UCLASS()
class GRAVITYMECHANIC_V2_API APortalManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortalManager();

	void SetControllerOwner(class AMainPlayerController* NewOwner);

	void SetPortalClass(TSubclassOf<class APortal> Class);

	void UpdatePortalView();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Avoids creating one scene component per portal, thus recycling it each time we switch to a specific portal actor in the level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Capture)
	USceneCaptureComponent2D* CaptureComponent;

private:
	//Owning player controller
	UPROPERTY()
	class AMainPlayerController* OwningController;
	
	//Portal class to spawn 
	UPROPERTY(EditAnywhere, Category = Portal, meta = (AllowPrivateAccess = true))
	TSubclassOf<class APortal> PortalClass;

};
