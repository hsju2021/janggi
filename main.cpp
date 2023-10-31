#include <iostream>
#include <stack>
#include <utility>
#include <vector>
#include <string>

#define BOARD_WIDTH 9
#define BOARD_HEIGHT 10

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

Game game;

void mainMenu();
void setupBoard(Game& game, Player& player);
void printBoard();
Piece* choosePiece(Player& player);
int isMovable(int x, int y, char team);
void kill();
bool choCheckWin();
void undo();
void setupInitialPieces();
int remove_piece_num();
void remove_select_piece(int num);

int isMovable(int x, int y, char team) {
    if (board[x][y] == nullptr) return 0;
    else if (board[x][y]->team != team)
        return 1;
    else return 2;
}

void kill() {

}

int remove_piece_num() {

}

void remove_select_piece(int num){

}

// derived class (Rook, Cannon, Knight, Elephant, King, Guard, Pawn)
class Rook : public Piece {
   public:
    Rook(int x, int y, char team) : Piece(x, y, team) {
        if (team == 'H') letter = 'R';
        else letter = 'r';
    }
    // 가능한 모든 경로를 생성하는 함수
    vector<pair<int, int>> generatePaths() override {
        vector<pair<int, int>> validMoves; // 반환할 경로 저장
        int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
        // 움직일 수 있는 4방향
        for (int i = 0; i < sizeof(directions) / sizeof(directions[0]); i++) {
            int x = this->x + directions[i][0];
            int y = this->y + directions[i][1];

            while (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT) { // 보드를 벗어나기 전까지 확장
                int movableFlag = isMovable(x, y, this->team);

                if (movableFlag == 0) { // isMovable로 움직일 수 있는 칸인지 체크
                    validMoves.push_back(make_pair(x, y)); // 빈 칸일 경우 validMoves에 추가 후 계속 진행
                    x += directions[i][0];
                    y += directions[i][1];
                } else if (movableFlag == 1) { // 적 기물을 만났을 경우 validMoves에 추가하고 탐색 종료
                    validMoves.push_back(make_pair(x, y));
                    break;
                } else // 아군 기물을 만났을 경우 탐색 종료
                    break;
            }
        }
        return validMoves;
    }
};

class Cannon : public Piece {
   public:
    Cannon(int x, int y, char team) : Piece(x, y, team) {
        if (team == 'H') letter = 'C';
        else letter = 'c';
    }

    // 가능한 모든 경로를 생성하는 함수
    vector<pair<int, int>> generatePaths() override { // 김종우 수정 사항
        vector<pair<int, int>> validMoves; // 반환할 경로 저장
        int directions[4][2] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };
        // 움직일 수 있는 4방향
        for (int i = 0; i < sizeof(directions) / sizeof(directions[0]); i++) {
            int x = this->x + directions[i][0];
            int y = this->y + directions[i][1];

            int canJump = 0; // 해당 방향에서 넘을 기물이 있는지 체크
            while (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT) { // 보드를 벗어나기 전까지 확장 (한쪽 방향으로)
                int movableFlag = isMovable(x, y, this->team);
                if (movableFlag == 0) { // isMoveable로 이동 예정 칸의 상태 체크 (빈칸, 상대기물, 내 기물 3가지 경우로 나누어 수행)
                    if (canJump == 1) {
                        validMoves.push_back(make_pair(x, y)); // 빈 칸이고 넘을 기물이 있을 경우 경로에 저장
                        x += directions[i][0];
                        y += directions[i][1];
                    }
                    // 빈칸인데 넘을 기물이 없다면 다음 탐색 진행
                }
                else if (movableFlag == 1) {
                    if (canJump == 0) { // 이전까지 넘을 기물 없었음
                        canJump = 1; // 해당 방향에 처음으로 넘을 적 기물이 나타남
                    }
                    else {
                        validMoves.push_back(make_pair(x, y)); // canJump == 1이고 (넘을 기물이 있고) 상대방 기물을 만나게 되면 해당 경로 저장
                        break; // 적 기물을 잡을 수 있는 경우이므로 탐색 종료
                    }
                }
                else {
                    if (canJump == 0) {
                        canJump = 1; // 해당 방향에서 처음으로 넘을 내 기물이 나타남
                    }
                    else
                        break; // 넘을 수 있는데 내 기물이 나타나면 해당 방향으로 더이상 이동 불가능 - 탐색 종료
                }

            }
        }
        
        int starpointsHan[4][2] = { {3, 0}, {5, 0}, {5, 2}, {3, 2} };   // 한나라 궁성 모서리 좌표 저장
        int starpointsCho[4][2] = { {3, 7}, {5, 7}, {5, 9}, {3, 9} };   // 한나라 궁성 모서리 좌표 저장

        for (int i = 0; i < 4; i++) {
            if ((this->x == starpointsHan[i][0]) && (this->y == starpointsHan[i][1])) {
                if (board[4][1] != nullptr) {
                    int x = starpointHan[((i + 2) % 4)][0];
                    int y = starpointHan[((i + 2) % 4)][1];
                    if (isMovable(x, y, this->team) == 2);
                    else {
                        validMoves.push_back(make_pair(x, y)); // 궁성 가운데 기물이 있을 때 반대쪽 방향 저장
                    }
                }
            }
        }
        for (int i = 0; i < 4; i++) {
            if ((this->x == starpointsCho[i][0]) && (this->y == starpointsCho[i][1])) {
                if (board[4][8] != nullptr) {
                    int x = starpointHan[((i + 2) % 4)][0];
                    int y = starpointHan[((i + 2) % 4)][1];
                    if (isMovable(x, y, this->team) == 2);
                    else {
                        validMoves.push_back(make_pair(x, y)); // 궁성 가운데 기물이 있을 때 반대쪽 방향 저장
                    }
                }
            }
        }

        return validMoves;
    }
};

class Knight : public Piece {
   public:
    Knight(int x, int y, char team) : Piece(x, y, team) {
        if (team == 'H') letter = 'N';
        else letter = 'n';
    }

    vector<pair<int, int>> generatePaths() override { // 김종우 수정 사항
        vector<pair<int, int>> validMoves;
        int moves[8][2] = { {1, 2}, {1, -2}, {-1, 2}, {-1, -2},     // knight가 최종적으로 이동하는 위치 8개 (최종 좌표)
                           {2, 1}, {2, -1}, {-2, 1}, {-2, -1} };
        int blockcheck[8][2] = { {0, 1}, {0, -1}, {0, 1},  {0, -1}, // knight의 최종 좌표 별 지나가는 좌표 8개
                                {1, 0}, {1, 0},  {-1, 0}, {-1, 0} };
        for (int i = 0; i < 8; i++) {   // 8개 좌표 모두 검사
            int x = this->x + blockcheck[i][0]; // 지나가는 좌표 x, y 값 저장
            int y = this->y + blockcheck[i][1];
            if (x >= 0 && x < 9 && y >= 0 && y < 10) {  // 지나가는 좌표가 판을 벗어나는지 검사
                if (isMovable(x, y, this->team) == 0) { // 지나가는 좌표에 기물이 없어야 다음 단계로 진행
                    x = this->x + moves[i][0];  // 최종 좌표 x, y 값 저장           
                    y = this->y + moves[i][1];
                    if (x >= 0 && x < 9 && y >= 0 && y < 10) {  // 최종 좌표가 판을 벗어나는지 검사
                        if (isMovable(x, y, this->team) == 2);  // 최종 좌표에 나의 기물이 있다면 다음 탐색 진행
                        else {
                            validMoves.push_back(make_pair(x, y)); // 빈 좌표나 적 기물이 있다면 경로에 저장
                        }
                    }
                }
            }
        }
        return validMoves;
    }
};

class Elephant : public Piece {
   public:
    Elephant(int x, int y, char team) : Piece(x, y, team) {
        if (team == 'H') letter = 'E';
        else letter = 'e';
    }

    vector<pair<int, int>> generatePaths() override { // 김종우 수정 사항
        vector<pair<int, int>> validMoves;
        int moves[8][2] = { {2, 3}, {2, -3}, {-2, 3}, {-2, -3},         // Elephant가 최종적으로 이동하는 위치 8개 (최종 좌표)
                           {3, 2}, {3, -2}, {-3, 2}, {-3, -2} };
        int blockcheck1[8][2] = { {0, 1}, {0, -1}, {0, 1},  {0, -1},    // Elephant의 최종 좌표 별 첫번째로 지나가는 좌표 8개
                                 {1, 0}, {1, 0},  {-1, 0}, {-1, 0} };
        int blockcheck2[8][2] = { {1, 2}, {1, -2}, {-1, 2}, {-1, -2},   // Elephant의 최종 좌표 별 두번째로 지나가는 좌표 8개
                                 {2, 1}, {2, -1}, {-2, 1}, {-2, -1} };
        for (int i = 0; i < 8; i++) {
            int x = this->x + blockcheck1[i][0];    // 첫번째로 지나가는 좌표 x, y값 저장
            int y = this->y + blockcheck1[i][1];
            if (x >= 0 && x < 9 && y >= 0 && y < 10 && !isMovable(x, y, this->team)) {  // 첫번째 좌표가 판을 벗어나는지, 해당 좌표가 비었는지 검사
                x = this->x + blockcheck2[i][0];    // 두번째로 지나가는 좌표 x, y값 저장
                y = this->y + blockcheck2[i][1];
                if (x >= 0 && x < 9 && y >= 0 && y < 10 && !isMovable(x, y, this->team)) {  // 두번째 좌표가 판을 벗어나는지, 해당 좌표가 비었는지 검사
                    x = this->x + moves[i][0];  // 최종 좌표 x, y 값 저장 
                    y = this->y + moves[i][1];
                    if (x >= 0 && x < 9 && y >= 0 && y < 10) { // 최종 좌표가 판을 벗어나는지 검사
                        if (isMovable(x, y, this->team) == 2);  // 최종 좌표에 나의 기물이 있다면 다음 탐색 진행
                        else
                            validMoves.push_back(make_pair(x, y)); // 최종 좌표가 비었거나 상대 기물이 있다면 경로 저장
                    }
                }
            }
        }
        return validMoves;
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
            int movableFlag = isMovable(x, y, team);

            if (this->team == 'H') {
                if (x >= 3 && x <= 5 && y >= 8 && y <= 10) {
                    if (movableFlag == 0 || movableFlag == 1) {
                        validMoves.push_back(make_pair(x, y));
                    }
                }
            } else {
                if (x >= 3 && x <= 5 && y >= 0 && y <= 2) {
                    if (movableFlag == 0 || movableFlag == 1) {
                        validMoves.push_back(make_pair(x, y));
                    }
                }
            }
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
            int movableFlag = isMovable(x, y, team);

            if (this->team == 'H') {
                if (x >= 3 && x <= 5 && y >= 8 && y <= 10) {
                    if (movableFlag == 0 || movableFlag == 1) {
                        validMoves.push_back(make_pair(x, y));
                    }
                }
            } else {
                if (x >= 3 && x <= 5 && y >= 0 && y <= 2) {
                    if (movableFlag == 0 || movableFlag == 1) {
                        validMoves.push_back(make_pair(x, y));
                    }
                }
            }
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
        if (this->team == 'H') {
            
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
    "해당되는 포진이 없습니다. 1과 4 사이의 정수를 입력하세요.\n",
    "잘못된 입력입니다. ‘y’또는 ‘Y’를 입력하세요.\n",
    "상대 플레이어의 기물은 선택할 수 없습니다. ",
    "초나라는 한나라에서 제거할 기물의 수를 입력하세요. (0~6)\n",
    "{num}()를 입력받았습니다.\n",
    "{player}는 포진 선택과정을 진행하세요.\n",
    "한나라는 제거할 {num}()개의 좌표를 입력하세요.\n",
    "접장기 규칙에 따라 한나라의 선공으로 진행합니다.\n"
};

string setup[] = {"1. 마상상마", "2. 마상마상", "3. 상마상마", "4. 상마마상"};

int main() {
    int remove, x, y;
    Piece* chosen;

    mainMenu();
    setupInitialPieces();
    remove = remove_piece_num();
    if (remove) { // 제거할 기물이 1개 이상
        // 한나라, 초나라 포진 과정
        setupBoard(game, game.han);
        remove_select_piece(remove);
        setupBoard(game, game.cho);

        while (true) {
            // 한나라 턴
            printBoard(); // 보드출력
            chosen = choosePiece(game.han); // 기물선택
            chosen->movePiece(x,y); // 기물이동
            printBoard(); // 이동후 보드출력

            if(choCheckWin()) break; // 승패여부 처리
            cout << endl; //턴 넘기기 메세지 처리 필요

            // 초나라 턴
            printBoard();
            chosen = choosePiece(game.cho); 
            chosen->movePiece(x,y);
            printBoard();

            if(choCheckWin()) break; // 승패여부 처리
            cout << endl; //턴 넘기기 메세지 처리 필요
        }
    }
    else { // 제거할 기물이 0개
        setupBoard(game, game.han);
        setupBoard(game, game.cho);
        while (true) {
            // 초나라 턴
            printBoard(); // 보드출력
            chosen = choosePiece(game.han); // 기물선택
            chosen->movePiece(x,y); // 기물이동
            printBoard(); // 이동후 보드출력

            if(choCheckWin()) break; // 승패여부 처리
            cout << endl; //턴 넘기기 메세지 처리 필요

            // 한나라 턴
            printBoard();
            chosen = choosePiece(game.cho); 
            chosen->movePiece(x,y);
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
                    cout << "초나라가 2. 마상마상 포진을 선택하였습니다.\n\n";
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
Piece* choosePiece(Player& player) {
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

        if ((&player == &game.han && board[tmpx][tmpy]->team == 'C') || (&player == &game.cho && board[tmpx][tmpy]->team == 'H')) {
            cout << msg[25] << msg[20] << endl;
            continue;
        }

        return board[tmpx][tmpy];
    }
}

// 김종우 작성 - 보드 출력 
void printBoard() {
    int starpoints[10][2] = { {3, 0}, {5, 0}, {4, 1}, {3, 2}, {5, 2},   // 궁성 좌표 저장
                            {3, 7}, {5, 7}, {4, 8}, {3, 9}, {5, 9} };
    system("clear");    // 프롬프트 clear
    cout << "   A B C D E F G H I   turn : " << game.turn << endl;  // 가장 윗줄 출력 
    for (int row = 0; row < 10; row++) {
        cout << " " << row << " |"; // 세로 숫자 줄 출력 + "|"
        for (int col = 0; col < 9; col++) { // 보드 출력 과정
            if (board[col][row] != nullptr) {
                cout << board[col][row]->letter << "|"; // Piece 있으면 letter 출력 + "|"
            }
            else {
                int isStarpoint = 0; // 궁성인지 아닌지 저장 (궁성 아닐 때 빈 자리 출력하기 위해 사용)
                for (int i = 0; i < 10; i++) {
                    if ((col == starpoints[i][0]) && (row == starpoints[i][1])) {   // 궁성의 10가지 경우의 수와 비교
                        cout << "*|";
                        isStarpoint = 1;
                        break;
                    }
                }
                if (isStarpoint == 0)
                    cout << " |";   // 궁성 아닌 빈자리 <공백문자> + "|" 출력
            }
        }
        if (row == 0) { cout << "한나라 score : " << game.han.score; } // 첫째 줄에 한나라 score 출력
        else if (row == 1) { cout << "초나라 score : " << game.cho.score; } // 둘째 줄에 초나라 score 출력
        cout << endl;
    }
}