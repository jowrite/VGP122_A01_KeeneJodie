// main.cpp

#include "blackjack.h"

int main() {
    std::cout << "Let's play BlackJack!\n\n";
    std::cout << "Before we get started, please review the rules of the game below: \n\n";
    std::cout << "- The objective of the game is to beat the dealer by getting a higher score, without going over 21\n";
    std::cout << "- If the game is tied, the bet is returned to the player\n";
    std::cout << "- The game begins by prompting the player to place a bet using the available credits\n";
    std::cout << "- Cards are dealt after betting, two cards dealt to the player face up, and two cards dealt to the dealer, one card face up, one hidden\n";
    std::cout << "- The player can request to HIT and will receive another card face up. The player can HIT until they reach a card count greater than 21\n";
    std::cout << "- The player can request to STAND and the hand will be passed to the dealer\n";
    std::cout << "- If a pair of cards is dealt on the initial deal, it can be SPLIT into two hands and another card will be dealt for each hand\n";
    std::cout << "- The player can also DOUBLE DOWN when the original hand totals 9, 10, or 11. This doubles their bet and the player will receive one card face up\n";
    std::cout << "- The player can also PASS after the original hand is dealt. The round will end and the dealer takes half the bet\n";
    std::cout << "- The dealer must HIT until reaching a card count of 17 or higher, but not exceeding 21\n";
    std::cout << "\nThat's all folks. Good luck and have fun!\n\n";

    srand(time(0)); // RANDOM NUMBER GENERATOR

    int wins = 0;
    int losses = 0;

    while (true) {
        int playerCredits = 1000;
        vector<int> deck(CARD_DECK);
        iota(deck.begin(), deck.end(), 1);

        shuffleDeck(deck);

        while (playerCredits > 0) {
            std::cout << "You have " << playerCredits << " credits.\n";
            int playerBet = getPlayerBet(playerCredits);

            getStats(wins, losses);

            vector<int> playerHand;
            vector<int> dealerHand;

            // DEAL
            dealCard(playerHand, deck);
            dealCard(playerHand, deck);
            dealCard(dealerHand, deck);
            dealCard(dealerHand, deck);

            // DISPLAY HAND
            std::cout << "Your hand:\n";
            displayHand(playerHand);
            std::cout << "\nDealer's hand:\n";
            displayHand(dealerHand, false);

            // CHECK FOR SPLIT
            if (isPair(playerHand)) {
                char splitChoice;
                while (true) {
                    std::cout << "\nDo you want to SPLIT (y/n)? ";
                    std::cin >> splitChoice;
                    splitChoice = tolower(splitChoice);
                    if (splitChoice == 'y' || splitChoice == 'n') {
                        break;
                    }
                    else {
                        std::cout << "Invalid choice. Please enter 'y' or 'n'.\n";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                }

                if (splitChoice == 'y') {
                    // SPLIT
                    vector<int> secondHand;
                    secondHand.push_back(playerHand.back());
                    playerHand.pop_back();
                    dealCard(playerHand, deck);
                    dealCard(secondHand, deck);

                    // PLAYER TURN IF SPLIT
                    std::cout << "First hand:\n";
                    displayHand(playerHand);
                    char choice;
                    while ((choice = getPlayerChoice()) != 's' && choice != 'x') {
                        if (choice == 'h') {
                            dealCard(playerHand, deck);
                            std::cout << "\nYour hand:\n";
                            displayHand(playerHand);
                            if (getHandValue(playerHand) > 21) {
                                std::cout << "BUSTED!\n";
                                playerCredits -= playerBet;
                                losses++;
                                break;
                            }
                        }
                        else if (choice == 'd') {
                            // OPTION TO DOUBLE DOWN
                            playerBet *= 2;
                            dealCard(playerHand, deck);
                            std::cout << "\nYour hand:\n";
                            displayHand(playerHand);
                            break; // Player can't draw any more cards after doubling down
                        }
                    }

                    std::cout << "\nSecond hand:\n";
                    displayHand(secondHand);
                    while ((choice = getPlayerChoice()) != 's' && choice != 'x') {
                        if (choice == 'h') {
                            dealCard(secondHand, deck);
                            std::cout << "\nYour hand:\n";
                            displayHand(secondHand);
                            if (getHandValue(secondHand) > 21) {
                                std::cout << "BUSTED!\n";
                                playerCredits -= playerBet;
                                losses++;
                                break;
                            }
                        }
                        else if (choice == 'd') {
                            // OPTION TO DOUBLE DOWN
                            playerBet *= 2;
                            dealCard(secondHand, deck);
                            std::cout << "\nYour hand:\n";
                            displayHand(secondHand);
                            break; // Player can't draw any more cards after doubling down
                        }
                    }

                    // DEALER TURN AFTER SPLIT
                    std::cout << "\nDealer's turn:\n";
                    displayHand(dealerHand, true);
                    while (getHandValue(dealerHand) < 17) {
                        dealCard(dealerHand, deck);
                        displayHand(dealerHand, true);
                    }

                    int dealerValue = getHandValue(dealerHand);
                    if (dealerValue > 21) {
                        std::cout << "Dealer busts! YOU WIN.\n";
                        playerCredits += playerBet;
                        wins++;
                    }
                    else {
                        // COMPARE HANDS
                        int playerValue = getHandValue(playerHand);
                        int secondValue = getHandValue(secondHand);
                        if (playerWins(playerValue, dealerValue) || playerWins(secondValue, dealerValue)) {
                            std::cout << "You win!\n";
                            playerCredits += playerBet;
                            wins++;
                        }
                        else if (dealerWins(playerValue, dealerValue) || dealerWins(secondValue, dealerValue)) {
                            std::cout << "Dealer wins!\n";
                            playerCredits -= playerBet;
                            losses++;
                        }
                        else {
                            std::cout << "It's a tie!\n";
                        }
                    }
                    continue;
                }
            }

            // PLAYER REGULAR TURN
            char choice;
            while ((choice = getPlayerChoice()) != 's' && choice != 'x') {
                if (choice == 'h') {
                    dealCard(playerHand, deck);
                    std::cout << "Your hand:\n";
                    displayHand(playerHand);
                    if (getHandValue(playerHand) > 21) {
                        std::cout << "BUSTED!\n";
                        playerCredits -= playerBet;
                        losses++;
                        break;
                    }
                }
                else if (choice == 'd') {
                    // OPTION TO DOUBLE DOWN
                    playerBet *= 2;
                    dealCard(playerHand, deck);
                    std::cout << "Your hand:\n";
                    displayHand(playerHand);
                    break; // Player can't draw more cards after doubling down
                }
            }

            if (choice == 'x') {
                std::cout << "Round skipped (half of bet returned).\n";
                playerCredits -= playerBet / 2;
                continue;
            }

            // DEALER TURN
            std::cout << "\nDealer's turn: \n";
            displayHand(dealerHand, true);

            while (getHandValue(dealerHand) < 17) {
                dealCard(dealerHand, deck);
                displayHand(dealerHand, true);
            }

            int dealerValue = getHandValue(dealerHand);
            if (dealerValue > 21) {
                std::cout << "Dealer busts! YOU WIN.\n";
                playerCredits += playerBet;
                wins++;
            }
            else {
                // COMPARE HANDS
                int playerValue = getHandValue(playerHand);
                if (playerWins(playerValue, dealerValue)) {
                    std::cout << "WINNER!\n";
                    playerCredits += playerBet;
                    wins++;
                }
                else if (dealerWins(playerValue, dealerValue)) {
                    std::cout << "YOU LOSE, DEALER WINS!\n";
                    playerCredits -= playerBet;
                    losses++;
                }
                else {
                    std::cout << "PUSH!\n";
                }
            }
        }
        std::cout << "GAME OVER! You've run out of credits.\n";
        if (!playAgain()) {
            break;
        }
    }
    return 0;
}