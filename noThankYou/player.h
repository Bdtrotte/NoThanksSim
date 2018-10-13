#ifndef PLAYER_H
#define PLAYER_H

#include "deck.h"

#include <QList>
#include <QPair>
#include <string>
#include <stdio.h>

using namespace std;

class AbstractPlayer
{
public:
    AbstractPlayer(string name);
    void reset()
    {
        mCoinCnt = 11;
        mCards.clear();
    }

    virtual bool takeTurn(Deck &deck, AbstractPlayer **players = nullptr, int playerId = -1, int playerCt = 0) = 0;

    int finalScore();
    int cardScore();
    //the change in score if card is taken with coins
    int scoreChange(int card, int coins);

    string name() { return mName; }

    void printCards();

protected:
    void takeCard(QPair<int, int> cardAndCoins);
    int mCoinCnt;

private:
    void addCard(int c);

    QList<int> mCards;

    string mName;
};

class AlwaysTakePlayer : public AbstractPlayer
{
public:
    AlwaysTakePlayer(string name)
        : AbstractPlayer(name) {}

    bool takeTurn(Deck &deck, AbstractPlayer **)
    {
        takeCard(deck.takeCard());
        return true;
    }
};

class AlwaysPassPlayer : public AbstractPlayer
{
public:
    AlwaysPassPlayer(string name)
        : AbstractPlayer(name) {}

    bool takeTurn(Deck &deck, AbstractPlayer **, int, int)
    {
        if (mCoinCnt > 0) {
            deck.passCard();
            --mCoinCnt;
            return false;
        }

        takeCard(deck.takeCard());
        return true;
    }
};

class StaticCoinValuePlayer : public AbstractPlayer
{
public:
    StaticCoinValuePlayer(string name, int coinValue)
        : AbstractPlayer(name)
        , mCoinValue(coinValue) {}

    bool takeTurn(Deck &deck, AbstractPlayer **, int, int)
    {
        if (mCoinCnt == 0
                || scoreChange(deck.currentCard(), mCoinValue * deck.coinCnt()) < 0) {
            takeCard(deck.takeCard());
            return true;
        }

        deck.passCard();
        --mCoinCnt;
        return false;
    }

private:
    int mCoinValue;
};

class VariableCoinValuePlayer : public AbstractPlayer
{
public:
    VariableCoinValuePlayer(string name, float maxCoinValue)
        : AbstractPlayer(name)
        , mMaxCoinValue(maxCoinValue) {}

    bool takeTurn(Deck &deck, AbstractPlayer **otherPlayers, int playerId, int playerCt)
    {
        if (shouldTake(deck, otherPlayers, playerId, playerCt)) {
            takeCard(deck.takeCard());
            return true;
        }

        deck.passCard();
        --mCoinCnt;
        return false;
    }

protected:
    bool shouldTake(const Deck &deck, AbstractPlayer **, int, int)
    {
        return mCoinCnt == 0 || scoreChange(deck.currentCard(), curCoinValue(deck) * (deck.coinCnt() + 1)) < 0;
    }

    float curCoinValue(const Deck &deck);

    float mMaxCoinValue;
};

class PlayerPlayer : public AbstractPlayer
{
public:
    PlayerPlayer(string name)
        : AbstractPlayer(name) {}

    bool takeTurn(Deck &deck, AbstractPlayer **, int, int)
    {
        printf("Player deck:\n");
        printCards();
        printf("Player turn (t/p): ");
        char c;
        scanf(" %c", &c);
        if (c == 't') {
            printf("Player take\n");
            takeCard(deck.takeCard());
            return true;
        } else {
            printf("Player pass\n");
            deck.passCard();
            --mCoinCnt;
            return false;
        }
    }
};

class OtherPlayerAwareVariableCoinValuePlayer : public AbstractPlayer
{
public:
    OtherPlayerAwareVariableCoinValuePlayer(string name, float maxCoinValue)
        : AbstractPlayer(name)
        , mMaxCoinValue(maxCoinValue) {}

    bool takeTurn(Deck &deck, AbstractPlayer **otherPlayers, int playerId, int playerCt)
    {
        if (shouldTake(deck, otherPlayers, playerId, playerCt)) {
            takeCard(deck.takeCard());
            return true;
        }

        deck.passCard();
        --mCoinCnt;
        return false;
    }

protected:
    bool shouldTake(const Deck &deck, AbstractPlayer **otherPlayers, int playerId, int playerCt)
    {
        if (mCoinCnt == 0 || scoreChange(deck.currentCard(), curCoinValue(deck) * (deck.coinCnt() + 1)) < 0) {
            if (mCoinCnt == 0) return true;
            //taking deemed a good move. Now ask if any other player will take. If not, let it wrap around again (pass)
            int p = (playerId + 1) % playerCt;
            int e = 2;//how many extra coins will be on when it gets to player p
            while (p != playerId) {
                AbstractPlayer *otherP = otherPlayers[p];
                if (otherP->scoreChange(deck.currentCard(), curCoinValue(deck) * (deck.coinCnt() + e)) < 0)
                    return true;

                p = (p + 1) % playerCt;
                ++e;
            }
        }

        return false;
    }

    float curCoinValue(const Deck &deck);

    float mMaxCoinValue;
};

#endif // PLAYER_H
