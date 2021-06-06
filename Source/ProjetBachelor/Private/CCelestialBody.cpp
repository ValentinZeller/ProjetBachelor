// Fill out your copyright notice in the Description page of Project Settings.


#include "CCelestialBody.h"
#include "Kismet/GameplayStatics.h"

#define GRAVITY 169.976532

// Sets default values
ACCelestialBody::ACCelestialBody()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_fInitialSpeed = 0;

	m_fInitialDirection = FVector(0);

	m_fMultiply = 1;

	bIsLaunched = false;
	
}

// Called when the game starts or when spawned
void ACCelestialBody::BeginPlay()
{
	Super::BeginPlay();
	
	Initialize(m_fMultiply, m_fInitialSpeed, m_fInitialDirection);
	//Récupération de la static mesh pour paramétrer la physique et enlever la gravité de base
	if (this->FindComponentByClass<UStaticMeshComponent>()) {
		Mesh = this->FindComponentByClass<UStaticMeshComponent>();
		Mesh->SetSimulatePhysics(true);
		Mesh->SetEnableGravity(false);
		UE_LOG(LogTemp,Warning,TEXT("%s"),*Mesh->GetName())
		//Lancement du corps céleste
		if (!bIsLaunched) {
			Mesh->AddForce(m_fInitialDirection * m_fInitialSpeed);
			bIsLaunched = true;
		}
		
	}
	
}

// Called every frame
void ACCelestialBody::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//Lancement du corps céleste
	if (!bIsLaunched) {
		if (!IsValid(Mesh))
		{
			if (this->FindComponentByClass<UStaticMeshComponent>()) {
				Mesh = this->FindComponentByClass<UStaticMeshComponent>();
				Mesh->SetSimulatePhysics(true);
				Mesh->SetEnableGravity(false);
			}
		}
		Mesh->AddForce(m_fInitialDirection * m_fInitialSpeed);
		bIsLaunched = true;

	} else {
		//S'il a été lancé, prendre en compte l'influence des autres corps célestes pour modifier la trajectoire

		TArray<AActor*> SpaceActors; //Tableau stockant les corps célestes
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACCelestialBody::StaticClass(), SpaceActors); //On récupére les corps célestes
		for (AActor* SpaceActor : SpaceActors) {
			//Pour chaque corp céleste du tableau, on récupère le primitive component
			UPrimitiveComponent* SpacePrimitive = SpaceActor->FindComponentByClass<UStaticMeshComponent>();

			if (IsValid(SpacePrimitive)) {

				if (SpacePrimitive->IsSimulatingPhysics()) {
					//On vérifie qu'il simule la physique et on change la trajectoire à partir des différents paramètres :
					//La direction, la distance entre les deux corps célestes, la masse et le facteur multipliant
					SpacePrimitive->AddForce(GetDirection(SpacePrimitive) * GetDistanceBetweenBodies(SpaceActor) * GetBodyMass(SpacePrimitive) * m_fMultiply);
				}
			}

		}
	}

}

//Récupère la distance entre les deux corps célestes et applique la constante de gravité
float ACCelestialBody::GetDistanceBetweenBodies(AActor* SpaceActor)
{
	float fDistance = 0;
	if (SpaceActor != this) {
		fDistance = GRAVITY / GetDistanceTo(SpaceActor);
	}
	return fDistance;
}

//Récupère la masse du corp céleste
float ACCelestialBody::GetBodyMass(UPrimitiveComponent* SpacePrimitive)
{
	float fMass = 0;
	fMass = SpacePrimitive->GetMass() * SpacePrimitive->GetMassScale();
	return fMass;
}

//Récupère la direction du corps céleste
FVector ACCelestialBody::GetDirection(UPrimitiveComponent* SpacePrimitive)
{
	FVector vDirection = FVector(0);

	vDirection = this->FindComponentByClass<UPrimitiveComponent>()->GetComponentLocation() - SpacePrimitive->GetComponentLocation();
	
	return vDirection;
}

//Initialisation des valeurs physiques du corps céleste
void ACCelestialBody::Initialize(float multiply, float speed, FVector direction)
{
	m_fMultiply = multiply;
	m_fInitialSpeed = speed;
	m_fInitialDirection = direction;
	bIsLaunched = false;
}

