// Fill out your copyright notice in the Description page of Project Settings.


#include "DifficultyButtonWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameModeBase.h"
#include "TicTacToeGameMode.h" 
#include "GameModeInterface.h"

void UDifficultyButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

    if (GetWorld())
    {
        AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
        if (GameMode && GameMode->GetClass()->ImplementsInterface(UGameModeInterface::StaticClass()))
        {
            GameModeInterface = Cast<IGameModeInterface>(GameMode);
        }
    }

    if (DifficultyButton)
    {
        DifficultyButton->OnClicked.AddDynamic(this, &UDifficultyButtonWidget::OnButtonClicked);
    }
}

void UDifficultyButtonWidget::OnButtonClicked()
{
    SetDifficulty();
    GameModeInterface->StartGame();
}

void UDifficultyButtonWidget::SetDifficulty()
{
    if (GameModeInterface)
    {
        GameModeInterface->SetAIDifficulty(DifficultyLevel);
    }
}

void UDifficultyButtonWidget::SetDifficultyLevel(int32 Level)
{
    DifficultyLevel = Level;
}

void UDifficultyButtonWidget::SetDifficultyText(FString NewDifficultyText)
{
    if (DifficultyText)
    {
        DifficultyText->SetText(FText::FromString(NewDifficultyText));

    }
}
