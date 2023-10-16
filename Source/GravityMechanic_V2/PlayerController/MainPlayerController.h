// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Portals/Portal.h"
#include "../Portals/PortalManager.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GRAVITYMECHANIC_V2_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	FMatrix GetCameraProjectionMatrix();

	 //Portal class to spawn 
	 UPROPERTY(EditDefaultsOnly, Category = Portal)
	 TSubclassOf<class APortal> PortalClass;
	
	 //Portal manager class to spawn 
	 UPROPERTY(EditDefaultsOnly, Category = Portal)
	 TSubclassOf<class APortalManager> PortalManagerClass;
		
	APortalManager* PortalManager;

	class ALiminalPortalManager* LiminalPortalManager;
	
	FORCEINLINE ALiminalPortalManager* GetPortalManager() {return LiminalPortalManager;}

protected:
	
	virtual void BeginPlay() override;
};
