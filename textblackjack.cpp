#include "blackjack.h"

using namespace std;

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
                cout << "A of ";
                break;
            case J:
                cout << "J of ";
                break;
            case Q:
                cout << "Q of ";
                break;
            case K:
                cout << "K of ";
                break;
            default:
                cout << card << " of ";
                break;
            }
            switch ((hand[i] - 1) / 13) {
            case 0:
                cout << "Hearts\n";
                break;
            case 1:
                cout << "Diamonds\n";
                break;
            case 2:
                cout << "Clubs\n";
                break;
            case 3:
                cout << "Spades\n";
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
        cout << "\nDo you want to HIT (h), STAND (s), DOUBLE DOWN (d), or PASS (x)? ";
        cin >> choice;
        choice = tolower(choice);

        if (choice == 'h' || choice == 's' || choice == 'd' || choice == 'x') {
            return choice;
        }
        else {
            cout << "Invalid choice. Please enter 'h', 's', 'd', or 'x'.\n";
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

// GET PLAYER BET
int getPlayerBet(int credits) {
    int bet;
    while (true) {
        cout << "\nEnter your bet (1-" << credits << "): ";
        cin >> bet;
        if (cin.fail() || bet < 1 || bet > credits) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear & remove invalid input
            cout << "Invalid input! Please enter a number between 1 and " << credits << ".\n";
        }
        else {
            return bet;
        }
    }
    
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
    if (hand.size() == 2) {
        return (hand[0] % 13 == hand[1] % 13);
    }
    return false;
}

// PLAY AGAIN
bool playAgain() {
    char choice;
    cout << "\nDo you want to play again? (y/n): ";
    cin >> choice;
    return choice == 'y';
}

// DISPLAY STATS
void getStats(int wins, int losses) {
    cout << "WINS: " << wins << " | LOSSES: " << losses << endl;
}

