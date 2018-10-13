#include "player.h"

#include <iostream>

using namespace std;

AbstractPlayer::AbstractPlayer(string name)
    : mCoinCnt(11)
    , mName(name)
{

}

int AbstractPlayer::finalScore()
{
    return cardScore() - mCoinCnt;
}

int AbstractPlayer::cardScore()
{
    int t = 0;

    int last = 0;
    for (int i : mCards) {
        if (++last == i) continue;
        t += i;
        last = i;
    }

    return t;
}

int AbstractPlayer::scoreChange(int card, int coins)
{
    int ccs = cardScore();
    QList<int> oc = mCards;
    addCard(card);
    int acs = cardScore();
    mCards = oc;

    return (acs - coins) - ccs;
}

void AbstractPlayer::printCards()
{
    cout << "Coins: " << mCoinCnt;
    int last = 0;
    for (int i : mCards) {
        if (++last == i) {
            cout << i << " ";
            continue;
        }
        cout << endl << i << " ";
        last = i;
    }

    cout << endl;
}

void AbstractPlayer::takeCard(QPair<int, int> cardAndCoins)
{
    addCard(cardAndCoins.first);
    mCoinCnt += cardAndCoins.second;
}

void AbstractPlayer::addCard(int c)
{
    for (int i = 0; i < mCards.size(); ++i) {
        if (c < mCards[i]) {
            mCards.insert(i, c);
            return;
        }
    }
    mCards.append(c);
}

float VariableCoinValuePlayer::curCoinValue(const Deck &deck)
{
    float f = (float)deck.passedPoints() / 492.0f;

    if (f > 1) f = 1;

    return (1 - f) * (mMaxCoinValue - 1) + 1;
}

float OtherPlayerAwareVariableCoinValuePlayer::curCoinValue(const Deck &deck)
{
    float f = (float)deck.passedPoints() / 492.0f;

    if (f > 1) f = 1;

    return (1 - f) * (mMaxCoinValue - 1) + 1;
}
