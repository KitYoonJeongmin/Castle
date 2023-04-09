// Fill out your copyright notice in the Description page of Project Settings.


#include "BossDoor.h"
#include "MainCharacter.h"
#include "MainGameModeBase.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ABossDoor::ABossDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	RootComponent = Mesh;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_DOOR(TEXT("StaticMesh'/Game/MedievalDungeon/Meshes/Architecture/Crypt/SM_Mausoleum_Lid_B.SM_Mausoleum_Lid_B'"));
	if (SM_DOOR.Succeeded())
	{
		Mesh->SetStaticMesh(SM_DOOR.Object);
	}
	InteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("INTERACTWIDGET"));
	InteractWidget->SetupAttachment(RootComponent);
	InteractWidget->SetDrawAtDesiredSize(true);
	InteractWidget->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	InteractWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> INTERACT_HUD(TEXT("WidgetBlueprint'/Game/AMyDirectory/UI/UI_Interactiion.UI_Interactiion_C'"));
	if (INTERACT_HUD.Succeeded())
	{
		InteractWidget->SetWidgetClass(INTERACT_HUD.Class);
		InteractWidget->SetDrawSize(FVector2D(1266.f, 636.f));
	}
	

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->InitSphereRadius(200.f);
	Sphere->AttachTo(GetRootComponent());
	Sphere->SetCollisionProfileName(TEXT("OverlapAll"));
	
}

// Called when the game starts or when spawned
void ABossDoor::BeginPlay()
{
	Super::BeginPlay();
	InteractWidget->SetVisibility(false);
}

// Called every frame
void ABossDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossDoor::GoBossMap()
{
	Cast<AMainGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->SaveData();
	UGameplayStatics::OpenLevel(this, FName("Demonstration"));
}

void ABossDoor::Activate_Implementation()
{
	GoBossMap();
}

void ABossDoor::SetWidget_Implementation(bool IsVisible)
{
	if (InteractWidget == nullptr) return;

	InteractWidget->SetVisibility(IsVisible);
}

  