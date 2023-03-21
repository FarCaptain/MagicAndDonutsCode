// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"

#include "DrawDebugHelpers.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = StaticMeshComponent;

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("RadialForceComponent");
	RadialForceComponent->SetupAttachment(StaticMeshComponent);

	RadialForceComponent->Radius = 600.f;
	RadialForceComponent->ImpulseStrength = 2000.f;
	RadialForceComponent->bImpulseVelChange = true;

	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetCollisionObjectType(ECC_PhysicsBody);
	// StaticMeshComponent->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnBarrelHit);
	StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ASExplosiveBarrel::OnBarrelHit);
}

void ASExplosiveBarrel::OnBarrelHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult)
{
	RadialForceComponent->FireImpulse();

	UE_LOG(LogTemp, Log, TEXT("OnActorhit in Explosive Barrel"));
	UE_LOG(LogTemp, Warning, TEXT("OtherActor： %s, at game time %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

	FString CombinedString = FString::Printf(TEXT("Hit at location: %s"), *SweepResult.ImpactPoint.ToString());
	DrawDebugString(GetWorld(), SweepResult.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

