// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TicTacToeWidget.generated.h"

class UTicTacToeCellWidget;
class UTextBlock;
class UGridPanel;
class UDifficultyButtonWidget;
class UUniformGridPanel;
class UButton;

UCLASS()
class TICTACTOETUTORIAL_API UTicTacToeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

    // Function to generate the grid dynamically
    void GenerateGrid();

    // Function to update the turn text
    void UpdateTurnText(const FString& TurnText);

    //function to update difficulty text
    void UpdateDifficultyText(const FString& Difficultytext);

    // Function to reset the game grid
    void ResetBoard();

    void UpdateGridTextColor(FLinearColor NewColor, UTicTacToeCellWidget* CellToUpdate);

    // Function to handle button clicks
    void OnCellClicked(int32 Row, int32 Col, FString CellSymbol);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UTicTacToeCellWidget> CellWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UDifficultyButtonWidget> DifficultyButtonWidgetClass;

    // Grid of buttons (9 buttons, 3x3)
    TArray<UTicTacToeCellWidget*> Cells;

    void StartGame();

protected:

    virtual void NativeConstruct() override;

    // UI Elements
    UPROPERTY(meta = (BindWidget))
    UTextBlock* TurnText;

    // The container for the Tic-Tac-Toe grid
    UPROPERTY(meta = (BindWidget))
    UGridPanel* GridPanel;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* DifficultyText;

    UPROPERTY(meta = (BindWidget))
    UUniformGridPanel* DifficultyButtonsPanel;

    UPROPERTY(meta = (BindWidget))
    UButton* RestartButton;

private:

    TArray<FString> DifficultyArray = { TEXT("Easy"), TEXT("Medium"), TEXT("Hard") };

    UFUNCTION()
    void OnRestartButtonClicked();
};
