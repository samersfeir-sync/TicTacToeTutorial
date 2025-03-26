
#include "TicTacToeCellWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameModeBase.h"
#include "TicTacToeGameMode.h" 
#include "GameModeInterface.h"

void UTicTacToeCellWidget::NativeConstruct()
{
    Super::NativeConstruct();
    if (Button)
    {
        Button->OnClicked.AddDynamic(this, &UTicTacToeCellWidget::OnButtonClicked);
    }
}

void UTicTacToeCellWidget::SetCellSymbol(const FString& SymbolText)
{
    if (Symbol)
    {
        Symbol->SetText(FText::FromString(SymbolText));
    }
}

void UTicTacToeCellWidget::SetButtonMargin(int32 Row, int32 Col)
{
    if (Button)
    {
        FMargin Margin = FMargin(1.0f, 1.0f, 1.0f, 1.0f);

        // Set different margins based on the position in the grid
        if (Col == 0) // Left column
        {
            Margin.Left = 0.0f; // No margin on the left
        }
        else if (Col == 2) // Right column
        {
            Margin.Right = 0.0f; // No margin on the right
        }

        if (Row == 0) // Top row
        {
            Margin.Top = 0.0f; // No margin at the top
        }
        else if (Row == 2) // Bottom row
        {
            Margin.Bottom = 0.0f; // No margin at the bottom
        }

        // Create a new style object and modify it
        FButtonStyle NewStyle = Button->WidgetStyle;
        NewStyle.Normal.Margin = Margin;
        NewStyle.Hovered.Margin = Margin;
        NewStyle.Pressed.Margin = Margin;

        // Apply the new style to the button
        Button->SetStyle(NewStyle);
    }
}

void UTicTacToeCellWidget::UnbindOnButtonClicked()
{
    if (Button)
    {
        Button->OnClicked.RemoveAll(this);
    }
}

void UTicTacToeCellWidget::ChangeSymbolColor(FLinearColor NewColor)
{
    if (Symbol)
    {
      Symbol->SetColorAndOpacity(FSlateColor(NewColor));
    }
}

void UTicTacToeCellWidget::OnButtonClicked()
{
    if (GetWorld())
    {
        AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
        if (GameMode && GameMode->GetClass()->ImplementsInterface(UGameModeInterface::StaticClass()))
        {
            IGameModeInterface* GameModeInterface = Cast<IGameModeInterface>(GameMode);
            if (GameModeInterface)
            {
                GameModeInterface->GetGameModeReference()->PlayerMove(CellRow, CellColumn);
            }
        }
    }
}
