// Fill out your copyright notice in the Description page of Project Settings.

#include "MeleeWeapon.h"

// Sets default values
AMeleeWeapon::AMeleeWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttackDamage = 5;
	Swinging = false;
	WeaponHolder = NULL;

	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	RootComponent = Mesh;

	ProxBox = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("ProxBox"));
	ProxBox->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::Prox);
	ProxBox->AttachTo(RootComponent);
}

void AMeleeWeapon::Prox_Implementation(UPrimitiveComponent * OverlappedComponent, AActor * otherActor, UPrimitiveComponent * otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// don't hit non root components
	if (otherComp != otherActor->GetRootComponent()) 
	{
		return;
	}

	//Avoid hitting things while not swinging, avoid hitting self and double hits
	if (Swinging && otherActor != WeaponHolder && !ThingsHit.Contains(otherActor)) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit"));
		otherActor->TakeDamage(AttackDamage + WeaponHolder->BaseAttackDamage, FDamageEvent(), NULL, this);
		ThingsHit.Add(otherActor);
	}
}

void AMeleeWeapon::Swing()
{
	ThingsHit.Empty();
	Swinging = true;
}

void AMeleeWeapon::Rest()
{
	ThingsHit.Empty();
	Swinging = false;
}

// Called when the game starts or when spawned
void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

