// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DifficultyButtonWidget.generated.h"

class UTextBlock;
class UButton;
class IGameModeInterface;

UCLASS()
class TICTACTOETUTORIAL_API UDifficultyButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:

	UPROPERTY(meta=(BindWidget))
	UTextBlock* DifficultyText;

	UPROPERTY(meta = (BindWidget))
	UButton* DifficultyButton;

	UFUNCTION()
	void OnButtonClicked();

	IGameModeInterface* GameModeInterface;

	int32 DifficultyLevel;

protected:

	virtual void NativeConstruct() override;

public:
	
	void SetDifficulty();
	void SetDifficultyLevel(int32 Level);
	void SetDifficultyText(FString NewDifficultyText);
};
