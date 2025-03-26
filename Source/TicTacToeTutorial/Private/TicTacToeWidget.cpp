// Fill out your copyright notice in the Description page of Project Settings.


#include "TicTacToeWidget.h"
#include "TicTacToeCellWidget.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "Components/Widget.h"
#include "Components/GridPanel.h"
#include "Components/Button.h"
#include "DifficultyButtonWidget.h"
#include "Components/UniformGridPanel.h"

void UTicTacToeWidget::StartGame()
{
    GenerateGrid();
    TurnText->SetVisibility(ESlateVisibility::HitTestInvisible);
    DifficultyText->SetVisibility(ESlateVisibility::HitTestInvisible);
    GridPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    DifficultyButtonsPanel->SetVisibility(ESlateVisibility::Collapsed);
    RestartButton->SetVisibility(ESlateVisibility::Visible);
}

void UTicTacToeWidget::NativeConstruct()
{
    Super::NativeConstruct();

    for (int32 i = 0; i < 3; i++)
    {
        UDifficultyButtonWidget* NewDiffButton = CreateWidget<UDifficultyButtonWidget>(GetWorld(), DifficultyButtonWidgetClass);
        NewDiffButton->SetDifficultyLevel(i);
        NewDiffButton->SetDifficultyText(DifficultyArray[i]);

        if (DifficultyButtonsPanel)
        {
            DifficultyButtonsPanel->AddChildToUniformGrid(NewDiffButton,0,i);       
        }
    }

    if (RestartButton)
    {
        RestartButton->OnClicked.AddDynamic(this, &UTicTacToeWidget::OnRestartButtonClicked);
    }
}

void UTicTacToeWidget::OnRestartButtonClicked()
{
    RestartButton->SetVisibility(ESlateVisibility::Collapsed);
    DifficultyText->SetVisibility(ESlateVisibility::Collapsed);
    GridPanel->SetVisibility(ESlateVisibility::Collapsed);
    TurnText->SetVisibility(ESlateVisibility::Collapsed);
    DifficultyButtonsPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    ResetBoard();
}

void UTicTacToeWidget::GenerateGrid()
{
    // Create the 9 cells (buttons)
    for (int32 Row = 0; Row < 3; Row++)
    {
        for (int32 Col = 0; Col < 3; Col++)
        {
            // Create a new TicTacToeCell widget
            UTicTacToeCellWidget* NewCell = CreateWidget<UTicTacToeCellWidget>(GetWorld(), CellWidgetClass);

            // Set the symbol for this cell (initially empty)
            NewCell->SetCellSymbol("");
            NewCell->SetCellColumn(Col);
            NewCell->SetCellRow(Row);
            NewCell->SetButtonMargin(Row, Col);

            // Add the cell to the grid
            if (GridPanel)
            {
                GridPanel->AddChildToGrid(NewCell, Row, Col);
            }

            // Store the created cell in the array for later reference
            Cells.Add(NewCell);
        }
    }
}

void UTicTacToeWidget::OnCellClicked(int32 Row, int32 Col, FString CellSymbol)
{
    // Update the cell with the correct symbol (X or O)
    int32 CellIndex = Row * 3 + Col;

    if (Cells.IsValidIndex(CellIndex))
    {
        Cells[CellIndex]->SetCellSymbol(CellSymbol);
    }
}

void UTicTacToeWidget::UpdateTurnText(const FString& TurnTextValue)
{
    if (TurnText)
    {
        TurnText->SetText(FText::FromString(TurnTextValue));
    }
}

void UTicTacToeWidget::UpdateDifficultyText(const FString& DifficultyTextValue)
{
    if (DifficultyText)
    {
        DifficultyText->SetText(FText::FromString(DifficultyTextValue));
    }
}

void UTicTacToeWidget::UpdateGridTextColor(FLinearColor NewColor, UTicTacToeCellWidget* CellToUpdate)
 {
    if (GridPanel)
    {
        if (CellToUpdate)
        {
            CellToUpdate->ChangeSymbolColor(NewColor);
        }
    }
}

void UTicTacToeWidget::ResetBoard()
{
    // Reset all the cells to be empty
    for (UTicTacToeCellWidget* Cell : Cells)
    {
        if (Cell)
        {
            Cell->SetCellSymbol("");
            UpdateGridTextColor(FLinearColor::White, Cell);
        }
    }

    UpdateTurnText(TEXT("Your Turn!"));
}