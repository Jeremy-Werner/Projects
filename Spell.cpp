// Fill out your copyright notice in the Description page of Project Settings.

#include "Spell.h"

// Sets default values
ASpell::ASpell(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProxBox = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("ProxBox"));
	Particles = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("Particle System"));

	RootComponent = Particles;
	ProxBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	Duration = 3;
	DamagePerSecond = 1;
	timeAlive = 0;
}

// Called when the game starts or when spawned
void ASpell::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ProxBox) {
		TArray<AActor*> actors;
		ProxBox->GetOverlappingActors(actors);

		for (int c = 0; c < actors.Num(); c++)
		{
			if (actors[c] != Caster)
			{
				AMonster* monster = Cast<AMonster>(actors[c]);

				if (ProxBox)
				{
					if (monster && ProxBox->IsOverlappingComponent(monster->Hitbox))
					{
						monster->TakeDamage(DamagePerSecond*DeltaTime, FDamageEvent(), 0, this);
					}
				}
			}
		}
	}

	timeAlive += DeltaTime;
	if (timeAlive > Duration)
	{
		Destroy();
	}
}

