#include <iostream>
#include <stack>
#include <utility>
#include <vector>
#include <string>
#include <sstream>
#include <stdlib.h>
// #include <windows.h>
#include <algorithm>
#include <map>
#include <cstring>

#define BOARD_WIDTH 9
#define BOARD_HEIGHT 10
#define MAX_TURN 100

using namespace std;

string msg[] = {
    ">>> ",
    "메뉴를 선택하세요.\n",
    "1. 게임 실행\n",
    "2. 게임 종료\n",
    "원하는 포진을 선택하세요.\n",
    "{player}나라의 기물을 선택하세요.\n",
    "{player}나라가 게임을 중단하여 메인 메뉴로 돌아갑니다.\n",
    "‘y’또는 ‘Y’를 입력해주세요.\n",
    "{player}나라 ‘{piece}’을(를) 선택하셨습니다.\n",
    "잘못된 좌표입니다. ",
    "기물을 다시 선택하려면 “back”을 입력하세요.\n",
    "게임을 중단하려면 “quit”을 입력하세요.\n",
    "이동할 좌표를 입력해주세요.\n",
    "{lose_player}나라의 궁이 잡혔습니다.\n",
    "{win_player}나라의 승리입니다!\n",
    "‘y’또는 ‘Y’를 입력하면 메인 메뉴로 돌아갑니다.\n",
    "100번의 턴을 진행하여 점수 합산으로 승패를 결정합니다.\n",
    "양측 기물의 점수 총합이 30 이하이므로 점수 합산으로 승패를 결정합니다.\n",
    "점수 총합 : 초나라 {cho_score}점, 한나라 {han_score}점\n",
    "{player}나라가 {setup} 포진을 선택하였습니다.\n",
    "{player1}나라 ‘{piece1}’이(가) {player2}나라 ‘{piece2}’을(를) 잡았습니다.\n ",
    "존재하지 않는 메뉴입니다. ",
    "다시 입력해 주세요.\n",
    "해당 좌표에 기물이 존재하지 않거나 옳지 않은 입력입니다. ",
    "불가능한 이동입니다. ",
    "해당되는 포진이 없습니다. 1과 4 사이의 정수를 입력하세요.\n",
    "잘못된 입력입니다. ‘y’또는 ‘Y’를 입력하세요.\n",
    "상대 플레이어의 기물은 선택할 수 없습니다. ",
    "초나라는 한나라에서 제거할 기물의 수를 입력하세요. (0~6)\n",
    "{num}({})를 입력받았습니다.\n",
    "{player}나라는 포진 선택과정을 진행하세요.\n",
    "한나라는 제거할 {num}()개의 좌표를 입력하세요.\n",
    "접장기 규칙에 따라 한나라의 선공으로 진행합니다.\n",
    "무르기를 요청하려면 “cancel”을 입력하세요.\n",
    "{player}나라가 무르기 요청을 하였습니다.\n수락하려면 ‘y’또는 ‘Y’를 입력하세요.\n거절하려면 ‘n’또는 ‘N’를 입력하세요.\n"
};

string input;

class Player {
   public:
    double score;
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
    double score;
    Piece(int x, int y, char team) {
        this->x = x;
        this->y = y;
        this->team = team;
    }
    string team2string() {
        if (this->team == 'H') return "한나라";
        else return "초나라";
    }
    string letter2string() {
        if (this->letter == 'R' || this->letter == 'r') return "차";
        else if (this->letter == 'C' || this->letter == 'c') return "포";
        else if (this->letter == 'N' || this->letter == 'n') return "마";
        else if (this->letter == 'E' || this->letter == 'e') return "상";
        else if (this->letter == 'K' || this->letter == 'k') return "왕";
        else if (this->letter == 'G' || this->letter == 'g') return "사";
        else if (this->letter == 'P' || this->letter == 'p') return "졸";
        else return "잘못된 기물";
    }

    int movePiece();
    virtual vector<pair<int, int>> generatePaths() = 0;

    virtual ~Piece() = default;
    
};

Piece* King_Location;
class BoardState {
public:
    Piece* state[9][10];

    BoardState(Piece* initialBoard[9][10]) {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 10; ++j) {
                state[i][j] = initialBoard[i][j];
            }
        }
    }
};

Piece* board[9][10] = {

};


int Piece::movePiece() {
    vector<pair<int, int>> paths = generatePaths();
    int tmpx, tmpy;
    string coord;

    while (1) {
        // print paths
        cout << "가능한 경로 : ";
        for (int i = 0; i < paths.size(); i++) {
            cout << paths[i].second << (char)(paths[i].first + 'A') << ' ';
        }
        cout << endl;
        cout << "게임을 종료하려면 quit을 입력하세요." << endl;
        cout << "다른 기물을 선택하려면 back을 입력하세요." << endl;
        cout << msg[12] << msg[0];
        getline(cin, coord);

        if (!coord.compare("quit")) {
            return 1;
        }

        if (!coord.compare("back")) {
            return 2;
        }

        // 좌표 입력 규칙 확인 (2글자이고, 첫번째는 숫자이고, 두번째는 소문자 혹은 대문자인지)
        if (coord.length() !=2 || !isdigit(coord[0]) || (!(coord[1] >= 'a' && coord[1] <= 'i') && !(coord[1] >= 'A' && coord[1] <= 'I')))  {
            cout << msg[24] << msg[22] << endl;
            continue;
        }

        // 규칙에 맞을 경우 숫자로 변환
        tmpy = coord[0] - '0';
        tmpx = (coord[1] >= 'a' && coord[1] <= 'i') ? coord[1] - 'a' : coord[1] - 'A';

        for (int i = 0; i < paths.size(); i++) {
            if (paths[i].first == tmpx && paths[i].second == tmpy) {
                // 기물 잡았을 경우 메시지 출력
                // ex) 초나라 '졸'이 한나라 '차'를 잡았습니다.
                if (board[tmpx][tmpy] != nullptr) {
                    cout << this->team2string() << " '" << this->letter2string() << "'이(가) " << board[tmpx][tmpy]->team2string() << " '" << board[tmpx][tmpy]->letter2string() << "'을(를) 잡았습니다." << endl;
                    //Sleep(2000);
                }

                board[tmpx][tmpy] = board[x][y];
                board[x][y] = nullptr;
                x = tmpx;
                y = tmpy;
                return 0;
            } 
        }
        cout << msg[24] << msg[22] << endl;
    }
}
int gamestart;
Game game;

void mainMenu();
void gameplay(int remove, Piece* chosen, int quitOnMove);
void turnhan(Piece* chosen, int quitOnMove);
void turncho(Piece* chosen, int quitOnMove);
void setupBoard(Game& game, Player& player);
void printBoard();
Piece* choosePiece(Player& player);
int isMovable(int x, int y, char team);
void kill();
bool choCheckWin();
bool choKingDie(Piece* kingLocation);
bool isScoreUnder(double score1, double score2);
bool isTurnOver(int turn);
bool isKingDie();
void undo();
void setup_score();
void setupInitialPieces(Game& game, Player& player);
int remove_piece_num();
void remove_select_piece(int num);
string format(const string& input, const map<string, string>& to);

int isMovable(int x, int y, char team) {
    if (board[x][y] == nullptr) return 0;
    else if (board[x][y]->team != team)
        return 1;
    else return 2;
}

void kill() {

}


// derived class (Rook, Cannon, Knight, Elephant, King, Guard, Pawn)
class Rook : public Piece {
   public:
    Rook(int x, int y, char team) : Piece(x, y, team) {
        if (team == 'H') letter = 'R';
        else letter = 'r';
        score = 13;
    }
    // 가능한 모든 경로를 생성하는 함수
    vector<pair<int, int>> generatePaths() override {
        vector<pair<int, int>> validMoves; // 반환할 경로 저장
        int directions[4][2] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };
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
                }
                else if (movableFlag == 1) { // 적 기물을 만났을 경우 validMoves에 추가하고 탐색 종료
                    validMoves.push_back(make_pair(x, y));
                    break;
                }
                else // 아군 기물을 만났을 경우 탐색 종료
                    break;
            }
        }
        // 궁성 내부 이동 - 김종우 추가
        int starpointsHan[4][2] = { {3, 0}, {5, 0}, {5, 2}, {3, 2} };
        int starpointsCho[4][2] = { {3, 7}, {5, 7}, {5, 9}, {3, 9} };
        if ((this->x == 4) && (this->y == 1)) { // 차가 가운데 있을 때 (4, 1)
            for (int i = 0; i < 4; i++) {   // 모서리 이동 가능 확인
                int x = starpointsHan[i][0];
                int y = starpointsHan[i][1];
                if (isMovable(x, y, this->team) == 2);
                else
                    validMoves.push_back(make_pair(x, y));
            }
        }
        else if ((this->x == 4) && (this->y == 8)) {    // 차가 가운데 있을 때 (4, 8)
            for (int i = 0; i < 4; i++) {   // 모서리 이동 가능 확인
                int x = starpointsCho[i][0];
                int y = starpointsCho[i][1];
                if (isMovable(x, y, this->team) == 2);
                else
                    validMoves.push_back(make_pair(x, y));
            }
        }
        else {  // 모서리에 차가 있는지 확인
            if (y <= 2) {   // 한나라
                for (int i = 0; i < 4; i++) {
                    int x = starpointsHan[i][0];
                    int y = starpointsHan[i][1];
                    if ((this->x == x) && (this->y == y)) { // 모서리에 있는지 확인
                        if (isMovable(4, 1, this->team) == 0) { // 가운데 비었을 때
                            validMoves.push_back(make_pair(4, 1));
                            x = starpointsHan[(i + 2) % 4][0];
                            y = starpointsHan[(i + 2) % 4][1];
                            if (isMovable(x, y, this->team) == 2) { // 반대편 확인
                                break;
                            }
                            else {
                                validMoves.push_back(make_pair(x, y));
                                break;
                            }
                        }
                        else if (isMovable(4, 1, this->team) == 1) {    // 가운데 적 기물있을 때
                            validMoves.push_back(make_pair(4, 1));
                            break;
                        }
                        else {  // 가운데 내 기물있을 때
                            break;
                        }
                    }
                }
            }
            else if (y >= 7) {
                for (int i = 0; i < 4; i++) {
                    int x = starpointsCho[i][0];
                    int y = starpointsCho[i][1];
                    if ((this->x == x) && (this->y == y)) {
                        if (isMovable(4, 8, this->team) == 0) { // 가운데 비었을 때
                            validMoves.push_back(make_pair(4, 8));
                            x = starpointsHan[(i + 2) % 4][0];
                            y = starpointsHan[(i + 2) % 4][1];
                            if (isMovable(x, y, this->team) == 2) { // 반대편 확인
                                break;
                            }
                            else {
                                validMoves.push_back(make_pair(x, y));
                                break;
                            }
                        }
                        else if (isMovable(4, 8, this->team) == 1) {    // 가운데 적 기물있을 때
                            validMoves.push_back(make_pair(4, 8));
                            break;
                        }
                        else {  // 가운데 내 기물있을 때
                            break;
                        }
                    }
                }
            }
            else {}
        }
        return validMoves;
    }
};

class Cannon : public Piece {
   public:
    Cannon(int x, int y, char team) : Piece(x, y, team) {
        if (team == 'H') letter = 'C';
        else letter = 'c';
        score = 7;
    }

    // 해당 기물이 Cannon인지 확인하는 함수 추가
    bool isCannon(Piece& cannon) {
        if ((cannon.letter == 'C') || (cannon.letter == 'c')) {
            return true;
        }
        else return false;
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
                    else {  // 빈칸인데 넘을 기물이 없다면 다음 탐색 진행
                        x += directions[i][0];
                        y += directions[i][1];
                    }
                }
                else if (movableFlag == 1) {
                    if (isCannon(*board[x][y])) {
                        break;
                    }
                    else if (canJump == 0) { // 이전까지 넘을 기물 없었음
                        canJump = 1; // 해당 방향에 처음으로 넘을 적 기물이 나타남
                        x += directions[i][0];
                        y += directions[i][1];
                    }
                    else {
                        validMoves.push_back(make_pair(x, y)); // canJump == 1이고 (넘을 기물이 있고) 상대방 기물을 만나게 되면 해당 경로 저장
                        break; // 적 기물을 잡을 수 있는 경우이므로 탐색 종료
                    }
                }
                else {
                    if (isCannon(*board[x][y])) {
                        break;
                    }
                    else if (canJump == 0) {
                        canJump = 1; // 해당 방향에서 처음으로 넘을 내 기물이 나타남
                        x += directions[i][0];
                        y += directions[i][1];
                    }
                    else
                        break; // 넘을 수 있는데 내 기물이 나타나면 해당 방향으로 더이상 이동 불가능 - 탐색 종료
                }

            }
        }

        int starpointsHan[4][2] = { {3, 0}, {5, 0}, {5, 2}, {3, 2} };   // 한나라 궁성 모서리 좌표 저장
        int starpointsCho[4][2] = { {3, 7}, {5, 7}, {5, 9}, {3, 9} };   // 한나라 궁성 모서리 좌표 저장

        for (int i = 0; i < 4; i++) {
            if ((this->x == starpointsHan[i][0]) && (this->y == starpointsHan[i][1])) { // 모서리 좌표 확인
                if (board[4][1] != nullptr) {   // 가운데 기물 있는지 확인
                    if (isCannon(*board[4][1])) {   // 가운데 기물이 cannon인지 확인
                        break;
                    }
                    int x = starpointsHan[((i + 2) % 4)][0];
                    int y = starpointsHan[((i + 2) % 4)][1];
                    if (isMovable(x, y, this->team) == 0) { // jump했을 때 이동 가능한지 확인
                        validMoves.push_back(make_pair(x, y));
                        break;
                    }
                    else if (isMovable(x, y, this->team) == 1) {
                        if (isCannon(*board[x][y])) {
                            break;
                        }
                        validMoves.push_back(make_pair(x, y));
                        break;
                    }
                    else break;
                }
                else break;
            }
        }
        for (int i = 0; i < 4; i++) {
            if ((this->x == starpointsCho[i][0]) && (this->y == starpointsCho[i][1])) {
                if (board[4][8] != nullptr) {
                    if (isCannon(*board[4][8])) {   // 가운데 기물이 cannon인지 확인
                        break;
                    }
                    int x = starpointsCho[((i + 2) % 4)][0];
                    int y = starpointsCho[((i + 2) % 4)][1];
                    if (isMovable(x, y, this->team) == 0) { // jump했을 때 이동 가능한지 확인
                        validMoves.push_back(make_pair(x, y));
                        break;
                    }
                    else if (isMovable(x, y, this->team) == 1) {
                        if (isCannon(*board[x][y])) {
                            break;
                        }
                        validMoves.push_back(make_pair(x, y));
                        break;
                    }
                    else break;
                }
                else break;
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
        score = 5;
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
        score = 3;
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
        score = 0;
    }

    vector<pair<int, int>> generatePaths() override {
        vector<pair<int, int>> validMoves;
        int starpointsHan[4][2] = { {3, 0}, {5, 0}, {5, 2}, {3, 2} };   // 한나라 궁성 모서리 좌표 저장
        int starpointsCho[4][2] = { {3, 7}, {5, 7}, {5, 9}, {3, 9} };   // 한나라 궁성 모서리 좌표 저장
        int directions[4][2] = { {0,1}, {1,0}, {0,-1}, {-1, 0} };

        for (int i = 0; i < 4; i++) {   // 십자 방향 
            int x = this->x + directions[i][0];
            int y = this->y + directions[i][1];
            int movableFlag = isMovable(x, y, team);

            if (this->team == 'H') {
                if (x >= 3 && x <= 5 && y >= 0 && y <= 2) {
                    if (movableFlag == 0 || movableFlag == 1) {
                        validMoves.push_back(make_pair(x, y));
                    }
                }

            }
            else {
                if (x >= 3 && x <= 5 && y >= 7 && y <= 9) {
                    if (movableFlag == 0 || movableFlag == 1) {
                        validMoves.push_back(make_pair(x, y));
                    }
                }
            }
        }
        if (this->team == 'H') {    // 한나라
            if (this->x == 4 && this->y == 1) { // 궁성 가운데
                for (int i = 0; i < 4; i++) {
                    int x = starpointsHan[i][0];
                    int y = starpointsHan[i][1];
                    int movableFlag = isMovable(x, y, team);
                    if (movableFlag == 0 || movableFlag == 1) {
                        validMoves.push_back(make_pair(x, y));
                    }
                }
            }
            else {   // 궁성 모서리
                for (int i = 0; i < 4; i++) {
                    int x = starpointsHan[i][0];
                    int y = starpointsHan[i][1];
                    if (this->x == x && this->y == y) {
                        int movableFlag = isMovable(4, 1, team);
                        if (movableFlag == 0 || movableFlag == 1) {
                            validMoves.push_back(make_pair(4, 1));
                            break;
                        }
                        else break;
                    }
                }
            }
        }
        else {  // 초나라
            if (this->x == 4 && this->y == 8) { // 궁성 가운데
                for (int i = 0; i < 4; i++) {
                    int x = starpointsCho[i][0];
                    int y = starpointsCho[i][1];
                    int movableFlag = isMovable(x, y, team);
                    if (movableFlag == 0 || movableFlag == 1) {
                        validMoves.push_back(make_pair(x, y));
                    }
                }
            }
            else {
                for (int i = 0; i < 4; i++) {
                    int x = starpointsCho[i][0];
                    int y = starpointsCho[i][1];
                    if (this->x == x && this->y == y) {
                        int movableFlag = isMovable(4, 8, team);
                        if (movableFlag == 0 || movableFlag == 1) {
                            validMoves.push_back(make_pair(4, 8));
                            break;
                        }
                        else break;
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
        score = 3;
    }

   vector<pair<int, int>> generatePaths() override {
        vector<pair<int, int>> validMoves;
        int starpointsHan[4][2] = { {3, 0}, {5, 0}, {5, 2}, {3, 2} };   // 한나라 궁성 모서리 좌표 저장
        int starpointsCho[4][2] = { {3, 7}, {5, 7}, {5, 9}, {3, 9} };   // 한나라 궁성 모서리 좌표 저장
        int directions[4][2] = { {0,1}, {1,0}, {0,-1}, {-1, 0} };

        for (int i = 0; i < 4; i++) {   // 십자 방향 
            int x = this->x + directions[i][0];
            int y = this->y + directions[i][1];
            int movableFlag = isMovable(x, y, team);

            if (this->team == 'H') {
                if (x >= 3 && x <= 5 && y >= 0 && y <= 2) {
                    if (movableFlag == 0 || movableFlag == 1) {
                        validMoves.push_back(make_pair(x, y));
                    }
                }

            }
            else {
                if (x >= 3 && x <= 5 && y >= 7 && y <= 9) {
                    if (movableFlag == 0 || movableFlag == 1) {
                        validMoves.push_back(make_pair(x, y));
                    }
                }
            }
        }
        if (this->team == 'H') {    // 한나라
            if (this->x == 4 && this->y == 1) { // 궁성 가운데
                for (int i = 0; i < 4; i++) {
                    int x = starpointsHan[i][0];
                    int y = starpointsHan[i][1];
                    int movableFlag = isMovable(x, y, team);
                    if (movableFlag == 0 || movableFlag == 1) {
                        validMoves.push_back(make_pair(x, y));
                    }
                }
            }
            else {   // 궁성 모서리
                for (int i = 0; i < 4; i++) {
                    int x = starpointsHan[i][0];
                    int y = starpointsHan[i][1];
                    if (this->x == x && this->y == y) {
                        int movableFlag = isMovable(4, 1, team);
                        if (movableFlag == 0 || movableFlag == 1) {
                            validMoves.push_back(make_pair(4, 1));
                            break;
                        }
                        else break;
                    }
                }
            }
        }
        else {  // 초나라
            if (this->x == 4 && this->y == 8) { // 궁성 가운데
                for (int i = 0; i < 4; i++) {
                    int x = starpointsCho[i][0];
                    int y = starpointsCho[i][1];
                    int movableFlag = isMovable(x, y, team);
                    if (movableFlag == 0 || movableFlag == 1) {
                        validMoves.push_back(make_pair(x, y));
                    }
                }
            }
            else {
                for (int i = 0; i < 4; i++) {
                    int x = starpointsCho[i][0];
                    int y = starpointsCho[i][1];
                    if (this->x == x && this->y == y) {
                        int movableFlag = isMovable(4, 8, team);
                        if (movableFlag == 0 || movableFlag == 1) {
                            validMoves.push_back(make_pair(4, 8));
                            break;
                        }
                        else break;
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
        score = 2;
    }

    vector<pair<int, int>> generatePaths() override {
        vector<pair<int, int>> validMoves;
        int hanDirections[3][2] = { {0, 1}, {1, 0}, {-1, 0} };
        int choDirections[3][2] = { {0, -1}, {1, 0}, {-1, 0} };
        if (this->team == 'H') {
            for (int i = 0; i < 3; i++) {
                int x = this->x + hanDirections[i][0];
                int y = this->y + hanDirections[i][1];

                if (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT) {
                    int movableFlag = isMovable(x, y, team);
                    if (movableFlag == 0 || movableFlag == 1) {
                        validMoves.push_back(make_pair(x, y));
                    }
                }
            }
            if ((this->x == 3 || this->x == 5) && this->y == 7) {    // 모서리 있을 때
                int movableFlag = isMovable(4, 8, team);
                if (movableFlag == 0 || movableFlag == 1) {
                    validMoves.push_back(make_pair(4, 8));
                }
            }
            else if (this->x == 4 && this->y == 8) { // 가운데 있을 때
                int movableFlag = isMovable(3, 9, team);
                if (movableFlag == 0 || movableFlag == 1) {
                    validMoves.push_back(make_pair(3, 9));
                }
                movableFlag = isMovable(5, 9, team);
                if (movableFlag == 0 || movableFlag == 1) {
                    validMoves.push_back(make_pair(5, 9));
                }
            }
            else;
        }
        else {
            for (int i = 0; i < 3; i++) {
                int x = this->x + choDirections[i][0];
                int y = this->y + choDirections[i][1];

                if (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT) {
                    int movableFlag = isMovable(x, y, team);
                    if (movableFlag == 0 || movableFlag == 1) {
                        validMoves.push_back(make_pair(x, y));
                    }
                }
            }
            if ((this->x == 3 || this->x == 5) && this->y == 2) {    // 모서리 있을 때
                int movableFlag = isMovable(4, 1, team);
                if (movableFlag == 0 || movableFlag == 1) {
                    validMoves.push_back(make_pair(4, 1));
                }
            }
            else if (this->x == 4 && this->y == 1) { // 가운데 있을 때
                int movableFlag = isMovable(3, 0, team);
                if (movableFlag == 0 || movableFlag == 1) {
                    validMoves.push_back(make_pair(3, 0));
                }
                movableFlag = isMovable(5, 0, team);
                if (movableFlag == 0 || movableFlag == 1) {
                    validMoves.push_back(make_pair(5, 0));
                }
            }
            else;
        }
        return validMoves;
    }
};

stack<BoardState> previous;


string setup[] = {"1. 마상상마", "2. 마상마상", "3. 상마상마", "4. 상마마상"};

Piece* quit = new Pawn(1, 1, '.');
Piece* cancel = new Pawn(1, 1, ',');

int main() {
    // 컴파일 시 한글 깨짐 해결
    system("chcp 65001");
    system("cls");
    int remove;
    Piece* chosen;
    int quitOnMove;
    
    while (true) {
        mainMenu();
        setupInitialPieces(game, game.han);
        gamestart = 0;
        remove = remove_piece_num();
        if (remove >= 1) { // 제거할 기물이 1개 이상
            // 한나라, 초나라 포진 과정
            setupBoard(game, game.han);
            printBoard();
            remove_select_piece(remove);
            printBoard();
            setupInitialPieces(game, game.cho);
            setupBoard(game, game.cho);
            gamestart = 1;
            cout << format(msg[32], { {"player", "한"} });
            //Sleep(2000);
            while (true) {
                // 한나라 턴
                previous.push(BoardState(board));
                while(true) {
                    printBoard(); // 보드출력
                    chosen = choosePiece(game.han); // 기물선택
                    if (chosen->team == '.') {
                        cout << format(msg[6], { {"player", "한"} }) << msg[15];
                        while (true) {
                            getline(cin, input);
                            if (input.compare("Y") == 0 || input.compare("y") == 0) break;
                            else cout << msg[26] << msg[0];
                        }
                        break;
                    }
                    else if (chosen->team == ',') {
                        cout << format(msg[34], { {"player", "한"} });
                        while (true) {
                            getline(cin, input);
                            if (input.compare("Y") == 0 || input.compare("y") == 0 || input.compare("N") == 0 || input.compare("n") == 0) break;
                            else cout << msg[22];
                        }
                        if (input.compare("Y") == 0 || input.compare("y") == 0) undo();
                    }
                    else{
                quitOnMove = chosen->movePiece();  // 기물이동
                if (quitOnMove == 1) {
                    cout << format(msg[6], { {"player", "한"} }) << msg[7] << msg[0];
                    while (true) {
                        getline(cin, input);
                        if (input.compare("Y") == 0 || input.compare("y") == 0) break;
                        else cout << msg[26] << msg[0];
                    }
                    continue;
                } else if (quitOnMove == 2) {
                    continue;
                }
                        break;
                    }
                }
                if (quitOnMove == 2) break;
                if (chosen->team == '.') break;
                printBoard(); // 이동후 보드출력

                if (choCheckWin()) {
                    cout << msg[15] << msg[0];
                    while (true) {
                        getline(cin, input);
                        if (input.compare("Y") == 0 || input.compare("y") == 0) break;
                        else cout << msg[26] << msg[0];
                    }
                    break; // 승패여부 처리
                }

                cout << msg[7] << msg[0];
                while (true) {
                    getline(cin, input);
                    if (input.compare("Y") == 0 || input.compare("y") == 0) break;
                    else cout << msg[26] << msg[0];
                }

                game.turn++;

                // 초나라 턴
                previous.push(BoardState(board));
                while(true) {
                    printBoard(); // 보드출력
                    chosen = choosePiece(game.cho); // 기물선택
                    if (chosen->team == '.') {
                        cout << format(msg[6], { {"player", "초"} }) << msg[15];
                        while (true) {
                            getline(cin, input);
                            if (input.compare("Y") == 0 || input.compare("y") == 0) break;
                            else cout << msg[26] << msg[0];
                        }
                        break;
                    }
                    else if (chosen->team == ',') {
                        cout << format(msg[34], { {"player", "초"} });
                        while (true) {
                            getline(cin, input);
                            if (input.compare("Y") == 0 || input.compare("y") == 0 || input.compare("N") == 0 || input.compare("n") == 0) break;
                            else cout << msg[22];
                        }
                        if (input.compare("Y") == 0 || input.compare("y") == 0) undo();
                    }
                    else{
                quitOnMove = chosen->movePiece();
                if (quitOnMove == 1) {
                    cout << format(msg[6], { {"player", "초"} }) << msg[7] << msg[0];
                    while (true) {
                        getline(cin, input);
                        if (input.compare("Y") == 0 || input.compare("y") == 0) break;
                        else cout << msg[26] << msg[0];
                    }
                    break;
                } else if (quitOnMove == 2) {
                    continue;
                }
                        break;
                    }
                }
                if (quitOnMove == 2) break;
                if (chosen->team == '.') break;
                
                printBoard();

                if (choCheckWin()) {
                    cout << msg[15] << msg[0];
                    while (true) {
                        getline(cin, input);
                        if (input.compare("Y") == 0 || input.compare("y") == 0) break;
                        else cout << msg[26] << msg[0];
                    }
                    break; // 승패여부 처리
                }
                cout << msg[7] << msg[0];
                while (true) {
                    getline(cin, input);
                    if (input.compare("Y") == 0 || input.compare("y") == 0) break;
                    else cout << msg[26] << msg[0];
                }

                game.turn++;
            }
        }
        else { // 제거할 기물이 0개
            setupBoard(game, game.han);
            setupInitialPieces(game, game.cho);
            setupBoard(game, game.cho);
            setup_score();
            gamestart = 1;
            while (true) {
                // 초나라 턴
                previous.push(BoardState(board));
                while(true) {
                    printBoard(); // 보드출력
                    chosen = choosePiece(game.cho); // 기물선택
                    if (chosen->team == '.') {
                        cout << format(msg[6], { {"player", "초"} }) << msg[15];
                        while (true) {
                            getline(cin, input);
                            if (input.compare("Y") == 0 || input.compare("y") == 0) break;
                            else cout << msg[26] << msg[0];
                        }
                        break;
                    }
                    else if (chosen->team == ',') {
                        cout << format(msg[34], { {"player", "초"} });
                        while (true) {
                            getline(cin, input);
                            if (input.compare("Y") == 0 || input.compare("y") == 0 || input.compare("N") == 0 || input.compare("n") == 0) break;
                            else cout << msg[22];
                        }
                        if (input.compare("Y") == 0 || input.compare("y") == 0) undo();
                    }
                    else{
                quitOnMove = chosen->movePiece(); // 기물이동
                if (quitOnMove == 1) {
                    cout << format(msg[6], { {"player", "초"} }) << msg[7] << msg[0];
                    while (true) {
                        getline(cin, input);
                        if (input.compare("Y") == 0 || input.compare("y") == 0) break;
                        else cout << msg[26] << msg[0];
                    }
                    break;
                } else if (quitOnMove == 2) {
                    continue;
                }
                        break;
                    }
                }
                if (quitOnMove == 2) break;
                if (chosen->team == '.') break;
                
                printBoard(); // 이동후 보드출력

                if (choCheckWin()) {
                    cout << msg[15] << msg[0];
                    while (true) {
                        getline(cin, input);
                        if (input.compare("Y") == 0 || input.compare("y") == 0) break;
                        else cout << msg[26] << msg[0];
                    }
                    break; // 승패여부 처리
                }
                cout << msg[7] << msg[0];
                while (true) {
                    getline(cin, input);
                    if (input.compare("Y") == 0 || input.compare("y") == 0) break;
                    else cout << msg[26];
                }

                game.turn++;

                // 한나라 턴
                previous.push(BoardState(board));
                while(true) {
                    printBoard(); // 보드출력
                    chosen = choosePiece(game.han); // 기물선택
                    if (chosen->team == '.') {
                        cout << format(msg[6], { {"player", "한"} }) << msg[15];
                        while (true) {
                            getline(cin, input);
                            if (input.compare("Y") == 0 || input.compare("y") == 0) break;
                            else cout << msg[26] << msg[0];
                        }
                        break;
                    }
                    else if (chosen->team == ',') {
                        cout << format(msg[34], { {"player", "한"} });
                        while (true) {
                            getline(cin, input);
                            if (input.compare("Y") == 0 || input.compare("y") == 0 || input.compare("N") == 0 || input.compare("n") == 0) break;
                            else cout << msg[22];
                        }
                        if (input.compare("Y") == 0 || input.compare("y") == 0) undo();
                    }
                    else{
                quitOnMove = chosen->movePiece();
                if (quitOnMove == 1) {
                    cout << format(msg[6], { {"player", "한"} }) << msg[7] << msg[0];
                    while (true) {
                        getline(cin, input);
                        if (input.compare("Y") == 0 || input.compare("y") == 0) break;
                        else cout << msg[26] << msg[0];
                    }
                    break;
                } else if (quitOnMove == 2) {
                    continue;
                }
                        break;
                    }
                }
                if (quitOnMove == 2) break;
                if (chosen->team == '.') break;
                
                printBoard();

                if (choCheckWin()) {
                    cout << msg[15] << msg[0];
                    while (true) {
                        getline(cin, input);
                        if (input.compare("Y") == 0 || input.compare("y") == 0) break;
                        else cout << msg[26] << msg[0];
                    }
                    break; // 승패여부 처리
                }
                cout << msg[7] << msg[0];
                while (true) {
                    getline(cin, input);
                    if (input.compare("Y") == 0 || input.compare("y") == 0) break;
                    else cout << msg[26] << msg[0];
                }

                game.turn++;
            }
        }
    }
    return 0;
}

void mainMenu() {
    // clear the console
    string s;
    while (1) {
        // system("cls");
        cout << msg[1] << msg[2] << msg[3] << msg[0];
        getline(cin, s);
        if (s != "1" && s != "2") {
            cout << msg[21] << msg[22];
        }
        if (s == "2")
            exit(0);
        if (s == "1")
            return;
        //Sleep(1000);
    }
    
}

// 김의찬 작성
void setupInitialPieces(Game& game, Player& player) {

    if (&player == &game.han) {
        // clear the board
        for (int i = 0; i < BOARD_WIDTH; i++) {
            for (int j = 0; j < BOARD_HEIGHT; j++) {
                board[i][j] = nullptr;
            }
        }
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
    }
    if (&player == &game.cho) {
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
}


void setupBoard(Game& game, Player& player) {


    setup_score();
    if (&player == &game.han) {
        while (true) { // while문
            int num;
            string input;
            cout << format(msg[30], { {"player", "한"} });
            cout << "    <한나라 포진>\n\n"
                << setup[0] << " 포진" << "\n    A B C D E F G H I" << "\n 0 |R|N|E|G| |G|E|N|R|\n\n"
                << setup[1] << " 포진" << "\n    A B C D E F G H I" << "\n 0 |R|N|E|G| |G|N|E|R|\n\n"
                << setup[2] << " 포진" << "\n    A B C D E F G H I" << "\n 0 |R|E|N|G| |G|E|N|R|\n\n"
                << setup[3] << " 포진" << "\n    A B C D E F G H I" << "\n 0 |R|E|N|G| |G|N|E|R|\n\n";
            cout << msg[4] << msg[0];
            getline(cin, input); // 사용자로부터 문자열로 입력을 받습니다.
            stringstream ss(input); // 입력받은 문자열을 스트림으로 변환합니다.
            if (ss >> num && num > 0 && num < 5) { // if문
                switch (num) { // switch문
                case 1:
                    cout << format(msg[19], { {"player", "한"}, {"setup", setup[0] } }) << endl;
                    game.han.placement = setup[0];
                    board[1][0] = new Knight(1, 0, 'H');
                    board[7][0] = new Knight(7, 0, 'H');
                    board[2][0] = new Elephant(2, 0, 'H');
                    board[6][0] = new Elephant(6, 0, 'H');
                    break;
                case 2:
                    cout << format(msg[19], { {"player", "한"}, {"setup", setup[1] } }) << endl;
                    game.han.placement = setup[1];
                    board[1][0] = new Knight(1, 0, 'H');
                    board[6][0] = new Knight(6, 0, 'H');
                    board[2][0] = new Elephant(2, 0, 'H');
                    board[7][0] = new Elephant(7, 0, 'H');
                    break;
                case 3:
                    cout << format(msg[19], { {"player", "한"}, {"setup", setup[2] } }) << endl;
                    game.han.placement = setup[2];
                    board[2][0] = new Knight(2, 0, 'H');
                    board[7][0] = new Knight(7, 0, 'H');
                    board[1][0] = new Elephant(1, 0, 'H');
                    board[6][0] = new Elephant(6, 0, 'H');
                    break;
                case 4:
                    cout << format(msg[19], { {"player", "한"}, {"setup", setup[3] } }) << endl;
                    game.han.placement = setup[3];
                    board[2][0] = new Knight(2, 0, 'H');
                    board[6][0] = new Knight(6, 0, 'H');
                    board[1][0] = new Elephant(1, 0, 'H');
                    board[7][0] = new Elephant(7, 0, 'H');
                    break;
                } // switch문
                break;
            } // if문
            else {
                cout << msg[25];
            }
        } // while문
    }

    else if (&player == &game.cho) {
        while (true) { // while문
            int num;
            string input;
            cout << format(msg[30], { {"player", "초"} });
            cout << "    <초나라 포진>\n\n"
                << setup[0] << " 포진" << "\n    A B C D E F G H I" << "\n 9 |R|N|E|G| |G|E|N|R|\n\n"
                << setup[1] << " 포진" << "\n    A B C D E F G H I" << "\n 9 |R|N|E|G| |G|N|E|R|\n\n"
                << setup[2] << " 포진" << "\n    A B C D E F G H I" << "\n 9 |R|E|N|G| |G|E|N|R|\n\n"
                << setup[3] << " 포진" << "\n    A B C D E F G H I" << "\n 9 |R|E|N|G| |G|N|E|R|\n\n";
            cout << format(msg[19], { {"player", "한"}, {"setup", game.han.placement } }) << msg[4] << msg[0];
            getline(cin, input); // 사용자로부터 문자열로 입력을 받습니다.
            stringstream ss(input); // 입력받은 문자열을 스트림으로 변환합니다.
            if (ss >> num && num > 0 && num < 5) { // if문
                switch (num) { // switch문
                case 1:
                    cout << format(msg[19], { {"player", "초"}, {"setup", setup[0] } }) << endl;
                    board[1][9] = new Knight(1, 9, 'C');
                    board[7][9] = new Knight(7, 9, 'C');
                    board[2][9] = new Elephant(2, 9, 'C');
                    board[6][9] = new Elephant(6, 9, 'C');
                    break;
                case 2:
                    cout << format(msg[19], { {"player", "초"}, {"setup", setup[1] } }) << endl;
                    board[1][9] = new Knight(1, 9, 'C');
                    board[6][9] = new Knight(6, 9, 'C');
                    board[2][9] = new Elephant(2, 9, 'C');
                    board[7][9] = new Elephant(7, 9, 'C');
                    break;
                case 3:
                    cout << format(msg[19], { {"player", "초"}, {"setup", setup[2] } }) << endl;
                    board[2][9] = new Knight(2, 9, 'C');
                    board[7][9] = new Knight(7, 9, 'C');
                    board[1][9] = new Elephant(1, 9, 'C');
                    board[6][9] = new Elephant(6, 9, 'C');
                    break;
                case 4:
                    cout << format(msg[19], { {"player", "초"}, {"setup", setup[3] } }) << endl;
                    board[2][9] = new Knight(2, 9, 'C');
                    board[6][9] = new Knight(6, 9, 'C');
                    board[1][9] = new Elephant(1, 9, 'C');
                    board[7][9] = new Elephant(7, 9, 'C');
                    break;
                } // switch문
                break;
            } // if문
            else {
                cout << msg[25];
            }
        } // while문
    }
    return;
}

int remove_piece_num() {
    int num;
    string input;
    while (true) {
        cout << msg[28] << msg[0];
        getline(cin, input); // 사용자로부터 문자열로 입력을 받습니다.
        stringstream ss(input); // 입력받은 문자열을 스트림으로 변환합니다.
        if (ss >> num && num >= 0 && num <= 6) {
            switch (num) {
            case 0:
                cout << "0을 입력받았습니다\n";
                break;
            case 1:
                cout << "1(차)를 입력받았습니다\n";
                break;
            case 2:
                cout << "2(차/포)를 입력받았습니다\n";
                break;
            case 3:
                cout << "3(차/포/마)를 입력받았습니다\n";
                break;
            case 4:
                cout << "4(차/포/마/상)를 입력받았습니다\n";
                break;
            case 5:
                cout << "5(차/포/마/상/사)를 입력받았습니다\n";
                break;
            case 6:
                cout << "6(차/포/마/상/사/졸)를 입력받았습니다\n";
                break;
            }
            return num;
        }
        else {
            cout << "0과 6 사이의 정수를 입력하시오.\n";
        }
    }
}

void remove_select_piece(int num) {
    vector<string> piecesToRemove;
    switch (num) {
    case 1:
        cout << "한나라는 제거할 1(차)개의 좌표를 입력하세요.\n" << msg[0];
        piecesToRemove.push_back("R");
        break;
    case 2:
        cout << "한나라는 제거할 2(차/포)개의 좌표를 입력하세요.\n" << msg[0];
        piecesToRemove.push_back("R");
        piecesToRemove.push_back("C");
        break;
    case 3:
        cout << "한나라는 제거할 3(차/포/마)개의 좌표를 입력하세요.\n" << msg[0];
        piecesToRemove.push_back("R");
        piecesToRemove.push_back("C");
        piecesToRemove.push_back("N");
        break;
    case 4:
        cout << "한나라는 제거할 4(차/포/마/상)개의 좌표를 입력하세요.\n" << msg[0];
        piecesToRemove.push_back("R");
        piecesToRemove.push_back("C");
        piecesToRemove.push_back("N");
        piecesToRemove.push_back("E");
        break;
    case 5:
        cout << "한나라는 제거할 5(차/포/마/상/사)개의 좌표를 입력하세요.\n" << msg[0];
        piecesToRemove.push_back("R");
        piecesToRemove.push_back("C");
        piecesToRemove.push_back("N");
        piecesToRemove.push_back("E");
        piecesToRemove.push_back("G");
        break;
    case 6:
        cout << "한나라는 제거할 6(차/포/마/상/사/졸)개의 좌표를 입력하세요.\n" << msg[0];
        piecesToRemove.push_back("R");
        piecesToRemove.push_back("C");
        piecesToRemove.push_back("N");
        piecesToRemove.push_back("E");
        piecesToRemove.push_back("G");
        piecesToRemove.push_back("P");
        break;
    default:
        return;
    }
    bool inputValid = true;
    vector<pair<int, int>> coordinates;

    do {
        inputValid = true; // 입력 유효성 플래그를 초기화
        coordinates.clear(); // 좌표 벡터를 초기화
        string input;
        getline(cin, input); // 사용자로부터 좌표 문자열을 입력받습니다.

        stringstream ss(input);
        string token;
        while (ss >> token) { // 공백을 기준으로 문자열을 분리합니다.
            if (token.size() == 2 && isdigit(token[0]) && isalpha(token[1])) {
                int y = token[0] - '0'; // 첫 번째 문자를 숫자로 변환
                int x = toupper(token[1]) - 'A'; // 두 번째문자를 알파벳 인덱스로 변환
                if (x >= 0 && x < 9 && y >= 0 && y < 10) {
                    coordinates.push_back({ x, y });
                }
                else {
                    inputValid = false; // 좌표 범위를 벗어났습니다.
                    break;
                }
            }
            else {
                inputValid = false; // 잘못된 형식입니다.
                break;
            }
        }

        if (!inputValid || coordinates.size() != piecesToRemove.size()) {
            cout << "입력된 좌표에 해당하는 기물이 존재하지 않습니다. 다시 입력하세요." << endl;
            return remove_select_piece(num);
        }
    } while (!inputValid || coordinates.size() != piecesToRemove.size());

    // 좌표에 따른 기물 제거 로직
    for (const auto& coord : coordinates) {
        int x = coord.first;
        int y = coord.second;

        if (board[x][y] && board[x][y]->team == 'H') {
            string currentPiece(1, board[x][y]->letter);
            auto it = find(piecesToRemove.begin(), piecesToRemove.end(), currentPiece);
            if (it != piecesToRemove.end()) {
                delete board[x][y];
                board[x][y] = nullptr;
                piecesToRemove.erase(it);  // 이미 제거된 기물은 리스트에서 제거
            }
            else {
                cout << "입력된 좌표에 해당하는 기물이 존재하지 않습니다. 다시 입력하세요." << endl;
                return remove_select_piece(num);  // 잘못된 좌표가 있으면 다시 입력받습니다.
            }
        }
        else {
            cout << "입력된 좌표에 해당하는 기물이 존재하지 않습니다. 다시 입력하세요." << endl;
            return remove_select_piece(num);  // 잘못된 좌표가 있으면 다시 입력받습니다.
        }
    }
}

// 백창현 작성, 좌표 입력은 잘 되는데 board랑은 확인해봐야함


void gameplay(int remove, Piece* chosen, int quitOnMove){
    if (remove >= 1) { // 제거할 기물이 1개 이상
            // 한나라, 초나라 포진 과정
            setupBoard(game, game.han);
            printBoard();
            remove_select_piece(remove);
            printBoard();
            setupInitialPieces(game, game.cho);
            setupBoard(game, game.cho);
            gamestart = 1;
            cout << format(msg[32], { {"player", "한"} });
            //Sleep(2000);
            turnhan(chosen, quitOnMove);
            turncho(chosen, quitOnMove);
                
    }
    else{ //밥먹고 추가

    }
}

Piece* choosePiece(Player& player) {
    int tmpx, tmpy;
    string coord;

    while (true) {
        string currentTurnTeam;
        if (&player == &game.han)
            currentTurnTeam = "한";
        else
            currentTurnTeam = "초";

        cout << format(msg[5], {{"player", currentTurnTeam}}) << msg[11];
        if(game.turn > 1){
            cout << msg[33];
        }
        cout << msg[0];

        getline(cin, coord);
        if (!coord.compare("quit")) {
            return quit;
        }

        if (!coord.compare("cancel") && game.turn > 1) {
            //무르기 처리
            // 
            //
            return cancel;
        }
        else if(!coord.compare("cancel") && game.turn <= 1){

            cout << "더 이상 무를 수 없습니다. 다시 입력하세요.\n";
            continue;
        }

        // 좌표 입력 규칙 확인 (2글자이고, 첫번째는 숫자이고, 두번째는 소문자 혹은 대문자인지)

        if(coord.length() == 1){
            cout << "다시 입력하세요.\n";
            continue;
        }
        if (coord.length() !=2 || !isdigit(coord[0]) || (!(coord[1] >= 'a' && coord[1] <= 'i') && !(coord[1] >= 'A' && coord[1] <= 'I')))  {
            cout << msg[23] << msg[22] << endl;
            continue;
        }

        tmpx = (coord[1] >= 'a' && coord[1] <= 'i') ? coord[1] - 'a' : coord[1] - 'A';
        tmpy = coord[0] - '0';
        
        if (board[tmpx][tmpy] == nullptr) {
            cout << msg[23] << msg[22] << endl;
            continue;
        }

        if ((&player == &game.han && board[tmpx][tmpy]->team == 'C') || (&player == &game.cho && board[tmpx][tmpy]->team == 'H')) {
            cout << msg[27] << msg[22] << endl;
            continue;
        }

        return board[tmpx][tmpy];
    }
}

void turnhan(Piece* chosen, int quitOnMove){
    while (true) {
                // 한나라 턴
                previous.push(BoardState(board));
                while(true) {
                    printBoard(); // 보드출력
                    chosen = choosePiece(game.han); // 기물선택
                    if (chosen->team == '.') {
                        cout << format(msg[6], { {"player", "한"} }) << msg[15];
                        while (true) {
                            getline(cin, input);
                            if (input.compare("Y") == 0 || input.compare("y") == 0) break;
                            else cout << msg[26] << msg[0];
                        }
                        break;
                    }
                    else if (chosen->team == ',') {
                        cout << format(msg[34], { {"player", "한"} });
                        while (true) {
                            getline(cin, input);
                            if (input.compare("Y") == 0 || input.compare("y") == 0 || input.compare("N") == 0 || input.compare("n") == 0) break;
                            else cout << msg[22];
                        }
                        if (input.compare("Y") == 0 || input.compare("y") == 0) undo();
                    }
                    else{
                quitOnMove = chosen->movePiece();  // 기물이동
                if (quitOnMove == 1) {
                    cout << format(msg[6], { {"player", "한"} }) << msg[7] << msg[0];
                    while (true) {
                        getline(cin, input);
                        if (input.compare("Y") == 0 || input.compare("y") == 0) break;
                        else cout << msg[26] << msg[0];
                    }
                    continue;
                } else if (quitOnMove == 2) {
                    continue;
                }
                        break;
                    }
                }
                if (quitOnMove == 2) break;
                if (chosen->team == '.') break;
                printBoard(); // 이동후 보드출력

                if (choCheckWin()) {
                    cout << msg[15] << msg[0];
                    while (true) {
                        getline(cin, input);
                        if (input.compare("Y") == 0 || input.compare("y") == 0) break;
                        else cout << msg[26] << msg[0];
                    }
                    break; // 승패여부 처리
                }

                cout << msg[7] << msg[0];
                while (true) {
                    getline(cin, input);
                    if (input.compare("Y") == 0 || input.compare("y") == 0) break;
                    else cout << msg[26] << msg[0];
                }

                game.turn++;
    }
}

void turncho(Piece* chosen, int quitOnMove){
    while(true){
        previous.push(BoardState(board));
            while(true) {
                printBoard(); // 보드출력
                chosen = choosePiece(game.cho); // 기물선택
                    if (chosen->team == '.') {
                        cout << format(msg[6], { {"player", "초"} }) << msg[15];
                        while (true) {
                            getline(cin, input);
                            if (input.compare("Y") == 0 || input.compare("y") == 0) break;
                            else cout << msg[26] << msg[0];
                        }
                            break;
                    }
                    else if (chosen->team == ',') {
                        cout << format(msg[34], { {"player", "초"} });
                        while (true) {
                            getline(cin, input);
                            if (input.compare("Y") == 0 || input.compare("y") == 0 || input.compare("N") == 0 || input.compare("n") == 0) break;
                            else cout << msg[22];
                        }
                            if (input.compare("Y") == 0 || input.compare("y") == 0) undo();
                    }
                    else{
                        quitOnMove = chosen->movePiece();
                        if (quitOnMove == 1) {
                            cout << format(msg[6], { {"player", "초"} }) << msg[7] << msg[0];
                            while (true) {
                                getline(cin, input);
                                if (input.compare("Y") == 0 || input.compare("y") == 0) break;
                                else cout << msg[26] << msg[0];
                            }
                            break;
                        }
                        else if (quitOnMove == 2) {
                            continue;
                        }
                        break;
                    }
                }
                if (quitOnMove == 2) break;
                if (chosen->team == '.') break;
                    
                printBoard();

                if (choCheckWin()) {
                    cout << msg[15] << msg[0];
                    while (true) {
                        getline(cin, input);
                        if (input.compare("Y") == 0 || input.compare("y") == 0) break;
                        else cout << msg[26] << msg[0];
                    }
                        break; // 승패여부 처리
                }
                    cout << msg[7] << msg[0];
                    while (true) {
                        getline(cin, input);
                        if (input.compare("Y") == 0 || input.compare("y") == 0) break;
                        else cout << msg[26] << msg[0];
                    }

                    game.turn++;
                }
    }
// 김종우 작성 - 보드 출력 
void printBoard() {

    setup_score();
    int starpoints[10][2] = { {3, 0}, {5, 0}, {4, 1}, {3, 2}, {5, 2},   // 궁성 좌표 저장
                            {3, 7}, {5, 7}, {4, 8}, {3, 9}, {5, 9} };
    system("cls");    // 프롬프트 clear
    if (gamestart == 0) {
        cout << "    A B C D E F G H I   " << endl;  // 가장 윗줄 출력 
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
            cout << endl;
        }
    }
    else {
        cout << "    A B C D E F G H I   turn : " << game.turn << endl;  // 가장 윗줄 출력 
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
            if (row == 0) { cout << " 한나라 score : " << game.han.score; } // 첫째 줄에 한나라 score 출력
            else if (row == 1) { cout << " 초나라 score : " << game.cho.score; } // 둘째 줄에 초나라 score 출력
            cout << endl;
        }
    }
}

bool choCheckWin(){ //남경식
    if(isKingDie() || isTurnOver(game.turn) || isScoreUnder(game.cho.score, game.han.score)){
        if(isKingDie()){
            if(choKingDie(King_Location)){
                cout << format(msg[13], {{"lose_player","초"}}) << format(msg[13], {{"win_player","한"}});
                return true;
            }
            else{
                cout << format(msg[13], {{"lose_player","한"}}) << format(msg[13], {{"win_player","초"}});
                return true;
            }
        }
        else if(isTurnOver(game.turn)){


            cout << msg[16];
            cout << "점수 총합 : 초나라 " << game.cho.score << "점, 한나라 " << game.han.score << "점\n";
            if(game.cho.score > game.han.score){
                cout << format(msg[14], {{"win_player", "초"}});
                return true;
            }
            else{
                cout << format(msg[14], {{"win_player", "한"}});
                return true;
            }
        }
        else{
            cout << msg[17];
           cout << "점수 총합 : 초나라 " << game.cho.score << "점, 한나라 " << game.han.score << "점\n";
            if(game.cho.score > game.han.score){
                cout << format(msg[14], {{"win_player", "초"}});
                return true;
            }
            else{
                cout << format(msg[14], {{"win_player", "한"}});
                return true;
            }
        }
    }
    else{
        return false;
    }
};

bool isKingDie(){ //남경식
    int count = 0;
    for(int i=0; i<9; i++){
        for(int j=0; j<10; j++){
            if (board[i][j] == nullptr) continue;
            if(board[i][j]->letter == 'K' || board[i][j]->letter == 'k'){
                King_Location = board[i][j];
                count++;
            }
            
        }
        
    }
    if(count == 2){ //모든 좌표에 대해서 king이 2개있는지 확인, 수정
            return false;
        }
        else{

            return true;
        }
};

//남경식
bool isTurnOver(int turn){
    return (game.turn >= 99);
};

//남경식
bool isScoreUnder(double score1, double score2){
    return (score1 <= 30 && score2 <= 30);
};

//남경식
bool choKingDie(Piece* piece){
    return (piece->team != 'C');
};

//남경식
void undo(){
     if (previous.size() > 1) {
        for(int i=0; i<9; i++){
            for(int j=0; j<10; j++){
                board[i][j] = nullptr;
            }
        }
        previous.pop();
        previous.pop();
        BoardState lastState = previous.top();
        game.turn--;
        game.turn--;

        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 10; j++) {
                board[i][j] = lastState.state[i][j];
                }
            }
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 10; j++) {
                if(board[i][j] != nullptr){
                    board[i][j]->x = i;
                    board[i][j]->y = j;
                    }
                }
            }

        }
}

// format(msg[내용], { {"바꿀문자열1", "바꾼후문자열1"}, {"바꿀문자열2", "바꾼후문자열2"}, ... })
string format(const string& input, const map<string, string>& to) {
    string result = input;
    for (const auto& pair : to) {
        string from = "{" + pair.first + "}";
        size_t pos = result.find(from);
        while (pos != string::npos) {
            result.replace(pos, from.length(), pair.second);
            pos = result.find(from, pos + pair.second.length());
        }
    }
    return result;
}

void setup_score(){

    game.cho.score = 0;
    game.han.score = 0;

    for(int i=0; i<9; i++){
        for(int j=0; j<10; j++){
            if(board[i][j] == nullptr) continue;

            if(board[i][j]->team == 'C'){
                game.cho.score += board[i][j]->score;
            }  
            else if(board[i][j]->team == 'H'){
                game.han.score += board[i][j]->score;
            }
        }
    }

    game.han.score += 1.5;

}
