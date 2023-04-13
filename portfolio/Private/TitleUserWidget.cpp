// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleUserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

UTitleUserWidget::UTitleUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}
void UTitleUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StartBtn = Cast<UButton>(GetWidgetFromName(TEXT("StartButton")));
	EndBtn = Cast<UButton>(GetWidgetFromName(TEXT("EndButton")));

	if(::IsValid(StartBtn))
		StartBtn->OnClicked.AddDynamic(this, &UTitleUserWidget::StartGame);
	if (::IsValid(EndBtn))
		EndBtn->OnClicked.AddDynamic(this, &UTitleUserWidget::ExitGame);

	
}



void UTitleUserWidget::StartGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("LandScape"));
}

void UTitleUserWidget::ExitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void UTitleUserWidget::PlayFade()
{
	PlayAnimation(Fade);
}
