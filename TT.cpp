#include <iostream>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 25
#define WIN_WIDTH 70

using namespace std;

void play();
void gotoxy(int x, int y);
void setcursor(bool visible, DWORD size);
void drawBorder();
void genAlphabet(int ind);
void drawAlphabet(int ind);
void eraseAlphabet(int ind);
void resetAlphabet(int ind);
void gameover(string gameOverPhrase);
void updateScore();
void instructions();

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;
char keys[10];
int keyPos[10][2];
int score = 0;

void gotoxy(int x, int y)
{
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}

void setcursor(bool visible, DWORD size)
{
    if (size == 0)
        size = 20;

    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible;
    lpCursor.dwSize = size;
    SetConsoleCursorInfo(console, &lpCursor);
}

void drawBorder()
{
    for (int i = 0; i < SCREEN_WIDTH + 1; i++)
    {
        gotoxy(i, SCREEN_HEIGHT + 1);
        cout << "^";
    }

    for (int i = 0; i < SCREEN_HEIGHT + 1; i++)
    {
        gotoxy(0, i);
        cout << "|>";
        gotoxy(SCREEN_WIDTH, i);
        cout << "|";
    }
    for (int i = 0; i < SCREEN_HEIGHT + 1; i++)
    {
        gotoxy(WIN_WIDTH, i);
        cout << "<|";
    }
}

void genAlphabet(int ind)
{
    int randValue = rand() % 52;
    if (randValue < 26)
    {
        // Generate uppercase letters
        keys[ind] = 'A' + randValue;
    }
    else if (randValue < 36)
    {
        // Generate numbers
        keys[ind] = '0' + (randValue - 26);
    }
    else if (randValue < 46)
    {
        // Generate symbols
        keys[ind] = '!' + (randValue - 36);
    }
    else
    {
        // Generate shift symbols
        keys[ind] = ':' + (randValue - 46);
    }

    keyPos[ind][0] = 2 + rand() % (WIN_WIDTH - 2);
    keyPos[ind][1] = 1;
}

void drawAlphabet(int ind)
{
    if (keyPos[ind][0] != 0)
    {
        gotoxy(keyPos[ind][0], keyPos[ind][1]);
        cout << keys[ind];
    }
}

void eraseAlphabet(int ind)
{
    if (keyPos[ind][0] != 0)
    {
        gotoxy(keyPos[ind][0], keyPos[ind][1]);
        cout << " ";
    }
}

void resetAlphabet(int ind)
{
    eraseAlphabet(ind);
    genAlphabet(ind);
}

void gameover(string gameOverPhrase)
{
    system("cls");
    cout << endl;
    cout << "\t\t--------------------------" << endl;
    cout << "\t\t-------- Game Over -------" << endl;
    cout << "\t\t------Total Score: " << score << "-----" << endl;
    cout << "\t\t--------------------------" << endl
         << endl;
    cout << "\t\t" << gameOverPhrase;
    cout << "\n\n\t\tPlay Again? (y/n)";

    while (1)
    {
        char choice = getch();
        if (choice == 'y' || choice == 'Y')
        {
            system("cls");
            play();
            return;
        }
        else if (choice == 'n' || choice == 'N')
        {
            break;
        }
    }

    cout << "\n\t\tPress any key to go back to the menu.";
    getch();
}

void updateScore()
{
    gotoxy(WIN_WIDTH + 7, 5);
    cout << "Score: " << score << endl;
}

void instructions()
{
    system("cls");
    cout << "Instructions";
    cout << "\n----------------";
    cout << "\n On the left side, you will see falling characters."
         << "\n You have to keep them from touching the ground."
         << "\n Press the respective key from the keyboard to keep playing."
         << "\nThe letters aren't Case Sensitive."
         << "\n\n Press 'escape' to exit."
         << "\n\nPress any key to go back to the menu.";
    getch();
}

void play()
{
    score = 0;
    int difficulty = 0, sleepDuration;
    int wrongKeyPresses = 0; // Track the number of wrong key presses
    while (difficulty < 1 || difficulty > 4)
    {
        system("cls");
        cout << "Select difficulty level:" << endl;
        cout << "1. Easy" << endl;
        cout << "2. Medium" << endl;
        cout << "3. Hard" << endl;
        cout << "4. Expert" << endl;
        cout << "Enter your choice: ";
        cin >> difficulty;
        switch (difficulty)
        {
        case 1:
            sleepDuration = 700;
            break;
        case 2:
            sleepDuration = 500;
            break;
        case 3:
            sleepDuration = 300;
            break;
        case 4:
            sleepDuration = 200;
            break;

        default:
            break;
        }
    }
    system("cls");
    drawBorder();
    updateScore();

    for (int i = 0; i < 10; i++)
        genAlphabet(i);

    gotoxy(WIN_WIDTH + 5, 2);
    cout << "Typing Tutor";
    gotoxy(WIN_WIDTH + 6, 4);
    cout << "----------";
    gotoxy(WIN_WIDTH + 6, 6);
    cout << "----------";

    gotoxy(18, 5);
    cout << "Press any key to start";
    getch();
    gotoxy(18, 5);
    cout << "                      ";

    while (1)
    {
        if (kbhit())
        {
            char ch = getch();
            bool correctKeyPress = false; // Track if the pressed key is correct
            for (int i = 0; i < 10; i++)
            {
                if (ch == keys[i] || ch - 32 == keys[i])
                {
                    resetAlphabet(i);
                    score++;
                    updateScore();
                    correctKeyPress = true;
                }
            }
            if (ch == 27)
            {
                break;
            }
            else if (!correctKeyPress)
            {
                wrongKeyPresses++;
                gotoxy(SCREEN_WIDTH / 2, SCREEN_HEIGHT + 4);
                cout << "Warning!! You have pressed the wrong Key. " << 3 - wrongKeyPresses << " Lives Left" << endl;
                if (wrongKeyPresses == 3)
                {
                    gameover("You have pressed the wrong Key 3 times!!!");
                    return;
                }
            }
        }

        for (int i = 0; i < 10; i++)
            drawAlphabet(i);

        Sleep(sleepDuration);

        for (int i = 0; i < 10; i++)
        {
            eraseAlphabet(i);
            keyPos[i][1] += 1;
            if (keyPos[i][1] > SCREEN_HEIGHT)
            {
                gameover("The letter has touched the ground");
                return;
            }
        }

        if (wrongKeyPresses == 3)
            break;
    }
}

int main()
{
    setcursor(0, 0);
    srand((unsigned)time(NULL));
    int Difficulty = 1;

    do
    {
        system("cls");
        gotoxy(10, 5);
        cout << " -------------------------- ";
        gotoxy(10, 6);
        cout << " |-----TYPING  TUTOR------| ";
        gotoxy(10, 7);
        cout << " --------------------------";
        gotoxy(10, 9);
        cout << "1--> ----Start Game----";
        gotoxy(10, 10);
        cout << "2--> ---Instructions---";
        gotoxy(10, 11);
        cout << "3--> -------Quit-------";
        gotoxy(10, 13);
        cout << "Select an option: ";
        char op = getche();
        system("cls");
        if (op == '1')
        {
            play();
        }
        else if (op == '2')
        {
            instructions();
        }
        else if (op == '3')
        {
            exit(0);
        }
    } while (1);

    return 0;
}