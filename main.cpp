#include <iostream>
#include <stack>
#include <utility>
#include <vector>
#include <string>

using namespace std;

class Player {
   public:
    int score;
    int* pieces;
    string placement;
};

class Game {
   public:
    int turn;
    Player cho;
    Player han;
};

class Piece {
   public:
    int x, y;
    char team;
    char letter;

    Piece(int x, int y, char team) {
        this->x = x;
        this->y = y;
        this->team = team;
    }

    virtual void movePiece() = 0;
    virtual vector<pair<int, int>> generatePaths() = 0;
};

// derived class (Rook, Cannon, Knight, Elephant, King, Guard, Pawn)
class Rook : public Piece {
   public:
    Rook(int x, int y, char team) : Piece(x, y, team) {
        if (team == 'H') letter = 'R';
        else letter = 'r';
    }

    void movePiece() override {}
    vector<pair<int, int>> generatePaths() override {
        vector<pair<int, int>> paths;
        
        return paths;
    }
};

class Cannon : public Piece {
   public:
    Cannon(int x, int y, char team) : Piece(x, y, team) {
        if (team == 'H') letter = 'C';
        else letter = 'c';
    }

    void movePiece() override {}
    vector<pair<int, int>> generatePaths() override {
        vector<pair<int, int>> paths;
        return paths;
    }
};

class Knight : public Piece {
   public:
    Knight(int x, int y, char team) : Piece(x, y, team) {
        if (team == 'H') letter = 'N';
        else letter = 'n';
    }

    void movePiece() override {}
    vector<pair<int, int>> generatePaths() override {
        vector<pair<int, int>> paths;
        return paths;
    }
};

class Elephant : public Piece {
   public:
    Elephant(int x, int y, char team) : Piece(x, y, team) {
        if (team == 'H') letter = 'E';
        else letter = 'e';
    }

    void movePiece() override {}
    vector<pair<int, int>> generatePaths() override {
        vector<pair<int, int>> paths;
        return paths;
    }
};

class King : public Piece {
   public:
    King(int x, int y, char team) : Piece(x, y, team) {
        if (team == 'H') letter = 'K';
        else letter = 'k';
    }

    void movePiece() override {}
    vector<pair<int, int>> generatePaths() override {
        vector<pair<int, int>> paths;
        return paths;
    }
};

class Guard : public Piece {
   public:
    Guard(int x, int y, char team) : Piece(x, y, team) {
        if (team == 'H') letter = 'G';
        else letter = 'g';
    }

    void movePiece() override {}
    vector<pair<int, int>> generatePaths() override {
        vector<pair<int, int>> paths;
        return paths;
    }
};

class Pawn : public Piece {
   public:
    Pawn(int x, int y, char team) : Piece(x, y, team) {
        if (team == 'H') letter = 'P';
        else letter = 'p';
    }

    void movePiece() override {}
    vector<pair<int, int>> generatePaths() override {
        vector<pair<int, int>> paths;
        
        return paths;
    }
};





Piece* board[9][10] = {

};

stack<Piece*> previous;



string msg[] = {
    ">>> ",
    "메뉴를 선택하세요.\n",
    "원하는 포진을 선택하세요.\n",
    "{player}나라의 기물을 선택하세요.\n",
    "{player}나라가 게임을 중단하여 메인 메뉴로 돌아갑니다.\n",
    "‘y’또는 ‘Y’를 입력해주세요.\n",
    "{player}나라의 기물을 선택하세요.\n",
    "{player}나라 ‘{piece}’을(를) 선택하셨습니다.\n",
    "기물을 다시 선택하려면 “back”을 입력하세요.\n",
    "게임을 중단하려면 “quit”을 입력하세요.\n",
    "이동할 좌표를 입력해주세요.\n",
    "{lose_player}나라의 궁이 잡혔습니다.\n",
    "{win_player}나라의 승리입니다!\n",
    "‘y’또는 ‘Y’를 입력하면 메인 메뉴로 돌아갑니다.\n",
    "100번의 턴을 진행하여 점수 합산으로 승패를 결정합니다.\n",
    "양측 기물의 점수 총합이 30 이하이므로 점수 합산으로 승패를 결정합니다.\n",
    "점수 총합 : 초나라 {score}점, 한나라 {score}점\n",
    "한나라가 {setup} 포진을 선택하였습니다.\n",
    "{player}나라 ‘{piece}’이(가) {player}나라 ‘{piece}’을(를) 잡았습니다.\n ",
    "존재하지 않는 메뉴입니다. ",
    "다시 입력해 주세요.\n",
    "해당 좌표에 기물이 존재하지 않거나 옳지 않은 입력입니다. ",
    "불가능한 이동입니다. ",
    "해당되는 포진이 없습니다. 1과 4 사이의 정수를 입력하세요.\n"
};

string setup[] = {"1. 마상상마", "2. 마상마상", "3. 상마상마", "4. 상마마상"};

void mainMenu();
void setupBoard(int han_setup, int cho_setup);
void printBoard();
Piece* choosePiece(int player);
bool isMovable(int x, int y, char team);
void kill();
void choCheckWin();
void undo();


int main() {
    mainMenu();
    setupInitialPieces();
    setupBoard(game, game.han);
    setupBoard(game, game.cho);

    return 0;
}


void mainMenu() {
    // clear the console
    system("clear");
    
}

void setupInitialPieces() {
    // 한나라의 기본 기물 배치
    board[0][0] = new Rook(0, 0, 'H');
    board[8][0] = new Rook(8, 0, 'H');
    board[1][2] = new Cannon(1, 2, 'H');
    board[7][2] = new Cannon(7, 2, 'H');
    board[3][0] = new Guard(3, 0, 'H');
    board[5][0] = new Guard(5, 0, 'H');
    board[4][1] = new King(4, 1, 'H');
    for (int i = 0; i < 9; i += 2) {
        board[i][3] = new Pawn(i, 3, 'H');
    }

    // 초나라의 기본 기물 배치
    board[0][9] = new Rook(0, 9, 'C');
    board[8][9] = new Rook(8, 9, 'C');
    board[1][7] = new Cannon(1, 7, 'C');
    board[7][7] = new Cannon(7, 7, 'C');
    board[3][9] = new Guard(3, 9, 'C');
    board[5][9] = new Guard(5, 9, 'C');
    board[4][8] = new King(4, 8, 'C');
    for (int i = 0; i < 9; i += 2) {
        board[i][6] = new Pawn(i, 6, 'C');
    }
}

string setup[] = { "1. 마상상마", "2. 마상마상", "3. 상마상마", "4. 상마마상" };

void setupBoard(Game& game, Player& player) {

    if (&player == &game.han) {
        while (true) { // while문
            int num;
            cout << "    <한나라 포진>\n\n"
                << setup[0] << " 포진" << "\n    A B C D E F G H I" << "\n 0 |R|N|E|G| |G|E|N|R|\n\n"
                << setup[1] << " 포진" << "\n    A B C D E F G H I" << "\n 0 |R|N|E|G| |G|N|E|R|\n\n"
                << setup[2] << " 포진" << "\n    A B C D E F G H I" << "\n 0 |R|E|N|G| |G|E|N|R|\n\n"
                << setup[3] << " 포진" << "\n    A B C D E F G H I" << "\n 0 |R|E|N|G| |G|N|E|R|\n\n";
            cout << "원하는 포진을 입력하세요\n" << ">>>";
            cin >> num;
            if (num > 0 && num < 5) { // if문
                switch (num) { // switch문
                case 1:
                    cout << "한나라가 1. 마상상마 포진을 선택하였습니다.\n\n";
                    board[1][0] = new Knight(1, 0, 'H');
                    board[6][0] = new Knight(7, 0, 'H');
                    board[2][0] = new Elephant(2, 0, 'H');
                    board[7][0] = new Elephant(6, 0, 'H');
                    break;
                case 2:
                    cout << "한나라가 2. 마상마상 포진을 선택하였습니다.\n\n";
                    board[1][0] = new Knight(1, 0, 'H');
                    board[6][0] = new Knight(6, 0, 'H');
                    board[2][0] = new Elephant(2, 0, 'H');
                    board[7][0] = new Elephant(7, 0, 'H');
                    break;
                case 3:
                    cout << "한나라가 3. 상마상마 포진을 선택하였습니다.\n\n";
                    board[2][0] = new Knight(2, 0, 'H');
                    board[7][0] = new Knight(7, 0, 'H');
                    board[1][0] = new Elephant(1, 0, 'H');
                    board[6][0] = new Elephant(6, 0, 'H');
                    break;
                case 4:
                    cout << "한나라가 4. 상마마상 포진을 선택하였습니다.\n\n";
                    board[2][0] = new Knight(2, 0, 'H');
                    board[6][0] = new Knight(6, 0, 'H');
                    board[1][0] = new Elephant(1, 0, 'H');
                    board[7][0] = new Elephant(7, 0, 'H');
                    break;
                } // switch문
                break;
            } // if문
            else {
                cout << "해당되는 포진이 없습니다.\n";
            }
        } // while문
    }

    else if (&player == &game.cho) {
        while (true) { // while문
            int num;
            cout << "    <초나라 포진>\n\n"
                << setup[0] << " 포진" << "\n    A B C D E F G H I" << "\n 9 |R|N|E|G| |G|E|N|R|\n\n"
                << setup[1] << " 포진" << "\n    A B C D E F G H I" << "\n 9 |R|N|E|G| |G|N|E|R|\n\n"
                << setup[2] << " 포진" << "\n    A B C D E F G H I" << "\n 9 |R|E|N|G| |G|E|N|R|\n\n"
                << setup[3] << " 포진" << "\n    A B C D E F G H I" << "\n 9 |R|E|N|G| |G|N|E|R|\n\n";
            cout << "원하는 포진을 입력하세요\n" << ">>>";
            cin >> num;
            if (num > 0 && num < 5) { // if문
                switch (num) { // switch문
                case 1:
                    cout << "초나라가 1. 마상상마 포진을 선택하였습니다.\n\n";
                    board[1][9] = new Knight(1, 9, 'C');
                    board[7][9] = new Knight(7, 9, 'C');
                    board[2][9] = new Elephant(2, 9, 'C');
                    board[6][9] = new Elephant(6, 9, 'C');
                    break;
                case 2:
                    cout << "초나라가 2. 마상마마 포진을 선택하였습니다.\n\n";
                    board[1][9] = new Knight(1, 9, 'C');
                    board[6][9] = new Knight(6, 9, 'C');
                    board[2][9] = new Elephant(2, 9, 'C');
                    board[7][9] = new Elephant(7, 9, 'C');
                    break;
                case 3:
                    cout << "초나라가 3. 상마상마 포진을 선택하였습니다.\n\n";
                    board[2][9] = new Knight(2, 9, 'C');
                    board[7][9] = new Knight(7, 9, 'C');
                    board[1][9] = new Elephant(1, 9, 'C');
                    board[6][9] = new Elephant(6, 9, 'C');
                    break;
                case 4:
                    cout << "초나라가 4. 상마마상 포진을 선택하였습니다.\n\n";
                    board[2][9] = new Knight(2, 9, 'C');
                    board[6][9] = new Knight(6, 9, 'C');
                    board[1][9] = new Elephant(1, 9, 'C');
                    board[7][9] = new Elephant(7, 9, 'C');
                    break;
                } // switch문
                break;
            } // if문
            else {
                cout << "해당되는 포진이 없습니다.\n";
            }
        } // while문
    }
}