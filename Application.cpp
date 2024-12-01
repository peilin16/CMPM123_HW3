#include "Application.h"
#include "imgui/imgui.h"
#include "classes/Chess.h"

namespace ClassGame {
        //
        // our global variables
        //
        Chess *game = nullptr;
        bool gameOver = false;
        int gameWinner = -1;

        //
        // game starting point
        // this is called by the main render loop in main.cpp
        //
        void GameStartUp() 
        {
            game = new Chess();
            game->setUpBoard();
            game->_gameOptions.currentTurnNo = 1;
        }

        //
        // game render loop
        // this is called by the main render loop in main.cpp
        //
        void RenderGame() 
        {
                ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

                //ImGui::ShowDemoWindow();

                //ImGui::Begin("Settings");
                //ImGui::Text("Current Player Number: %d", game->getCurrentPlayer()->playerNumber());
                //ImGui::Text("Current Board State: %s", game->stateString().c_str());

                if (gameOver) {
                    ImGui::Text("Game Over!");
                    ImGui::Text("Winner: %d", gameWinner);
                    if (ImGui::Button("Reset Game")) {
                        //game->stopGame();
                        resetGame();
                        game->_gameOptions.currentTurnNo = 1;
                        game->_gameOptions.gameNumber = 0;
                    }
                    /*if (ImGui::Button("Reset")) {
                        //game->stopGame();
                        resetGame();
                        game->_gameOptions.gameNumber = 1;
                    }*/
                }
                //ImGui::End();

                ImGui::Begin("GameWindow");

                ImGui::SetNextWindowSize(ImVec2(200,200),0);
                game->drawFrame();
                game->drawCpatureGrid();
                ImGui::SetCursorPos(ImVec2 (85, 540));
                ImGui::Button("Player to Player");
                if(ImGui::IsItemClicked()){
                    resetGame();
                    game->_gameOptions.gameNumber = 1;
                    game->_gameOptions.currentTurnNo = 0;
                    game->_gameOptions.AIPlaying = false;
                   // game->startGame();
                }
                ImGui::SetCursorPos(ImVec2 (225, 540));
                ImGui::Button("Player to AI");
                if(ImGui::IsItemClicked()){
                    game->_gameOptions.AIPlaying = true;
                    game->_gameOptions.gameNumber = 3;
                    resetGame();
                }
                ImGui::SetCursorPos(ImVec2 (335, 540));
                ImGui::Button("Undo");
                if(ImGui::IsItemClicked()){
                    game->undo();
                }
                ImGui::SetCursorPos(ImVec2 (400, 540));
                ImGui::Button("Free Mode");
                if(ImGui::IsItemClicked()){
                    resetGame();
                    game->_gameOptions.gameNumber = 0;
                    game->_gameOptions.currentTurnNo = 1;
                    game->_gameOptions.AIPlaying = false;
                }
                ImGui::SetCursorPos(ImVec2 (25, 540));
                ImGui::Button("reset");
                if(ImGui::IsItemClicked()){
                    resetGame();
                    if(game->_gameOptions.gameNumber == 1)
                        game->_gameOptions.currentTurnNo = 1;
                    //game->startGame();
                }
                ImGui::Text(game->getFENstring().c_str());
                ImGui::SetCursorPos(ImVec2 (55, 570));
                ImGui::End();
        }

        //
        // end turn is called by the game code at the end of each turn
        // this is where we check for a winner
        //
        void EndOfTurn() 
        {
            Player *winner = game->checkForWinner();
            if (winner)
            {
                gameOver = true;
                gameWinner = winner->playerNumber();
            }
            if (game->checkForDraw()) {
                gameOver = true;
                gameWinner = -1;
            }
        }
        //reset game
        void resetGame(){
            game->clearBoard();
            game->setUpBoard();
            gameOver = false;
            gameWinner = -1;
            game->_winner = nullptr;
        }
}
