// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJETBACHELOR_API ACPlayerController : public APlayerController
{
	GENERATED_BODY()

	void SetupInputComponent();
	void MoveForwardBackward(float value);
	void MoveRightLeft(float value);
	void MoveUpDown(float value);
	void VerticalMovement(float value);
	void HorizontalMovement(float value);
	void ChangeCameraView();
	void Launch();
	void Charging();
	void StopAim();

	void ReturnToMenu();
};
