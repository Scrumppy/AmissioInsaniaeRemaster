// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PlayerController/MainPlayerController.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Portal.generated.h"

UCLASS()
class GRAVITYMECHANIC_V2_API APortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortal();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	//Status of the Portal (being visualized by the player or not)
	UFUNCTION(BlueprintPure, Category="Portal")
	bool IsActive();

	UFUNCTION(BlueprintCallable, Category="Portal")
	void SetActive( bool NewActive );

	void TeleportPlayer();

	void CheckCanTeleport();

	/** Function to resize the scene capture to match the screen's size */
	UFUNCTION(BlueprintCallable, Category = "SceneCapture", meta = (BlueprintProtected = "true"))
	void ResizeRenderTarget(UTextureRenderTarget2D* TextureRenderTarget, float SizeX, float SizeY);
	
	UFUNCTION(BlueprintCallable, Category = "SceneCapture", meta = (BlueprintProtected = "true"))
	void SetPortalView();

	UFUNCTION(BlueprintNativeEvent, Category = "Teleport")
	void ChangeObjectVelocity();

	//Getters & Setters

	UFUNCTION(BlueprintCallable, Category = "Setters")
	void SetTarget(APortal* NewTarget);

	UFUNCTION(BlueprintCallable, Category = "Getters")
	APortal* GetTarget() const;

	UFUNCTION(BlueprintCallable, Category = "Setters")
	void SetPortalSurface(UPrimitiveComponent* Surface);

	UFUNCTION(BlueprintPure, Category = "Getters")
	FVector GetBoxComponentLocation();

	UFUNCTION(BlueprintPure, Category = "Getters")
	FVector GetBoxComponentForwardVector();

	UFUNCTION(BlueprintPure, Category = "Getters")
	UTextureRenderTarget2D* GetRenderTarget();

	UFUNCTION(BlueprintPure, Category = "Getters")
	FORCEINLINE USceneCaptureComponent2D* GetCaptureComponent() const {return CaptureComponent;}

	UFUNCTION(BlueprintPure, Category = "Getters")
	FORCEINLINE USceneComponent* GetBackFacingSceneComponent() const { return BackFacingVector; }

	UFUNCTION(BlueprintPure, Category = "Getters")
	FORCEINLINE UArrowComponent* GetArrowComponent() const { return ArrowComponent; }
	
	/** Property used to choose the quality of the portal's camera. 1 Means screen resolution, 0.5 half, etc.
 * Lower to increase performance */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SceneCapture")
	float PortalCameraQuality;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Components

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Capture)
	USceneCaptureComponent2D* CaptureComponent;
	
	//Portal Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Portal)
	UStaticMeshComponent* PortalMesh;

	//Portal Collision
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Portal)
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Other")
	UArrowComponent* ArrowComponent;

	//Render target and other capture related components
	UPROPERTY(EditAnywhere, Category = Capture)
	UTextureRenderTarget2D* RenderTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Capture)
	UMaterialInterface* Material;
	UPROPERTY(EditAnywhere, Category = Capture)
	UMaterialInstanceDynamic* MaterialInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Capture)
	APortal* Target;

	// Character that is inside this portal
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actors In Portal", meta = (BlueprintProtected = "true"))
	class APlayerCharacter* CharacterInsidePortal;

	// Character that is inside this portal
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actors In Portal", meta = (BlueprintProtected = "true"))
	AActor* ObjectInsidePortal;

	UFUNCTION(BlueprintCallable, Category = "Portal", meta = (BlueprintProtected = "true"))
	void EnableCanTeleportPlayer();

	
private:
	FVector LastPosition;
	
	bool bIsActive;
	
	bool bLastInFront;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool bOverlapping;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool bCanTeleport = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool bCanTeleportObject = false;
	
	UPrimitiveComponent* PortalSurface;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal", meta = (AllowPrivateAccess = "true"))
	USceneComponent* BackFacingVector;
};

