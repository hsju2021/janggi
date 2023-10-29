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
bool choCheckWin();
void undo();


int main() {
    int num, han_setup, cho_setup;
    Piece* chosen;

    mainMenu();
    //remove_piece_num();
    //setupBoard(han_setup, cho_setup); 수정필요
    //remove_select_piece(num);
    if (num) { // 제거할 기물이 1개 이상 --> 한나라 선공
        while (true) {
            // 한나라 턴
            printBoard(); // 보드출력
            chosen = choosePiece(2); // 기물선택
            chosen->movePiece(); // 기물이동
            printBoard(); // 이동후 보드출력

            if(choCheckWin()) break; // 승패여부 처리
            cout << endl; //턴 넘기기 메세지 처리 필요

            // 초나라 턴
            printBoard();
            chosen = choosePiece(1); 
            chosen->movePiece();
            printBoard();

            if(choCheckWin()) break; // 승패여부 처리
            cout << endl; //턴 넘기기 메세지 처리 필요
        }
    }
    else { // 제거할 기물이 0개
        while (true) {
            // 한나라 턴
            printBoard(); // 보드출력
            chosen = choosePiece(1); // 기물선택
            chosen->movePiece(); // 기물이동
            printBoard(); // 이동후 보드출력

            if(choCheckWin()) break; // 승패여부 처리
            cout << endl; //턴 넘기기 메세지 처리 필요

            // 초나라 턴
            printBoard();
            chosen = choosePiece(2); 
            chosen->movePiece();
            printBoard();

            if(choCheckWin()) break; // 승패여부 처리
            cout << endl; //턴 넘기기 메세지 처리 필요
        }
    }
    return 0;
}


void mainMenu() {
    // clear the console
    system("clear");
    
}


// 백창현 작성, 좌표 입력은 잘 되는데 board랑은 확인해봐야함
Piece* choosePiece(int player) { // player가 1이면 초, 2이면 한
    int tmpx, tmpy;
    string coord;

    while (true) {
        cout << msg[6] << endl << ">>>";
        getline(cin, coord);

        // 좌표 입력 규칙 확인 (2글자이고, 첫번째는 숫자이고, 두번째는 소문자 혹은 대문자인지)
        if (coord.length() !=2 || !isdigit(coord[0]) || (!(coord[1] >= 'a' && coord[1] <= 'i') && !(coord[1] >= 'A' && coord[1] <= 'I')))  {
            cout << msg[21] << msg[20] << endl;
            continue;
        }

        tmpx = coord[0] - '0';
        tmpy = (coord[1] >= 'a' && coord[1] <= 'i') ? coord[1] - 'a' : coord[1] - 'A';

        if (board[tmpx][tmpy] == nullptr) {
            cout << msg[21] << msg[20] << endl;
            continue;
        }

        if ((player == 2 && board[tmpx][tmpy]->team == '초') || (player == 1 && board[tmpx][tmpy]->team == '한')) {
            cout << msg[25] << msg[20] << endl;
            continue;
        }

        return board[tmpx][tmpy];
    }
}