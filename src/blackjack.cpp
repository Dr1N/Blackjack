#include <iostream>
#include <iomanip>
#include <ctime>
#include <Windows.h>
using namespace std;

const int MAX_CARDS = 10;

//Card

struct Card
{
	char* face;
	char* suit;
	int point:5;
};

//Player

struct PlayerCards
{
	int amount;
	int points;
	Card cards[MAX_CARDS];
};

//Gloals

int card_counter = 0;
int game = 0, wins = 0, loses = 0;

int face_p[] = {11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 2, 3, 4};
char* face[] = {"A", "2", "3", "4", "5", "6", "7", "8","9","10","J","Q","K"};
char *suit[] = {"\x3", "\x4", "\x5", "\x6"};

enum {TAKE = 1, ENOUGH};

//Declare functions 

void fillDeck(Card* const, char* [], char* []);
void shuffleDeck(Card* const);
void dealCard(Card* const, PlayerCards&, int&);
void printCards(PlayerCards& const);
void printCards(Card* const);
void printGameData(PlayerCards& const, PlayerCards& const, bool const);
void firstTurn(Card* const, PlayerCards&, PlayerCards&);
bool isGold(PlayerCards& const);
void setNull(PlayerCards&, PlayerCards&, bool&);
void showResult(PlayerCards& const, PlayerCards& const);

void main()
{
	Card deck[52];
	PlayerCards player, computer;
	int action = 0, next = 0;
	bool end_turn = false;
		
	//Fill deck

	srand((int)time(0));
	fillDeck(deck, face, suit);
	shuffleDeck(deck);
	
	while(true)
	{
		game++;

		//game begins

		setNull(player, computer, end_turn);
		firstTurn(deck, player, computer);
		printGameData(player, computer, false);
		
		//first check

		if(isGold(computer) || computer.points == 21)
		{
			loses++;
			cout<<"\nComputer win\n";
			setNull(player, computer, end_turn);
			continue;
		} 
		else if (isGold(player))
		{
			wins++;
			cout<<"\nYou win\n";
			setNull(player, computer, end_turn);
			continue;
		}
		
		cin.clear();
		cin.sync();

		//players game

		while (player.points <= 21)
		{
			if (end_turn) break;
			do
			{
				cout<<"\nAction (1 - take card, 2 - enough):\t";
				cin>>action;
			} while (!(action == 1 || action == 2));
		
			switch (action)
			{
			case TAKE:
				dealCard(deck, player, card_counter);
				printGameData(player, computer, false);
				break;
			case ENOUGH:
				end_turn = true;
				break;
			}
		}

		printGameData(player, computer, true);

		//computers game 

		while (computer.points < 21 && player.points <= 21)
		{
			Sleep(500);
			if(computer.points < player.points)
			{
				dealCard(deck, computer, card_counter);
				printGameData(player, computer, true);
			}
			else if(computer.points >= player.points || computer.points == 21)
				break;
		}

		//result

		showResult(player, computer);
		
		//play more

		cout<<"\nNext game? (0 - exit): ";
		cout.flush().clear();
		cin>>next;
		if(!next) 
			return;

		//shuffle deck

		if (card_counter >= 52 / 2)
		{
			card_counter = 0;
			shuffleDeck(deck);
		}
	}
}	

void fillDeck(Card* const wDeck, char *wFace[], char *wSuit[])
{
	for(int i = 0; i < 52; i++)
	{
		wDeck[i].face = wFace[i % 13];
		wDeck[i].suit = wSuit[i / 13];
		wDeck[i].point = face_p[i % 13];
	}
}

void shuffleDeck(Card * const wDeck)
{
	for(int i = 0; i < 52; i++)
	{
		int j = rand() % 52;
		Card temp = wDeck[i];
		wDeck[i] = wDeck[j];
		wDeck[j] = temp;
	}
}

void dealCard (Card * const wDeck, PlayerCards& pCards, int& count)
{
	pCards.cards[pCards.amount++] = wDeck[count];
	pCards.points += wDeck[count].point;
	count++;
}

void printCards(PlayerCards& const pCards)
{
	for(int i = 0; i < pCards.amount; i++)
		cout<<pCards.cards[i].face
			<<pCards.cards[i].suit<<'\t';
	cout<<endl<<pCards.points<<endl;
}

void printCards(Card* const wDeck)
{
	for(int i = 0; i < 52; i++)
		cout<< wDeck[i].face
			<< wDeck[i].suit<<":"<<wDeck[i].point<<'\t';
	cout<<endl;
}

void printGameData(PlayerCards& const p1Cards, PlayerCards& const p2Cards, bool const showCard)
{
	system("cls");
	cout<<"==================="<<endl;
	cout<<"\tStatistic:\n";
	cout<<"==================="<<endl;
	cout<<"Game:\t\t"<<game<<endl;
	cout<<"Player:\t\t"<<wins<<endl;
	cout<<"Computer:\t"<<loses<<endl;
	cout<<"Cards in deck\t"<<52 - card_counter<<endl;
	cout<<endl;

	cout<<"==================="<<endl;
	cout<<"\tPlayer 1:\n";
	cout<<"==================="<<endl;
	if(p1Cards.amount)
	{
		cout<<"Cards:\t";
		for(int i = 0; i < p1Cards.amount; i++)
			cout<<p1Cards.cards[i].face<<p1Cards.cards[i].suit<<'\t';
		cout<<endl<<"Score:\t"<<p1Cards.points<<endl;
	}
	
	cout<<"==================="<<endl;
	cout<<"\tPlayer 2:\n";
	cout<<"==================="<<endl;
	if(p2Cards.amount)
	{
		cout<<"Cards:\t";
		if(!showCard)
		{
			for(int i = 0; i < p2Cards.amount; i++)
				cout<<"X"<<"x"<<'\t';
			cout<<endl<<"Score:\t"<<"X"<<endl;
		}
		else
		{
			for(int i = 0; i < p2Cards.amount; i++)
				cout<<p2Cards.cards[i].face<<p2Cards.cards[i].suit<<'\t';
			cout<<endl<<"Score:\t"<<p2Cards.points<<endl;
		}
	}
}

void firstTurn(Card* const wDeck, PlayerCards& p1Cards, PlayerCards& p2Cards)
{
	dealCard(wDeck, p1Cards, card_counter);
	dealCard(wDeck, p1Cards, card_counter);
	dealCard(wDeck, p2Cards, card_counter);
	dealCard(wDeck, p2Cards, card_counter);
}

bool isGold(PlayerCards& const pCard)
{
	return pCard.amount == 2 && pCard.points == 22;
}

void setNull(PlayerCards& p1Cards, PlayerCards& p2Cards, bool& end)
{
	p1Cards.points = 0;
	p1Cards.amount = 0;
	p2Cards.points = 0;
	p2Cards.amount = 0;
	end = false;
}

void showResult(PlayerCards& const pCards, PlayerCards& const cCards)
{
	if(pCards.points > 21)
	{
		loses++;
		cout<<"\nComputer win\n";
	} 
	else if (cCards.points > 21)
	{
		wins++;
		cout<<"\nYou win\n";
	}
	else if(pCards.points <= cCards.points)
	{
		loses++;
		cout<<"\nComputer win\n";
	}
	else if(pCards.points > cCards.points)
	{
		wins++;
		cout<<"\nYou win\n";
	}
}