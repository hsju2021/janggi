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
    void movePiece(int x, int y);
    virtual vector<pair<int, int>> generatePaths() = 0;
};

Piece* board[9][10] = {

};

void Piece::movePiece(int x, int y) {
    vector<pair<int, int>> paths = generatePaths();
    for (int i = 0; i < paths.size(); i++) {
        if (paths[i].first == x && paths[i].second == y) {
            board[x][y] = board[this->x][this->y];
            board[this->x][this->y] = nullptr;
            this->x = x;
            this->y = y;
            break;
        }
    }
}

void mainMenu();
void setupBoard(int han_setup, int cho_setup);
void printBoard();
Piece* choosePiece(int player);
bool isMovable(int x, int y, char team);
void kill();
bool choCheckWin();
void undo();


bool isMovable(int x, int y, char team) {
    if (board[x][y] == nullptr || board[x][y]->team == team) return false;
    else return true;
}

void kill() {

}

// derived class (Rook, Cannon, Knight, Elephant, King, Guard, Pawn)
class Rook : public Piece {
   public:
    Rook(int x, int y, char team) : Piece(x, y, team) {
        if (team == 'H') letter = 'R';
        else letter = 'r';
    }

    vector<pair<int, int>> generatePaths() override {
        vector<pair<int, int>> validMoves;
        int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
        for (int i = 0; i < 4; i++) {
            int x = this->x + directions[i][0];
            int y = this->y + directions[i][1];
            while (x >= 0 && x < 9 && y >= 0 && y < 10) {
                if (isMovable(x, y, this->team)) {
                    validMoves.push_back(make_pair(x, y));
                    break;
                } else {
                    validMoves.push_back(make_pair(x, y));
                    x += directions[i][0];
                    y += directions[i][1];
                }
            }
        }
        return validMoves;
    }

    void movePiece(int x, int y) {
        vector<pair<int, int>> paths = generatePaths();
        for (int i = 0; i < paths.size(); i++) {
            if (paths[i].first == x && paths[i].second == y) {
                board[x][y] = board[this->x][this->y];
                board[this->x][this->y] = nullptr;
                this->x = x;
                this->y = y;
                break;
            }
        }
    }
};

class Cannon : public Piece {
   public:
    Cannon(int x, int y, char team) : Piece(x, y, team) {
        if (team == 'H') letter = 'C';
        else letter = 'c';
    }

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

    vector<pair<int, int>> generatePaths() override {
        vector<pair<int, int>> validMoves;
        int directions[8][2] = {{0, 1}, {1, 0},  {0, -1}, {-1, 0},
                                {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
        for (int i = 0; i < 8; i++) {
            int x = this->x + directions[i][0];
            int y = this->y + directions[i][1];
            if (x >= 0 && x < 9 && y >= 0 && y < 10 &&
                !isMovable(x, y, this->team))
                validMoves.push_back(make_pair(x, y));
        }
        return validMoves;
    }
};

class Guard : public Piece {
   public:
    Guard(int x, int y, char team) : Piece(x, y, team) {
        if (team == 'H') letter = 'G';
        else letter = 'g';
    }

    vector<pair<int, int>> generatePaths() override {
        vector<pair<int, int>> validMoves;
        int directions[8][2] = {{0, 1}, {1, 0},  {0, -1}, {-1, 0},
                                {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
        for (int i = 0; i < 8; i++) {
            int x = this->x + directions[i][0];
            int y = this->y + directions[i][1];
            if (x >= 0 && x < 9 && y >= 0 && y < 10 &&
                !isMovable(x, y, this->team))
                validMoves.push_back(make_pair(x, y));
        }
        return validMoves;
    }
};

class Pawn : public Piece {
   public:
    Pawn(int x, int y, char team) : Piece(x, y, team) {
        if (team == 'H') letter = 'P';
        else letter = 'p';
    }

    vector<pair<int, int>> generatePaths() override {
        vector<pair<int, int>> validMoves;
        if (this->team == 'C') {
            if (this->x + 1 < 9 && board[this->x + 1][this->y]->team == 'H')
                validMoves.push_back(make_pair(this->x + 1, this->y));
            if (this->x - 1 >= 0 && board[this->x - 1][this->y]->team == 'H')
                validMoves.push_back(make_pair(this->x - 1, this->y));
            if (this->y + 1 < 10 && board[this->x][this->y + 1]->team == 'H')
                validMoves.push_back(make_pair(this->x, this->y + 1));
        } else {
            if (this->x + 1 < 9 && board[this->x + 1][this->y]->team == 'C')
                validMoves.push_back(make_pair(this->x + 1, this->y));
            if (this->x - 1 >= 0 && board[this->x - 1][this->y]->team == 'C')
                validMoves.push_back(make_pair(this->x - 1, this->y));
            if (this->y - 1 >= 0 && board[this->x][this->y - 1]->team == 'C')
                validMoves.push_back(make_pair(this->x, this->y - 1));
        }
        return validMoves;
    }

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

int main() {
    int num, x, y;
    Piece* chosen;

    mainMenu();
    setupInitialPieces();
    setupBoard(game, game.han);
    setupBoard(game, game.cho);

    if (num) { // 제거할 기물이 1개 이상 --> 한나라 선공
        while (true) {
            // 한나라 턴
            printBoard(); // 보드출력
            chosen = choosePiece(2); // 기물선택
            chosen->movePiece(x,y); // 기물이동
            printBoard(); // 이동후 보드출력

            if(choCheckWin()) break; // 승패여부 처리
            cout << endl; //턴 넘기기 메세지 처리 필요

            // 초나라 턴
            printBoard();
            chosen = choosePiece(1); 
            chosen->movePiece(x,y);
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
            chosen->movePiece(x,y); // 기물이동
            printBoard(); // 이동후 보드출력

            if(choCheckWin()) break; // 승패여부 처리
            cout << endl; //턴 넘기기 메세지 처리 필요

            // 초나라 턴
            printBoard();
            chosen = choosePiece(2); 
            chosen->movePiece(x,y);
            printBoard();

            if(choCheckWin()) break; // 승패여부 처리
            cout << endl; //턴 넘기기 메세지 처리 필요
        }
    }
    return 0;
}

void setupInitialPieces() {
    // 한나라의 기본 기물 배치
    board[0][0] = new Rook(0, 0, 'H');
	cout << "해당되는 포진이 없습니다.\n";
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

// 백창현 작성, 좌표 입력은 잘 되는데 board랑은 확인해봐야함
Piece* choosePiece(int player) { // player가 1이면 초, 2이면 한
    int tmpx, tmpy;
    string coord;

    while (true) {
        cout << msg[6] << endl << ">>>";
        getline(cin, coord);
        if (!coord.compare("quit")) {
            return nullptr;
        }

        if (!coord.compare("cancel")) {
            //무르기 처리
            undo();
        }

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

        if ((player == 2 && board[tmpx][tmpy]->team == 'C') || (player == 1 && board[tmpx][tmpy]->team == 'H')) {
            cout << msg[25] << msg[20] << endl;
            continue;
        }

        return board[tmpx][tmpy];
    }
}