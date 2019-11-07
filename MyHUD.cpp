// Fill out your copyright notice in the Description page of Project Settings.

#include "MyHUD.h"
#include "Engine.h"
//#include "Runtime/Engine/Classes/Engine/Canvas.h"
#include "Engine/World.h"
#include "Avatar.h"
#include "Kismet/GameplayStatics.h"
//#include "Runtime/UMG/Public/UMG.h"
//#include "Runtime/UMG/Public/UMGStyle.h"
//#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
//#include "Runtime/UMG/Public/IUMGModule.h"
//#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

void AMyHUD::DrawHealthBar()
{
	AAvatar* avatar = Cast<AAvatar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	float barWidth = 200, barHeight = 50, barPad = 12, barMargin = 50;
	float percHp = avatar->getHealth() / avatar->getMaxHealth();
	DrawRect(FLinearColor(0, 0, 0, 1), Canvas->SizeX - barWidth - barPad - barMargin, Canvas->SizeY - barHeight - barPad - barMargin, 
		barWidth + 2 * barPad, barHeight + 2 * barPad);
	DrawRect(FLinearColor(1 - percHp, percHp, 0, 1), Canvas->SizeX - barWidth - barMargin, 
		Canvas->SizeY - barHeight - barMargin, barWidth*percHp, barHeight);
}

void AMyHUD::MouseClicked()
{
	FVector2D mouse;
	APlayerController* PController = GetWorld()->GetFirstPlayerController();
	PController->GetMousePosition(mouse.X, mouse.Y);
	heldWidget = NULL;
	//Checks if mouse hits widgets
	for (int c = 0; c < widgets.Num(); c++) 
	{
		if (widgets[c].hit(mouse)) 
		{
			heldWidget = &widgets[c];//Save Widget
			return;
		}
	}
}

void AMyHUD::MouseMovedY()
{
	static FVector2D lastMouse;
	FVector2D thisMouse, dMouse;
	APlayerController* PController = GetWorld()->GetFirstPlayerController();
	PController->GetMousePosition(thisMouse.X, thisMouse.Y);
	dMouse = thisMouse - lastMouse;
	float time = PController->GetInputKeyTimeDown(
		EKeys::LeftMouseButton);
	if (time > 0.f && heldWidget)
	{
		//mouse is being held down so move widget
		heldWidget->pos.Y += dMouse.Y;
	}
	lastMouse = thisMouse;
}

void AMyHUD::MouseMovedX()
{
	static FVector2D lastMouse;
	FVector2D thisMouse, dMouse;
	APlayerController* PController = GetWorld()->GetFirstPlayerController();
	PController->GetMousePosition(thisMouse.X, thisMouse.Y);
	dMouse = thisMouse - lastMouse;
	float time = PController->GetInputKeyTimeDown(
		EKeys::LeftMouseButton);
	if (time > 0.f && heldWidget) 
	{
		//mouse is being held down so move widget
		heldWidget->pos.X += dMouse.X;
	}
	lastMouse = thisMouse;
}



void AMyHUD::DrawHUD()
{
	Super::DrawHUD();

	float ScreenX = 750;
	float ScreenY = 400;

	dims.X = Canvas->SizeX;
	dims.Y = Canvas->SizeY;

	DrawHealthBar();
	DrawWidgets();

	for (int c = messages.Num() - 1; c >= 0; c--) {
		float outputWidth, outputHeight, pad = 10.f;
		GetTextSize(messages[c].message, outputWidth, outputHeight, hudFont, 1.f);

		float messageH = outputHeight + 2.f*pad;
		float x = 0.f, y = c * messageH;
	  
	
		// black backing
		DrawRect(FLinearColor::Black, x, y, Canvas->SizeX, messageH);
		// draw message
        DrawText(messages[c].message, messages[c].color ,x+pad+35, y+pad, hudFont, 2, false); 
		//Draw Icon
		DrawTexture(messages[c].icon, x, y, messageH, messageH, 0, 0, 1, 1);
		messages[c].time -= GetWorld()->GetDeltaSeconds();

		if (messages[c].time < 0) {
			messages.RemoveAt(c);
		}
	}
	
}

void AMyHUD::DrawWidgets()
{
	for (int c = 0; c < widgets.Num(); c++) 
	{
		DrawTexture(widgets[c].icon.tex, widgets[c].pos.X, widgets[c].pos.Y, widgets[c].size.X, widgets[c].size.Y, 0, 0, 1, 1);
		DrawText(widgets[c].icon.name, FLinearColor::Yellow, widgets[c].pos.X, widgets[c].pos.Y-20, hudFont, 1.f, false);
	}
}

void AMyHUD::MouseRightClicked()
{

	FVector2D mouse;
	APlayerController* PController = GetWorld()->GetFirstPlayerController();
	PController->GetMousePosition(mouse.X, mouse.Y);
	for (int c = 0; c < widgets.Num(); c++)
	{
		AAvatar* avatar = Cast<AAvatar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		if (widgets[c].hit(mouse) && widgets[c].bpSpell)
		{
			avatar->setSpell(widgets[c].bpSpell);
			addMessage(Message(NULL, FString(" Selected ") + widgets[c].icon.name.LeftChop(widgets[c].icon.name.Len() - widgets[c].icon.name.Find(" ")), 5.f, FColor::White));
		}

	}
}

void AMyHUD::addMessage(Message msg)
{
	messages.Add(msg);
}

void AMyHUD::addWidget(Widget wid)
{
	

	FVector2D start(200, 200), pad(12, 12);
	wid.size = FVector2D(100, 100);
	wid.pos = start;
	
	for (int c = 0; c < widgets.Num(); c++) 
	{
		wid.pos.X += wid.size.X + pad.X;
		if (wid.pos.X + wid.size.X > dims.X) 
		{
			wid.pos.X = start.X;
			wid.pos.Y += wid.size.Y + pad.Y;
		}
	}
	widgets.Add(wid);
}
