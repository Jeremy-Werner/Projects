// Fill out your copyright notice in the Description page of Project Settings.

#include "Avatar.h"
#include "MyHUD.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Engine.h"
#include "PickUpItem.h"

// Sets default values
AAvatar::AAvatar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAvatar::BeginPlay()
{
	Super::BeginPlay();	
}

float AAvatar::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (health > 0) 
	{
		health -= Damage;
			if (health<= 0) 
			{
				DisableInput(Cast<APlayerController>(this));
			}

	}

	knockBack = GetActorLocation() - DamageCauser->GetActorLocation();
	knockBack.Normalize();
	knockBack *= Damage * 500; // knock back proprtional to damage
	return 0.0f;
}



// Called every frame
void AAvatar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddMovementInput(knockBack, 1.f);
	knockBack *= 0.5f;//half knock back each frame

}

void AAvatar::Pickup(APickUpItem * item)
{
	if (BackPack.Find(item->itemName)) 
	{
		//if exists in backpack increase quantity
		BackPack[item->itemName] += item->Quantity;
	}
	else 
	{
		//the item was not in the back pack add now
		BackPack.Add(item->itemName, item->Quantity);
		// record ref to the tex the first time picked up
		Icons.Add(item->itemName, item->Icon);

		Spells.Add(item->itemName, item->Spell);
	}
}

void AAvatar::ToggleInventory()
{
	//get controller and hud
	APlayerController* PController = GetWorld()->GetFirstPlayerController();
	AMyHUD* hud = Cast<AMyHUD>(PController->GetHUD());

	if (inventoryShowing) 
	{	
		hud->clearWidgets();
		inventoryShowing = false;
		PController->bShowMouseCursor = false;
		return;
	}

	inventoryShowing = true;
	PController->bShowMouseCursor = true;
	for (TMap<FString, int>::TIterator it = BackPack.CreateIterator(); it; ++it) 
	{
		FString fs = it->Key + FString::Printf(TEXT(" x %d"), it->Value);
		UTexture2D* tex = NULL;
		if (Icons.Find(it->Key)) 
		{
			tex = Icons[it->Key];
		}
		Widget w( Icon(fs, tex), NULL);
		w.bpSpell = Spells[it->Key];
		hud->addWidget(w);
	}
}

// Called to bind functionality to input
void AAvatar::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	check(InputComponent);
	InputComponent->BindAxis("Forwards", this, &AAvatar::MoveForward);
	InputComponent->BindAxis("Strafe", this, &AAvatar::MoveRight);
	InputComponent->BindAxis("Yaw", this, &AAvatar::Yaw);
	InputComponent->BindAxis("Pitch", this, &AAvatar::Pitch);
	InputComponent->BindAction("Inventory", IE_Pressed, this,
		&AAvatar::ToggleInventory);
	InputComponent->BindAction("LeftClick", IE_Pressed, this,
		&AAvatar::MouseClicked);
	InputComponent->BindAction("RightClick", IE_Pressed, this,
		&AAvatar::MouseRightClick);
}

void AAvatar::MoveForward(float amount) {
	//Dont enter function if controller is not set up
	if (inventoryShowing)
	{
		//When inventory is showing dont allow player to move
		return;
	}

	if (Controller&&amount) {
		FVector fwd = GetActorForwardVector();
		//We call AddMovementInput to move the Character
		AddMovementInput(fwd, amount);
	}
}

void AAvatar::MoveRight(float amount)
{
	if (inventoryShowing)
	{
		//When inventory is showing dont allow player to move
		return;
	}
	
	if (Controller&&amount) 
	{
		FVector right = GetActorRightVector();

		AddMovementInput(right, amount);
	}
}

void AAvatar::Yaw(float amount)
{
	if (inventoryShowing) 
	{
		//When inventory is showing dont allow player to move and pass input to hud
		APlayerController* PController = GetWorld()->GetFirstPlayerController();
		AMyHUD* hud = Cast<AMyHUD>(PController->GetHUD());
		hud->MouseMovedX();
		return;
	}
	AddControllerYawInput(200.f * amount * GetWorld()->GetDeltaSeconds());
}

void AAvatar::Pitch(float amount)
{
	if (inventoryShowing)
	{
		//When inventory is showing dont allow player to move
		APlayerController* PController = GetWorld()->GetFirstPlayerController();
		AMyHUD* hud = Cast<AMyHUD>(PController->GetHUD());
		hud->MouseMovedY();
		return;
	}
	AddControllerPitchInput(200.f * amount * GetWorld()->GetDeltaSeconds());
}

void AAvatar::MouseClicked()
{
	if (inventoryShowing) {
		APlayerController* PController = GetWorld()->GetFirstPlayerController();
		AMyHUD* hud = Cast<AMyHUD>(PController->GetHUD());
		hud->MouseClicked();
	}
	else if(Spell)
		CastSpell(Spell);
}

void AAvatar::MouseRightClick()
{
	if (inventoryShowing) 
	{
		APlayerController* PController = GetWorld()->GetFirstPlayerController();
		AMyHUD* hud = Cast<AMyHUD>(PController->GetHUD());
		hud->MouseRightClicked();
	}
}

void AAvatar::CastSpell(UClass * bpSpell)
{
	ASpell* spell = GetWorld()->SpawnActor<ASpell>(bpSpell, FVector(0), FRotator(0));

	if (spell) 
	{
		spell->SetCaster(this);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Can't Cast Spell"));
}



