// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeWidget.h"
#include "SkillNodeWidget.h"

USkillTreeWidget::USkillTreeWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//연결 입력
	TreeConnection.Emplace(0, 2);
	TreeConnection.Emplace(1, 3);
	TreeConnection.Emplace(1, 4);
	TreeConnection.Emplace(2, 5);
	TreeConnection.Emplace(3, 5);
	TreeConnection.Emplace(4, 6);
}

void USkillTreeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	for (int i = 0; i < NodeNum; i++)
	{
		FString NodeName = FString::Printf(TEXT("SkillNode%d"), i);
		USkillNodeWidget* Node = Cast<USkillNodeWidget>(GetWidgetFromName(FName(*NodeName)));
		if (Node != nullptr)
		{
			SkillTree.Add(Node);
			NodeNumMap.Add(Node, i);
			Node->SetNodeState(1);
			Node->ParentWidget = this;
		}

	}

	for(auto tuple : TreeConnection)
	{
		SkillTree[tuple.Key]->NextSkillNodes.Add(tuple.Value);
	}
	SkillTree[0]->SetNodeState(0);
	SkillTree[1]->SetNodeState(0);
	
}

void USkillTreeWidget::UpdateTree()
{

	//Tree Load Update
	for (auto SkillNode : SkillTree)
	{
		SkillNode->SetNodeState(1);
	}
	for (auto SelectNode : TreeLoad)
	{
		SkillTree[SelectNode]->SetNodeState(2);
	}
	//Next Node UI Change
	for (auto j : SkillTree[TreeLoad[TreeLoad.Num()-1]]->NextSkillNodes)
	{
		SkillTree[j]->SetNodeState(0);
	}
}

void USkillTreeWidget::SelectNode(USkillNodeWidget* NodeWidget)
{
	int32 Num = (int32)NodeNumMap[NodeWidget];
	
	TreeLoad.AddUnique(Num);
}


void USkillTreeWidget::FadeAnimationPlay(bool IsIn)
{
	if (IsIn)
	{
		SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(Fade);
	}
	else
	{
		PlayAnimation(Fade, 0.f, 1, EUMGSequencePlayMode::Reverse);
	}
		
}
