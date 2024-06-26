// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreSetup/CustomHUD.h"

#include "Blueprint/UserWidget.h"
#include "Widgets/EndScreen.h"
#include "Widgets/HUDWidget.h"

#pragma region CreatingWidgets

void ACustomHUD::CreateAndShowHUDWidget()
{
	if(HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UHUDWidget>(GetWorld(), HUDWidgetClass);
		if(HUDWidget)
		{
			HUDWidget->AddToViewport();
			HUDWidget->ShowInteractCrosshair(false);
		}
	}
}

void ACustomHUD::CreateAndCollapseEndScreen()
{
	if(EndScreenClass)
	{
		EndScreen = CreateWidget<UEndScreen>(GetWorld(), EndScreenClass);
		if(EndScreen)
		{
			EndScreen->AddToViewport();
			EndScreen->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void ACustomHUD::CreateAndCollapsePauseMenu()
{
	if(PauseMenuClass)
	{
		PauseMenu = CreateWidget<UUserWidget>(GetWorld(), PauseMenuClass);
		if(PauseMenu)
		{
			PauseMenu->AddToViewport();
			PauseMenu->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

#pragma endregion

void ACustomHUD::SetInputModeGameOnly() const
{
	FInputModeGameOnly InputMode;
	GetOwningPlayerController()->SetInputMode(InputMode);
	GetOwningPlayerController()->bShowMouseCursor = false;
}

void ACustomHUD::SetInputModeGameAndUI(const bool ShowMouseCursor) const
{
	FInputModeGameAndUI InputMode;
	GetOwningPlayerController()->SetInputMode(InputMode);
	
	GetOwningPlayerController()->bShowMouseCursor = ShowMouseCursor;
	if(ShowMouseCursor) CenterMouseCursor();
}

void ACustomHUD::SetInputModeUIOnly() const
{
	FInputModeUIOnly InputMode;
	APlayerController* PlayerController = GetOwningPlayerController();
	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = true;

	CenterMouseCursor();
}

void ACustomHUD::CenterMouseCursor() const
{
	APlayerController* PlayerController = GetOwningPlayerController();
	int32 ScreenWidth, ScreenHeight;
	PlayerController->GetViewportSize(ScreenWidth, ScreenHeight);
	FVector2D ScreenCenter(ScreenWidth * 0.5f, ScreenHeight * 0.5f);
	PlayerController->SetMouseLocation(ScreenCenter.X, ScreenCenter.Y);
}

void ACustomHUD::ShowEndScreen(const bool Success, const FText& Text) const
{
	if(!EndScreen || EndScreen->GetVisibility() == ESlateVisibility::Visible) return;
	
	SetInputModeUIOnly();
	
	EndScreen->SetText(Text);
	EndScreen->SetBackgroundColor(Success);

	if(Success) GetOwningPlayerController()->SetPause(true);
	
	if(HUDWidget && HUDWidget->IsVisible()) HUDWidget->SetVisibility(ESlateVisibility::Collapsed);
	
	EndScreen->SetVisibility(ESlateVisibility::Visible);
}

void ACustomHUD::TogglePauseMenu()
{
	if(!PauseMenu) return;

	if(bPauseMenuOpen)
	{
		SetInputModeGameOnly();
		
		PauseMenu->SetVisibility(ESlateVisibility::Collapsed);
		GetOwningPlayerController()->SetPause(false);
	}
	else
	{
		SetInputModeGameAndUI(true);
		
		PauseMenu->SetVisibility(ESlateVisibility::Visible);
		GetOwningPlayerController()->SetPause(true);
	}
	bPauseMenuOpen = !bPauseMenuOpen;
}

void ACustomHUD::BeginPlay()
{
	Super::BeginPlay();

	SetInputModeGameOnly();

	CreateAndShowHUDWidget();
	CreateAndCollapseEndScreen();
	CreateAndCollapsePauseMenu();
}
