// Fill out your copyright notice in the Description page of Project Settings.


#include "CAPlayerPawn.h"


// Sets default values
ACAPlayerPawn::ACAPlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_fInitialSpeed = 0;
	m_fMaxSpeed = 200;
	m_fMovementMultiplier = 50;
	m_bIsCharging = false;
	m_bIsFreeView = false;
	m_Spawned = nullptr;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	m_armComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	m_armComponent->SetupAttachment(Root);
	m_armComponent->TargetArmLength = 997.0f;
	m_armComponent->bUsePawnControlRotation = true;

	m_AimCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_AimCamera->SetupAttachment(m_armComponent);
	m_AimCamera->bUsePawnControlRotation = true;

	m_FreeView = CreateDefaultSubobject<UCameraComponent>(TEXT("freeView"));
	m_FreeView->SetupAttachment(m_AimCamera);
	m_FreeView->SetAutoActivate(false);
	m_FreeView->SetActive(false);

	
}

// Called when the game starts or when spawned
void ACAPlayerPawn::BeginPlay()
{
	Super::BeginPlay();


	
}

// Called every frame
void ACAPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void ACAPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACAPlayerPawn::MoveForwardBackward(float value)
{
	if (m_bIsFreeView) {
		FVector offset = FVector(value * m_fMovementMultiplier, 0, 0);
		m_FreeView->AddLocalOffset(offset);
	}
}

void ACAPlayerPawn::MoveRightLeft(float value)
{
	if (m_bIsFreeView) {
		FVector offset = FVector(0,value * m_fMovementMultiplier, 0);
		m_FreeView->AddLocalOffset(offset);
	}
}

void ACAPlayerPawn::MoveUpDown(float value)
{
	if (m_bIsFreeView) {
		FVector offset = FVector(0,0,value * m_fMovementMultiplier);
		m_FreeView->AddLocalOffset(offset);
	}
}

void ACAPlayerPawn::VerticalMovement(float value)
{
	if (!m_bIsCharging) {
		AddControllerPitchInput(value);
	}
	else {
		if (m_bIsFreeView == true) {
			FVector rotation = FVector(0, -value, 0);
			FQuat delta = FQuat::MakeFromEuler(rotation);
			m_FreeView->AddLocalRotation(delta);
		}
		else {
			UpdateLaunchingSpeed(value);
		}
	}
	
}

void ACAPlayerPawn::HorizontalMovement(float value)
{
	if ((m_bIsCharging == false) && (m_bIsFreeView == false)) {
		AddControllerYawInput(value);
	}
	else {
		if (m_bIsFreeView == true) {
			FVector rotation = FVector(0, 0,value);
			FQuat delta = FQuat::MakeFromEuler(rotation);
			m_FreeView->AddLocalRotation(delta);
		}
	}
}

void ACAPlayerPawn::ChangeCameraView()
{
	if (m_bIsFreeView) {
		m_bIsFreeView = false;
		m_FreeView->ResetRelativeTransform();
		m_FreeView->SetActive(false);
		m_AimCamera->SetActive(true);
	}
	else {
		m_bIsFreeView = true;
		m_bIsCharging = false;
		m_AimCamera->SetActive(false);
		m_FreeView->SetActive(true);
	}
}

void ACAPlayerPawn::Launch()
{	
	if (IsLaunching()) {
		
		
		if (IsValid(m_Spawned)) {
			m_Spawned->Destroy();
		}

		
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		m_Spawned = GetWorld()->SpawnActor<ACCelestialBody>(SpawnedBP,GetActorLocation(),GetActorRotation(),SpawnInfo);
		m_Spawned->Initialize(0.1, m_fInitialSpeed, GetActorForwardVector() * 2000);
		
	
		UActorComponent* comp = m_Spawned->GetComponentByClass(UStaticMeshComponent::StaticClass());
		UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(comp);
		mesh->SetRenderCustomDepth(true);
		
		m_Sphere->SetVisibility(false);
		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &ACAPlayerPawn::AfterLaunchVisibility, 1, false);
		
	}
}

void ACAPlayerPawn::StopAim()
{
	m_bIsCharging = false;
}

void ACAPlayerPawn::Charging()
{
	if (m_bIsFreeView == false) {
		
		m_bIsCharging = true;
	}
}

void ACAPlayerPawn::AfterLaunchVisibility()
{
	m_Sphere->SetVisibility(true);
	m_bIsCharging = false;
}

float ACAPlayerPawn::UpdateLaunchingSpeed(float value)
{
	m_fInitialSpeed = FMath::Clamp(value * -10 ,0.0f,m_fMaxSpeed);
	return m_fInitialSpeed;
}

bool ACAPlayerPawn::IsLaunching()
{
	return ((m_bIsFreeView == false) && (m_bIsCharging == true));
}

