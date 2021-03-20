// Fill out your copyright notice in the Description page of Project Settings.


#include "CCelestialBody.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

#define GRAVITY 269.976532

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

	if (this->FindComponentByClass<UStaticMeshComponent>()) {
		Mesh = this->FindComponentByClass<UStaticMeshComponent>();
		Mesh->SetSimulatePhysics(true);
		Mesh->SetEnableGravity(false);

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

	if (!bIsLaunched) {
		Mesh->AddForce(m_fInitialDirection * m_fInitialSpeed);
		bIsLaunched = true;
	} else {

		TArray<AActor*> SpaceActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACCelestialBody::StaticClass(), SpaceActors);
		for (AActor* SpaceActor : SpaceActors) {
			UPrimitiveComponent* SpacePrimitive = SpaceActor->FindComponentByClass<UPrimitiveComponent>();
			if (SpacePrimitive->IsSimulatingPhysics()) {
				SpacePrimitive->AddForce(GetDirection(SpacePrimitive) * GetDistanceBetweenBodies(SpaceActor) * GetBodyMass(SpacePrimitive) * m_fMultiply);
			}
		}
	}

}

float ACCelestialBody::GetDistanceBetweenBodies(AActor* SpaceActor)
{
	float fDistance = 0;
	if (SpaceActor != this) {
		fDistance = GRAVITY / GetDistanceTo(SpaceActor);
	}
	return fDistance;
}

float ACCelestialBody::GetBodyMass(UPrimitiveComponent* SpacePrimitive)
{
	float fMass = 0;
	fMass = SpacePrimitive->GetMass() * SpacePrimitive->GetMassScale();
	return fMass;
}

FVector ACCelestialBody::GetDirection(UPrimitiveComponent* SpacePrimitive)
{
	FVector vDirection = FVector(0);

	vDirection = this->FindComponentByClass<UPrimitiveComponent>()->GetComponentLocation() - SpacePrimitive->GetComponentLocation();
	
	return vDirection;
}

