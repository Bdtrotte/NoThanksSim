#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <chrono>
#include <time.h>

#include <QList>

#include "deck.h"
#include "player.h"

using namespace std;

AbstractPlayer *players[] =
{
    new OtherPlayerAwareVariableCoinValuePlayer("Davd", 6),
    new OtherPlayerAwareVariableCoinValuePlayer("Beth", 6),
    new OtherPlayerAwareVariableCoinValuePlayer("Carl", 6),
    new OtherPlayerAwareVariableCoinValuePlayer("Alex", 6),
    new PlayerPlayer("Player")
};

QPair<int, int> wins[] =
{
    QPair<int, int>(0, 0),
    QPair<int, int>(0, 0),
    QPair<int, int>(0, 0),
    QPair<int, int>(0, 0),
    QPair<int, int>(0, 0)
};

void shufflePlayers()
{
    QList<int> ints = { 0, 1, 2, 3, 4 };
    QList<int> map;

    while (!ints.isEmpty())
        map.append(ints.takeAt(rand() % ints.size()));

    AbstractPlayer *p[5];
    QPair<int, int> w[5];
    for (int i = 0; i < 5; ++i) {
        p[i] = players[map[i]];
        w[i] = wins[map[i]];
    }

    for (int i = 0; i < 5; ++i) {
        players[i] = p[i];
        wins[i] = w[i];
    }
}

void playGame(bool printGame, bool printResults, chrono::duration<int64_t> sleepTime)
{
    Deck deck;

    if (printGame)
        cout << "New game!\nFirst card is: " << deck.currentCard() << endl;

    int curCard = deck.currentCard();
    int coinCoint = deck.coinCnt();

    while (!deck.isDeckEmpty()) {
        for (int i = 0; i < 5; ++i) {
            AbstractPlayer *p = players[i];
            if (printGame)
                cout << p->name() << "'s turn: \n";
            if (p->takeTurn(deck, players, i, 5)) {
                if (printGame)
                    cout << "Takes card: " << curCard << " with " << coinCoint << " coins.\n";
                if (deck.isDeckEmpty()) goto GameEnd;
                curCard = deck.currentCard();
                coinCoint = 0;
                if (printGame)
                    cout << "New card: " << curCard << endl;
            } else if (printGame) {
                cout << "Passes card\nCoin count is " << (coinCoint = deck.coinCnt()) << endl;
            }

            if (printGame)
                this_thread::sleep_for(sleepTime);
        }
    }

    GameEnd:
    int winner = 0;
    int score = players[0]->finalScore();
    wins[0].second += score;
    for (int i = 1; i < 5; ++i) {
        int fs = players[i]->finalScore();
        wins[i].second += fs;
        if (fs < score) {
            score = fs;
            winner = i;
        }
    }
    ++wins[winner].first;

    if (printResults) {
        cout << "Final scores!: \n";
        for (AbstractPlayer *p : players) {
            cout << p->name() << " : " << p->finalScore() << endl;
            p->printCards();
            cout << endl;
        }

        cout << "Winner is " << players[winner]->name() << " with " << score << endl << endl;
    }

    for (AbstractPlayer *p : players) {
        p->reset();
    }
}

int main()
{

    srand(time(nullptr));

    for (int i = 0; i < 1; ++i) {
        playGame(true, false, chrono::seconds(0));
        shufflePlayers();
    }

    for (int i = 0; i < 5; ++i) {
        printf("%s: %10d %10d\n", players[i]->name().c_str(), wins[i].first, wins[i].second);
    }

    return EXIT_SUCCESS;
}
