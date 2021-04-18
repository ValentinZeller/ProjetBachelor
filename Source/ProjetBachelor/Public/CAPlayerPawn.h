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
	
	float m_fMovementMultiplier;
	float m_fMaxSpeed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCameraComponent* m_AimCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCameraComponent* m_FreeView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USpringArmComponent* m_armComponent;


public:	
	UPROPERTY(EditDefaultsOnly, Category = "ActorSpawning")
		TSubclassOf<ACCelestialBody> SpawnedBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_bIsCharging;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_bIsFreeView;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_fInitialSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACCelestialBody* m_Spawned;
	
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

	float UpdateLaunchingSpeed(float value);
	bool IsLaunching();
};
