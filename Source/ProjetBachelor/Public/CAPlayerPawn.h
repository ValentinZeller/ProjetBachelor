// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CCelestialBody.h"

#include <Runtime/Engine/Classes/Camera/CameraComponent.h>
#include <Runtime/Engine/Classes/GameFramework/SpringArmComponent.h>

#include "CAPlayerPawn.generated.h"

UCLASS()
class PROJETBACHELOR_API ACAPlayerPawn : public APawn
{
	GENERATED_BODY()

private:
	//Valeur scalaire pour les déplacements de la caméra libre
	float m_fMovementMultiplier;
	//Vitesse maximale
	float m_fMaxSpeed;
	//Vitesse changeable
	bool m_bIsSpeedChangeable;
	//Timer pour l'apparition du fantome de lancer
	FTimerHandle ghostTimer;

	bool m_bCanLaunch;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Caméra de viser/lancer
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCameraComponent* m_AimCamera;
	//Caméra libre
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCameraComponent* m_FreeView;
	//Spring Arm pour la caméra de viser
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USpringArmComponent* m_armComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialInterface* SpawnedMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bDestroySpawned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bHasSpawned;


public:	
	//Type BP_CelestialBody pour le lancer
	UPROPERTY(EditDefaultsOnly, Category = "ActorSpawning")
		TSubclassOf<ACCelestialBody> SpawnedBP;
	//Type BP_Ghost pour le fantome de lancer
	UPROPERTY(EditDefaultsOnly, Category = "ActorSpawning")
		TSubclassOf<ACCelestialBody> GhostBP;
	
	//Etat chargé
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_bIsCharging;
	//Etat caméra libre
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_bIsFreeView;
	//Vitesse initiale au lancer
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_fInitialSpeed;
	//Référence de l'acteur spawné
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACCelestialBody* m_Spawned;
	//Sphere du joueur servant de visuel
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* m_Sphere;

	// Sets default values for this pawn's properties
	ACAPlayerPawn();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForwardBackward(float value);
	void MoveRightLeft(float value);
	void MoveUpDown(float value);
	void VerticalMovement(float value);
	void HorizontalMovement(float value);
	void ChangeCameraView();
	void Launch();
	void StopAim();
	void Charging();
	void AfterLaunchVisibility();

	UFUNCTION(BlueprintCallable)
	float UpdateLaunchingSpeed(float value);
	bool IsLaunching();

	void LaunchGhost();
};
