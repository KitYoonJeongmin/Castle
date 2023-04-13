// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemGen.h"
#include "MainCharacter.h"
#include "InventoryComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
// Sets default values
AItemGen::AItemGen()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->InitSphereRadius(200.f);
	Sphere->AttachTo(GetRootComponent());
	Sphere->SetCollisionProfileName(TEXT("Interact"));
	RootComponent = Sphere;

	//Widget
	InteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("INTERACTWIDGET"));
	InteractWidget->SetupAttachment(RootComponent);
	InteractWidget->SetDrawAtDesiredSize(true);
	InteractWidget->SetRelativeLocation(FVector(0.f, 100.f, 0.f));
	InteractWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> INTERACT_HUD(TEXT("WidgetBlueprint'/Game/AMyDirectory/UI/UI_Interactiion.UI_Interactiion_C'"));
	if (INTERACT_HUD.Succeeded())
	{
		InteractWidget->SetWidgetClass(INTERACT_HUD.Class);
		InteractWidget->SetDrawSize(FVector2D(1266.f, 636.f));
	}
	
	
}

// Called when the game starts or when spawned
void AItemGen::BeginPlay()
{
	Super::BeginPlay();
	InteractWidget->SetVisibility(false);
}

void AItemGen::Activate_Implementation()
{
	GenerateItem();
}

void AItemGen::SetWidget_Implementation(bool IsVisible)
{
	if (InteractWidget == nullptr) return;

	InteractWidget->SetVisibility(IsVisible);
}

// Called every frame
void AItemGen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemGen::GenerateItem()
{
	AMainCharacter* Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	int32 item = FMath::RandRange(0, 1);
	if (item == 0)
	{
		if(Player->Inventory != nullptr)
			Player->Inventory->AddPotion(FMath::RandRange(1, 2));
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Get Potion"));
	}
	else if (item == 1)
	{
		if(Player->Inventory != nullptr)
			Player->Inventory->AddGold(FMath::RandRange(1, 100));
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Get Gold"));
	}
	this->Destroy();
}
 
