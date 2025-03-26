#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TicTacToeCellWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class TICTACTOETUTORIAL_API UTicTacToeCellWidget : public UUserWidget
{
    GENERATED_BODY()

public:

    // Function to set the cell symbol (X or O)
    void SetCellSymbol(const FString& Symbol);

    void SetButtonMargin(int32 Row, int32 Col);

    void SetCellRow(int32 OutRow) { CellRow = OutRow; }
    void SetCellColumn(int32 OutCol) { CellColumn = OutCol; }
    void UnbindOnButtonClicked();
    void ChangeSymbolColor(FLinearColor NewColor);

protected:

    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Symbol;  // Display X or O

    // UI Elements
    UPROPERTY(meta = (BindWidget))
    UButton* Button;

private:

    int32 CellRow;
    int32 CellColumn;

    UFUNCTION()
    void OnButtonClicked();
};
