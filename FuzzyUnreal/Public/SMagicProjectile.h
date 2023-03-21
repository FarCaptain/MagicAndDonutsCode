// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "GameFramework/Actor.h"
#include "SMagicProjectile.generated.h"

UCLASS()
class FUZZYUNREAL_API ASMagicProjectile : public ASProjectileBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ASMagicProjectile();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioComponent* AudioComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundBase* ImpactSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float damage;

	UFUNCTION()
	virtual void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult);

	// virtual void OnActorHit(UPrimitiveComponent* HitCompoment, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	virtual void Explode_Implementation() override;
	virtual void PostInitializeComponents() override;
};
