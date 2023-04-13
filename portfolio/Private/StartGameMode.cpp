// Fill out your copyright notice in the Description page of Project Settings.


#include "StartGameMode.h"
#include "TitleUserWidget.h"
#include "Kismet/GameplayStatics.h"

AStartGameMode::AStartGameMode()
{
	static ConstructorHelpers::FClassFinder<UTitleUserWidget> TitleWidgetClass(TEXT("WidgetBlueprint'/Game/AMyDirectory/UI/Title/UI_Title.UI_Title_C'"));

	if (TitleWidgetClass.Succeeded())
		TitleUserWidgetClass = TitleWidgetClass.Class;
}

void AStartGameMode::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor = true;
	if (IsValid(TitleUserWidgetClass))
	{
		// 위젯을 생성한다
		TitleUserWidget = Cast<UTitleUserWidget>(CreateWidget(GetWorld(), TitleUserWidgetClass));

		if (IsValid(TitleUserWidget))
		{
			// 위젯을 뷰포트에 추가한다
			TitleUserWidget->AddToViewport();
			TitleUserWidget->PlayFade();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TitleWidget Is Not Valid"));
	}
}
