#include "gobangclass.h"

Chess::Chess(chessType type) :_type(type)
{
}

Chess& Chess::operator=(const Chess& c)
{
	if (this == &c)
		return *this;
	_type = c._type;
	return *this;
	// TODO: 在此处插入 return 语句
}

bool operator==(const Chess& a, const Chess& b)
{
	return (a._type == b._type);
}

bool operator!=(const Chess& a, const Chess& b)
{
	return (a._type != b._type);
}

ostream& operator<<(ostream& os, const Chess& c)
{
	if (c._type == black) {
		os << " @";
	}
	else if (c._type == white) {
		os << " o";
	}
	else
		os << " +";
	return os;
	// TODO: 在此处插入 return 语句
}

ChessBoard::ChessBoard() :_lines(15), _is_finish(0), winner(null_chess)
{
	_ldx = _ldy = 14;
	_rux = _ruy = 0;
	_board.resize(_lines);
	for (auto it = _board.begin(); it != _board.end(); it++) {
		it->resize(_lines);
	}
}

bool ChessBoard::HasChess(pair<int, int> pos, int range) const
{
	int lx = pos.first - range; if (lx < 0) lx = 0;
	int ly = pos.second - range; if (ly < 0) ly = 0;
	int ux = pos.first + range; if (ux >= _lines) ux = _lines - 1;
	int uy = pos.second + range; if (uy >= _lines) uy = _lines - 1;

	for (int i = lx; i <= ux; i++) {
		for (int j = ly; j <= uy; j++) {
			if (_board[i][j] != Chess(null_chess))
				return true;
		}
	}
	return false;
}

int ChessBoard::Evaluate(chessType type)
{
	vector<string> _board_string;
	string str;
	int value = 0;
	//横向转string
	for (int i = 0; i < _lines; i++) {
		str = "";
		for (int j = 0; j < _lines; j++) {
			if (_board[i][j] == Chess(type))
				str += "Y";
			else if (_board[i][j] == Chess(null_chess))
				str += "0";
			else
				str += "N";
		}
		_board_string.push_back(str);
	}
	//纵向转string
	for (int i = 0; i < _lines; i++) {
		str = "";
		for (int j = 0; j < _lines; j++) {
			if (_board[j][i] == Chess(type))
				str += "Y";
			else if (_board[j][i] == Chess(null_chess))
				str += "0";
			else
				str += "N";
		}
		_board_string.push_back(str);
	}
	//45du转string
	for (int m = 0; m < _lines; m++) {
		if (m == 0) {
			for (int n = 14; n >= 0; n--) {
				str = "";
				int i = m, j = n; 
				while (i < _lines && j < _lines){
					if (_board[i][j] == Chess(type))
						str += "Y";
					else if (_board[i][j] == Chess(null_chess))
						str += "0";
					else
						str += "N";
					i++; j++;
				}
				_board_string.push_back(str); 
			}
		}
		else {
			str = "";
			int n = 0;
			int i = m, j = n;
			while (i < _lines && j < _lines) {
				if (_board[i][j] == Chess(type))
					str += "Y";
				else if (_board[i][j] == Chess(null_chess))
					str += "0";
				else
					str += "N";
				i++; j++;
			}
			_board_string.push_back(str);
		}
	}

	//135du转string
	for (int n = 0; n < _lines; n++) {
		if (n == 14) {
			for (int m = 0; m < _lines; m++) {
				str = "";
				int i = m, j = n;
				while (i < _lines && j >= 0) {
					if (_board[i][j] == Chess(type))
						str += "Y";
					else if (_board[i][j] == Chess(null_chess))
						str += "0";
					else
						str += "N";
					i++; j--;
				}
				_board_string.push_back(str);
			}
		}
		else {
			str = "";
			int m = 0;
			int i = m, j = n;
			while (i < _lines && j >= 0) {
				if (_board[i][j] == Chess(type))
					str += "Y";
				else if (_board[i][j] == Chess(null_chess))
					str += "0";
				else
					str += "N";
				i++; j--;
			}
			_board_string.push_back(str);
		}
	}

	// 遍历str
	//// test cout
	//for (auto it = _board_string.begin(); it != _board_string.end(); it++) {
	//	cout << *it << endl;
	//}
	value = TestString(_board_string);
	return value;
}

void ChessBoard::TestFinish(pair<int, int> pos, chessType type)
{
	if (TestFull()) {
		_is_finish = 1;
		winner = null_chess;
	}
	//检测横向
	if (TestLine(pos, type, 1, 0)) {
		_is_finish = 1;
		winner = type;
	}
	//检测纵向
	if (TestLine(pos, type, 0, 1)) {
		_is_finish = 1;
		winner = type;
	}
	//检测45°反向
	if (TestLine(pos, type, 1, 1)) {
		_is_finish = 1;
		winner = type;
	}
	//检测135°方向
	if (TestLine(pos, type, 1, -1)) {
		_is_finish = 1;
		winner = type;
	}
}

bool ChessBoard::TestFull()
{
	for (auto it = _board.begin(); it != _board.end(); it++) {
		for (auto it2 = it->begin(); it2 != it->end(); it2++) {
			if (*it2 == Chess(null_chess)) {
				return false;
			}
		}
	}
	return true;
}

bool ChessBoard::TestLine(pair<int, int> pos, chessType type, int xdiff, int ydiff)
{
	int max_nums = 1;
	int x, y;
	//反向检测4个
	for (int i = 1; i < 5; i++) {
		x = pos.first - i * xdiff;
		y = pos.second - i * ydiff;
		if (x < 0 || y < 0 || x > 14 || y > 14) {
			break;
		}
		if (_board[x][y] == Chess(type)) {
			max_nums++;
			continue;
		}
		break;
	}
	//正向检测4个
	for (int i = 1; i < 5; i++) {
		x = pos.first + i * xdiff;
		y = pos.second + i * ydiff;
		if (x < 0 || y < 0 || x > 14 || y > 14) {
			break;
		}
		if (_board[x][y] == Chess(type)) {
			max_nums++;
			continue;
		}
		break;
	}
	//如果连子数量达到5个返回true
	if (max_nums >= 5)
		return true;
	else
		return false;
}

//从string中得到分数
int ChessBoard::TestString(vector<string>& str)
{
	string::size_type index;
	int decr; // 降档
	int y_value=0, n_value=0;
	string y_lev2 = "YY", y_lev3 = "YYY", y_lev4 = "YYYY", y_lev5 = "YYYYY";
	string n_lev2 = "NN", n_lev3 = "NNN", n_lev4 = "NNNN", n_lev5 = "NNNNN";
	string y_lev[5] = { "Y", "YY", "YYY", "YYYY", "YYYYY" };
	string n_lev[5] = { "N", "NN", "NNN", "NNNN", "NNNNN" };
	int goal[5] = { lev1, lev2, lev3, lev4, lev5 };
	for (auto it = str.begin(); it != str.end(); it++) {
		//y得分
		for (int i = 1; i < 5; i++) {
			//i子相连
			index = 0;
			while ((index = it->find(y_lev[i], index)) != string::npos) {
				index += (i + 1);
				//排除i+1子相连
				if (((index - i - 1) != 0) && (it->substr(index - i - 2, 1) == y_lev[0]) || ((index + 1) < it->size()) && (it->substr(index, 1) == y_lev[0]))
					continue;

				//活棋加分，眠棋降档，死棋不加分 （5子不降档）
				if (i == 4) {
					y_value += goal[i];
					continue;
				}
				decr = 0;
				if ((((index - i - 1) != 0) && (it->substr(index - i - 2, 1) == n_lev[0])) || (index - i - 1) == 0)
					decr++;
				if ((((index) < it->size()) && (it->substr(index, 1) == n_lev[0])) || (index) == it->size())
					decr++;

				if (decr == 2) continue;
				else if (decr == 1) y_value += goal[i-1];
				else y_value += goal[i];
			}
		}
		//n得分
		for (int i = 1; i < 6; i++) {
			//i子相连
			index = 0;
			while ((index = it->find(n_lev[i], index)) != string::npos) {
				index += (i + 1);
				//排除i+1子相连
				if (((index - i - 1) != 0) && (it->substr(index - i - 2, 1) == n_lev[0]) || ((index + 1) < it->size()) && (it->substr(index, 1) == n_lev[0]))
					continue;
				//活棋加分，眠棋降档，死棋不加分
				if (i == 4) {
					y_value += goal[i];
					continue;
				}
				decr = 0;
				if ((((index - i - 1) != 0) && (it->substr(index - i - 2, 1) == y_lev[0])) || (index - i - 1) == 0)
					decr++;
				if ((((index) < it->size()) && (it->substr(index,1) == y_lev[0])) || (index) == it->size())
					decr++;

				if (decr == 2) continue;
				else if (decr == 1) n_value += goal[i-1]; 
				else n_value += goal[i];
			}
		}
	}
	return (y_value - n_value);
}

bool ChessBoard::SetChess(pair<int, int> pos, chessType type)
{
	if (pos.first < 0 || pos.first >14 || pos.second < 0 || pos.second > 14) {
		return false;
	}
	if (_board[pos.first][pos.second] != Chess(null_chess)) {
		return false;
	}
	_board[pos.first][pos.second] = Chess(type);
	TestFinish(pos, type);

	//扩展下一子的检测范围
	int ldx = pos.first - 2; if (ldx < 0) ldx = 0;
	int ldy = pos.second - 2; if (ldy < 0) ldy = 0;
	int rux = pos.first + 2; if (rux >= _lines) rux = _lines - 1;
	int ruy = pos.second + 2; if (ruy >= _lines) ruy = _lines - 1;
	if (ldx < _ldx) _ldx = ldx;
	if (ldy < _ldy) _ldy = ldy;
	if (rux > _rux) _rux = rux;
	if (ruy > _ruy) _ruy = ruy;

	return true;
}

int ChessBoard::GetLdx() const
{
	return _ldx;
}

int ChessBoard::GetLdy() const
{
	return _ldy;
}

int ChessBoard::GetRux() const
{
	return _rux;
}

int ChessBoard::GetRuy() const
{
	return _ruy;
}

void ChessBoard::ShowBoard()
{
	cout.width(4);
	cout << " ";
	for (int i = 0; i < _lines; i++) {
		cout.width(4);
		cout << i;
	}
	cout << endl << endl;
	for (int i = 0; i < _lines; i++) {
		cout.width(4);
		cout << i;
		for (int j = 0; j < _lines; j++) {
			cout.width(4);
			cout << _board[i][j];
		}
		cout << endl << endl;
	}
}

bool ChessBoard::IsFinish()
{
	if (_is_finish == 0)
		return false;
	else
		return true;
}

int ChessBoard::GetChessNum()
{
	int num = 0;
	for (int i = 0; i < _lines; i++) {
		for (int j = 0; j < _lines; j++) {
			if (_board[i][j] != Chess(null_chess)) {
				num++;
			}
		}
	}
	return num;
}

pair<int, int> ChessBoard::FindChess(chessType type)
{
	int i, j;
	for (i = 0; i < _lines; i++) {
		for (j = 0; j < _lines; j++) {
			if (_board[i][j] == Chess(type)) {
				break;
			}
		}
		if (j < _lines) {
			break;
		}
	}
	return pair<int, int>(i, j);
}

chessType ChessBoard::GetWinner()
{
	return winner;
}

Player::Player(chessType type) :_type(type)
{
}

chessType Player::GetType()
{
	return _type;
}

void Player::SetType(chessType type)
{
	_type = type;
}

int AiPlayer::GetNodeValue(Nodeptr p_node, int depth, int last_value)
{
	p_node->ResetValue();
	vector<Nodeptr> child = p_node->GetChild();
	int value;
	if (child.empty() || p_node->GetDepth() == depth) {
		value = p_node->GetChessBoard().Evaluate(_type);
		p_node->SetValue(value);
		return value;
	}
	for (auto it = child.begin(); it != child.end(); it++) {
		if ((p_node->GetDepth() % 2 == 0 && p_node->GetValue() < last_value) || p_node->GetDepth() == 0) { // max layer value 小于上层 value或者第一层
			p_node->SetValue(GetNodeValue(*it, depth, p_node->GetValue()));
		}
		if (p_node->GetDepth() % 2 != 0 && p_node->GetValue() > last_value) { // max layer
			p_node->SetValue(GetNodeValue(*it, depth, p_node->GetValue()));
		}
	}
	return p_node->GetValue();
}

int AiPlayer::GetValueByDepth(int depth)
{
	vector<Nodeptr> cur_child;
	_open.push_back(_start_node);
	//广度优先遍历
	while (!_open.empty()) {
		Nodeptr cur = _open.front();
		_open.pop_front();
		cur_child = cur->GetChild();
		//深度不够的节点，拓展child节点
		if (cur->GetDepth() < depth) {
			if (cur_child.empty()) {
				cur->SetChild();
			}
			cur_child = cur->GetChild();
			for (auto it = cur_child.begin(); it != cur_child.end(); it++) {
				_open.push_back(*it);
			}
		}
	}
	
	//深度优先遍历博弈树
	int value = GetNodeValue(_start_node, depth, 0); //第一层不存在last_value
	return value;
}

pair<int, int> AiPlayer::GetNextPos()
{
	int value;
	for (int i = 1; i <= _max_depth; i++) {
		value = GetValueByDepth(i);
		if (value >= lev5 / 2) { //检索深度内可以获得胜利，不再向下检索
			break;
		}
	}
	vector<Nodeptr> child = _start_node->GetChild();
	vector<Nodeptr>::iterator it;
	for (it = child.begin(); it != child.end(); it++) {
		if ((*it)->GetValue() == value)
			break;
	}
	return (*it)->GetPos();
}

AiPlayer::AiPlayer(chessType type, int depth) :Player(type), _max_depth(depth)
{
}

void AiPlayer::SetDepth(int depth)
{
	_max_depth = depth;
}

bool AiPlayer::PutChess(ChessBoard& board)
{
	
	//第一手下中间
	int chess_num = board.GetChessNum();
	if (chess_num == 0) {
		board.SetChess(make_pair(6, 6), _type);
		return true;
	}
	//第二手下黑子旁
	pair<int, int> pos;
	if (chess_num == 1) {
		pos = board.FindChess(chessType(-_type));
		multimap<int, pair<int, int>> pos_list;
		pair<int, int> next_pos;
		pos_list.insert(make_pair((pow((6 - (pos.first - 1)), 2) + pow((6 - pos.second), 2)), make_pair(pos.first - 1, pos.second)));
		pos_list.insert(make_pair((pow((6 - (pos.first)), 2) + pow((6 - (pos.second - 1)), 2)), make_pair(pos.first, pos.second - 1)));
		pos_list.insert(make_pair((pow((6 - (pos.first + 1)), 2) + pow((6 - pos.second), 2)), make_pair(pos.first + 1, pos.second)));
		pos_list.insert(make_pair((pow((6 - (pos.first)), 2) + pow((6 - (pos.second + 1)), 2)), make_pair(pos.first, pos.second + 1)));
		next_pos = pos_list.begin()->second;
		board.SetChess(next_pos, _type);
		return true;
	}
	_start_node = Nodeptr(new Node(board, make_pair(-1, -1), _type, 0));
	pos = GetNextPos();
	if (board.SetChess(pos, _type)) {
		cout << "AI put the chess in: " << pos.first << "  " << pos.second << endl;
		return true;
	}
	cout << "AI index error! " << endl;
	return false;
}

bool HumanPlayer::PutChess(ChessBoard& board)
{
	cout << "please input the index of next chess: x y" << endl;
	int x = -1, y = -1;
	while (!board.IsFinish()) {
		cin >> x >> y;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(255, '\n');
		}
		if (board.SetChess(make_pair(x, y), _type)) {
			return true;
		}
		cout << "index error! please input again" << endl;
	}
	return false;
}

Game::Game()
{
	int type_num;
	int dif;
	while (1) {
		cout << "please choose you color: black or white(-1: black; 1: white; black first)" << endl;
		cin >> type_num;
		cin.clear();
		cin.ignore(255, '\n');
		if (type_num == -1 || type_num == 1)
			break;
	}
	while (1) {
		cout << "please choose difficulty : (1 or 2 or 3)" << endl;
		cin >> dif;
		cin.clear();
		cin.ignore(255, '\n');
		if (dif >= 1 && dif <= 3)
			break;
	}
	chessType humtype = chessType(type_num);
	_hum.SetType(humtype);
	_ai.SetDepth(dif);
	if (humtype == black)
		_ai.SetType(white);
	else
		_ai.SetType(black);
}

void Game::PlayGame()
{
	if (_hum.GetType() == black) {
		while (1) {
			_hum.PutChess(_b);
			//_b.ShowBoard();
			if (_b.IsFinish()) {
				_b.ShowBoard();
				break;
			}
			_ai.PutChess(_b);
			_b.ShowBoard();
			if (_b.IsFinish())
				break;
		}
	}
	else {
		while (1) {
			_ai.PutChess(_b);
			_b.ShowBoard();
			if (_b.IsFinish())
				break;
			_hum.PutChess(_b);
			//_b.ShowBoard();
			if (_b.IsFinish()) {
				_b.ShowBoard();
				break;
			}
		}
	}
	chessType winner = _b.GetWinner();
	cout << "the winner is " ;
	switch (winner) {
	case -1:
		cout << "black" << endl; break;
	case 1:
		cout << "white" << endl; break;
	case 0:
		cout << "null" << endl; break;
	}
}

//设置节点棋盘状态，节点深度，分别最大最小
Node::Node(const ChessBoard& board, pair<int, int> new_step, chessType type, int depth) :_b(board), _depth(depth)
{
	_type = type;
	_new_step = new_step;
	if (_depth % 2 == 0)
		_value = lev_min;
	else
		_value = lev_max;
}

void Node::SetDepth(int depth)
{
	_depth = depth;
}

int Node::GetDepth() const
{
	return _depth;
}

void Node::SetValue(int value)
{
	if (_depth % 2 == 0 && value > _value) {
		_value = value;
	}
	if (_depth % 2 != 0 && value < _value) {
		_value = value;
	}
}

int Node::GetValue() const
{
	return _value;
}

void Node::SetChild()
{
	int ldx = _b.GetLdx();
	int ldy = _b.GetLdy();
	int rux = _b.GetRux();
	int ruy = _b.GetRuy();

	for (int i = ldx; i <= rux; i++) {
		for (int j = ldy; j <= ruy; j++) {
			if (_b.HasChess(make_pair(i, j), 1)) {
				ChessBoard _new = _b;
				if (_new.SetChess(make_pair(i, j), _type)) {
					Nodeptr _newp(new Node(_new, make_pair(i, j), (chessType)(-_type), _depth + 1));
						_child.push_back(_newp);
				}
			}
		}
	}
	//ur_child = _child.begin();
}

void Node::ResetValue()
{
	if (_depth % 2 == 0)
		_value = lev_min;
	else
		_value = lev_max;
}

pair<int, int> Node::GetPos() const
{
	return _new_step;
}

//获取一个child并迭代器加一
vector<Nodeptr>& Node::GetChild()
{
	return _child;
}

//Nodeptr Node::GetParent() const
//{
//	return _par;
//}

ChessBoard Node::GetChessBoard() const
{
	return _b;
}

