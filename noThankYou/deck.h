#ifndef DECK_H
#define DECK_H

#include <QList>
#include <QPair>

class Deck
{
public:
    Deck();

    bool isDeckEmpty() const { return mCards.isEmpty(); }

    int currentCard() const { return mCards.back(); }
    int coinCnt() const { return mCoinCnt; }

    int passedPoints() const { return mPassedPoints; }

    QPair<int, int> takeCard();
    void passCard() { ++mCoinCnt; }

private:
    QList<int> mCards;
    int mCoinCnt;

    int mPassedPoints;
};

#endif // DECK_H
