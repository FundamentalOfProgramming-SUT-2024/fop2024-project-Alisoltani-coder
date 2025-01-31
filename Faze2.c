//speed magic food va speed enemy

//good music choice
//exit back
//menus
//unicodes
//scoreboard
//Ali Soltani 403106092
#pragma region includings
#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <locale.h>
#include <wchar.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#pragma endregion

#pragma region Structs

typedef struct Room
{
    int TopLeft_x;
    int TopLeft_Y;
    int Width;
    int Height;
    int middlex;
    int middley;
    bool show;
    int wdow;
} Room;

typedef struct trap
{
    int show;
    int x;
    int y;
} trap;

typedef struct door
{
    bool lock;
    int y;
    int x;
    int PassWord;
    int trysleft;
} door;

typedef struct Weapon
{
    int mode;
    int times_use_left;
} Weapon;

typedef struct Spell
{
    int mode;
} Spell;

typedef struct Corridor
{
    int x;
    int y;
} Corridor;

typedef struct Window
{
    int x;
    int y;
    int dir;
} Window;

typedef struct Food
{
    int type;
} Food;

typedef struct Enemy
{
    char type;
    double hp;
    int move;
    int x;
    int y;
}Enemy;

typedef struct Wasted_Weapon
{
    int x;
    int y;
}Wasted_Weapon;

#pragma endregion

#pragma region Globool
int rows=-1, cols=-1;
char Line1[200];
char Line2[200];
char Player_UserName[100];
int level = 1;
int color = 1;
int playerGOLD = 0;
int playerHP = 1000;
int playerHPMax = 1000;
int TillLast = 75;
int FoodEffect = 5;
int AncientKeys = 0;
int AncientKey[5];
int UsedKey = 0;
int WeaponCount = 1;
int SpellCount = 0;
int FoodCount = 0;
int EnemyCount = 0;
int MovesTillLastFood = 0;
double PlayerFood = 10;
int m = -1;
int onetime = 0;
double power = 1;
double speed = 0;
int Health_Pot = 0;
int MovesTillLastpot = 0;
char Current_Weapon = 'm';
int Wasted_Weapon_Count = 0;
Wasted_Weapon *Wasted_Weapon_Array;
Weapon *WeaponArray;
Spell *SpellArray;
Food *FoodArray;
Enemy *EnemyArray;
// O = Pillar
// G,g = Gold
// A = Ancientkey
//^ = Trap
//& = Generate pass
// 1-5 Weapon
// 6-8 Spell
// Z(z,x,c,v) food
//  gcc -o my_program my_program.c -lncursesw -lSDL2 -lSDL2_mixer
//./ali
//color();
// 1 Blue
// 2 Green
// 3 Cyan
// 4 Magenta
// 5 Red
// 6 White
// 7 Yellow


int y, x;
char **map;
int Room_Placed = 0;
int Trap_Placed = 0;
int Door_Placed = 0;
int Corridor_Placed = 0;
int Window_Placed = 0;
Room *RoomArray;
trap *TrapArray;
door *DoorArray;
Corridor *CorridorArray;
Window *WindowArray;
bool Player_Placed = false;
bool firsttime = true;
#pragma endregion

#pragma region Func_Prototype
void Rogue_Animation();
char LoginorRegisterPage();
void ForgotPassword(const char Username[100], const char text[100]);
void Login();
void Register();
void MainMenu();
void Settings();
int YesNoChoice();
void FoodChoice();
void SaveGame_StairCase();
void LoadGame_StairCase();
void MakeRoom();
int MakeGame(int c);
void NewGame();
void BeforeGame();
void play_music(const char *filename);
void AddtoScoreBoard();
void ScoreBoard();
#pragma endregion

Mix_Music *music;
void play_music(const char *filename)
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        fprintf(stderr, "Mix_OpenAudio: %s\n", Mix_GetError());
        exit(1);
    }

    music = Mix_LoadMUS(filename);
    if (music == NULL)
    {
        fprintf(stderr, "Mix_LoadMUS: %s\n", Mix_GetError());
        exit(1);
    }

    if (Mix_PlayMusic(music, -1) < 0)
    {
        fprintf(stderr, "Mix_PlayMusic: %s\n", Mix_GetError());
        exit(1);
    }
}

void Rogue_Animation()
{
    clear();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    int term_height, term_width;
    getmaxyx(stdscr, term_height, term_width);
    int start_y = (term_height - 18) / 2;
    start_y -= 5;
    int start_x = (term_width - 108) / 2;
    start_color();
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(start_y, start_x, "                                                                                                       \n");
    mvprintw(start_y + 1, start_x, "                                                                                                       \n");
    mvprintw(start_y + 2, start_x, "RRRRRRRRRRRRRRRRR        OOOOOOOOOO               GGGGGGGGGGGG UUUUUUU     UUUUUUU EEEEEEEEEEEEEEEEEEEEEE\n");
    mvprintw(start_y + 3, start_x, "R::::::::::::::::R     OO::::::::::OO          GGG:::::::::::G U:::::U     U:::::U E::::::::::::::::::::E\n");
    mvprintw(start_y + 4, start_x, "R::::::RRRRRR:::::R  OO::::::::::::::OO      GG::::::::::::::G U:::::U     U:::::U E::::::::::::::::::::E\n");
    mvprintw(start_y + 5, start_x, "RR:::::R     R:::::R O:::::::OOO:::::::O   G:::::GGGGGGGG::::G U:::::U     U:::::U EE::::::EEEEEEEEE::::E\n");
    mvprintw(start_y + 6, start_x, "  R::::R     R:::::R O::::::O   O::::::O G:::::G       GGGGGG  U:::::U     U:::::U   E:::::E       EEEEEE\n");
    mvprintw(start_y + 7, start_x, "  R::::R     R:::::R O:::::O     O:::::O G:::::G               U:::::U     U:::::U   E:::::E             \n");
    mvprintw(start_y + 8, start_x, "  R::::RRRRRR:::::R  O:::::O     O:::::O G:::::G               U:::::U     U:::::U   E::::::EEEEEEEEEE   \n");
    mvprintw(start_y + 9, start_x, "  R:::::::::::::RR   O:::::O     O:::::O G:::::G    GIGANIGGAG U:::::U     U:::::U   E:::::::::::::::E   \n");
    mvprintw(start_y + 10, start_x, "  R::::RRRRRR:::::R  O:::::O     O:::::O G:::::G    G::::::::G U:::::U     U:::::U   E:::::::::::::::E   \n");
    mvprintw(start_y + 11, start_x, "  R::::R     R:::::R O:::::O     O:::::O G:::::G    GGGGG::::G U:::::U     U:::::U   E::::::EEEEEEEEEE   \n");
    mvprintw(start_y + 12, start_x, "  R::::R     R:::::R O:::::O     O:::::O G:::::G        G::::G U:::::U     U:::::U   E:::::E             \n");
    mvprintw(start_y + 13, start_x, "  R::::R     R:::::R O::::::O   O::::::O  G:::::G       G::::G U::::::U   U::::::U   E:::::E       EEEEEE\n");
    mvprintw(start_y + 14, start_x, "RR:::::R     R:::::R O:::::::OOO:::::::O   G:::::GGGGGGGG::::G U:::::::UWU:::::::U EE::::::EEEEEEEE:::::E\n");
    mvprintw(start_y + 15, start_x, "R::::::R     R:::::R  OO:::::::::::::OO     GG:::::::::::::::G  UU:::::::::::::UU  E::::::::::::::::::::E\n");
    mvprintw(start_y + 16, start_x, "R::::::R     R:::::R    OO:::::::::OO         GGG::::::GGG:::G    UU:::::::::UU    E::::::::::::::::::::E\n");
    mvprintw(start_y + 17, start_x, "RRRRRRRR     RRRRRRR      OOOOOOOOO              GGGGGG   GGGG      UUUUUUUUU      EEEEEEEEEEEEEEEEEEEEEE\n");
    mvprintw(start_y + 18, start_x, "\n\n");
    attroff(COLOR_PAIR(2) | A_BOLD);
    attron(A_BLINK);
    mvprintw(start_y + 24, start_x, "                                    Press any key to continue! \n");
    attroff(A_BLINK);
    char c = getch();
    echo();
    curs_set(true);
}

char LoginorRegisterPage()
{
    printw("Do you want to login or register?\n\n");
    printw("L for login-R for register\n\n");
    char input;
    scanw("%c", &input);
    if (input == 'L' || input == 'l')
    {
        clear();
        return 'L';
    }
    else
    {
        if (input == 'R' || input == 'r')
        {
            clear();
            return 'R';
        }
        else
        {
            clear();
            printw("Wrong Input!\n");
            return 'E';
        }
    }
}

void ForgotPassword(const char Username[100], const char text[100])
{
    echo();
    curs_set(true);
    while (1)
    {
        printw("Password Regex: ");
        char text[100];
        char regex[100];
        scanw("%s", regex);
        int cursor_text = 0;
        int index$ = strlen(regex) - 1;
        for (int i = 1; i < index$; i++)
        {
            int check = 0;
            if ((regex[i + 1] == '+' || regex[i + 1] == '*' || regex[i + 1] == '?') && regex[i] != '\\')
            {
                check = 1;
            }
            if (check == 1)
            {
            }
            else
            {
                if ((regex[i] == text[cursor_text]) && regex[i] != '\\')
                {
                    // printf("ZZZ\n");
                    cursor_text++;
                }
                else
                {
                    if (regex[i] == '+')
                    {
                        int counter = 0;
                        // printf("jjjjjjjjjj%d %c %c\n",cursor_text,text[cursor_text],regex[i - 1]);
                        if (regex[i - 1] == text[cursor_text])
                        {
                            counter++;
                        }
                        if (counter == 0)
                        {
                            printf("No");
                            return;
                        }
                        else
                        {
                            while (regex[i - 1] == text[cursor_text])
                            {
                                cursor_text++;
                            }
                            continue;
                        }
                    }
                    else
                    {
                        if (regex[i] == '*')
                        {
                            while (regex[i - 1] == text[cursor_text])
                            {
                                cursor_text++;
                            }
                            continue;
                        }
                        else
                        {
                            if (regex[i] == '?')
                            {
                                if (regex[i - 1] == text[cursor_text])
                                {
                                    cursor_text++;
                                }
                                continue;
                            }
                            else
                            {
                                if (regex[i] == '\\' && regex[i + 1] == 'd')
                                {
                                    if (regex[i + 2] == '+')
                                    {
                                        int counter = 0;
                                        // printf("%d %c\n",cursor_text,text[cursor_text]);
                                        while (text[cursor_text] >= '0' && text[cursor_text] <= '9')
                                        {
                                            counter++;
                                            cursor_text++;
                                        }
                                        if (counter == 0)
                                        {
                                            // printf("hell naw");
                                            printf("No");
                                            return;
                                        }
                                        else
                                        {
                                            i += 2;
                                        }
                                        continue;
                                    }
                                    else
                                    {
                                        if (regex[i + 2] == '*')
                                        {
                                            // printf("%d %c\n",cursor_text,text[cursor_text]);
                                            while (text[cursor_text] >= '0' && text[cursor_text] <= '9')
                                            {
                                                cursor_text++;
                                            }
                                            i += 2;
                                            continue;
                                        }
                                        else
                                        {
                                            if (regex[i + 2] == '?')
                                            {
                                                // printf("%d %c\n",cursor_text,text[cursor_text]);
                                                if (text[cursor_text] >= '0' && text[cursor_text] <= '9')
                                                {
                                                    cursor_text++;
                                                }
                                                i += 2;
                                                continue;
                                            }
                                            else
                                            {
                                                if (text[cursor_text] >= '0' && text[cursor_text] <= '9')
                                                {
                                                    cursor_text++;
                                                    i++;
                                                }
                                                continue;
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    if (regex[i] == '\\')
                                    {
                                        if (regex[i + 2] == '+')
                                        {
                                            // printf("im in\n");
                                            int counter = 0;
                                            while (regex[i + 1] == text[cursor_text])
                                            {
                                                cursor_text++;
                                                counter++;
                                            }
                                            if (counter == 0)
                                            {
                                                // printf("hell naw");
                                                printf("No");
                                                return;
                                            }
                                            else
                                            {
                                                i += 2;
                                            }
                                            continue;
                                        }
                                        else
                                        {
                                            if (regex[i + 2] == '*')
                                            {
                                                while (regex[i + 1] == text[cursor_text])
                                                {
                                                    cursor_text++;
                                                }
                                                i += 2;
                                                continue;
                                            }
                                            else
                                            {
                                                if (regex[i + 2] == '?')
                                                {
                                                    if (regex[i + 1] == text[cursor_text])
                                                    {
                                                        cursor_text++;
                                                    }
                                                    i += 2;
                                                }
                                                else
                                                {
                                                    if (regex[i + 1] == text[cursor_text])
                                                    {
                                                        i++;
                                                        cursor_text++;
                                                    }
                                                    continue;
                                                }
                                            }
                                        }
                                    }
                                    else
                                    {
                                        // printf("really nigga?");
                                        printf("No");
                                        return;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        if (cursor_text - 1 >= strlen(text) - 1)
        {
            clear();
            printw("Sucsessfully logged in as %s\n", Username);
            noecho();
            curs_set(false);
            return;
        }
    }
}

void Login()
{
    char Username[100];
    char Password[100];
    char tryuser[100];
    char trypassword[100];
    int valid = 0;
    char input[100];
    while (valid == 0)
    {
        FILE *pf = fopen("Users.txt", "r");
        printw("\nUsername: ");
        scanw("%s", Username);
        printw("Password: ");
        scanw("%s", Password);
        while (fscanf(pf, "%s", input) != EOF)
        {
            char realUsername[100];
            char realPassword[100];
            int idxuser = 0;
            int idxpass = 0;
            int i;
            for (i = 0; input[i] != '-'; i++)
            {
                realUsername[idxuser] = input[i];
                idxuser++;
            }
            for (int j = i + 1; input[j] != '-'; j++)
            {
                realPassword[idxpass] = input[j];
                idxpass++;
            }
            realUsername[idxuser] = '\0';
            realPassword[idxpass] = '\0';
            if ((strcmp(realUsername, Username) == 0) && (strcmp(realPassword, Password)) == 0)
            {
                valid = 1;
                break;
            }
            else
            {
                if ((strcmp(realUsername, Username) == 0))
                {
                    strcpy(tryuser, Username);
                    strcpy(trypassword, Password);
                }
            }
        }
        if (valid == 0)
        {
            printw("Username or Password is wrong!\n\n");
            // mohit
            int cursor = 0;
            int c;
            noecho();
            curs_set(0);
            keypad(stdscr, TRUE);
            int u = 0;
            int curenty, curentx;
            getyx(stdscr, curenty, curentx);
            do
            {
                move(10, 0);
                clrtoeol();
                move(12, 0);
                clrtoeol();
                // move(y,x);
                // clear();
                // refresh();
                attron(A_BOLD);
                mvprintw(5, 0, "Forgot Password?\n\n");
                attroff(A_BOLD);
                curenty++;
                if (cursor == 0)
                {
                    mvprintw(10, 3, "No\n");
                }
                else
                {
                    mvprintw(10, 0, "No\n");
                }
                curenty++;
                if (cursor == 1)
                {
                    mvprintw(12, 3, "Yes\n");
                }
                else
                {
                    mvprintw(12, 0, "Yes\n");
                }

                c = getch();

                if (c == KEY_UP)
                {
                    cursor--;
                    if (cursor < 0)
                        cursor = 1;
                }

                if (c == KEY_DOWN)
                {
                    cursor++;
                    if (cursor > 1)
                        cursor = 0;
                }

            } while (c != 10);
            curs_set(1);
            echo();
            // end mohit
            if (cursor == 0)
            {
                clear();
                continue;
            }
            else
            {
                ForgotPassword(tryuser, trypassword);
                break;
            }
        }
        else
        {
            clear();
            printw("Sucsessfully logged in as %s\n", Username);
            strcpy(Player_UserName,Username);
            break;
        }
        fclose(pf);
    }
}

void Register()
{
    char Username[100];
    char Password[100];
    char Password_copy[100];
    char Email[100];
    int length = 0;
    int number = 0;
    int lower = 0;
    int upper = 0;
    int same = 0;
    int emailemail = 0;
    printw("Fill the Following information to Register\n\n");
    printw("Your password should match these conditions: \n\n");
    printw("1-Password should be at-least 7 characters\n\n");
    printw("2-Password should have a lower-case alphabet\n and an upper-case alphabet and at-least one number\n\n");
    int validation = 0;
    while (validation == 0)
    {
        printw("Username: ");
        scanw("%s", Username);
        char user[100];
        FILE *pf1 = fopen("Users.txt", "r");

        while (fscanf(pf1, "%s", user) != EOF)
        {
            char realuser[100] = {0};
            int idx = 0;
            for (int i = 0; user[i] != '-'; i++)
            {
                realuser[idx] = user[i];
                idx++;
            }
            realuser[idx] = '\0';
            // printw("%s\n",realuser);
            if (strcmp(realuser, Username) == 0)
            {
                printw("Username already exists!\n");
                validation = -1;
                break;
            }
        }

        if (validation == 0)
        {
            validation = 1;
        }
        else
        {
            validation = 0;
        }
        fclose(pf1);
    }
    strcpy(Player_UserName,Username);
    int valid = 0;
    int valid2 = 0;
    while (valid == 0)
    {
        char choose;
        printw("Do you want an automatically generated password?(Y for yes)(N for no)\n");
        scanw("%c", &choose);
        if (choose == 'Y' || choose == 'y')
        {
            srand(time(0));

            // making random pass
            int countnumbers = (rand() % 4) + 7;
            int BigChar = 0;
            int SmallChar = 0;
            int Number1 = 0;
            int condition = 0;
            while (condition == 0)
            {
                for (int ij = 0; ij < countnumbers; ij++)
                {
                    int whichone = rand() % 3;
                    if (whichone == 0)
                    {
                        Password[ij] = (char)((rand() % 26) + 65);
                        BigChar++;
                    }
                    if (whichone == 1)
                    {
                        Password[ij] = (char)((rand() % 26) + 97);
                        SmallChar++;
                    }
                    if (whichone == 2)
                    {
                        Password[ij] = (char)((rand() % 10) + 48);
                        Number1++;
                    }
                }
                if (Number1 > 0 && SmallChar > 0 && BigChar > 0)
                {
                    Password[countnumbers] = '\0';
                    condition = 1;
                }
            }

            printw("Your password is %s\n", Password);
            valid = 1;
            valid2 = 1;
        }
        else
        {
            if (choose == 'N' || choose == 'n')
            {
                printw("Password: ");
                scanw("%s", Password);
                if (strlen(Password) >= 7)
                {
                    length = 1;
                }
                else
                {
                    length = 0;
                }
                for (int i = 0; i < strlen(Password); i++)
                {
                    if (Password[i] >= '0' && Password[i] <= '9')
                    {
                        number++;
                    }
                    if (Password[i] >= 'A' && Password[i] <= 'Z')
                    {
                        upper++;
                    }
                    if (Password[i] >= 'a' && Password[i] <= 'z')
                    {
                        lower++;
                    }
                }
                if (length * upper * lower * number > 0)
                {
                    valid = 1;
                }
                else
                {
                    printw("Invalid password!\n");
                }
            }
            else
            {
                clear();
                printw("Wrong input!\n");
                Register();
                break;
            }
        }
    }

    while (valid2 == 0)
    {
        printw("Password Again: ");
        scanw("%s", Password_copy);
        if (strcmp(Password, Password_copy) == 0)
        {
            valid2 = 1;
        }
        else
        {
            printw("This is not the same password!\n");
        }
    }
    emailemail = 0;
    while (emailemail == 0)
    {
        printw("Email: ");
        scanw("%s", Email);
        int indexatsine;
        int indexdot;
        int countatsign = 0;
        int countdot = 0;
        for (int i = 0; i < strlen(Email); i++)
        {
            if (Email[i] == '@')
            {
                indexatsine = i;
                countatsign++;
            }
            if (Email[i] == '.')
            {
                indexdot = i;
                countdot++;
            }
        }
        if (indexdot - indexatsine < 3 || countatsign != 1 || countdot != 1)
        {
            printw("Please write a valid Email!\n");
        }
        else
        {
            emailemail = 1;
        }
    }
    FILE *pf = fopen("Users.txt", "a");
    fprintf(pf, "%s-%s-%s\n", Username, Password, Email);
    fclose(pf);
    clear();
    printw("You have succesfully made your account named %s!\n", Username);
}

void MainMenu()
{
    int cursor = 0;
    int c;
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    int colss = (getmaxx(stdscr) / 2);
    colss -= 5;
    int u = 0;
    do
    {
        clear();
        refresh();
        attron(A_BOLD);
        mvprintw(5, colss - 1, "MAIN MENU");
        attroff(A_BOLD);

        if (cursor == 0)
        {
            mvprintw(9, colss, "* ");
            attron(COLOR_PAIR(7) | A_BOLD);
            mvprintw(9, colss + 4, "Register\n");
            attroff(COLOR_PAIR(7) | A_BOLD);
        }
        else
        {
            mvprintw(9, colss, "* Register\n");
        }

        if (cursor == 1)
        {
            mvprintw(12, colss, "*  ");
            attron(COLOR_PAIR(7) | A_BOLD);
            mvprintw(12, colss + 4, "Login\n");
            attroff(COLOR_PAIR(7) | A_BOLD);
        }
        else
        {
            mvprintw(12, colss, "* Login\n");
        }

        if (cursor == 2)
        {
            mvprintw(15, colss, "*  ");
            attron(COLOR_PAIR(7) | A_BOLD);
            mvprintw(15, colss + 4, "Guest\n");
            attroff(COLOR_PAIR(7) | A_BOLD);
        }
        else
        {
            mvprintw(15, colss, "* Guest\n");
        }

        if (cursor == 3)
        {
            mvprintw(18, colss, "*  ");
            attron(COLOR_PAIR(7) | A_BOLD);
            mvprintw(18, colss + 4, "Exit\n");
            attroff(COLOR_PAIR(7) | A_BOLD);
        }
        else
        {
            mvprintw(18, colss, "*  Exit\n");
        }

        c = getch();

        if (c == KEY_UP)
        {
            cursor--;
            if (cursor < 0)
                cursor = 3;
        }

        if (c == KEY_DOWN)
        {
            cursor++;
            if (cursor > 3)
                cursor = 0;
        }

    } while (c != 10);
    curs_set(1);
    echo();
    if (cursor == 0)
    {
        clear();
        Register();
    }
    if (cursor == 1)
    {
        clear();
        Login();
    }
    if (cursor == 2)
    {
        clear();
        char username[100];
        printw("Username: ");
        scanw("%s", username);
        clear();
        printw("Wellcome! %s\n", username);
        strcpy(Player_UserName,username);
    }
    if (cursor == 3)
    {
        clear();
        endwin();
        exit(0);
    }
}

void Colours()
{
    int cursor = 0;
    int c;
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    int colss = (getmaxx(stdscr) / 2);
    colss -= 10;
    do
    {
        clear();
        refresh();
        attron(A_BOLD);
        mvprintw(5, colss - 1, "Please choose Color");
        attroff(A_BOLD);

        if (cursor == 0)
        {
            wchar_t message[] = L"₪ ";
            mvprintw(9, colss, "%ls", message);
            attron(COLOR_PAIR(5) | A_BOLD);
            mvprintw(9, colss + 4, "Red");
            attroff(COLOR_PAIR(5) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"₪ ";
            mvprintw(9, colss, "%ls Red\n", message);
        }

        if (cursor == 1)
        {
            wchar_t message[] = L"₪ ";
            mvprintw(12, colss, "%ls", message);
            attron(COLOR_PAIR(1) | A_BOLD);
            mvprintw(12, colss + 4, "Blue");
            attroff(COLOR_PAIR(1) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"₪ ";
            mvprintw(12, colss, "%ls Blue\n", message);
        }

        if (cursor == 2)
        {
            wchar_t message[] = L"₪ ";
            mvprintw(15, colss, "%ls", message);
            attron(COLOR_PAIR(2) | A_BOLD);
            mvprintw(15, colss + 4, "Green");
            attroff(COLOR_PAIR(2) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"₪ ";
            mvprintw(15, colss, "%ls Green\n", message);
        }

        if (cursor == 3)
        {
            wchar_t message[] = L"₪ ";
            mvprintw(18, colss, "%ls", message);
            attron(COLOR_PAIR(7) | A_BOLD);
            mvprintw(18, colss + 4, "Yellow");
            attroff(COLOR_PAIR(7) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"₪ ";
            mvprintw(18, colss, "%ls Yellow\n", message);
        }

        if (cursor == 4)
        {
            wchar_t message[] = L"₪ ";
            mvprintw(21, colss, "%ls", message);
            attron(COLOR_PAIR(4) | A_BOLD);
            mvprintw(21, colss + 4, "Magenta");
            attroff(COLOR_PAIR(4) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"₪ ";
            mvprintw(21, colss, "%ls Magenta\n", message);
        }

        if (cursor == 5)
        {
            wchar_t message[] = L"₪ ";
            mvprintw(24, colss, "%ls", message);
            attron(COLOR_PAIR(3) | A_BOLD);
            mvprintw(24, colss + 4, "Cyan");
            attroff(COLOR_PAIR(3) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"₪ ";
            mvprintw(24, colss, "%ls Cyan\n", message);
        }

        c = getch();

        if (c == KEY_UP)
        {
            cursor--;
            if (cursor < 0)
                cursor = 5;
        }

        if (c == KEY_DOWN)
        {
            cursor++;
            if (cursor > 5)
                cursor = 0;
        }

    } while (c != 10);
    curs_set(1);
    echo();
    color = cursor;
    clear();
    Settings();
}

void Song()
{
    clear();
    int c;
    int u = 0;
    int cursor = 0;
    do
    {
        clear();
        attron(A_BOLD);
        mvprintw(5, (cols/2)-5, "Songs");
        attroff(A_BOLD);
        if (cursor == 0)
        {
            attron(COLOR_PAIR(7));
            mvprintw(8, (cols/2)-3, "Ksi\n");
            attroff(COLOR_PAIR(7));
        }
        else
        {
            mvprintw(8, (cols/2)-5, "Ksi\n");
        }
        if (cursor == 1)
        {
            attron(COLOR_PAIR(7));
            mvprintw(10, (cols/2)-3, "Pastol\n");
            attroff(COLOR_PAIR(7));
        }
        else
        {
            mvprintw(10, (cols/2)-5, "Pastol\n");
        }
        c = getch();

        if (c == KEY_UP)
        {
            cursor--;
            if (cursor < 0)
                cursor = 1;
        }

        if (c == KEY_DOWN)
        {
            cursor++;
            if (cursor > 1)
                cursor = 0;
        }
    } while (c != 10);
    clear();
    if (cursor == 0)
    {
        play_music("1.mp3");
    }
    if (cursor == 1)
    {
        play_music("2.mp3");
    }
    clear();
    Settings();
}

void Difficulty()
{
    clear();
    int c;
    int u = 0;
    int cursor = 0;
    do
    {
        clear();
        attron(A_BOLD);
        mvprintw(5, (cols/2)-5, "Difficulty");
        attroff(A_BOLD);
        if (cursor == 0)
        {
            attron(COLOR_PAIR(7));
            mvprintw(8, (cols/2)-3, "Peaceful\n");
            attroff(COLOR_PAIR(7));
        }
        else
        {
            mvprintw(8, (cols/2)-5, "Peaceful\n");
        }
        if (cursor == 1)
        {
            attron(COLOR_PAIR(7));
            mvprintw(10, (cols/2)-3, "Easy\n");
            attroff(COLOR_PAIR(7));
        }
        else
        {
            mvprintw(10, (cols/2)-5, "Easy\n");
        }
        if (cursor == 2)
        {
            attron(COLOR_PAIR(7));
            mvprintw(12, (cols/2)-3, "Medium\n");
            attroff(COLOR_PAIR(7));
        }
        else
        {
            mvprintw(12, (cols/2)-5, "Medium\n");
        }
        if (cursor == 3)
        {
            attron(COLOR_PAIR(7));
            mvprintw(14, (cols/2)-3, "Hard\n");
            attroff(COLOR_PAIR(7));
        }
        else
        {
            mvprintw(14, (cols/2)-5, "Hard\n");
        }
        c = getch();

        if (c == KEY_UP)
        {
            cursor--;
            if (cursor < 0)
                cursor = 3;
        }

        if (c == KEY_DOWN)
        {
            cursor++;
            if (cursor > 3)
                cursor = 0;
        }
    } while (c != 10);
    clear();
    if (cursor == 0)
    {
        TillLast = 100;
    }
    if (cursor == 1)
    {
        TillLast = 75;
    }
    if (cursor == 2)
    {
        TillLast = 60;
    }
    if (cursor == 3)
    {
        TillLast = 40;
    }
    clear();
    Settings();
}

void Settings()
{
    clear();
    int c;
    int u = 0;
    int cursor = 0;
    do
    {
        clear();
        attron(A_BOLD);
        mvprintw(5, (cols/2)-5, "Settings");
        attroff(A_BOLD);
        if (cursor == 0)
        {
            attron(COLOR_PAIR(7));
            mvprintw(8, (cols/2)-3, "Colors\n");
            attroff(COLOR_PAIR(7));
        }
        else
        {
            mvprintw(8, (cols/2)-5, "Colors\n");
        }
        if (cursor == 1)
        {
            attron(COLOR_PAIR(7));
            mvprintw(10, (cols/2)-3, "Difficulty\n");
            attroff(COLOR_PAIR(7));
        }
        else
        {
            mvprintw(10, (cols/2)-5, "Difficulty\n");
        }
        if (cursor == 2)
        {
            attron(COLOR_PAIR(7));
            mvprintw(12, (cols/2)-3, "Song\n");
            attroff(COLOR_PAIR(7));
        }
        else
        {
            mvprintw(12, (cols/2)-5, "Song\n");
        }
        if (cursor == 3)
        {
            attron(COLOR_PAIR(7));
            mvprintw(14, (cols/2)-3, "Back\n");
            attroff(COLOR_PAIR(7));
        }
        else
        {
            mvprintw(14, (cols/2)-5, "Back\n");
        }
        c = getch();

        if (c == KEY_UP)
        {
            cursor--;
            if (cursor < 0)
                cursor = 3;
        }

        if (c == KEY_DOWN)
        {
            cursor++;
            if (cursor > 3)
                cursor = 0;
        }
    } while (c != 10);
    clear();
    if (cursor == 0)
    {
        Colours();
    }
    if (cursor == 1)
    {
        Difficulty();
    }
    if (cursor == 2)
    {
        Song();
    }
    if (cursor == 3)
    {
        BeforeGame();
    }
}

int YesNoChoice()
{
    clear();
    int c;
    int u = 0;
    int cursor = 0;
    do
    {
        clear();
        attron(A_BOLD);
        mvprintw(0, 0, "Pick up item?");
        attroff(A_BOLD);
        attron(COLOR_PAIR(4));
        if (cursor == 0)
        {

            mvprintw(3, 1, "No\n");
        }
        else
        {
            mvprintw(3, 0, "No\n");
        }
        if (cursor == 1)
        {
            mvprintw(5, 1, "Yes\n");
        }
        else
        {
            mvprintw(5, 0, "Yes\n");
        }

        c = getch();

        if (c == KEY_UP)
        {
            cursor--;
            if (cursor < 0)
                cursor = 1;
        }

        if (c == KEY_DOWN)
        {
            cursor++;
            if (cursor > 1)
                cursor = 0;
        }
        attroff(COLOR_PAIR(4));
    } while (c != 10);
    clear();
    attroff(COLOR_PAIR(4));
    if (cursor == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void FoodChoice()
{
    int normalfood = 0;
    int goodfood = 0;
    int magicfood = 0;
    int badfood = 0;
    for (int i = 0; i < FoodCount; i++)
    {
        if (FoodArray[i].type == 0)
        {
            normalfood++;
        }
        if (FoodArray[i].type == 1)
        {
            goodfood++;
        }
        if (FoodArray[i].type == 2)
        {
            magicfood++;
        }
        if (FoodArray[i].type == 3)
        {
            badfood++;
        }
    }
    clear();
    mvprintw(0, 0, "Normal Food and Bad food: %d", normalfood + badfood);
    mvprintw(1, 0, "Good Food : %d", goodfood);
    mvprintw(2, 0, "Magic Food : %d", magicfood);
    attron(A_BLINK | A_BOLD);
    printw("\nPress Anything to Continue!");
    attroff(A_BLINK | A_BOLD);
    char cc = getch();
    clear();
    int c;
    int u = 0;
    int cursor = 0;
    int valid = 0;
    do
    {
        valid = 0;
        clear();
        attron(A_BOLD | A_BLINK);
        mvprintw(0, 0, "Which type of food?");
        attroff(A_BOLD| A_BLINK);
        attron(COLOR_PAIR(3));
        if (cursor == 0)
        {

            mvprintw(3, 3, "Normal and Bad Food\n");
        }
        else
        {
            mvprintw(3, 0, "Normal and Bad Food\n");
        }
        if (cursor == 1)
        {
            mvprintw(5, 3, "Good Food\n");
        }
        else
        {
            mvprintw(5, 0, "Good Food\n");
        }
        if (cursor == 2)
        {
            mvprintw(7, 3, "Magic Food\n");
        }
        else
        {
            mvprintw(7, 0, "Magic Food\n");
        }
        if (cursor == 3)
        {
            mvprintw(11, 3, "None\n");
        }
        else
        {
            mvprintw(11, 0, "None\n");
        }
        c = getch();

        if (c == KEY_UP)
        {
            cursor--;
            if (cursor < 0)
                cursor = 3;
        }

        if (c == KEY_DOWN)
        {
            cursor++;
            if (cursor > 3)
                cursor = 0;
        }
        attroff(COLOR_PAIR(3));
        if (cursor == 0 && normalfood + badfood > 0)
        {
            valid = 1;
        }
        if (cursor == 1 && goodfood > 0)
        {
            valid = 1;
        }
        if (cursor == 2 && magicfood > 0)
        {
            valid = 1;
        }
        if (cursor == 3)
        {
            valid = 1;
        }
    } while (c != 10 || valid <= 0);
    clear();
    attroff(COLOR_PAIR(3));
    // normal and bad food
    if (cursor == 0)
    {
        if (badfood == 0)
        {
            for (int i = 0; i < FoodCount; i++)
            {
                if (FoodArray[i].type == 0)
                {
                    FoodArray[i].type = -1;
                    break;
                }
            }
            MovesTillLastFood = 0;
            if (PlayerFood + FoodEffect > 10)
            {
                PlayerFood = 10;
            }
            else
            {
                PlayerFood += FoodEffect;
            }
        }
        else
        {
            if(normalfood == 0)
            {
                for (int i = 0; i < FoodCount; i++)
                {
                    if (FoodArray[i].type == 3)
                    {
                        FoodArray[i].type = -1;
                        break;
                    }
                }
                if (PlayerFood - FoodEffect < 0)
                {
                    PlayerFood = 0;
                }
                else
                {
                    PlayerFood -= FoodEffect;
                }
            }
            else
            {
                int ranfood = rand() % 4;
                if (ranfood != 0)
                {
                    for (int i = 0; i < FoodCount; i++)
                    {
                        if (FoodArray[i].type == 0)
                        {
                            FoodArray[i].type = -1;
                            break;
                        }
                    }
                    MovesTillLastFood = 0;
                    if (PlayerFood + FoodEffect > 10)
                    {
                        PlayerFood = 10;
                    }
                    else
                    {
                        PlayerFood += FoodEffect;
                    }
                }
                else
                {
                    for (int i = 0; i < FoodCount; i++)
                    {
                        if (FoodArray[i].type == 3)
                        {
                            FoodArray[i].type = -1;
                            break;
                        }
                    }
                    if (PlayerFood - FoodEffect < 0)
                    {
                        PlayerFood = 0;
                    }
                    else
                    {
                        PlayerFood -= FoodEffect;
                    }
                }
            }
        }
    }
    // good food
    if (cursor == 1)
    {
        power += 0.5;
        MovesTillLastFood = 0;
        for(int i = 0 ; i < FoodCount;i++)
        {
            if(FoodArray[i].type == 1)
            {
                FoodArray[i].type = -1;
                break;
            }
        }
        if (PlayerFood + FoodEffect > 10)
        {
            PlayerFood = 10;
        }
        else
        {
            PlayerFood += FoodEffect;
        }
    }
    // magic food
    if (cursor == 2)
    {
        MovesTillLastFood = 0;
        speed = 2;
        for(int i = 0 ; i < FoodCount;i++)
        {
            if(FoodArray[i].type == 2)
            {
                FoodArray[i].type = -1;
                break;
            }
        }
        if (PlayerFood + FoodEffect > 10)
        {
            PlayerFood = 10;
        }
        else
        {
            PlayerFood += FoodEffect;
        }
    }
    //none
    if (cursor == 3)
    {
        
    }
}

void SpellChoice()
{
    int health = 0;
    int speed1 = 0;
    int damage = 0;
    for (int i = 0; i < SpellCount; i++)
    {
        if (SpellArray[i].mode == 6)
        {
            health++;
        }
        if (SpellArray[i].mode == 7)
        {
            speed1++;
        }
        if (SpellArray[i].mode == 8)
        {
            damage++;
        }
    }
    clear();
    mvprintw(0, 0, "Health : %d", health);
    mvprintw(1, 0, "Speed : %d", speed1);
    mvprintw(2, 0, "Damage : %d", damage);
    attron(A_BLINK);
    mvprintw(5, 0, "Press any key to continue!");
    attroff(A_BLINK);
    char cc = getch();
    clear();
    //
    int c;
    int u = 0;
    int cursor = 0;
    int valid = 0;
    do
    {
        valid = 0;
        clear();
        attron(A_BOLD | A_BLINK);
        mvprintw(0, 0, "Which type of Spell?");
        attroff(A_BOLD| A_BLINK);
        attron(COLOR_PAIR(3));
        if (cursor == 0)
        {
            mvprintw(3, 3, "Health\n");
        }
        else
        {
            mvprintw(3, 0, "Health\n");
        }
        if (cursor == 1)
        {
            mvprintw(5, 3, "Speed\n");
        }
        else
        {
            mvprintw(5, 0, "Speed\n");
        }
        if (cursor == 2)
        {
            mvprintw(7, 3, "Damage\n");
        }
        else
        {
            mvprintw(7, 0, "Damage\n");
        }
        if (cursor == 3)
        {
            mvprintw(11, 3, "None\n");
        }
        else
        {
            mvprintw(11, 0, "None\n");
        }
        c = getch();

        if (c == KEY_UP)
        {
            cursor--;
            if (cursor < 0)
                cursor = 3;
        }

        if (c == KEY_DOWN)
        {
            cursor++;
            if (cursor > 3)
                cursor = 0;
        }
        attroff(COLOR_PAIR(3));
        if (cursor == 0 && health > 0)
        {
            valid = 1;
        }
        if (cursor == 1 && speed1 > 0)
        {
            valid = 1;
        }
        if (cursor == 2 && damage > 0)
        {
            valid = 1;
        }
        if (cursor == 3)
        {
            valid = 1;
        }
    } while (c != 10 || valid <= 0);
    clear();
    attroff(COLOR_PAIR(3));
    // Health
    if (cursor == 0)
    {
        MovesTillLastpot += 10;
        for (int i = 0; i < SpellCount; i++)
        {
            if (SpellArray[i].mode == 6)
            {
                SpellArray[i].mode = -1;
                break;
            }
        }
        Health_Pot = 100;
    }
    // Speed
    if (cursor == 1)
    {
        speed = 2;
        for(int i = 0 ; i < SpellCount;i++)
        {
            if(SpellArray[i].mode == 7)
            {
                SpellArray[i].mode = -1;
                break;
            }
        }
    }
    // Damage
    if (cursor == 2)
    {
        power += 0.5;
        for(int i = 0 ; i < SpellCount;i++)
        {
            if(SpellArray[i].mode == 8)
            {
                SpellArray[i].mode = -1;
                break;
            }
        }
    }
    //none
    if (cursor == 3)
    {

    }
}

void WeaponChoice()
{
    int mace = 0;
    int dagger = 0;
    int magicwand = 0;
    int normalarrow = 0;
    int sword = 0;
    for (int i = 0; i < WeaponCount; i++)
    {
        if (WeaponArray[i].mode == 1)
        {
            mace++;
        }
        if (WeaponArray[i].mode == 2)
        {
            dagger += WeaponArray[i].times_use_left;
        }
        if (WeaponArray[i].mode == 3)
        {
            magicwand += WeaponArray[i].times_use_left;
        }
        if (WeaponArray[i].mode == 4)
        {
            normalarrow += WeaponArray[i].times_use_left;
        }
        if (WeaponArray[i].mode == 5)
        {
            sword++;
        }
    }
    /*attron(A_BLINK);
    mvprintw(7, 0, "Press any key to continue!");
    attroff(A_BLINK);
    char cc = getch();
    clear();*/
    //
    int c;
    int u = 0;
    int cursor = 0;
    do
    {
        clear();
        mvprintw(0, 0, "Mace : %d CloseRange", mace);
        mvprintw(1, 0, "Dagger : %d FarRange", dagger);
        mvprintw(2, 0, "Magic Wand : %d FarRange", magicwand);
        mvprintw(3, 0, "Normal Arrow : %d FarRange", normalarrow);
        mvprintw(4, 0, "Sword : %d CloseRange", sword);
        if (cursor == 0)
        {
            mvprintw(7, 1, "Back to Game\n");
        }
        else
        {
            mvprintw(7, 0, "Back to Game\n");
        }
        if (cursor == 1)
        {
            mvprintw(9, 1, "Change Weapon\n");
        }
        else
        {
            mvprintw(9, 0, "Change Weapon\n");
        }

        c = getch();

        if (c == KEY_UP)
        {
            cursor--;
            if (cursor < 0)
                cursor = 1;
        }

        if (c == KEY_DOWN)
        {
            cursor++;
            if (cursor > 1)
                cursor = 0;
        }
    } while (c != 10);
    clear();
    if (cursor == 0)
    {
        return;
    }
    else
    {
        clear();
        //
        int cc;
        int uu = 0;
        int cursorr = 0;
        int validd = 0;
        char curw[100];
        if (Current_Weapon == 'm')
        {
            strcpy(curw, "Mace");
        }
        if (Current_Weapon == 'D')
        {
            strcpy(curw, "Dagger");
        }
        if (Current_Weapon == 'M')
        {
            strcpy(curw, "Magic Wand");
        }
        if (Current_Weapon == 'N')
        {
            strcpy(curw, "Normal Arrow");
        }
        if (Current_Weapon == 'S')
        {
            strcpy(curw, "Sword");
        }
        do
        {
            validd = 0;
            clear();
            attron(A_BOLD | A_BLINK);
            mvprintw(0, 0, "You are Currently holding %s",curw);
            attroff(A_BOLD| A_BLINK);
            if (cursorr == 0)
            {
                mvprintw(3, 3, "Mace\n");
            }
            else
            {
                mvprintw(3, 0, "Mace\n");
            }
            if (cursorr == 1)
            {
                mvprintw(5, 3, "Dagger\n");
            }
            else
            {
                mvprintw(5, 0, "Dagger\n");
            }
            if (cursorr == 2)
            {
                mvprintw(7, 3, "Magic Wand\n");
            }
            else
            {
                mvprintw(7, 0, "Magic Wand\n");
            }
            if (cursorr == 3)
            {
                mvprintw(9, 3, "Normal Arrow\n");
            }
            else
            {
                mvprintw(9, 0, "Normal Arrow\n");
            }
            if (cursorr == 4)
            {
                mvprintw(11, 3, "Sword\n");
            }
            else
            {
                mvprintw(11, 0, "Sword\n");
            }
            if (cursorr == 5)
            {
                mvprintw(15, 3, "Back\n");
            }
            else
            {
                mvprintw(15, 0, "Back\n");
            }
            cc = getch();

            if (cc == KEY_UP)
            {
                cursorr--;
                if (cursorr < 0)
                    cursorr = 5;
            }

            if (cc == KEY_DOWN)
            {
                cursorr++;
                if (cursorr > 5)
                    cursorr = 0;
            }
            if (cursorr == 0 && mace > 0)
            {
                validd = 1;
            }
            if (cursorr == 1 && dagger > 0)
            {
                validd = 1;
            }
            if (cursorr == 2 && magicwand > 0)
            {
                validd = 1;
            }
            if (cursorr == 3 && normalarrow > 0)
            {
                validd = 1;
            }
            if (cursorr == 4 && sword > 0)
            {
                validd = 1;
            }
            if (cursorr == 5)
            {
                validd = 1;
            }
        } while (cc != 10 || validd <= 0);
        clear();
        // Mace
        if (cursorr == 0)
        {
            Current_Weapon = 'm';
        }
        // Dagger
        if (cursorr == 1)
        {
            Current_Weapon = 'D';
        }
        // Magic Wand
        if (cursorr == 2)
        {
            Current_Weapon = 'M';
        }
        // Normal Arrow
        if (cursorr == 3)
        {
            Current_Weapon = 'N';
        }
        // Sword
        if (cursorr == 4)
        {
            Current_Weapon = 'S';
        }
        //Back
        if (cursorr == 5)
        {

        }
    }
}

void SaveGame_StairCase()
{
    // Generate the filename
    char filename[256];
    snprintf(filename, sizeof(filename), "%s_%d.txt", Player_UserName, level);
    FILE *file = fopen(filename, "w");
    fprintf(file, "%d\n",y);
    fprintf(file, "%d\n",x);
    fprintf(file, "%d\n",rows);
    fprintf(file, "%d\n",cols);
    //Map
    for (int i = 0; i < rows; i++)
    {
        fprintf(file, "%s\n", map[i]);
    }
    fclose(file);
    //Room
    snprintf(filename, sizeof(filename), "%s_%d_Room.txt", Player_UserName, level);
    FILE *file1 = fopen(filename, "w");
    fprintf(file1, "%d\n", Room_Placed);
    for (int i = 0; i < Room_Placed; i++)
    {
        fprintf(file1, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",RoomArray[i].TopLeft_Y,RoomArray[i].TopLeft_x,
        RoomArray[i].Width,RoomArray[i].Height,RoomArray[i].middlex,RoomArray[i].middley,
        RoomArray[i].show,RoomArray[i].wdow);
    }
    fclose(file1);
    //Trap
    snprintf(filename, sizeof(filename), "%s_%d_Trap.txt", Player_UserName, level);
    FILE *file2 = fopen(filename, "w");
    fprintf(file2, "%d\n", Trap_Placed);
    for (int i = 0; i < Trap_Placed; i++)
    {
        fprintf(file2, "%d\n%d\n%d\n",TrapArray[i].show,TrapArray[i].x,TrapArray[i].y);
    }
    fclose(file2);
    //Door
    snprintf(filename, sizeof(filename), "%s_%d_Door.txt", Player_UserName, level);
    FILE *file3 = fopen(filename, "w");
    fprintf(file3, "%d\n", Door_Placed);
    for (int i = 0; i < Door_Placed; i++)
    {
        fprintf(file3, "%d\n%d\n%d\n%d\n%d\n",DoorArray[i].lock,DoorArray[i].y,
        DoorArray[i].x,DoorArray[i].PassWord,DoorArray[i].trysleft);
    }
    fclose(file3);
    //Corridor
    snprintf(filename, sizeof(filename), "%s_%d_Corridor.txt", Player_UserName, level);
    FILE *file4 = fopen(filename, "w");
    fprintf(file4, "%d\n", Corridor_Placed);
    for (int i = 0; i < Corridor_Placed; i++)
    {
        fprintf(file4, "%d\n%d\n",CorridorArray[i].x,CorridorArray[i].y);
    }
    fclose(file4);
    //Window
    snprintf(filename, sizeof(filename), "%s_%d_Window.txt", Player_UserName, level);
    FILE *file5 = fopen(filename, "w");
    fprintf(file5, "%d\n", Window_Placed);
    for (int i = 0; i < Window_Placed; i++)
    {
        fprintf(file5, "%d\n%d\n%d\n",WindowArray[i].y,WindowArray[i].x,WindowArray[i].dir);
    }
    fclose(file5);
    //Free
    for (int i = 0; i < rows; i++)
    {
        free(map[i]);
    }
    free(map);
    free(RoomArray);
    free(TrapArray);
    free(CorridorArray);
    free(WindowArray);
    free(DoorArray);
    free(EnemyArray);
    free(Wasted_Weapon_Array);
    Player_Placed = false;
    firsttime = true;
    Room_Placed = 0;
    Trap_Placed = 0;
    Corridor_Placed = 0;
    Window_Placed = 0;
    Door_Placed = 0;
    EnemyCount = 0;
    Wasted_Weapon_Count = 0;
    level++;
    clear();
    refresh();
    NewGame();
}

void LoadGame_StairCase()
{
    //Free
    for (int i = 0; i < rows; i++)
    {
        free(map[i]);
    }
    free(map);
    free(RoomArray);
    free(TrapArray);
    free(CorridorArray);
    free(WindowArray);
    free(DoorArray);
    // Generate the filename
    char filename[256];
    snprintf(filename, sizeof(filename), "%s_%d.txt", Player_UserName, level-1);
    FILE *file = fopen(filename, "r");

    fscanf(file, "%d", &y);
    fscanf(file, "%d", &x);
    fscanf(file, "%d", &rows);
    fscanf(file, "%d", &cols);
    //map
    map = (char **)malloc(2*(rows+1) * sizeof(char *));
    for (int i = 0; i < rows; i++) {
        map[i] = (char *)malloc(2*(cols+1) * sizeof(char));
    }
    for (int i = 0; i < rows; i++)
    {
        //fscanf(file, "%s", map[i]);
        fgets(map[i], cols+5, file);
        //size_t len = strlen(map[i]);
        int ynigga = strlen(map[i]);
        for(int j = 0 ; j < ynigga;j++)
        {
            if(map[i][j] == '\n')
            {
                map[i][j] = '\0';
            }
        }
    }
    
    fclose(file);
    // Room
    char filename1[256];
    snprintf(filename1, sizeof(filename1), "%s_%d_Room.txt", Player_UserName, level-1);
    FILE *file1 = fopen(filename1, "r");
    fscanf(file1, "%d", &Room_Placed);
    RoomArray = (Room*)malloc(2*Room_Placed * sizeof(Room));
    for (int i = 0; i < Room_Placed; i++) {
        fscanf(file1, "%d", &(RoomArray[i].TopLeft_Y));
        fscanf(file1, "%d", &(RoomArray[i].TopLeft_x));
        fscanf(file1, "%d", &(RoomArray[i].Width));
        fscanf(file1, "%d", &(RoomArray[i].Height));
        fscanf(file1, "%d", &(RoomArray[i].middlex));
        fscanf(file1, "%d", &(RoomArray[i].middley));
        int temp;
        fscanf(file1, "%d", &temp);
        if(temp == 0)
        {
            RoomArray[i].show = false;
        }
        else
        {
            RoomArray[i].show = true;
        }
        fscanf(file1, "%d", &(RoomArray[i].wdow));
    }
    fclose(file1);
    // Trap
    char filename2[256];
    snprintf(filename2, sizeof(filename2), "%s_%d_Trap.txt", Player_UserName, level-1);
    FILE *file2 = fopen(filename2, "r");
    fscanf(file2, "%d", &Trap_Placed);
    TrapArray = (trap*)malloc(2*Trap_Placed * sizeof(trap));
    for (int i = 0; i < Trap_Placed; i++) {
        fscanf(file2, "%d", &(TrapArray[i].show));
        fscanf(file2, "%d", &(TrapArray[i].x));
        fscanf(file2, "%d", &(TrapArray[i].y));
    }
    fclose(file2);
    // Door
    char filename3[256];
    snprintf(filename3, sizeof(filename3), "%s_%d_Door.txt", Player_UserName, level-1);
    FILE *file3 = fopen(filename3, "r");
    fscanf(file3, "%d", &(Door_Placed));
    DoorArray = (door*)malloc(2*Door_Placed * sizeof(door));
    for (int i = 0; i < Door_Placed; i++) {
        int temp;
        fscanf(file3, "%d", &temp);
        if(temp == 0)
        {
            DoorArray[i].lock = false;
        }
        else
        {
            DoorArray[i].lock = true;
        }
        fscanf(file3, "%d", &(DoorArray[i].y));
        fscanf(file3, "%d", &(DoorArray[i].x));
        fscanf(file3, "%d", &(DoorArray[i].PassWord));
        fscanf(file3, "%d", &(DoorArray[i].trysleft));
    }
    fclose(file3);
    // Corridor
    char filename4[256];
    snprintf(filename4, sizeof(filename4), "%s_%d_Corridor.txt", Player_UserName, level-1);
    FILE *file4 = fopen(filename4, "r");
    fscanf(file4, "%d", &(Corridor_Placed));
    CorridorArray = (Corridor*)malloc(2*Corridor_Placed * sizeof(Corridor));
    for (int i = 0; i < Corridor_Placed; i++) {
        fscanf(file4, "%d", &(CorridorArray[i].x));
        fscanf(file4, "%d", &(CorridorArray[i].y));
    }
    fclose(file4);
    // Window
    char filename5[256];
    snprintf(filename5, sizeof(filename5), "%s_%d_Window.txt", Player_UserName, level-1);
    FILE *file5 = fopen(filename5, "r");
    fscanf(file5, "%d", &(Window_Placed));
    WindowArray = (Window*)malloc(2*Window_Placed * sizeof(Window));
    for (int i = 0; i < Window_Placed; i++) {
        fscanf(file5, "%d", &(WindowArray[i].y));
        fscanf(file5, "%d", &(WindowArray[i].x));
        fscanf(file5, "%d", &(WindowArray[i].dir));
    }
    fclose(file5);
    clear();
    for (int i = 0; i < rows - 1; i++)
    {
        strcpy(map[i],map[i+1]);
    }
    /*for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printw("%c",map[i][j]);
        }
        printw("\n");
    }*/
   /*printw("%d:",Room_Placed);
   for(int i = 0 ; i < Room_Placed;i++)
   {
    printw("%d ",RoomArray[i].middlex);
   }
   printw("\n");
   printw("%d:",Door_Placed);
   for(int i = 0 ; i < Door_Placed;i++)
   {
    printw("%d ",DoorArray[i].x);
   }
   printw("\n");
   printw("%d:",Trap_Placed);
   for(int i = 0 ; i < Trap_Placed;i++)
   {
    printw("%d ",TrapArray[i].x);
   }*/
    refresh();
    //char f = getch();
    Player_Placed = true;
    firsttime = false;
    level--;
    clear();
    NewGame();
}

void AddtoScoreBoard()
{
    FILE *file55 = fopen("ScoreBoard.txt", "a");
    fprintf(file55, "%s\n%d\n",Player_UserName,playerGOLD);
    fclose(file55);
}

void ScoreBoard()
{
    clear();
    int count = 0;
    char UserNames[1000][1000];
    int Scores[1000];
    FILE *fil = fopen("ScoreBoard.txt", "r");
    while (fgets(UserNames[count], 100, fil))
    {
        UserNames[count][strcspn(UserNames[count], "\n")] = '\0';
        char ScoreLine[100];
        if (fgets(ScoreLine, 100, fil))
        {
            Scores[count] = atoi(ScoreLine);
            count++;
        }
    }
    fclose(fil);
    for(int i = 0 ; i < count;i++)
    {
        for (int j = 0; j < count; j++)
        {
            if(Scores[j] < Scores[i])
            {
                int temp;
                temp = Scores[i];
                Scores[i] = Scores[j];
                Scores[j] = temp;
                char Temp[100];
                strcpy(Temp,UserNames[i]);
                strcpy(UserNames[i],UserNames[j]);
                strcpy(UserNames[j],Temp);
            }
        }
    }
    curs_set(0);
    wchar_t message[] = L"╔══════════════════════════════╗";
    mvprintw(1, (cols/2)-20, "%ls", message);
    printw("\n");
    wchar_t message1[] = L"║";
    mvprintw(2, (cols/2)-20, "%ls", message1);
    attron(A_BOLD);
    mvprintw(2, (cols/2)-19,"\t  ScoreBoard\t\t");
    attroff(A_BOLD);
    mvprintw(2, (cols/2) + 11,"%ls", message1);
    printw("\n");
    wchar_t message2[] = L"╠══════════════════════════════╣";
    mvprintw(3, (cols/2)-20, "%ls", message2);
    refresh();
    int rowstart = 10;
    attron(A_BOLD | COLOR_PAIR(7));
    mvprintw(7,(cols/2)-18,"Rank\t|  UserName  |  Score  |  Games_Played  |  Time\n");
    attroff(A_BOLD | COLOR_PAIR(7));
    for(int i = 0 ; i < count;i++)
    {
        if (strcmp(UserNames[i], Player_UserName) == 0)
        {
            attron(A_BOLD);
        }
        else
        {
            if (i == 0 || i == 1 || i == 2)
            {
                attron(A_BOLD);
            }
        }
        //Times Played
        FILE *file;
        char filename[256];
        snprintf(filename, sizeof(filename), "%s_Times_Played.txt", UserNames[i]);
        int number;
        file = fopen(filename, "r");
        if (file != NULL)
        {
            fscanf(file, "%d", &number);
            fclose(file);
        }
        else
        {
            number = 1;
        }

        double difference = 0;
        FILE *filee;
        char filename1[256];
        snprintf(filename1, sizeof(filename1), "%s_Player_Time.txt", UserNames[i]);
        time_t file_time, current_time;
        struct tm time_info;
        char time_string[100];

        filee = fopen(filename1, "r");
        if (filee == NULL)
        {

        }
        else
        {
            if (fgets(time_string, sizeof(time_string), filee) != NULL)
            {
                if (strptime(time_string, "%Y-%m-%d %H:%M:%S", &time_info) == NULL)
                {
                    fclose(filee);
                }

                file_time = mktime(&time_info);
                if (file_time == -1)
                {
                    fclose(filee);
                }
                fclose(filee);
            }
            else
            {
                fclose(filee);
            }
            time(&current_time);
            difference = difftime(current_time, file_time);
        }
        if(i == 0)
        {
            attron(COLOR_PAIR(4));
            wchar_t message2[] = L"🥇";
            mvprintw(rowstart+1, (cols/2)-40, "%ls", message2);
            mvprintw(rowstart+1, (cols / 2)-50, "GOAT");
            mvprintw(++rowstart, (cols / 2) - 14, "%d  |     %s     |  %d  |  %d  |  %.0f  \n", i + 1, UserNames[i], Scores[i],number, difference);
            ++rowstart;
            attroff(COLOR_PAIR(4));
        }
        else
        {
            if (i == 1)
            {
                attron(COLOR_PAIR(2));
                wchar_t message2[] = L"🥈";
                mvprintw(rowstart+1, (cols/2)-40, "%ls", message2);
                mvprintw(rowstart+1, (cols / 2)-50, "LEGEND");
                mvprintw(++rowstart, (cols / 2) - 14, "%d  |     %s     |  %d  |  %d  |  %.0f  \n", i + 1, UserNames[i], Scores[i],number, difference);
                ++rowstart;
                attroff(COLOR_PAIR(2));
            }
            else
            {
                if (i == 2)
                {
                    attron(COLOR_PAIR(3));
                    wchar_t message2[] = L"🥉";
                    mvprintw(rowstart+1, (cols/2)-40, "%ls", message2);
                    mvprintw(rowstart+1, (cols / 2)-50, "GOD");
                    mvprintw(++rowstart, (cols / 2) - 14, "%d  |     %s     |  %d  |  %d  |  %.0f  \n", i + 1, UserNames[i], Scores[i],number, difference);
                    ++rowstart;
                    attroff(COLOR_PAIR(3));
                }
                else
                {
                    mvprintw(++rowstart, (cols / 2) - 14, "%d  |     %s     |  %d  |  %d  |  %.0f  \n", i + 1, UserNames[i], Scores[i],number, difference);
                    ++rowstart;
                }
            }
        }
        if (i == 0 || i == 1 || i == 2)
        {
            attroff(A_BOLD);
        }
        else
        {
            if (strcmp(UserNames[i], Player_UserName) == 0)
            {
                attroff(A_BOLD);
            }
        }
        if(((i + 1) % 10) == 0 || (i == (count - 1)))
        {
            noecho();
            printw("Press N for next page and P for previous page and q for quit");
            char sd = getch();
            if(sd == 'N' || sd == 'n')
            {
                if(i != (count - 1))
                {
                    rowstart = 10;
                    clear();
                }
                else
                {
                    int j;
                    for(j = i ; j >= 0 ;j--)
                    {
                        if((j + 1) % 10 == 0)
                        {
                            break;
                        }
                    }
                    i = j;
                    rowstart = 10;
                    clear();
                }
            }
            if(sd == 'P' || sd == 'p')
            {
                if (i >= 19)
                {
                    i -= 20;
                    rowstart = 10;
                    clear();
                }
                else
                {
                    i = -1;
                    rowstart = 10;
                    clear();
                }
            }
            if(sd == 'q')
            {
                BeforeGame();
            }
        }
    }
    
    char nidaliy = getch();
    
    BeforeGame();
}

void MakeRoom()
{
    map = (char **)malloc((rows+1) * sizeof(char *));
    for (int i = 0; i < rows+1; i++)
    {
        map[i] = (char *)malloc((cols+1) * sizeof(char));
    }
    srand(time(0));
    // Space top va bottom
    for (int yy = 0; yy < rows; yy++)
    {
        for (int xx = 0; xx < cols; xx++)
        {
            // divar bala
            if (yy == 0 || yy == 1 || yy == 2 || yy == rows || yy == rows - 1 || yy == rows - 2)
            {
                map[yy][xx] = '%';
            }
            else
            {
                // divar bagala
                if ((xx == 0 || xx == cols) && yy != rows && yy != rows - 1 && yy == rows - 2 && yy != 0 && yy != 1 && yy != 2)
                {
                    map[yy][xx] = '%';
                }
                else
                {
                    map[yy][xx] = '#';
                }
            }
        }
    }
    int r_size_y;
    int r_size_x;
    int ry, rx;
    int r_center_x, r_center_y, r_oldcenter_x, r_oldcenter_y;
    int room_num = (rand() % 5) + 5;
    WeaponArray = (Weapon *)malloc(room_num * sizeof(Weapon) * 2);
    WeaponArray[0].mode = 1;
    Wasted_Weapon_Array = (Wasted_Weapon*)malloc(3 * sizeof(Wasted_Weapon) * room_num);
    SpellArray = (Spell *)malloc(room_num * sizeof(Spell) * 2);
    FoodArray = (Food *)malloc(room_num * sizeof(Food) * 3);
    RoomArray = (Room *)malloc(room_num * sizeof(Room) * 2);
    TrapArray = (trap *)malloc(room_num * sizeof(trap));
    DoorArray = (door *)malloc(room_num * sizeof(door) * 3);
    CorridorArray = (Corridor *)malloc(room_num * sizeof(Corridor) * 2000);
    WindowArray = (Window *)malloc(room_num * sizeof(Window) * 2);
    while ((Room_Placed < room_num))
    {
        bool collision;
        do
        {
            collision = 0;
            // room points random generate
            do
            {
                ry = (rand() % abs(rows - 15)) + 5;
                rx = (rand() % abs(cols - 25)) + 15;
                r_size_y = rand() % 8 + 5;
                r_size_x = rand() % 16 + 5;
            } while (ry + r_size_y >= rows - 3 || rx + r_size_x >= cols - 3);
            // check collision (margin rooms)(corridor setup)
            for (int yy = ry; yy <= ry + r_size_y; yy++)
            {
                for (int xx = rx; xx <= rx + r_size_x; xx++)
                {
                    int counterr = 0;
                    for (int xxx = -4; xxx <= 4; xxx++)
                    {
                        for (int yyy = -4; yyy <= 4; yyy++)
                        {
                            if (map[yyy + yy][xxx + xx] == ' ' || map[yyy + yy][xxx + xx] == '%')
                            {
                                counterr++;
                            }
                        }
                    }
                    if (counterr > 0)
                    {
                        collision = 1;
                        yy += 10000;
                        break;
                    }
                }
            }
            if (collision == 0)
            {
                int yyyy = 0;
                for (int i = 0; i < Room_Placed; i++)
                {
                    int RoomArrayRoom_PlacedTopLeft_x = rx;
                    int RoomArrayRoom_PlacedTopLeft_Y = ry;
                    int RoomArrayRoom_PlacedTopWidth = r_size_x + 1;
                    int RoomArrayRoom_PlacedTopHeight = r_size_y + 1;
                    int RoomArrayRoom_Placedmiddlex = rx + ((r_size_x + 1) / 2);
                    int RoomArrayRoom_Placedmiddley = ry + ((r_size_y + 1) / 2);

                    if (RoomArrayRoom_Placedmiddlex == RoomArray[i].TopLeft_x - 1 ||
                        RoomArrayRoom_Placedmiddlex == RoomArray[i].TopLeft_x ||
                        RoomArrayRoom_Placedmiddlex == RoomArray[i].TopLeft_x + 1 ||
                        RoomArrayRoom_Placedmiddlex == RoomArray[i].TopLeft_x + RoomArray[i].Width + 1 ||
                        RoomArrayRoom_Placedmiddlex == RoomArray[i].TopLeft_x + RoomArray[i].Width - 1 ||
                        RoomArrayRoom_Placedmiddlex == RoomArray[i].TopLeft_x + RoomArray[i].Width ||
                        RoomArrayRoom_Placedmiddley == RoomArray[i].TopLeft_Y - 1 ||
                        RoomArrayRoom_Placedmiddley == RoomArray[i].TopLeft_Y ||
                        RoomArrayRoom_Placedmiddley == RoomArray[i].TopLeft_Y + 1 ||
                        RoomArrayRoom_Placedmiddley == RoomArray[i].TopLeft_Y + RoomArray[i].Height ||
                        RoomArrayRoom_Placedmiddley == RoomArray[i].TopLeft_Y + RoomArray[i].Height + 1 ||
                        RoomArrayRoom_Placedmiddley == RoomArray[i].TopLeft_Y + RoomArray[i].Height - 1 ||
                        RoomArrayRoom_PlacedTopLeft_x - 1 == RoomArray[i].middlex ||
                        RoomArrayRoom_PlacedTopLeft_x == RoomArray[i].middlex ||
                        RoomArrayRoom_PlacedTopLeft_x + 1 == RoomArray[i].middlex ||
                        RoomArrayRoom_PlacedTopLeft_Y - 1 == RoomArray[i].middley ||
                        RoomArrayRoom_PlacedTopLeft_Y == RoomArray[i].middley ||
                        RoomArrayRoom_PlacedTopLeft_Y + 1 == RoomArray[i].middley ||
                        RoomArrayRoom_PlacedTopLeft_x + RoomArrayRoom_PlacedTopWidth == RoomArray[i].middlex ||
                        RoomArrayRoom_PlacedTopLeft_x + RoomArrayRoom_PlacedTopWidth - 1 == RoomArray[i].middlex ||
                        RoomArrayRoom_PlacedTopLeft_x + RoomArrayRoom_PlacedTopWidth + 1 == RoomArray[i].middlex ||
                        RoomArrayRoom_PlacedTopLeft_Y + RoomArrayRoom_PlacedTopHeight == RoomArray[i].middley ||
                        RoomArrayRoom_PlacedTopLeft_Y + RoomArrayRoom_PlacedTopHeight + 1 == RoomArray[i].middley ||
                        RoomArrayRoom_PlacedTopLeft_Y + RoomArrayRoom_PlacedTopHeight - 1 == RoomArray[i].middley)
                    {
                        yyyy++;
                    }
                }
                if (yyyy != 0)
                {
                    collision = 1;
                }
                /*for(int i = 0 ; i < Room_Placed;i++)
                {
                    if(abs(RoomArray[i].TopLeft_x - (rx + r_size_x + 1)) < 10 ||
                    abs(RoomArray[i].TopLeft_Y - (ry + r_size_y + 1)) < 10 ||
                    abs(RoomArray[i].TopLeft_x + RoomArray[i].Width - (rx)) < 10 ||
                    abs(RoomArray[i].TopLeft_Y + RoomArray[i].Height - (ry)) < 10)
                    {
                        collision = 1;
                    }
                }*/
            }
        } while (collision);
        // fill struct
        RoomArray[Room_Placed].TopLeft_x = rx;
        RoomArray[Room_Placed].TopLeft_Y = ry;
        RoomArray[Room_Placed].Width = r_size_x + 1;
        RoomArray[Room_Placed].Height = r_size_y + 1;
        RoomArray[Room_Placed].middlex = rx + ((r_size_x + 1) / 2);
        RoomArray[Room_Placed].middley = ry + ((r_size_y + 1) / 2);
        // fill map with rooms
        for (int yy = ry; yy <= ry + r_size_y; yy++)
        {
            for (int xx = rx; xx <= rx + r_size_x; xx++)
            {
                if (map[yy][xx] != '@' && yy > 0 && yy < rows - 1 && xx > 0 && xx < cols)
                {
                    map[yy][xx] = ' ';
                }
            }
        }
        Room_Placed++;
    }
    refresh();
    // sort rooms
    for (int ii = 0; ii < room_num; ii++)
    {
        for (int jj = 0; jj < room_num; jj++)
        {
            if (RoomArray[ii].TopLeft_x + (RoomArray[ii].TopLeft_Y) < RoomArray[jj].TopLeft_x + (RoomArray[jj].TopLeft_Y))
            {
                Room temp;
                temp = RoomArray[ii];
                RoomArray[ii] = RoomArray[jj];
                RoomArray[jj] = temp;
            }
        }
    }
    // corridors generate
    for (int ii = 0; ii < room_num - 1; ii++)
    {
        int jj;
        for (jj = RoomArray[ii].middley; jj != RoomArray[ii + 1].middley;)
        {
            map[jj][RoomArray[ii].middlex] = '$';
            if (RoomArray[ii].middley < RoomArray[ii + 1].middley)
            {
                jj++;
            }
            else
            {
                if (RoomArray[ii].middley > RoomArray[ii + 1].middley)
                {
                    jj--;
                }
                else
                {
                    break;
                }
            }
        }

        ///
        for (int kk = RoomArray[ii].middlex; kk != RoomArray[ii + 1].middlex;)
        {

            map[jj][kk] = '$';
            if (RoomArray[ii].middlex < RoomArray[ii + 1].middlex)
            {
                kk++;
            }
            else
            {
                if (RoomArray[ii].middlex > RoomArray[ii + 1].middlex)
                {
                    kk--;
                }
                else
                {
                    break;
                }
            }
        }
    }

    // fill door struct
    for (int index = 0; index < Room_Placed; index++)
    {
        for (int yy = (RoomArray[index].TopLeft_Y - 1); yy <= RoomArray[index].TopLeft_Y + RoomArray[index].Height; yy++)
        {
            for (int xx = (RoomArray[index].TopLeft_x - 1); xx <= RoomArray[index].TopLeft_x + RoomArray[index].Width; xx++)
            {
                if (yy >= RoomArray[index].TopLeft_Y && yy <= (RoomArray[index].TopLeft_Y + RoomArray[index].Height - 1) &&
                    xx >= RoomArray[index].TopLeft_x && xx <= (RoomArray[index].TopLeft_x + RoomArray[index].Width - 1))
                {
                }
                else
                {

                    int countt = 0;
                    if (map[yy][xx] == '$')
                    {
                        countt++;
                    }
                    if (yy == (RoomArray[index].TopLeft_Y - 1) || yy == (RoomArray[index].TopLeft_Y + RoomArray[index].Height))
                    {
                        if (countt == 1)
                        {
                            DoorArray[Door_Placed].y = yy;
                            DoorArray[Door_Placed].x = xx;
                            Door_Placed++;
                        }
                    }
                    else
                    {
                        if (countt == 1)
                        {
                            DoorArray[Door_Placed].y = yy;
                            DoorArray[Door_Placed].x = xx;
                            Door_Placed++;
                        }
                    }
                }
            }
        }
    }
    // random lock
    int randomdoor = rand() % Door_Placed;
    for (int i = 0; i < Door_Placed; i++)
    {
        if (i == randomdoor)
        {
            DoorArray[i].lock = true;
            DoorArray[i].trysleft = 3;
        }
        else
        {
            DoorArray[i].lock = false;
        }
    }
    int Doors_Room = 0;
    for (int i = 0; i < Room_Placed; i++)
    {
        if (DoorArray[randomdoor].y >= RoomArray[i].TopLeft_Y - 1 &&
            DoorArray[randomdoor].y <= RoomArray[i].TopLeft_Y + RoomArray[i].Height &&
            DoorArray[randomdoor].x >= RoomArray[i].TopLeft_x - 1 &&
            DoorArray[randomdoor].x <= RoomArray[i].TopLeft_x + RoomArray[i].Width)
        {
            int corner = rand() % 4;
            if (corner == 0)
            {
                map[RoomArray[i].TopLeft_Y][RoomArray[i].TopLeft_x] = '&';
            }
            if (corner == 1)
            {
                map[RoomArray[i].TopLeft_Y][RoomArray[i].TopLeft_x + RoomArray[i].Width - 1] = '&';
            }
            if (corner == 2)
            {
                map[RoomArray[i].TopLeft_Y + RoomArray[i].Height - 1][RoomArray[i].TopLeft_x] = '&';
            }
            if (corner == 3)
            {
                map[RoomArray[i].TopLeft_Y + RoomArray[i].Height - 1][RoomArray[i].TopLeft_x + RoomArray[i].Width - 1] = '&';
            }
            if (i - 1 >= 0)
            {
                corner = rand() % 4;
                if (corner == 0)
                {
                    map[RoomArray[i - 1].TopLeft_Y][RoomArray[i - 1].TopLeft_x] = '&';
                }
                if (corner == 1)
                {
                    map[RoomArray[i - 1].TopLeft_Y][RoomArray[i - 1].TopLeft_x + RoomArray[i - 1].Width - 1] = '&';
                }
                if (corner == 2)
                {
                    map[RoomArray[i - 1].TopLeft_Y + RoomArray[i - 1].Height - 1][RoomArray[i - 1].TopLeft_x] = '&';
                }
                if (corner == 3)
                {
                    map[RoomArray[i - 1].TopLeft_Y + RoomArray[i - 1].Height - 1][RoomArray[i - 1].TopLeft_x + RoomArray[i - 1].Width - 1] = '&';
                }
            }
            if (i + 1 < Room_Placed)
            {
                corner = rand() % 4;
                if (corner == 0)
                {
                    map[RoomArray[i + 1].TopLeft_Y][RoomArray[i + 1].TopLeft_x] = '&';
                }
                if (corner == 1)
                {
                    map[RoomArray[i + 1].TopLeft_Y][RoomArray[i + 1].TopLeft_x + RoomArray[i + 1].Width - 1] = '&';
                }
                if (corner == 2)
                {
                    map[RoomArray[i + 1].TopLeft_Y + RoomArray[i + 1].Height - 1][RoomArray[i + 1].TopLeft_x] = '&';
                }
                if (corner == 3)
                {
                    map[RoomArray[i + 1].TopLeft_Y + RoomArray[i + 1].Height - 1][RoomArray[i + 1].TopLeft_x + RoomArray[i + 1].Width - 1] = '&';
                }
            }
        }
    }

    // pillars and traps and gold and GOLD and weapon and Spell and food
    for (int index = 0; index < Room_Placed; index++)
    {
        // init show to false
        RoomArray[index].show = false;
        // pillars
        int orandomx;
        int orandomy;
        do
        {
            orandomx = (rand() % (RoomArray[index].Width - 4)) + RoomArray[index].TopLeft_x + 1;
            orandomy = (rand() % (RoomArray[index].Height - 4)) + RoomArray[index].TopLeft_Y + 1;
        } while ((map[orandomy][orandomx] == '&'));
        map[orandomy][orandomx] = 'O';
        // traps
        int yesno = rand() % 3;
        if (yesno == 1)
        {
            do
            {
                orandomx = (rand() % (RoomArray[index].Width - 4)) + RoomArray[index].TopLeft_x + 1;
                orandomy = (rand() % (RoomArray[index].Height - 4)) + RoomArray[index].TopLeft_Y + 1;
            } while ((map[orandomy][orandomx] == 'O') || (map[orandomy][orandomx] == '&'));
            TrapArray[Trap_Placed].x = orandomx;
            TrapArray[Trap_Placed].y = orandomy;
            Trap_Placed++;
            map[orandomy][orandomx] = '^';
        }
        // Gold
        yesno = rand() % 3;
        for (int inti = 0; inti < yesno; inti++)
        {
            do
            {
                orandomx = (rand() % (RoomArray[index].Width - 4)) + RoomArray[index].TopLeft_x + 1;
                orandomy = (rand() % (RoomArray[index].Height - 4)) + RoomArray[index].TopLeft_Y + 1;
            } while ((map[orandomy][orandomx] == 'O') || (map[orandomy][orandomx] == '^') ||
                     (map[orandomy][orandomx] == '&'));
            map[orandomy][orandomx] = 'G';
        }
        // gold
        yesno = rand() % 7;
        if (yesno == 4)
        {
            do
            {
                orandomx = (rand() % (RoomArray[index].Width - 4)) + RoomArray[index].TopLeft_x + 1;
                orandomy = (rand() % (RoomArray[index].Height - 4)) + RoomArray[index].TopLeft_Y + 1;
            } while ((map[orandomy][orandomx] == 'O') || (map[orandomy][orandomx] == '^') ||
                     (map[orandomy][orandomx] == 'G') || (map[orandomy][orandomx] == '&'));
            map[orandomy][orandomx] = 'g';
        }
        // Weapon
        yesno = rand() % 2;
        if (yesno == 0)
        {
            do
            {
                orandomx = (rand() % (RoomArray[index].Width - 4)) + RoomArray[index].TopLeft_x + 1;
                orandomy = (rand() % (RoomArray[index].Height - 4)) + RoomArray[index].TopLeft_Y + 1;
            } while ((map[orandomy][orandomx] == 'O') || (map[orandomy][orandomx] == '^') ||
                     (map[orandomy][orandomx] == 'G') || (map[orandomy][orandomx] == '&') || (map[orandomy][orandomx] == 'g'));
            int randomniggro = (rand() % 4) + 2;
            /*if (randomniggro == 1)
            {
                map[orandomy][orandomx] = '1';
            }*/
            if (randomniggro == 2)
            {
                map[orandomy][orandomx] = '2';
            }
            if (randomniggro == 3)
            {
                map[orandomy][orandomx] = '3';
            }
            if (randomniggro == 4)
            {
                map[orandomy][orandomx] = '4';
            }
            if (randomniggro == 5)
            {
                map[orandomy][orandomx] = '5';
            }
        }
        // Spell
        yesno = rand() % 2;
        if (yesno == 0)
        {
            do
            {
                orandomx = (rand() % (RoomArray[index].Width - 4)) + RoomArray[index].TopLeft_x + 1;
                orandomy = (rand() % (RoomArray[index].Height - 4)) + RoomArray[index].TopLeft_Y + 1;
            } while ((map[orandomy][orandomx] == 'O') || (map[orandomy][orandomx] == '^') ||
                     (map[orandomy][orandomx] == 'G') || (map[orandomy][orandomx] == '&') ||
                     (map[orandomy][orandomx] == 'g') || 
                     (map[orandomy][orandomx] == '2') || (map[orandomy][orandomx] == '3') ||
                     (map[orandomy][orandomx] == '4') || (map[orandomy][orandomx] == '5'));
            int randomniggro = (rand() % 3) + 6;
            if (randomniggro == 6)
            {
                map[orandomy][orandomx] = '6';
            }
            if (randomniggro == 7)
            {
                map[orandomy][orandomx] = '7';
            }
            if (randomniggro == 8)
            {
                map[orandomy][orandomx] = '8';
            }
        }
        // Food
        yesno = rand() % 2;
        yesno = 0;
        if (yesno == 0)
        {
            do
            {
                orandomx = (rand() % (RoomArray[index].Width - 4)) + RoomArray[index].TopLeft_x + 1;
                orandomy = (rand() % (RoomArray[index].Height - 4)) + RoomArray[index].TopLeft_Y + 1;
            } while ((map[orandomy][orandomx] == 'O') || (map[orandomy][orandomx] == '^') ||
                     (map[orandomy][orandomx] == 'G') || (map[orandomy][orandomx] == '&') ||
                     (map[orandomy][orandomx] == 'g') || 
                     (map[orandomy][orandomx] == '2') || (map[orandomy][orandomx] == '3') ||
                     (map[orandomy][orandomx] == '4') || (map[orandomy][orandomx] == '5') ||
                     (map[orandomy][orandomx] == '6') || (map[orandomy][orandomx] == '7') ||
                     (map[orandomy][orandomx] == '8'));
            int typefood = rand() % 4;
            if (typefood == 0)
            {
                map[orandomy][orandomx] = 'Z';
            }
            if (typefood == 1)
            {
                map[orandomy][orandomx] = 'X';
            }
            if (typefood == 2)
            {
                map[orandomy][orandomx] = 'C';
            }
            if (typefood == 3)
            {
                map[orandomy][orandomx] = 'V';
            }
        }
    }
    // staircases
    int whichroom = rand() % Room_Placed;
    int srandomx;
    int srandomy;
    do
    {
        srandomx = (rand() % (RoomArray[whichroom].Width - 4)) + RoomArray[whichroom].TopLeft_x + 1;
        srandomy = (rand() % (RoomArray[whichroom].Height - 4)) + RoomArray[whichroom].TopLeft_Y + 1;
    } while ((srandomx == x && srandomy == y) || (map[srandomy][srandomx] == 'O') ||
             (map[srandomy][srandomx] == '^') || (map[srandomy][srandomx] == 'G') ||
             (map[srandomy][srandomx] == 'g') || (map[srandomy][srandomx] == '&') ||
             (map[srandomy][srandomx] == '2') || (map[srandomy][srandomx] == '3') ||
             (map[srandomy][srandomx] == '4') || (map[srandomy][srandomx] == '5') ||
             (map[srandomy][srandomx] == '6') || (map[srandomy][srandomx] == '7') ||
             (map[srandomy][srandomx] == '8') || (map[srandomy][srandomx] == 'Z') ||
             (map[srandomy][srandomx] == 'X') || (map[srandomy][srandomx] == 'C') ||
             (map[srandomy][srandomx] == 'V'));
    map[srandomy][srandomx] = '<';
    // Ancient Key
    whichroom = rand() % Room_Placed;
    do
    {
        srandomx = (rand() % (RoomArray[whichroom].Width - 4)) + RoomArray[whichroom].TopLeft_x + 1;
        srandomy = (rand() % (RoomArray[whichroom].Height - 4)) + RoomArray[whichroom].TopLeft_Y + 1;
    } while ((srandomx == x && srandomy == y) || (map[srandomy][srandomx] == 'O') ||
             (map[srandomy][srandomx] == '^') || (map[srandomy][srandomx] == 'G') ||
             (map[srandomy][srandomx] == 'g') || (map[srandomy][srandomx] == '&') ||
             (map[srandomy][srandomx] == '<') ||
             (map[srandomy][srandomx] == '2') || (map[srandomy][srandomx] == '3') ||
             (map[srandomy][srandomx] == '4') || (map[srandomy][srandomx] == '5') ||
             (map[srandomy][srandomx] == '6') || (map[srandomy][srandomx] == '7') ||
             (map[srandomy][srandomx] == '8') || (map[srandomy][srandomx] == 'Z') ||
             (map[srandomy][srandomx] == 'X') || (map[srandomy][srandomx] == 'C') ||
             (map[srandomy][srandomx] == 'V'));
    map[srandomy][srandomx] = 'A';
    // Windows
    for (int i = 0; i < Room_Placed; i++)
    {
        int v = rand() % 3;
        if (v == 0)
        {
            int vv = rand() % 6;
            // 0 top
            if (vv == 0)
            {
                int checking = 5;
                do
                {
                    checking = 0;
                    int vvv = (rand() % (RoomArray[i].Width / 2)) + RoomArray[i].Width / 4;
                    WindowArray[Window_Placed].x = RoomArray[i].TopLeft_x + vvv;
                    WindowArray[Window_Placed].y = RoomArray[i].TopLeft_Y - 1;
                    for (int j = 0; j < Door_Placed; j++)
                    {
                        if (DoorArray[j].x == RoomArray[i].TopLeft_x + vvv && DoorArray[j].y == RoomArray[i].TopLeft_Y - 1)
                        {
                            checking++;
                        }
                    }

                } while (checking > 0);
                WindowArray[Window_Placed].dir = 0;
            }
            // 1 right
            if (vv == 1 || vv == 4)
            {
                int checking = 5;
                do
                {
                    checking = 0;
                    int vvv = (rand() % (RoomArray[i].Height / 2)) + RoomArray[i].Height / 4;
                    WindowArray[Window_Placed].x = RoomArray[i].TopLeft_x + RoomArray[i].Width;
                    WindowArray[Window_Placed].y = RoomArray[i].TopLeft_Y + vvv;
                    for (int j = 0; j < Door_Placed; j++)
                    {
                        if (DoorArray[j].x == RoomArray[i].TopLeft_x + RoomArray[i].Width && DoorArray[j].y == RoomArray[i].TopLeft_Y + vvv)
                        {
                            checking++;
                        }
                    }
                } while (checking > 0);
                WindowArray[Window_Placed].dir = 1;
            }
            // 2 bottom
            if (vv == 2)
            {
                int checking = 5;
                do
                {
                    checking = 0;
                    int vvv = (rand() % (RoomArray[i].Width / 2)) + RoomArray[i].Width / 4;
                    WindowArray[Window_Placed].x = RoomArray[i].TopLeft_x + vvv;
                    WindowArray[Window_Placed].y = RoomArray[i].TopLeft_Y + RoomArray[i].Height;
                    for (int j = 0; j < Door_Placed; j++)
                    {
                        if (DoorArray[j].x == RoomArray[i].TopLeft_x + vvv && DoorArray[j].y == RoomArray[i].TopLeft_Y + RoomArray[i].Height)
                        {
                            checking++;
                        }
                    }
                } while (checking > 0);
                WindowArray[Window_Placed].dir = 2;
            }
            // 3 left
            if (vv == 3 || vv == 5)
            {
                int checking = 5;
                do
                {
                    checking = 0;
                    int vvv = (rand() % (RoomArray[i].Height / 2)) + RoomArray[i].Height / 4;
                    WindowArray[Window_Placed].x = RoomArray[i].TopLeft_x - 1;
                    WindowArray[Window_Placed].y = RoomArray[i].TopLeft_Y + vvv;
                    for (int j = 0; j < Door_Placed; j++)
                    {
                        if (DoorArray[j].x == RoomArray[i].TopLeft_x - 1 && DoorArray[j].y == RoomArray[i].TopLeft_Y + vvv)
                        {
                            checking++;
                        }
                    }
                } while (checking > 0);
                WindowArray[Window_Placed].dir = 3;
            }
            ++Window_Placed;
        }
    }
    // Gang room
    if (onetime == 0)
    {
        if (level == 4)
        {
            onetime = 1;
            int roomin = -1;
            for (int i = 0; i < Room_Placed; i++)
            {
                int tx = RoomArray[i].TopLeft_x;
                int txw = RoomArray[i].TopLeft_x + RoomArray[i].Width;
                for (int j = tx; j < txw; j++)
                {
                    int ty = RoomArray[i].TopLeft_Y;
                    int tyw = RoomArray[i].TopLeft_Y + RoomArray[i].Height;
                    for (int k = ty; k < tyw; k++)
                    {
                        if (map[k][j] == '<')
                        {
                            roomin = i;
                            j += 1000;
                            i += 1000;
                            break;
                        }
                    }
                }
            }
            for (int i = RoomArray[roomin].TopLeft_x; i < RoomArray[roomin].TopLeft_x + RoomArray[roomin].Width; i++)
            {
                for (int j = RoomArray[roomin].TopLeft_Y; j < RoomArray[roomin].TopLeft_Y + RoomArray[roomin].Height; j++)
                {
                    if (map[j][i] == ' ')
                    {
                        int fg = rand() % 3;
                        if (fg == 0)
                        {
                            map[j][i] = 'g';
                        }
                        if (fg == 1)
                        {
                            TrapArray[Trap_Placed].show = false;
                            TrapArray[Trap_Placed].x = i;
                            TrapArray[Trap_Placed].y = j;
                            ++Trap_Placed;
                            map[j][i] = '^';
                        }
                        if (fg == 2)
                        {
                        }
                    }
                }
            }
            /*printw("%d\n",roomin);
            for(int i = 0 ; i < rows;i++)
            {
                printw("%s\n",map[i]);
            }*/
        }
    }
}

void Player_Time()
{
    FILE *file;
    char filename[256];
    snprintf(filename, sizeof(filename), "%s_Player_Time.txt", Player_UserName);
    time_t current_time;
    struct tm *time_info;
    char time_string[100];

    file = fopen(filename, "r");
    if (file != NULL) 
    {
        fclose(file);
        return;
    }

    file = fopen(filename, "w");

    time(&current_time);
    time_info = localtime(&current_time);
    strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", time_info);

    fprintf(file, "%s\n", time_string);
    fclose(file);
}

void load()
{
    clear();
    // Generate the filename
    char filename[256];
    snprintf(filename, sizeof(filename), "%s_Stats.txt", Player_UserName);
    FILE *file = fopen(filename, "r");

    fscanf(file, "%d", &y);
    fscanf(file, "%d", &x);
    fscanf(file, "%d", &rows);
    fscanf(file, "%d", &cols);
    fscanf(file, "%d",&level);
    fscanf(file, "%d",&color);
    fscanf(file, "%d",&playerGOLD);
    fscanf(file, "%d",&playerHP);
    fscanf(file, "%d",&TillLast);
    fscanf(file, "%d",&FoodEffect);
    fscanf(file, "%d",&AncientKeys);
    for(int i = 0 ; i < AncientKeys;i++)
    {
        fscanf(file, "%d",&AncientKey[i]);
    }
    fclose(file);
    snprintf(filename, sizeof(filename), "%s_Stats1.txt", Player_UserName);
    FILE *filee = fopen(filename, "r");
    fscanf(filee, "%d",&UsedKey);
    fscanf(filee, "%d",&WeaponCount);
    fscanf(filee, "%d",&SpellCount);
    fscanf(filee, "%d",&FoodCount);
    fscanf(filee, "%d",&MovesTillLastFood);
    fscanf(filee, "%d",&m);
    fclose(filee);
    //Map
    snprintf(filename, sizeof(filename), "%s_Map.txt", Player_UserName);
    FILE *file1 = fopen(filename, "r");
    map = (char **)malloc(2*(rows+1) * sizeof(char *));
    for (int i = 0; i < rows; i++) {
        map[i] = (char *)malloc(2*(cols+1) * sizeof(char));
    }
    for (int i = 0; i < rows; i++)
    {
        fgets(map[i], cols+5, file);
        int ynigga = strlen(map[i]);
        for(int j = 0 ; j < ynigga;j++)
        {
            if(map[i][j] == '\n')
            {
                map[i][j] = '\0';
            }
        }
    }
    fclose(file1);
    // Room
    snprintf(filename, sizeof(filename), "%s_Room.txt", Player_UserName);
    FILE *file2 = fopen(filename, "r");
    fscanf(file2, "%d", &Room_Placed);
    RoomArray = (Room*)malloc(2*Room_Placed * sizeof(Room));
    for (int i = 0; i < Room_Placed; i++) {
        fscanf(file2, "%d", &(RoomArray[i].TopLeft_Y));
        fscanf(file2, "%d", &(RoomArray[i].TopLeft_x));
        fscanf(file2, "%d", &(RoomArray[i].Width));
        fscanf(file2, "%d", &(RoomArray[i].Height));
        fscanf(file2, "%d", &(RoomArray[i].middlex));
        fscanf(file2, "%d", &(RoomArray[i].middley));
        int temp;
        fscanf(file2, "%d", &temp);
        if(temp == 0)
        {
            RoomArray[i].show = false;
        }
        else
        {
            RoomArray[i].show = true;
        }
        fscanf(file2, "%d", &(RoomArray[i].wdow));
    }
    fclose(file2);
    // Trap
    char filename2[256];
    snprintf(filename2, sizeof(filename2), "%s_Trap.txt", Player_UserName);
    FILE *file3 = fopen(filename2, "r");
    fscanf(file3, "%d", &Trap_Placed);
    TrapArray = (trap*)malloc(2*Trap_Placed * sizeof(trap));
    for (int i = 0; i < Trap_Placed; i++) {
        fscanf(file3, "%d", &(TrapArray[i].show));
        fscanf(file3, "%d", &(TrapArray[i].x));
        fscanf(file3, "%d", &(TrapArray[i].y));
    }
    fclose(file3);
    // Door
    char filename3[256];
    snprintf(filename3, sizeof(filename3), "%s_Door.txt", Player_UserName);
    FILE *file4 = fopen(filename3, "r");
    fscanf(file4, "%d", &(Door_Placed));
    DoorArray = (door*)malloc(2*Door_Placed * sizeof(door));
    for (int i = 0; i < Door_Placed; i++) {
        int temp;
        fscanf(file4, "%d", &temp);
        if(temp == 0)
        {
            DoorArray[i].lock = false;
        }
        else
        {
            DoorArray[i].lock = true;
        }
        fscanf(file4, "%d", &(DoorArray[i].y));
        fscanf(file4, "%d", &(DoorArray[i].x));
        fscanf(file4, "%d", &(DoorArray[i].PassWord));
        fscanf(file4, "%d", &(DoorArray[i].trysleft));
    }
    fclose(file4);
    // Corridor
    char filename4[256];
    snprintf(filename4, sizeof(filename4), "%s_Corridor.txt", Player_UserName);
    FILE *file5 = fopen(filename4, "r");
    fscanf(file5, "%d", &(Corridor_Placed));
    CorridorArray = (Corridor*)malloc(2*Corridor_Placed * sizeof(Corridor));
    for (int i = 0; i < Corridor_Placed; i++) {
        fscanf(file5, "%d", &(CorridorArray[i].x));
        fscanf(file5, "%d", &(CorridorArray[i].y));
    }
    fclose(file5);
    // Window
    char filename5[256];
    snprintf(filename5, sizeof(filename5), "%s_Window.txt", Player_UserName);
    FILE *file6 = fopen(filename5, "r");
    fscanf(file6, "%d", &(Window_Placed));
    WindowArray = (Window*)malloc(2*Window_Placed * sizeof(Window));
    for (int i = 0; i < Window_Placed; i++) {
        fscanf(file6, "%d", &(WindowArray[i].y));
        fscanf(file6, "%d", &(WindowArray[i].x));
        fscanf(file6, "%d", &(WindowArray[i].dir));
    }
    fclose(file6);
    // Weapon
    char filename6[256];
    snprintf(filename6, sizeof(filename6), "%s_Weapon.txt", Player_UserName);
    FILE *file7 = fopen(filename6, "r");
    fscanf(file7, "%d", &(WeaponCount));
    WeaponArray = (Weapon*)malloc(2*WeaponCount * sizeof(Weapon));
    for (int i = 0; i < WeaponCount; i++) 
    {
        fscanf(file7, "%d", &(WeaponArray[i].mode));
    }
    fclose(file7);
    // Spell
    char filename7[256];
    snprintf(filename7, sizeof(filename7), "%s_Spell.txt", Player_UserName);
    FILE *file8 = fopen(filename7, "r");
    fscanf(file8, "%d", &(SpellCount));
    SpellArray = (Spell*)malloc(2*SpellCount * sizeof(Spell));
    for (int i = 0; i < SpellCount; i++) 
    {
        fscanf(file8, "%d", &(SpellArray[i].mode));
    }
    fclose(file8);
    // Food
    char filename8[256];
    snprintf(filename8, sizeof(filename8), "%s_Food.txt", Player_UserName);
    FILE *file9 = fopen(filename8, "r");
    fscanf(file9, "%d", &(FoodCount));
    FoodArray = (Food*)malloc(2*FoodCount * sizeof(Food));
    for (int i = 0; i < FoodCount; i++) 
    {
        fscanf(file9, "%d", &(FoodArray[i].type));
    }
    fclose(file9);
    refresh();
    Player_Placed = true;
    firsttime = false;
    clear();
    NewGame();
}

void save()
{
    // Generate the filename
    char filename[256];
    snprintf(filename, sizeof(filename), "%s_Stats.txt", Player_UserName);
    FILE *file = fopen(filename, "w");
    fprintf(file, "%d\n",y);
    fprintf(file, "%d\n",x);
    fprintf(file, "%d\n",rows);
    fprintf(file, "%d\n",cols);
    fprintf(file, "%d\n",level);
    fprintf(file, "%d\n",color);
    fprintf(file, "%d\n",playerGOLD);
    fprintf(file, "%d\n",playerHP);
    fprintf(file, "%d\n",TillLast);
    fprintf(file, "%d\n",FoodEffect);
    fprintf(file, "%d\n",AncientKeys);
    for(int i = 0 ; i < AncientKeys;i++)
    {
        fprintf(file, "%d\n",AncientKey[i]);
    }
    fclose(file);
    snprintf(filename, sizeof(filename), "%s_Stats1.txt", Player_UserName);
    FILE *filee = fopen(filename, "w");
    fprintf(filee, "%d\n",UsedKey);
    fprintf(filee, "%d\n",WeaponCount);
    fprintf(filee, "%d\n",SpellCount);
    fprintf(filee, "%d\n",FoodCount);
    fprintf(filee, "%d\n",MovesTillLastFood);
    fprintf(filee, "%d\n",m);
    fclose(filee);
    snprintf(filename, sizeof(filename), "%s_Map.txt", Player_UserName);
    FILE *file1 = fopen(filename, "w");
    //Map
    for (int i = 0; i < rows; i++)
    {
        fprintf(file1, "%s\n", map[i]);
    }
    fclose(file1);
    //Room
    snprintf(filename, sizeof(filename), "%s_Room.txt", Player_UserName);
    FILE *file2 = fopen(filename, "w");
    fprintf(file2, "%d\n", Room_Placed);
    for (int i = 0; i < Room_Placed; i++)
    {
        fprintf(file2, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",RoomArray[i].TopLeft_Y,RoomArray[i].TopLeft_x,
        RoomArray[i].Width,RoomArray[i].Height,RoomArray[i].middlex,RoomArray[i].middley,
        RoomArray[i].show,RoomArray[i].wdow);
    }
    fclose(file2);
    //Trap
    snprintf(filename, sizeof(filename), "%s_Trap.txt", Player_UserName);
    FILE *file3 = fopen(filename, "w");
    fprintf(file3, "%d\n", Trap_Placed);
    for (int i = 0; i < Trap_Placed; i++)
    {
        fprintf(file3, "%d\n%d\n%d\n",TrapArray[i].show,TrapArray[i].x,TrapArray[i].y);
    }
    fclose(file3);
    //Door
    snprintf(filename, sizeof(filename), "%s_Door.txt", Player_UserName);
    FILE *file4 = fopen(filename, "w");
    fprintf(file4, "%d\n", Door_Placed);
    for (int i = 0; i < Door_Placed; i++)
    {
        fprintf(file4, "%d\n%d\n%d\n%d\n%d\n",DoorArray[i].lock,DoorArray[i].y,
        DoorArray[i].x,DoorArray[i].PassWord,DoorArray[i].trysleft);
    }
    fclose(file4);
    //Corridor
    snprintf(filename, sizeof(filename), "%s_Corridor.txt", Player_UserName);
    FILE *file5 = fopen(filename, "w");
    fprintf(file5, "%d\n", Corridor_Placed);
    for (int i = 0; i < Corridor_Placed; i++)
    {
        fprintf(file5, "%d\n%d\n",CorridorArray[i].x,CorridorArray[i].y);
    }
    fclose(file5);
    //Window
    snprintf(filename, sizeof(filename), "%s_Window.txt", Player_UserName);
    FILE *file6 = fopen(filename, "w");
    fprintf(file6, "%d\n", Window_Placed);
    for (int i = 0; i < Window_Placed; i++)
    {
        fprintf(file6, "%d\n%d\n%d\n",WindowArray[i].y,WindowArray[i].x,WindowArray[i].dir);
    }
    fclose(file6);
    //Weapon
    snprintf(filename, sizeof(filename), "%s_Weapon.txt", Player_UserName);
    FILE *file7 = fopen(filename, "w");
    fprintf(file7, "%d\n", WeaponCount);
    for (int i = 0; i < WeaponCount; i++)
    {
        fprintf(file7, "%d\n",WeaponArray[i].mode);
    }
    fclose(file7);
    //Spell
    snprintf(filename, sizeof(filename), "%s_Spell.txt", Player_UserName);
    FILE *file8 = fopen(filename, "w");
    fprintf(file8, "%d\n", SpellCount);
    for (int i = 0; i < SpellCount; i++)
    {
        fprintf(file8, "%d\n",SpellArray[i].mode);
    }
    fclose(file8);
    //Food
    snprintf(filename, sizeof(filename), "%s_Food.txt", Player_UserName);
    FILE *file9 = fopen(filename, "w");
    fprintf(file9, "%d\n", FoodCount);
    for (int i = 0; i < FoodCount; i++)
    {
        fprintf(file9, "%d\n",FoodArray[i].type);
    }
    fclose(file9);
    //Free
    for (int i = 0; i < rows; i++)
    {
        free(map[i]);
    }
    free(map);
    free(RoomArray);
    free(TrapArray);
    free(CorridorArray);
    free(WindowArray);
    free(DoorArray);
    endwin();
    exit(0);
}

void Enemy_gen()
{
    EnemyArray = (Enemy*)malloc(sizeof(Enemy) * Room_Placed * 2);
    for(int i = 0 ; i < Room_Placed;i++)
    {
        int xe;
        int ye;
        do
        {
            xe = (rand() % RoomArray[i].Width) + RoomArray[i].TopLeft_x;
            ye = (rand() % RoomArray[i].Height) + RoomArray[i].TopLeft_Y;
        } while ((map[ye][xe] != ' '));
        int whih = rand() % 5;
        if(whih == 0)
        {
            EnemyArray[EnemyCount].hp = 5;
            EnemyArray[EnemyCount].move = 0;
            EnemyArray[EnemyCount].type = 'd';
            map[ye][xe] = 'd';
            EnemyArray[EnemyCount].x = xe;
            EnemyArray[EnemyCount].y = ye;
        }
        if(whih == 1)
        {
            EnemyArray[EnemyCount].hp = 10;
            EnemyArray[EnemyCount].move = 0;
            EnemyArray[EnemyCount].type = 'f';
            map[ye][xe] = 'f';
            EnemyArray[EnemyCount].x = xe;
            EnemyArray[EnemyCount].y = ye;
        }
        if(whih == 2)
        {
            EnemyArray[EnemyCount].hp = 15;
            EnemyArray[EnemyCount].move = 0;
            EnemyArray[EnemyCount].type = 't';
            map[ye][xe] = 't';
            EnemyArray[EnemyCount].x = xe;
            EnemyArray[EnemyCount].y = ye;
        }
        if(whih == 3)
        {
            EnemyArray[EnemyCount].hp = 20;
            EnemyArray[EnemyCount].move = 0;
            EnemyArray[EnemyCount].type = 's';
            map[ye][xe] = 's';
            EnemyArray[EnemyCount].x = xe;
            EnemyArray[EnemyCount].y = ye;
        }
        if(whih == 4)
        {
            EnemyArray[EnemyCount].hp = 30;
            EnemyArray[EnemyCount].move = 0;
            EnemyArray[EnemyCount].type = 'u';
            map[ye][xe] = 'u';
            EnemyArray[EnemyCount].x = xe;
            EnemyArray[EnemyCount].y = ye;
        }
        EnemyCount++;
    }
}

void Enemy_Move()
{
    for (int i = 0; i < EnemyCount; i++)
    {
        if (EnemyArray[i].move == 1 && EnemyArray[i].hp > 0)
        {
            //(map[EnemyArray[i].y][EnemyArray[i].x] == 'd' || map[EnemyArray[i].y][EnemyArray[i].x] == 'f' ||
            // map[EnemyArray[i].y][EnemyArray[i].x] == 't' || map[EnemyArray[i].y][EnemyArray[i].x] == 's' ||
            // map[EnemyArray[i].y][EnemyArray[i].x] == 'u')
            if (map[EnemyArray[i].y][EnemyArray[i].x] == 'd' || map[EnemyArray[i].y][EnemyArray[i].x] == 'f' ||
                map[EnemyArray[i].y][EnemyArray[i].x] == 't' || map[EnemyArray[i].y][EnemyArray[i].x] == 's' ||
                map[EnemyArray[i].y][EnemyArray[i].x] == 'u')
            {
                map[EnemyArray[i].y][EnemyArray[i].x] = ' ';
            }
            double distance_up = pow(EnemyArray[i].x - x, 2) + pow(EnemyArray[i].y - 1 - y, 2);
            double distance_down = pow(EnemyArray[i].x - x, 2) + pow(EnemyArray[i].y + 1 - y, 2);
            double distance_right = pow(EnemyArray[i].x + 1 - x, 2) + pow(EnemyArray[i].y - y, 2);
            double distance_left = pow(EnemyArray[i].x - 1 - x, 2) + pow(EnemyArray[i].y - y, 2);
            double distance_up_right = pow(EnemyArray[i].x + 1 - x, 2) + pow(EnemyArray[i].y - 1 - y, 2);
            double distance_up_left = pow(EnemyArray[i].x - 1 - x, 2) + pow(EnemyArray[i].y - 1 - y, 2);
            double distance_down_left = pow(EnemyArray[i].x - 1 - x, 2) + pow(EnemyArray[i].y + 1 - y, 2);
            double distance_down_right = pow(EnemyArray[i].x + 1 - x, 2) + pow(EnemyArray[i].y + 1 - y, 2);
            // up
            if (map[EnemyArray[i].y - 1][EnemyArray[i].x] != '#' && map[EnemyArray[i].y - 1][EnemyArray[i].x] != '%' && map[EnemyArray[i].y - 1][EnemyArray[i].x] != 'O' && map[EnemyArray[i].y - 1][EnemyArray[i].x] != 'd' && map[EnemyArray[i].y - 1][EnemyArray[i].x] != 'f' && map[EnemyArray[i].y - 1][EnemyArray[i].x] != 't' && map[EnemyArray[i].y - 1][EnemyArray[i].x] != 's' && map[EnemyArray[i].y - 1][EnemyArray[i].x] != 'u')
            {
            }
            else
            {
                distance_up += 1000;
            }
            // down
            if (map[EnemyArray[i].y + 1][EnemyArray[i].x] != '#' && map[EnemyArray[i].y + 1][EnemyArray[i].x] != '%' && map[EnemyArray[i].y + 1][EnemyArray[i].x] != 'O' && map[EnemyArray[i].y + 1][EnemyArray[i].x] != 'd' && map[EnemyArray[i].y + 1][EnemyArray[i].x] != 'f' && map[EnemyArray[i].y + 1][EnemyArray[i].x] != 't' && map[EnemyArray[i].y + 1][EnemyArray[i].x] != 's' && map[EnemyArray[i].y + 1][EnemyArray[i].x] != 'u')
            {
            }
            else
            {
                distance_down += 1000;
            }
            // right
            if (map[EnemyArray[i].y][EnemyArray[i].x + 1] != '#' && map[EnemyArray[i].y][EnemyArray[i].x + 1] != '%' && map[EnemyArray[i].y][EnemyArray[i].x + 1] != 'O' && map[EnemyArray[i].y][EnemyArray[i].x + 1] != 'd' && map[EnemyArray[i].y][EnemyArray[i].x + 1] != 'f' && map[EnemyArray[i].y][EnemyArray[i].x + 1] != 't' && map[EnemyArray[i].y][EnemyArray[i].x + 1] != 's' && map[EnemyArray[i].y][EnemyArray[i].x + 1] != 'u')
            {
            }
            else
            {
                distance_right += 1000;
            }
            // left
            if (map[EnemyArray[i].y][EnemyArray[i].x - 1] != '#' && map[EnemyArray[i].y][EnemyArray[i].x - 1] != '%' && map[EnemyArray[i].y][EnemyArray[i].x - 1] != 'O' && map[EnemyArray[i].y][EnemyArray[i].x - 1] != 'd' && map[EnemyArray[i].y][EnemyArray[i].x - 1] != 'f' && map[EnemyArray[i].y][EnemyArray[i].x - 1] != 't' && map[EnemyArray[i].y][EnemyArray[i].x - 1] != 's' && map[EnemyArray[i].y][EnemyArray[i].x - 1] != 'u')
            {
            }
            else
            {
                distance_left += 1000;
            }
            // up right
            if (map[EnemyArray[i].y - 1][EnemyArray[i].x + 1] != '#' && map[EnemyArray[i].y - 1][EnemyArray[i].x + 1] != '%' && map[EnemyArray[i].y - 1][EnemyArray[i].x + 1] != 'O' && map[EnemyArray[i].y - 1][EnemyArray[i].x + 1] != 'd' && map[EnemyArray[i].y - 1][EnemyArray[i].x + 1] != 'f' && map[EnemyArray[i].y - 1][EnemyArray[i].x + 1] != 't' && map[EnemyArray[i].y - 1][EnemyArray[i].x + 1] != 's' && map[EnemyArray[i].y - 1][EnemyArray[i].x + 1] != 'u')
            {
            }
            else
            {
                distance_up_right += 1000;
            }
            // up left
            if (map[EnemyArray[i].y - 1][EnemyArray[i].x - 1] != '#' && map[EnemyArray[i].y - 1][EnemyArray[i].x - 1] != '%' && map[EnemyArray[i].y - 1][EnemyArray[i].x - 1] != 'O' && map[EnemyArray[i].y - 1][EnemyArray[i].x - 1] != 'd' && map[EnemyArray[i].y - 1][EnemyArray[i].x - 1] != 'f' && map[EnemyArray[i].y - 1][EnemyArray[i].x - 1] != 't' && map[EnemyArray[i].y - 1][EnemyArray[i].x - 1] != 's' && map[EnemyArray[i].y - 1][EnemyArray[i].x - 1] != 'u')
            {
            }
            else
            {
                distance_up_left += 1000;
            }
            // down left
            if (map[EnemyArray[i].y + 1][EnemyArray[i].x - 1] != '#' && map[EnemyArray[i].y + 1][EnemyArray[i].x - 1] != '%' && map[EnemyArray[i].y + 1][EnemyArray[i].x - 1] != 'O' && map[EnemyArray[i].y + 1][EnemyArray[i].x - 1] != 'd' && map[EnemyArray[i].y + 1][EnemyArray[i].x - 1] != 'f' && map[EnemyArray[i].y + 1][EnemyArray[i].x - 1] != 't' && map[EnemyArray[i].y + 1][EnemyArray[i].x - 1] != 's' && map[EnemyArray[i].y + 1][EnemyArray[i].x - 1] != 'u')
            {
            }
            else
            {
                distance_down_left += 1000;
            }
            // down right
            if (map[EnemyArray[i].y + 1][EnemyArray[i].x + 1] != '#' && map[EnemyArray[i].y + 1][EnemyArray[i].x + 1] != '%' && map[EnemyArray[i].y + 1][EnemyArray[i].x + 1] != 'O' && map[EnemyArray[i].y + 1][EnemyArray[i].x + 1] != 'd' && map[EnemyArray[i].y + 1][EnemyArray[i].x + 1] != 'f' && map[EnemyArray[i].y + 1][EnemyArray[i].x + 1] != 't' && map[EnemyArray[i].y + 1][EnemyArray[i].x + 1] != 's' && map[EnemyArray[i].y + 1][EnemyArray[i].x + 1] != 'u')
            {
            }
            else
            {
                distance_down_right += 1000;
            }
            // right
            if (distance_right <= distance_up &&
                distance_right <= distance_down &&
                distance_right <= distance_right &&
                distance_right <= distance_left &&
                distance_right <= distance_up_right &&
                distance_right <= distance_up_left &&
                distance_right <= distance_down_left &&
                distance_right <= distance_down_right &&
                map[EnemyArray[i].y][EnemyArray[i].x + 1] != '#' && map[EnemyArray[i].y][EnemyArray[i].x + 1] != '%' && map[EnemyArray[i].y][EnemyArray[i].x + 1] != 'O' && map[EnemyArray[i].y][EnemyArray[i].x + 1] != 'd' && map[EnemyArray[i].y][EnemyArray[i].x + 1] != 'f' && map[EnemyArray[i].y][EnemyArray[i].x + 1] != 't' && map[EnemyArray[i].y][EnemyArray[i].x + 1] != 's' && map[EnemyArray[i].y][EnemyArray[i].x + 1] != 'u')
            {
                if (distance_right == 0)
                {
                    strcpy(Line1, "You have been attacked!                                             ");
                    if (playerHP >= 50)
                    {
                        playerHP -= 50;
                    }
                    else
                    {
                        playerHP = 0;
                    }
                    if (map[EnemyArray[i].y][EnemyArray[i].x] == ' ')
                    {
                        map[EnemyArray[i].y][EnemyArray[i].x] = EnemyArray[i].type;
                    }
                }
                else
                {
                    EnemyArray[i].x = EnemyArray[i].x + 1;
                    if (map[EnemyArray[i].y][EnemyArray[i].x] == ' ')
                    {
                        map[EnemyArray[i].y][EnemyArray[i].x] = EnemyArray[i].type;
                    }
                }
            }
            else
            {
                // left
                if (distance_left <= distance_up &&
                    distance_left <= distance_down &&
                    distance_left <= distance_left &&
                    distance_left <= distance_up_right &&
                    distance_left <= distance_up_left &&
                    distance_left <= distance_down_left &&
                    distance_left <= distance_down_right &&
                    map[EnemyArray[i].y][EnemyArray[i].x - 1] != '#' && map[EnemyArray[i].y][EnemyArray[i].x - 1] != '%' && map[EnemyArray[i].y][EnemyArray[i].x - 1] != 'O' && map[EnemyArray[i].y][EnemyArray[i].x - 1] != 'd' && map[EnemyArray[i].y][EnemyArray[i].x - 1] != 'f' && map[EnemyArray[i].y][EnemyArray[i].x - 1] != 't' && map[EnemyArray[i].y][EnemyArray[i].x - 1] != 's' && map[EnemyArray[i].y][EnemyArray[i].x - 1] != 'u')
                {
                    if (distance_left == 0)
                    {
                        strcpy(Line1, "You have been attacked!                                     ");
                        if (playerHP >= 50)
                        {
                            playerHP -= 50;
                        }
                        else
                        {
                            playerHP = 0;
                        }
                        if (map[EnemyArray[i].y][EnemyArray[i].x] == ' ')
                        {
                            map[EnemyArray[i].y][EnemyArray[i].x] = EnemyArray[i].type;
                        }
                    }
                    else
                    {
                        EnemyArray[i].x = EnemyArray[i].x - 1;
                        if (map[EnemyArray[i].y][EnemyArray[i].x] == ' ')
                        {
                            map[EnemyArray[i].y][EnemyArray[i].x] = EnemyArray[i].type;
                        }
                    }
                }
                else
                {
                    // up
                    if (distance_up <= distance_up &&
                        distance_up <= distance_down &&
                        distance_up <= distance_up_right &&
                        distance_up <= distance_up_left &&
                        distance_up <= distance_down_left &&
                        distance_up <= distance_down_right &&
                        map[EnemyArray[i].y - 1][EnemyArray[i].x] != '#' && map[EnemyArray[i].y - 1][EnemyArray[i].x] != '%' && map[EnemyArray[i].y - 1][EnemyArray[i].x] != 'O' && map[EnemyArray[i].y - 1][EnemyArray[i].x] != 'd' && map[EnemyArray[i].y - 1][EnemyArray[i].x] != 'f' && map[EnemyArray[i].y - 1][EnemyArray[i].x] != 't' && map[EnemyArray[i].y - 1][EnemyArray[i].x] != 's' && map[EnemyArray[i].y - 1][EnemyArray[i].x] != 'u')
                    {
                        if (distance_up == 0)
                        {
                            strcpy(Line1, "You have been attacked!                       ");
                            if (playerHP >= 50)
                            {
                                playerHP -= 50;
                            }
                            else
                            {
                                playerHP = 0;
                            }
                            if (map[EnemyArray[i].y][EnemyArray[i].x] == ' ')
                            {
                                map[EnemyArray[i].y][EnemyArray[i].x] = EnemyArray[i].type;
                            }
                        }
                        else
                        {
                            EnemyArray[i].y = EnemyArray[i].y - 1;
                            if (map[EnemyArray[i].y][EnemyArray[i].x] == ' ')
                            {
                                map[EnemyArray[i].y][EnemyArray[i].x] = EnemyArray[i].type;
                            }
                        }
                    }
                    else
                    {
                        // down
                        if (distance_down <= distance_down &&
                            distance_down <= distance_up_right &&
                            distance_down <= distance_up_left &&
                            distance_down <= distance_down_left &&
                            distance_down <= distance_down_right &&
                            map[EnemyArray[i].y + 1][EnemyArray[i].x] != '#' && map[EnemyArray[i].y + 1][EnemyArray[i].x] != '%' && map[EnemyArray[i].y + 1][EnemyArray[i].x] != 'O' && map[EnemyArray[i].y + 1][EnemyArray[i].x] != 'd' && map[EnemyArray[i].y + 1][EnemyArray[i].x] != 'f' && map[EnemyArray[i].y + 1][EnemyArray[i].x] != 't' && map[EnemyArray[i].y + 1][EnemyArray[i].x] != 's' && map[EnemyArray[i].y + 1][EnemyArray[i].x] != 'u')
                        {
                            if (distance_down == 0)
                            {
                                strcpy(Line1, "You have been attacked!                             ");
                                if (playerHP >= 50)
                                {
                                    playerHP -= 50;
                                }
                                else
                                {
                                    playerHP = 0;
                                }
                                if (map[EnemyArray[i].y][EnemyArray[i].x] == ' ')
                                {
                                    map[EnemyArray[i].y][EnemyArray[i].x] = EnemyArray[i].type;
                                }
                            }
                            else
                            {
                                EnemyArray[i].y = EnemyArray[i].y + 1;
                                if (map[EnemyArray[i].y][EnemyArray[i].x] == ' ')
                                {
                                    map[EnemyArray[i].y][EnemyArray[i].x] = EnemyArray[i].type;
                                }
                            }
                        }
                        else
                        {
                            // up right
                            if (distance_up_right <= distance_up_right &&
                                distance_up_right <= distance_up_left &&
                                distance_up_right <= distance_down_left &&
                                distance_up_right <= distance_down_right &&
                                map[EnemyArray[i].y - 1][EnemyArray[i].x + 1] != '#' && map[EnemyArray[i].y - 1][EnemyArray[i].x + 1] != '%' && map[EnemyArray[i].y - 1][EnemyArray[i].x + 1] != 'O' && map[EnemyArray[i].y - 1][EnemyArray[i].x + 1] != 'd' && map[EnemyArray[i].y - 1][EnemyArray[i].x + 1] != 'f' && map[EnemyArray[i].y - 1][EnemyArray[i].x + 1] != 't' && map[EnemyArray[i].y - 1][EnemyArray[i].x + 1] != 's' && map[EnemyArray[i].y - 1][EnemyArray[i].x + 1] != 'u')
                            {
                                if (distance_up_right == 0)
                                {
                                    strcpy(Line1, "You have been attacked!                               ");
                                    if (playerHP >= 50)
                                    {
                                        playerHP -= 50;
                                    }
                                    else
                                    {
                                        playerHP = 0;
                                    }
                                    if (map[EnemyArray[i].y][EnemyArray[i].x] == ' ')
                                    {
                                        map[EnemyArray[i].y][EnemyArray[i].x] = EnemyArray[i].type;
                                    }
                                }
                                else
                                {
                                    EnemyArray[i].x = EnemyArray[i].x + 1;
                                    EnemyArray[i].y = EnemyArray[i].y - 1;
                                    if (map[EnemyArray[i].y][EnemyArray[i].x] == ' ')
                                    {
                                        map[EnemyArray[i].y][EnemyArray[i].x] = EnemyArray[i].type;
                                    }
                                }
                            }
                            else
                            {
                                // up left
                                if (distance_up_left <= distance_up_left &&
                                    distance_up_left <= distance_down_left &&
                                    distance_up_left <= distance_down_right &&
                                    map[EnemyArray[i].y - 1][EnemyArray[i].x - 1] != '#' && map[EnemyArray[i].y - 1][EnemyArray[i].x - 1] != '%' && map[EnemyArray[i].y - 1][EnemyArray[i].x - 1] != 'O' && map[EnemyArray[i].y - 1][EnemyArray[i].x - 1] != 'd' && map[EnemyArray[i].y - 1][EnemyArray[i].x - 1] != 'f' && map[EnemyArray[i].y - 1][EnemyArray[i].x - 1] != 't' && map[EnemyArray[i].y - 1][EnemyArray[i].x - 1] != 's' && map[EnemyArray[i].y - 1][EnemyArray[i].x - 1] != 'u')
                                {
                                    if (distance_up_left == 0)
                                    {
                                        strcpy(Line1, "You have been attacked!                                  ");
                                        if (playerHP >= 50)
                                        {
                                            playerHP -= 50;
                                        }
                                        else
                                        {
                                            playerHP = 0;
                                        }
                                        if (map[EnemyArray[i].y][EnemyArray[i].x] == ' ')
                                        {
                                            map[EnemyArray[i].y][EnemyArray[i].x] = EnemyArray[i].type;
                                        }
                                    }
                                    else
                                    {
                                        EnemyArray[i].x--;
                                        EnemyArray[i].y--;
                                        if (map[EnemyArray[i].y][EnemyArray[i].x] == ' ')
                                        {
                                            map[EnemyArray[i].y][EnemyArray[i].x] = EnemyArray[i].type;
                                        }
                                    }
                                }
                                else
                                {
                                    // down left
                                    if (distance_down_left <= distance_down_left &&
                                        distance_down_left <= distance_down_right &&
                                        map[EnemyArray[i].y + 1][EnemyArray[i].x - 1] != '#' && map[EnemyArray[i].y + 1][EnemyArray[i].x - 1] != '%' && map[EnemyArray[i].y + 1][EnemyArray[i].x - 1] != 'O' && map[EnemyArray[i].y + 1][EnemyArray[i].x - 1] != 'd' && map[EnemyArray[i].y + 1][EnemyArray[i].x - 1] != 'f' && map[EnemyArray[i].y + 1][EnemyArray[i].x - 1] != 't' && map[EnemyArray[i].y + 1][EnemyArray[i].x - 1] != 's' && map[EnemyArray[i].y + 1][EnemyArray[i].x - 1] != 'u')
                                    {
                                        if (distance_down_left == 0)
                                        {
                                            strcpy(Line1, "You have been attacked!                               ");
                                            if (playerHP >= 50)
                                            {
                                                playerHP -= 50;
                                            }
                                            else
                                            {
                                                playerHP = 0;
                                            }
                                            if (map[EnemyArray[i].y][EnemyArray[i].x] == ' ')
                                            {
                                                map[EnemyArray[i].y][EnemyArray[i].x] = EnemyArray[i].type;
                                            }
                                        }
                                        else
                                        {
                                            EnemyArray[i].x--;
                                            EnemyArray[i].y++;
                                            if (map[EnemyArray[i].y][EnemyArray[i].x] == ' ')
                                            {
                                                map[EnemyArray[i].y][EnemyArray[i].x] = EnemyArray[i].type;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        // down right
                                        if (distance_down_right <= distance_down_right &&
                                            map[EnemyArray[i].y + 1][EnemyArray[i].x + 1] != '#' && map[EnemyArray[i].y + 1][EnemyArray[i].x + 1] != '%' && map[EnemyArray[i].y + 1][EnemyArray[i].x + 1] != 'O' && map[EnemyArray[i].y + 1][EnemyArray[i].x + 1] != 'd' && map[EnemyArray[i].y + 1][EnemyArray[i].x + 1] != 'f' && map[EnemyArray[i].y + 1][EnemyArray[i].x + 1] != 't' && map[EnemyArray[i].y + 1][EnemyArray[i].x + 1] != 's' && map[EnemyArray[i].y + 1][EnemyArray[i].x + 1] != 'u')
                                        {
                                            if (distance_down_right == 0)
                                            {
                                                strcpy(Line1, "You have been attacked!                            ");
                                                if (playerHP >= 50)
                                                {
                                                    playerHP -= 50;
                                                }
                                                else
                                                {
                                                    playerHP = 0;
                                                }
                                                if (map[EnemyArray[i].y][EnemyArray[i].x] == ' ')
                                                {
                                                    map[EnemyArray[i].y][EnemyArray[i].x] = EnemyArray[i].type;
                                                }
                                            }
                                            else
                                            {
                                                EnemyArray[i].y++;
                                                EnemyArray[i].x++;
                                                if (map[EnemyArray[i].y][EnemyArray[i].x] == ' ')
                                                {
                                                    map[EnemyArray[i].y][EnemyArray[i].x] = EnemyArray[i].type;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void Lose()
{
    clear();
    printw("Lost");
    char cc = getch();
    endwin();
    exit(0);
}

void Won()
{
    clear();
    printw("Won");
    char cc = getch();
    endwin();
    exit(0);
}

void battle(int yy,int xx)
{
    strcpy(Line1,"Fight!                                                                   ");
    double attack;
    if(Current_Weapon == 'm')
    {
        /*for(int i = 0 ; i < WeaponCount;i++)
        {
            if(WeaponArray[i].mode == 1)
            {

            }
        }*/
        attack = 5;
    }
    /*if(Current_Weapon == 'D')
    {
        for(int i = 0 ; i < WeaponCount;i++)
        {
            if(WeaponArray[i].mode == 2)
            {
                WeaponArray[i].times_use_left--;
                break;
            }
        }
        attack = 12;
    }
    if(Current_Weapon == 'M')
    {
        for(int i = 0 ; i < WeaponCount;i++)
        {
            if(WeaponArray[i].mode == 3)
            {
                WeaponArray[i].times_use_left--;
                break;
            }
        }
        attack = 15;
    }
    if(Current_Weapon == 'N')
    {
        for(int i = 0 ; i < WeaponCount;i++)
        {
            if(WeaponArray[i].mode == 4)
            {
                WeaponArray[i].times_use_left--;
                break;
            }
        }
        attack = 5;
    }*/
    if(Current_Weapon == 'S')
    {
        /*for(int i = 0 ; i < WeaponCount;i++)
        {
            if(WeaponArray[i].mode == 5)
            {
                
            }
        }*/
        attack = 10;
    }
    if(Current_Weapon == 'm' || Current_Weapon == 'S')
    {
        attack *= power;
        for (int i = 0; i < EnemyCount; i++)
        {
            if (EnemyArray[i].x == xx && EnemyArray[i].y == yy && EnemyArray[i].hp > 0)
            {
                if (EnemyArray[i].hp >= attack)
                {
                    EnemyArray[i].hp -= attack;
                    if (EnemyArray[i].hp == 0)
                    {
                        strcpy(Line1, "Enemy Killed!");
                        map[yy][xx] = ' ';
                    }
                    if (playerHP >= 50)
                    {
                        playerHP -= 50;
                    }
                    else
                    {
                        playerHP = 0;
                    }
                }
                else
                {
                    strcpy(Line1, "Enemy Killed!");
                    EnemyArray[i].hp = 0;
                    map[yy][xx] = ' ';
                    if (playerHP >= 50)
                    {
                        playerHP -= 50;
                    }
                    else
                    {
                        playerHP = 0;
                    }
                }
            }
        }
    }
}

int Check_Enemy(int yy,int xx)
{
    for(int i = 0 ; i < EnemyCount;i++)
    {
        if(EnemyArray[i].x == xx && EnemyArray[i].y == yy && EnemyArray[i].hp > 0)
        {
            return 0;
        }
    }
    return 1;
}

void Far_Battle()
{
    if (Current_Weapon == 'M')
    {
        int attack = 15 * power;
        int have = 0;
        for(int i = 0 ; i < WeaponCount;i++)
        {
            if(WeaponArray[i].mode == 3)
            {
                WeaponArray[i].times_use_left--;
                have = 1;
                break;
            }
        }
        if(have == 0)
        {
            strcpy(Line1,"You don't have a Weapon!                     ");
            return;
        }
        int cc = getch();
        if (cc == KEY_UP)
        {
            for(int i = y - 1; i >= y - 5; i--)
            {
                //if hit enemy
                if(Check_Enemy(i,x) == 0)  
                {
                    strcpy(Line1,"You hit Enemy!                                                        ");
                    int index = -1;
                    for(int j = 0 ; j < EnemyCount;j++)
                    {
                        if(EnemyArray[j].x == x && EnemyArray[j].y == i && EnemyArray[j].hp>0)
                        {
                            index = j;
                            break;
                        }
                    }
                    EnemyArray[index].move = -1;
                    if (EnemyArray[index].hp >= attack)
                    {
                        EnemyArray[index].hp -= attack;
                        if (EnemyArray[index].hp == 0)
                        {
                            strcpy(Line1, "Enemy Killed!                                      ");
                            int which = 0;
                            for(int k = 0 ; k < Corridor_Placed;k++)
                            {
                                if(i == CorridorArray[k].y && x == CorridorArray[k].x)
                                {
                                    which = 1;
                                    break;
                                }
                            }
                            if(which == 0)
                            {
                                map[i][x] = ' ';
                            }
                            else
                            {
                                map[i][x] = '$';
                            }
                        }
                    }
                    else
                    {
                        strcpy(Line1, "Enemy Killed!                                                     ");
                        EnemyArray[index].hp = 0;
                    }
                    break;
                }
                //if hit wall
                if(map[i][x] == '#' || map[i][x] == 'O' || map[i][x] == '%')
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = x;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = i+1;
                    Wasted_Weapon_Count++;
                    map[i+1][x] = '3';
                    break;
                }
                //if range is over
                if(i == y - 5)
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = x;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = i;
                    Wasted_Weapon_Count++;
                    map[i][x] = '3';
                }
            }
        }
        if (cc == KEY_DOWN)
        {
            for(int i = y + 1; i <= y + 5; i++)
            {
                //if hit enemy
                if(Check_Enemy(i,x) == 0)  
                {
                    strcpy(Line1,"You hit Enemy!                                                        ");
                    int index = -1;
                    for(int j = 0 ; j < EnemyCount;j++)
                    {
                        if(EnemyArray[j].x == x && EnemyArray[j].y == i && EnemyArray[j].hp>0)
                        {
                            index = j;
                            break;
                        }
                    }
                    EnemyArray[index].move = -1;
                    if (EnemyArray[index].hp >= attack)
                    {
                        EnemyArray[index].hp -= attack;
                        if (EnemyArray[index].hp == 0)
                        {
                            strcpy(Line1, "Enemy Killed!                                      ");
                            int which = 0;
                            for(int k = 0 ; k < Corridor_Placed;k++)
                            {
                                if(i == CorridorArray[k].y && x == CorridorArray[k].x)
                                {
                                    which = 1;
                                    break;
                                }
                            }
                            if(which == 0)
                            {
                                map[i][x] = ' ';
                            }
                            else
                            {
                                map[i][x] = '$';
                            }
                        }
                    }
                    else
                    {
                        strcpy(Line1, "Enemy Killed!                                                     ");
                        EnemyArray[index].hp = 0;
                    }
                    break;
                }
                //if hit wall
                if(map[i][x] == '#' || map[i][x] == 'O' || map[i][x] == '%')
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = x;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = i-1;
                    Wasted_Weapon_Count++;
                    map[i-1][x] = '3';
                    break;
                }
                //if range is over
                if(i == y + 5)
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = x;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = i;
                    Wasted_Weapon_Count++;
                    map[i][x] = '3';
                }
            }
        }
        if (cc == KEY_RIGHT)
        {
            for(int i = x + 1; i <= x + 5; i++)
            {
                //if hit enemy
                if(Check_Enemy(y,i) == 0)  
                {
                    strcpy(Line1,"You hit Enemy!                                                        ");
                    int index = -1;
                    for(int j = 0 ; j < EnemyCount;j++)
                    {
                        if(EnemyArray[j].x == i && EnemyArray[j].y == y && EnemyArray[j].hp>0)
                        {
                            index = j;
                            break;
                        }
                    }
                    EnemyArray[index].move = -1;
                    if (EnemyArray[index].hp >= attack)
                    {
                        EnemyArray[index].hp -= attack;
                        if (EnemyArray[index].hp == 0)
                        {
                            strcpy(Line1, "Enemy Killed!                                      ");
                            int which = 0;
                            for(int k = 0 ; k < Corridor_Placed;k++)
                            {
                                if(y == CorridorArray[k].y && i == CorridorArray[k].x)
                                {
                                    which = 1;
                                    break;
                                }
                            }
                            if(which == 0)
                            {
                                map[y][i] = ' ';
                            }
                            else
                            {
                                map[y][i] = '$';
                            }
                        }
                    }
                    else
                    {
                        strcpy(Line1, "Enemy Killed!                                                     ");
                        EnemyArray[index].hp = 0;
                    }
                    break;
                }
                //if hit wall
                if(map[y][i] == '#' || map[y][i] == 'O' || map[y][i] == '%')
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = i-1;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = y;
                    Wasted_Weapon_Count++;
                    map[y][i-1] = '3';
                    break;
                }
                //if range is over
                if(i == x + 5)
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = i;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = y;
                    Wasted_Weapon_Count++;
                    map[y][i] = '3';
                }
            }
        }
        if (cc == KEY_LEFT)
        {
            for(int i = x - 1; i >= x - 5; i--)
            {
                //if hit enemy
                if(Check_Enemy(y,i) == 0)  
                {
                    strcpy(Line1,"You hit Enemy!                                                        ");
                    int index = -1;
                    for(int j = 0 ; j < EnemyCount;j++)
                    {
                        if(EnemyArray[j].x == i && EnemyArray[j].y == y && EnemyArray[j].hp>0)
                        {
                            index = j;
                            break;
                        }
                    }
                    EnemyArray[index].move = -1;
                    if (EnemyArray[index].hp >= attack)
                    {
                        EnemyArray[index].hp -= attack;
                        if (EnemyArray[index].hp == 0)
                        {
                            strcpy(Line1, "Enemy Killed!                                      ");
                            int which = 0;
                            for(int k = 0 ; k < Corridor_Placed;k++)
                            {
                                if(y == CorridorArray[k].y && i == CorridorArray[k].x)
                                {
                                    which = 1;
                                    break;
                                }
                            }
                            if(which == 0)
                            {
                                map[y][i] = ' ';
                            }
                            else
                            {
                                map[y][i] = '$';
                            }
                        }
                    }
                    else
                    {
                        strcpy(Line1, "Enemy Killed!                                                     ");
                        EnemyArray[index].hp = 0;
                    }
                    break;
                }
                //if hit wall
                if(map[y][i] == '#' || map[y][i] == 'O' || map[y][i] == '%')
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = i+1;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = y;
                    Wasted_Weapon_Count++;
                    map[y][i+1] = '3';
                    break;
                }
                //if range is over
                if(i == x - 5)
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = i;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = y;
                    Wasted_Weapon_Count++;
                    map[y][i] = '3';
                }
            }
        }
    }
    if (Current_Weapon == 'D')
    {
        int attack = 12 * power;
        int have = 0;
        for(int i = 0 ; i < WeaponCount;i++)
        {
            if(WeaponArray[i].mode == 2)
            {
                WeaponArray[i].times_use_left--;
                have = 1;
                break;
            }
        }
        if(have == 0)
        {
            strcpy(Line1,"You don't have a Weapon!                     ");
            return;
        }
        int cc = getch();
        if (cc == KEY_UP)
        {
            for(int i = y - 1; i >= y - 5; i--)
            {
                //if hit enemy
                if(Check_Enemy(i,x) == 0)  
                {
                    strcpy(Line1,"You hit Enemy!                                                        ");
                    int index = -1;
                    for(int j = 0 ; j < EnemyCount;j++)
                    {
                        if(EnemyArray[j].x == x && EnemyArray[j].y == i && EnemyArray[j].hp>0)
                        {
                            index = j;
                            break;
                        }
                    }
                    if (EnemyArray[index].hp >= attack)
                    {
                        EnemyArray[index].hp -= attack;
                        if (EnemyArray[index].hp == 0)
                        {
                            strcpy(Line1, "Enemy Killed!                                      ");
                            int which = 0;
                            for(int k = 0 ; k < Corridor_Placed;k++)
                            {
                                if(i == CorridorArray[k].y && x == CorridorArray[k].x)
                                {
                                    which = 1;
                                    break;
                                }
                            }
                            if(which == 0)
                            {
                                map[i][x] = ' ';
                            }
                            else
                            {
                                map[i][x] = '$';
                            }
                        }
                    }
                    else
                    {
                        strcpy(Line1, "Enemy Killed!                                                     ");
                        EnemyArray[index].hp = 0;
                    }
                    break;
                }
                //if hit wall
                if(map[i][x] == '#' || map[i][x] == 'O' || map[i][x] == '%')
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = x;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = i+1;
                    Wasted_Weapon_Count++;
                    map[i+1][x] = '2';
                    break;
                }
                //if range is over
                if(i == y - 5)
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = x;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = i;
                    Wasted_Weapon_Count++;
                    map[i][x] = '2';
                }
            }
        }
        if (cc == KEY_DOWN)
        {
            for(int i = y + 1; i <= y + 5; i++)
            {
                //if hit enemy
                if(Check_Enemy(i,x) == 0)  
                {
                    strcpy(Line1,"You hit Enemy!                                                        ");
                    int index = -1;
                    for(int j = 0 ; j < EnemyCount;j++)
                    {
                        if(EnemyArray[j].x == x && EnemyArray[j].y == i && EnemyArray[j].hp>0)
                        {
                            index = j;
                            break;
                        }
                    }
                    if (EnemyArray[index].hp >= attack)
                    {
                        EnemyArray[index].hp -= attack;
                        if (EnemyArray[index].hp == 0)
                        {
                            strcpy(Line1, "Enemy Killed!                                      ");
                            int which = 0;
                            for(int k = 0 ; k < Corridor_Placed;k++)
                            {
                                if(i == CorridorArray[k].y && x == CorridorArray[k].x)
                                {
                                    which = 1;
                                    break;
                                }
                            }
                            if(which == 0)
                            {
                                map[i][x] = ' ';
                            }
                            else
                            {
                                map[i][x] = '$';
                            }
                        }
                    }
                    else
                    {
                        strcpy(Line1, "Enemy Killed!                                                     ");
                        EnemyArray[index].hp = 0;
                    }
                    break;
                }
                //if hit wall
                if(map[i][x] == '#' || map[i][x] == 'O' || map[i][x] == '%')
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = x;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = i-1;
                    Wasted_Weapon_Count++;
                    map[i-1][x] = '2';
                    break;
                }
                //if range is over
                if(i == y + 5)
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = x;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = i;
                    Wasted_Weapon_Count++;
                    map[i][x] = '2';
                }
            }
        }
        if (cc == KEY_RIGHT)
        {
            for(int i = x + 1; i <= x + 5; i++)
            {
                //if hit enemy
                if(Check_Enemy(y,i) == 0)  
                {
                    strcpy(Line1,"You hit Enemy!                                                        ");
                    int index = -1;
                    for(int j = 0 ; j < EnemyCount;j++)
                    {
                        if(EnemyArray[j].x == i && EnemyArray[j].y == y && EnemyArray[j].hp>0)
                        {
                            index = j;
                            break;
                        }
                    }
                    if (EnemyArray[index].hp >= attack)
                    {
                        EnemyArray[index].hp -= attack;
                        if (EnemyArray[index].hp == 0)
                        {
                            strcpy(Line1, "Enemy Killed!                                      ");
                            int which = 0;
                            for(int k = 0 ; k < Corridor_Placed;k++)
                            {
                                if(y == CorridorArray[k].y && i == CorridorArray[k].x)
                                {
                                    which = 1;
                                    break;
                                }
                            }
                            if(which == 0)
                            {
                                map[y][i] = ' ';
                            }
                            else
                            {
                                map[y][i] = '$';
                            }
                        }
                    }
                    else
                    {
                        strcpy(Line1, "Enemy Killed!                                                     ");
                        EnemyArray[index].hp = 0;
                    }
                    break;
                }
                //if hit wall
                if(map[y][i] == '#' || map[y][i] == 'O' || map[y][i] == '%')
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = i-1;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = y;
                    Wasted_Weapon_Count++;
                    map[y][i-1] = '2';
                    break;
                }
                //if range is over
                if(i == x + 5)
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = i;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = y;
                    Wasted_Weapon_Count++;
                    map[y][i] = '2';
                }
            }
        }
        if (cc == KEY_LEFT)
        {
            for(int i = x - 1; i >= x - 5; i--)
            {
                //if hit enemy
                if(Check_Enemy(y,i) == 0)  
                {
                    strcpy(Line1,"You hit Enemy!                                                        ");
                    int index = -1;
                    for(int j = 0 ; j < EnemyCount;j++)
                    {
                        if(EnemyArray[j].x == i && EnemyArray[j].y == y && EnemyArray[j].hp>0)
                        {
                            index = j;
                            break;
                        }
                    }
                    if (EnemyArray[index].hp >= attack)
                    {
                        EnemyArray[index].hp -= attack;
                        if (EnemyArray[index].hp == 0)
                        {
                            strcpy(Line1, "Enemy Killed!                                      ");
                            int which = 0;
                            for(int k = 0 ; k < Corridor_Placed;k++)
                            {
                                if(y == CorridorArray[k].y && i == CorridorArray[k].x)
                                {
                                    which = 1;
                                    break;
                                }
                            }
                            if(which == 0)
                            {
                                map[y][i] = ' ';
                            }
                            else
                            {
                                map[y][i] = '$';
                            }
                        }
                    }
                    else
                    {
                        strcpy(Line1, "Enemy Killed!                                                     ");
                        EnemyArray[index].hp = 0;
                    }
                    break;
                }
                //if hit wall
                if(map[y][i] == '#' || map[y][i] == 'O' || map[y][i] == '%')
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = i+1;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = y;
                    Wasted_Weapon_Count++;
                    map[y][i+1] = '2';
                    break;
                }
                //if range is over
                if(i == x - 5)
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = i;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = y;
                    Wasted_Weapon_Count++;
                    map[y][i] = '2';
                }
            }
        }
    }
    if (Current_Weapon == 'N')
    {
        int attack = 5 * power;
        int have = 0;
        for(int i = 0 ; i < WeaponCount;i++)
        {
            if(WeaponArray[i].mode == 4)
            {
                WeaponArray[i].times_use_left--;
                have = 1;
                break;
            }
        }
        if(have == 0)
        {
            strcpy(Line1,"You don't have a Weapon!                     ");
            return;
        }
        int cc = getch();
        if (cc == KEY_UP)
        {
            for(int i = y - 1; i >= y - 5; i--)
            {
                //if hit enemy
                if(Check_Enemy(i,x) == 0)  
                {
                    strcpy(Line1,"You hit Enemy!                                                        ");
                    int index = -1;
                    for(int j = 0 ; j < EnemyCount;j++)
                    {
                        if(EnemyArray[j].x == x && EnemyArray[j].y == i && EnemyArray[j].hp>0)
                        {
                            index = j;
                            break;
                        }
                    }
                    if (EnemyArray[index].hp >= attack)
                    {
                        EnemyArray[index].hp -= attack;
                        if (EnemyArray[index].hp == 0)
                        {
                            strcpy(Line1, "Enemy Killed!                                      ");
                            int which = 0;
                            for(int k = 0 ; k < Corridor_Placed;k++)
                            {
                                if(i == CorridorArray[k].y && x == CorridorArray[k].x)
                                {
                                    which = 1;
                                    break;
                                }
                            }
                            if(which == 0)
                            {
                                map[i][x] = ' ';
                            }
                            else
                            {
                                map[i][x] = '$';
                            }
                        }
                    }
                    else
                    {
                        strcpy(Line1, "Enemy Killed!                                                     ");
                        EnemyArray[index].hp = 0;
                    }
                    break;
                }
                //if hit wall
                if(map[i][x] == '#' || map[i][x] == 'O' || map[i][x] == '%')
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = x;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = i+1;
                    Wasted_Weapon_Count++;
                    map[i+1][x] = '4';
                    break;
                }
                //if range is over
                if(i == y - 5)
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = x;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = i;
                    Wasted_Weapon_Count++;
                    map[i][x] = '4';
                }
            }
        }
        if (cc == KEY_DOWN)
        {
            for(int i = y + 1; i <= y + 5; i++)
            {
                //if hit enemy
                if(Check_Enemy(i,x) == 0)  
                {
                    strcpy(Line1,"You hit Enemy!                                                        ");
                    int index = -1;
                    for(int j = 0 ; j < EnemyCount;j++)
                    {
                        if(EnemyArray[j].x == x && EnemyArray[j].y == i && EnemyArray[j].hp>0)
                        {
                            index = j;
                            break;
                        }
                    }
                    if (EnemyArray[index].hp >= attack)
                    {
                        EnemyArray[index].hp -= attack;
                        if (EnemyArray[index].hp == 0)
                        {
                            strcpy(Line1, "Enemy Killed!                                      ");
                            int which = 0;
                            for(int k = 0 ; k < Corridor_Placed;k++)
                            {
                                if(i == CorridorArray[k].y && x == CorridorArray[k].x)
                                {
                                    which = 1;
                                    break;
                                }
                            }
                            if(which == 0)
                            {
                                map[i][x] = ' ';
                            }
                            else
                            {
                                map[i][x] = '$';
                            }
                        }
                    }
                    else
                    {
                        strcpy(Line1, "Enemy Killed!                                                     ");
                        EnemyArray[index].hp = 0;
                    }
                    break;
                }
                //if hit wall
                if(map[i][x] == '#' || map[i][x] == 'O' || map[i][x] == '%')
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = x;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = i-1;
                    Wasted_Weapon_Count++;
                    map[i-1][x] = '4';
                    break;
                }
                //if range is over
                if(i == y + 5)
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = x;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = i;
                    Wasted_Weapon_Count++;
                    map[i][x] = '4';
                }
            }
        }
        if (cc == KEY_RIGHT)
        {
            for(int i = x + 1; i <= x + 5; i++)
            {
                //if hit enemy
                if(Check_Enemy(y,i) == 0)  
                {
                    strcpy(Line1,"You hit Enemy!                                                        ");
                    int index = -1;
                    for(int j = 0 ; j < EnemyCount;j++)
                    {
                        if(EnemyArray[j].x == i && EnemyArray[j].y == y && EnemyArray[j].hp>0)
                        {
                            index = j;
                            break;
                        }
                    }
                    if (EnemyArray[index].hp >= attack)
                    {
                        EnemyArray[index].hp -= attack;
                        if (EnemyArray[index].hp == 0)
                        {
                            strcpy(Line1, "Enemy Killed!                                      ");
                            int which = 0;
                            for(int k = 0 ; k < Corridor_Placed;k++)
                            {
                                if(y == CorridorArray[k].y && i == CorridorArray[k].x)
                                {
                                    which = 1;
                                    break;
                                }
                            }
                            if(which == 0)
                            {
                                map[y][i] = ' ';
                            }
                            else
                            {
                                map[y][i] = '$';
                            }
                        }
                    }
                    else
                    {
                        strcpy(Line1, "Enemy Killed!                                                     ");
                        EnemyArray[index].hp = 0;
                    }
                    break;
                }
                //if hit wall
                if(map[y][i] == '#' || map[y][i] == 'O' || map[y][i] == '%')
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = i-1;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = y;
                    Wasted_Weapon_Count++;
                    map[y][i-1] = '4';
                    break;
                }
                //if range is over
                if(i == x + 5)
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = i;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = y;
                    Wasted_Weapon_Count++;
                    map[y][i] = '4';
                }
            }
        }
        if (cc == KEY_LEFT)
        {
            for(int i = x - 1; i >= x - 5; i--)
            {
                //if hit enemy
                if(Check_Enemy(y,i) == 0)  
                {
                    strcpy(Line1,"You hit Enemy!                                                        ");
                    int index = -1;
                    for(int j = 0 ; j < EnemyCount;j++)
                    {
                        if(EnemyArray[j].x == i && EnemyArray[j].y == y && EnemyArray[j].hp>0)
                        {
                            index = j;
                            break;
                        }
                    }
                    if (EnemyArray[index].hp >= attack)
                    {
                        EnemyArray[index].hp -= attack;
                        if (EnemyArray[index].hp == 0)
                        {
                            strcpy(Line1, "Enemy Killed!                                      ");
                            int which = 0;
                            for(int k = 0 ; k < Corridor_Placed;k++)
                            {
                                if(y == CorridorArray[k].y && i == CorridorArray[k].x)
                                {
                                    which = 1;
                                    break;
                                }
                            }
                            if(which == 0)
                            {
                                map[y][i] = ' ';
                            }
                            else
                            {
                                map[y][i] = '$';
                            }
                        }
                    }
                    else
                    {
                        strcpy(Line1, "Enemy Killed!                                                     ");
                        EnemyArray[index].hp = 0;
                    }
                    break;
                }
                //if hit wall
                if(map[y][i] == '#' || map[y][i] == 'O' || map[y][i] == '%')
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = i+1;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = y;
                    Wasted_Weapon_Count++;
                    map[y][i+1] = '4';
                    break;
                }
                //if range is over
                if(i == x - 5)
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = i;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = y;
                    Wasted_Weapon_Count++;
                    map[y][i] = '4';
                }
            }
        }
    }
}

struct timeval start, end;
int MakeGame(int c)
{
    // Generate Map
    if (Room_Placed == 0)
    {
        clear();
        MakeRoom();
    }
    // New Room message
    if (c == 113)
    {
        strcpy(Line1, "New Level!");
        Enemy_gen();
    }
    // Pot_Health Effect
    if (MovesTillLastpot == 0)
    {
        Health_Pot = 0;
    }
    if (power > 1)
    {
        if (power - 0.05 >= 1)
        {
            power -= 0.05;
        }
        else
        {
            power = 1;
        }
    }
    // Heal food
    if (PlayerFood > 7)
    {
        if (playerHP == playerHPMax)
        {
        }
        else
        {
            if (playerHP <= playerHPMax - 100 - Health_Pot)
            {
                playerHP += 100 + Health_Pot;
            }
            else
            {
                playerHP = playerHPMax;
            }
            PlayerFood -= 0.5;
        }
    }
    // Pause and Resume Song
    if (c == 'P' || c == 'p')
    {
        Mix_PauseMusic();
    }
    if (c == 'r' || c == 'R')
    {
        Mix_ResumeMusic();
    }
    // Empty PassWord
    gettimeofday(&end, NULL);
    if ((int)(end.tv_sec - start.tv_sec) > 10)
    {
        strcpy(Line2, "                                                       ");
    }
    // Lose
    /*if(playerHP <= 0)
    {
        Lose();
    }*/
    // Farrange Fight
    if (c == ' ')
    {
        Far_Battle();
    }
    // locked door
    int lockeddooryesno = 0;
    int keypass = 0;
    for (int i = 0; i < Door_Placed; i++)
    {
        if (c == 'j' && DoorArray[i].x == x && DoorArray[i].y == y - 1 && DoorArray[i].lock == true)
        {
            for (int ii = 0; ii < AncientKeys; ii++)
            {
                if (AncientKey[ii] == 1)
                {
                    int broken = rand() % 10;
                    if (broken == 0)
                    {
                        AncientKey[ii] = -1;
                    }
                    else
                    {
                        keypass = 1;
                        AncientKey[ii] = -1;
                        break;
                    }
                }
            }
            lockeddooryesno = 1;
            break;
        }
        if (c == 'k' && DoorArray[i].x == x && DoorArray[i].y == y + 1 && DoorArray[i].lock == true)
        {
            for (int ii = 0; ii < AncientKeys; ii++)
            {
                if (AncientKey[ii] == 1)
                {
                    int broken = rand() % 10;
                    if (broken == 0)
                    {
                        AncientKey[ii] = -1;
                    }
                    else
                    {
                        keypass = 1;
                        AncientKey[ii] = -1;
                        break;
                    }
                }
            }
            lockeddooryesno = 1;
            break;
        }
        if (c == 'l' && DoorArray[i].x == x + 1 && DoorArray[i].y == y && DoorArray[i].lock == true)
        {
            for (int ii = 0; ii < AncientKeys; ii++)
            {
                if (AncientKey[ii] == 1)
                {
                    int broken = rand() % 10;
                    if (broken == 0)
                    {
                        AncientKey[ii] = -1;
                    }
                    else
                    {
                        keypass = 1;
                        AncientKey[ii] = -1;
                        break;
                    }
                }
            }
            lockeddooryesno = 1;
            break;
        }
        if (c == 'h' && DoorArray[i].x == x - 1 && DoorArray[i].y == y && DoorArray[i].lock == true)
        {
            for (int ii = 0; ii < AncientKeys; ii++)
            {
                if (AncientKey[ii] == 1)
                {
                    int broken = rand() % 10;
                    if (broken == 0)
                    {
                        AncientKey[ii] = -1;
                    }
                    else
                    {
                        keypass = 1;
                        AncientKey[ii] = -1;
                        break;
                    }
                }
            }
            lockeddooryesno = 1;
            break;
        }
        if (c == 'y' && DoorArray[i].x == x - 1 && DoorArray[i].y == y - 1 && DoorArray[i].lock == true)
        {
            for (int ii = 0; ii < AncientKeys; ii++)
            {
                if (AncientKey[ii] == 1)
                {
                    int broken = rand() % 10;
                    if (broken == 0)
                    {
                        AncientKey[ii] = -1;
                    }
                    else
                    {
                        keypass = 1;
                        AncientKey[ii] = -1;
                        break;
                    }
                }
            }
            lockeddooryesno = 1;
            break;
        }
        if (c == 'u' && DoorArray[i].x == x + 1 && DoorArray[i].y == y - 1 && DoorArray[i].lock == true)
        {
            for (int ii = 0; ii < AncientKeys; ii++)
            {
                if (AncientKey[ii] == 1)
                {
                    int broken = rand() % 10;
                    if (broken == 0)
                    {
                        AncientKey[ii] = -1;
                    }
                    else
                    {
                        keypass = 1;
                        AncientKey[ii] = -1;
                        break;
                    }
                }
            }
            lockeddooryesno = 1;
            break;
        }
        if (c == 'b' && DoorArray[i].x == x - 1 && DoorArray[i].y == y + 1 && DoorArray[i].lock == true)
        {
            for (int ii = 0; ii < AncientKeys; ii++)
            {
                if (AncientKey[ii] == 1)
                {
                    int broken = rand() % 10;
                    if (broken == 0)
                    {
                        AncientKey[ii] = -1;
                    }
                    else
                    {
                        keypass = 1;
                        AncientKey[ii] = -1;
                        break;
                    }
                }
            }
            lockeddooryesno = 1;
            break;
        }
        if (c == 'n' && DoorArray[i].x == x + 1 && DoorArray[i].y == y + 1 && DoorArray[i].lock == true)
        {
            for (int ii = 0; ii < AncientKeys; ii++)
            {
                if (AncientKey[ii] == 1)
                {
                    int broken = rand() % 10;
                    if (broken == 0)
                    {
                        AncientKey[ii] = -1;
                    }
                    else
                    {
                        keypass = 1;
                        AncientKey[ii] = -1;
                        break;
                    }
                }
            }
            lockeddooryesno = 1;
            break;
        }
    }
    if (1)
    {
        // Battle 1-1
        if (c == 'j' && Check_Enemy(y - 1, x) == 0)
        {
            battle(y - 1, x);
        }
        else
        {
            if (c == 'k' && Check_Enemy(y + 1, x) == 0)
            {
                battle(y + 1, x);
            }
            else
            {
                if (c == 'l' && Check_Enemy(y, x + 1) == 0)
                {
                    battle(y, x + 1);
                }
                else
                {
                    if (c == 'h' && Check_Enemy(y, x - 1) == 0)
                    {
                        battle(y, x - 1);
                    }
                    else
                    {
                        if (c == 'y' && Check_Enemy(y - 1, x - 1) == 0)
                        {
                            battle(y - 1, x - 1);
                        }
                        else
                        {
                            if (c == 'u' && Check_Enemy(y - 1, x + 1) == 0)
                            {
                                battle(y - 1, x + 1);
                            }
                            else
                            {
                                if (c == 'b' && Check_Enemy(y + 1, x - 1) == 0)
                                {
                                    battle(y + 1, x - 1);
                                }
                                else
                                {
                                    if (c == 'n' && Check_Enemy(y + 1, x + 1) == 0)
                                    {
                                        battle(y + 1, x + 1);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        // Movement
        if (c == 'F' || c == 'f')
        {
            char cc = getch();
            if (cc == 'w')
            {
                for (int i = y; i >= 2; i--)
                {
                    if (map[i][x] == ' ' && map[i - 1][x] != ' ')
                    {
                        y = i;
                        break;
                        // return;
                    }
                }
            }
            if (cc == 's')
            {
                for (int i = y; i <= rows - 3; i++)
                {
                    if (map[i][x] == ' ' && map[i + 1][x] != ' ')
                    {
                        y = i;
                        break;
                        // return;
                    }
                }
            }
            if (cc == 'd')
            {
                for (int i = x; i <= cols - 3; i++)
                {
                    if (map[y][i] == ' ' && map[y][i + 1] != ' ')
                    {
                        x = i;
                        break;
                        // return;
                    }
                }
            }
            if (cc == 'a')
            {
                for (int i = x; i >= 2; i--)
                {
                    if (map[y][i] == ' ' && map[y][i - 1] != ' ')
                    {
                        x = i;
                        break;
                        // return;
                    }
                }
            }
        }
        if (c == 'j' && map[y - 1][x] != '#' && map[y - 1][x] != '%' && map[y - 1][x] != 'O' && Check_Enemy(y - 1, x) == 1 && lockeddooryesno == 0)
        {
            y--;
            if (MovesTillLastpot >= 1)
            {
                MovesTillLastpot--;
            }
            MovesTillLastFood++;
            if (MovesTillLastFood > TillLast)
            {
                if (PlayerFood >= 1)
                {
                    PlayerFood--;
                }
            }
            if (MovesTillLastFood % (TillLast + 20) > TillLast + 19)
            {
                for (int i = 0; i < FoodCount; i++)
                {
                    if (FoodArray[i].type == 0)
                    {
                        FoodArray[i].type = 3;
                        break;
                    }
                    if (FoodArray[i].type == 1 || FoodArray[i].type == 2)
                    {
                        FoodArray[i].type = 0;
                        break;
                    }
                }
            }
            if (PlayerFood < 3)
            {
                if (playerHP >= 50)
                {
                    playerHP -= 50;
                }
            }
        }
        else
        {
            if (c == 'k' && map[y + 1][x] != '#' && map[y + 1][x] != '%' && map[y + 1][x] != 'O' && Check_Enemy(y + 1, x) == 1 && lockeddooryesno == 0)
            {
                y++;
                MovesTillLastFood++;
                if (MovesTillLastpot >= 1)
                {
                    MovesTillLastpot--;
                }
                if (MovesTillLastFood > TillLast)
                {
                    if (PlayerFood >= 1)
                    {
                        PlayerFood--;
                    }
                }
                if (MovesTillLastFood % (TillLast + 20) > TillLast + 19)
                {
                    for (int i = 0; i < FoodCount; i++)
                    {
                        if (FoodArray[i].type == 0)
                        {
                            FoodArray[i].type = 3;
                            break;
                        }
                        if (FoodArray[i].type == 1 || FoodArray[i].type == 2)
                        {
                            FoodArray[i].type = 0;
                            break;
                        }
                    }
                }
                if (PlayerFood < 3)
                {
                    if (playerHP >= 50)
                    {
                        playerHP -= 50;
                    }
                }
            }
            else
            {
                if (c == 'l' && map[y][x + 1] != '#' && map[y][x + 1] != '%' && map[y][x + 1] != 'O' && Check_Enemy(y, x + 1) == 1 && lockeddooryesno == 0)
                {
                    if (MovesTillLastpot >= 1)
                    {
                        MovesTillLastpot--;
                    }
                    x++;
                    MovesTillLastFood++;
                    if (MovesTillLastFood > TillLast)
                    {
                        if (PlayerFood >= 1)
                        {
                            PlayerFood--;
                        }
                    }
                    if (MovesTillLastFood % (TillLast + 20) > TillLast + 19)
                    {
                        for (int i = 0; i < FoodCount; i++)
                        {
                            if (FoodArray[i].type == 0)
                            {
                                FoodArray[i].type = 3;
                                break;
                            }
                            if (FoodArray[i].type == 1 || FoodArray[i].type == 2)
                            {
                                FoodArray[i].type = 0;
                                break;
                            }
                        }
                    }
                    if (PlayerFood < 3)
                    {
                        if (playerHP >= 50)
                        {
                            playerHP -= 50;
                        }
                    }
                }
                else
                {
                    if (c == 'h' && map[y][x - 1] != '#' && map[y][x - 1] != '%' && map[y][x - 1] != 'O' && Check_Enemy(y, x - 1) == 1 && lockeddooryesno == 0)
                    {
                        if (MovesTillLastpot >= 1)
                        {
                            MovesTillLastpot--;
                        }
                        x--;
                        MovesTillLastFood++;
                        if (MovesTillLastFood > TillLast)
                        {
                            if (PlayerFood >= 1)
                            {
                                PlayerFood--;
                            }
                        }
                        if (MovesTillLastFood % (TillLast + 20) > TillLast + 19)
                        {
                            for (int i = 0; i < FoodCount; i++)
                            {
                                if (FoodArray[i].type == 0)
                                {
                                    FoodArray[i].type = 3;
                                    break;
                                }
                                if (FoodArray[i].type == 1 || FoodArray[i].type == 2)
                                {
                                    FoodArray[i].type = 0;
                                    break;
                                }
                            }
                        }
                        if (PlayerFood < 3)
                        {
                            if (playerHP >= 50)
                            {
                                playerHP -= 50;
                            }
                        }
                    }
                    else
                    {
                        if (c == 'y' && map[y - 1][x - 1] != '#' && map[y - 1][x - 1] != '%' && map[y - 1][x - 1] != 'O' && Check_Enemy(y - 1, x - 1) == 1 && lockeddooryesno == 0)
                        {
                            if (MovesTillLastpot >= 1)
                            {
                                MovesTillLastpot--;
                            }
                            y--;
                            x--;
                            MovesTillLastFood++;
                            if (MovesTillLastFood > TillLast)
                            {
                                if (PlayerFood >= 1)
                                {
                                    PlayerFood--;
                                }
                            }
                            if (MovesTillLastFood % (TillLast + 20) > TillLast + 19)
                            {
                                for (int i = 0; i < FoodCount; i++)
                                {
                                    if (FoodArray[i].type == 0)
                                    {
                                        FoodArray[i].type = 3;
                                        break;
                                    }
                                    if (FoodArray[i].type == 1 || FoodArray[i].type == 2)
                                    {
                                        FoodArray[i].type = 0;
                                        break;
                                    }
                                }
                            }
                            if (PlayerFood < 3)
                            {
                                if (playerHP >= 50)
                                {
                                    playerHP -= 50;
                                }
                            }
                        }
                        else
                        {
                            if (c == 'u' && map[y - 1][x + 1] != '#' && map[y - 1][x + 1] != '%' && map[y - 1][x + 1] != 'O' && Check_Enemy(y - 1, x + 1) == 1 && lockeddooryesno == 0)
                            {
                                if (MovesTillLastpot >= 1)
                                {
                                    MovesTillLastpot--;
                                }
                                y--;
                                x++;
                                MovesTillLastFood++;
                                if (MovesTillLastFood > TillLast)
                                {
                                    if (PlayerFood >= 1)
                                    {
                                        PlayerFood--;
                                    }
                                }
                                if (MovesTillLastFood % (TillLast + 20) > TillLast + 19)
                                {
                                    for (int i = 0; i < FoodCount; i++)
                                    {
                                        if (FoodArray[i].type == 0)
                                        {
                                            FoodArray[i].type = 3;
                                            break;
                                        }
                                        if (FoodArray[i].type == 1 || FoodArray[i].type == 2)
                                        {
                                            FoodArray[i].type = 0;
                                            break;
                                        }
                                    }
                                }
                                if (PlayerFood < 3)
                                {
                                    if (playerHP >= 50)
                                    {
                                        playerHP -= 50;
                                    }
                                }
                            }
                            else
                            {
                                if (c == 'b' && map[y + 1][x - 1] != '#' && map[y + 1][x - 1] != '%' && map[y + 1][x - 1] != 'O' && Check_Enemy(y + 1, x - 1) == 1 && lockeddooryesno == 0)
                                {
                                    if (MovesTillLastpot >= 1)
                                    {
                                        MovesTillLastpot--;
                                    }
                                    x--;
                                    y++;
                                    MovesTillLastFood++;
                                    if (MovesTillLastFood > TillLast)
                                    {
                                        if (PlayerFood >= 1)
                                        {
                                            PlayerFood--;
                                        }
                                    }
                                    if (MovesTillLastFood % (TillLast + 20) > TillLast + 19)
                                    {
                                        for (int i = 0; i < FoodCount; i++)
                                        {
                                            if (FoodArray[i].type == 0)
                                            {
                                                FoodArray[i].type = 3;
                                                break;
                                            }
                                            if (FoodArray[i].type == 1 || FoodArray[i].type == 2)
                                            {
                                                FoodArray[i].type = 0;
                                                break;
                                            }
                                        }
                                    }
                                    if (PlayerFood < 3)
                                    {
                                        if (playerHP >= 50)
                                        {
                                            playerHP -= 50;
                                        }
                                    }
                                }
                                else
                                {
                                    if (c == 'n' && map[y + 1][x + 1] != '#' && map[y + 1][x + 1] != '%' && map[y + 1][x + 1] != 'O' && Check_Enemy(y + 1, x + 1) == 1 && lockeddooryesno == 0)
                                    {
                                        if (MovesTillLastpot >= 1)
                                        {
                                            MovesTillLastpot--;
                                        }
                                        x++;
                                        y++;
                                        MovesTillLastFood++;
                                        if (MovesTillLastFood > TillLast)
                                        {
                                            if (PlayerFood >= 1)
                                            {
                                                PlayerFood--;
                                            }
                                        }
                                        if (MovesTillLastFood % (TillLast + 20) > TillLast + 19)
                                        {
                                            for (int i = 0; i < FoodCount; i++)
                                            {
                                                if (FoodArray[i].type == 0)
                                                {
                                                    FoodArray[i].type = 3;
                                                    break;
                                                }
                                                if (FoodArray[i].type == 1 || FoodArray[i].type == 2)
                                                {
                                                    FoodArray[i].type = 0;
                                                    break;
                                                }
                                            }
                                        }
                                        if (PlayerFood < 3)
                                        {
                                            if (playerHP >= 50)
                                            {
                                                playerHP -= 50;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if (firsttime == true)
                                        {
                                        }
                                        else
                                        {
                                            for (int i = 0; i < Window_Placed; i++)
                                            {
                                                if ((WindowArray[i].x == x && WindowArray[i].y == y) ||
                                                    (WindowArray[i].x == x - 1 && WindowArray[i].y == y) ||
                                                    (WindowArray[i].x == x + 1 && WindowArray[i].y == y) ||
                                                    (WindowArray[i].x == x && WindowArray[i].y == y + 1) ||
                                                    (WindowArray[i].x == x && WindowArray[i].y == y - 1))
                                                {
                                                    for (int j = 0; j < Room_Placed; j++)
                                                    {
                                                        if (WindowArray[i].dir == 0)
                                                        {
                                                            if (RoomArray[j].TopLeft_Y < WindowArray[i].y)
                                                            {
                                                                RoomArray[j].wdow = 2;
                                                            }
                                                        }
                                                        if (WindowArray[i].dir == 1)
                                                        {
                                                            if (RoomArray[j].TopLeft_x > WindowArray[i].x)
                                                            {
                                                                RoomArray[j].wdow = 2;
                                                            }
                                                        }
                                                        if (WindowArray[i].dir == 2)
                                                        {
                                                            if (RoomArray[j].TopLeft_Y > WindowArray[i].y)
                                                            {
                                                                RoomArray[j].wdow = 2;
                                                            }
                                                        }
                                                        if (WindowArray[i].dir == 3)
                                                        {
                                                            if (RoomArray[j].TopLeft_x < WindowArray[i].x)
                                                            {
                                                                RoomArray[j].wdow = 2;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                            if (c != 113)
                                            {
                                                strcpy(Line1, "You cant move through!");
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    // Save
    if (c == '~')
    {
        save();
    }
    // StairCase
    if (map[y][x] == '<')
    {
        if (c == KEY_LEFT && level != 4)
        {
            SaveGame_StairCase();
        }
        if (c == KEY_LEFT && level == 4)
        {
            int number;
            AddtoScoreBoard();
            FILE *file26;
            char filename[256];
            snprintf(filename, sizeof(filename), "%s_Times_Played.txt", Player_UserName);
            file26 = fopen(filename, "r+");

            fscanf(file26, "%d", &number);

            number += 1;

            rewind(file26);

            fprintf(file26, "%d", number);
            fclose(file26);
            Player_Time();
            clear();
            Won();
        }
        if (c == KEY_RIGHT && level != 1)
        {
            LoadGame_StairCase();
        }
    }
    // Activate Enemy
    for (int i = 0; i < Room_Placed; i++)
    {
        if (y >= RoomArray[i].TopLeft_Y && y <= RoomArray[i].TopLeft_Y + RoomArray[i].Height - 1 &&
            x >= RoomArray[i].TopLeft_x && x <= RoomArray[i].TopLeft_x + RoomArray[i].Width - 1)
        {
            for (int j = 0; j < EnemyCount; j++)
            {
                if (EnemyArray[j].y >= RoomArray[i].TopLeft_Y && EnemyArray[j].y <= RoomArray[i].TopLeft_Y + RoomArray[i].Height - 1 &&
                    EnemyArray[j].x >= RoomArray[i].TopLeft_x && EnemyArray[j].x <= RoomArray[i].TopLeft_x + RoomArray[i].Width - 1)
                {
                    if (EnemyArray[j].hp > 0 && EnemyArray[j].move != -1)
                    {
                        EnemyArray[j].move = 1;
                    }
                }
            }
            break;
        }
    }
    // step on door(show room) and Diactivate Enemy
    for (int index = 0; index < Door_Placed; index++)
    {
        if (y == DoorArray[index].y && x == DoorArray[index].x)
        {
            for (int i = 0; i < Room_Placed; i++)
            {
                if (y >= RoomArray[i].TopLeft_Y - 1 && y <= RoomArray[i].TopLeft_Y + RoomArray[i].Height &&
                    x >= RoomArray[i].TopLeft_x - 1 && x <= RoomArray[i].TopLeft_x + RoomArray[i].Width)
                {
                    if (RoomArray[i].show == false)
                    {
                        strcpy(Line1, "New Room");
                    }
                    RoomArray[i].show = true;
                }
            }
            for (int j = 0; j < EnemyCount; j++)
            {
                if (EnemyArray[j].type == 'd' || EnemyArray[j].type == 'f')
                {
                    EnemyArray[j].move = 0;
                }
            }
        }
    }
    // Enemy Move
    Enemy_Move();
    // step on trap
    if (map[y][x] == '^')
    {
        strcpy(Line1, "You have stepped on a trap!");
        int ij;
        for (ij = 0; ij < Trap_Placed; ij++)
        {
            if (y == TrapArray[ij].y && x == TrapArray[ij].x)
            {
                break;
            }
        }
        TrapArray[ij].show = 1;
        if (playerHP >= 100)
        {
            playerHP -= 100;
        }
        else
        {
            playerHP = 0;
        }
    }
    // step on gold
    if (map[y][x] == 'G')
    {
        int rd = (rand() % 10) + 1;
        playerGOLD += rd;
        map[y][x] = ' ';
        sprintf(Line1, "You got %d gold!", rd);
    }
    // step on big gold
    if (map[y][x] == 'g')
    {
        int rd = (rand() % 40) + 20;
        playerGOLD += rd;
        map[y][x] = ' ';
        sprintf(Line1, "You got %d gold!", rd);
    }
    // password generator
    if (map[y][x] == '&')
    {
        int rrr = (rand() % 9000) + 1000;
        sprintf(Line2, "Password is: %d", rrr);
        for (int i = 0; i < Door_Placed; i++)
        {
            if (DoorArray[i].lock == true)
            {
                DoorArray[i].PassWord = rrr;
                gettimeofday(&start, NULL);
                break;
            }
        }
    }
    // locked door
    if (lockeddooryesno == 1)
    {
        clear();
        echo();
        curs_set(true);
        if (keypass == 1)
        {
            UsedKey++;
            int makeriaziyekobastim;
            for (int i = 0; i < Door_Placed; i++)
            {
                if (DoorArray[i].lock == true)
                {
                    makeriaziyekobastim = i;
                    break;
                }
            }
            DoorArray[makeriaziyekobastim].lock = false;
        }
        else
        {
            int passs;
            int makeriaziyekobastim;
            int doesheeven;
            for (int i = 0; i < Door_Placed; i++)
            {
                if (DoorArray[i].lock == true)
                {
                    makeriaziyekobastim = i;
                    passs = DoorArray[i].PassWord;
                    doesheeven = DoorArray[i].trysleft;
                    break;
                }
            }
            if (doesheeven >= 1)
            {
                int input;
                clear();
                int what = rand() % 2;
                if (what == 0)
                {
                    do
                    {
                        strcpy(Line2, "PassWord: ");
                        mvprintw(0, 0, "%s", Line1);
                        mvprintw(1, 0, "%s", Line2);
                        strcpy(Line1, "                                                                     ");
                        scanw("%d", &input);
                        if (input == passs)
                        {
                            DoorArray[makeriaziyekobastim].lock = false;
                            break;
                        }
                        else
                        {
                            DoorArray[makeriaziyekobastim].trysleft--;
                            clear();
                            sprintf(Line1, " PassWord Was Wrong! You have %d trys left!", DoorArray[makeriaziyekobastim].trysleft);
                        }
                        mvprintw(0, 0, "%s", Line1);
                        mvprintw(1, 0, "%s", Line2);
                        strcpy(Line1, "                                                                      ");
                    } while ((passs != input) && DoorArray[makeriaziyekobastim].trysleft >= 1);
                }
                else
                {
                    char adad[4];
                    do
                    {
                        strcpy(Line2, "PassWord: ");
                        mvprintw(0, 0, "%s", Line1);
                        mvprintw(1, 0, "%s", Line2);
                        strcpy(Line1, "                                                                ");
                        scanw("%d", &input);
                        adad[0] = (char)(passs % 10);
                        adad[1] = (char)((passs / 10) % 10);
                        adad[2] = (char)((passs / 100) % 10);
                        adad[3] = (char)((passs / 1000) % 10);
                        if (input == (((int)adad[0]) * 1000 + ((int)adad[1]) * 100 + ((int)adad[2]) * 10 + ((int)adad[3])))
                        {
                            DoorArray[makeriaziyekobastim].lock = false;
                            break;
                        }
                        else
                        {
                            DoorArray[makeriaziyekobastim].trysleft--;
                            clear();
                            sprintf(Line1, " PassWord Was Wrong! You have %d trys left!", DoorArray[makeriaziyekobastim].trysleft);
                        }
                        mvprintw(0, 0, "%s", Line1);
                        mvprintw(1, 0, "%s", Line2);
                        strcpy(Line1, "                                                                    ");
                    } while (((((int)adad[0]) * 1000 + ((int)adad[1]) * 100 + ((int)adad[2]) * 10 + ((int)adad[3])) != input) && DoorArray[makeriaziyekobastim].trysleft >= 1);
                }
            }
        }
        clear();
        noecho();
        curs_set(false);
    }
    // Ancient Keys
    if (map[y][x] == 'A')
    {
        int gh = AncientKeys;
        AncientKey[gh] = 1;
        AncientKeys += 1;
        map[y][x] = ' ';
    }
    // Dagger 2
    if (map[y][x] == '2')
    {
        if (YesNoChoice())
        {
            WeaponArray[WeaponCount].mode = 2;
            int haha = 0;
            for(int i = 0 ; i < Wasted_Weapon_Count;i++)
            {
                if(Wasted_Weapon_Array[i].x == x && Wasted_Weapon_Array[i].y == y)
                {
                    haha = 1;
                    break;
                }
            }
            if(haha == 0)
            {
                WeaponArray[WeaponCount].times_use_left = 10;
            }
            else
            {
                WeaponArray[WeaponCount].times_use_left = 1;
            }
            WeaponCount++;
            map[y][x] = ' ';
        }
    }
    // Magic Wand 3
    if (map[y][x] == '3')
    {
        if (YesNoChoice())
        {
            WeaponArray[WeaponCount].mode = 3;
            int haha = 0;
            for(int i = 0 ; i < Wasted_Weapon_Count;i++)
            {
                if(Wasted_Weapon_Array[i].x == x && Wasted_Weapon_Array[i].y == y)
                {
                    haha = 1;
                    break;
                }
            }
            if(haha == 0)
            {
                WeaponArray[WeaponCount].times_use_left = 8;
            }
            else
            {
                WeaponArray[WeaponCount].times_use_left = 1;
            }
            WeaponCount++;
            map[y][x] = ' ';
        }
    }
    // Normal Arrow 4
    if (map[y][x] == '4')
    {
        if (YesNoChoice())
        {
            WeaponArray[WeaponCount].mode = 4;
            int haha = 0;
            for(int i = 0 ; i < Wasted_Weapon_Count;i++)
            {
                if(Wasted_Weapon_Array[i].x == x && Wasted_Weapon_Array[i].y == y)
                {
                    haha = 1;
                    break;
                }
            }
            if(haha == 0)
            {
                WeaponArray[WeaponCount].times_use_left = 20;
            }
            else
            {
                WeaponArray[WeaponCount].times_use_left = 1;
            }
            WeaponCount++;
            map[y][x] = ' ';
        }
    }
    // Sword 5
    if (map[y][x] == '5')
    {
        if (YesNoChoice())
        {
            WeaponArray[WeaponCount].mode = 5;
            WeaponCount++;
            map[y][x] = ' ';
        }
    }
    // Health 6♥
    if (map[y][x] == '6')
    {
        if (YesNoChoice())
        {
            SpellArray[SpellCount].mode = 6;
            SpellCount++;
            map[y][x] = ' ';
        }
    }
    // Speed 7⌁
    if (map[y][x] == '7')
    {
        if (YesNoChoice())
        {
            SpellArray[SpellCount].mode = 7;
            SpellCount++;
            map[y][x] = ' ';
        }
    }
    // Damage 8☣
    if (map[y][x] == '8')
    {
        if (YesNoChoice())
        {
            SpellArray[SpellCount].mode = 8;
            SpellCount++;
            map[y][x] = ' ';
        }
    }
    // Food
    if (map[y][x] == 'Z')
    {
        FoodArray[FoodCount].type = 0;
        FoodCount++;
        map[y][x] = ' ';
    }
    if (map[y][x] == 'X')
    {
        FoodArray[FoodCount].type = 1;
        FoodCount++;
        map[y][x] = ' ';
    }
    if (map[y][x] == 'C')
    {
        FoodArray[FoodCount].type = 2;
        FoodCount++;
        map[y][x] = ' ';
    }
    if (map[y][x] == 'V')
    {
        FoodArray[FoodCount].type = 3;
        FoodCount++;
        map[y][x] = ' ';
    }
    // Weapon Inventory
    if (c == 'W' || c == 'w')
    {
        WeaponChoice();
        clear();
    }
    // Spell Inventory
    if (c == 'S' || c == 's')
    {
        SpellChoice();
        clear();
    }
    // Show Map
    if (c == 'M' || c == 'm')
    {
        m *= -1;
        if (m == -1)
        {
            clear();
        }
        else
        {
            // print corridors and delete inside room corridors
            for (int yy = 0; yy < rows; yy++)
            {
                for (int xx = 0; xx < cols; xx++)
                {
                    if (map[yy][xx] == '$')
                    {
                        int ccounter = 0;
                        for (int iii = 0; iii < Room_Placed; iii++)
                        {
                            if (yy >= RoomArray[iii].TopLeft_Y && yy <= RoomArray[iii].TopLeft_Y + RoomArray[iii].Height - 1 &&
                                xx >= RoomArray[iii].TopLeft_x && xx <= RoomArray[iii].TopLeft_x + RoomArray[iii].Width - 1)
                            {
                                ccounter++;
                            }
                        }
                        if (ccounter == 0)
                        {
                            wchar_t message[] = L"⌯";
                            mvprintw(yy, xx, "%ls", message);
                        }
                        else
                        {
                            map[yy][xx] = ' ';
                            mvaddch(yy, xx, ' ');
                        }
                    }
                }
            }
        }
    }
    // Food Inentory
    if (c == 'E' || c == 'e')
    {
        FoodChoice();
        clear();
    }

    // random place player
    if (Player_Placed == false)
    {
        int check = 5;
        do
        {
            check = 0;
            y = rand() % rows;
            x = rand() % cols;
            int roomindex = -1;
            for (int i = 0; i < Room_Placed; i++)
            {
                if (y >= RoomArray[i].TopLeft_Y && y <= RoomArray[i].TopLeft_Y + RoomArray[i].Height - 1 &&
                    x >= RoomArray[i].TopLeft_x && x <= RoomArray[i].TopLeft_x + RoomArray[i].Width - 1)
                {
                    roomindex = i;
                    break;
                }
            }
            if (roomindex != -1)
            {
                for (int i = RoomArray[roomindex].TopLeft_x; i <= RoomArray[roomindex].TopLeft_x + RoomArray[roomindex].Width - 1; i++)
                {
                    for (int j = RoomArray[roomindex].TopLeft_Y; j <= RoomArray[roomindex].TopLeft_Y + RoomArray[roomindex].Height - 1; j++)
                    {
                        if (map[j][i] == '<')
                        {
                            check = 5;
                        }
                    }
                }
            }
            else
            {
                check = 5;
            }
        } while (map[y][x] != ' ' || y == rows || y == 0 || y == 1 || y == rows - 1 || y == rows - 2 || y == 2 || check != 0);
        Player_Placed = true;
        for (int i = 0; i < Room_Placed; i++)
        {
            if (y >= RoomArray[i].TopLeft_Y && y <= RoomArray[i].TopLeft_Y + RoomArray[i].Height - 1 &&
                x >= RoomArray[i].TopLeft_x && x <= RoomArray[i].TopLeft_x + RoomArray[i].Width - 1)
            {
                RoomArray[i].show = true;
            }
        }
    }

    // print limited corridors and fill corridor-array
    if (map[y][x] == '$')
    {
        CorridorArray[Corridor_Placed].x = x;
        CorridorArray[Corridor_Placed].y = y;
        Corridor_Placed++;
        wchar_t message[] = L"⌯";
        mvprintw(y, x, "%ls", message);
        for (int i = 0; i < Door_Placed; i++)
        {
            if (DoorArray[i].y == y && DoorArray[i].x == x && DoorArray[i].lock == false)
            {
                attron(COLOR_PAIR(7));
                wchar_t message[] = L"∎";
                mvprintw(y, x, "%ls", message);
                attroff(COLOR_PAIR(7));
            }
            if (DoorArray[i].y == y && DoorArray[i].x == x && DoorArray[i].lock == true)
            {
                attron(COLOR_PAIR(5));
                wchar_t message[] = L"∎";
                mvprintw(y, x, "%ls", message);
                attroff(COLOR_PAIR(5));
            }
        }
    }
    if (map[y - 1][x] == '$')
    {
        CorridorArray[Corridor_Placed].x = x;
        CorridorArray[Corridor_Placed].y = y - 1;
        Corridor_Placed++;
        wchar_t message[] = L"⌯";
        mvprintw(y - 1, x, "%ls", message);
        for (int i = 0; i < Door_Placed; i++)
        {
            if (DoorArray[i].y == y - 1 && DoorArray[i].x == x && DoorArray[i].lock == false)
            {
                attron(COLOR_PAIR(7));
                wchar_t message[] = L"∎";
                mvprintw(y - 1, x, "%ls", message);
                attroff(COLOR_PAIR(7));
            }
            if (DoorArray[i].y == y - 1 && DoorArray[i].x == x && DoorArray[i].lock == true)
            {
                attron(COLOR_PAIR(5));
                wchar_t message[] = L"∎";
                mvprintw(y - 1, x, "%ls", message);
                attroff(COLOR_PAIR(5));
            }
        }
    }
    if (map[y + 1][x] == '$')
    {
        CorridorArray[Corridor_Placed].x = x;
        CorridorArray[Corridor_Placed].y = y + 1;
        Corridor_Placed++;
        wchar_t message[] = L"⌯";
        mvprintw(y + 1, x, "%ls", message);
        for (int i = 0; i < Door_Placed; i++)
        {
            if (DoorArray[i].y == y + 1 && DoorArray[i].x == x && DoorArray[i].lock == false)
            {
                attron(COLOR_PAIR(7));
                wchar_t message[] = L"∎";
                mvprintw(y + 1, x, "%ls", message);
                attroff(COLOR_PAIR(7));
            }
            if (DoorArray[i].y == y + 1 && DoorArray[i].x == x && DoorArray[i].lock == true)
            {
                attron(COLOR_PAIR(5));
                wchar_t message[] = L"∎";
                mvprintw(y + 1, x, "%ls", message);
                attroff(COLOR_PAIR(5));
            }
        }
    }
    if (map[y][x - 1] == '$')
    {
        CorridorArray[Corridor_Placed].x = x - 1;
        CorridorArray[Corridor_Placed].y = y;
        Corridor_Placed++;
        wchar_t message[] = L"⌯";
        mvprintw(y, x - 1, "%ls", message);
        for (int i = 0; i < Door_Placed; i++)
        {
            if (DoorArray[i].y == y && DoorArray[i].x == x - 1 && DoorArray[i].lock == false)
            {
                attron(COLOR_PAIR(7));
                wchar_t message[] = L"∎";
                mvprintw(y, x - 1, "%ls", message);
                attroff(COLOR_PAIR(7));
            }
            if (DoorArray[i].y == y && DoorArray[i].x == x - 1 && DoorArray[i].lock == true)
            {
                attron(COLOR_PAIR(5));
                wchar_t message[] = L"∎";
                mvprintw(y, x - 1, "%ls", message);
                attroff(COLOR_PAIR(5));
            }
        }
    }
    if (map[y][x + 1] == '$')
    {
        CorridorArray[Corridor_Placed].x = x + 1;
        CorridorArray[Corridor_Placed].y = y;
        Corridor_Placed++;
        wchar_t message[] = L"⌯";
        mvprintw(y, x + 1, "%ls", message);
        for (int i = 0; i < Door_Placed; i++)
        {
            if (DoorArray[i].y == y && DoorArray[i].x == x + 1 && DoorArray[i].lock == false)
            {
                init_pair(66, COLOR_YELLOW, COLOR_BLACK);
                attron(COLOR_PAIR(66));
                wchar_t message[] = L"∎";
                mvprintw(y, x + 1, "%ls", message);
                attroff(COLOR_PAIR(66));
            }
            if (DoorArray[i].y == y && DoorArray[i].x == x + 1 && DoorArray[i].lock == true)
            {
                init_pair(41, COLOR_RED, COLOR_BLACK);
                attron(COLOR_PAIR(41));
                wchar_t message[] = L"∎";
                mvprintw(y, x + 1, "%ls", message);
                attroff(COLOR_PAIR(41));
            }
        }
    }
    if (map[y + 1][x + 1] == '$')
    {
        CorridorArray[Corridor_Placed].x = x + 1;
        CorridorArray[Corridor_Placed].y = y + 1;
        Corridor_Placed++;
        wchar_t message[] = L"⌯";
        mvprintw(y + 1, x + 1, "%ls", message);
        for (int i = 0; i < Door_Placed; i++)
        {
            if (DoorArray[i].y == y + 1 && DoorArray[i].x == x + 1 && DoorArray[i].lock == false)
            {
                attron(COLOR_PAIR(7));
                wchar_t message[] = L"∎";
                mvprintw(y + 1, x + 1, "%ls", message);
                attroff(COLOR_PAIR(7));
            }
            if (DoorArray[i].y == y + 1 && DoorArray[i].x == x + 1 && DoorArray[i].lock == true)
            {
                attron(COLOR_PAIR(5));
                wchar_t message[] = L"∎";
                mvprintw(y + 1, x + 1, "%ls", message);
                attroff(COLOR_PAIR(5));
            }
        }
    }
    if (map[y + 1][x - 1] == '$')
    {
        CorridorArray[Corridor_Placed].x = x - 1;
        CorridorArray[Corridor_Placed].y = y + 1;
        Corridor_Placed++;
        wchar_t message[] = L"⌯";
        mvprintw(y + 1, x - 1, "%ls", message);
        for (int i = 0; i < Door_Placed; i++)
        {
            if (DoorArray[i].y == y + 1 && DoorArray[i].x == x - 1 && DoorArray[i].lock == false)
            {
                attron(COLOR_PAIR(7));
                wchar_t message[] = L"∎";
                mvprintw(y + 1, x - 1, "%ls", message);
                attroff(COLOR_PAIR(7));
            }
            if (DoorArray[i].y == y + 1 && DoorArray[i].x == x - 1 && DoorArray[i].lock == true)
            {
                attron(COLOR_PAIR(5));
                wchar_t message[] = L"∎";
                mvprintw(y + 1, x - 1, "%ls", message);
                attroff(COLOR_PAIR(5));
            }
        }
    }
    if (map[y - 1][x + 1] == '$')
    {
        CorridorArray[Corridor_Placed].x = x + 1;
        CorridorArray[Corridor_Placed].y = y - 1;
        Corridor_Placed++;
        wchar_t message[] = L"⌯";
        mvprintw(y - 1, x + 1, "%ls", message);
        for (int i = 0; i < Door_Placed; i++)
        {
            if (DoorArray[i].y == y - 1 && DoorArray[i].x == x + 1 && DoorArray[i].lock == false)
            {
                attron(COLOR_PAIR(7));
                wchar_t message[] = L"∎";
                mvprintw(y - 1, x + 1, "%ls", message);
                attroff(COLOR_PAIR(7));
            }
            if (DoorArray[i].y == y - 1 && DoorArray[i].x == x + 1 && DoorArray[i].lock == true)
            {
                attron(COLOR_PAIR(5));
                wchar_t message[] = L"∎";
                mvprintw(y - 1, x + 1, "%ls", message);
                attroff(COLOR_PAIR(5));
            }
        }
    }
    if (map[y - 1][x - 1] == '$')
    {
        CorridorArray[Corridor_Placed].x = x - 1;
        CorridorArray[Corridor_Placed].y = y - 1;
        Corridor_Placed++;
        wchar_t message[] = L"⌯";
        mvprintw(y - 1, x - 1, "%ls", message);
        for (int i = 0; i < Door_Placed; i++)
        {
            if (DoorArray[i].y == y - 1 && DoorArray[i].x == x - 1 && DoorArray[i].lock == false)
            {
                attron(COLOR_PAIR(7));
                wchar_t message[] = L"∎";
                mvprintw(y - 1, x - 1, "%ls", message);
                attroff(COLOR_PAIR(7));
            }
            if (DoorArray[i].y == y - 1 && DoorArray[i].x == x - 1 && DoorArray[i].lock == true)
            {
                attron(COLOR_PAIR(5));
                wchar_t message[] = L"∎";
                mvprintw(y - 1, x - 1, "%ls", message);
                attroff(COLOR_PAIR(5));
            }
        }
    }
    if (map[y][x] != '$' && m != 1)
    {
        clear();
    }
    // print corridors and delete inside room corridors
    for (int yy = 0; yy < rows; yy++)
    {
        for (int xx = 0; xx < cols; xx++)
        {
            if (map[yy][xx] == '$')
            {
                int ccounter = 0;
                for (int iii = 0; iii < Room_Placed; iii++)
                {
                    if (yy >= RoomArray[iii].TopLeft_Y && yy <= RoomArray[iii].TopLeft_Y + RoomArray[iii].Height - 1 &&
                        xx >= RoomArray[iii].TopLeft_x && xx <= RoomArray[iii].TopLeft_x + RoomArray[iii].Width - 1)
                    {
                        ccounter++;
                    }
                }
                if (ccounter == 0)
                {
                }
                else
                {
                    map[yy][xx] = ' ';
                    mvaddch(yy, xx, ' ');
                }
            }
        }
    }
    for (int ia = 0; ia < Corridor_Placed; ia++)
    {
        int ghg = 0;
        for (int i = 0; i < Door_Placed; i++)
        {
            if (CorridorArray[ia].y == DoorArray[i].y && CorridorArray[ia].x == DoorArray[i].x)
            {
                ghg = 1;
            }
        }
        if (ghg == 0)
        {
            wchar_t message[] = L"⌯";
            mvprintw(CorridorArray[ia].y, CorridorArray[ia].x, "%ls", message);
        }
    }
    // Print Rooms
    for (int index = 0; index < Room_Placed; index++)
    {
        if (RoomArray[index].show == true || m == 1)
        {
            for (int yy = (RoomArray[index].TopLeft_Y - 1); yy <= RoomArray[index].TopLeft_Y + RoomArray[index].Height; yy++)
            {
                for (int xx = (RoomArray[index].TopLeft_x - 1); xx <= RoomArray[index].TopLeft_x + RoomArray[index].Width; xx++)
                {
                    if (yy >= RoomArray[index].TopLeft_Y && yy <= (RoomArray[index].TopLeft_Y + RoomArray[index].Height - 1) &&
                        xx >= RoomArray[index].TopLeft_x && xx <= (RoomArray[index].TopLeft_x + RoomArray[index].Width - 1))
                    {
                        int prt = 0;
                        /*for (int carol = 0; carol < EnemyCount; carol++)
                        {
                            if (EnemyArray[carol].x == xx && EnemyArray[carol].y == yy && EnemyArray[carol].hp > 0)
                            {
                                prt = 1;
                                if (EnemyArray[carol].type == 'd')
                                {
                                    mvaddch(yy, xx, 'D');
                                }
                                if (EnemyArray[carol].type == 'f')
                                {
                                    mvaddch(yy, xx, 'F');
                                }
                                if (EnemyArray[carol].type == 't')
                                {
                                    mvaddch(yy, xx, 'G');
                                }
                                if (EnemyArray[carol].type == 's')
                                {
                                    mvaddch(yy, xx, 'S');
                                }
                                if (EnemyArray[carol].type == 'u')
                                {
                                    mvaddch(yy, xx, 'U');
                                }
                            }
                        }*/
                        if (prt == 0)
                        {
                            if (map[yy][xx] != 'O')
                            {
                                if (map[yy][xx] == '^')
                                {
                                    int ij;
                                    for (ij = 0; ij < Trap_Placed; ij++)
                                    {
                                        if (yy == TrapArray[ij].y && xx == TrapArray[ij].x)
                                        {
                                            break;
                                        }
                                    }
                                    if (TrapArray[ij].show == 1)
                                    {
                                        attron(COLOR_PAIR(5));
                                        mvaddch(yy, xx, '^');
                                        attroff(COLOR_PAIR(5));
                                    }
                                    else
                                    {
                                        attron(COLOR_PAIR(2));
                                        wchar_t message[] = L"∙";
                                        mvprintw(yy, xx, "%ls", message);
                                        attroff(COLOR_PAIR(2));
                                    }
                                }
                                else
                                {
                                    if (map[yy][xx] == '<')
                                    {
                                        attron(COLOR_PAIR(4));
                                        mvaddch(yy, xx, '<');
                                        attroff(COLOR_PAIR(4));
                                    }
                                    else
                                    {
                                        if (map[yy][xx] == 'G')
                                        {
                                            attron(COLOR_PAIR(7));
                                            wchar_t message[] = L"⛀";
                                            mvprintw(yy, xx, "%ls", message);
                                            attroff(COLOR_PAIR(7));
                                        }
                                        else
                                        {
                                            if (map[yy][xx] == 'g')
                                            {
                                                attron(COLOR_PAIR(7));
                                                wchar_t message[] = L"⛂";
                                                mvprintw(yy, xx, "%ls", message);
                                                attroff(COLOR_PAIR(7));
                                            }
                                            else
                                            {
                                                if (map[yy][xx] == '&')
                                                {
                                                    mvaddch(yy, xx, '&');
                                                }
                                                else
                                                {
                                                    if (map[yy][xx] == 'A')
                                                    {
                                                        attron(COLOR_PAIR(5));
                                                        wchar_t message[] = L"⚷";
                                                        mvprintw(yy, xx, "%ls", message);
                                                        attroff(COLOR_PAIR(5));
                                                    }
                                                    else
                                                    {
                                                        if (map[yy][xx] == '2' || map[yy][xx] == '3' || map[yy][xx] == '4' || map[yy][xx] == '5')
                                                        {
                                                            // ⚒🗡🪄➳⚔
                                                            // ⚒𐃉𝅀➳⚔
                                                            /*if (map[yy][xx] == '1')
                                                            {
                                                                wchar_t message[] = L"⚒";
                                                                mvprintw(yy, xx, "%ls", message);
                                                            }*/
                                                            if (map[yy][xx] == '2')
                                                            {
                                                                wchar_t message[] = L"†";
                                                                mvprintw(yy, xx, "%ls", message);
                                                            }
                                                            if (map[yy][xx] == '3')
                                                            {
                                                                wchar_t message[] = L"⚕";
                                                                mvprintw(yy, xx, "%ls", message);
                                                            }
                                                            if (map[yy][xx] == '4')
                                                            {
                                                                wchar_t message[] = L"➳";
                                                                mvprintw(yy, xx, "%ls", message);
                                                            }
                                                            if (map[yy][xx] == '5')
                                                            {
                                                                wchar_t message[] = L"⚔";
                                                                mvprintw(yy, xx, "%ls", message);
                                                            }
                                                        }
                                                        else
                                                        {
                                                            if (map[yy][xx] == '6' || map[yy][xx] == '7' || map[yy][xx] == '8')
                                                            {
                                                                if (map[yy][xx] == '6')
                                                                {
                                                                    wchar_t message[] = L"♥";
                                                                    mvprintw(yy, xx, "%ls", message);
                                                                }
                                                                if (map[yy][xx] == '7')
                                                                {
                                                                    wchar_t message[] = L"⌁";
                                                                    mvprintw(yy, xx, "%ls", message);
                                                                }
                                                                if (map[yy][xx] == '8')
                                                                {
                                                                    wchar_t message[] = L"☠";
                                                                    mvprintw(yy, xx, "%ls", message);
                                                                }
                                                            }
                                                            else
                                                            {
                                                                if (map[yy][xx] == 'Z' || map[yy][xx] == 'X' || map[yy][xx] == 'C' || map[yy][xx] == 'V')
                                                                {
                                                                    if (map[yy][xx] == 'Z')
                                                                    {
                                                                        // ₂₃
                                                                        wchar_t message[] = L"₁";
                                                                        mvprintw(yy, xx, "%ls", message);
                                                                    }
                                                                    if (map[yy][xx] == 'X')
                                                                    {
                                                                        // ₂₃
                                                                        wchar_t message[] = L"₂";
                                                                        mvprintw(yy, xx, "%ls", message);
                                                                    }
                                                                    if (map[yy][xx] == 'C')
                                                                    {
                                                                        // ₂₃
                                                                        wchar_t message[] = L"₃";
                                                                        mvprintw(yy, xx, "%ls", message);
                                                                    }
                                                                    if (map[yy][xx] == 'V')
                                                                    {
                                                                        // ₂₃
                                                                        wchar_t message[] = L"₁";
                                                                        mvprintw(yy, xx, "%ls", message);
                                                                    }
                                                                }
                                                                else
                                                                {
                                                                    attron(COLOR_PAIR(2));
                                                                    wchar_t message[] = L"∙";
                                                                    mvprintw(yy, xx, "%ls", message);
                                                                    attroff(COLOR_PAIR(2));
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            else
                            {
                                attron(COLOR_PAIR(2));
                                mvaddch(yy, xx, 'O');
                                attroff(COLOR_PAIR(2));
                            }
                        }
                    }
                    else
                    {
                        int countt = 0;
                        if (map[yy][xx] == '$')
                        {
                            countt++;
                        }
                        if (yy == (RoomArray[index].TopLeft_Y - 1) || yy == (RoomArray[index].TopLeft_Y + RoomArray[index].Height))
                        {
                            if (countt == 1)
                            {
                                int buzinigga = 0;
                                for (int fort = 0; fort < Door_Placed; fort++)
                                {
                                    if (DoorArray[fort].y == yy && DoorArray[fort].x == xx && DoorArray[fort].lock == true)
                                    {
                                        buzinigga = 1;
                                    }
                                }
                                if (buzinigga == 0)
                                {
                                    attron(COLOR_PAIR(7));
                                    wchar_t message[] = L"∎";
                                    mvprintw(yy, xx, "%ls", message);
                                    attroff(COLOR_PAIR(7));
                                }
                                else
                                {
                                    attron(COLOR_PAIR(5));
                                    wchar_t message[] = L"∎";
                                    mvprintw(yy, xx, "%ls", message);
                                    attroff(COLOR_PAIR(5));
                                }
                            }
                            else
                            {
                                attron(COLOR_PAIR(7));
                                wchar_t message[] = L"≡";
                                mvprintw(yy, xx, "%ls", message);
                                attroff(COLOR_PAIR(7));
                            }
                        }
                        else
                        {
                            if (countt == 1)
                            {
                                int buzinigga = 0;
                                for (int fort = 0; fort < Door_Placed; fort++)
                                {
                                    if (DoorArray[fort].y == yy && DoorArray[fort].x == xx && DoorArray[fort].lock == true)
                                    {
                                        buzinigga = 1;
                                    }
                                }
                                if (buzinigga == 0)
                                {
                                    attron(COLOR_PAIR(7));
                                    wchar_t message[] = L"∎";
                                    mvprintw(yy, xx, "%ls", message);
                                    attroff(COLOR_PAIR(7));
                                }
                                else
                                {
                                    attron(COLOR_PAIR(5));
                                    wchar_t message[] = L"∎";
                                    mvprintw(yy, xx, "%ls", message);
                                    attroff(COLOR_PAIR(5));
                                }
                            }
                            else
                            {
                                attron(COLOR_PAIR(7));
                                wchar_t message[] = L"‖";
                                mvprintw(yy, xx, "%ls", message);
                                attroff(COLOR_PAIR(7));
                            }
                        }
                        // window∅
                        for (int iji = 0; iji < Window_Placed; iji++)
                        {
                            if (WindowArray[iji].x == xx && WindowArray[iji].y == yy)
                            {
                                attron(COLOR_PAIR(4));
                                wchar_t message[] = L"◻";
                                mvprintw(yy, xx, "%ls", message);
                                attroff(COLOR_PAIR(4));
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if (RoomArray[index].wdow == 2)
            {
                for (int yy = (RoomArray[index].TopLeft_Y - 1); yy <= RoomArray[index].TopLeft_Y + RoomArray[index].Height; yy++)
                {
                    for (int xx = (RoomArray[index].TopLeft_x - 1); xx <= RoomArray[index].TopLeft_x + RoomArray[index].Width; xx++)
                    {
                        if (yy >= RoomArray[index].TopLeft_Y && yy <= (RoomArray[index].TopLeft_Y + RoomArray[index].Height - 1) &&
                            xx >= RoomArray[index].TopLeft_x && xx <= (RoomArray[index].TopLeft_x + RoomArray[index].Width - 1))
                        {
                            int prt = 0;
                            /*for (int carol = 0; carol < EnemyCount; carol++)
                            {
                                if (EnemyArray[carol].x == xx && EnemyArray[carol].y == yy && EnemyArray[carol].hp > 0)
                                {
                                    prt = 1;
                                    if (EnemyArray[carol].type == 'd')
                                    {
                                        mvaddch(yy, xx, 'D');
                                    }
                                    if (EnemyArray[carol].type == 'f')
                                    {
                                        mvaddch(yy, xx, 'F');
                                    }
                                    if (EnemyArray[carol].type == 't')
                                    {
                                        mvaddch(yy, xx, 'G');
                                    }
                                    if (EnemyArray[carol].type == 's')
                                    {
                                        mvaddch(yy, xx, 'S');
                                    }
                                    if (EnemyArray[carol].type == 'u')
                                    {
                                        mvaddch(yy, xx, 'U');
                                    }
                                }
                            }*/
                            if (prt == 0)
                            {
                                if (map[yy][xx] != 'O')
                                {
                                    if (map[yy][xx] == '^')
                                    {
                                        int ij;
                                        for (ij = 0; ij < Trap_Placed; ij++)
                                        {
                                            if (yy == TrapArray[ij].y && xx == TrapArray[ij].x)
                                            {
                                                break;
                                            }
                                        }
                                        if (TrapArray[ij].show == 1)
                                        {
                                            attron(COLOR_PAIR(5));
                                            mvaddch(yy, xx, '^');
                                            attroff(COLOR_PAIR(5));
                                        }
                                        else
                                        {
                                            attron(COLOR_PAIR(2));
                                            wchar_t message[] = L"∙";
                                            mvprintw(yy, xx, "%ls", message);
                                            attroff(COLOR_PAIR(2));
                                        }
                                    }
                                    else
                                    {
                                        if (map[yy][xx] == '<')
                                        {
                                            attron(COLOR_PAIR(4));
                                            mvaddch(yy, xx, '<');
                                            attroff(COLOR_PAIR(4));
                                        }
                                        else
                                        {
                                            if (map[yy][xx] == 'G')
                                            {
                                                attron(COLOR_PAIR(7));
                                                wchar_t message[] = L"⛀";
                                                mvprintw(yy, xx, "%ls", message);
                                                attroff(COLOR_PAIR(7));
                                            }
                                            else
                                            {
                                                if (map[yy][xx] == 'g')
                                                {
                                                    attron(COLOR_PAIR(7));
                                                    wchar_t message[] = L"⛂";
                                                    mvprintw(yy, xx, "%ls", message);
                                                    attroff(COLOR_PAIR(7));
                                                }
                                                else
                                                {
                                                    if (map[yy][xx] == '&')
                                                    {
                                                        mvaddch(yy, xx, '&');
                                                    }
                                                    else
                                                    {
                                                        if (map[yy][xx] == 'A')
                                                        {
                                                            attron(COLOR_PAIR(5));
                                                            wchar_t message[] = L"⚷";
                                                            mvprintw(yy, xx, "%ls", message);
                                                            attroff(COLOR_PAIR(5));
                                                        }
                                                        else
                                                        {
                                                            if (map[yy][xx] == '2' || map[yy][xx] == '3' || map[yy][xx] == '4' || map[yy][xx] == '5')
                                                            {
                                                                // ⚒🗡🪄➳⚔
                                                                // ⚒𐃉𝅀➳⚔
                                                                /*if (map[yy][xx] == '1')
                                                                {
                                                                    wchar_t message[] = L"⚒";
                                                                    mvprintw(yy, xx, "%ls", message);
                                                                }*/
                                                                if (map[yy][xx] == '2')
                                                                {
                                                                    wchar_t message[] = L"†";
                                                                    mvprintw(yy, xx, "%ls", message);
                                                                }
                                                                if (map[yy][xx] == '3')
                                                                {
                                                                    wchar_t message[] = L"⚕";
                                                                    mvprintw(yy, xx, "%ls", message);
                                                                }
                                                                if (map[yy][xx] == '4')
                                                                {
                                                                    wchar_t message[] = L"➳";
                                                                    mvprintw(yy, xx, "%ls", message);
                                                                }
                                                                if (map[yy][xx] == '5')
                                                                {
                                                                    wchar_t message[] = L"⚔";
                                                                    mvprintw(yy, xx, "%ls", message);
                                                                }
                                                            }
                                                            else
                                                            {
                                                                if (map[yy][xx] == '6' || map[yy][xx] == '7' || map[yy][xx] == '8')
                                                                {
                                                                    if (map[yy][xx] == '6')
                                                                    {
                                                                        wchar_t message[] = L"♥";
                                                                        mvprintw(yy, xx, "%ls", message);
                                                                    }
                                                                    if (map[yy][xx] == '7')
                                                                    {
                                                                        wchar_t message[] = L"⌁";
                                                                        mvprintw(yy, xx, "%ls", message);
                                                                    }
                                                                    if (map[yy][xx] == '8')
                                                                    {
                                                                        wchar_t message[] = L"☠";
                                                                        mvprintw(yy, xx, "%ls", message);
                                                                    }
                                                                }
                                                                else
                                                                {
                                                                    if (map[yy][xx] == 'Z' || map[yy][xx] == 'X' || map[yy][xx] == 'C' || map[yy][xx] == 'V')
                                                                    {
                                                                        if (map[yy][xx] == 'Z')
                                                                        {
                                                                            // ₂₃
                                                                            wchar_t message[] = L"₁";
                                                                            mvprintw(yy, xx, "%ls", message);
                                                                        }
                                                                        if (map[yy][xx] == 'X')
                                                                        {
                                                                            // ₂₃
                                                                            wchar_t message[] = L"₂";
                                                                            mvprintw(yy, xx, "%ls", message);
                                                                        }
                                                                        if (map[yy][xx] == 'C')
                                                                        {
                                                                            // ₂₃
                                                                            wchar_t message[] = L"₃";
                                                                            mvprintw(yy, xx, "%ls", message);
                                                                        }
                                                                        if (map[yy][xx] == 'V')
                                                                        {
                                                                            // ₂₃
                                                                            wchar_t message[] = L"₁";
                                                                            mvprintw(yy, xx, "%ls", message);
                                                                        }
                                                                    }
                                                                    else
                                                                    {
                                                                        attron(COLOR_PAIR(2));
                                                                        wchar_t message[] = L"∙";
                                                                        mvprintw(yy, xx, "%ls", message);
                                                                        attroff(COLOR_PAIR(2));
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    attron(COLOR_PAIR(2));
                                    mvaddch(yy, xx, 'O');
                                    attroff(COLOR_PAIR(2));
                                }
                            }
                        }
                        else
                        {

                            int countt = 0;
                            if (map[yy][xx] == '$')
                            {
                                countt++;
                            }
                            if (yy == (RoomArray[index].TopLeft_Y - 1) || yy == (RoomArray[index].TopLeft_Y + RoomArray[index].Height))
                            {
                                if (countt == 1)
                                {
                                    int buzinigga = 0;
                                    for (int fort = 0; fort < Door_Placed; fort++)
                                    {
                                        if (DoorArray[fort].y == yy && DoorArray[fort].x == xx && DoorArray[fort].lock == true)
                                        {
                                            buzinigga = 1;
                                        }
                                    }
                                    if (buzinigga == 0)
                                    {
                                        attron(COLOR_PAIR(7));
                                        wchar_t message[] = L"∎";
                                        mvprintw(yy, xx, "%ls", message);
                                        attroff(COLOR_PAIR(7));
                                    }
                                    else
                                    {
                                        attron(COLOR_PAIR(5));
                                        wchar_t message[] = L"∎";
                                        mvprintw(yy, xx, "%ls", message);
                                        attroff(COLOR_PAIR(5));
                                    }
                                }
                                else
                                {
                                    attron(COLOR_PAIR(7));
                                    wchar_t message[] = L"≡";
                                    mvprintw(yy, xx, "%ls", message);
                                    attroff(COLOR_PAIR(7));
                                }
                            }
                            else
                            {
                                if (countt == 1)
                                {
                                    int buzinigga = 0;
                                    for (int fort = 0; fort < Door_Placed; fort++)
                                    {
                                        if (DoorArray[fort].y == yy && DoorArray[fort].x == xx && DoorArray[fort].lock == true)
                                        {
                                            buzinigga = 1;
                                        }
                                    }
                                    if (buzinigga == 0)
                                    {
                                        attron(COLOR_PAIR(7));
                                        wchar_t message[] = L"∎";
                                        mvprintw(yy, xx, "%ls", message);
                                        attroff(COLOR_PAIR(7));
                                    }
                                    else
                                    {
                                        attron(COLOR_PAIR(5));
                                        wchar_t message[] = L"∎";
                                        mvprintw(yy, xx, "%ls", message);
                                        attroff(COLOR_PAIR(5));
                                    }
                                }
                                else
                                {
                                    attron(COLOR_PAIR(7));
                                    wchar_t message[] = L"‖";
                                    mvprintw(yy, xx, "%ls", message);
                                    attroff(COLOR_PAIR(7));
                                }
                            }
                            // window∅
                            for (int iji = 0; iji < Window_Placed; iji++)
                            {
                                if (WindowArray[iji].x == xx && WindowArray[iji].y == yy)
                                {
                                    attron(COLOR_PAIR(3));
                                    wchar_t message[] = L"◻";
                                    mvprintw(yy, xx, "%ls", message);
                                    attroff(COLOR_PAIR(3));
                                }
                            }
                        }
                    }
                }
                RoomArray[index].wdow = -2;
            }
        }
    }

    for (int carol = 0; carol < EnemyCount; carol++)
    {
        if (EnemyArray[carol].hp > 0)
        {
            int indexx = -1;
            for (int index = 0; index < Room_Placed; index++)
            {
                if (EnemyArray[carol].y >= RoomArray[index].TopLeft_Y && EnemyArray[carol].y <= RoomArray[index].TopLeft_Y + RoomArray[index].Height - 1 &&
                    EnemyArray[carol].x >= RoomArray[index].TopLeft_x && EnemyArray[carol].x <= RoomArray[index].TopLeft_x + RoomArray[index].Width - 1)
                {
                    indexx = index;
                    break;
                }
            }
            if (RoomArray[indexx].show == true || m == 1 || RoomArray[indexx].wdow == 2)
            {
                if (EnemyArray[carol].type == 'd')
                {
                    mvaddch(EnemyArray[carol].y, EnemyArray[carol].x, 'D');
                }
                if (EnemyArray[carol].type == 'f')
                {
                    mvaddch(EnemyArray[carol].y, EnemyArray[carol].x, 'F');
                }
                if (EnemyArray[carol].type == 't')
                {
                    mvaddch(EnemyArray[carol].y, EnemyArray[carol].x, 'G');
                }
                if (EnemyArray[carol].type == 's')
                {
                    mvaddch(EnemyArray[carol].y, EnemyArray[carol].x, 'S');
                }
                if (EnemyArray[carol].type == 'u')
                {
                    mvaddch(EnemyArray[carol].y, EnemyArray[carol].x, 'U');
                }
            }
        }
    }
    /*for (int yy = 0; yy <= rows; yy++)
    {
        for (int xx = 0; xx <= cols; xx++)
        {
            mvaddch(yy,xx,map[yy][xx]);
        }
    }*/
    // set color
    if (color == 0)
    {
        attron(COLOR_PAIR(5) | A_BOLD);
        mvaddch(y, x, '@');
        attroff(COLOR_PAIR(5) | A_BOLD);
    }
    if (color == 1)
    {
        attron(COLOR_PAIR(1) | A_BOLD);
        mvaddch(y, x, '@');
        attroff(COLOR_PAIR(1) | A_BOLD);
    }
    if (color == 2)
    {
        attron(COLOR_PAIR(2) | A_BOLD);
        mvaddch(y, x, '@');
        attroff(COLOR_PAIR(2) | A_BOLD);
    }
    if (color == 3)
    {
        attron(COLOR_PAIR(7) | A_BOLD);
        mvaddch(y, x, '@');
        attroff(COLOR_PAIR(7) | A_BOLD);
    }
    if (color == 4)
    {
        attron(COLOR_PAIR(4) | A_BOLD);
        mvaddch(y, x, '@');
        attroff(COLOR_PAIR(4) | A_BOLD);
    }
    if (color == 5)
    {
        attron(COLOR_PAIR(3) | A_BOLD);
        mvaddch(y, x, '@');
        attroff(COLOR_PAIR(3) | A_BOLD);
    }
    // Messages
    mvprintw(0, 0, "%s", Line1);
    mvprintw(1, 0, "%s", Line2);
    strcpy(Line1, "                                                                   ");
    // Stats
    mvprintw(rows, 0, "Gold: (%d)     HP: (%d)     AncientKeys: (%d)     Moves: (%d)     Food: (%lf)       level: (%d)", playerGOLD, playerHP, AncientKeys - UsedKey, MovesTillLastFood, PlayerFood, level);
    firsttime = false;
    if (speed > 0)
    {
        if (speed == 2)
        {
            speed -= 0.1;
        }
        else
        {
            speed -= 0.1;
            if (((int)(speed * 10)) % 2 == 0)
            {
                MakeGame(c);
            }
        }
    }
}

void NewGame()
{
    start_color();
    clear();
    curs_set(0);
    int c=113;
    keypad(stdscr, true);
    noecho();
    do
    {
        MakeGame(c);
    } while ((c = getch()) != 27);
    MainMenu();
}

void Times_PLayed()
{
    FILE *file;
    char filename[256];
    snprintf(filename, sizeof(filename), "%s_Times_Played.txt", Player_UserName);
    file = fopen(filename, "r");
    if (file != NULL)
    {
        fclose(file);
    }
    else
    {
        file = fopen(filename, "w");
        fprintf(file, "0");
        fclose(file);
    }
}

void BeforeGame()
{
    Times_PLayed();
    int cursor = 0;
    int c;
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    int colss = (getmaxx(stdscr) / 2);
    colss -= 20;
    int u = 0;
    do
    {
        clear();
        refresh();
        attron(A_BOLD);
        mvprintw(5, colss - 1, "Please Choose One Of The Following\n\n\n");
        attroff(A_BOLD);

        if (cursor == 0)
        {
            mvprintw(9, colss, "1- ");
            attron(COLOR_PAIR(7) | A_BOLD);
            mvprintw(9, colss + 4, "Create a New Game\n");
            attroff(COLOR_PAIR(7) | A_BOLD);
        }
        else
        {
            mvprintw(9, colss, "1- Create a New Game\n");
        }

        if (cursor == 1)
        {
            mvprintw(12, colss, "2- ");
            attron(COLOR_PAIR(7) | A_BOLD);
            mvprintw(12, colss + 4, "Continue the last Game");
            attroff(COLOR_PAIR(7) | A_BOLD);
        }
        else
        {
            mvprintw(12, colss, "2- Continue the last Game");
        }

        if (cursor == 2)
        {
            mvprintw(15, colss, "3- ");
            attron(COLOR_PAIR(7) | A_BOLD);
            mvprintw(15, colss + 4, "Scores Table");
            attroff(COLOR_PAIR(7) | A_BOLD);
        }
        else
        {
            mvprintw(15, colss, "3- Scores Table");
        }

        if (cursor == 3)
        {
            mvprintw(18, colss, "4- ");
            attron(COLOR_PAIR(7) | A_BOLD);
            mvprintw(18, colss + 4, "Settings");
            attroff(COLOR_PAIR(7) | A_BOLD);
        }
        else
        {
            mvprintw(18, colss, "4- Settings");
        }

        c = getch();

        if (c == KEY_UP)
        {
            cursor--;
            if (cursor < 0)
                cursor = 3;
        }

        if (c == KEY_DOWN)
        {
            cursor++;
            if (cursor > 3)
                cursor = 0;
        }

    } while (c != 10);
    curs_set(1);
    echo();
    if (cursor == 0)
    {
        clear();
        NewGame();
    }
    if (cursor == 1)
    {
        clear();
        load();
    }
    if (cursor == 2)
    {
        ScoreBoard();
        clear();
    }
    if (cursor == 3)
    {
        clear();
        Settings();
    }
}

int main()
{
    setlocale(LC_ALL, "");
    srand(time(0));
    initscr();
    int r,c;
    getmaxyx(stdscr, r, c);
    rows += r;
    cols += c;
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(5, COLOR_RED, COLOR_BLACK);
    init_pair(6, COLOR_WHITE, COLOR_BLACK);
    init_pair(7, COLOR_YELLOW, COLOR_BLACK);
    Rogue_Animation();
    MainMenu();
    BeforeGame();
    refresh();
    endwin();
    return 0;
}