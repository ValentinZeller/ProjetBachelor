// Fill out your copyright notice in the Description page of Project Settings.


#include "CPlayerController.h"
#include "CAPlayerPawn.h"

void ACPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForwardBackward",this,&ACPlayerController::MoveForwardBackward);
	InputComponent->BindAxis("MoveRightLeft",this,&ACPlayerController::MoveRightLeft);
	InputComponent->BindAxis("MoveUpDown",this,&ACPlayerController::MoveUpDown);
	InputComponent->BindAxis("VerticalMovement",this,&ACPlayerController::VerticalMovement);
	InputComponent->BindAxis("HorizontalMovement",this,&ACPlayerController::HorizontalMovement);

	InputComponent->BindAction("ChangeCameraView", IE_Pressed,this, &ACPlayerController::ChangeCameraView);
	InputComponent->BindAction("Launch", IE_Released, this, &ACPlayerController::Launch);
	InputComponent->BindAction("Launch", IE_Pressed, this, &ACPlayerController::Charging);
	InputComponent->BindAction("StopAim", IE_Pressed, this, &ACPlayerController::StopAim);
}

void ACPlayerController::MoveForwardBackward(float value)
{
	ACAPlayerPawn* pawn = Cast<ACAPlayerPawn>(this->GetPawn());

	if (pawn)
	{
		pawn->MoveForwardBackward(value);
	}
}

void ACPlayerController::MoveRightLeft(float value)
{
	ACAPlayerPawn* pawn = Cast<ACAPlayerPawn>(this->GetPawn());

	if (pawn)
	{
		pawn->MoveRightLeft(value);
	}

}

void ACPlayerController::MoveUpDown(float value)
{
	ACAPlayerPawn* pawn = Cast<ACAPlayerPawn>(this->GetPawn());

	if (pawn)
	{
		pawn->MoveUpDown(value);
	}
}

void ACPlayerController::VerticalMovement(float value)
{
	ACAPlayerPawn* pawn = Cast<ACAPlayerPawn>(this->GetPawn());

	if (pawn)
	{
		pawn->VerticalMovement(value);
	}
}

void ACPlayerController::HorizontalMovement(float value)
{
	ACAPlayerPawn* pawn = Cast<ACAPlayerPawn>(this->GetPawn());

	if (pawn)
	{
		pawn->HorizontalMovement(value);
	}

}

void ACPlayerController::ChangeCameraView()
{
	ACAPlayerPawn* pawn = Cast<ACAPlayerPawn>(this->GetPawn());

	if (pawn)
	{
		pawn->ChangeCameraView();
	}
}

void ACPlayerController::Launch()
{
	ACAPlayerPawn* pawn = Cast<ACAPlayerPawn>(this->GetPawn());

	if (pawn)
	{
		pawn->Launch();
	}
}

void ACPlayerController::Charging()
{
	ACAPlayerPawn* pawn = Cast<ACAPlayerPawn>(this->GetPawn());

	if (pawn)
	{
		pawn->Charging();
	}
}

void ACPlayerController::StopAim()
{
	ACAPlayerPawn* pawn = Cast<ACAPlayerPawn>(this->GetPawn());

	if (pawn)
	{
		pawn->StopAim();
	}
}


