// ExamenCPP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <algorithm> 
#include <ctime>
#include <string>
#include <vector>

class Card
{
private:
    char m_card;
    int m_value,
        m_suite;
    std::string m_nameS;
    bool m_given;        // Checks if the card is given

    int assignValue();

public:
    Card(char, int);
    Card(const Card&);

    int getValue()        const;
    int getSuite()        const;
    std::string getName() const;
    bool getIfGiven()     const;
    char getCard()        const;

    void setGiven(bool x);

    void nameCard();
};

int Card::assignValue()
{
    if (m_card == 'A') return 11;
    else if (m_card == 'T' || m_card == 'J' || m_card == 'Q') return 10;
    else return (m_card - '0');
}

Card::Card(char cardV, int suite)
{
    m_card = cardV;
    m_value = assignValue();
    m_suite = suite;
    m_given = false;
}

Card::Card(const Card& card)
{
    m_card = card.getCard();
    m_value = card.getValue();
    m_suite = card.getSuite();
    m_nameS = card.getName();
    m_given = card.getIfGiven();
}

int Card::getValue() const
{
    return m_value;
}

int Card::getSuite() const
{
    return m_suite;
}

std::string Card::getName() const
{
    return m_nameS;
}

bool Card::getIfGiven() const
{
    return m_given;
}

char Card::getCard() const
{
    return m_card;
}

void Card::setGiven(bool x)
{
    m_given = x;
}



void Card::nameCard()
{
    switch (m_suite)
    {
    case 0: m_nameS = "spades"; break;
    case 1: m_nameS = "clubs"; break;
    case 2: m_nameS = "hearts"; break;
    case 3: m_nameS = "diamonds";
    }
    if (m_card == 'A') std::cout << "Ace";
    else if (m_card == 'J') std::cout << "Jack";
    else if (m_card == 'Q') std::cout << "Queen";
    else                    std::cout << m_value;

    std::cout << " of " << m_nameS << std::endl;
}

class Deck
{
private:
    std::vector<Card*> m_deck;

public:
    Deck();

    void fillDeck();
    void shuffle();
    Card giveCard();
};

Deck::Deck()
{
    // Initialize the deck: 
    fillDeck();
    shuffle();
}

void Deck::fillDeck()
{
    for (int i = 1; i <= 12; ++i)
    {
        char c;
        switch (i) {
        case 1:  c = 'A'; break;
        case 2:  c = '2'; break;
        case 3:  c = '3'; break;
        case 4:  c = '4'; break;
        case 5:  c = '5'; break;
        case 6:  c = '6'; break;
        case 7:  c = '7'; break;
        case 8:  c = '8'; break;
        case 9:  c = '9'; break;
        case 10: c = 'T'; break;
        case 11: c = 'J'; break;
        case 12: c = 'Q';
        }
        for (int n = 0; n <= 3; ++n)
        {
            m_deck.push_back(new Card(c, n));
        }
    }
}

void Deck::shuffle()
{
    std::random_shuffle(m_deck.begin(), m_deck.end());
}

Card Deck::giveCard()
{
    for (int i = 0; i < m_deck.size(); ++i)
    {
        if (!m_deck[i]->getIfGiven())
        {
            m_deck[i]->setGiven(1);
            return (*m_deck[i]);
        }
    }
}

class Hand
{

private:
    std::vector<Card> m_hand;

public:

    void init(Card, Card);
    void display();
    void displayAsDealer();
    void addCard(Card);

    int getNumOfCards();
    int getTotalVal();
};

void Hand::init(Card card1, Card card2)
{
    m_hand.push_back(card1);
    m_hand.push_back(card2);
}

void Hand::display()
{
    for (std::vector<Card>::iterator iter = m_hand.begin();
        iter != m_hand.end(); (++iter))
    {
        iter->nameCard();
    }
}

void Hand::displayAsDealer()
{
    m_hand[0].nameCard();
}

void Hand::addCard(Card card)
{
    std::cout << "\nTook out: ";
    card.nameCard();

    m_hand.push_back(card);
}

int Hand::getTotalVal()
{
    int sum = 0;
    for (std::vector<Card>::iterator iter = m_hand.begin();
        iter != m_hand.end(); (++iter))
    {
        sum += iter->getValue();
    }
    return sum;
}

int Hand::getNumOfCards()
{
    return m_hand.size();
}

class Player
{
protected:
    Deck m_deck;
    Hand m_hand;
    bool m_canGetCards;
    int score;

public:
    Player(Deck deck);
    void plant();
    void anotherCard();
    void seeCards();

    int getScore() const;

    bool lost();
    bool won();
};

Player::Player(Deck deck)
{
    m_canGetCards = 1;
    m_deck = deck;
    m_hand.init(deck.giveCard(), deck.giveCard());
}

void Player::plant()
{
    m_canGetCards = 0;
    score = m_hand.getTotalVal();
}

void Player::anotherCard()
{
    if (m_canGetCards) m_hand.addCard(m_deck.giveCard());
    else std::cout << "You cannot get more cards" << std::endl;
}

bool Player::lost()
{
    if (m_hand.getTotalVal() > 21)
    {
        std::cout << "You exceeded 21, you lost!" << std::endl;
        return true;
    }
    return false;
}

bool Player::won()
{
    return (m_hand.getTotalVal() == 21 && m_hand.getNumOfCards() == 2);
}

void Player::seeCards()
{
    std::cout << "Your cards: \n";
    m_hand.display();
}

int Player::getScore() const
{
    return score;
}

class Dealer : public Player  // Dealer is also a player
{
public:
    using Player::Player;

    void seeCard();
    void game(Player);
};

void Dealer::seeCard()
{
    std::cout << "\nDealer's card: \n";
    m_hand.displayAsDealer();
    std::cout << std::endl;
}

void Dealer::game(Player play)
{
    while (m_hand.getTotalVal() <= 21)
    {
        std::cout << "\nDealer's cards: ";
        m_hand.display();
        std::cout << std::endl;
        if (m_hand.getTotalVal() > play.getScore() && m_hand.getTotalVal() <= 21)
        {
            std::cout << "The dealer has more points, you lost!" << std::endl;
            return;
        }
        if (m_hand.getTotalVal() == play.getScore())
        {
            std::cout << "A tie! " << std::endl;
            return;
        }
        anotherCard();
    }
    if (m_hand.getTotalVal() > 21)
    {
        std::cout << "The dealer exceeded 21, you won!";
        return;
    }
}

void introduct()
{
    std::cout << "Rules: take cards and get as close as possible to 21. If you exceed 21: you lose. You can take up to 5 cards\n\n";
    std::cout << "Card value: \nTwo to ten = 2 till 10 points, \nJack, Queen and King = 10 Points, \nAce = 11 Points\n\n";
    std::cout << "Controls: \n1 for take another card" << std::endl;
    std::cout << "2 for planting" << std::endl;
    std::cout << "\n\n";
    std::cout << "Have Fun!! \n\n" << std::endl;
}

int main()
{
    srand(time(0));

    introduct();

    Deck deck;
    Player you(deck);
    Dealer dealer(deck);
    char x;
    bool bbreak = 0;

    you.seeCards();
    dealer.seeCard();

    if (you.won())
    {
        std::cout << "You won! " << std::endl;
    }
    else if (dealer.won())
    {
        std::cout << "The dealer won!" << std::endl;
    }
    else
    {
        while (!you.lost())
        {
            std::cout << "What do you want to do: ";
            std::cin >> x;
            if (x == '1')
            {
                you.anotherCard();
            }
            else if (x == '2')
            {
                you.plant();
                dealer.game(you);
                break;

            }
            system("cls");
            introduct();
            you.seeCards();
            dealer.seeCard();
        }
    }

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
