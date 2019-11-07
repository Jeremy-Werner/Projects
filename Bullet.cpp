// Fill out your copyright notice in the Description page of Project Settings.

#include "Bullet.h"

// Sets default values
ABullet::ABullet(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	RootComponent = Mesh;

	ProxSphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("Proximity Sphere"));
	ProxSphere->AttachTo(RootComponent);

	ProxSphere->OnComponentBeginOverlap.AddDynamic(this, &ABullet::Prox);
	Damage = 1;
}

void ABullet::Prox_Implementation(UPrimitiveComponent * OverlappedComponent, AActor * otherActor, UPrimitiveComponent * otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (otherComp != otherActor->GetRootComponent()) 
	{
		return;
	}
	otherActor->TakeDamage(Damage, FDamageEvent(), NULL, this);
	Destroy();
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

