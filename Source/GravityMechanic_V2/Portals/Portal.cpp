// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"

#include "EngineUtils.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GravityMechanic_V2/Libraries/MathLibrary.h"
#include "GravityMechanic_V2/Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APortal::APortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsActive = true
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComponent);
	
	CaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(FName("CaptureComponent"));
	CaptureComponent->SetupAttachment(RootComponent);
	
	//Portal Mesh
	PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));
	PortalMesh->SetupAttachment(RootComponent);
	
	//Box collision
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(FName("BoxComponent"));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnOverlapBegin);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &APortal::OnOverlapEnd);
	BoxComponent->SetupAttachment(RootComponent);

	BackFacingVector = CreateDefaultSubobject<USceneComponent>(TEXT("BackFacingScene"));
	BackFacingVector->SetupAttachment(RootComponent);
	BackFacingVector->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("DirectionArrow"));
	ArrowComponent->SetupAttachment(RootComponent);
	
	PortalCameraQuality = 0.65f;
	bCanTeleport = true;
	bCanTeleportObject = true;
}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();
	
	//CreateRender Target and Settings
	RenderTarget = NewObject<UTextureRenderTarget2D>(this, UTextureRenderTarget2D::StaticClass(), FName("RenderTarget"));
	RenderTarget->InitAutoFormat(1080, 720);
	RenderTarget->AddressX = TextureAddress::TA_Wrap;
	RenderTarget->AddressY = TextureAddress::TA_Wrap;
	RenderTarget->bAutoGenerateMips = false;
	RenderTarget->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA16f;
	RenderTarget->Filter = TextureFilter::TF_Bilinear;

	CaptureComponent->bEnableClipPlane = true;

	//Create material instance to take in the render target
	MaterialInstance = UMaterialInstanceDynamic::Create(Material, this);
	MaterialInstance->SetTextureParameterValue(FName("RenderTargetTexture"), RenderTarget);
	PortalMesh->SetMaterial(0, MaterialInstance);

	//SetPortalView();
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Target != nullptr)
	{
		if (bCanTeleport || bCanTeleportObject)
		{
			TeleportPlayer();
		}
	}
}

void APortal::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                             int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* OtherActorCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	
	// If there is no overlapping player or no target, return
	if (OtherActorCharacter == nullptr || Target == nullptr)
	{
		return;
	}

	if (OtherActor->ActorHasTag("Portable") && OtherActor != OtherActorCharacter)
	{
		//UE_LOG(LogTemp, Warning, TEXT("object inside portal: %s"), *OtherActor->GetName())
		bCanTeleportObject = true;
		ObjectInsidePortal = OtherActor;
	}
	else if (OtherActor == OtherActorCharacter)
	{
		bCanTeleport = true;
		// UE_LOG(LogTemp, Warning, TEXT("character inside portal: %s"), *OtherActor->GetName())
		CharacterInsidePortal = OtherActorCharacter;
	}
}

void APortal::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	// If I haven't overlapped a character or there are no linked portals, don't to anything
	APlayerCharacter* OtherActorCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	
	if (OtherActorCharacter == nullptr || Target == nullptr)
	{
		return;
	}

	if (OtherActor->ActorHasTag("Portable") && OtherActor != OtherActorCharacter)
	{
		bCanTeleportObject = false;
		ObjectInsidePortal = nullptr;
	}
	
	else if (OtherActor == OtherActorCharacter)
	{
		bCanTeleport = false;
		//If a character ends overlap with this portal, enable collision with walls where portals are placed
		CharacterInsidePortal = nullptr;
		OtherActorCharacter->GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"), true);
	}
}

bool APortal::IsActive()
{
	return bIsActive;
}

void APortal::SetActive(bool NewActive)
{
	bIsActive = NewActive;
}

void APortal::TeleportPlayer()
{
	if (CharacterInsidePortal)
	{
		const auto PlayerVelocity = CharacterInsidePortal->GetVelocity() * GetWorld()->GetDeltaSeconds();
		//Check if player is crossing the portals
		if(UMathLibrary::CheckIsCrossing(CharacterInsidePortal->GetActorLocation(), GetActorLocation(), GetActorForwardVector(), bLastInFront, LastPosition))
		{
			//Get player location
			FVector Location = UMathLibrary::ConvertLocation(CharacterInsidePortal->GetActorLocation(), this, Target);
			//Player rotation
			FRotator PlayerNewRotation;
			//Store velocity to apply it after exiting the portal
			FVector PlayerLocalVelocity;
		
			//Get player local velocity
			PlayerLocalVelocity = UKismetMathLibrary::InverseTransformDirection(CharacterInsidePortal->GetActorTransform(),
			CharacterInsidePortal->GetVelocity());
		
			//Get camera direction of player in the portal 
			FTransform PlayerCameraRelativeDirection = UKismetMathLibrary::MakeRelativeTransform(
			CharacterInsidePortal->GetFirstPersonCameraComponent()->GetComponentTransform(),
			GetBackFacingSceneComponent()->GetComponentTransform());
		
			//Make it relative to linked portal. Gets Transform of the player when exiting the linked portal
			FTransform NewPlayerTransform = UKismetMathLibrary::ComposeTransforms(PlayerCameraRelativeDirection,
			Target->GetActorTransform()); 
		
			//Subtract camera offset. Avoids camera snapping
			FVector NewPlayerLocationFixed = NewPlayerTransform.GetLocation() -CharacterInsidePortal->GetFirstPersonCameraComponent()->GetRelativeLocation();
		
			//Store relative player rotation to apply when teleporting
			PlayerNewRotation = FRotator(NewPlayerTransform.Rotator().Pitch, NewPlayerTransform.Rotator().Yaw, 0.f);
		
			//Set Character location in front of linked portal. Avoids getting stuck in wall
			CharacterInsidePortal->SetActorLocation((Target->GetActorForwardVector() * 15.f) + NewPlayerLocationFixed,
			false, nullptr, ETeleportType::TeleportPhysics);
		
			//Get player character 
			ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
			//Check if player is null
			if (PlayerCharacter == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("Null player character"));
				return;
			}
		
			//Apply new rotation to the player
			PlayerCharacter->GetController()->SetControlRotation(PlayerNewRotation);
		
			//Convert player local velocity to world
			FTransform PlayerCurrentTransform = UKismetMathLibrary::MakeTransform(
			PlayerCharacter->GetActorLocation(),
			PlayerCharacter->GetController()->GetControlRotation(), FVector(1, 1, 1));
			
			FVector PlayerNewWorldVelocity = UKismetMathLibrary::TransformDirection(PlayerCurrentTransform, PlayerLocalVelocity);
		
			// Apply it to the player
			PlayerCharacter->GetMovementComponent()->Velocity = PlayerNewWorldVelocity;
					
			//Last position is the last location
			LastPosition = Location;
			//After teleportation, player is no longer overlapping
			bCanTeleport = false;
		}
	}
	if (ObjectInsidePortal)
	{
		if (UMathLibrary::CheckIsCrossing(ObjectInsidePortal->GetActorLocation(), GetActorLocation(), GetActorForwardVector(), bLastInFront, LastPosition))
		{
			FHitResult HitResult;
			
			//Get location
			FVector Location = UMathLibrary::ConvertLocation(ObjectInsidePortal->GetActorLocation(), this, Target);
			//Get rotation
			FRotator Rotation = UMathLibrary::ConvertRotation(ObjectInsidePortal->GetActorRotation(), this, Target);
			//Store object velocity
			//FVector SavedVelocity = ObjectInsidePortal->GetVelocity();
			
			//Set object location
			ObjectInsidePortal->SetActorLocationAndRotation((Target->GetActorForwardVector() * 20.f) + Location, Rotation, false, &HitResult, ETeleportType::TeleportPhysics);

			//Calculate new velocity
			//FVector NewVelocity = (Target->GetBoxComponentForwardVector()) * SavedVelocity;

			//Apply new velocity to the object
			ChangeObjectVelocity();

			GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Teleported object"));
			//Last position is the last location
			LastPosition = Location;
			//After teleportation, player is no longer overlapping
			bCanTeleportObject = false;
			ObjectInsidePortal = nullptr;
		}
	}
}

void APortal::CheckCanTeleport()
{
	const auto PlayerVelocity = CharacterInsidePortal->GetVelocity() * GetWorld()->GetDeltaSeconds();
	const FVector PlayerPositionNextFrame = CharacterInsidePortal->GetActorLocation() + PlayerVelocity;

	// Vector pointing from portal to player next frame's position
	FVector PortalToNextPlayerPosition = PlayerPositionNextFrame - GetActorLocation();
	PortalToNextPlayerPosition = PortalToNextPlayerPosition.GetSafeNormal(0.00001f);

	// Player is behind the portal
	if (UKismetMathLibrary::Dot_VectorVector(PortalToNextPlayerPosition, GetActorForwardVector()) < 0.f &&
		bCanTeleport)
	{
		TeleportPlayer();
		
		bCanTeleport = false;
		Target->bCanTeleport = false;

		// Re-enable can player teleport from this portal in 1 second. Avoids teleporting player to same portal
		FTimerHandle EnableCanTeleportPlayer;
		GetWorldTimerManager().SetTimer(EnableCanTeleportPlayer, this,
			&APortal::EnableCanTeleportPlayer,1.f,false);
	}
}

void APortal::SetPortalView()
{
	// if (this != nullptr && Target != nullptr )
	// {
	// 	CaptureComponent->TextureTarget = Target->GetRenderTarget();
	// 	CaptureComponent->CaptureScene();
	// }

	//For each portal
	for (TActorIterator<APortal> It(GetWorld()); It; ++It)
	{
		APortal* Portal = *It;
		//Get target of each portal
		APortal* TargetPortal = Portal->GetTarget();
				
		if (Portal != nullptr && TargetPortal != nullptr)
		{
			CaptureComponent->TextureTarget = Portal->GetRenderTarget();
			CaptureComponent->CaptureScene();
		}
	}
}

void APortal::ChangeObjectVelocity_Implementation()
{
}

void APortal::ResizeRenderTarget(UTextureRenderTarget2D* TextureRenderTarget, float SizeX, float SizeY)
{
	if (RenderTarget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Render Target used to resize texture is nullptr"));
		return;
	}
	RenderTarget->ResizeTarget(SizeX, SizeY);
}

void APortal::EnableCanTeleportPlayer()
{
	bCanTeleport = true;
	Target->bCanTeleport = true;
}


APortal* APortal::GetTarget() const
{
	return Target;
}

void APortal::SetTarget(APortal* NewTarget)
{
	if (NewTarget != nullptr)
	{
		Target = NewTarget;
	}
}

void APortal::SetPortalSurface(UPrimitiveComponent* Surface)
{
	PortalSurface = Surface;
}

FVector APortal::GetBoxComponentForwardVector()
{
	return BoxComponent->GetForwardVector();
}

FVector APortal::GetBoxComponentLocation()
{
	return BoxComponent->GetComponentLocation();
}

UTextureRenderTarget2D* APortal::GetRenderTarget()
{
	return RenderTarget;
}
