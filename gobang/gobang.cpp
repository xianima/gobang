// gobang.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#include "gobangclass.h"

int main()
{
    //ChessBoard board;
    //board.SetChess(make_pair(2, 2), black);
    //board.SetChess(make_pair(3, 2), white);
    //board.SetChess(make_pair(3, 3), black);
    //board.SetChess(make_pair(2, 1), white);
    //board.SetChess(make_pair(1, 1), black);
    //board.ShowBoard();
    //cout<<board.Evaluate(black);
    Game game;
    game.PlayGame();

    system("pause");
    return 0;
}
