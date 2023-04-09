// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopNPC.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "MainCharacter.h"
#include "ShopWidget.h"
// Sets default values
AShopNPC::AShopNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//메시 설정
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.0f), FRotator(0.f, -90.f, 0.f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CHARMBASE(TEXT("SkeletalMesh'/Game/AMyDirectory/NPC/ShopNPC/Old_Man_Idle.Old_Man_Idle'"));
	if (SK_CHARMBASE.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CHARMBASE.Object);
	}
	//애니메이션 설정
	static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM(TEXT("AnimBlueprint'/Game/AMyDirectory/NPC/ShopNPC/AnimBP_ShopNPC.AnimBP_ShopNPC_C'"));
	if (ANIM.Succeeded())// 애니메이션 설정
	{
		GetMesh()->SetAnimInstanceClass(ANIM.Class);
	}
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->InitSphereRadius(200.f);
	Sphere->AttachTo(GetRootComponent());
	Sphere->SetCollisionProfileName(TEXT("OverlapAll"));
	static ConstructorHelpers::FClassFinder<UUserWidget> ShopWidgetAsset(TEXT("WidgetBlueprint'/Game/AMyDirectory/UI/Shop/UI_Shop.UI_Shop_C'"));
	if (ShopWidgetAsset.Succeeded())
	{
		ShopWidgetClass = ShopWidgetAsset.Class;
		ShopWidget = Cast<UShopWidget>(CreateWidget(GetWorld(), ShopWidgetAsset.Class));
	}
	//Widget
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
}

// Called when the game starts or when spawned
void AShopNPC::BeginPlay()
{
	Super::BeginPlay();
	
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AShopNPC::OnOverlapEnd);
	InteractWidget->SetVisibility(false);
}

// Called every frame
void AShopNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShopNPC::Activate_Implementation()
{
	ShowShopWidget();
}

void AShopNPC::SetWidget_Implementation(bool IsVisible)
{
	if (InteractWidget == nullptr) return;

	InteractWidget->SetVisibility(IsVisible);
}

void AShopNPC::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<AMainCharacter>(OtherActor))
	{
		ShopWidget->Exit();
	}
}

void AShopNPC::ShowShopWidget()
{
	if (ShopWidget == nullptr)
	{
		ShopWidget = Cast<UShopWidget>(CreateWidget(GetWorld(), ShopWidgetClass));
		
	}
	ShopWidget->AddToViewport();
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->bShowMouseCursor = true;
}


