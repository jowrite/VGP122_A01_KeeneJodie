#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <random>
#include <numeric>

using namespace std;

// FACE CARDS
const int A = 1;
const int J = 11;
const int Q = 12;
const int K = 13;
const int CARD_DECK = 52;

// FUNCTIONS
void shuffleDeck(vector<int>& deck);
void dealCard(vector<int>& hand, vector<int>& deck);
void displayHand(const vector<int>& hand, bool showFirstCard = true);
int getHandValue(const vector<int>& hand);
char getPlayerChoice();
int getPlayerBet(int credits);
bool playerWins(int playerValue, int dealerValue);
bool dealerWins(int playerValue, int dealerValue);
bool isPair(const vector<int>& hand);
bool playAgain(int& wins, int& losses);
void getStats(int wins, int losses);

int main() {

    // RULES
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
                }
                    // DEALER TURN AFTER SPLIT
                    std::cout << "\nDealer's turn:\n";
                    displayHand(dealerHand, true);
                    while (getHandValue(dealerHand) < 17) {
                        dealCard(dealerHand, deck);
                        displayHand(dealerHand, true);
                        if (getHandValue(dealerHand) > 21) {
                            std::cout << "Dealer busts! YOU WIN.\n";
                            playerCredits += playerBet;
                            wins++;
                            break;
                        }
                    }
                    // COMPARE HANDS
                    int playerValue = getHandValue(playerHand);
                    int secondValue = getHandValue(secondHand);
                    if (playerWins(playerValue, getHandValue(dealerHand)) || playerWins(secondValue, getHandValue(dealerHand))) {
                        std::cout << "You win!\n";
                        playerCredits += playerBet;
                        wins++;
                    }
                    else if (dealerWins(playerValue, getHandValue(dealerHand)) || dealerWins(secondValue, getHandValue(dealerHand))) {
                        std::cout << "Dealer wins!\n";
                        playerCredits -= playerBet;
                        losses++;
                    }
                    else {
                        std::cout << "It's a tie!\n";
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
        if (!playAgain(wins, losses)) {
            break;
        }
    }
    return 0;
}

// SHUFFLE DECK
void shuffleDeck(vector<int>& deck) {
    random_device rd;
    mt19937 g(rd());
    shuffle(deck.begin(), deck.end(), g);
}

// DEAL CARD
void dealCard(vector<int>& hand, vector<int>& deck) {
    if (!deck.empty()) {
        hand.push_back(deck.back());
        deck.pop_back();
    }
}

// DISPLAY HAND
void displayHand(const vector<int>& hand, bool showFirstCard) {
    for (size_t i = 0; i < hand.size(); ++i) {
        if (i == 0 && !showFirstCard) {
            std::cout << "Hidden\n";
        }
        else {
            int card = hand[i] % 13 + 1;
            switch (card) {
            case A:
                std::cout << "A of ";
                break;
            case J:
                std::cout << "J of ";
                break;
            case Q:
                std::cout << "Q of ";
                break;
            case K:
                std::cout << "K of ";
                break;
            default:
                std::cout << card << " of ";
                break;
            }
            switch ((hand[i] - 1) / 13) {
            case 0:
                std::cout << "Hearts\n";
                break;
            case 1:
                std::cout << "Diamonds\n";
                break;
            case 2:
                std::cout << "Clubs\n";
                break;
            case 3:
                std::cout << "Spades\n";
                break;
            }
        }
    }
}

// GET HAND VALUE
int getHandValue(const vector<int>& hand) {
    int value = 0;
    int aces = 0;
    for (int card : hand) {
        int cardValue = card % 13 + 1;
        if (cardValue > 10) {
            cardValue = 10; // FACE CARDS
        }
        else if (cardValue == A) {
            ++aces; // COUNT ACES SEPARATELY
            cardValue = 11; // DEFAULT VALUE FOR ACE IS 11
        }
        value += cardValue;
    }
    while (value > 21 && aces > 0) {
        value -= 10; // CONVERT AN ACE FROM 11 TO 1
        --aces;
    }
    return value;
}

// GET PLAYER CHOICE
char getPlayerChoice() {
    char choice;
    while (true) {
        std::cout << "\nDo you want to HIT (h), STAND (s), DOUBLE DOWN (d), or PASS (x)? ";
        std::cin >> choice;
        choice = tolower(choice);

        if (choice == 'h' || choice == 's' || choice == 'd' || choice == 'x') {
            return choice; 
        }
            else {
                std::cout << "Invalid choice. Please enter 'h', 's', 'd', or 'x'.\n";
                std::cin.clear();
                std:cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

// GET PLAYER BET
int getPlayerBet(int credits) {
    int bet;
    while (true) {
        std::cout << "\nEnter your bet (1-" << credits << "): ";
        std::cin >> bet;
        if (std::cin.fail() || bet < 1 || bet > credits) {
            std::cout << "Invalid input! Please enter a number between 1 and " << credits << ".\n";
            std::cin.clear();
            std::cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear & remove invalid input
        }
        else {
            break;
        }
    } while (bet < 1 || bet > credits);
    return bet;
}

// PLAYER WINS
bool playerWins(int playerValue, int dealerValue) {
    return playerValue <= 21 && (dealerValue > 21 || playerValue > dealerValue);
}

// DEALER WINS
bool dealerWins(int playerValue, int dealerValue) {
    return dealerValue <= 21 && (playerValue > 21 || dealerValue > playerValue);
}

// CHECK FOR PAIR
bool isPair(const vector<int>& hand) {
    return hand.size() == 2 && (hand[0] % 13 == hand[1] % 13);
}

// PLAY AGAIN
bool playAgain(int& wins, int& losses) {
    char choice;
    std::cout << "\nDo you want to play again? (y/n): ";
    std::cin >> choice;
    if (choice == 'y') {
        wins = 0;
        losses = 0;
        return true;
    }
    return false;
}

// DISPLAY STATS
void getStats(int wins, int losses) {
    std::cout << "WINS: " << wins << "\n";
    std::cout << "LOSSES: " << losses << "\n\n";
}
