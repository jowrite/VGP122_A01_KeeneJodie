// blackjack.h

#ifndef BLACKJACK_H
#define BLACKJACK_H

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
bool playAgain();
void getStats(int wins, int losses);

#endif // BLACKJACK_H
