#include <iostream>
#include <ctime>
#include <conio.h>
#include <Windows.h>
#include <thread> 


class Snake {
    static const char HEAD = 'D';
    static const char BODY = '=';
    static const char TAIL = '8';
    Snake* next;

    void MakeMove() {
        if (next != nullptr) {
            next->MakeMove();
            next->i = this->i;
            next->j = this->j;
        }
        return;
    }

public:
    int i, j;
    Snake(int i, int j, int length) {
        length > 1 ? this->next = new Snake(i + 1, j, length - 1) : this->next = nullptr;
        this->i = i;
        this->j = j;
    }
    Snake(int i, int j, Snake* snake) {
        this->i = i;
        this->j = j;
        this->next = snake;
    }
    ~Snake() {
        if (next != nullptr)
            delete next;
    }
    bool CheckEat(int i, int j) {
        if (this->i == i && this->j == j)
            return true;
        else if (next != nullptr)
            return next->CheckEat(i, j);
        else return false;
    }

    void Show(HANDLE hCon, COORD cPos, char ch=HEAD) {
        cPos.Y = i;
        cPos.X = j;
        SetConsoleCursorPosition(hCon, cPos);
        std::cout << ch;
        if (next != nullptr)
            next->Show(hCon, cPos, BODY);
    }

    void Move(int i, int j, int size, int size2) {
        i != 0 ? i /= abs(i) : 0;
        j != 0 ? j /= abs(j) : 0;
        next->MakeMove();
        next->i = this->i;
        next->j = this->j;
        this->i += i;
        this->j += j;
        (this->i < 0) ? this->i += size : ((this->i >= size) ? this->i -= size : 0);
        (this->j < 0) ? this->j += size2 : ((this->j >= size2) ? this->j -= size2 : 0);
    }
};

class Apple {
    int i;
    int j;
    Snake* snake;
public:
    Apple(int size, int size2, Snake* snake) {
        this->snake = snake;
        srand(time(0));
        do {
            this->i = rand() % size;
            this->j = rand() % size2;
        } while (snake->CheckEat(i, j));
    }
    ~Apple() {}

    void Show(HANDLE hCon, COORD cPos) {
        cPos.Y = i;
        cPos.X = j;
        SetConsoleCursorPosition(hCon, cPos);
        std::cout << '0';
    }

    bool CheckEat() {
        return snake->CheckEat(i, j);
    }
};

class GUI {
public:
    static void Show(int size, int size2, Snake* snake, Apple* apple) {
        system("cls"); 
        HANDLE hCon;
        COORD cPos;
        cPos.Y = 0;
        cPos.X = 0;
        hCon = GetStdHandle(STD_OUTPUT_HANDLE);
        snake->Show(hCon, cPos);
        apple->Show(hCon, cPos);
        //for (int i = 0; i < size; i++) {
        //    for (int j = 0; j < size2; j++) {
        //        std::cout << ' ';
        //    }
        //    std::cout << std::endl;
        //}
    }
};

class Control {
public:
    static void Ctrl(int& i, int& j) {
        while (_kbhit()) {
            if (_getch() == 224) {
                int code = _getch();
                switch (code)
                {
                case 75:
                    (j != 1) ? j = -1, i = 0 : 0;
                    break;
                case 77:
                    (j != -1) ? j = 1, i = 0 : 0;
                    break;
                case 72:
                    (i != 1) ? i = -1, j = 0 : 0;
                    break;
                case 80:
                    (i != -1) ? i = 1, j = 0 : 0;
                    break;
                    //case 13:
                    //    break;
                }
            }
        }
    }
};

class Game {
    int size, size2;
    Snake* snake;
    Apple* apple;

public:
    Game(int size, int length) {
        this->size = size;
        size % 2 ? size2 = size : size2 = size + 1;
        snake = new Snake(size / 2 + 1, size2 / 2 + 1, length);
    }
    ~Game() {
        delete snake;
        delete apple;
    }
    void Start() {
        apple = new Apple(size, size2, snake);
        int i=-1, j=0;
        //std::thread ctrl(Control::Ctrl, std::ref(i), std::ref(j));

        do {
            Control::Ctrl(i, j);
            snake->Move(i, j, size, size2);
            if (apple->CheckEat()) {
                delete apple;
                apple = new Apple(size, size2, snake);
                Snake* sk = new Snake(snake->i + i, snake->j + j, snake);
                this->snake = sk;
            }
            GUI::Show(size, size2, snake, apple);
            Sleep(500);
            //bool check = !snake->CheckEat(snake->i + i, snake->j + j);
                
        } while (!snake->CheckEat(snake->i + i, snake->j + j));
    }
};


int main()
{
    Game* game = new Game(10, 3);
    game->Start();
    //Game game(10, 3);
    //game.Start();
    //game.~Game();
    return 0;
}