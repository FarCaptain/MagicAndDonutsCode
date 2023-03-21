// Fill out your copyright notice in the Description page of Project Settings.


#include "FuzzyUnreal/Public/SCharacter.h"

#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "SInteractionComponent.h"
#include "SMagicProjectile.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyLibraries/SPeriodicNumbers.h"

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float MaxHealth,
                                  float Delta)
{
	if(Delta < 0.f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
		if(NewHealth <= 0.0f)
		{
			APlayerController* PC = Cast<APlayerController>(GetController());
			DisableInput(PC);
			GetCapsuleComponent()->SetGenerateOverlapEvents(false);
		}
	}
}

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
	

	InteractionComponent = CreateDefaultSubobject<USInteractionComponent>("InteractionComponent");
	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>("AttributeComponent");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
	TurnbackSpeed = 3.f;
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComponent->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

void ASCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if(ensureAlways(ClassToSpawn))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
		
		UGameplayStatics::SpawnEmitterAttached(AttachedParticle, GetMesh(), "Muzzle_01", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
		UGameplayStatics::PlayWorldCameraShake(this, CameraShakeClass, GetActorLocation(), 0.5f, 1050.f);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		FCollisionShape Shape;
		Shape.SetSphere(10.f);
		
		FCollisionObjectQueryParams  ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

		FHitResult Hit;
		FVector CamLocation = CameraComponent->GetComponentLocation();
		FVector EndLocation = CamLocation + (GetControlRotation().Vector() * 5000.f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		
		if(GetWorld()->SweepSingleByObjectType(Hit, CamLocation, EndLocation, FQuat::Identity, ObjectQueryParams, Shape, QueryParams))
		{
			EndLocation = Hit.ImpactPoint;
			DrawDebugString(GetWorld(), GetActorLocation(), GetDebugName(Hit.GetActor()), nullptr, FColor::White, 4.0f, true);
		}
	
		FRotator ProjectileRot = UKismetMathLibrary::FindLookAtRotation(HandLocation, EndLocation);
	
		FTransform SpawnTM = FTransform( ProjectileRot,HandLocation);
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
	}
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(TurnbackCurve)
	{
		// Setup the turnback timeline
		FOnTimelineFloat ProgressUpdate;
		ProgressUpdate.BindUFunction(this, FName("TurnbackUpdate"));

		FOnTimelineEvent FinishedEvent;
		FinishedEvent.BindUFunction(this, FName("TurnbackFinished"));

		TurnbackTimeline.AddInterpFloat(TurnbackCurve, ProgressUpdate);
		TurnbackTimeline.SetTimelineFinishedFunc(FinishedEvent);
	}
}

void ASCharacter::TurnbackUpdate(float Alpha)
{
	FRotator NewRot = GetActorRotation();
	// NewRot.Yaw = FMath::Lerp(OriginalYaw, GetControlRotation().Yaw, Alpha);
	NewRot.Yaw = SPeriodicNumbers::circ_shortlerp(Alpha, OriginalYaw, GetControlRotation().Yaw);
	
	SetActorRotation(NewRot);
}

void ASCharacter::TurnbackFinished()
{
	
}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	
	AddMovementInput(RightVector, Value);
}

void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);

	ControllerYaw = GetControlRotation().Yaw;
	OriginalYaw = GetActorRotation().Yaw;
	TurnbackTimeline.PlayFromStart();
	
	// Handle -> Name of Timer, , The callback Func, time
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectile(ProjectileClass);
}

void ASCharacter::Teleport()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::Teleport_TimeElapsed, 0.2f);
}

void ASCharacter::Teleport_TimeElapsed()
{
	SpawnProjectile(DashProjectileClass);
}

void ASCharacter::ShootBlackhole()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::ShootBlackhole_TimeElapsed, 0.2f);
}

void ASCharacter::ShootBlackhole_TimeElapsed()
{
	SpawnProjectile(BlackholeProjectileClass);
}

void ASCharacter::PrimaryInteract()
{
	if(InteractionComponent)
	{
		InteractionComponent->PrimaryInteract();
	}
}


// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TurnbackTimeline.TickTimeline(TurnbackSpeed * DeltaTime);
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ASCharacter::Teleport);
	PlayerInputComponent->BindAction("Blackhole", IE_Pressed, this, &ASCharacter::ShootBlackhole);
	
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
}

