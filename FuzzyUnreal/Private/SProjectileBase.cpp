// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileBase.h"


#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASProjectileBase::ASProjectileBase()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	RootComponent = SphereComp;
	SphereComp->SetCollisionProfileName("Projectile");
	
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComponent");
	EffectComp->SetupAttachment(SphereComp);
	
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	
	DecalRotationOffset = FRotator(-90, 0, 0);
}

void ASProjectileBase::OnActorHit(UPrimitiveComponent* HitCompoment, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(DecalVFX)
	{
		UGameplayStatics::SpawnEmitterAttached(DecalVFX, OtherComp, NAME_None, Hit.Location, UKismetMathLibrary::Conv_VectorToRotator(Hit.Normal) + DecalRotationOffset, FVector(0.18f), EAttachLocation::KeepWorldPosition);
	}
	
	Explode();
	// UE_LOG(LogTemp, Warning, TEXT("hIT!"));
}

void ASProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentHit.AddDynamic(this, &ASProjectileBase::OnActorHit);
}


void ASProjectileBase::Explode_Implementation()
{
	if(ensure(!IsPendingKill()))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
		Destroy();
	}
}
