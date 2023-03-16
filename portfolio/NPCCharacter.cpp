// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCCharacter.h"
#include "DialogueUserWidget.h"
#include "Components/SphereComponent.h"
#include "Blueprint/UserWidget.h"
// Sets default values
ANPCCharacter::ANPCCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//메시 설정
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.0f), FRotator(0.f, -90.f, 0.f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CHARMBASE(TEXT("SkeletalMesh'/Game/AMyDirectory/NPC/Mesh/Dwarf_Idle.Dwarf_Idle'"));
	if (SK_CHARMBASE.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CHARMBASE.Object);
	}
	//애니메이션 설정
	static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM(TEXT("AnimBlueprint'/Game/AMyDirectory/NPC/Mesh/NPC_Anim.NPC_Anim_C'"));
	if (ANIM.Succeeded())// 애니메이션 설정
	{
		GetMesh()->SetAnimInstanceClass(ANIM.Class);
	}
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->InitSphereRadius(200.f);
	Sphere->AttachTo(GetRootComponent());
	Sphere->SetCollisionProfileName(TEXT("OverlapAll"));

	static ConstructorHelpers::FClassFinder<UUserWidget> DialogueWidgetAsset(TEXT("WidgetBlueprint'/Game/AMyDirectory/UI/UI_Dialogue.UI_Dialogue_C'"));
	if (DialogueWidgetAsset.Succeeded())
	{
		DialogueWidgetClass = DialogueWidgetAsset.Class;
		DialogueWidget = Cast<UDialogueUserWidget>(CreateWidget(GetWorld(), DialogueWidgetClass));
	}
	
	
}

// Called when the game starts or when spawned
void ANPCCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	DialogueArr.Add(TEXT("왔구나, 숲을 지나 서쪽으로 가면 성이 보일거야."));
	DialogueArr.Add(TEXT("성 입구는 막혀있을거야, \n입구 왼쪽에 성 입구를 오를 수 있게 나무 판자를 뒀으니 짚고 올라가도록해"));
	DialogueArr.Add(TEXT("성 중심부로 들어가 헤일러 장군을 찾은 뒤 그를 제거해"));
}

// Called every frame
void ANPCCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANPCCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

void ANPCCharacter::NextDiagolue()
{

	if (DialogueWidget == nullptr) return;
	if (CurrentDia == -1)
	{
		DialogueWidget->AddToViewport();
	}
	CurrentDia++;
	if (CurrentDia >= DialogueArr.Num())
	{
		CurrentDia = -1;
		DialogueWidget->RemoveFromViewport();
		return;
	}

	DialogueWidget->SetDiagloueText(DialogueArr[CurrentDia]);
}

