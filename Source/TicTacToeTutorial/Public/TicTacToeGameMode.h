// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeInterface.h"
#include "TicTacToeGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameWon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameLost);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameDrawn); 

class UTicTacToeWidget;
class UTicTacToeCellWidget;

UENUM(BlueprintType)
enum class EAI_Difficulty : uint8
{
    Easy     UMETA(DisplayName = "Easy"),
    Medium   UMETA(DisplayName = "Medium"),
    Hard     UMETA(DisplayName = "Hard")
};

UCLASS()
class TICTACTOETUTORIAL_API ATicTacToeGameMode : public AGameModeBase, public IGameModeInterface
{
	GENERATED_BODY()
	
public:

    ATicTacToeGameMode();

    //game Variables
    bool bIsPlayerTurn;
    int32 Grid[3][3];  //3x3 Tic-Tac-Toe grid
    EAI_Difficulty AIDifficulty = EAI_Difficulty::Easy;  //AI difficulty level

    //game Methods
    void PlayerMove(int32 Row, int32 Col);
    void AIMove();
    bool CheckWin(int32 Player);
    void RestartGame();
    void SwitchTurn();

    //AI Logic
    void EasyModeAI(int32& OutRow, int32& OutCol);
    bool MediumModeAI(int32& OutRow, int32& OutCol); 
    void HardModeAI(int32& OutRow, int32& OutCol);
    bool IsBoardFull() const;  //check if the board is full

    virtual ATicTacToeGameMode* GetGameModeReference() override { return this; }
    virtual bool SetAIDifficulty(int32 DifficultyLevel) override;
    virtual void StartGame() override;

    /**
* Recursively evaluates the best possible move using the Minimax algorithm.
*
* @param bIsMaximizingPlayer - If true, the function is evaluating the best move for the AI (maximizing player).
*                              If false, it is evaluating the best move for the player (minimizing player).
* @return An integer score:
*         - 1 if AI wins
*         - -1 if Player wins
*         - 0 if it's a draw
*/
    int32 EvaluateBestMove(bool bIsMaximizingPlayer);  //recursively evaluate the best move

protected:
    virtual void BeginPlay() override;

    // Reference to the TicTacToeWidget class via its path
    UPROPERTY()
    UClass* TicTacToeWidgetClass;

    // Instance of the TicTacToeWidget
    UPROPERTY()
    UTicTacToeWidget* TicTacToeWidgetInstance;

private:

    FOnGameWon OnGameWonDelegate;
    FOnGameLost OnGameLostDelegate;
    FOnGameDrawn OnGameDrawnDelegate;

    TArray<UTicTacToeCellWidget*> Winning_Losing_Cells;

    UFUNCTION()
    void OnGameWon();
    UFUNCTION()
    void OnGameLost();
    UFUNCTION()
    void OnGameDrawn();
};
