// Fill out your copyright notice in the Description page of Project Settings.


#include "CAPlayerPawn.h"
#include <Runtime/Engine/Classes/Components/TextRenderComponent.h>


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
	m_bIsSpeedChangeable = false;

	//Création des components et de la hierarchie pour le blueprint

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

// Mouvement avant/arrière de la caméra libre
void ACAPlayerPawn::MoveForwardBackward(float value)
{
	if (m_bIsFreeView) {
		FVector offset = FVector(value * m_fMovementMultiplier, 0, 0);
		m_FreeView->AddLocalOffset(offset);
	}
}

// Mouvement droite/gauche de la caméra libre
void ACAPlayerPawn::MoveRightLeft(float value)
{
	if (m_bIsFreeView) {
		FVector offset = FVector(0,value * m_fMovementMultiplier, 0);
		m_FreeView->AddLocalOffset(offset);
	}
}

// Mouvement haut/bas de la caméra libre
void ACAPlayerPawn::MoveUpDown(float value)
{
	if (m_bIsFreeView) {
		FVector offset = FVector(0,0,value * m_fMovementMultiplier);
		m_FreeView->AddLocalOffset(offset);
	}
}

// Mouvement vertical pour tourner la caméra ou initialiser la vitesse initiale de lancer
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
			if (m_bIsSpeedChangeable) {
				UpdateLaunchingSpeed(value);
			}
			
		}
	}
	
}

// Mouvement horizontal pour tourner la caméra
void ACAPlayerPawn::HorizontalMovement(float value)
{
	if ((m_bIsCharging == false) && (m_bIsFreeView == false)) {
		AddControllerYawInput(value);
	}
	else {
		if (m_bIsFreeView == true) {
			FVector rotation = FVector(0, 0, value);
			FQuat delta = FQuat::MakeFromEuler(rotation);
			m_FreeView->AddLocalRotation(delta);
		}
	}
}

// Changement entre caméra de viser et caméra libre
void ACAPlayerPawn::ChangeCameraView()
{
	//Si caméra libre, repasser en caméra de viser
	if (m_bIsFreeView) {
		m_bIsFreeView = false;
		m_FreeView->ResetRelativeTransform();
		m_FreeView->SetActive(false);
		m_AimCamera->SetActive(true);
	}
	//Sinon, passer en caméra libre
	else {
		m_bIsFreeView = true;
		m_bIsCharging = false;
		m_AimCamera->SetActive(false);
		m_FreeView->SetActive(true);
	}
}

// Lancer un corps céleste
void ACAPlayerPawn::Launch()
{	
	if (IsLaunching()) {

		//Vitesse non changeable temporairement
		m_bIsSpeedChangeable = false;

		//Si un corps celeste a déjà été spawn, le supprimer
		if (IsValid(m_Spawned)) {
			m_Spawned->Destroy();
		}

		//Stoper l'apparition de fantome
		GetWorldTimerManager().ClearTimer(ghostTimer);
		
		//Information de Spawn : toujours, peut importe les collisions
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//Spawn du corps céleste et initialisation
		m_Spawned = GetWorld()->SpawnActor<ACCelestialBody>(SpawnedBP,GetActorLocation(),GetActorRotation(),SpawnInfo);
		m_Spawned->Initialize(0.1, m_fInitialSpeed, GetActorForwardVector() * 2000);
		
		//Mise en place du custom depth pour illuminer le corps celeste quand il est en bon orbite
		UActorComponent* comp = m_Spawned->GetComponentByClass(UStaticMeshComponent::StaticClass());
		UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(comp);
		mesh->SetRenderCustomDepth(true);
		
		//Si la sphere est
		if (IsValid(m_Sphere)) {
			m_Sphere->SetVisibility(false);
		}
		
		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &ACAPlayerPawn::AfterLaunchVisibility, 1, false);
	}
}

// Arreter de viser
void ACAPlayerPawn::StopAim()
{
	m_bIsCharging = false;
	m_bIsSpeedChangeable = false;
	GetWorldTimerManager().ClearTimer(ghostTimer);
}

// Charger le lancer
void ACAPlayerPawn::Charging()
{
	if (m_bIsFreeView == false) {
		
		m_bIsCharging = true;
		m_bIsSpeedChangeable = true;

		//Lancer de fantome toutes les 5 secondes
		GetWorldTimerManager().SetTimer(ghostTimer, this, &ACAPlayerPawn::LaunchGhost, 5, true, 0.5f);

	}
}

// Après le lancer, rendre visible la sphere et état non chargé
void ACAPlayerPawn::AfterLaunchVisibility()
{
	m_Sphere->SetVisibility(true);
	m_bIsCharging = false;
}

// Mettre à jour la vitesse initiale de lancer
float ACAPlayerPawn::UpdateLaunchingSpeed(float value)
{
	float speed = FMath::Clamp(value * -10, 0.0f, m_fMaxSpeed);
	if (speed > 0) {
		m_fInitialSpeed = speed;
	}
	return m_fInitialSpeed;
}

// Savoir si le joueur est entrain de lancer
bool ACAPlayerPawn::IsLaunching()
{
	return ((m_bIsFreeView == false) && (m_bIsCharging == true));
}


// Lancer de fantome
void ACAPlayerPawn::LaunchGhost()
{
	ACCelestialBody* m_Ghost;

	//Information de Spawn : toujours, peut importe les collisions
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//Spawn du fantome et initialisation
	m_Ghost = GetWorld()->SpawnActor<ACCelestialBody>(GhostBP, GetActorLocation(), GetActorRotation(), SpawnInfo);
	m_Ghost->Initialize(0.1, m_fInitialSpeed, GetActorForwardVector() * 2000);

	m_Ghost->SetLifeSpan(4);
}

