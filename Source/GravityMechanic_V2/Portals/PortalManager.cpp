// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalManager.h"

#include "EngineUtils.h"
#include "../Libraries/MathLibrary.h"
#include "Portal.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APortalManager::APortalManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(FName("CaptureComponent"));
	CaptureComponent->SetupAttachment(RootComponent);

	//Post Process Settings, for optimization, disabling motion blur, etc
	FPostProcessSettings CaptureSettings;
	CaptureSettings.bOverride_AmbientOcclusionQuality = true;
	CaptureSettings.bOverride_MotionBlurAmount = true;
	CaptureSettings.bOverride_SceneFringeIntensity = true;
	CaptureSettings.bOverride_GrainIntensity = true;
	CaptureSettings.bOverride_ScreenSpaceReflectionQuality = true;
	CaptureSettings.bOverride_ScreenPercentage = true;
	CaptureSettings.AmbientOcclusionQuality = 0.0f;
	CaptureSettings.MotionBlurAmount = 0.0f;
	CaptureSettings.SceneFringeIntensity = 0.0f;
	CaptureSettings.GrainIntensity = 0.0f;
	CaptureSettings.ScreenSpaceReflectionQuality = 0.0f;
	CaptureSettings.ScreenPercentage = 100.0f;

	//Capture Component Settings
	CaptureComponent->bCaptureEveryFrame = false;
	CaptureComponent->bCaptureOnMovement = false;
	CaptureComponent->LODDistanceFactor = 3;
	CaptureComponent->TextureTarget = nullptr;
	CaptureComponent->bEnableClipPlane = true;
	CaptureComponent->bUseCustomProjectionMatrix = true;
	CaptureComponent->CaptureSource = ESceneCaptureSource::SCS_SceneColorSceneDepth;
	CaptureComponent->PostProcessSettings = CaptureSettings;
}

// Called when the game starts or when spawned
void APortalManager::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void APortalManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdatePortalView();
}

void APortalManager::UpdatePortalView()
{
	if (OwningController != nullptr)
	{
		//Get player camera manager
		APlayerCameraManager* CameraManager = OwningController->PlayerCameraManager;
		
		if (CameraManager != nullptr)
		{
			//For each portal
			for (TActorIterator<APortal> It(GetWorld()); It; ++It)
			{
				APortal* Portal = *It;
				//Get target of each portal
				APortal* Target = Portal->GetTarget();
				
				if (Portal != nullptr && Target != nullptr)
				{
					//Update the portal view position
					FVector Position = UMathLibrary::ConvertLocation(CameraManager->GetCameraLocation(), Portal, Target);
					//Update the portal view rotation
					FRotator Rotation = UMathLibrary::ConvertRotation(CameraManager->GetCameraRotation(), Portal, Target);
					FRotator NewRotation(0.f, Rotation.Yaw, 0.f);
					//Set position and rotation of the camera to respective vectors
					CaptureComponent->SetWorldLocationAndRotation(Position, Rotation);
					
					//Clip the scene capture plane
					CaptureComponent->ClipPlaneNormal = Target->GetActorForwardVector();
					CaptureComponent->ClipPlaneBase = Target->GetActorLocation() + (CaptureComponent->ClipPlaneNormal * -1.5f);
					CaptureComponent->CustomProjectionMatrix = OwningController->GetCameraProjectionMatrix();
					CaptureComponent->TextureTarget = Portal->GetRenderTarget();
					CaptureComponent->CaptureScene();
				}
			}
		}
	}
}

void APortalManager::SetControllerOwner(AMainPlayerController* NewOwner)
{
	OwningController = NewOwner;
}

void APortalManager::SetPortalClass(TSubclassOf<APortal> Class)
{
	PortalClass = Class;
}


