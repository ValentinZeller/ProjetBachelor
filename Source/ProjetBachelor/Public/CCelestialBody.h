// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "CCelestialBody.generated.h"


UCLASS()
class PROJETBACHELOR_API ACCelestialBody : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float m_fInitialSpeed; //Vitesse initiale

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector m_fInitialDirection; // Direction initiale

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float m_fMultiply; // Facteur multipliant l'influence gravitationnel

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsLaunched; // Etat lancé

	UStaticMeshComponent* Mesh;

	// Sets default values for this actor's properties
	ACCelestialBody();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float GetDistanceBetweenBodies(AActor* SpaceActor);
	float GetBodyMass(UPrimitiveComponent* SpacePrimitive);
	FVector GetDirection(UPrimitiveComponent* SpacePrimitive);
	void Initialize(float multiply, float speed, FVector direction);
};
