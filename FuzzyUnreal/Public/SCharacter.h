// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USAttributeComponent;
class USInteractionComponent;
class USpringArmComponent;
class UCameraComponent;
class UCurveFloat;

UCLASS()
class FUZZYUNREAL_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Effect")
	UParticleSystem* AttachedParticle;

	UPROPERTY(EditAnywhere, Category = "Effect")
	TSubclassOf<UCameraShakeBase> CameraShakeClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = "Skills")
	TSubclassOf<AActor> DashProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Skills")
	TSubclassOf<AActor> BlackholeProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Aim")
	UCurveFloat* TurnbackCurve;

	UPROPERTY(EditDefaultsOnly, Category = "Aim")
	float TurnbackSpeed;

	float ControllerYaw;
	float OriginalYaw;
	
	FTimeline TurnbackTimeline;
	
	FTimerHandle TimerHandle_PrimaryAttack;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float MaxHealth, float Delta);

	virtual void PostInitializeComponents() override;
	
	UFUNCTION()
	void TurnbackUpdate(float Alpha);
	UFUNCTION()
	void TurnbackFinished();

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComponent;

	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void PrimaryAttack();
	void PrimaryAttack_TimeElapsed();

	void Teleport();
	void Teleport_TimeElapsed();

	void ShootBlackhole();
	void ShootBlackhole_TimeElapsed();
	
	void PrimaryInteract();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
