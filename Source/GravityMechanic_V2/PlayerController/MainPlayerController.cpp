// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "../Portals/Portal.h"
#include "../Portals/PortalManager.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	PortalManager = GetWorld()->SpawnActor<APortalManager>(PortalManagerClass, FVector::ZeroVector, FRotator::ZeroRotator);
	PortalManager->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	PortalManager->SetControllerOwner(this);
	PortalManager->SetPortalClass(PortalClass);
}


FMatrix AMainPlayerController::GetCameraProjectionMatrix()
{
	//Get camera projection matrix to apply it on the portal manager update portal view function
	FMatrix ProjectionMatrix;

	if (GetLocalPlayer() != nullptr)
	{
		FSceneViewProjectionData PlayerProjectionData;
		GetLocalPlayer()->GetProjectionData(GetLocalPlayer()->ViewportClient->Viewport, eSSP_FULL, PlayerProjectionData);
		ProjectionMatrix = PlayerProjectionData.ProjectionMatrix;
	}
	return ProjectionMatrix;

	
}
