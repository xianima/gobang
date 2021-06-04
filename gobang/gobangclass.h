#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <sstream>
#include <iterator>
#include <memory>
#include <deque>
#include <map>
#include <cmath>


using namespace std;

class Node;
typedef shared_ptr<Node> Nodeptr;
enum chessType { black = -1, white = 1, null_chess = 0 };
enum scoal { lev_min = -50000, lev1 = 0, lev2 = 10, lev3 = 100, lev4 = 1000, lev5 = 20000, lev_max = 50000 };

class Chess
{
private:
	chessType _type;
public:
	Chess(chessType type = null_chess);
	Chess& operator=(const Chess& c);
	friend bool operator==(const Chess& a, const Chess& b);
	friend bool operator!=(const Chess& a, const Chess& b);
	friend ostream& operator<<(ostream &os, const Chess& c);
}; 

class ChessBoard
{
private:
	int _lines;
	int _ldx, _ldy;
	int _rux, _ruy;
	vector<vector<Chess>> _board;
	int _is_finish;
	chessType winner;
	void TestFinish(pair<int, int> pos, chessType type);
	bool TestFull();
	bool TestLine(pair<int, int> pos, chessType type, int xdiff, int ydiff);
	int TestString(vector<string>& str);
public:
	ChessBoard();
	//ÆåÅÌÆÀ¹Àº¯Êý
	bool HasChess(pair<int, int> pos, int range) const;
	int Evaluate(chessType type);
	bool SetChess(pair<int, int> pos, chessType type);
	int GetLdx() const;
	int GetLdy() const;
	int GetRux() const;
	int GetRuy() const;
	void ShowBoard();
	bool IsFinish();
	int GetChessNum();
	pair<int, int> FindChess(chessType type);
	chessType GetWinner();
	
};

class Player
{
protected:
	chessType _type;
public:
	Player(chessType type = null_chess);
	virtual ~Player() {};
	void SetType(chessType type);
	chessType GetType();
	virtual bool PutChess(ChessBoard& board) = 0;
};

class Node :public enable_shared_from_this<Node>
{
private:
	ChessBoard _b;
	chessType _type;
	pair<int, int> _new_step;
	int _value;
	int _depth;
	vector<Nodeptr> _child;
	//vector<Nodeptr>::iterator _cur_child;
	//Nodeptr _par;
public:
	Node(const ChessBoard& board, pair<int, int> new_step, chessType type, int depth = 0);
	void SetDepth(int depth);
	int GetDepth() const;
	void SetValue(int value);
	int GetValue() const;
	void SetChild();
	void ResetValue();
	pair<int, int> GetPos() const;
	//void SetParent(Nodeptr par);
	vector<Nodeptr>& GetChild();
	//Nodeptr GetParent() const;
	ChessBoard GetChessBoard() const;
};

class AiPlayer :public Player
{
private:
	ChessBoard _board;
	Nodeptr _start_node;
	deque<Nodeptr> _open;
	int _max_depth;
	int GetNodeValue(Nodeptr p_node, int depth, int last_value);
	int GetValueByDepth(int depth);
	pair<int, int> GetNextPos();
public:
	AiPlayer(chessType type = null_chess, int depth = 3);
	void SetDepth(int depth);
	bool PutChess(ChessBoard& board);
};

class HumanPlayer :public Player
{
public:
	using Player::Player;
	bool PutChess(ChessBoard& board);
};

class Game
{
private:
	ChessBoard _b;
	AiPlayer _ai;
	HumanPlayer _hum;
public:
	Game();
	void PlayGame();
};
