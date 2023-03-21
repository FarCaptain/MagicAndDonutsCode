// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include <SAttributeComponent.h>
#include <solver/PxSolverDefs.h>

#include "DrawDebugHelpers.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->GetClass() == this->GetClass())
		return;
	
	UE_LOG(LogTemp, Warning, TEXT("Overlapped"));
	 if(OtherActor && OtherActor != GetInstigator())
	 {
	 	
	 	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	 	if(AttributeComp)
	 	{
	 		AttributeComp->ApplyHealthChange(GetInstigator(), -damage);
	 	}
	 	Explode();

	 	// FString CombinedString = FString::Printf(TEXT("Overlapped at location: %s"), *SweepResult.ImpactPoint.ToString());
	 	// DrawDebugString(GetWorld(), SweepResult.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);

	 	// FString CombinedString = FString::Printf(TEXT("OtherActor: %s While Instigator: %s"), *GetDebugName(OtherActor), *GetDebugName(GetInstigator()));
	 	// DrawDebugString(GetWorld(), SweepResult.ImpactPoint, CombinedString, nullptr, FColor::Blue, 5.0f, true);
	 }
}

void ASMagicProjectile::Explode_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), GetActorRotation());
	Super::Explode_Implementation();
}

void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
}

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
	AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
	AudioComponent->SetupAttachment(SphereComp);
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;
	MovementComp->InitialSpeed = 8000.0f;
	InitialLifeSpan = 15.f;

	damage = 20.f;
}

