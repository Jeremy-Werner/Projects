// Fill out your copyright notice in the Description page of Project Settings.
#include "Monster.h"
#include "Runtime/Engine/Classes/Engine/SkeletalMeshSocket.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Avatar.h"
#include "MeleeWeapon.h"

AMeleeWeapon* MeleeWeapon;

// Sets default values
AMonster::AMonster(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Speed = 20;
	HitPoints = 20;
	Exp = 0;
	BPLoot = NULL;
	BaseAttackDamage = 1;
	AttackTimeout = 1.5f;
	TimeSinceLastAttack = 0;

	SightSphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("Sight Sphere"));
	SightSphere->AttachTo(RootComponent);

	AttackRangeSphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("Proximity Sphere"));
	AttackRangeSphere->AttachTo(RootComponent);

	Hitbox = ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("Hit Box"));
	Hitbox->SetCapsuleHalfHeight(100);
	Hitbox->SetCapsuleRadius(40);
	Hitbox->SetSimulatePhysics(true);
	Hitbox->AttachTo(RootComponent);
	
}

bool AMonster::isInAttackRange()
{
	AAvatar * avatar = Cast<AAvatar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!avatar) {
		return false;
	}

	FVector toPlayer = avatar->GetActorLocation() - GetActorLocation();

	float distanceToPlayer = toPlayer.Size();

	if (distanceToPlayer < AttackRangeSphere->GetScaledSphereRadius())
	{
		return true;
	}
	else {
		return false;
	}
}

void AMonster::SwordSwung()
{
	if (MeleeWeapon)
	{
		MeleeWeapon->Swing();
	}
}

void AMonster::SwordRest()
{
	if (MeleeWeapon)
	{
		MeleeWeapon->Rest();
	}
}

// Called when the game starts or when spawned
void AMonster::PostInitializeComponents()
{			
	Super::PostInitializeComponents();

		if (BPWeapon) 
	{
		MeleeWeapon = GetWorld()->SpawnActor<AMeleeWeapon>(BPWeapon, FVector(), FRotator());

		if (MeleeWeapon)
		{
			MeleeWeapon->WeaponHolder = this;
			const USkeletalMeshSocket* socket = GetMesh()->GetSocketByName("RightHandSocket");
			socket->AttachActor(MeleeWeapon, GetMesh());
		}
	}
}

float AMonster::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	HitPoints -= Damage;

	if (HitPoints <= 0) 
	{
		Destroy();
		if (MeleeWeapon)
		{
			MeleeWeapon->DestroyActor();
		}
	}

	return 0.0f;
}

void AMonster::Attack(AActor * thing)
{
	if (MeleeWeapon) 
	{
		MeleeWeapon->Swing();
	}
	else if (BPBullet) 
	{
		FVector fwd = GetActorForwardVector();
		FVector nozzle = GetMesh()->GetBoneLocation("RightHand");

		nozzle += fwd *250;

		FVector toOpponent = thing->GetActorLocation() - nozzle;
		toOpponent.Normalize();

		ABullet* bullet = GetWorld()->SpawnActor<ABullet>(BPBullet, nozzle, RootComponent->GetComponentRotation());

		if (bullet) 
		{
			bullet->Firer = this;
			bullet->Mesh->AddImpulse(fwd*BulletLaunchImpulse);
			//bullet->ProxSphere->AddImpulse(fwd*BulletLaunchImpulse);
		}
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("Bullet could not be spawned"));
		}
	}
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AAvatar * avatar = Cast<AAvatar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!avatar)
		return;

	FVector toPlayer = avatar->GetActorLocation() - GetActorLocation();
	
	float distanceToPlayer = toPlayer.Size();
	//If Player not in sight return
	if (isInSightRange(distanceToPlayer))
	{
		// If player is not in sight do not chase
		return;
	}

	toPlayer /= distanceToPlayer; // normalize the vector

	

	//Rotate to face player
	FRotator toPlayerRotation = toPlayer.Rotation();
	toPlayerRotation.Pitch = 0;
	RootComponent->SetWorldRotation(toPlayerRotation);

	if (isInAttackRange()) 
	{
		if (!TimeSinceLastAttack) 
		{
			Attack(avatar);
		}

		TimeSinceLastAttack += DeltaTime;
		if (TimeSinceLastAttack > AttackTimeout) 
		{
			TimeSinceLastAttack = 0;
		}
		return;
	}
	else 
	{
		//Move monster to player
		AddMovementInput(toPlayer, Speed*DeltaTime);
	}
}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

