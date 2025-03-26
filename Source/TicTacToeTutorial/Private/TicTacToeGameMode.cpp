// Fill out your copyright notice in the Description page of Project Settings.


#include "TicTacToeGameMode.h"
#include "TicTacToeWidget.h"
#include "TicTacToeCellWidget.h"

ATicTacToeGameMode::ATicTacToeGameMode()
{
	bIsPlayerTurn = true; //player's turn by default

    //load the TicTacToeWidget class via the reference path
    static ConstructorHelpers::FClassFinder<UTicTacToeWidget> WidgetClass(TEXT("/Game/UI/TicTacToeWidget_BP"));

    if (WidgetClass.Succeeded())
    {
        TicTacToeWidgetClass = WidgetClass.Class;
    }
}

void ATicTacToeGameMode::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PC = GetWorld()->GetFirstPlayerController(); //equivalent to get player controller node in blueprints
    if (PC)
    {
        PC->bShowMouseCursor = true; //always show the mouse cursor
        PC->SetInputMode(FInputModeUIOnly());
    }

    // Create and display the Tic-Tac-Toe widget
    if (TicTacToeWidgetClass)
    {
        TicTacToeWidgetInstance = CreateWidget<UTicTacToeWidget>(GetWorld(), TicTacToeWidgetClass);
        if (TicTacToeWidgetInstance)
        {
            TicTacToeWidgetInstance->AddToViewport();
        }
    }
    OnGameWonDelegate.AddDynamic(this, &ATicTacToeGameMode::OnGameWon);
    OnGameLostDelegate.AddDynamic(this, &ATicTacToeGameMode::OnGameLost);
    OnGameDrawnDelegate.AddDynamic(this, &ATicTacToeGameMode::OnGameDrawn);
}

void ATicTacToeGameMode::OnGameWon()
{
    //loop through each cell in the Cells array
    for (UTicTacToeCellWidget* Cell : TicTacToeWidgetInstance->Cells)
    {
        if (Cell)
        {
            Cell->UnbindOnButtonClicked();
        }
    }

    for (UTicTacToeCellWidget* Cell : Winning_Losing_Cells)
    {
        TicTacToeWidgetInstance->UpdateGridTextColor(FLinearColor::Green, Cell);
    }
    TicTacToeWidgetInstance->UpdateTurnText("You Won!");
}

void ATicTacToeGameMode::OnGameLost()
{
    //loop through each cell in the Cells array
    for (UTicTacToeCellWidget* Cell : TicTacToeWidgetInstance->Cells)
    {
        if (Cell)
        {
            Cell->UnbindOnButtonClicked();
        }
    }

    for (UTicTacToeCellWidget* Cell : Winning_Losing_Cells)
    {
        TicTacToeWidgetInstance->UpdateGridTextColor(FLinearColor::Red, Cell);
    }

    TicTacToeWidgetInstance->UpdateTurnText("You Lost!");
}

void ATicTacToeGameMode::OnGameDrawn()
{
    TicTacToeWidgetInstance->UpdateTurnText("Draw!");
}

void ATicTacToeGameMode::StartGame()
{
    Winning_Losing_Cells.Empty();
    FMemory::Memset(Grid, 0, sizeof(Grid));
    bIsPlayerTurn = true;
    TicTacToeWidgetInstance->UpdateTurnText("Your Turn!");
    FString DifficultyString = UEnum::GetDisplayValueAsText(AIDifficulty).ToString();
    TicTacToeWidgetInstance->UpdateDifficultyText(DifficultyString);
    TicTacToeWidgetInstance->StartGame();
}

void ATicTacToeGameMode::PlayerMove(int32 Row, int32 Col)
{
    if (bIsPlayerTurn)
    {
        //player makes a move on the grid
        if (Grid[Row][Col] == 0) //if the cell is empty
        {
            Grid[Row][Col] = 1; //player is represented by 1

            TicTacToeWidgetInstance->OnCellClicked(Row, Col, "X");

            if (!CheckWin(1)) //check if player won or draw
            {
                if (!IsBoardFull())
                {
                    SwitchTurn();
                }
            }
            else
            {
                OnGameWonDelegate.Broadcast();
            }
        }
    }
}

void ATicTacToeGameMode::AIMove()
{
    int32 Row = -1, Col = -1;

    switch (AIDifficulty)
    {
    case EAI_Difficulty::Easy:
        //easy: Random move

        EasyModeAI(Row, Col);
        break;

    case EAI_Difficulty::Medium:
        //medium: Block player's winning move or random
        if (!MediumModeAI(Row, Col))
        {
            EasyModeAI(Row, Col);
        }
        break;

    case EAI_Difficulty::Hard:
        //hard: Use Minimax algorithm
        HardModeAI(Row, Col);
        break;

    default:
        break;
    }

    //place AI move
    Grid[Row][Col] = -1;  //AI is represented by -1

    TicTacToeWidgetInstance->OnCellClicked(Row, Col, "O");

    if (!CheckWin(-1)) //check if AI didnt win
    {
        if (!IsBoardFull()) //check if not draw
        {
            SwitchTurn();
        }
    }
    else
    {
        OnGameLostDelegate.Broadcast();
    }
}

bool ATicTacToeGameMode::CheckWin(int32 Player)
{
    // Check rows for a win
    for (int32 i = 0; i < 3; i++)
    {
        if (Grid[i][0] == Player && Grid[i][1] == Player && Grid[i][2] == Player)
        {
            Winning_Losing_Cells.Empty();
            Winning_Losing_Cells.Add(TicTacToeWidgetInstance->Cells[i * 3]);      // First cell in the row
            Winning_Losing_Cells.Add(TicTacToeWidgetInstance->Cells[i * 3 + 1]);  // Second cell in the row
            Winning_Losing_Cells.Add(TicTacToeWidgetInstance->Cells[i * 3 + 2]);  // Third cell in the row
            return true; // Row win
        }
    }

    // Check columns for a win
    for (int32 i = 0; i < 3; i++)
    {
        if (Grid[0][i] == Player && Grid[1][i] == Player && Grid[2][i] == Player)
        {
            Winning_Losing_Cells.Empty();
            Winning_Losing_Cells.Add(TicTacToeWidgetInstance->Cells[i]);         // First cell in the column
            Winning_Losing_Cells.Add(TicTacToeWidgetInstance->Cells[i + 3]);     // Second cell in the column
            Winning_Losing_Cells.Add(TicTacToeWidgetInstance->Cells[i + 6]);     // Third cell in the column
            return true; // Column win
        }
    }

    // Check diagonals for a win
    if (Grid[0][0] == Player && Grid[1][1] == Player && Grid[2][2] == Player)
    {
        Winning_Losing_Cells.Empty();
        Winning_Losing_Cells.Add(TicTacToeWidgetInstance->Cells[0]);  // First cell in the diagonal
        Winning_Losing_Cells.Add(TicTacToeWidgetInstance->Cells[4]);  // Second cell in the diagonal
        Winning_Losing_Cells.Add(TicTacToeWidgetInstance->Cells[8]);  // Third cell in the diagonal
        return true; // Diagonal win
    }

    if (Grid[0][2] == Player && Grid[1][1] == Player && Grid[2][0] == Player)
    {
        Winning_Losing_Cells.Empty();
        Winning_Losing_Cells.Add(TicTacToeWidgetInstance->Cells[2]);  // First cell in the diagonal
        Winning_Losing_Cells.Add(TicTacToeWidgetInstance->Cells[4]);  // Second cell in the diagonal
        Winning_Losing_Cells.Add(TicTacToeWidgetInstance->Cells[6]);  // Third cell in the diagonal
        return true; // Diagonal win
    }

    return false; // No win
}

void ATicTacToeGameMode::RestartGame()
{
    StartGame();
}

void ATicTacToeGameMode::SwitchTurn()
{
    //switch turns
    bIsPlayerTurn = !bIsPlayerTurn;

    if (!bIsPlayerTurn)
    {
        TicTacToeWidgetInstance->UpdateTurnText("AI Turn!");

        FTimerHandle AIMoveTimer;
        GetWorld()->GetTimerManager().SetTimer(AIMoveTimer, this, &ATicTacToeGameMode::AIMove, 1.0f, false);

    }
    else TicTacToeWidgetInstance->UpdateTurnText("Your Turn!");
}

int32 ATicTacToeGameMode::EvaluateBestMove(bool bIsMaximizingPlayer)
{
    /*
    Step-by-Step Explanation
Base Case (Checking for Terminal States)

If Player (1) wins, return -1 (bad for AI).
If AI (-1) wins, return 1 (good for AI).
If the board is full, return 0 (draw).
Recursive Minimax Evaluation

If it’s the AI’s turn (bIsMaximizingPlayer == true):
It tries to maximize its score by picking the move that gives the highest value.
If it’s the Player’s turn (bIsMaximizingPlayer == false):
The player tries to minimize AI’s score by choosing the move that gives the lowest value.
Iterating Over All Possible Moves

The function simulates every possible move.
It temporarily places a move (-1 for AI, 1 for Player).
It recursively calls itself to evaluate the board.
It undoes the move (backtracking).
It stores the best possible score for that move.
*/
    // Base case: Check if the game has ended
    /* Check for Player's Win: If the player (represented by 1) has won, the function returns -1.
    * This is because we are using Minimax, and the player wants to minimize the AI’s score. So, if the player wins, it’s a bad result for the AI.
    */
    if (CheckWin(1))   // Player wins
        return -1;     

    /*Check for AI's Win: If the AI (represented by -1) has won, the function returns 1. 
    * This is good for the AI because the AI wants to maximize its score.
    */
    if (CheckWin(-1))  // AI wins
        return 1;      // AI wants to maximize its score

    /*Check for Draw: If the board is full (no more moves can be made), it’s a draw. 
    * The function returns 0, which means the result is neutral (neither good nor bad).
    */
    if (IsBoardFull()) // Draw
        return 0;


    //Now, if the game isn't over yet, the function moves into the "recursive" part, where it evaluates potential moves:
    // 
    // If it's AI's turn (Maximizing Player) The AI wants to maximize its score (i.e., make the best possible move):
    if (bIsMaximizingPlayer)
    {
        int32 BestScore = -1000; //Starts with a very low value (BestScore = -1000) for comparison.

        //It then loops through all the empty cells on the board.
        //For each empty cell, the AI places its marker (-1) and calls EvaluateBestMove(false) to evaluate the board assuming it’s the player's turn next.
        for (int32 Row = 0; Row < 3; Row++)
        {
            for (int32 Col = 0; Col < 3; Col++)
            {
                if (Grid[Row][Col] == 0)  // Check if the cell is empty
                {
                    Grid[Row][Col] = -1;  // AI places its move (-1)

                    //If the player tries to minimize the AI’s score, it checks what the worst move for the AI will be (hence, we call EvaluateBestMove(false)).
                    int32 Score = EvaluateBestMove(false);

                    Grid[Row][Col] = 0;  //After each evaluation, it "undoes" the move (backtracking) by setting the cell back to empty.

                    //The function keeps track of the best score it can get, and finally returns that score after checking all possible moves.
                    BestScore = FMath::Max(BestScore, Score); // Store the max value
                }
            }
        }
        return BestScore; // Return the best score the AI can achieve
    }
    // If it's Player's turn (Minimizing Player) the player tries to minimize the AI's score by making the worst possible move for the AI:
    else
    {
        int32 BestScore = 1000; //Starts with a very high value (BestScore = 1000) for comparison.

        //It then loops through all the empty cells on the board.
        //For each empty cell, the player places their marker (1) and calls EvaluateBestMove(true) to evaluate the board assuming it’s the AI’s turn next.
        for (int32 Row = 0; Row < 3; Row++)
        {
            for (int32 Col = 0; Col < 3; Col++)
            {
                if (Grid[Row][Col] == 0)  // Check if the cell is empty
                {
                    Grid[Row][Col] = 1;  // Player places its move (1)

                    //If it’s the AI’s turn, it checks what the best move for the AI will be (hence, we call EvaluateBestMove(true)).   
                    int32 Score = EvaluateBestMove(true);

                    Grid[Row][Col] = 0;  // Undo move (backtracking)
                    
                    //The function keeps track of the worst possible score for the AI, and finally returns that score after checking all possible moves.
                    BestScore = FMath::Min(BestScore, Score); // Store the min value
                }
            }
        }
        return BestScore; // Return the best score the player can force AI into
    }
}

void ATicTacToeGameMode::EasyModeAI(int32& OutRow, int32& OutCol)
{

    /*Step-by-Step Logic
Randomly pick a row and column

FMath::RandRange(0, 2) generates a random number between 0 and 2, representing row and column indices.
Check if the selected cell is empty (Grid[OutRow][OutCol] == 0)

If the selected position is already occupied (by 1 for the player or -1 for the AI), the loop repeats and picks another random position.
Repeat until an empty cell is found

The do-while loop ensures that the AI keeps picking random positions until it finds an empty spot.
Once a valid position is found, the function returns

The selected OutRow and OutCol will be used to place the AI's move
*/
    do
    {
        // Randomly pick a row (0 to 2)
        OutRow = FMath::RandRange(0, 2);

        // Randomly pick a column (0 to 2)
        OutCol = FMath::RandRange(0, 2);

    } while (Grid[OutRow][OutCol] != 0);  // Repeat if the chosen cell is not empty
}

bool ATicTacToeGameMode::MediumModeAI(int32& OutRow, int32& OutCol)
{
    /*
Step - by - Step Logic
Loop through all cells in the grid

The AI checks every row(Row) and column(Col) in the 3x3 grid.
Find an empty cell(Grid[Row][Col] == 0)

If the cell is empty, it means the player could potentially place their next move here.
Simulate the player's move (X = 1)

Temporarily place 1 in the empty cell to check if it would result in a win for the player.
Check if this move would make the player win(CheckWin(1))

If CheckWin(1) returns true, it means that if the player were to place X here, they would win the game.
Block the winning move

Store the cell's position in OutRow and OutCol.
Place - 1 (AI's marker) in that position to block the player from winning.
    Return true to indicate a block was made

    The AI successfully blocked the player’s winning move, so the function ends here.
    If no block is found, return false

    The AI does nothing and will later make a random move(handled elsewhere).
*/

    // Try to block the player from winning by simulating their next move
    for (int32 Row = 0; Row < 3; Row++) // Loop through each row of the grid
    {
        for (int32 Col = 0; Col < 3; Col++) // Loop through each column of the grid
        {
            if (Grid[Row][Col] == 0)  // Check if the current cell is empty
            {
                // Temporarily place the player's marker ('X' is represented by 1)
                Grid[Row][Col] = 1;

                // Check if this move results in a win for the player
                if (CheckWin(1))
                {
                    // If the player would win, store this cell's position
                    OutRow = Row;
                    OutCol = Col;

                    // Replace the player's temporary marker with AI's marker ('O' is represented by -1)
                    Grid[Row][Col] = -1;

                    return true; // Return true to indicate that AI blocked the player
                }

                // Reset the cell back to empty since the move was only a simulation
                Grid[Row][Col] = 0;
            }
        }
    }
    return false;  // No blocking move was necessary, return false
}

void ATicTacToeGameMode::HardModeAI(int32& OutRow, int32& OutCol)
{
    //We start by initializing variables to track the best move.
    int32 BestScore = -1000;  //BestScore is set to a very low value (-1000) to ensure that the AI will always find a better move.
    //BestRow and BestCol are set to -1 as placeholders (invalid values) until a better move is found.
    int32 BestRow = -1;
    int32 BestCol = -1;

    //loop through all cells on the Tic-Tac-Toe board.
    for (int32 Row = 0; Row < 3; Row++)
    {
        for (int32 Col = 0; Col < 3; Col++)
        {
            if (Grid[Row][Col] == 0)  //if the cell is empty
            {
                Grid[Row][Col] = -1;   //If a cell is empty(0), the AI temporarily places its move(-1) in that spot.

                //calls EvaluateBestMove(false), which:
                //Uses the Minimax algorithm to calculate how good or bad this move is.
                //The parameter false means it's now the player’s turn to move (AI is maximizing, player is minimizing).
                int32 Score = EvaluateBestMove(false);  //get the score for this move

                Grid[Row][Col] = 0;  //After evaluating the move, we undo it to try other moves.

                if (Score > BestScore)  //maximize AI's score
                {
                    //If the current move results in a better score than the previous best move, we update BestScore, BestRow, and BestCol.
                    //This ensures that the AI chooses the move that gives it the highest possible score.
                    BestScore = Score;
                    BestRow = Row;
                    BestCol = Col;
                }
            }
        }
    }
    //Once the loop is done, we set the best move coordinates (OutRow, OutCol) so that the AI can play its best move.
    //Since Minimax always picks the optimal move, the AI will never lose unless the game was already lost.
    OutRow = BestRow;
    OutCol = BestCol;
}

bool ATicTacToeGameMode::IsBoardFull() const
{
    for (int32 Row = 0; Row < 3; Row++)
    {
        for (int32 Col = 0; Col < 3; Col++)
        {
            if (Grid[Row][Col] == 0)  //empty cell found
                return false;
        }
    }
    OnGameDrawnDelegate.Broadcast();
    return true;  //no empty cells
}

bool ATicTacToeGameMode::SetAIDifficulty(int32 DifficultyLevel)
{
    if (DifficultyLevel < 0 || DifficultyLevel>2) return false;

    switch (DifficultyLevel)
    {
    case 0:
        AIDifficulty = EAI_Difficulty::Easy;
        break;

    case 1:
        AIDifficulty = EAI_Difficulty::Medium;
        break;
    case 2:
        AIDifficulty = EAI_Difficulty::Hard;
        break;

    default:
        return false;
    }
    return true;
}
