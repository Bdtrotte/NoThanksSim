#include "deck.h"

Deck::Deck()
    : mCoinCnt(0)
    , mPassedPoints(0)
{
    QList<int> unShuff;
    for (int i = 3; i <= 35; ++i) {
        unShuff.append(i);
    }

    while (unShuff.size() > 9) {
        int s = rand() % unShuff.size();
        mCards.append(unShuff.takeAt(s));
    }
}

QPair<int, int> Deck::takeCard()
{
    QPair<int, int> r;
    r.first = mCards.takeLast();
    r.second = mCoinCnt;
    mCoinCnt = 0;

    mPassedPoints += r.first;

    return r;
}
