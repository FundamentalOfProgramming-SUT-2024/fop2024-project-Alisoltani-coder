// Ali Soltani 403106092
#pragma region includings
#include <mysql/mysql.h>
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
    int kaboos;
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
} Enemy;

typedef struct Wasted_Weapon
{
    int x;
    int y;
} Wasted_Weapon;

#pragma endregion

#pragma region Globool
int guesting = 0;
int logiregi = 0;
int hiden_door_show = 0;
int lood = 0;
const int rows = 38;
const int cols = 183;
int genone = 0;
char Line1[200];
char Line2[200];
char Player_UserName[100];
int level = 1;
int color = 4;
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
Wasted_Weapon *Wasted_Weapon_Array2;
Enemy *EnemyArray2;
int Enemycount2 = 0;
int Wasted_Weapon_count2 = 0;
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
// color();
// 1 Blue
// 2 Green
// 3 Cyan
// 4 Magenta
// 5 Red
// 6 White
// 7 Yellow

int y, x;
char map[38][183];
char map3[24][24];
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
void play_music(const char *filename);
void Rogue_Animation();
char LoginorRegisterPage();
void ForgotPassword(const char Username[100], const char text[100]);
void Login();
void Register();
void MainMenu();
void Colours();
void Song();
void Difficulty();
void Settings();
int YesNoChoice();
void FoodChoice();
void SpellChoice();
void WeaponChoice();
void SaveGame_StairCase();
void LoadGame_StairCase();
void AddtoScoreBoard();
void ScoreBoard();
void MakeRoom();
void Player_Time();
void load();
void save();
void Enemy_gen();
void Enemy_Move();
void Lose();
void Won();
void battle(int yy, int xx);
int Check_Enemy(int yy, int xx);
void Far_Battle();
int MakeGame(int c);
void NewGame();
void Times_PLayed();
void BeforeGame();
int main();
void Telesm_Room();
int YesNoChoice_H();
void Inventory();
#pragma endregion
#define TRP L"^"
//∙
// mysql -u root -p
// USE GameData;
/*
SELECT * FROM PlayerStats;
SELECT * FROM AncientKeys;
SELECT * FROM Room;
SELECT * FROM Trap;
SELECT * FROM `Door`;
SELECT * FROM Weapon;
SELECT * FROM Spell;
SELECT * FROM Corridor;
SELECT * FROM `Window`;
SELECT * FROM Food;
SELECT * FROM Enemy;
SELECT * FROM Wasted_Weapon;
*/
// gcc -o prjcopy prjcopy.c -I/usr/include/mysql -I/usr/include/SDL2 -lmysqlclient -lm -lncursesw -lSDL2 -lSDL2_mixer
//./prjcopy
// mysql -u root -p
// USE GameData
// SHOW TABLES;
// SELECT * FROM PlayerStats
MYSQL *conn;
void connect_to_db()
{
    conn = mysql_init(NULL);
    if (conn == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }

    if (mysql_real_connect(conn, "localhost", "root", "123456789aAbBcC@#$", "GameData", 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "mysql_real_connect() failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }

    printf("Connected to the database successfully.\n"); // Debugging line
}

void save2()
{
    connect_to_db();

    // Insert or update PlayerStats
    char query[2048];
    snprintf(query, sizeof(query),
             "INSERT INTO PlayerStats (Player_UserName, y, x, `rows`, `cols`, level, color, playerGOLD, playerHP, playerHPMax, TillLast, FoodEffect, AncientKeys, UsedKey, WeaponCount, SpellCount, FoodCount, EnemyCount, MovesTillLastFood, PlayerFood, m, onetime, power, speed, Health_Pot, MovesTillLastpot, Current_Weapon, Wasted_Weapon_Count) "
             "VALUES ('%s', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %lf, %d, %d, %lf, %lf, %d, %d, '%c', %d) "
             "ON DUPLICATE KEY UPDATE y=VALUES(y), x=VALUES(x), `rows`=VALUES(`rows`), `cols`=VALUES(`cols`), level=VALUES(level), color=VALUES(color), playerGOLD=VALUES(playerGOLD), playerHP=VALUES(playerHP), playerHPMax=VALUES(playerHPMax), TillLast=VALUES(TillLast), FoodEffect=VALUES(FoodEffect), AncientKeys=VALUES(AncientKeys), UsedKey=VALUES(UsedKey), WeaponCount=VALUES(WeaponCount), SpellCount=VALUES(SpellCount), FoodCount=VALUES(FoodCount), EnemyCount=VALUES(EnemyCount), MovesTillLastFood=VALUES(MovesTillLastFood), PlayerFood=VALUES(PlayerFood), m=VALUES(m), onetime=VALUES(onetime), power=VALUES(power), speed=VALUES(speed), Health_Pot=VALUES(Health_Pot), MovesTillLastpot=VALUES(MovesTillLastpot), Current_Weapon=VALUES(Current_Weapon), Wasted_Weapon_Count=VALUES(Wasted_Weapon_Count)",
             Player_UserName, y, x, rows, cols, level, color, playerGOLD, playerHP, playerHPMax, TillLast, FoodEffect, AncientKeys, UsedKey, WeaponCount, SpellCount, FoodCount, EnemyCount, MovesTillLastFood, PlayerFood, m, onetime, power, speed, Health_Pot, MovesTillLastpot, Current_Weapon, Wasted_Weapon_Count);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        mysql_close(conn);
        return;
    }

    // Insert or update AncientKeys
    for (int i = 0; i < AncientKeys; i++)
    {
        snprintf(query, sizeof(query),
                 "INSERT INTO AncientKeys (Player_UserName, KeyID, KeyStatus) "
                 "VALUES ('%s', %d, %d) ON DUPLICATE KEY UPDATE KeyStatus=VALUES(KeyStatus)",
                 Player_UserName, i, AncientKey[i]);

        if (mysql_query(conn, query))
        {
            fprintf(stderr, "%s\n", mysql_error(conn));
            mysql_close(conn);
            return;
        }
    }

    // Insert or update Room
    for (int i = 0; i < Room_Placed; i++)
    {
        snprintf(query, sizeof(query),
                 "INSERT INTO Room (Player_UserName, TopLeft_x, TopLeft_Y, Width, Height, middlex, middley, `show`, wdow, kaboos) "
                 "VALUES ('%s', %d, %d, %d, %d, %d, %d, %d, %d, %d) "
                 "ON DUPLICATE KEY UPDATE TopLeft_x=VALUES(TopLeft_x), TopLeft_Y=VALUES(TopLeft_Y), Width=VALUES(Width), Height=VALUES(Height), middlex=VALUES(middlex), middley=VALUES(middley), `show`=VALUES(`show`), wdow=VALUES(wdow), kaboos=VALUES(kaboos)",
                 Player_UserName, RoomArray[i].TopLeft_x, RoomArray[i].TopLeft_Y, RoomArray[i].Width, RoomArray[i].Height, RoomArray[i].middlex, RoomArray[i].middley, RoomArray[i].show, RoomArray[i].wdow, RoomArray[i].kaboos);

        if (mysql_query(conn, query))
        {
            fprintf(stderr, "%s\n", mysql_error(conn));
            mysql_close(conn);
            return;
        }
    }

    // Insert or update Trap
    for (int i = 0; i < Trap_Placed; i++)
    {
        snprintf(query, sizeof(query),
                 "INSERT INTO Trap (Player_UserName, `show`, x, y) "
                 "VALUES ('%s', %d, %d, %d) "
                 "ON DUPLICATE KEY UPDATE `show`=VALUES(`show`), x=VALUES(x), y=VALUES(y)",
                 Player_UserName, TrapArray[i].show, TrapArray[i].x, TrapArray[i].y);

        if (mysql_query(conn, query))
        {
            fprintf(stderr, "%s\n", mysql_error(conn));
            mysql_close(conn);
            return;
        }
    }

    // Insert or update Door
    for (int i = 0; i < Door_Placed; i++)
    {
        snprintf(query, sizeof(query),
                 "INSERT INTO `Door` (Player_UserName, `lock`, y, x, PassWord, trysleft) "
                 "VALUES ('%s', %d, %d, %d, %d, %d) "
                 "ON DUPLICATE KEY UPDATE `lock`=VALUES(`lock`), y=VALUES(y), x=VALUES(x), PassWord=VALUES(PassWord), trysleft=VALUES(trysleft)",
                 Player_UserName, DoorArray[i].lock, DoorArray[i].y, DoorArray[i].x, DoorArray[i].PassWord, DoorArray[i].trysleft);

        if (mysql_query(conn, query))
        {
            fprintf(stderr, "%s\n", mysql_error(conn));
            mysql_close(conn);
            return;
        }
    }

    // Insert or update Weapon
    for (int i = 0; i < WeaponCount; i++)
    {
        snprintf(query, sizeof(query),
                 "INSERT INTO Weapon (Player_UserName, mode, times_use_left) "
                 "VALUES ('%s', %d, %d) "
                 "ON DUPLICATE KEY UPDATE mode=VALUES(mode), times_use_left=VALUES(times_use_left)",
                 Player_UserName, WeaponArray[i].mode, WeaponArray[i].times_use_left);

        if (mysql_query(conn, query))
        {
            fprintf(stderr, "%s\n", mysql_error(conn));
            mysql_close(conn);
            return;
        }
    }

    // Insert or update Spell
    for (int i = 0; i < SpellCount; i++)
    {
        snprintf(query, sizeof(query),
                 "INSERT INTO Spell (Player_UserName, mode) "
                 "VALUES ('%s', %d) "
                 "ON DUPLICATE KEY UPDATE mode=VALUES(mode)",
                 Player_UserName, SpellArray[i].mode);

        if (mysql_query(conn, query))
        {
            fprintf(stderr, "%s\n", mysql_error(conn));
            mysql_close(conn);
            return;
        }
    }

    // Insert or update Corridor
    for (int i = 0; i < Corridor_Placed; i++)
    {
        snprintf(query, sizeof(query),
                 "INSERT INTO Corridor (Player_UserName, x, y) "
                 "VALUES ('%s', %d, %d) "
                 "ON DUPLICATE KEY UPDATE x=VALUES(x), y=VALUES(y)",
                 Player_UserName, CorridorArray[i].x, CorridorArray[i].y);

        if (mysql_query(conn, query))
        {
            fprintf(stderr, "%s\n", mysql_error(conn));
            mysql_close(conn);
            return;
        }

        // Insert or update Window
        for (int i = 0; i < Window_Placed; i++)
        {
            snprintf(query, sizeof(query),
                     "INSERT INTO `Window` (Player_UserName, x, y, dir) "
                     "VALUES ('%s', %d, %d, %d) "
                     "ON DUPLICATE KEY UPDATE x=VALUES(x), y=VALUES(y), dir=VALUES(dir)",
                     Player_UserName, WindowArray[i].x, WindowArray[i].y, WindowArray[i].dir);

            if (mysql_query(conn, query))
            {
                fprintf(stderr, "%s\n", mysql_error(conn));
                mysql_close(conn);
                return;
            }
        }

        // Insert or update Food
        for (int i = 0; i < FoodCount; i++)
        {
            snprintf(query, sizeof(query),
                     "INSERT INTO Food (Player_UserName, type) "
                     "VALUES ('%s', %d) "
                     "ON DUPLICATE KEY UPDATE type=VALUES(type)",
                     Player_UserName, FoodArray[i].type);

            if (mysql_query(conn, query))
            {
                fprintf(stderr, "%s\n", mysql_error(conn));
                mysql_close(conn);
                return;
            }
        }

        // Insert or update Enemy
        for (int i = 0; i < EnemyCount; i++)
        {
            snprintf(query, sizeof(query),
                     "INSERT INTO Enemy (Player_UserName, `type`, hp, `move`, x, y) "
                     "VALUES ('%s', '%c', %lf, %d, %d, %d) "
                     "ON DUPLICATE KEY UPDATE `type`=VALUES(`type`), hp=VALUES(hp), `move`=VALUES(`move`), x=VALUES(x), y=VALUES(y)",
                     Player_UserName, EnemyArray[i].type, EnemyArray[i].hp, EnemyArray[i].move, EnemyArray[i].x, EnemyArray[i].y);

            if (mysql_query(conn, query))
            {
                fprintf(stderr, "%s\n", mysql_error(conn));
                mysql_close(conn);
                return;
            }
        }

        // Insert or update Wasted_Weapon
        for (int i = 0; i < Wasted_Weapon_Count; i++)
        {
            snprintf(query, sizeof(query),
                     "INSERT INTO Wasted_Weapon (Player_UserName, x, y) "
                     "VALUES ('%s', %d, %d) "
                     "ON DUPLICATE KEY UPDATE x=VALUES(x), y=VALUES(y)",
                     Player_UserName, Wasted_Weapon_Array[i].x, Wasted_Weapon_Array[i].y);

            if (mysql_query(conn, query))
            {
                fprintf(stderr, "%s\n", mysql_error(conn));
                mysql_close(conn);
                return;
            }
        }

        // Free allocated memory and close MySQL connection
        mysql_close(conn);

        free(RoomArray);
        free(TrapArray);
        free(CorridorArray);
        free(WindowArray);
        free(DoorArray);
        free(WeaponArray);
        free(SpellArray);
        free(FoodArray);
        free(Wasted_Weapon_Array);
        free(EnemyArray);
        endwin();
        exit(0);
    }
}

Mix_Music *music;

void play_music(const char *filename)
{
    // Stop current music if any
    if (music != NULL)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(music);
        music = NULL;
    }

    // Load the new music file
    music = Mix_LoadMUS(filename);
    if (music == NULL)
    {
        fprintf(stderr, "Mix_LoadMUS: %s\n", Mix_GetError());
        exit(1);
    }

    // Play the new music
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
    mvprintw(start_y + 9, start_x, "  R:::::::::::::RR   O:::::O     O:::::O G:::::G    GGGGGGGGGG U:::::U     U:::::U   E:::::::::::::::E   \n");
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
            strcpy(Player_UserName, Username);
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
    strcpy(Player_UserName, Username);
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

        wchar_t message1[] = L"╔══════════════════════════════╗";
        mvprintw(4, colss - 10, "%ls", message1);
        wchar_t message2[] = L"║           MAIN MENU          ║";
        mvprintw(5, colss - 10, "%ls", message2);
        wchar_t message3[] = L"╚══════════════════════════════╝";
        mvprintw(6, colss - 10, "%ls", message3);
        attroff(A_BOLD);

        if (cursor == 0)
        {
            wchar_t message[] = L"📝 ";
            mvprintw(9, colss + 1, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(9, colss + 5 + 1, "Register\n");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"📝";
            mvprintw(9, colss + 1, "%ls Register\n", message);
        }

        if (cursor == 1)
        {
            wchar_t message[] = L"🔑 ";
            mvprintw(12, colss + 1, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(12, colss + 5 + 1, "Login\n");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"🔑";
            mvprintw(12, colss + 1, "%ls Login\n", message);
        }

        if (cursor == 2)
        {
            wchar_t message[] = L"👤 ";
            mvprintw(15, colss + 1, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(15, colss + 5 + 1, "Guest\n");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"👤";
            mvprintw(15, colss + 1, "%ls Guest\n", message);
        }

        if (cursor == 3)
        {
            wchar_t message[] = L"❌ ";
            mvprintw(18, colss + 1, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(18, colss + 5 + 1, "Exit\n");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"❌";
            mvprintw(18, colss + 1, "%ls Exit\n", message);
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
        logiregi = 1;
        Register();
    }
    if (cursor == 1)
    {
        clear();
        logiregi = 1;
        Login();
    }
    if (cursor == 2)
    {
        clear();
        guesting = 1;
        char username[100];
        printw("Username: ");
        scanw("%s", username);
        clear();
        printw("Wellcome! %s\n", username);
        strcpy(Player_UserName, username);
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
        wchar_t message1[] = L"╔══════════════════════════════╗";
        mvprintw(4, colss - 10, "%ls", message1);
        wchar_t message2[] = L"║            Color             ║";
        mvprintw(5, colss - 10, "%ls", message2);
        wchar_t message3[] = L"╚══════════════════════════════╝";
        mvprintw(6, colss - 10, "%ls", message3);
        attroff(A_BOLD);

        if (cursor == 0)
        {
            wchar_t message[] = L"₪ ";
            mvprintw(9, colss, "%ls", message);
            attron(COLOR_PAIR(55) | A_BOLD);
            mvprintw(9, colss + 4, "Red");
            attroff(COLOR_PAIR(55) | A_BOLD);
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
            attron(COLOR_PAIR(11) | A_BOLD);
            mvprintw(12, colss + 4, "Blue");
            attroff(COLOR_PAIR(11) | A_BOLD);
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
            attron(COLOR_PAIR(22) | A_BOLD);
            mvprintw(15, colss + 4, "Green");
            attroff(COLOR_PAIR(22) | A_BOLD);
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
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(18, colss + 4, "Yellow");
            attroff(COLOR_PAIR(77) | A_BOLD);
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
            attron(COLOR_PAIR(44) | A_BOLD);
            mvprintw(21, colss + 4, "Magenta");
            attroff(COLOR_PAIR(44) | A_BOLD);
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
            attron(COLOR_PAIR(33) | A_BOLD);
            mvprintw(24, colss + 4, "Cyan");
            attroff(COLOR_PAIR(33) | A_BOLD);
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
    // curs_set(1);
    // echo();
    color = cursor;
    clear();
    Settings();
}

void Song()
{
    clear();
    int c;
    int u = 0;
    int colss = (getmaxx(stdscr) / 2);
    colss -= 10;
    int cursor = 0;
    do
    {
        clear();
        attron(A_BOLD);
        wchar_t message1[] = L"╔══════════════════════════════╗";
        mvprintw(4, colss - 10, "%ls", message1);
        wchar_t message2[] = L"║            Songs             ║";
        mvprintw(5, colss - 10, "%ls", message2);
        wchar_t message3[] = L"╚══════════════════════════════╝";
        mvprintw(6, colss - 10, "%ls", message3);
        attroff(A_BOLD);
        if (cursor == 0)
        {
            wchar_t message[] = L"₪ ";
            mvprintw(9, colss, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(9, colss + 4, "Song1");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"₪ ";
            mvprintw(9, colss, "%ls Song1\n", message);
        }
        if (cursor == 1)
        {
            wchar_t message[] = L"₪ ";
            mvprintw(11, colss, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(11, colss + 4, "Song2");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"₪ ";
            mvprintw(11, colss, "%ls Song2\n", message);
        }
        if (cursor == 2)
        {
            wchar_t message[] = L"₪ ";
            mvprintw(13, colss, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(13, colss + 4, "Song3");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"₪ ";
            mvprintw(13, colss, "%ls Song3\n", message);
        }
        if (cursor == 3)
        {
            wchar_t message[] = L"₪ ";
            mvprintw(15, colss, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(15, colss + 4, "Song4");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"₪ ";
            mvprintw(15, colss, "%ls Song4\n", message);
        }
        if (cursor == 4)
        {
            wchar_t message[] = L"₪ ";
            mvprintw(17, colss, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(17, colss + 4, "STOP");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"₪ ";
            mvprintw(17, colss, "%ls STOP\n", message);
        }
        if (cursor == 5)
        {
            wchar_t message[] = L"₪ ";
            mvprintw(19, colss, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(19, colss + 4, "PLAY");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"₪ ";
            mvprintw(19, colss, "%ls PLAY\n", message);
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
    clear();
    if (cursor == 0)
    {
        SDL_Delay(1000);
        play_music("1.mp3");
        SDL_Delay(1000);
    }
    if (cursor == 1)
    {
        SDL_Delay(1000);
        play_music("2.mp3");
        SDL_Delay(1000);
    }
    if (cursor == 2)
    {
        SDL_Delay(1000);
        play_music("3.mp3");
        SDL_Delay(1000);
    }
    if (cursor == 3)
    {
        SDL_Delay(1000);
        play_music("4.mp3");
        SDL_Delay(1000);
    }
    if (cursor == 4)
    {
        Mix_PauseMusic();
    }
    if (cursor == 5)
    {
        Mix_ResumeMusic();
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
    int colss = (getmaxx(stdscr) / 2);
    colss -= 10;
    do
    {
        clear();
        attron(A_BOLD);
        wchar_t message1[] = L"╔══════════════════════════════╗";
        mvprintw(4, colss - 10, "%ls", message1);
        wchar_t message2[] = L"║          Difficulty          ║";
        mvprintw(5, colss - 10, "%ls", message2);
        wchar_t message3[] = L"╚══════════════════════════════╝";
        mvprintw(6, colss - 10, "%ls", message3);
        attroff(A_BOLD);
        if (cursor == 0)
        {
            wchar_t message[] = L"₪ ";
            mvprintw(9, colss, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(9, colss + 4, "Peaceful");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"₪ ";
            mvprintw(9, colss, "%ls Peaceful\n", message);
        }
        if (cursor == 1)
        {
            wchar_t message[] = L"₪ ";
            mvprintw(11, colss, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(11, colss + 4, "Easy");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"₪ ";
            mvprintw(11, colss, "%ls Easy\n", message);
        }
        if (cursor == 2)
        {
            wchar_t message[] = L"₪ ";
            mvprintw(13, colss, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(13, colss + 4, "Medium");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"₪ ";
            mvprintw(13, colss, "%ls Medium\n", message);
        }
        if (cursor == 3)
        {
            wchar_t message[] = L"₪ ";
            mvprintw(15, colss, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(15, colss + 4, "Hard");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"₪ ";
            mvprintw(15, colss, "%ls Hard\n", message);
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
    int colss = (getmaxx(stdscr) / 2);
    colss -= 10;
    do
    {
        clear();
        attron(A_BOLD);
        wchar_t message1[] = L"╔══════════════════════════════╗";
        mvprintw(4, colss - 10, "%ls", message1);
        wchar_t message2[] = L"║           Settings           ║";
        mvprintw(5, colss - 10, "%ls", message2);
        wchar_t message3[] = L"╚══════════════════════════════╝";
        mvprintw(6, colss - 10, "%ls", message3);
        attroff(A_BOLD);
        if (cursor == 0)
        {
            wchar_t message[] = L"🎨 ";
            mvprintw(9, colss, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(9, colss + 4 + 2, "Colors");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"🎨 ";
            mvprintw(9, colss, "%ls Colors\n", message);
        }
        if (cursor == 1)
        {
            wchar_t message[] = L"🚀 ";
            mvprintw(11, colss, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(11, colss + 4 + 2, "Difficulty");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"🚀 ";
            mvprintw(11, colss, "%ls Difficulty\n", message);
        }
        if (cursor == 2)
        {
            wchar_t message[] = L"🎵 ";
            mvprintw(13, colss, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(13, colss + 4 + 2, "Song");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"🎵 ";
            mvprintw(13, colss, "%ls Song\n", message);
        }
        if (cursor == 3)
        {
            wchar_t message[] = L"❌ ";
            mvprintw(15, colss, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(15, colss + 4 + 2, "Back");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"❌ ";
            mvprintw(15, colss, "%ls Back\n", message);
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
    int colss = (getmaxx(stdscr) / 2);
    colss -= 10;
    do
    {
        clear();
        attron(A_BOLD);
        wchar_t message1[] = L"╔══════════════════════════════╗";
        mvprintw(4, colss - 10, "%ls", message1);
        wchar_t message2[] = L"║         Pick up item?        ║";
        mvprintw(5, colss - 10, "%ls", message2);
        wchar_t message3[] = L"╚══════════════════════════════╝";
        mvprintw(6, colss - 10, "%ls", message3);
        attroff(A_BOLD);
        if (cursor == 0)
        {
            wchar_t message[] = L"₪ ";
            mvprintw(9, colss, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(9, colss + 4, "No");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"₪ ";
            mvprintw(9, colss, "%ls No\n", message);
        }
        if (cursor == 1)
        {
            wchar_t message[] = L"₪ ";
            mvprintw(11, colss, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(11, colss + 4, "Yes");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"₪ ";
            mvprintw(11, colss, "%ls Yes\n", message);
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
        return 0;
    }
    else
    {
        return 1;
    }
}

int YesNoChoice_H()
{
    clear();
    int c;
    int u = 0;
    int cursor = 0;
    int colss = (getmaxx(stdscr) / 2);
    colss -= 10;
    do
    {
        clear();
        attron(A_BOLD);
        wchar_t message1[] = L"╔═════════════════════════════════════════╗";
        mvprintw(4, colss - 10, "%ls", message1);
        wchar_t message2[] = L"║  Do you want to go to the hidden room?  ║";
        mvprintw(5, colss - 10, "%ls", message2);
        wchar_t message3[] = L"╚═════════════════════════════════════════╝";
        mvprintw(6, colss - 10, "%ls", message3);
        attroff(A_BOLD);
        if (cursor == 0)
        {
            wchar_t message[] = L"₪ ";
            mvprintw(9, colss, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(9, colss + 4, "No");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"₪ ";
            mvprintw(9, colss, "%ls No\n", message);
        }
        if (cursor == 1)
        {
            wchar_t message[] = L"₪ ";
            mvprintw(11, colss, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(11, colss + 4, "Yes");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"₪ ";
            mvprintw(11, colss, "%ls Yes\n", message);
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
    attron(A_BOLD);
    mvprintw(3, cols / 2 - 10, "Normal Food and Bad food: %d", normalfood + badfood);
    mvprintw(4, cols / 2 - 10, "Good Food : %d", goodfood);
    mvprintw(5, cols / 2 - 10, "Magic Food : %d", magicfood);
    attroff(A_BOLD);
    attron(A_BLINK | A_BOLD);
    mvprintw(7, cols / 2 - 10, "Press Anything to Continue!");
    attroff(A_BLINK | A_BOLD);
    char cc = getch();
    clear();
    int c;
    int u = 0;
    int cursor = 0;
    int valid = 0;
    int colss = (getmaxx(stdscr) / 2);
    colss -= 10;
    do
    {
        valid = 0;
        clear();
        attron(A_BOLD);
        wchar_t message1[] = L"╔══════════════════════════════╗";
        mvprintw(4, colss - 10, "%ls", message1);
        wchar_t message2[] = L"║             Food             ║";
        mvprintw(5, colss - 10, "%ls", message2);
        wchar_t message3[] = L"╚══════════════════════════════╝";
        mvprintw(6, colss - 10, "%ls", message3);
        attroff(A_BOLD);
        if (cursor == 0)
        {
            wchar_t message[] = L"₪ ";
            mvprintw(9, colss, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(9, colss + 4, "Normal and Bad Food");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"₪ ";
            mvprintw(9, colss, "%ls Normal and Bad Food\n", message);
        }
        if (cursor == 1)
        {
            wchar_t message[] = L"₪ ";
            mvprintw(11, colss, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(11, colss + 4, "Good Food");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"₪ ";
            mvprintw(11, colss, "%ls Good Food", message);
        }
        if (cursor == 2)
        {
            wchar_t message[] = L"₪ ";
            mvprintw(13, colss, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(13, colss + 4, "Magic Food");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"₪ ";
            mvprintw(13, colss, "%ls Magic Food", message);
        }
        if (cursor == 3)
        {
            wchar_t message[] = L"₪ ";
            mvprintw(15, colss, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(15, colss + 4, "Back");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"₪ ";
            mvprintw(15, colss, "%ls Back", message);
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
        if (cursor == 0 && normalfood + badfood > 0 && PlayerFood != 10)
        {
            valid = 1;
        }
        if (cursor == 1 && goodfood > 0 && PlayerFood != 10)
        {
            valid = 1;
        }
        if (cursor == 2 && magicfood > 0 && PlayerFood != 10)
        {
            valid = 1;
        }
        if (cursor == 3)
        {
            valid = 1;
        }
    } while (c != 10 || valid <= 0);
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
            if (normalfood == 0)
            {
                for (int i = 0; i < FoodCount; i++)
                {
                    if (FoodArray[i].type == 3)
                    {
                        FoodArray[i].type = -1;
                        break;
                    }
                }
                if (PlayerFood - (FoodEffect / 2) < 0)
                {
                    PlayerFood = 0;
                }
                else
                {
                    PlayerFood -= (FoodEffect / 2);
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
                    if (PlayerFood - (FoodEffect / 2) < 0)
                    {
                        PlayerFood = 0;
                    }
                    else
                    {
                        PlayerFood -= (FoodEffect / 2);
                    }
                }
            }
        }
        return;
    }
    // good food
    if (cursor == 1)
    {
        power += 0.5;
        MovesTillLastFood = 0;
        for (int i = 0; i < FoodCount; i++)
        {
            if (FoodArray[i].type == 1)
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
        return;
    }
    // magic food
    if (cursor == 2)
    {
        MovesTillLastFood = 0;
        speed = 2;
        for (int i = 0; i < FoodCount; i++)
        {
            if (FoodArray[i].type == 2)
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
        return;
    }
    // none
    if (cursor == 3)
    {
        return;
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
    attron(A_BOLD);
    mvprintw(3, cols / 2 - 10, "Health : %d", health);
    mvprintw(4, cols / 2 - 10, "Speed : %d", speed1);
    mvprintw(5, cols / 2 - 10, "Damage : %d", damage);
    attroff(A_BOLD);
    attron(A_BLINK);
    mvprintw(7, cols / 2 - 10, "Press any key to continue!");
    attroff(A_BLINK);
    char cc = getch();
    clear();
    //
    int c;
    int u = 0;
    int cursor = 0;
    int valid = 0;
    int colss = (getmaxx(stdscr) / 2);
    colss -= 10;
    do
    {
        valid = 0;
        clear();
        attron(A_BOLD);
        wchar_t message1[] = L"╔══════════════════════════════╗";
        mvprintw(2, colss - 10, "%ls", message1);
        wchar_t message2[] = L"║             Spell            ║";
        mvprintw(4, colss - 10, "%ls", message2);
        wchar_t message3[] = L"╚══════════════════════════════╝";
        mvprintw(6, colss - 10, "%ls", message3);
        attroff(A_BOLD);
        if (cursor == 0)
        {
            wchar_t message[] = L"₪ ";
            mvprintw(9, colss, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(9, colss + 4, "Health");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"₪ ";
            mvprintw(9, colss, "%ls Health\n", message);
        }
        if (cursor == 1)
        {
            wchar_t message[] = L"₪ ";
            mvprintw(11, colss, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(11, colss + 4, "Speed");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"₪ ";
            mvprintw(11, colss, "%ls Speed\n", message);
        }
        if (cursor == 2)
        {
            wchar_t message[] = L"₪ ";
            mvprintw(13, colss, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(13, colss + 4, "Damage");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"₪ ";
            mvprintw(13, colss, "%ls Damage\n", message);
        }
        if (cursor == 3)
        {
            wchar_t message[] = L"₪ ";
            mvprintw(15, colss, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(15, colss + 4, "Back");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"₪ ";
            mvprintw(15, colss, "%ls Back\n", message);
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
    // Health
    if (cursor == 0)
    {
        if (PlayerFood < 8)
        {
            PlayerFood = 8;
        }
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
        for (int i = 0; i < SpellCount; i++)
        {
            if (SpellArray[i].mode == 7)
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
        for (int i = 0; i < SpellCount; i++)
        {
            if (SpellArray[i].mode == 8)
            {
                SpellArray[i].mode = -1;
                break;
            }
        }
    }
    // none
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
    //
    int c;
    int u = 0;
    int cursor = 0;
    int colss = (getmaxx(stdscr) / 2);
    colss -= 10;
    do
    {
        clear();
        /*mvprintw(0, 0, "Mace : %d CloseRange", mace);
        mvprintw(1, 0, "Dagger : %d FarRange", dagger);
        mvprintw(2, 0, "Magic Wand : %d FarRange", magicwand);
        mvprintw(3, 0, "Normal Arrow : %d FarRange", normalarrow);
        mvprintw(4, 0, "Sword : %d CloseRange", sword);*/
        attron(A_BOLD);
        wchar_t message1[] = L"╔══════════════════════════════╗";
        mvprintw(2, colss - 10, "%ls", message1);
        mvprintw(4, colss - 10, "Mace : %d CloseRange  Damage: 5", mace);
        mvprintw(8, colss - 10, "Dagger : %d FarRange  Damage: 12", dagger);
        mvprintw(6, colss - 10, "Magic Wand : %d FarRange  Damage: 15", magicwand);
        mvprintw(7, colss - 10, "Normal Arrow : %d FarRange  Damage: 5", normalarrow);
        mvprintw(5, colss - 10, "Sword : %d CloseRange  Damage: 10", sword);
        wchar_t message3[] = L"╚══════════════════════════════╝";
        mvprintw(10, colss - 10, "%ls", message3);
        attroff(A_BOLD);
        if (cursor == 0)
        {
            wchar_t message[] = L"₪ ";
            mvprintw(13, colss, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(13, colss + 4, "Back to Game");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"₪ ";
            mvprintw(13, colss, "%ls Back to Game", message);
        }
        if (cursor == 1)
        {
            wchar_t message[] = L"₪ ";
            mvprintw(15, colss, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(15, colss + 4, "Change Weapon");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"₪ ";
            mvprintw(15, colss, "%ls Change Weapon", message);
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
            // mvprintw(0, 0, "You are Currently holding %s",curw);
            attron(A_BOLD);
            wchar_t message1[] = L"╔══════════════════════════════╗";
            mvprintw(2, colss - 10, "%ls", message1);
            mvprintw(4, colss - 10, "You are Currently holding %s", curw);
            wchar_t message3[] = L"╚══════════════════════════════╝";
            mvprintw(6, colss - 10, "%ls", message3);
            attroff(A_BOLD);
            if (cursorr == 0)
            {
                wchar_t message[] = L"₪ ";
                mvprintw(9, colss, "%ls", message);
                attron(COLOR_PAIR(77) | A_BOLD);
                mvprintw(9, colss + 4, "Mace");
                attroff(COLOR_PAIR(77) | A_BOLD);
            }
            else
            {
                wchar_t message[] = L"₪ ";
                mvprintw(9, colss, "%ls Mace", message);
            }
            if (cursorr == 1)
            {
                wchar_t message[] = L"₪ ";
                mvprintw(11, colss, "%ls", message);
                attron(COLOR_PAIR(77) | A_BOLD);
                mvprintw(11, colss + 4, "Dagger");
                attroff(COLOR_PAIR(77) | A_BOLD);
            }
            else
            {
                wchar_t message[] = L"₪ ";
                mvprintw(11, colss, "%ls Dagger", message);
            }
            if (cursorr == 2)
            {
                wchar_t message[] = L"₪ ";
                mvprintw(13, colss, "%ls", message);
                attron(COLOR_PAIR(77) | A_BOLD);
                mvprintw(13, colss + 4, "Magic Wand");
                attroff(COLOR_PAIR(77) | A_BOLD);
            }
            else
            {
                wchar_t message[] = L"₪ ";
                mvprintw(13, colss, "%ls Magic Wand", message);
            }
            if (cursorr == 3)
            {
                wchar_t message[] = L"₪ ";
                mvprintw(15, colss, "%ls", message);
                attron(COLOR_PAIR(77) | A_BOLD);
                mvprintw(15, colss + 4, "Normal Arrow");
                attroff(COLOR_PAIR(77) | A_BOLD);
            }
            else
            {
                wchar_t message[] = L"₪ ";
                mvprintw(15, colss, "%ls Normal Arrow", message);
            }
            if (cursorr == 4)
            {
                wchar_t message[] = L"₪ ";
                mvprintw(17, colss, "%ls", message);
                attron(COLOR_PAIR(77) | A_BOLD);
                mvprintw(17, colss + 4, "Sword");
                attroff(COLOR_PAIR(77) | A_BOLD);
            }
            else
            {
                wchar_t message[] = L"₪ ";
                mvprintw(17, colss, "%ls Sword", message);
            }
            if (cursorr == 5)
            {
                wchar_t message[] = L"₪ ";
                mvprintw(19, colss, "%ls", message);
                attron(COLOR_PAIR(77) | A_BOLD);
                mvprintw(19, colss + 4, "Back");
                attroff(COLOR_PAIR(77) | A_BOLD);
            }
            else
            {
                wchar_t message[] = L"₪ ";
                mvprintw(19, colss, "%ls Back", message);
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
        // Back
        if (cursorr == 5)
        {
        }
    }
}

void SaveGame_StairCase()
{
    hiden_door_show = 0;
    genone = 1;
    // Generate the filename
    char filename[256];
    snprintf(filename, sizeof(filename), "%s_%d.txt", Player_UserName, level);
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        strcpy(Line1, "Error");
        return;
    }
    fprintf(file, "%d\n", y);
    fprintf(file, "%d\n", x);
    fprintf(file, "%d\n", rows);
    fprintf(file, "%d\n", cols);
    // Map
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            char gg = map[i][j];
            if ((gg >= '0' && gg <= '9') || (gg >= 'a' && gg <= 'z') || (gg >= 'A' && gg <= 'Z') || gg == '^' || gg == '<' || gg == '#' || gg == '%' || gg == '&' || gg == '$' || gg == ' ')
            {
                fprintf(file, "%c", map[i][j]);
            }
        }
    }
    fclose(file);
    // Room
    snprintf(filename, sizeof(filename), "%s_%d_Room.txt", Player_UserName, level);
    FILE *file1 = fopen(filename, "w");
    if (file1 == NULL)
    {
        strcpy(Line1, "Error");
        return;
    }
    fprintf(file1, "%d\n", Room_Placed);
    for (int i = 0; i < Room_Placed; i++)
    {
        fprintf(file1, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n", RoomArray[i].TopLeft_Y, RoomArray[i].TopLeft_x,
                RoomArray[i].Width, RoomArray[i].Height, RoomArray[i].middlex, RoomArray[i].middley,
                RoomArray[i].show, RoomArray[i].wdow, RoomArray[i].kaboos);
    }
    fclose(file1);
    // Trap
    snprintf(filename, sizeof(filename), "%s_%d_Trap.txt", Player_UserName, level);
    FILE *file2 = fopen(filename, "w");
    if (file2 == NULL)
    {
        strcpy(Line1, "Error");
        return;
    }
    fprintf(file2, "%d\n", Trap_Placed);
    for (int i = 0; i < Trap_Placed; i++)
    {
        if (TrapArray[i].show == 0 || TrapArray[i].show == 1 || TrapArray[i].show == -1)
        {
            fprintf(file2, "%d\n%d\n%d\n", TrapArray[i].show, TrapArray[i].x, TrapArray[i].y);
        }
        else
        {
            fprintf(file2, "%d\n%d\n%d\n", -1, TrapArray[i].x, TrapArray[i].y);
        }
    }
    fclose(file2);
    // Door
    snprintf(filename, sizeof(filename), "%s_%d_Door.txt", Player_UserName, level);
    FILE *file3 = fopen(filename, "w");
    if (file3 == NULL)
    {
        strcpy(Line1, "Error");
        return;
    }
    fprintf(file3, "%d\n", Door_Placed);
    for (int i = 0; i < Door_Placed; i++)
    {
        if (DoorArray[i].lock == 1)
        {
            fprintf(file3, "%d\n%d\n%d\n%d\n%d\n", DoorArray[i].lock, DoorArray[i].y,
                    DoorArray[i].x, DoorArray[i].PassWord, DoorArray[i].trysleft);
        }
        else
        {
            fprintf(file3, "%d\n%d\n%d\n%d\n%d\n", DoorArray[i].lock, DoorArray[i].y,
                    DoorArray[i].x, 0, DoorArray[i].trysleft);
        }
    }
    fclose(file3);
    // Corridor
    snprintf(filename, sizeof(filename), "%s_%d_Corridor.txt", Player_UserName, level);
    FILE *file4 = fopen(filename, "w");
    if (file4 == NULL)
    {
        strcpy(Line1, "Error");
        return;
    }
    fprintf(file4, "%d\n", Corridor_Placed);
    for (int i = 0; i < Corridor_Placed; i++)
    {
        fprintf(file4, "%d\n%d\n", CorridorArray[i].x, CorridorArray[i].y);
    }
    fclose(file4);
    // Window
    snprintf(filename, sizeof(filename), "%s_%d_Window.txt", Player_UserName, level);
    FILE *file5 = fopen(filename, "w");
    if (file5 == NULL)
    {
        strcpy(Line1, "Error");
        return;
    }
    fprintf(file5, "%d\n", Window_Placed);
    for (int i = 0; i < Window_Placed; i++)
    {
        fprintf(file5, "%d\n%d\n%d\n", WindowArray[i].y, WindowArray[i].x, WindowArray[i].dir);
    }
    fclose(file5);
    // Free
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            map[i][j] = ' ';
        }
    }
    free(RoomArray);
    free(TrapArray);
    free(CorridorArray);
    free(WindowArray);
    free(DoorArray);
    Player_Placed = false;
    firsttime = true;
    Room_Placed = 0;
    Trap_Placed = 0;
    Corridor_Placed = 0;
    Window_Placed = 0;
    Door_Placed = 0;
    level++;
    // clear();
    // refresh();
    NewGame();
}

void LoadGame_StairCase()
{
    // Free
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            map[i][j] = ' ';
        }
    }
    free(RoomArray);
    free(TrapArray);
    free(CorridorArray);
    free(WindowArray);
    free(DoorArray);
    // Generate the filename
    char filename[256];
    snprintf(filename, sizeof(filename), "%s_%d.txt", Player_UserName, level - 1);
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        strcpy(Line1, "Error");
        return;
    }
    fscanf(file, "%d", &y);
    fscanf(file, "%d", &x);
    int rowl;
    int coll;
    fscanf(file, "%d", &rowl);
    fscanf(file, "%d\n", &coll);
    // map
    /*map = (char **)malloc(2*(rows+1) * sizeof(char *));
    for (int i = 0; i < rows; i++) {
        map[i] = (char *)malloc(2*(cols+1) * sizeof(char));
    }*/
    /*for (int i = 0; i < rows; i++)
    {
        fgets(map[i], cols+5, file);
        int ynigga = strlen(map[i]);
        for(int j = 0 ; j <= ynigga;j++)
        {
            if(map[i][j] == '\n')
            {
                map[i][j] = '\0';
            }
        }
    }*/
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            char gg = fgetc(file);
            // fscanf(file, "%c", &gg);
            if (gg == '\n')
            {
                break;
                i += 1000;
            }
            else
            {
                if ((gg >= '0' && gg <= '9') || (gg >= 'a' && gg <= 'z') || (gg >= 'A' && gg <= 'Z') || gg == '^' || gg == '<' || gg == '#' || gg == '%' || gg == '&' || gg == '$' || gg == ' ')
                {
                    map[i][j] = gg;
                }
            }
        }
    }
    /*for (int i = 0; i < rows - 1; i++)
    {
        strcpy(map[i],map[i+1]);
    }*/
    /*clear();
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            addch(map[i][j]);
        }
        printw("\n");
    }
    char chec = getch();*/
    fclose(file);
    // Room
    char filename1[256];
    snprintf(filename1, sizeof(filename1), "%s_%d_Room.txt", Player_UserName, level - 1);
    FILE *file1 = fopen(filename1, "r");
    if (file1 == NULL)
    {
        strcpy(Line1, "Error");
        return;
    }
    fscanf(file1, "%d", &Room_Placed);
    RoomArray = (Room *)malloc(2 * Room_Placed * sizeof(Room));
    for (int i = 0; i < Room_Placed; i++)
    {
        fscanf(file1, "%d", &(RoomArray[i].TopLeft_Y));
        fscanf(file1, "%d", &(RoomArray[i].TopLeft_x));
        fscanf(file1, "%d", &(RoomArray[i].Width));
        fscanf(file1, "%d", &(RoomArray[i].Height));
        fscanf(file1, "%d", &(RoomArray[i].middlex));
        fscanf(file1, "%d", &(RoomArray[i].middley));
        int temp;
        fscanf(file1, "%d", &temp);
        if (temp == 0)
        {
            RoomArray[i].show = 0;
        }
        else
        {
            RoomArray[i].show = 1;
        }
        fscanf(file1, "%d", &(RoomArray[i].wdow));
    }
    fclose(file1);
    // Trap
    char filename2[256];
    snprintf(filename2, sizeof(filename2), "%s_%d_Trap.txt", Player_UserName, level - 1);
    FILE *file2 = fopen(filename2, "r");
    if (file2 == NULL)
    {
        strcpy(Line1, "Error");
        return;
    }
    fscanf(file2, "%d", &Trap_Placed);
    TrapArray = (trap *)malloc(Room_Placed * sizeof(trap));
    for (int i = 0; i < Trap_Placed; i++)
    {
        fscanf(file2, "%d", &(TrapArray[i].show));
        fscanf(file2, "%d", &(TrapArray[i].x));
        fscanf(file2, "%d", &(TrapArray[i].y));
    }
    fclose(file2);
    // Door
    char filename3[256];
    snprintf(filename3, sizeof(filename3), "%s_%d_Door.txt", Player_UserName, level - 1);
    FILE *file3 = fopen(filename3, "r");
    if (file3 == NULL)
    {
        strcpy(Line1, "Error");
        return;
    }
    fscanf(file3, "%d", &(Door_Placed));
    DoorArray = (door *)malloc(3 * Room_Placed * sizeof(door));
    for (int i = 0; i < Door_Placed; i++)
    {
        int temp;
        fscanf(file3, "%d", &temp);
        if (temp == 0)
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
    snprintf(filename4, sizeof(filename4), "%s_%d_Corridor.txt", Player_UserName, level - 1);
    FILE *file4 = fopen(filename4, "r");
    if (file4 == NULL)
    {
        strcpy(Line1, "Error");
        return;
    }
    fscanf(file4, "%d", &(Corridor_Placed));
    CorridorArray = (Corridor *)malloc(2000 * Room_Placed * sizeof(Corridor));
    for (int i = 0; i < Corridor_Placed; i++)
    {
        fscanf(file4, "%d", &(CorridorArray[i].x));
        fscanf(file4, "%d", &(CorridorArray[i].y));
    }
    fclose(file4);
    // Window
    char filename5[256];
    snprintf(filename5, sizeof(filename5), "%s_%d_Window.txt", Player_UserName, level - 1);
    FILE *file5 = fopen(filename5, "r");
    if (file5 == NULL)
    {
        strcpy(Line1, "Error");
        return;
    }
    fscanf(file5, "%d", &(Window_Placed));
    WindowArray = (Window *)malloc(2 * Room_Placed * sizeof(Window));
    for (int i = 0; i < Window_Placed; i++)
    {
        fscanf(file5, "%d", &(WindowArray[i].y));
        fscanf(file5, "%d", &(WindowArray[i].x));
        fscanf(file5, "%d", &(WindowArray[i].dir));
    }
    fclose(file5);
    clear();
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
    // char f = getch();
    Player_Placed = true;
    firsttime = false;
    level--;
    clear();
    NewGame();
}

void AddtoScoreBoard()
{
    FILE *file55 = fopen("ScoreBoard.txt", "a");
    if (file55 != NULL)
    {
        fprintf(file55, "%s\n%d\n", Player_UserName, playerGOLD);
        fclose(file55);
    }
}

void ScoreBoard()
{
    clear();
    int count2 = 0;
    char UserNames2[1000][1000];
    int Scores2[1000];
    FILE *fil = fopen("ScoreBoard.txt", "r");
    while (fgets(UserNames2[count2], 100, fil))
    {
        UserNames2[count2][strcspn(UserNames2[count2], "\n")] = '\0';
        char ScoreLine[100];
        if (fgets(ScoreLine, 100, fil))
        {
            Scores2[count2] = atoi(ScoreLine);
            count2++;
        }
    }
    fclose(fil);
    //
    int count = 0;
    char UserNames[1000][1000];
    int Scores[1000];
    for (int i = 0; i < count2; i++)
    {
        int flaq = 0;
        int placeindex = 0;
        for (int j = 0; j < count; j++)
        {
            if (strcmp(UserNames2[i], UserNames[j]) == 0)
            {
                flaq++;
                placeindex = j;
            }
        }
        if (flaq == 0)
        {
            strcpy(UserNames[count], UserNames2[i]);
            Scores[count] = Scores2[i];
            count++;
        }
        else
        {
            Scores[placeindex] += Scores2[i];
        }
    }
    //
    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < count; j++)
        {
            if (Scores[j] < Scores[i])
            {
                int temp;
                temp = Scores[i];
                Scores[i] = Scores[j];
                Scores[j] = temp;
                char Temp[100];
                strcpy(Temp, UserNames[i]);
                strcpy(UserNames[i], UserNames[j]);
                strcpy(UserNames[j], Temp);
            }
        }
    }

    curs_set(0);
    wchar_t message[] = L"╔═══════════════════════════════════════════════════╗";
    mvprintw(1, (cols / 2) - 20 - 5, "%ls", message);
    printw("\n");
    wchar_t message1[] = L"║";
    mvprintw(2, (cols / 2) - 25, "%ls", message1);
    attron(A_BOLD);
    mvprintw(2, (cols / 2) - 4, "ScoreBoard");
    attroff(A_BOLD);
    mvprintw(2, (cols / 2) + 27, "%ls", message1);
    wchar_t message2[] = L"╠═══════════════════════════════════════════════════╣";
    mvprintw(3, (cols / 2) - 25, "%ls", message2);
    refresh();
    attron(A_BOLD | COLOR_PAIR(7));
    mvprintw(7, (cols / 2) - 25, "Rank  |  UserName  |  Score  |  Games_Played  |  Time\n");
    attroff(A_BOLD | COLOR_PAIR(7));
    int rowstart = 10;
    for (int i = 0; i < count; i++)
    {
        if (strcmp(UserNames[i], Player_UserName) == 0)
        {
            attron(A_UNDERLINE);
        }
        else
        {
            if (i == 0 || i == 1 || i == 2)
            {
                attron(A_BOLD);
            }
        }
        // Times Played
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
        if (i == 0)
        {
            attron(COLOR_PAIR(4));
            wchar_t message2[] = L"🥇";
            mvprintw(rowstart + 1, (cols / 2) - 50 - 1, "%ls", message2);
            mvprintw(rowstart + 1, (cols / 2) - 60 - 1, "GOAT");
            mvprintw(++rowstart, (cols / 2) - 24 - 1, "%3d   |    %-8s|   %-6d|        %-8d|   %-4.0f  \n", i + 1, UserNames[i], Scores[i], number, difference);
            ++rowstart;
            attroff(COLOR_PAIR(4));
        }
        else
        {
            if (i == 1)
            {
                attron(COLOR_PAIR(2));
                wchar_t message2[] = L"🥈";
                mvprintw(rowstart + 1, (cols / 2) - 40 - 10 - 1, "%ls", message2);
                mvprintw(rowstart + 1, (cols / 2) - 50 - 10 - 1, "LEGEND");
                mvprintw(++rowstart, (cols / 2) - 14 - 10 - 1, "%3d   |    %-8s|   %-6d|        %-8d|   %-4.0f  \n", i + 1, UserNames[i], Scores[i], number, difference);
                ++rowstart;
                attroff(COLOR_PAIR(2));
            }
            else
            {
                if (i == 2)
                {
                    attron(COLOR_PAIR(3));
                    wchar_t message2[] = L"🥉";
                    mvprintw(rowstart + 1, (cols / 2) - 40 - 10 - 1, "%ls", message2);
                    mvprintw(rowstart + 1, (cols / 2) - 50 - 10 - 1, "GOD");
                    mvprintw(++rowstart, (cols / 2) - 14 - 10 - 1, "%3d   |    %-8s|   %-6d|        %-8d|   %-4.0f  \n", i + 1, UserNames[i], Scores[i], number, difference);
                    ++rowstart;
                    attroff(COLOR_PAIR(3));
                }
                else
                {
                    mvprintw(++rowstart, (cols / 2) - 14 - 10 - 1, "%3d   |    %-8s|   %-6d|        %-8d|   %-4.0f  \n", i + 1, UserNames[i], Scores[i], number, difference);
                    ++rowstart;
                }
            }
        }
        attroff(A_BOLD);
        attroff(A_UNDERLINE);
        if (((i + 1) % 10) == 0 || (i == (count - 1)))
        {
            noecho();
            printw("\n\n\n\n                                                               Press N for next page and P for previous page and q for quit");
            char sd = getch();
            if (sd == 'N' || sd == 'n')
            {
                if (i != (count - 1))
                {
                    rowstart = 10;
                    clear();
                    wchar_t message[] = L"╔═══════════════════════════════════════════════════╗";
                    mvprintw(1, (cols / 2) - 20 - 5, "%ls", message);
                    printw("\n");
                    wchar_t message1[] = L"║";
                    mvprintw(2, (cols / 2) - 25, "%ls", message1);
                    attron(A_BOLD);
                    mvprintw(2, (cols / 2) - 4, "ScoreBoard");
                    attroff(A_BOLD);
                    mvprintw(2, (cols / 2) + 27, "%ls", message1);
                    wchar_t message2[] = L"╠═══════════════════════════════════════════════════╣";
                    mvprintw(3, (cols / 2) - 25, "%ls", message2);
                    refresh();
                    attron(A_BOLD | COLOR_PAIR(7));
                    mvprintw(7, (cols / 2) - 25, "Rank  |  UserName  |  Score  |  Games_Played  |  Time\n");
                    attroff(A_BOLD | COLOR_PAIR(7));
                }
                else
                {
                    int j;
                    for (j = i - 1; j >= 0; j--)
                    {
                        if ((j + 1) % 10 == 0)
                        {
                            break;
                        }
                    }
                    i = j;
                    rowstart = 10;
                    clear();
                    wchar_t message[] = L"╔═══════════════════════════════════════════════════╗";
                    mvprintw(1, (cols / 2) - 20 - 5, "%ls", message);
                    printw("\n");
                    wchar_t message1[] = L"║";
                    mvprintw(2, (cols / 2) - 25, "%ls", message1);
                    attron(A_BOLD);
                    mvprintw(2, (cols / 2) - 4, "ScoreBoard");
                    attroff(A_BOLD);
                    mvprintw(2, (cols / 2) + 27, "%ls", message1);
                    wchar_t message2[] = L"╠═══════════════════════════════════════════════════╣";
                    mvprintw(3, (cols / 2) - 25, "%ls", message2);
                    refresh();
                    attron(A_BOLD | COLOR_PAIR(7));
                    mvprintw(7, (cols / 2) - 25, "Rank  |  UserName  |  Score  |  Games_Played  |  Time\n");
                    attroff(A_BOLD | COLOR_PAIR(7));
                }
            }
            if (sd == 'P' || sd == 'p')
            {
                if (i >= 19)
                {
                    i -= 20;
                    rowstart = 10;
                    clear();
                    wchar_t message[] = L"╔═══════════════════════════════════════════════════╗";
                    mvprintw(1, (cols / 2) - 20 - 5, "%ls", message);
                    printw("\n");
                    wchar_t message1[] = L"║";
                    mvprintw(2, (cols / 2) - 25, "%ls", message1);
                    attron(A_BOLD);
                    mvprintw(2, (cols / 2) - 4, "ScoreBoard");
                    attroff(A_BOLD);
                    mvprintw(2, (cols / 2) + 27, "%ls", message1);
                    wchar_t message2[] = L"╠═══════════════════════════════════════════════════╣";
                    mvprintw(3, (cols / 2) - 25, "%ls", message2);
                    refresh();
                    attron(A_BOLD | COLOR_PAIR(7));
                    mvprintw(7, (cols / 2) - 25, "Rank  |  UserName  |  Score  |  Games_Played  |  Time\n");
                    attroff(A_BOLD | COLOR_PAIR(7));
                }
                else
                {
                    i = -1;
                    rowstart = 10;
                    clear();
                    wchar_t message[] = L"╔═══════════════════════════════════════════════════╗";
                    mvprintw(1, (cols / 2) - 20 - 5, "%ls", message);
                    printw("\n");
                    wchar_t message1[] = L"║";
                    mvprintw(2, (cols / 2) - 25, "%ls", message1);
                    attron(A_BOLD);
                    mvprintw(2, (cols / 2) - 4, "ScoreBoard");
                    attroff(A_BOLD);
                    mvprintw(2, (cols / 2) + 27, "%ls", message1);
                    wchar_t message2[] = L"╠═══════════════════════════════════════════════════╣";
                    mvprintw(3, (cols / 2) - 25, "%ls", message2);
                    refresh();
                    attron(A_BOLD | COLOR_PAIR(7));
                    mvprintw(7, (cols / 2) - 25, "Rank  |  UserName  |  Score  |  Games_Played  |  Time\n");
                    attroff(A_BOLD | COLOR_PAIR(7));
                }
            }
            if (sd == 'q')
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
    int twd = 0;
    while (twd == 0)
    {
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
        int room_num = (rand() % 5) + 4;

        if (genone == 0)
        {
            WeaponArray = (Weapon *)malloc(room_num * sizeof(Weapon) * 2);
            WeaponArray[0].mode = 1;
            SpellArray = (Spell *)malloc(room_num * sizeof(Spell) * 2);
            FoodArray = (Food *)malloc(room_num * sizeof(Food) * 3);
        }
        if (lood == 0)
        {
            RoomArray = (Room *)malloc(room_num * sizeof(Room) * 2);
            RoomArray = (Room *)malloc(room_num * sizeof(Room) * 2);
            TrapArray = (trap *)malloc(room_num * sizeof(trap));
            DoorArray = (door *)malloc(room_num * sizeof(door) * 3);
            CorridorArray = (Corridor *)malloc(room_num * sizeof(Corridor) * 2000);
            WindowArray = (Window *)malloc(room_num * sizeof(Window) * 2);
        }
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
                if (i - 2 >= 0)
                {
                    corner = rand() % 4;
                    if (corner == 0)
                    {
                        map[RoomArray[i - 2].TopLeft_Y][RoomArray[i - 2].TopLeft_x] = '&';
                    }
                    if (corner == 1)
                    {
                        map[RoomArray[i - 2].TopLeft_Y][RoomArray[i - 2].TopLeft_x + RoomArray[i - 2].Width - 1] = '&';
                    }
                    if (corner == 2)
                    {
                        map[RoomArray[i - 2].TopLeft_Y + RoomArray[i - 2].Height - 1][RoomArray[i - 2].TopLeft_x] = '&';
                    }
                    if (corner == 3)
                    {
                        map[RoomArray[i - 2].TopLeft_Y + RoomArray[i - 2].Height - 1][RoomArray[i - 2].TopLeft_x + RoomArray[i - 2].Width - 1] = '&';
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
                if (i + 2 < Room_Placed)
                {
                    corner = rand() % 4;
                    if (corner == 0)
                    {
                        map[RoomArray[i + 2].TopLeft_Y][RoomArray[i + 2].TopLeft_x] = '&';
                    }
                    if (corner == 1)
                    {
                        map[RoomArray[i + 2].TopLeft_Y][RoomArray[i + 2].TopLeft_x + RoomArray[i + 2].Width - 1] = '&';
                    }
                    if (corner == 2)
                    {
                        map[RoomArray[i + 2].TopLeft_Y + RoomArray[i + 2].Height - 1][RoomArray[i + 2].TopLeft_x] = '&';
                    }
                    if (corner == 3)
                    {
                        map[RoomArray[i + 2].TopLeft_Y + RoomArray[i + 2].Height - 1][RoomArray[i + 2].TopLeft_x + RoomArray[i + 2].Width - 1] = '&';
                    }
                }
            }
        }

        // pillars and traps and gold and GOLD and weapon and Spell and food
        for (int index = 0; index < Room_Placed; index++)
        {
            // init show to false
            RoomArray[index].show = 0;
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
        // Hiden Door
        if (level != 4)
        {
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
            int y_placed;
            int x_placed;
            int vv = rand() % 6;
            // 0 top
            if (vv == 0)
            {
                int checking = 5;
                do
                {
                    checking = 0;
                    int vvv = (rand() % (RoomArray[roomin].Width / 2)) + RoomArray[roomin].Width / 4;
                    x_placed = RoomArray[roomin].TopLeft_x + vvv;
                    y_placed = RoomArray[roomin].TopLeft_Y - 1;
                    for (int j = 0; j < Door_Placed; j++)
                    {
                        if (DoorArray[j].x == RoomArray[roomin].TopLeft_x + vvv && DoorArray[j].y == RoomArray[roomin].TopLeft_Y - 1)
                        {
                            checking++;
                        }
                    }

                } while (checking > 0);
                map[y_placed][x_placed] = 'H';
            }
            // 1 right
            if (vv == 1 || vv == 4)
            {
                int checking = 5;
                do
                {
                    checking = 0;
                    int vvv = (rand() % (RoomArray[roomin].Height / 2)) + RoomArray[roomin].Height / 4;
                    x_placed = RoomArray[roomin].TopLeft_x + RoomArray[roomin].Width;
                    y_placed = RoomArray[roomin].TopLeft_Y + vvv;
                    for (int j = 0; j < Door_Placed; j++)
                    {
                        if (DoorArray[j].x == RoomArray[roomin].TopLeft_x + RoomArray[roomin].Width && DoorArray[j].y == RoomArray[roomin].TopLeft_Y + vvv)
                        {
                            checking++;
                        }
                    }
                } while (checking > 0);
                map[y_placed][x_placed] = 'H';
            }
            // 2 bottom
            if (vv == 2)
            {
                int checking = 5;
                do
                {
                    checking = 0;
                    int vvv = (rand() % (RoomArray[roomin].Width / 2)) + RoomArray[roomin].Width / 4;
                    x_placed = RoomArray[roomin].TopLeft_x + vvv;
                    y_placed = RoomArray[roomin].TopLeft_Y + RoomArray[roomin].Height;
                    for (int j = 0; j < Door_Placed; j++)
                    {
                        if (DoorArray[j].x == RoomArray[roomin].TopLeft_x + vvv && DoorArray[j].y == RoomArray[roomin].TopLeft_Y + RoomArray[roomin].Height)
                        {
                            checking++;
                        }
                    }
                } while (checking > 0);
                map[y_placed][x_placed] = 'H';
            }
            // 3 left
            if (vv == 3 || vv == 5)
            {
                int checking = 5;
                do
                {
                    checking = 0;
                    int vvv = (rand() % (RoomArray[roomin].Height / 2)) + RoomArray[roomin].Height / 4;
                    x_placed = RoomArray[roomin].TopLeft_x - 1;
                    y_placed = RoomArray[roomin].TopLeft_Y + vvv;
                    for (int j = 0; j < Door_Placed; j++)
                    {
                        if (DoorArray[j].x == RoomArray[roomin].TopLeft_x - 1 && DoorArray[j].y == RoomArray[roomin].TopLeft_Y + vvv)
                        {
                            checking++;
                        }
                    }
                } while (checking > 0);
                map[y_placed][x_placed] = 'H';
            }
        }
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
                        if (map[RoomArray[i].TopLeft_Y - 1][RoomArray[i].TopLeft_x + vvv] == 'H')
                        {
                            checking++;
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
                        if (map[RoomArray[i].TopLeft_Y + vvv][RoomArray[i].TopLeft_x + RoomArray[i].Width] == 'H')
                        {
                            checking++;
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
                        if (map[RoomArray[i].TopLeft_Y + RoomArray[i].Height][RoomArray[i].TopLeft_x + vvv] == 'H')
                        {
                            checking++;
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
                        if (map[RoomArray[i].TopLeft_Y + vvv][RoomArray[i].TopLeft_x - 1] == 'H')
                        {
                            checking++;
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
                                TrapArray[Trap_Placed].show = 0;
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
        // kaboos
        if(level != 4)
        {
        int ref = rand() % (Room_Placed);
        RoomArray[ref].kaboos = 22;
        }
        twd = 1;
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
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            map[i][j] = ' ';
        }
    }
    lood = 1;
    genone = 1;
    clear();
    // Generate the filename
    char filename[256];
    snprintf(filename, sizeof(filename), "%s_Stats.txt", Player_UserName);
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printw("Error");
        return;
    }
    fscanf(file, "%d", &y);
    fscanf(file, "%d", &x);
    int ror;
    int cor;
    fscanf(file, "%d", &ror);
    fscanf(file, "%d", &cor);
    fscanf(file, "%d", &level);
    fscanf(file, "%d", &color);
    fscanf(file, "%d", &playerGOLD);
    fscanf(file, "%d", &playerHP);
    fscanf(file, "%d", &TillLast);
    fscanf(file, "%d", &FoodEffect);
    fscanf(file, "%d", &AncientKeys);
    for (int i = 0; i < AncientKeys; i++)
    {
        fscanf(file, "%d", &AncientKey[i]);
    }
    fclose(file);
    char filename1[256];
    snprintf(filename1, sizeof(filename1), "%s_Stats1.txt", Player_UserName);
    FILE *filee = fopen(filename1, "r");
    if (filee == NULL)
    {
        printw("Error");
        return;
    }
    fscanf(filee, "%d", &UsedKey);
    fscanf(filee, "%d", &WeaponCount);
    fscanf(filee, "%d", &SpellCount);
    fscanf(filee, "%d", &FoodCount);
    fscanf(filee, "%d", &EnemyCount);
    fscanf(filee, "%d", &Wasted_Weapon_Count);
    fscanf(filee, "%d", &MovesTillLastFood);
    fscanf(filee, "%d", &m);
    fscanf(filee, "%d", &hiden_door_show);
    int loodd;
    int genonee;
    fscanf(filee, "%d", &loodd);
    fscanf(filee, "%d", &genonee);
    fscanf(filee, "%lf", &PlayerFood);
    fscanf(filee, "%d", &onetime);
    fscanf(filee, "%lf", &power);
    fscanf(filee, "%lf", &speed);
    fscanf(filee, "%d", &Health_Pot);
    fscanf(filee, "%d", &MovesTillLastpot);
    char backslashnisshit;
    fscanf(filee, "%c", &backslashnisshit);
    fscanf(filee, "%c", &Current_Weapon);
    // Current_Weapon = fgetc(filee);
    fclose(filee);
    // Map
    char filename2[256];
    snprintf(filename2, sizeof(filename2), "%s_Map.txt", Player_UserName);
    FILE *file1 = fopen(filename2, "r");
    if (file1 == NULL)
    {
        printw("Error");
        return;
    }
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            char gg = fgetc(file1);
            if (gg == '\n')
            {
                break;
                i += 1000;
            }
            else
            {
                if ((gg >= '0' && gg <= '9') || (gg >= 'a' && gg <= 'z') || (gg >= 'A' && gg <= 'Z') || gg == '^' || gg == '<' || gg == '#' || gg == '%' || gg == '&' || gg == '$' || gg == ' ')
                {
                    map[i][j] = gg;
                }
            }
        }
    }
    /*clear();
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            addch(map[i][j]);
        }
        printw("\n");
    }
    char chec = getch();*/
    fclose(file1);
    // Room
    char filename3[256];
    snprintf(filename3, sizeof(filename3), "%s_Room.txt", Player_UserName);
    FILE *file2 = fopen(filename3, "r");
    if (file2 == NULL)
    {
        printw("Error");
        return;
    }
    fscanf(file2, "%d", &Room_Placed);
    RoomArray = (Room *)malloc(2 * Room_Placed * sizeof(Room));
    for (int i = 0; i < Room_Placed; i++)
    {
        fscanf(file2, "%d", &(RoomArray[i].TopLeft_Y));
        fscanf(file2, "%d", &(RoomArray[i].TopLeft_x));
        fscanf(file2, "%d", &(RoomArray[i].Width));
        fscanf(file2, "%d", &(RoomArray[i].Height));
        fscanf(file2, "%d", &(RoomArray[i].middlex));
        fscanf(file2, "%d", &(RoomArray[i].middley));
        int temp;
        fscanf(file2, "%d", &temp);
        if (temp == 0)
        {
            RoomArray[i].show = 0;
        }
        else
        {
            RoomArray[i].show = 1;
        }
        fscanf(file2, "%d", &(RoomArray[i].wdow));
        fscanf(file2, "%d", &(RoomArray[i].kaboos));
    }
    fclose(file2);
    // Trap
    char filename4[256];
    snprintf(filename4, sizeof(filename4), "%s_Trap.txt", Player_UserName);
    FILE *file3 = fopen(filename4, "r");
    if (file3 == NULL)
    {
        printw("Error");
        return;
    }
    fscanf(file3, "%d", &Trap_Placed);
    TrapArray = (trap *)malloc(Room_Placed * sizeof(trap));
    for (int i = 0; i < Trap_Placed; i++)
    {
        fscanf(file3, "%d", &(TrapArray[i].show));
        fscanf(file3, "%d", &(TrapArray[i].x));
        fscanf(file3, "%d", &(TrapArray[i].y));
    }
    fclose(file3);
    // Door
    char filename5[256];
    snprintf(filename5, sizeof(filename5), "%s_Door.txt", Player_UserName);
    FILE *file4 = fopen(filename5, "r");
    if (file4 == NULL)
    {
        printw("Error");
        return;
    }
    fscanf(file4, "%d", &(Door_Placed));
    DoorArray = (door *)malloc(3 * Room_Placed * sizeof(door));
    for (int i = 0; i < Door_Placed; i++)
    {
        int temp;
        fscanf(file4, "%d", &temp);
        if (temp == 0)
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
    char filename6[256];
    snprintf(filename6, sizeof(filename6), "%s_Corridor.txt", Player_UserName);
    FILE *file5 = fopen(filename6, "r");
    if (file5 == NULL)
    {
        printw("Error");
        return;
    }
    fscanf(file5, "%d", &(Corridor_Placed));
    CorridorArray = (Corridor *)malloc(2000 * Room_Placed * sizeof(Corridor));
    for (int i = 0; i < Corridor_Placed; i++)
    {
        fscanf(file5, "%d", &(CorridorArray[i].x));
        fscanf(file5, "%d", &(CorridorArray[i].y));
    }
    fclose(file5);
    // Window
    char filename7[256];
    snprintf(filename7, sizeof(filename7), "%s_Window.txt", Player_UserName);
    FILE *file6 = fopen(filename7, "r");
    if (file6 == NULL)
    {
        printw("Error");
        return;
    }
    fscanf(file6, "%d", &(Window_Placed));
    WindowArray = (Window *)malloc(2 * Room_Placed * sizeof(Window));
    for (int i = 0; i < Window_Placed; i++)
    {
        fscanf(file6, "%d", &(WindowArray[i].y));
        fscanf(file6, "%d", &(WindowArray[i].x));
        fscanf(file6, "%d", &(WindowArray[i].dir));
    }
    fclose(file6);
    // Weapon
    char filename8[256];
    snprintf(filename8, sizeof(filename8), "%s_Weapon.txt", Player_UserName);
    FILE *file7 = fopen(filename8, "r");
    if (file7 == NULL)
    {
        printw("Error");
        return;
    }
    fscanf(file7, "%d", &(WeaponCount));
    WeaponArray = (Weapon *)malloc(2 * Room_Placed * sizeof(Weapon));
    for (int i = 0; i < WeaponCount; i++)
    {
        fscanf(file7, "%d", &(WeaponArray[i].mode));
        fscanf(file7, "%d", &(WeaponArray[i].times_use_left));
    }
    fclose(file7);
    // Spell
    char filename9[256];
    snprintf(filename9, sizeof(filename9), "%s_Spell.txt", Player_UserName);
    FILE *file8 = fopen(filename9, "r");
    if (file8 == NULL)
    {
        printw("Error");
        return;
    }
    fscanf(file8, "%d", &(SpellCount));
    SpellArray = (Spell *)malloc(2 * Room_Placed * sizeof(Spell));
    for (int i = 0; i < SpellCount; i++)
    {
        fscanf(file8, "%d", &(SpellArray[i].mode));
    }
    fclose(file8);
    // Food
    char filename11[256];
    snprintf(filename11, sizeof(filename11), "%s_Food.txt", Player_UserName);
    FILE *file10 = fopen(filename11, "r");
    if (file10 == NULL)
    {
        printw("Error");
        return;
    }
    fscanf(file10, "%d", &(FoodCount));
    FoodArray = (Food *)malloc(3 * Room_Placed * sizeof(Food));
    for (int i = 0; i < FoodCount; i++)
    {
        fscanf(file10, "%d", &(FoodArray[i].type));
    }
    fclose(file10);
    // Wasted
    char filename100[256];
    snprintf(filename100, sizeof(filename100), "%s_Wasted_Weapon.txt", Player_UserName);
    FILE *file111 = fopen(filename100, "r");
    if (file111 == NULL)
    {
        printw("Error");
        return;
    }
    Wasted_Weapon_Array = (Wasted_Weapon *)malloc(100 * Room_Placed * sizeof(Wasted_Weapon));
    for (int i = 0; i < Wasted_Weapon_Count; i++)
    {
        fscanf(file111, "%d", &(Wasted_Weapon_Array[i].x));
        fscanf(file111, "%d", &(Wasted_Weapon_Array[i].y));
    }
    fclose(file111);
    // Enemy
    char filename12[256];
    snprintf(filename12, sizeof(filename12), "%s_Enemy.txt", Player_UserName);
    FILE *file11 = fopen(filename12, "r");
    EnemyArray = (Enemy *)malloc(100 * Room_Placed * sizeof(Enemy));
    if (file11 == NULL)
    {
        strcpy(Line1, "Error");
        return;
    }
    for (int i = 0; i < EnemyCount; i++)
    {
        fscanf(file11, "%d", &(EnemyArray[i].x));
        fscanf(file11, "%d", &(EnemyArray[i].y));
        char ihatebackslashn;
        fscanf(file11, "%c", &(ihatebackslashn));
        fscanf(file11, "%c", &(EnemyArray[i].type));
        fscanf(file11, "%lf", &(EnemyArray[i].hp));
        fscanf(file11, "%d", &(EnemyArray[i].move));
    }
    fclose(file11);
    refresh();
    Player_Placed = true;
    firsttime = false;
    genone = 1;
    clear();
    NewGame();
}

/*
void read_PlayerStats()
{
    printf("Player_UserName: %s\n", Player_UserName); // Debugging line

    char query[256];
    snprintf(query, sizeof(query), "SELECT y, x, `rows`, `cols`, level, color, playerGOLD, playerHP, playerHPMax, TillLast, FoodEffect, AncientKeys, UsedKey, WeaponCount, SpellCount, FoodCount, EnemyCount, MovesTillLastFood, PlayerFood, m, onetime, power, speed, Health_Pot, MovesTillLastpot, Current_Weapon, Wasted_Weapon_Count FROM PlayerStats WHERE Player_UserName='%s'", Player_UserName);

    printf("Query: %s\n", query); // Print the query for debugging

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL)
    {
        fprintf(stderr, "Store result failed: %s\n", mysql_error(conn));
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    int aduibas;
    int uasdbio;
    if (row)
    {
        printf("Row fetched successfully.\n"); // Debug print

        y = atoi(row[0]);
        x = atoi(row[1]);
        aduibas = atoi(row[2]);
        uasdbio = atoi(row[3]);
        level = atoi(row[4]);
        color = atoi(row[5]);
        playerGOLD = atoi(row[6]);
        playerHP = atoi(row[7]);
        playerHPMax = atoi(row[8]);
        TillLast = atoi(row[9]);
        FoodEffect = atoi(row[10]);
        AncientKeys = atoi(row[11]);
        UsedKey = atoi(row[12]);
        WeaponCount = atoi(row[13]);
        SpellCount = atoi(row[14]);
        FoodCount = atoi(row[15]);
        EnemyCount = atoi(row[16]);
        MovesTillLastFood = atoi(row[17]);
        PlayerFood = atof(row[18]);
        m = atoi(row[19]);
        onetime = atoi(row[20]);
        power = atof(row[21]);
        speed = atof(row[22]);
        Health_Pot = atoi(row[23]);
        MovesTillLastpot = atoi(row[24]);
        Current_Weapon = row[25][0];
        Wasted_Weapon_Count = atoi(row[26]);

        // Print values for debugging
        printf("y: %d, x: %d, rows: %d, cols: %d, level: %d, color: %d, playerGOLD: %d, playerHP: %d, playerHPMax: %d, TillLast: %d, FoodEffect: %d, AncientKeys: %d, UsedKey: %d, WeaponCount: %d, SpellCount: %d, FoodCount: %d, EnemyCount: %d, MovesTillLastFood: %d, PlayerFood: %f, m: %d, onetime: %d, power: %f, speed: %f, Health_Pot: %d, MovesTillLastpot: %d, Current_Weapon: %c, Wasted_Weapon_Count: %d\n",
               y, x, rows, cols, level, color, playerGOLD, playerHP, playerHPMax, TillLast, FoodEffect, AncientKeys, UsedKey, WeaponCount, SpellCount, FoodCount, EnemyCount, MovesTillLastFood, PlayerFood, m, onetime, power, speed, Health_Pot, MovesTillLastpot, Current_Weapon, Wasted_Weapon_Count);
    }
    else
    {
        printf("No row fetched.\n"); // Debug print
    }

    mysql_free_result(result);
}

void read_AncientKeys()
{
    char query[256];
    snprintf(query, sizeof(query), "SELECT KeyID, KeyStatus FROM AncientKeys WHERE Player_UserName='%s'", Player_UserName);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }

    int i = 0;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)))
    {
        AncientKey[i] = atoi(row[1]);
        i++;
    }

    mysql_free_result(result);
}

void read_Room()
{
    char query[256];
    snprintf(query, sizeof(query), "SELECT TopLeft_x, TopLeft_Y, Width, Height, middlex, middley, `show`, wdow, kaboos FROM Room WHERE Player_UserName='%s'", Player_UserName);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }

    int i = 0;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)))
    {
        RoomArray[i].TopLeft_x = atoi(row[0]);
        RoomArray[i].TopLeft_Y = atoi(row[1]);
        RoomArray[i].Width = atoi(row[2]);
        RoomArray[i].Height = atoi(row[3]);
        RoomArray[i].middlex = atoi(row[4]);
        RoomArray[i].middley = atoi(row[5]);
        RoomArray[i].show = atoi(row[6]);
        RoomArray[i].wdow = atoi(row[7]);
        RoomArray[i].kaboos = atoi(row[8]);
        i++;
    }

    mysql_free_result(result);
}

void read_Trap()
{
    char query[256];
    snprintf(query, sizeof(query), "SELECT `show`, x, y FROM Trap WHERE Player_UserName='%s'", Player_UserName);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }

    int i = 0;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)))
    {
        TrapArray[i].show = atoi(row[0]);
        TrapArray[i].x = atoi(row[1]);
        TrapArray[i].y = atoi(row[2]);
        i++;
    }

    mysql_free_result(result);
}

void read_Door()
{
    char query[256];
    snprintf(query, sizeof(query), "SELECT `lock`, y, x, PassWord, trysleft FROM `Door` WHERE Player_UserName='%s'", Player_UserName);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }

    int i = 0;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)))
    {
        DoorArray[i].lock = atoi(row[0]);
        DoorArray[i].y = atoi(row[1]);
        DoorArray[i].x = atoi(row[2]);
        DoorArray[i].PassWord = atoi(row[3]);
        DoorArray[i].trysleft = atoi(row[4]);
        i++;
    }

    mysql_free_result(result);
}

void read_Weapon()
{
    char query[256];
    snprintf(query, sizeof(query), "SELECT mode, times_use_left FROM Weapon WHERE Player_UserName='%s'", Player_UserName);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }

    int i = 0;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)))
    {
        WeaponArray[i].mode = atoi(row[0]);
        WeaponArray[i].times_use_left = atoi(row[1]);
        i++;
    }

    mysql_free_result(result);
}

void read_Spell()
{
    char query[256];
    snprintf(query, sizeof(query), "SELECT mode FROM Spell WHERE Player_UserName='%s'", Player_UserName);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }

    int i = 0;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)))
    {
        SpellArray[i].mode = atoi(row[0]);
        i++;
    }

    mysql_free_result(result);
}

void read_Corridor()
{
    char query[256];
    snprintf(query, sizeof(query), "SELECT x, y FROM Corridor WHERE Player_UserName='%s'", Player_UserName);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }

    int i = 0;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)))
    {
        CorridorArray[i].x = atoi(row[0]);
        CorridorArray[i].y = atoi(row[1]);
        i++;
    }

    mysql_free_result(result);
}

void read_Window()
{
    char query[256];
    snprintf(query, sizeof(query), "SELECT x, y, dir FROM `Window` WHERE Player_UserName='%s'", Player_UserName);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }

    int i = 0;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)))
    {
        WindowArray[i].x = atoi(row[0]);
        WindowArray[i].y = atoi(row[1]);
        WindowArray[i].dir = atoi(row[2]);
        i++;
    }

    mysql_free_result(result);
}

void read_Food()
{
    char query[256];
    snprintf(query, sizeof(query), "SELECT type FROM Food WHERE Player_UserName='%s'", Player_UserName);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }

    int i = 0;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)))
    {
        FoodArray[i].type = atoi(row[0]);
        i++;
    }

    mysql_free_result(result);
}

void read_Enemy()
{
    char query[256];
    snprintf(query, sizeof(query), "SELECT `type`, hp, `move`, x, y FROM Enemy WHERE Player_UserName='%s'", Player_UserName);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }

    int i = 0;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)))
    {
        EnemyArray[i].type = row[0][0];
        EnemyArray[i].hp = atof(row[1]);
        EnemyArray[i].move = atoi(row[2]);
        EnemyArray[i].x = atoi(row[3]);
        EnemyArray[i].y = atoi(row[4]);
        i++;
    }

    mysql_free_result(result);
}

void read_Wasted_Weapon()
{
    char query[256];
    snprintf(query, sizeof(query), "SELECT x, y FROM Wasted_Weapon WHERE Player_UserName='%s'", Player_UserName);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }

    int i = 0;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)))
    {
        Wasted_Weapon_Array[i].x = atoi(row[0]);
        Wasted_Weapon_Array[i].y = atoi(row[1]);
        i++;
    }

    mysql_free_result(result);
}
*/

void save()
{
    // Generate the filename
    char filename[256];
    snprintf(filename, sizeof(filename), "%s_Stats.txt", Player_UserName);
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        strcpy(Line1, "Error");
        return;
    }
    fprintf(file, "%d\n", y);
    fprintf(file, "%d\n", x);
    fprintf(file, "%d\n", rows);
    fprintf(file, "%d\n", cols);
    fprintf(file, "%d\n", level);
    fprintf(file, "%d\n", color);
    fprintf(file, "%d\n", playerGOLD);
    fprintf(file, "%d\n", playerHP);
    fprintf(file, "%d\n", TillLast);
    fprintf(file, "%d\n", FoodEffect);
    fprintf(file, "%d\n", AncientKeys);
    for (int i = 0; i < AncientKeys; i++)
    {
        if (AncientKey[i] == 1 || AncientKey[i] == -1)
        {
            fprintf(file, "%d\n", AncientKey[i]);
        }
        else
        {
            fprintf(file, "%d\n", 0);
        }
    }
    fclose(file);
    snprintf(filename, sizeof(filename), "%s_Stats1.txt", Player_UserName);
    FILE *filee = fopen(filename, "w");
    if (filee == NULL)
    {
        strcpy(Line1, "Error");
        return;
    }
    fprintf(filee, "%d\n", UsedKey);
    fprintf(filee, "%d\n", WeaponCount);
    fprintf(filee, "%d\n", SpellCount);
    fprintf(filee, "%d\n", FoodCount);
    // Nenijja
    fprintf(filee, "%d\n", EnemyCount);
    fprintf(filee, "%d\n", Wasted_Weapon_Count);
    fprintf(filee, "%d\n", MovesTillLastFood);
    fprintf(filee, "%d\n", m);
    // Neninjja
    fprintf(filee, "%d\n", hiden_door_show);
    fprintf(filee, "%d\n", lood);
    fprintf(filee, "%d\n", genone);
    fprintf(filee, "%lf\n", PlayerFood);
    fprintf(filee, "%d\n", onetime);
    fprintf(filee, "%lf\n", power);
    fprintf(filee, "%lf\n", speed);
    fprintf(filee, "%d\n", Health_Pot);
    fprintf(filee, "%d\n", MovesTillLastpot);
    fprintf(filee, "%c\n", Current_Weapon);
    fclose(filee);
    snprintf(filename, sizeof(filename), "%s_Map.txt", Player_UserName);
    FILE *file1 = fopen(filename, "w");
    if (file1 == NULL)
    {
        strcpy(Line1, "Error");
        return;
    }
    // Map
    /*for (int i = 0; i < rows; i++)
    {
        fprintf(file1, "%s\n", map[i]);
    }*/
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            char gg = map[i][j];
            if ((gg >= '0' && gg <= '9') || (gg >= 'a' && gg <= 'z') || (gg >= 'A' && gg <= 'Z') || gg == '^' || gg == '<' || gg == '#' || gg == '%' || gg == '&' || gg == '$' || gg == ' ')
            {
                fprintf(file1, "%c", map[i][j]);
            }
        }
    }
    fclose(file1);
    // Room
    snprintf(filename, sizeof(filename), "%s_Room.txt", Player_UserName);
    FILE *file2 = fopen(filename, "w");
    if (file2 == NULL)
    {
        strcpy(Line1, "Error");
        return;
    }
    fprintf(file2, "%d\n", Room_Placed);
    for (int i = 0; i < Room_Placed; i++)
    {
        fprintf(file2, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n", RoomArray[i].TopLeft_Y, RoomArray[i].TopLeft_x,
                RoomArray[i].Width, RoomArray[i].Height, RoomArray[i].middlex, RoomArray[i].middley,
                RoomArray[i].show, RoomArray[i].wdow, RoomArray[i].kaboos);
    }
    fclose(file2);
    // Trap
    snprintf(filename, sizeof(filename), "%s_Trap.txt", Player_UserName);
    FILE *file3 = fopen(filename, "w");
    if (file3 == NULL)
    {
        strcpy(Line1, "Error");
        return;
    }
    fprintf(file3, "%d\n", Trap_Placed);
    for (int i = 0; i < Trap_Placed; i++)
    {
        if (TrapArray[i].show == 0 || TrapArray[i].show == 1 || TrapArray[i].show == -1)
        {
            fprintf(file3, "%d\n%d\n%d\n", TrapArray[i].show, TrapArray[i].x, TrapArray[i].y);
        }
        else
        {
            fprintf(file3, "%d\n%d\n%d\n", -1, TrapArray[i].x, TrapArray[i].y);
        }
    }
    fclose(file3);
    // Door
    snprintf(filename, sizeof(filename), "%s_Door.txt", Player_UserName);
    FILE *file4 = fopen(filename, "w");
    if (file4 == NULL)
    {
        strcpy(Line1, "Error");
        return;
    }
    fprintf(file4, "%d\n", Door_Placed);
    for (int i = 0; i < Door_Placed; i++)
    {
        if (DoorArray[i].lock == 1)
        {
            fprintf(file4, "%d\n%d\n%d\n%d\n%d\n", DoorArray[i].lock, DoorArray[i].y,
                    DoorArray[i].x, DoorArray[i].PassWord, DoorArray[i].trysleft);
        }
        else
        {
            fprintf(file4, "%d\n%d\n%d\n%d\n%d\n", DoorArray[i].lock, DoorArray[i].y,
                    DoorArray[i].x, 0, DoorArray[i].trysleft);
        }
    }
    fclose(file4);
    // Corridor
    snprintf(filename, sizeof(filename), "%s_Corridor.txt", Player_UserName);
    FILE *file5 = fopen(filename, "w");
    if (file5 == NULL)
    {
        strcpy(Line1, "Error");
        return;
    }
    fprintf(file5, "%d\n", Corridor_Placed);
    for (int i = 0; i < Corridor_Placed; i++)
    {
        fprintf(file5, "%d\n%d\n", CorridorArray[i].x, CorridorArray[i].y);
    }
    fclose(file5);
    // Window
    snprintf(filename, sizeof(filename), "%s_Window.txt", Player_UserName);
    FILE *file6 = fopen(filename, "w");
    if (file6 == NULL)
    {
        strcpy(Line1, "Error");
        return;
    }
    fprintf(file6, "%d\n", Window_Placed);
    for (int i = 0; i < Window_Placed; i++)
    {
        fprintf(file6, "%d\n%d\n%d\n", WindowArray[i].y, WindowArray[i].x, WindowArray[i].dir);
    }
    fclose(file6);
    // Weapon
    // Neninjja
    snprintf(filename, sizeof(filename), "%s_Weapon.txt", Player_UserName);
    FILE *file7 = fopen(filename, "w");
    if (file7 == NULL)
    {
        strcpy(Line1, "Error");
        return;
    }
    fprintf(file7, "%d\n", WeaponCount);
    for (int i = 0; i < WeaponCount; i++)
    {
        fprintf(file7, "%d\n%d\n", WeaponArray[i].mode, WeaponArray[i].times_use_left);
    }
    fclose(file7);
    // Spell
    snprintf(filename, sizeof(filename), "%s_Spell.txt", Player_UserName);
    FILE *file8 = fopen(filename, "w");
    if (file8 == NULL)
    {
        strcpy(Line1, "Error");
        return;
    }
    fprintf(file8, "%d\n", SpellCount);
    for (int i = 0; i < SpellCount; i++)
    {
        fprintf(file8, "%d\n", SpellArray[i].mode);
    }
    fclose(file8);
    // Food
    snprintf(filename, sizeof(filename), "%s_Food.txt", Player_UserName);
    FILE *file9 = fopen(filename, "w");
    if (file9 == NULL)
    {
        strcpy(Line1, "Error");
        return;
    }
    fprintf(file9, "%d\n", FoodCount);
    for (int i = 0; i < FoodCount; i++)
    {
        fprintf(file9, "%d\n", FoodArray[i].type);
    }
    fclose(file9);
    // Wasted_Weapon
    snprintf(filename, sizeof(filename), "%s_Wasted_Weapon.txt", Player_UserName);
    FILE *file100 = fopen(filename, "w");
    if (file100 == NULL)
    {
        strcpy(Line1, "Error");
        return;
    }
    for (int i = 0; i < Wasted_Weapon_Count; i++)
    {
        fprintf(file100, "%d\n%d\n", Wasted_Weapon_Array[i].x, Wasted_Weapon_Array[i].y);
    }
    fclose(file100);
    // Enemy
    snprintf(filename, sizeof(filename), "%s_Enemy.txt", Player_UserName);
    FILE *file11 = fopen(filename, "w");
    if (file11 == NULL)
    {
        strcpy(Line1, "Error");
        return;
    }
    for (int i = 0; i < EnemyCount; i++)
    {
        fprintf(file11, "%d\n%d\n%c\n%lf\n%d\n", EnemyArray[i].x, EnemyArray[i].y, EnemyArray[i].type, EnemyArray[i].hp, EnemyArray[i].move);
    }
    fclose(file11);
    // Free
    /*free(RoomArray);
    free(TrapArray);
    free(CorridorArray);
    free(WindowArray);
    free(DoorArray);
    free(WeaponArray);
    free(SpellArray);
    free(FoodArray);
    free(Wasted_Weapon_Array);
    free(EnemyArray);
    endwin();
    exit(0);*/
    save2();
}

void Enemy_gen()
{
    if (lood != 1)
    {
        Wasted_Weapon_Array = (Wasted_Weapon *)malloc(100 * Room_Placed * sizeof(Wasted_Weapon));
        EnemyArray = (Enemy *)malloc(sizeof(Enemy) * Room_Placed * 100);
        for (int i = 0; i < Room_Placed; i++)
        {
            int xe;
            int ye;
            do
            {
                xe = (rand() % RoomArray[i].Width) + RoomArray[i].TopLeft_x;
                ye = (rand() % RoomArray[i].Height) + RoomArray[i].TopLeft_Y;
            } while ((map[ye][xe] != ' '));
            int whih = rand() % 5;
            if (whih == 0)
            {
                EnemyArray[EnemyCount].hp = 5;
                EnemyArray[EnemyCount].move = 0;
                EnemyArray[EnemyCount].type = 'd';
                map[ye][xe] = 'd';
                EnemyArray[EnemyCount].x = xe;
                EnemyArray[EnemyCount].y = ye;
            }
            if (whih == 1)
            {
                EnemyArray[EnemyCount].hp = 10;
                EnemyArray[EnemyCount].move = 0;
                EnemyArray[EnemyCount].type = 'f';
                map[ye][xe] = 'f';
                EnemyArray[EnemyCount].x = xe;
                EnemyArray[EnemyCount].y = ye;
            }
            if (whih == 2)
            {
                EnemyArray[EnemyCount].hp = 15;
                EnemyArray[EnemyCount].move = 0;
                EnemyArray[EnemyCount].type = 't';
                map[ye][xe] = 't';
                EnemyArray[EnemyCount].x = xe;
                EnemyArray[EnemyCount].y = ye;
            }
            if (whih == 3)
            {
                EnemyArray[EnemyCount].hp = 20;
                EnemyArray[EnemyCount].move = 0;
                EnemyArray[EnemyCount].type = 's';
                map[ye][xe] = 's';
                EnemyArray[EnemyCount].x = xe;
                EnemyArray[EnemyCount].y = ye;
            }
            if (whih == 4)
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
        if (level == 4)
        {
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
                        if (fg == 2)
                        {
                            int whih = rand() % 7;
                            if (whih == 0)
                            {
                                EnemyArray[EnemyCount].hp = 5;
                                EnemyArray[EnemyCount].move = 0;
                                EnemyArray[EnemyCount].type = 'd';
                                map[j][i] = 'd';
                                EnemyArray[EnemyCount].x = i;
                                EnemyArray[EnemyCount].y = j;
                            }
                            if (whih == 1)
                            {
                                EnemyArray[EnemyCount].hp = 10;
                                EnemyArray[EnemyCount].move = 0;
                                EnemyArray[EnemyCount].type = 'f';
                                map[j][i] = 'f';
                                EnemyArray[EnemyCount].x = i;
                                EnemyArray[EnemyCount].y = j;
                            }
                            if (whih == 2)
                            {
                                EnemyArray[EnemyCount].hp = 15;
                                EnemyArray[EnemyCount].move = 0;
                                EnemyArray[EnemyCount].type = 't';
                                map[j][i] = 't';
                                EnemyArray[EnemyCount].x = i;
                                EnemyArray[EnemyCount].y = j;
                            }
                            if (whih == 3)
                            {
                                EnemyArray[EnemyCount].hp = 20;
                                EnemyArray[EnemyCount].move = 0;
                                EnemyArray[EnemyCount].type = 's';
                                map[j][i] = 's';
                                EnemyArray[EnemyCount].x = i;
                                EnemyArray[EnemyCount].y = j;
                            }
                            if (whih == 4)
                            {
                                EnemyArray[EnemyCount].hp = 30;
                                EnemyArray[EnemyCount].move = 0;
                                EnemyArray[EnemyCount].type = 'u';
                                map[j][i] = 'u';
                                EnemyArray[EnemyCount].x = i;
                                EnemyArray[EnemyCount].y = j;
                            }
                            EnemyCount++;
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

void Enemy_Move2(int playery, int playerx)
{
    for (int i = 0; i < Enemycount2; i++)
    {
        if (EnemyArray2[i].move == 1 && EnemyArray2[i].hp > 0)
        {
            //(map3[EnemyArray2[i].y][EnemyArray2[i].x] == 'd' || map3[EnemyArray2[i].y][EnemyArray2[i].x] == 'f' ||
            // map3[EnemyArray2[i].y][EnemyArray2[i].x] == 't' || map3[EnemyArray2[i].y][EnemyArray2[i].x] == 's' ||
            // map3[EnemyArray2[i].y][EnemyArray2[i].x] == 'u')
            if (map3[EnemyArray2[i].y][EnemyArray2[i].x] == 'd' || map3[EnemyArray2[i].y][EnemyArray2[i].x] == 'f' ||
                map3[EnemyArray2[i].y][EnemyArray2[i].x] == 't' || map3[EnemyArray2[i].y][EnemyArray2[i].x] == 's' ||
                map3[EnemyArray2[i].y][EnemyArray2[i].x] == 'u')
            {
                map3[EnemyArray2[i].y][EnemyArray2[i].x] = ' ';
            }
            double distance_up = pow(EnemyArray2[i].x - playerx, 2) + pow(EnemyArray2[i].y - 1 - playery, 2);
            double distance_down = pow(EnemyArray2[i].x - playerx, 2) + pow(EnemyArray2[i].y + 1 - playery, 2);
            double distance_right = pow(EnemyArray2[i].x + 1 - playerx, 2) + pow(EnemyArray2[i].y - playery, 2);
            double distance_left = pow(EnemyArray2[i].x - 1 - playerx, 2) + pow(EnemyArray2[i].y - playery, 2);
            double distance_up_right = pow(EnemyArray2[i].x + 1 - playerx, 2) + pow(EnemyArray2[i].y - 1 - playery, 2);
            double distance_up_left = pow(EnemyArray2[i].x - 1 - playerx, 2) + pow(EnemyArray2[i].y - 1 - playery, 2);
            double distance_down_left = pow(EnemyArray2[i].x - 1 - playerx, 2) + pow(EnemyArray2[i].y + 1 - playery, 2);
            double distance_down_right = pow(EnemyArray2[i].x + 1 - playerx, 2) + pow(EnemyArray2[i].y + 1 - playery, 2);
            // up
            if (map3[EnemyArray2[i].y - 1][EnemyArray2[i].x] != '#' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x] != '%' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x] != 'O' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x] != 'd' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x] != 'f' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x] != 't' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x] != 's' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x] != 'u')
            {
            }
            else
            {
                distance_up += 1000;
            }
            // down
            if (map3[EnemyArray2[i].y + 1][EnemyArray2[i].x] != '#' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x] != '%' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x] != 'O' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x] != 'd' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x] != 'f' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x] != 't' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x] != 's' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x] != 'u')
            {
            }
            else
            {
                distance_down += 1000;
            }
            // right
            if (map3[EnemyArray2[i].y][EnemyArray2[i].x + 1] != '#' && map3[EnemyArray2[i].y][EnemyArray2[i].x + 1] != '%' && map3[EnemyArray2[i].y][EnemyArray2[i].x + 1] != 'O' && map3[EnemyArray2[i].y][EnemyArray2[i].x + 1] != 'd' && map3[EnemyArray2[i].y][EnemyArray2[i].x + 1] != 'f' && map3[EnemyArray2[i].y][EnemyArray2[i].x + 1] != 't' && map3[EnemyArray2[i].y][EnemyArray2[i].x + 1] != 's' && map3[EnemyArray2[i].y][EnemyArray2[i].x + 1] != 'u')
            {
            }
            else
            {
                distance_right += 1000;
            }
            // left
            if (map3[EnemyArray2[i].y][EnemyArray2[i].x - 1] != '#' && map3[EnemyArray2[i].y][EnemyArray2[i].x - 1] != '%' && map3[EnemyArray2[i].y][EnemyArray2[i].x - 1] != 'O' && map3[EnemyArray2[i].y][EnemyArray2[i].x - 1] != 'd' && map3[EnemyArray2[i].y][EnemyArray2[i].x - 1] != 'f' && map3[EnemyArray2[i].y][EnemyArray2[i].x - 1] != 't' && map3[EnemyArray2[i].y][EnemyArray2[i].x - 1] != 's' && map3[EnemyArray2[i].y][EnemyArray2[i].x - 1] != 'u')
            {
            }
            else
            {
                distance_left += 1000;
            }
            // up right
            if (map3[EnemyArray2[i].y - 1][EnemyArray2[i].x + 1] != '#' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x + 1] != '%' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x + 1] != 'O' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x + 1] != 'd' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x + 1] != 'f' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x + 1] != 't' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x + 1] != 's' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x + 1] != 'u')
            {
            }
            else
            {
                distance_up_right += 1000;
            }
            // up left
            if (map3[EnemyArray2[i].y - 1][EnemyArray2[i].x - 1] != '#' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x - 1] != '%' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x - 1] != 'O' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x - 1] != 'd' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x - 1] != 'f' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x - 1] != 't' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x - 1] != 's' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x - 1] != 'u')
            {
            }
            else
            {
                distance_up_left += 1000;
            }
            // down left
            if (map3[EnemyArray2[i].y + 1][EnemyArray2[i].x - 1] != '#' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x - 1] != '%' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x - 1] != 'O' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x - 1] != 'd' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x - 1] != 'f' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x - 1] != 't' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x - 1] != 's' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x - 1] != 'u')
            {
            }
            else
            {
                distance_down_left += 1000;
            }
            // down right
            if (map3[EnemyArray2[i].y + 1][EnemyArray2[i].x + 1] != '#' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x + 1] != '%' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x + 1] != 'O' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x + 1] != 'd' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x + 1] != 'f' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x + 1] != 't' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x + 1] != 's' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x + 1] != 'u')
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
                map3[EnemyArray2[i].y][EnemyArray2[i].x + 1] != '#' && map3[EnemyArray2[i].y][EnemyArray2[i].x + 1] != '%' && map3[EnemyArray2[i].y][EnemyArray2[i].x + 1] != 'O' && map3[EnemyArray2[i].y][EnemyArray2[i].x + 1] != 'd' && map3[EnemyArray2[i].y][EnemyArray2[i].x + 1] != 'f' && map3[EnemyArray2[i].y][EnemyArray2[i].x + 1] != 't' && map3[EnemyArray2[i].y][EnemyArray2[i].x + 1] != 's' && map3[EnemyArray2[i].y][EnemyArray2[i].x + 1] != 'u')
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
                    if (map3[EnemyArray2[i].y][EnemyArray2[i].x] == ' ')
                    {
                        map3[EnemyArray2[i].y][EnemyArray2[i].x] = EnemyArray2[i].type;
                    }
                }
                else
                {
                    EnemyArray2[i].x = EnemyArray2[i].x + 1;
                    if (map3[EnemyArray2[i].y][EnemyArray2[i].x] == ' ')
                    {
                        map3[EnemyArray2[i].y][EnemyArray2[i].x] = EnemyArray2[i].type;
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
                    map3[EnemyArray2[i].y][EnemyArray2[i].x - 1] != '#' && map3[EnemyArray2[i].y][EnemyArray2[i].x - 1] != '%' && map3[EnemyArray2[i].y][EnemyArray2[i].x - 1] != 'O' && map3[EnemyArray2[i].y][EnemyArray2[i].x - 1] != 'd' && map3[EnemyArray2[i].y][EnemyArray2[i].x - 1] != 'f' && map3[EnemyArray2[i].y][EnemyArray2[i].x - 1] != 't' && map3[EnemyArray2[i].y][EnemyArray2[i].x - 1] != 's' && map3[EnemyArray2[i].y][EnemyArray2[i].x - 1] != 'u')
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
                        if (map3[EnemyArray2[i].y][EnemyArray2[i].x] == ' ')
                        {
                            map3[EnemyArray2[i].y][EnemyArray2[i].x] = EnemyArray2[i].type;
                        }
                    }
                    else
                    {
                        EnemyArray2[i].x = EnemyArray2[i].x - 1;
                        if (map3[EnemyArray2[i].y][EnemyArray2[i].x] == ' ')
                        {
                            map3[EnemyArray2[i].y][EnemyArray2[i].x] = EnemyArray2[i].type;
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
                        map3[EnemyArray2[i].y - 1][EnemyArray2[i].x] != '#' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x] != '%' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x] != 'O' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x] != 'd' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x] != 'f' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x] != 't' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x] != 's' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x] != 'u')
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
                            if (map3[EnemyArray2[i].y][EnemyArray2[i].x] == ' ')
                            {
                                map3[EnemyArray2[i].y][EnemyArray2[i].x] = EnemyArray2[i].type;
                            }
                        }
                        else
                        {
                            EnemyArray2[i].y = EnemyArray2[i].y - 1;
                            if (map3[EnemyArray2[i].y][EnemyArray2[i].x] == ' ')
                            {
                                map3[EnemyArray2[i].y][EnemyArray2[i].x] = EnemyArray2[i].type;
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
                            map3[EnemyArray2[i].y + 1][EnemyArray2[i].x] != '#' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x] != '%' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x] != 'O' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x] != 'd' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x] != 'f' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x] != 't' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x] != 's' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x] != 'u')
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
                                if (map3[EnemyArray2[i].y][EnemyArray2[i].x] == ' ')
                                {
                                    map3[EnemyArray2[i].y][EnemyArray2[i].x] = EnemyArray2[i].type;
                                }
                            }
                            else
                            {
                                EnemyArray2[i].y = EnemyArray2[i].y + 1;
                                if (map3[EnemyArray2[i].y][EnemyArray2[i].x] == ' ')
                                {
                                    map3[EnemyArray2[i].y][EnemyArray2[i].x] = EnemyArray2[i].type;
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
                                map3[EnemyArray2[i].y - 1][EnemyArray2[i].x + 1] != '#' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x + 1] != '%' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x + 1] != 'O' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x + 1] != 'd' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x + 1] != 'f' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x + 1] != 't' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x + 1] != 's' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x + 1] != 'u')
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
                                    if (map3[EnemyArray2[i].y][EnemyArray2[i].x] == ' ')
                                    {
                                        map3[EnemyArray2[i].y][EnemyArray2[i].x] = EnemyArray2[i].type;
                                    }
                                }
                                else
                                {
                                    EnemyArray2[i].x = EnemyArray2[i].x + 1;
                                    EnemyArray2[i].y = EnemyArray2[i].y - 1;
                                    if (map3[EnemyArray2[i].y][EnemyArray2[i].x] == ' ')
                                    {
                                        map3[EnemyArray2[i].y][EnemyArray2[i].x] = EnemyArray2[i].type;
                                    }
                                }
                            }
                            else
                            {
                                // up left
                                if (distance_up_left <= distance_up_left &&
                                    distance_up_left <= distance_down_left &&
                                    distance_up_left <= distance_down_right &&
                                    map3[EnemyArray2[i].y - 1][EnemyArray2[i].x - 1] != '#' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x - 1] != '%' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x - 1] != 'O' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x - 1] != 'd' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x - 1] != 'f' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x - 1] != 't' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x - 1] != 's' && map3[EnemyArray2[i].y - 1][EnemyArray2[i].x - 1] != 'u')
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
                                        if (map3[EnemyArray2[i].y][EnemyArray2[i].x] == ' ')
                                        {
                                            map3[EnemyArray2[i].y][EnemyArray2[i].x] = EnemyArray2[i].type;
                                        }
                                    }
                                    else
                                    {
                                        EnemyArray2[i].x--;
                                        EnemyArray2[i].y--;
                                        if (map3[EnemyArray2[i].y][EnemyArray2[i].x] == ' ')
                                        {
                                            map3[EnemyArray2[i].y][EnemyArray2[i].x] = EnemyArray2[i].type;
                                        }
                                    }
                                }
                                else
                                {
                                    // down left
                                    if (distance_down_left <= distance_down_left &&
                                        distance_down_left <= distance_down_right &&
                                        map3[EnemyArray2[i].y + 1][EnemyArray2[i].x - 1] != '#' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x - 1] != '%' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x - 1] != 'O' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x - 1] != 'd' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x - 1] != 'f' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x - 1] != 't' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x - 1] != 's' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x - 1] != 'u')
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
                                            if (map3[EnemyArray2[i].y][EnemyArray2[i].x] == ' ')
                                            {
                                                map3[EnemyArray2[i].y][EnemyArray2[i].x] = EnemyArray2[i].type;
                                            }
                                        }
                                        else
                                        {
                                            EnemyArray2[i].x--;
                                            EnemyArray2[i].y++;
                                            if (map3[EnemyArray2[i].y][EnemyArray2[i].x] == ' ')
                                            {
                                                map3[EnemyArray2[i].y][EnemyArray2[i].x] = EnemyArray2[i].type;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        // down right
                                        if (distance_down_right <= distance_down_right &&
                                            map3[EnemyArray2[i].y + 1][EnemyArray2[i].x + 1] != '#' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x + 1] != '%' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x + 1] != 'O' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x + 1] != 'd' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x + 1] != 'f' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x + 1] != 't' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x + 1] != 's' && map3[EnemyArray2[i].y + 1][EnemyArray2[i].x + 1] != 'u')
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
                                                if (map3[EnemyArray2[i].y][EnemyArray2[i].x] == ' ')
                                                {
                                                    map3[EnemyArray2[i].y][EnemyArray2[i].x] = EnemyArray2[i].type;
                                                }
                                            }
                                            else
                                            {
                                                EnemyArray2[i].y++;
                                                EnemyArray2[i].x++;
                                                if (map3[EnemyArray2[i].y][EnemyArray2[i].x] == ' ')
                                                {
                                                    map3[EnemyArray2[i].y][EnemyArray2[i].x] = EnemyArray2[i].type;
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
    printw("  Y   Y  OOO  U   U     L      OOO  SSSS  TTTTT\n");
    printw("   Y Y  O   O U   U     L     O   O S       T\n");
    printw("    Y   O   O U   U     L     O   O  SSS    T\n");
    printw("    Y   O   O U   U     L     O   O     S   T\n");
    printw("    Y    OOO   UUU      LLLLL  OOO  SSSS    T\n");
    char cc = getch();
    endwin();
    exit(0);
}

void Won()
{
    clear();
    printw("  Y   Y  OOO  U   U     W   W   OOO   N   N\n");
    printw("   Y Y  O   O U   U     W   W  O   O  NN  N\n");
    printw("    Y   O   O U   U     W W W  O   O  N N N\n");
    printw("    Y   O   O U   U     WW WW  O   O  N  NN\n");
    printw("    Y    OOO   UUU      W   W   OOO   N   N\n");

    printw("points: %d", playerGOLD);
    char cc = getch();
    endwin();
    exit(0);
}

void battle(int yy, int xx)
{
    strcpy(Line1, "Fight!                                                                   ");
    double attack;
    if (Current_Weapon == 'm')
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
    if (Current_Weapon == 'S')
    {
        /*for(int i = 0 ; i < WeaponCount;i++)
        {
            if(WeaponArray[i].mode == 5)
            {

            }
        }*/
        attack = 10;
    }
    if (Current_Weapon == 'm' || Current_Weapon == 'S')
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

void battle2(int yy, int xx)
{
    strcpy(Line1, "Fight!                                                                   ");
    double attack;
    if (Current_Weapon == 'm')
    {
        attack = 5;
    }
    if (Current_Weapon == 'S')
    {
        attack = 10;
    }
    if (Current_Weapon == 'm' || Current_Weapon == 'S')
    {
        attack *= power;
        for (int i = 0; i < Enemycount2; i++)
        {
            if (EnemyArray2[i].x == xx && EnemyArray2[i].y == yy && EnemyArray2[i].hp > 0)
            {
                if (EnemyArray2[i].hp >= attack)
                {
                    EnemyArray2[i].hp -= attack;
                    if (EnemyArray2[i].hp == 0)
                    {
                        strcpy(Line1, "Enemy Killed!");
                        map3[yy][xx] = ' ';
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
                    EnemyArray2[i].hp = 0;
                    map3[yy][xx] = ' ';
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

int Check_Enemy(int yy, int xx)
{
    for (int i = 0; i < EnemyCount; i++)
    {
        if (EnemyArray[i].x == xx && EnemyArray[i].y == yy && EnemyArray[i].hp > 0)
        {
            return 0;
        }
    }
    return 1;
}

int Check_Enemy2(int yy, int xx)
{
    for (int i = 0; i < Enemycount2; i++)
    {
        if (EnemyArray2[i].x == xx && EnemyArray2[i].y == yy && EnemyArray2[i].hp > 0)
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
        for (int i = 0; i < WeaponCount; i++)
        {
            if (WeaponArray[i].mode == 3 && WeaponArray[i].times_use_left >= 1)
            {
                WeaponArray[i].times_use_left--;
                have = 1;
                break;
            }
        }
        if (have == 0)
        {
            strcpy(Line1, "You don't have a Weapon!                     ");
            return;
        }
        int cc = getch();
        if (cc == KEY_UP)
        {
            for (int i = y - 1; i >= y - 5; i--)
            {
                // if hit enemy
                if (Check_Enemy(i, x) == 0)
                {
                    strcpy(Line1, "You hit Enemy!                                                        ");
                    int index = -1;
                    for (int j = 0; j < EnemyCount; j++)
                    {
                        if (EnemyArray[j].x == x && EnemyArray[j].y == i && EnemyArray[j].hp > 0)
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
                            for (int k = 0; k < Corridor_Placed; k++)
                            {
                                if (i == CorridorArray[k].y && x == CorridorArray[k].x)
                                {
                                    which = 1;
                                    break;
                                }
                            }
                            if (which == 0)
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
                // if hit wall
                if (map[i][x] == '#' || map[i][x] == 'O' || map[i][x] == '%')
                {
                    if (map[i + 1][x] != '<')
                    {
                        Wasted_Weapon_Array[Wasted_Weapon_Count].x = x;
                        Wasted_Weapon_Array[Wasted_Weapon_Count].y = i + 1;
                        Wasted_Weapon_Count++;
                        map[i + 1][x] = '3';
                        break;
                    }
                }
                // if range is over
                if (i == y - 5)
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = x;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = i;
                    Wasted_Weapon_Count++;
                    if (map[i][x] != '<')
                    {
                        map[i][x] = '3';
                        break;
                    }
                }
                wchar_t message[] = L"⚕";
                mvprintw(i, x, "%ls", message);
                refresh();
                usleep(500000);
            }
        }
        if (cc == KEY_DOWN)
        {
            for (int i = y + 1; i <= y + 5; i++)
            {
                // if hit enemy
                if (Check_Enemy(i, x) == 0)
                {
                    strcpy(Line1, "You hit Enemy!                                                        ");
                    int index = -1;
                    for (int j = 0; j < EnemyCount; j++)
                    {
                        if (EnemyArray[j].x == x && EnemyArray[j].y == i && EnemyArray[j].hp > 0)
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
                            for (int k = 0; k < Corridor_Placed; k++)
                            {
                                if (i == CorridorArray[k].y && x == CorridorArray[k].x)
                                {
                                    which = 1;
                                    break;
                                }
                            }
                            if (which == 0)
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
                // if hit wall
                if (map[i][x] == '#' || map[i][x] == 'O' || map[i][x] == '%')
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = x;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = i - 1;
                    Wasted_Weapon_Count++;
                    if (map[i - 1][x] != '<')
                    {
                        map[i - 1][x] = '3';
                        break;
                    }
                }
                // if range is over
                if (i == y + 5)
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = x;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = i;
                    Wasted_Weapon_Count++;
                    if (map[i][x] != '<')
                    {
                        map[i][x] = '3';
                        break;
                    }
                }
                wchar_t message[] = L"⚕";
                mvprintw(i, x, "%ls", message);
                refresh();
                usleep(500000);
            }
        }
        if (cc == KEY_RIGHT)
        {
            for (int i = x + 1; i <= x + 5; i++)
            {
                // if hit enemy
                if (Check_Enemy(y, i) == 0)
                {
                    strcpy(Line1, "You hit Enemy!                                                        ");
                    int index = -1;
                    for (int j = 0; j < EnemyCount; j++)
                    {
                        if (EnemyArray[j].x == i && EnemyArray[j].y == y && EnemyArray[j].hp > 0)
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
                            for (int k = 0; k < Corridor_Placed; k++)
                            {
                                if (y == CorridorArray[k].y && i == CorridorArray[k].x)
                                {
                                    which = 1;
                                    break;
                                }
                            }
                            if (which == 0)
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
                // if hit wall
                if (map[y][i] == '#' || map[y][i] == 'O' || map[y][i] == '%')
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = i - 1;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = y;
                    Wasted_Weapon_Count++;
                    if (map[y][i - 1] != '<')
                    {
                        map[y][i - 1] = '3';
                        break;
                    }
                }
                // if range is over
                if (i == x + 5)
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = i;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = y;
                    Wasted_Weapon_Count++;
                    if (map[y][i] != '<')
                    {
                        map[y][i] = '3';
                        break;
                    }
                }
                wchar_t message[] = L"⚕";
                mvprintw(y, i, "%ls", message);
                refresh();
                usleep(500000);
            }
        }
        if (cc == KEY_LEFT)
        {
            for (int i = x - 1; i >= x - 5; i--)
            {
                // if hit enemy
                if (Check_Enemy(y, i) == 0)
                {
                    strcpy(Line1, "You hit Enemy!                                                        ");
                    int index = -1;
                    for (int j = 0; j < EnemyCount; j++)
                    {
                        if (EnemyArray[j].x == i && EnemyArray[j].y == y && EnemyArray[j].hp > 0)
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
                            for (int k = 0; k < Corridor_Placed; k++)
                            {
                                if (y == CorridorArray[k].y && i == CorridorArray[k].x)
                                {
                                    which = 1;
                                    break;
                                }
                            }
                            if (which == 0)
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
                // if hit wall
                if (map[y][i] == '#' || map[y][i] == 'O' || map[y][i] == '%')
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = i + 1;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = y;
                    Wasted_Weapon_Count++;
                    if (map[y][i + 1] != '<')
                    {
                        map[y][i + 1] = '3';
                        break;
                    }
                }
                // if range is over
                if (i == x - 5)
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = i;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = y;
                    Wasted_Weapon_Count++;
                    if (map[y][i] != '<')
                    {
                        map[y][i] = '3';
                        break;
                    }
                }
                wchar_t message[] = L"⚕";
                mvprintw(y, i, "%ls", message);
                refresh();
                usleep(500000);
            }
        }
    }
    if (Current_Weapon == 'D')
    {
        int attack = 12 * power;
        int have = 0;
        for (int i = 0; i < WeaponCount; i++)
        {
            if (WeaponArray[i].mode == 2 && WeaponArray[i].times_use_left >= 1)
            {
                WeaponArray[i].times_use_left--;
                have = 1;
                break;
            }
        }
        if (have == 0)
        {
            strcpy(Line1, "You don't have a Weapon!                     ");
            return;
        }
        int cc = getch();
        if (cc == KEY_UP)
        {
            for (int i = y - 1; i >= y - 5; i--)
            {
                // if hit enemy
                if (Check_Enemy(i, x) == 0)
                {
                    strcpy(Line1, "You hit Enemy!                                                        ");
                    int index = -1;
                    for (int j = 0; j < EnemyCount; j++)
                    {
                        if (EnemyArray[j].x == x && EnemyArray[j].y == i && EnemyArray[j].hp > 0)
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
                            for (int k = 0; k < Corridor_Placed; k++)
                            {
                                if (i == CorridorArray[k].y && x == CorridorArray[k].x)
                                {
                                    which = 1;
                                    break;
                                }
                            }
                            if (which == 0)
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
                // if hit wall
                if (map[i][x] == '#' || map[i][x] == 'O' || map[i][x] == '%')
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = x;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = i + 1;
                    Wasted_Weapon_Count++;
                    if (map[i + 1][x] != '<')
                    {
                        map[i + 1][x] = '2';
                        break;
                    }
                }
                // if range is over
                if (i == y - 5)
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = x;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = i;
                    Wasted_Weapon_Count++;
                    if (map[i][x] != '<')
                    {
                        map[i][x] = '2';
                    }
                }
                // †
                wchar_t message[] = L"†";
                mvprintw(i, x, "%ls", message);
                refresh();
                usleep(500000);
            }
        }
        if (cc == KEY_DOWN)
        {
            for (int i = y + 1; i <= y + 5; i++)
            {
                // if hit enemy
                if (Check_Enemy(i, x) == 0)
                {
                    strcpy(Line1, "You hit Enemy!                                                        ");
                    int index = -1;
                    for (int j = 0; j < EnemyCount; j++)
                    {
                        if (EnemyArray[j].x == x && EnemyArray[j].y == i && EnemyArray[j].hp > 0)
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
                            for (int k = 0; k < Corridor_Placed; k++)
                            {
                                if (i == CorridorArray[k].y && x == CorridorArray[k].x)
                                {
                                    which = 1;
                                    break;
                                }
                            }
                            if (which == 0)
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
                // if hit wall
                if (map[i][x] == '#' || map[i][x] == 'O' || map[i][x] == '%')
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = x;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = i - 1;
                    Wasted_Weapon_Count++;
                    if (map[i - 1][x] != '<')
                    {
                        map[i - 1][x] = '2';
                        break;
                    }
                }
                // if range is over
                if (i == y + 5)
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = x;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = i;
                    Wasted_Weapon_Count++;
                    if (map[i][x] != '<')
                    {
                        map[i][x] = '2';
                        break;
                    }
                }
                wchar_t message[] = L"†";
                mvprintw(i, x, "%ls", message);
                refresh();
                usleep(500000);
            }
        }
        if (cc == KEY_RIGHT)
        {
            for (int i = x + 1; i <= x + 5; i++)
            {
                // if hit enemy
                if (Check_Enemy(y, i) == 0)
                {
                    strcpy(Line1, "You hit Enemy!                                                        ");
                    int index = -1;
                    for (int j = 0; j < EnemyCount; j++)
                    {
                        if (EnemyArray[j].x == i && EnemyArray[j].y == y && EnemyArray[j].hp > 0)
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
                            for (int k = 0; k < Corridor_Placed; k++)
                            {
                                if (y == CorridorArray[k].y && i == CorridorArray[k].x)
                                {
                                    which = 1;
                                    break;
                                }
                            }
                            if (which == 0)
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
                // if hit wall
                if (map[y][i] == '#' || map[y][i] == 'O' || map[y][i] == '%')
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = i - 1;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = y;
                    Wasted_Weapon_Count++;
                    if (map[y][i - 1] != '<')
                    {
                        map[y][i - 1] = '2';
                        break;
                    }
                }
                // if range is over
                if (i == x + 5)
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = i;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = y;
                    Wasted_Weapon_Count++;
                    if (map[y][i] != '<')
                    {
                        map[y][i] = '2';
                        break;
                    }
                }
                wchar_t message[] = L"†";
                mvprintw(y, i, "%ls", message);
                refresh();
                usleep(500000);
            }
        }
        if (cc == KEY_LEFT)
        {
            for (int i = x - 1; i >= x - 5; i--)
            {
                // if hit enemy
                if (Check_Enemy(y, i) == 0)
                {
                    strcpy(Line1, "You hit Enemy!                                                        ");
                    int index = -1;
                    for (int j = 0; j < EnemyCount; j++)
                    {
                        if (EnemyArray[j].x == i && EnemyArray[j].y == y && EnemyArray[j].hp > 0)
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
                            for (int k = 0; k < Corridor_Placed; k++)
                            {
                                if (y == CorridorArray[k].y && i == CorridorArray[k].x)
                                {
                                    which = 1;
                                    break;
                                }
                            }
                            if (which == 0)
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
                // if hit wall
                if (map[y][i] == '#' || map[y][i] == 'O' || map[y][i] == '%')
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = i + 1;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = y;
                    Wasted_Weapon_Count++;
                    if (map[y][i + 1] != '<')
                    {
                        map[y][i + 1] = '2';
                        break;
                    }
                }
                // if range is over
                if (i == x - 5)
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = i;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = y;
                    Wasted_Weapon_Count++;
                    if (map[y][i] != '<')
                    {
                        map[y][i] = '2';
                        break;
                    }
                }
                wchar_t message[] = L"†";
                mvprintw(y, i, "%ls", message);
                refresh();
                usleep(500000);
            }
        }
    }
    if (Current_Weapon == 'N')
    {
        int attack = 5 * power;
        int have = 0;
        for (int i = 0; i < WeaponCount; i++)
        {
            if (WeaponArray[i].mode == 4 && WeaponArray[i].times_use_left >= 1)
            {
                WeaponArray[i].times_use_left--;
                have = 1;
                break;
            }
        }
        if (have == 0)
        {
            strcpy(Line1, "You don't have a Weapon!                     ");
            return;
        }
        int cc = getch();
        if (cc == KEY_UP)
        {
            for (int i = y - 1; i >= y - 5; i--)
            {
                // if hit enemy
                if (Check_Enemy(i, x) == 0)
                {
                    strcpy(Line1, "You hit Enemy!                                                        ");
                    int index = -1;
                    for (int j = 0; j < EnemyCount; j++)
                    {
                        if (EnemyArray[j].x == x && EnemyArray[j].y == i && EnemyArray[j].hp > 0)
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
                            for (int k = 0; k < Corridor_Placed; k++)
                            {
                                if (i == CorridorArray[k].y && x == CorridorArray[k].x)
                                {
                                    which = 1;
                                    break;
                                }
                            }
                            if (which == 0)
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
                // if hit wall
                if (map[i][x] == '#' || map[i][x] == 'O' || map[i][x] == '%')
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = x;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = i + 1;
                    Wasted_Weapon_Count++;
                    if (map[i + 1][x] != '<')
                    {
                        map[i + 1][x] = '4';
                        break;
                    }
                }
                // if range is over
                if (i == y - 5)
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = x;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = i;
                    Wasted_Weapon_Count++;
                    if (map[i][x] != '<')
                    {
                        map[i][x] = '4';
                    }
                }
                wchar_t message[] = L"➳";
                mvprintw(i, x, "%ls", message);
                // mvprintw(i, x, "*");
                refresh();
                usleep(500000);
            }
        }
        if (cc == KEY_DOWN)
        {
            for (int i = y + 1; i <= y + 5; i++)
            {
                // if hit enemy
                if (Check_Enemy(i, x) == 0)
                {
                    strcpy(Line1, "You hit Enemy!                                                        ");
                    int index = -1;
                    for (int j = 0; j < EnemyCount; j++)
                    {
                        if (EnemyArray[j].x == x && EnemyArray[j].y == i && EnemyArray[j].hp > 0)
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
                            for (int k = 0; k < Corridor_Placed; k++)
                            {
                                if (i == CorridorArray[k].y && x == CorridorArray[k].x)
                                {
                                    which = 1;
                                    break;
                                }
                            }
                            if (which == 0)
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
                // if hit wall
                if (map[i][x] == '#' || map[i][x] == 'O' || map[i][x] == '%')
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = x;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = i - 1;
                    Wasted_Weapon_Count++;
                    if (map[i - 1][x] != '<')
                    {
                        map[i - 1][x] = '4';
                        break;
                    }
                }
                // if range is over
                if (i == y + 5)
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = x;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = i;
                    Wasted_Weapon_Count++;
                    if (map[i][x] != '<')
                    {
                        map[i][x] = '4';
                        break;
                    }
                }
                wchar_t message[] = L"➳";
                mvprintw(i, x, "%ls", message);
                // mvprintw(i, x, "*");
                refresh();
                usleep(500000);
            }
        }
        if (cc == KEY_RIGHT)
        {
            for (int i = x + 1; i <= x + 5; i++)
            {
                // if hit enemy
                if (Check_Enemy(y, i) == 0)
                {
                    strcpy(Line1, "You hit Enemy!                                                        ");
                    int index = -1;
                    for (int j = 0; j < EnemyCount; j++)
                    {
                        if (EnemyArray[j].x == i && EnemyArray[j].y == y && EnemyArray[j].hp > 0)
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
                            for (int k = 0; k < Corridor_Placed; k++)
                            {
                                if (y == CorridorArray[k].y && i == CorridorArray[k].x)
                                {
                                    which = 1;
                                    break;
                                }
                            }
                            if (which == 0)
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
                // if hit wall
                if (map[y][i] == '#' || map[y][i] == 'O' || map[y][i] == '%')
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = i - 1;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = y;
                    Wasted_Weapon_Count++;
                    if (map[y][i - 1] != '<')
                    {
                        map[y][i - 1] = '4';
                        break;
                    }
                }
                // if range is over
                if (i == x + 5)
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = i;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = y;
                    Wasted_Weapon_Count++;
                    if (map[y][i] != '<')
                    {
                        map[y][i] = '4';
                        break;
                    }
                }
                wchar_t message[] = L"➳";
                mvprintw(y, i, "%ls", message);
                // mvprintw(y, i, "*");
                refresh();
                usleep(500000);
            }
        }
        if (cc == KEY_LEFT)
        {
            for (int i = x - 1; i >= x - 5; i--)
            {
                // if hit enemy
                if (Check_Enemy(y, i) == 0)
                {
                    strcpy(Line1, "You hit Enemy!                                                        ");
                    int index = -1;
                    for (int j = 0; j < EnemyCount; j++)
                    {
                        if (EnemyArray[j].x == i && EnemyArray[j].y == y && EnemyArray[j].hp > 0)
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
                            for (int k = 0; k < Corridor_Placed; k++)
                            {
                                if (y == CorridorArray[k].y && i == CorridorArray[k].x)
                                {
                                    which = 1;
                                    break;
                                }
                            }
                            if (which == 0)
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
                // if hit wall
                if (map[y][i] == '#' || map[y][i] == 'O' || map[y][i] == '%')
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = i + 1;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = y;
                    Wasted_Weapon_Count++;
                    if (map[y][i + 1] != '<')
                    {
                        map[y][i + 1] = '4';
                        break;
                    }
                }
                // if range is over
                if (i == x - 5)
                {
                    Wasted_Weapon_Array[Wasted_Weapon_Count].x = i;
                    Wasted_Weapon_Array[Wasted_Weapon_Count].y = y;
                    Wasted_Weapon_Count++;
                    if (map[y][i] != '<')
                    {
                        map[y][i] = '4';
                        break;
                    }
                }
                wchar_t message[] = L"➳";
                mvprintw(y, i, "%ls", message);
                // mvprintw(y, i, "*");
                refresh();
                usleep(500000);
            }
        }
    }
}

void Far_Battle2(int playery, int playerx)
{
    if (Current_Weapon == 'M')
    {
        int attack = 15 * power;
        int have = 0;
        for (int i = 0; i < WeaponCount; i++)
        {
            if (WeaponArray[i].mode == 3 && WeaponArray[i].times_use_left >= 1)
            {
                WeaponArray[i].times_use_left--;
                have = 1;
                break;
            }
        }
        if (have == 0)
        {
            strcpy(Line1, "You don't have a Weapon!                     ");
            return;
        }
        int cc = getch();
        if (cc == KEY_UP)
        {
            for (int i = playery - 1; i >= playery - 5; i--)
            {
                // if hit enemy
                if (Check_Enemy2(i, playerx) == 0)
                {
                    strcpy(Line1, "You hit Enemy!                                                        ");
                    int index = -1;
                    for (int j = 0; j < Enemycount2; j++)
                    {
                        if (EnemyArray2[j].x == playerx && EnemyArray2[j].y == i && EnemyArray2[j].hp > 0)
                        {
                            index = j;
                            break;
                        }
                    }
                    EnemyArray2[index].move = -1;
                    if (EnemyArray2[index].hp >= attack)
                    {
                        EnemyArray2[index].hp -= attack;
                        if (EnemyArray2[index].hp == 0)
                        {
                            strcpy(Line1, "Enemy Killed!                                      ");
                        }
                    }
                    else
                    {
                        strcpy(Line1, "Enemy Killed!                                                     ");
                        EnemyArray2[index].hp = 0;
                    }
                    break;
                }
                // if hit wall
                if (map3[i][playerx] == '#' || map3[i][playerx] == 'O' || map3[i][playerx] == '%')
                {
                    if (map3[i + 1][playerx] != '<')
                    {
                        Wasted_Weapon_Array2[Wasted_Weapon_count2].x = playerx;
                        Wasted_Weapon_Array2[Wasted_Weapon_count2].y = i + 1;
                        Wasted_Weapon_count2++;
                        map3[i + 1][playerx] = '3';
                        break;
                    }
                }
                // if range is over
                if (i == playery - 5)
                {
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].x = playerx;
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].y = i;
                    Wasted_Weapon_Count++;
                    if (map3[i][playerx] != '<')
                    {
                        map3[i][playerx] = '3';
                        break;
                    }
                }
                wchar_t message[] = L"⚕";
                mvprintw(i, playerx, "%ls", message);
                refresh();
                usleep(500000);
            }
        }
        if (cc == KEY_DOWN)
        {
            for (int i = playery + 1; i <= playery + 5; i++)
            {
                // if hit enemy
                if (Check_Enemy2(i, playerx) == 0)
                {
                    strcpy(Line1, "You hit Enemy!                                                        ");
                    int index = -1;
                    for (int j = 0; j < Enemycount2; j++)
                    {
                        if (EnemyArray2[j].x == playerx && EnemyArray2[j].y == i && EnemyArray2[j].hp > 0)
                        {
                            index = j;
                            break;
                        }
                    }
                    EnemyArray2[index].move = -1;
                    if (EnemyArray2[index].hp >= attack)
                    {
                        EnemyArray2[index].hp -= attack;
                        if (EnemyArray2[index].hp == 0)
                        {
                            strcpy(Line1, "Enemy Killed!                                      ");
                        }
                    }
                    else
                    {
                        strcpy(Line1, "Enemy Killed!                                                     ");
                        EnemyArray2[index].hp = 0;
                    }
                    break;
                }
                // if hit wall
                if (map3[i][playerx] == '#' || map3[i][playerx] == 'O' || map3[i][playerx] == '%')
                {
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].x = playerx;
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].y = i - 1;
                    Wasted_Weapon_count2++;
                    if (map3[i - 1][playerx] != '<')
                    {
                        map3[i - 1][playerx] = '3';
                        break;
                    }
                }
                // if range is over
                if (i == playery + 5)
                {
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].x = playerx;
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].y = i;
                    Wasted_Weapon_count2++;
                    if (map3[i][playerx] != '<')
                    {
                        map3[i][playerx] = '3';
                        break;
                    }
                }
                wchar_t message[] = L"⚕";
                mvprintw(i, playerx, "%ls", message);
                refresh();
                usleep(500000);
            }
        }
        if (cc == KEY_RIGHT)
        {
            for (int i = playerx + 1; i <= playerx + 5; i++)
            {
                // if hit enemy
                if (Check_Enemy2(playery, i) == 0)
                {
                    strcpy(Line1, "You hit Enemy!                                                        ");
                    int index = -1;
                    for (int j = 0; j < Enemycount2; j++)
                    {
                        if (EnemyArray2[j].x == i && EnemyArray2[j].y == playery && EnemyArray2[j].hp > 0)
                        {
                            index = j;
                            break;
                        }
                    }
                    EnemyArray2[index].move = -1;
                    if (EnemyArray2[index].hp >= attack)
                    {
                        EnemyArray2[index].hp -= attack;
                        if (EnemyArray2[index].hp == 0)
                        {
                            strcpy(Line1, "Enemy Killed!                                      ");
                            int which = 0;
                        }
                    }
                    else
                    {
                        strcpy(Line1, "Enemy Killed!                                                     ");
                        EnemyArray2[index].hp = 0;
                    }
                    break;
                }
                // if hit wall
                if (map3[playery][i] == '#' || map3[playery][i] == 'O' || map3[playery][i] == '%')
                {
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].x = i - 1;
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].y = playery;
                    Wasted_Weapon_count2++;
                    if (map3[playery][i - 1] != '<')
                    {
                        map3[playery][i - 1] = '3';
                        break;
                    }
                }
                // if range is over
                if (i == playerx + 5)
                {
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].x = i;
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].y = playery;
                    Wasted_Weapon_count2++;
                    if (map3[playery][i] != '<')
                    {
                        map3[playery][i] = '3';
                        break;
                    }
                }
                wchar_t message[] = L"⚕";
                mvprintw(playery, i, "%ls", message);
                refresh();
                usleep(500000);
            }
        }
        if (cc == KEY_LEFT)
        {
            for (int i = playerx - 1; i >= playerx - 5; i--)
            {
                // if hit enemy
                if (Check_Enemy2(playery, i) == 0)
                {
                    strcpy(Line1, "You hit Enemy!                                                        ");
                    int index = -1;
                    for (int j = 0; j < Enemycount2; j++)
                    {
                        if (EnemyArray2[j].x == i && EnemyArray2[j].y == playery && EnemyArray2[j].hp > 0)
                        {
                            index = j;
                            break;
                        }
                    }
                    EnemyArray2[index].move = -1;
                    if (EnemyArray2[index].hp >= attack)
                    {
                        EnemyArray2[index].hp -= attack;
                        if (EnemyArray2[index].hp == 0)
                        {
                            strcpy(Line1, "Enemy Killed!                                      ");
                        }
                    }
                    else
                    {
                        strcpy(Line1, "Enemy Killed!                                                     ");
                        EnemyArray2[index].hp = 0;
                    }
                    break;
                }
                // if hit wall
                if (map[playery][i] == '#' || map[playery][i] == 'O' || map[playery][i] == '%')
                {
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].x = i + 1;
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].y = playery;
                    Wasted_Weapon_count2++;
                    if (map3[playery][i + 1] != '<')
                    {
                        map3[playery][i + 1] = '3';
                        break;
                    }
                }
                // if range is over
                if (i == playerx - 5)
                {
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].x = i;
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].y = playery;
                    Wasted_Weapon_count2++;
                    if (map3[playery][i] != '<')
                    {
                        map3[playery][i] = '3';
                        break;
                    }
                }
                wchar_t message[] = L"⚕";
                mvprintw(playery, i, "%ls", message);
                refresh();
                usleep(500000);
            }
        }
    }
    if (Current_Weapon == 'D')
    {
        int attack = 12 * power;
        int have = 0;
        for (int i = 0; i < WeaponCount; i++)
        {
            if (WeaponArray[i].mode == 2 && WeaponArray[i].times_use_left >= 1)
            {
                WeaponArray[i].times_use_left--;
                have = 1;
                break;
            }
        }
        if (have == 0)
        {
            strcpy(Line1, "You don't have a Weapon!                     ");
            return;
        }
        int cc = getch();
        if (cc == KEY_UP)
        {
            for (int i = playery - 1; i >= playery - 5; i--)
            {
                // if hit enemy
                if (Check_Enemy2(i, playerx) == 0)
                {
                    strcpy(Line1, "You hit Enemy!                                                        ");
                    int index = -1;
                    for (int j = 0; j < Enemycount2; j++)
                    {
                        if (EnemyArray2[j].x == playerx && EnemyArray2[j].y == i && EnemyArray2[j].hp > 0)
                        {
                            index = j;
                            break;
                        }
                    }
                    if (EnemyArray2[index].hp >= attack)
                    {
                        EnemyArray2[index].hp -= attack;
                        if (EnemyArray2[index].hp == 0)
                        {
                            strcpy(Line1, "Enemy Killed!                                      ");
                        }
                    }
                    else
                    {
                        strcpy(Line1, "Enemy Killed!                                                     ");
                        EnemyArray2[index].hp = 0;
                    }
                    break;
                }
                // if hit wall
                if (map3[i][playerx] == '#' || map3[i][playerx] == 'O' || map3[i][playerx] == '%')
                {
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].x = playerx;
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].y = i + 1;
                    Wasted_Weapon_count2++;
                    if (map3[i + 1][playerx] != '<')
                    {
                        map3[i + 1][playerx] = '2';
                        break;
                    }
                }
                // if range is over
                if (i == playery - 5)
                {
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].x = playerx;
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].y = i;
                    Wasted_Weapon_count2++;
                    if (map3[i][playerx] != '<')
                    {
                        map3[i][playerx] = '2';
                    }
                }
                // †
                wchar_t message[] = L"†";
                mvprintw(i, playerx, "%ls", message);
                refresh();
                usleep(500000);
            }
        }
        if (cc == KEY_DOWN)
        {
            for (int i = playery + 1; i <= playery + 5; i++)
            {
                // if hit enemy
                if (Check_Enemy2(i, playerx) == 0)
                {
                    strcpy(Line1, "You hit Enemy!                                                        ");
                    int index = -1;
                    for (int j = 0; j < Enemycount2; j++)
                    {
                        if (EnemyArray2[j].x == playerx && EnemyArray2[j].y == i && EnemyArray2[j].hp > 0)
                        {
                            index = j;
                            break;
                        }
                    }
                    if (EnemyArray2[index].hp >= attack)
                    {
                        EnemyArray2[index].hp -= attack;
                        if (EnemyArray2[index].hp == 0)
                        {
                            strcpy(Line1, "Enemy Killed!                                      ");
                        }
                    }
                    else
                    {
                        strcpy(Line1, "Enemy Killed!                                                     ");
                        EnemyArray2[index].hp = 0;
                    }
                    break;
                }
                // if hit wall
                if (map3[i][playerx] == '#' || map3[i][playerx] == 'O' || map3[i][playerx] == '%')
                {
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].x = playerx;
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].y = i - 1;
                    Wasted_Weapon_count2++;
                    if (map3[i - 1][playerx] != '<')
                    {
                        map3[i - 1][playerx] = '2';
                        break;
                    }
                }
                // if range is over
                if (i == playery + 5)
                {
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].x = playerx;
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].y = i;
                    Wasted_Weapon_count2++;
                    if (map3[i][playerx] != '<')
                    {
                        map3[i][playerx] = '2';
                        break;
                    }
                }
                wchar_t message[] = L"†";
                mvprintw(i, playerx, "%ls", message);
                refresh();
                usleep(500000);
            }
        }
        if (cc == KEY_RIGHT)
        {
            for (int i = playerx + 1; i <= playerx + 5; i++)
            {
                // if hit enemy
                if (Check_Enemy2(playery, i) == 0)
                {
                    strcpy(Line1, "You hit Enemy!                                                        ");
                    int index = -1;
                    for (int j = 0; j < Enemycount2; j++)
                    {
                        if (EnemyArray2[j].x == i && EnemyArray2[j].y == playery && EnemyArray2[j].hp > 0)
                        {
                            index = j;
                            break;
                        }
                    }
                    if (EnemyArray2[index].hp >= attack)
                    {
                        EnemyArray2[index].hp -= attack;
                        if (EnemyArray2[index].hp == 0)
                        {
                            strcpy(Line1, "Enemy Killed!                                      ");
                        }
                    }
                    else
                    {
                        strcpy(Line1, "Enemy Killed!                                                     ");
                        EnemyArray2[index].hp = 0;
                    }
                    break;
                }
                // if hit wall
                if (map3[playery][i] == '#' || map3[playery][i] == 'O' || map3[playery][i] == '%')
                {
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].x = i - 1;
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].y = playery;
                    Wasted_Weapon_count2++;
                    if (map3[playery][i - 1] != '<')
                    {
                        map3[playery][i - 1] = '2';
                        break;
                    }
                }
                // if range is over
                if (i == playerx + 5)
                {
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].x = i;
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].y = playery;
                    Wasted_Weapon_count2++;
                    if (map3[playery][i] != '<')
                    {
                        map3[playery][i] = '2';
                        break;
                    }
                }
                wchar_t message[] = L"†";
                mvprintw(playery, i, "%ls", message);
                refresh();
                usleep(500000);
            }
        }
        if (cc == KEY_LEFT)
        {
            for (int i = playerx - 1; i >= playerx - 5; i--)
            {
                // if hit enemy
                if (Check_Enemy2(playery, i) == 0)
                {
                    strcpy(Line1, "You hit Enemy!                                                        ");
                    int index = -1;
                    for (int j = 0; j < Enemycount2; j++)
                    {
                        if (EnemyArray2[j].x == i && EnemyArray2[j].y == playery && EnemyArray2[j].hp > 0)
                        {
                            index = j;
                            break;
                        }
                    }
                    if (EnemyArray2[index].hp >= attack)
                    {
                        EnemyArray2[index].hp -= attack;
                        if (EnemyArray2[index].hp == 0)
                        {
                            strcpy(Line1, "Enemy Killed!                                      ");
                        }
                    }
                    else
                    {
                        strcpy(Line1, "Enemy Killed!                                                     ");
                        EnemyArray2[index].hp = 0;
                    }
                    break;
                }
                // if hit wall
                if (map3[playery][i] == '#' || map3[playery][i] == 'O' || map3[playery][i] == '%')
                {
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].x = i + 1;
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].y = playery;
                    Wasted_Weapon_count2++;
                    if (map3[playery][i + 1] != '<')
                    {
                        map3[playery][i + 1] = '2';
                        break;
                    }
                }
                // if range is over
                if (i == playerx - 5)
                {
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].x = i;
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].y = playery;
                    Wasted_Weapon_count2++;
                    if (map3[playery][i] != '<')
                    {
                        map3[playery][i] = '2';
                        break;
                    }
                }
                wchar_t message[] = L"†";
                mvprintw(playery, i, "%ls", message);
                refresh();
                usleep(500000);
            }
        }
    }
    if (Current_Weapon == 'N')
    {
        int attack = 5 * power;
        int have = 0;
        for (int i = 0; i < WeaponCount; i++)
        {
            if (WeaponArray[i].mode == 4 && WeaponArray[i].times_use_left >= 1)
            {
                WeaponArray[i].times_use_left--;
                have = 1;
                break;
            }
        }
        if (have == 0)
        {
            strcpy(Line1, "You don't have a Weapon!                     ");
            return;
        }
        int cc = getch();
        if (cc == KEY_UP)
        {
            for (int i = playery - 1; i >= playery - 5; i--)
            {
                // if hit enemy
                if (Check_Enemy2(i, playerx) == 0)
                {
                    strcpy(Line1, "You hit Enemy!                                                        ");
                    int index = -1;
                    for (int j = 0; j < Enemycount2; j++)
                    {
                        if (EnemyArray2[j].x == playerx && EnemyArray2[j].y == i && EnemyArray2[j].hp > 0)
                        {
                            index = j;
                            break;
                        }
                    }
                    if (EnemyArray2[index].hp >= attack)
                    {
                        EnemyArray2[index].hp -= attack;
                        if (EnemyArray2[index].hp == 0)
                        {
                            strcpy(Line1, "Enemy Killed!                                      ");
                        }
                    }
                    else
                    {
                        strcpy(Line1, "Enemy Killed!                                                     ");
                        EnemyArray2[index].hp = 0;
                    }
                    break;
                }
                // if hit wall
                if (map3[i][playerx] == '#' || map3[i][playerx] == 'O' || map3[i][playerx] == '%')
                {
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].x = playerx;
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].y = i + 1;
                    Wasted_Weapon_count2++;
                    if (map3[i + 1][playerx] != '<')
                    {
                        map3[i + 1][playerx] = '4';
                        break;
                    }
                }
                // if range is over
                if (i == playery - 5)
                {
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].x = playerx;
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].y = i;
                    Wasted_Weapon_count2++;
                    if (map3[i][playerx] != '<')
                    {
                        map3[i][playerx] = '4';
                    }
                }
                wchar_t message[] = L"➳";
                mvprintw(i, playerx, "%ls", message);
                // mvprintw(i, x, "*");
                refresh();
                usleep(500000);
            }
        }
        if (cc == KEY_DOWN)
        {
            for (int i = playery + 1; i <= playery + 5; i++)
            {
                // if hit enemy
                if (Check_Enemy2(i, playerx) == 0)
                {
                    strcpy(Line1, "You hit Enemy!                                                        ");
                    int index = -1;
                    for (int j = 0; j < Enemycount2; j++)
                    {
                        if (EnemyArray2[j].x == playerx && EnemyArray2[j].y == i && EnemyArray2[j].hp > 0)
                        {
                            index = j;
                            break;
                        }
                    }
                    if (EnemyArray2[index].hp >= attack)
                    {
                        EnemyArray2[index].hp -= attack;
                        if (EnemyArray2[index].hp == 0)
                        {
                            strcpy(Line1, "Enemy Killed!                                      ");
                        }
                    }
                    else
                    {
                        strcpy(Line1, "Enemy Killed!                                                     ");
                        EnemyArray2[index].hp = 0;
                    }
                    break;
                }
                // if hit wall
                if (map3[i][playerx] == '#' || map3[i][playerx] == 'O' || map3[i][playerx] == '%')
                {
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].x = playerx;
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].y = i - 1;
                    Wasted_Weapon_count2++;
                    if (map3[i - 1][playerx] != '<')
                    {
                        map3[i - 1][playerx] = '4';
                        break;
                    }
                }
                // if range is over
                if (i == playery + 5)
                {
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].x = playerx;
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].y = i;
                    Wasted_Weapon_count2++;
                    if (map3[i][playerx] != '<')
                    {
                        map3[i][playerx] = '4';
                        break;
                    }
                }
                wchar_t message[] = L"➳";
                mvprintw(i, playerx, "%ls", message);
                // mvprintw(i, x, "*");
                refresh();
                usleep(500000);
            }
        }
        if (cc == KEY_RIGHT)
        {
            for (int i = playerx + 1; i <= playerx + 5; i++)
            {
                // if hit enemy
                if (Check_Enemy2(playery, i) == 0)
                {
                    strcpy(Line1, "You hit Enemy!                                                        ");
                    int index = -1;
                    for (int j = 0; j < Enemycount2; j++)
                    {
                        if (EnemyArray2[j].x == i && EnemyArray2[j].y == playery && EnemyArray2[j].hp > 0)
                        {
                            index = j;
                            break;
                        }
                    }
                    if (EnemyArray2[index].hp >= attack)
                    {
                        EnemyArray2[index].hp -= attack;
                        if (EnemyArray2[index].hp == 0)
                        {
                            strcpy(Line1, "Enemy Killed!                                      ");
                        }
                    }
                    else
                    {
                        strcpy(Line1, "Enemy Killed!                                                     ");
                        EnemyArray2[index].hp = 0;
                    }
                    break;
                }
                // if hit wall
                if (map3[playery][i] == '#' || map3[playery][i] == 'O' || map3[playery][i] == '%')
                {
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].x = i - 1;
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].y = playery;
                    Wasted_Weapon_count2++;
                    if (map3[playery][i - 1] != '<')
                    {
                        map3[playery][i - 1] = '4';
                        break;
                    }
                }
                // if range is over
                if (i == playerx + 5)
                {
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].x = i;
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].y = playery;
                    Wasted_Weapon_count2++;
                    if (map3[playery][i] != '<')
                    {
                        map3[playery][i] = '4';
                        break;
                    }
                }
                wchar_t message[] = L"➳";
                mvprintw(playery, i, "%ls", message);
                // mvprintw(y, i, "*");
                refresh();
                usleep(500000);
            }
        }
        if (cc == KEY_LEFT)
        {
            for (int i = playerx - 1; i >= playerx - 5; i--)
            {
                // if hit enemy
                if (Check_Enemy2(playery, i) == 0)
                {
                    strcpy(Line1, "You hit Enemy!                                                        ");
                    int index = -1;
                    for (int j = 0; j < Enemycount2; j++)
                    {
                        if (EnemyArray2[j].x == i && EnemyArray2[j].y == playery && EnemyArray2[j].hp > 0)
                        {
                            index = j;
                            break;
                        }
                    }
                    if (EnemyArray2[index].hp >= attack)
                    {
                        EnemyArray2[index].hp -= attack;
                        if (EnemyArray2[index].hp == 0)
                        {
                            strcpy(Line1, "Enemy Killed!                                      ");
                        }
                    }
                    else
                    {
                        strcpy(Line1, "Enemy Killed!                                                     ");
                        EnemyArray2[index].hp = 0;
                    }
                    break;
                }
                // if hit wall
                if (map3[playery][i] == '#' || map3[playery][i] == 'O' || map3[playery][i] == '%')
                {
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].x = i + 1;
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].y = playery;
                    Wasted_Weapon_count2++;
                    if (map3[playery][i + 1] != '<')
                    {
                        map3[playery][i + 1] = '4';
                        break;
                    }
                }
                // if range is over
                if (i == playerx - 5)
                {
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].x = i;
                    Wasted_Weapon_Array2[Wasted_Weapon_count2].y = playery;
                    Wasted_Weapon_count2++;
                    if (map3[playery][i] != '<')
                    {
                        map3[playery][i] = '4';
                        break;
                    }
                }
                wchar_t message[] = L"➳";
                mvprintw(playery, i, "%ls", message);
                // mvprintw(y, i, "*");
                refresh();
                usleep(500000);
            }
        }
    }
}
/*Mix_HaltMusic();
    Mix_FreeMusic(music);
    music = NULL;
    SDL_Delay(1000);
    play_music("2.mp3");
    SDL_Delay(1000);*/
    /*Mix_HaltMusic();
    Mix_FreeMusic(music);
    music = NULL;
    SDL_Delay(1000);
    play_music("1.mp3");
    SDL_Delay(1000);*/

void Telesm_Room2()
{
    
    // init map with space
    char map2[12][12];
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            map2[i][j] = ' ';
        }
    }
    trap TRAPS[20];
    int tpcount = 0;
    // set borders of map
    for (int i = 0; i < 12; i++)
    {
        map2[0][i] = '#';
        map2[11][i] = '#';
        map2[i][0] = '#';
        map2[i][11] = '#';
    }
    int randomx_player = (rand() % 5) + 2;
    int randomy_player = (rand() % 5) + 2;
    int randomx;
    int randomy;
    // generate 50 traps in map
    for (int bar = 1; bar <= 50; bar++)
    {
        do
        {
            randomx = (rand() % 10) + 1;
            randomy = (rand() % 10) + 1;
        } while ((randomy == randomy_player && randomx == randomx_player) || map2[randomy][randomx] == '^');
        map2[randomy][randomx] = '^';
        TRAPS[tpcount].x = randomx;
        TRAPS[tpcount].y = randomy;
        TRAPS[tpcount].show = 0;
        tpcount++;
    }

    int c = 113;
    do
    {
        // Lose
        if (playerHP <= 0)
        {
            Lose();
        }

        if (c != 113)
        {
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
            //

            if (c == 'j' && map2[randomy_player - 1][randomx_player] != '#')
            {
                randomy_player--;
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
                if (c == 'k' && map2[randomy_player + 1][randomx_player] != '#')
                {
                    randomy_player++;
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
                    if (c == 'l' && map2[randomy_player][randomx_player + 1] != '#')
                    {
                        if (MovesTillLastpot >= 1)
                        {
                            MovesTillLastpot--;
                        }
                        randomx_player++;
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
                        if (c == 'h' && map2[randomy_player][randomx_player - 1] != '#')
                        {
                            if (MovesTillLastpot >= 1)
                            {
                                MovesTillLastpot--;
                            }
                            randomx_player--;
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
                            if (c == 'y' && map2[randomy_player - 1][randomx_player - 1] != '#')
                            {
                                if (MovesTillLastpot >= 1)
                                {
                                    MovesTillLastpot--;
                                }
                                randomy_player--;
                                randomx_player--;
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
                                if (c == 'u' && map2[randomy_player - 1][randomx_player + 1] != '#')
                                {
                                    if (MovesTillLastpot >= 1)
                                    {
                                        MovesTillLastpot--;
                                    }
                                    randomy_player--;
                                    randomx_player++;
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
                                    if (c == 'b' && map2[randomy_player + 1][randomx_player - 1] != '#')
                                    {
                                        if (MovesTillLastpot >= 1)
                                        {
                                            MovesTillLastpot--;
                                        }
                                        randomx_player--;
                                        randomy_player++;
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
                                        if (c == 'n' && map2[randomy_player + 1][randomx_player + 1] != '#')
                                        {
                                            if (MovesTillLastpot >= 1)
                                            {
                                                MovesTillLastpot--;
                                            }
                                            randomx_player++;
                                            randomy_player++;
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
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if (map2[randomy_player][randomx_player] == '^')
            {
                int ij;
                for (ij = 0; ij < tpcount; ij++)
                {
                    if (randomy_player == TRAPS[ij].y && randomx_player == TRAPS[ij].x)
                    {
                        break;
                    }
                }
                TRAPS[ij].show = 1;
                if (playerHP >= 100)
                {
                    playerHP -= 100;
                }
                else
                {
                    playerHP = 0;
                }
            }

            // Food Inentory
            if (c == 'E' || c == 'e')
            {
                FoodChoice();
                clear();
            }
        }

        // print map
        for (int i = 0; i < 12; i++)
        {
            for (int j = 0; j < 12; j++)
            {
                if (map2[i][j] == '^')
                {
                    int sh = 0;
                    for (int k = 0; k < tpcount; k++)
                    {
                        if (TRAPS[k].x == j && TRAPS[k].y == i)
                        {
                            if (TRAPS[k].show == 1)
                            {
                                sh = 1;
                            }
                            break;
                        }
                    }
                    if (sh == 0)
                    {
                        mvaddch(i, j, ' ');
                    }
                    else
                    {
                        mvaddch(i, j, '^');
                    }
                }
                else
                {
                    mvprintw(i, j, "%c", map2[i][j]);
                }
            }
            printw("\n");
        }
        // print player
        mvaddch(randomy_player, randomx_player, '@');
        // Stats
        wchar_t messag1[] = L"💰";
        wchar_t messag2[] = L"❤️";
        wchar_t messag3[] = L"🔑";
        wchar_t messag4[] = L"🏃";
        wchar_t messag5[] = L"🍔";
        wchar_t messag6[] = L"⏫";
        wchar_t messag7[] = L"🚀";
        wchar_t messag8[] = L"💪";
        mvprintw(rows - 2, 0, "                                                                                                                                                                                              ");
        // show HP
        // mvprintw(rows - 2, 0 + 30, "%ls  :", messag2);
        if (playerHP == 1000)
        {
            wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP > 900 && playerHP < 1000)
        {
            wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ 💔";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP == 900)
        {
            wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ 🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP > 800 && playerHP < 900)
        {
            wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ 💔🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP == 800)
        {
            wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ 🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP > 700 && playerHP < 800)
        {
            wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ 💔🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP == 700)
        {
            wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ 🤍🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP > 600 && playerHP < 700)
        {
            wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ ❤️ ❤️ 💔🤍🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP == 600)
        {
            wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ ❤️ ❤️ 🤍🤍🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP > 500 && playerHP < 600)
        {
            wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ ❤️ 💔🤍🤍🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP == 500)
        {
            wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ ❤️ 🤍🤍🤍🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP > 400 && playerHP < 500)
        {
            wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ 💔🤍🤍🤍🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP == 400)
        {
            wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ 🤍🤍🤍🤍🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP > 300 && playerHP < 400)
        {
            wchar_t messag9[] = L"❤️ ❤️ ❤️ 💔🤍🤍🤍🤍🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP == 300)
        {
            wchar_t messag9[] = L"❤️ ❤️ ❤️ 🤍🤍🤍🤍🤍🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP > 200 && playerHP < 300)
        {
            wchar_t messag9[] = L"❤️ ❤️ 💔🤍🤍🤍🤍🤍🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP == 200)
        {
            wchar_t messag9[] = L"❤️ ❤️ 🤍🤍🤍🤍🤍🤍🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP > 100 && playerHP < 200)
        {
            wchar_t messag9[] = L"❤️ 💔🤍🤍🤍🤍🤍🤍🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP == 100)
        {
            wchar_t messag9[] = L"❤️ 🤍🤍🤍🤍🤍🤍🤍🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP > 0 && playerHP < 100)
        {
            wchar_t messag9[] = L"💔🤍🤍🤍🤍🤍🤍🤍🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP == 0)
        {
            wchar_t messag9[] = L"🤍🤍🤍🤍🤍🤍🤍🤍🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        attron(COLOR_PAIR(1) | A_BOLD);
        mvprintw(rows - 2, 28 + 30, "%d%%", playerHP / 10);
        attroff(COLOR_PAIR(1) | A_BOLD);
        // Food
        // mvprintw(rows - 2, 0 + 30 + 50 + 50 - 10, "%ls  :", messag5);
        if (PlayerFood == 10)
        {
            wchar_t messag9[] = L"🍏🍏🍏🍏🍏🍏🍏🍏🍏🍏";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood > 9 && PlayerFood < 10)
        {
            wchar_t messag9[] = L"🍏🍏🍏🍏🍏🍏🍏🍏🍏⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood == 9)
        {
            wchar_t messag9[] = L"🍏🍏🍏🍏🍏🍏🍏🍏🍏⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood > 8 && PlayerFood < 9)
        {
            wchar_t messag9[] = L"🍏🍏🍏🍏🍏🍏🍏🍏⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood == 8)
        {
            wchar_t messag9[] = L"🍏🍏🍏🍏🍏🍏🍏🍏⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood > 7 && PlayerFood < 8)
        {
            wchar_t messag9[] = L"🍏🍏🍏🍏🍏🍏🍏⚪⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood == 7)
        {
            wchar_t messag9[] = L"🍏🍏🍏🍏🍏🍏🍏⚪⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood > 6 && PlayerFood < 7)
        {
            wchar_t messag9[] = L"🍏🍏🍏🍏🍏🍏⚪⚪⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood == 6)
        {
            wchar_t messag9[] = L"🍏🍏🍏🍏🍏🍏⚪⚪⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood > 5 && PlayerFood < 6)
        {
            wchar_t messag9[] = L"🍏🍏🍏🍏🍏⚪⚪⚪⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood == 5)
        {
            wchar_t messag9[] = L"🍏🍏🍏🍏🍏⚪⚪⚪⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood > 4 && PlayerFood < 5)
        {
            wchar_t messag9[] = L"🍏🍏🍏🍏⚪⚪⚪⚪⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood == 4)
        {
            wchar_t messag9[] = L"🍏🍏🍏🍏⚪⚪⚪⚪⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood > 3 && PlayerFood < 4)
        {
            wchar_t messag9[] = L"🍏🍏🍏⚪⚪⚪⚪⚪⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood == 3)
        {
            wchar_t messag9[] = L"🍏🍏🍏⚪⚪⚪⚪⚪⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood > 2 && PlayerFood < 3)
        {
            wchar_t messag9[] = L"🍏🍏⚪⚪⚪⚪⚪⚪⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood == 2)
        {
            wchar_t messag9[] = L"🍏🍏⚪⚪⚪⚪⚪⚪⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood > 1 && PlayerFood < 2)
        {
            wchar_t messag9[] = L"🍏⚪⚪⚪⚪⚪⚪⚪⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood == 1)
        {
            wchar_t messag9[] = L"🍏⚪⚪⚪⚪⚪⚪⚪⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood > 0 && PlayerFood < 1)
        {
            wchar_t messag9[] = L"⚪⚪⚪⚪⚪⚪⚪⚪⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood == 0)
        {
            wchar_t messag9[] = L"⚪⚪⚪⚪⚪⚪⚪⚪⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        attron(COLOR_PAIR(1) | A_BOLD);
        mvprintw(rows - 2, 28 + 30 + 50 + 50 - 10, "%d%%", (int)(PlayerFood * 10));
        attroff(COLOR_PAIR(1) | A_BOLD);
        //
        mvprintw(rows + 1, 0, "%ls  %d                                                                                 %ls  %d                                                                              %ls  %d",
                 messag1, playerGOLD, messag4, MovesTillLastFood, messag6, level);
        attron(A_BLINK);
        mvprintw(rows - 10, 0, "Press Escape to go back!");
        attroff(A_BLINK);
        refresh();
        // Lose
        if (playerHP <= 0)
        {
            Lose();
        }
    } while ((c = getch()) != 27);
    
}

void Telesm_Room()
{
    Mix_HaltMusic();
    Mix_FreeMusic(music);
    music = NULL;
    SDL_Delay(1000);
    play_music("2.mp3");
    SDL_Delay(1000);
    char map2[12][12];
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            map2[i][j] = ' ';
        }
    }
    trap TRAPS[20];
    int tpcount = 0;
    for (int i = 0; i < 12; i++)
    {
        map2[0][i] = '#';
        map2[11][i] = '#';
        map2[i][0] = '#';
        map2[i][11] = '#';
    }
    int randomx_player = (rand() % 10) + 1;
    int randomy_player = (rand() % 10) + 1;
    int randomx;
    int randomy;
    for (int bar = 1; bar <= 20; bar++)
    {
        do
        {
            randomx = (rand() % 10) + 1;
            randomy = (rand() % 10) + 1;
        } while ((randomy == randomy_player && randomx == randomx_player) || map2[randomy][randomx] == '^');
        map2[randomy][randomx] = '^';
        TRAPS[tpcount].x = randomx;
        TRAPS[tpcount].y = randomy;
        TRAPS[tpcount].show = 0;
        tpcount++;
    }

    int c = 113;
    do
    {
        // Lose
        if (playerHP <= 0)
        {
            Lose();
        }

        if (c != 113)
        {
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
            //

            if (c == 'j' && map2[randomy_player - 1][randomx_player] != '#')
            {
                randomy_player--;
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
                if (c == 'k' && map2[randomy_player + 1][randomx_player] != '#')
                {
                    randomy_player++;
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
                    if (c == 'l' && map2[randomy_player][randomx_player + 1] != '#')
                    {
                        if (MovesTillLastpot >= 1)
                        {
                            MovesTillLastpot--;
                        }
                        randomx_player++;
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
                        if (c == 'h' && map2[randomy_player][randomx_player - 1] != '#')
                        {
                            if (MovesTillLastpot >= 1)
                            {
                                MovesTillLastpot--;
                            }
                            randomx_player--;
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
                            if (c == 'y' && map2[randomy_player - 1][randomx_player - 1] != '#')
                            {
                                if (MovesTillLastpot >= 1)
                                {
                                    MovesTillLastpot--;
                                }
                                randomy_player--;
                                randomx_player--;
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
                                if (c == 'u' && map2[randomy_player - 1][randomx_player + 1] != '#')
                                {
                                    if (MovesTillLastpot >= 1)
                                    {
                                        MovesTillLastpot--;
                                    }
                                    randomy_player--;
                                    randomx_player++;
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
                                    if (c == 'b' && map2[randomy_player + 1][randomx_player - 1] != '#')
                                    {
                                        if (MovesTillLastpot >= 1)
                                        {
                                            MovesTillLastpot--;
                                        }
                                        randomx_player--;
                                        randomy_player++;
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
                                        if (c == 'n' && map2[randomy_player + 1][randomx_player + 1] != '#')
                                        {
                                            if (MovesTillLastpot >= 1)
                                            {
                                                MovesTillLastpot--;
                                            }
                                            randomx_player++;
                                            randomy_player++;
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
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if (map2[randomy_player][randomx_player] == '^')
            {
                int ij;
                for (ij = 0; ij < tpcount; ij++)
                {
                    if (randomy_player == TRAPS[ij].y && randomx_player == TRAPS[ij].x)
                    {
                        break;
                    }
                }
                TRAPS[ij].show = 1;
                if (playerHP >= 100)
                {
                    playerHP -= 100;
                }
                else
                {
                    playerHP = 0;
                }
            }
        }

        // print
        for (int i = 0; i < 12; i++)
        {
            for (int j = 0; j < 12; j++)
            {
                if (map2[i][j] == '^')
                {
                    int sh = 0;
                    for (int k = 0; k < tpcount; k++)
                    {
                        if (TRAPS[k].x == j && TRAPS[k].y == i)
                        {
                            if (TRAPS[k].show == 1)
                            {
                                sh = 1;
                            }
                            break;
                        }
                    }
                    if (sh == 0)
                    {
                        mvaddch(i, j, ' ');
                    }
                    else
                    {
                        mvaddch(i, j, '^');
                    }
                }
                else
                {
                    mvprintw(i, j, "%c", map2[i][j]);
                }
            }
            printw("\n");
        }
        // print player
        mvaddch(randomy_player, randomx_player, '@');
        wchar_t messag1[] = L"💰";
        wchar_t messag2[] = L"❤️";
        wchar_t messag3[] = L"🔑";
        wchar_t messag4[] = L"🏃";
        wchar_t messag5[] = L"🍔";
        wchar_t messag6[] = L"⏫";
        wchar_t messag7[] = L"🚀";
        wchar_t messag8[] = L"💪";
        mvprintw(rows - 2, 0, "                                                                                                                                                                                              ");
        mvprintw(rows - 2, 0 + 30, "%ls  :", messag2);
        if (playerHP >= 700)
        {
            if (playerHP >= 700 && playerHP < 800)
            {
                attron(COLOR_PAIR(5));
                wchar_t messag9[] = L"######";
                mvprintw(rows - 2, 6 + 30, "%ls", messag9);
                attroff(COLOR_PAIR(5));
                attron(COLOR_PAIR(7));
                wchar_t messag10[] = L"########";
                mvprintw(rows - 2, 12 + 30, "%ls", messag10);
                attroff(COLOR_PAIR(7));
                attron(COLOR_PAIR(2));
                wchar_t messag11[] = L"##";
                mvprintw(rows - 2, 20 + 30, "%ls", messag11);
                attroff(COLOR_PAIR(2));
                attron(COLOR_PAIR(1) | A_BOLD);
                mvprintw(rows - 2, 24 + 30, "%d%%", playerHP / 10);
                attroff(COLOR_PAIR(1) | A_BOLD);
            }
            if (playerHP >= 800 && playerHP < 900)
            {
                attron(COLOR_PAIR(5));
                wchar_t messag9[] = L"######";
                mvprintw(rows - 2, 6 + 30, "%ls", messag9);
                attroff(COLOR_PAIR(5));
                attron(COLOR_PAIR(7));
                wchar_t messag10[] = L"########";
                mvprintw(rows - 2, 12 + 30, "%ls", messag10);
                attroff(COLOR_PAIR(7));
                attron(COLOR_PAIR(2));
                wchar_t messag11[] = L"####";
                mvprintw(rows - 2, 20 + 30, "%ls", messag11);
                attroff(COLOR_PAIR(2));
                attron(COLOR_PAIR(1) | A_BOLD);
                mvprintw(rows - 2, 26 + 30, "%d%%", playerHP / 10);
                attroff(COLOR_PAIR(1) | A_BOLD);
            }
            if (playerHP >= 900 && playerHP <= 1000)
            {
                attron(COLOR_PAIR(5));
                wchar_t messag9[] = L"######";
                mvprintw(rows - 2, 6 + 30, "%ls", messag9);
                attroff(COLOR_PAIR(5));
                attron(COLOR_PAIR(7));
                wchar_t messag10[] = L"########";
                mvprintw(rows - 2, 12 + 30, "%ls", messag10);
                attroff(COLOR_PAIR(7));
                attron(COLOR_PAIR(2));
                wchar_t messag11[] = L"######";
                mvprintw(rows - 2, 20 + 30, "%ls", messag11);
                attroff(COLOR_PAIR(2));
                attron(COLOR_PAIR(1) | A_BOLD);
                mvprintw(rows - 2, 28 + 30, "%d%%", playerHP / 10);
                attroff(COLOR_PAIR(1) | A_BOLD);
            }
        }
        else
        {
            if (playerHP > 300)
            {
                if (playerHP > 300 && playerHP < 400)
                {
                    attron(COLOR_PAIR(5));
                    wchar_t messag9[] = L"######";
                    mvprintw(rows - 2, 6 + 30, "%ls", messag9);
                    attroff(COLOR_PAIR(5));
                    attron(COLOR_PAIR(7));
                    wchar_t messag10[] = L"##";
                    mvprintw(rows - 2, 12 + 30, "%ls", messag10);
                    attroff(COLOR_PAIR(7));
                    attron(COLOR_PAIR(1) | A_BOLD);
                    mvprintw(rows - 2, 16 + 30, "%d%%", playerHP / 10);
                    attroff(COLOR_PAIR(1) | A_BOLD);
                }
                if (playerHP >= 400 && playerHP < 500)
                {
                    attron(COLOR_PAIR(5));
                    wchar_t messag9[] = L"######";
                    mvprintw(rows - 2, 6 + 30, "%ls", messag9);
                    attroff(COLOR_PAIR(5));
                    attron(COLOR_PAIR(7));
                    wchar_t messag10[] = L"####";
                    mvprintw(rows - 2, 12 + 30, "%ls", messag10);
                    attroff(COLOR_PAIR(7));
                    attron(COLOR_PAIR(1) | A_BOLD);
                    mvprintw(rows - 2, 18 + 30, "%d%%", playerHP / 10);
                    attroff(COLOR_PAIR(1) | A_BOLD);
                }
                if (playerHP >= 500 && playerHP < 600)
                {
                    attron(COLOR_PAIR(5));
                    wchar_t messag9[] = L"######";
                    mvprintw(rows - 2, 6 + 30, "%ls", messag9);
                    attroff(COLOR_PAIR(5));
                    attron(COLOR_PAIR(7));
                    wchar_t messag10[] = L"######";
                    mvprintw(rows - 2, 12 + 30, "%ls", messag10);
                    attroff(COLOR_PAIR(7));
                    attron(COLOR_PAIR(1) | A_BOLD);
                    mvprintw(rows - 2, 20 + 30, "%d%%", playerHP / 10);
                    attroff(COLOR_PAIR(1) | A_BOLD);
                }
                if (playerHP >= 600 && playerHP < 700)
                {
                    attron(COLOR_PAIR(5));
                    wchar_t messag9[] = L"######";
                    mvprintw(rows - 2, 6 + 30, "%ls", messag9);
                    attroff(COLOR_PAIR(5));
                    attron(COLOR_PAIR(7));
                    wchar_t messag10[] = L"########";
                    mvprintw(rows - 2, 12 + 30, "%ls", messag10);
                    attroff(COLOR_PAIR(7));
                    attron(COLOR_PAIR(1) | A_BOLD);
                    mvprintw(rows - 2, 22 + 30, "%d%%", playerHP / 10);
                    attroff(COLOR_PAIR(1) | A_BOLD);
                }
            }
            else
            {
                if (playerHP > 0 && playerHP < 100)
                {
                    attron(COLOR_PAIR(5));
                    wchar_t messag9[] = L"##";
                    mvprintw(rows - 2, 6 + 30, "%ls", messag9);
                    attroff(COLOR_PAIR(5));
                    attron(COLOR_PAIR(1) | A_BOLD);
                    mvprintw(rows - 2, 10 + 30, "%d%%", playerHP / 10);
                    attroff(COLOR_PAIR(1) | A_BOLD);
                }
                if (playerHP >= 100 && playerHP < 200)
                {
                    attron(COLOR_PAIR(5));
                    wchar_t messag9[] = L"####";
                    mvprintw(rows - 2, 6 + 30, "%ls", messag9);
                    attroff(COLOR_PAIR(5));
                    attron(COLOR_PAIR(1) | A_BOLD);
                    mvprintw(rows - 2, 12 + 30, "%d%%", playerHP / 10);
                    attroff(COLOR_PAIR(1) | A_BOLD);
                }
                if (playerHP >= 200 && playerHP <= 300)
                {
                    attron(COLOR_PAIR(5));
                    wchar_t messag9[] = L"######";
                    mvprintw(rows - 2, 6 + 30, "%ls", messag9);
                    attroff(COLOR_PAIR(5));
                    attron(COLOR_PAIR(1) | A_BOLD);
                    mvprintw(rows - 2, 14 + 30, "%d%%", playerHP / 10);
                    attroff(COLOR_PAIR(1) | A_BOLD);
                }
            }
        }
        //
        mvprintw(rows - 2, 0 + 30 + 50 + 50 - 10, "%ls  :", messag5);
        if (PlayerFood >= 7)
        {
            if (PlayerFood >= 7 && PlayerFood < 8)
            {
                attron(COLOR_PAIR(5));
                wchar_t messag9[] = L"######";
                mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
                attroff(COLOR_PAIR(5));
                attron(COLOR_PAIR(7));
                wchar_t messag10[] = L"########";
                mvprintw(rows - 2, 12 + 30 + 50 + 50 - 10, "%ls", messag10);
                attroff(COLOR_PAIR(7));
                attron(COLOR_PAIR(2));
                wchar_t messag11[] = L"##";
                mvprintw(rows - 2, 20 + 30 + 50 + 50 - 10, "%ls", messag11);
                attroff(COLOR_PAIR(2));
                attron(COLOR_PAIR(1) | A_BOLD);
                mvprintw(rows - 2, 24 + 30 + 50 + 50 - 10, "%d%%", (int)(PlayerFood * 10));
                attroff(COLOR_PAIR(1) | A_BOLD);
            }
            if (PlayerFood >= 8 && PlayerFood < 9)
            {
                attron(COLOR_PAIR(5));
                wchar_t messag9[] = L"######";
                mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
                attroff(COLOR_PAIR(5));
                attron(COLOR_PAIR(7));
                wchar_t messag10[] = L"########";
                mvprintw(rows - 2, 12 + 30 + 50 + 50 - 10, "%ls", messag10);
                attroff(COLOR_PAIR(7));
                attron(COLOR_PAIR(2));
                wchar_t messag11[] = L"####";
                mvprintw(rows - 2, 20 + 30 + 50 + 50 - 10, "%ls", messag11);
                attroff(COLOR_PAIR(2));
                attron(COLOR_PAIR(1) | A_BOLD);
                mvprintw(rows - 2, 26 + 30 + 50 + 50 - 10, "%d%%", (int)(PlayerFood * 10));
                attroff(COLOR_PAIR(1) | A_BOLD);
            }
            if (PlayerFood >= 9 && PlayerFood <= 10)
            {
                attron(COLOR_PAIR(5));
                wchar_t messag9[] = L"######";
                mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
                attroff(COLOR_PAIR(5));
                attron(COLOR_PAIR(7));
                wchar_t messag10[] = L"########";
                mvprintw(rows - 2, 12 + 30 + 50 + 50 - 10, "%ls", messag10);
                attroff(COLOR_PAIR(7));
                attron(COLOR_PAIR(2));
                wchar_t messag11[] = L"######";
                mvprintw(rows - 2, 20 + 30 + 50 + 50 - 10, "%ls", messag11);
                attroff(COLOR_PAIR(2));
                attron(COLOR_PAIR(1) | A_BOLD);
                mvprintw(rows - 2, 28 + 30 + 50 + 50 - 10, "%d%%", (int)(PlayerFood * 10));
                attroff(COLOR_PAIR(1) | A_BOLD);
            }
        }
        else
        {
            if (PlayerFood > 3)
            {
                if (PlayerFood > 3 && PlayerFood < 4)
                {
                    attron(COLOR_PAIR(5));
                    wchar_t messag9[] = L"######";
                    mvprintw(rows - 2, 6 + 30 + 100 - 10, "%ls", messag9);
                    attroff(COLOR_PAIR(5));
                    attron(COLOR_PAIR(7));
                    wchar_t messag10[] = L"##";
                    mvprintw(rows - 2, 12 + 30 + 100 - 10, "%ls", messag10);
                    attroff(COLOR_PAIR(7));
                    attron(COLOR_PAIR(1) | A_BOLD);
                    mvprintw(rows - 2, 16 + 30 + 100 - 10, "%d%%", (int)(PlayerFood * 10));
                    attroff(COLOR_PAIR(1) | A_BOLD);
                }
                if (PlayerFood >= 4 && PlayerFood < 5)
                {
                    attron(COLOR_PAIR(5));
                    wchar_t messag9[] = L"######";
                    mvprintw(rows - 2, 6 + 30 + 100 - 10, "%ls", messag9);
                    attroff(COLOR_PAIR(5));
                    attron(COLOR_PAIR(7));
                    wchar_t messag10[] = L"####";
                    mvprintw(rows - 2, 12 + 30 + 100 - 10, "%ls", messag10);
                    attroff(COLOR_PAIR(7));
                    attron(COLOR_PAIR(1) | A_BOLD);
                    mvprintw(rows - 2, 18 + 30 + 100 - 10, "%d%%", (int)(PlayerFood * 10));
                    attroff(COLOR_PAIR(1) | A_BOLD);
                }
                if (PlayerFood >= 5 && PlayerFood < 6)
                {
                    attron(COLOR_PAIR(5));
                    wchar_t messag9[] = L"######";
                    mvprintw(rows - 2, 6 + 30 + 100 - 10, "%ls", messag9);
                    attroff(COLOR_PAIR(5));
                    attron(COLOR_PAIR(7));
                    wchar_t messag10[] = L"######";
                    mvprintw(rows - 2, 12 + 30 + 100 - 10, "%ls", messag10);
                    attroff(COLOR_PAIR(7));
                    attron(COLOR_PAIR(1) | A_BOLD);
                    mvprintw(rows - 2, 20 + 30 + 100 - 10, "%d%%", (int)(PlayerFood * 10));
                    attroff(COLOR_PAIR(1) | A_BOLD);
                }
                if (PlayerFood >= 6 && PlayerFood < 7)
                {
                    attron(COLOR_PAIR(5));
                    wchar_t messag9[] = L"######";
                    mvprintw(rows - 2, 6 + 30 + 100 - 10, "%ls", messag9);
                    attroff(COLOR_PAIR(5));
                    attron(COLOR_PAIR(7));
                    wchar_t messag10[] = L"########";
                    mvprintw(rows - 2, 12 + 30 + 100 - 10, "%ls", messag10);
                    attroff(COLOR_PAIR(7));
                    attron(COLOR_PAIR(1) | A_BOLD);
                    mvprintw(rows - 2, 22 + 30 + 100 - 10, "%d%%", (int)(PlayerFood * 10));
                    attroff(COLOR_PAIR(1) | A_BOLD);
                }
            }
            else
            {
                if (PlayerFood > 0 && PlayerFood < 1)
                {
                    attron(COLOR_PAIR(5));
                    wchar_t messag9[] = L"##";
                    mvprintw(rows - 2, 6 + 30 + 100 - 10, "%ls", messag9);
                    attroff(COLOR_PAIR(5));
                    attron(COLOR_PAIR(1) | A_BOLD);
                    mvprintw(rows - 2, 10 + 30 + 100 - 10, "%d%%", (int)(PlayerFood * 10));
                    attroff(COLOR_PAIR(1) | A_BOLD);
                }
                if (PlayerFood >= 1 && PlayerFood < 2)
                {
                    attron(COLOR_PAIR(5));
                    wchar_t messag9[] = L"####";
                    mvprintw(rows - 2, 6 + 30 + 100 - 10, "%ls", messag9);
                    attroff(COLOR_PAIR(5));
                    attron(COLOR_PAIR(1) | A_BOLD);
                    mvprintw(rows - 2, 12 + 30 + 100 - 10, "%d%%", (int)(PlayerFood * 10));
                    attroff(COLOR_PAIR(1) | A_BOLD);
                }
                if (PlayerFood >= 2 && PlayerFood <= 3)
                {
                    attron(COLOR_PAIR(5));
                    wchar_t messag9[] = L"######";
                    mvprintw(rows - 2, 6 + 30 + 100 - 10, "%ls", messag9);
                    attroff(COLOR_PAIR(5));
                    attron(COLOR_PAIR(1) | A_BOLD);
                    mvprintw(rows - 2, 14 + 30 + 100 - 10, "%d%%", (int)(PlayerFood * 10));
                    attroff(COLOR_PAIR(1) | A_BOLD);
                }
            }
        }
        //
        mvprintw(rows, 0, "%ls  %d                                                                             %ls  %d     %ls  %d                                                                             %ls  %d",
                 messag1, playerGOLD, messag3, AncientKeys - UsedKey, messag4, MovesTillLastFood, messag6, level);
        attron(A_BLINK);
        mvprintw(rows - 10, 0, "Press Any Key to Continue!");
        attroff(A_BLINK);
        refresh();
        // Lose
        if (playerHP <= 0)
        {
            Lose();
        }
    } while ((c = getch()) != 27);
    Mix_HaltMusic();
    Mix_FreeMusic(music);
    music = NULL;
    SDL_Delay(1000);
    play_music("1.mp3");
    SDL_Delay(1000);
}

void Inventory()
{
}

void Fight_Room()
{
    Mix_PauseMusic();
    SDL_Delay(1000);
    play_music("4.mp3");
    SDL_Delay(1000);
    clear();
    int move_allow = 0;
    Enemycount2 = 0;
    Wasted_Weapon_count2 = 0;
    // init map with space
    for (int i = 0; i < 24; i++)
    {
        for (int j = 0; j < 24; j++)
        {
            map3[i][j] = ' ';
        }
    }
    // set borders of map
    for (int i = 0; i < 24; i++)
    {
        map3[0][i] = '#';
        map3[23][i] = '#';
        map3[i][0] = '#';
        map3[i][23] = '#';
    }
    int randomx_player = (rand() % 15) + 2;
    int randomy_player = (rand() % 15) + 2;
    Wasted_Weapon_Array2 = (Wasted_Weapon *)malloc(100 * Room_Placed * sizeof(Wasted_Weapon));
    EnemyArray2 = (Enemy *)malloc(sizeof(Enemy) * Room_Placed * 100);
    for (int i = 0; i < 30; i++)
    {
        int xe;
        int ye;
        do
        {
            xe = (rand() % 15) + 5;
            ye = (rand() % 15) + 5;
        } while ((map3[ye][xe] != ' '));
        int whih = rand() % 5;
        if (whih == 0)
        {
            EnemyArray2[Enemycount2].hp = 5;
            EnemyArray2[Enemycount2].move = 1;
            EnemyArray2[Enemycount2].type = 'd';
            map3[ye][xe] = 'd';
            EnemyArray2[Enemycount2].x = xe;
            EnemyArray2[Enemycount2].y = ye;
        }
        if (whih == 1)
        {
            EnemyArray2[Enemycount2].hp = 10;
            EnemyArray2[Enemycount2].move = 1;
            EnemyArray2[Enemycount2].type = 'f';
            map3[ye][xe] = 'f';
            EnemyArray2[Enemycount2].x = xe;
            EnemyArray2[Enemycount2].y = ye;
        }
        if (whih == 2)
        {
            EnemyArray2[Enemycount2].hp = 15;
            EnemyArray2[Enemycount2].move = 1;
            EnemyArray2[Enemycount2].type = 't';
            map3[ye][xe] = 't';
            EnemyArray2[Enemycount2].x = xe;
            EnemyArray2[Enemycount2].y = ye;
        }
        if (whih == 3)
        {
            EnemyArray2[Enemycount2].hp = 20;
            EnemyArray2[Enemycount2].move = 1;
            EnemyArray2[Enemycount2].type = 's';
            map3[ye][xe] = 's';
            EnemyArray2[Enemycount2].x = xe;
            EnemyArray2[Enemycount2].y = ye;
        }
        if (whih == 4)
        {
            EnemyArray2[Enemycount2].hp = 30;
            EnemyArray2[Enemycount2].move = 1;
            EnemyArray2[Enemycount2].type = 'u';
            map3[ye][xe] = 'u';
            EnemyArray2[Enemycount2].x = xe;
            EnemyArray2[Enemycount2].y = ye;
        }
        Enemycount2++;
    }
    int c = 113;
    do
    {
        // Lose
        if (playerHP <= 0)
        {
            Lose();
        }
        clear();
        if (c != 113)
        {
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
            //
            // movement
            if (c == 'j' && map3[randomy_player - 1][randomx_player] != '#')
            {
                move_allow++;
                randomy_player--;
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
                if (c == 'k' && map3[randomy_player + 1][randomx_player] != '#')
                {
                    move_allow++;
                    randomy_player++;
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
                    if (c == 'l' && map3[randomy_player][randomx_player + 1] != '#')
                    {
                        move_allow++;
                        if (MovesTillLastpot >= 1)
                        {
                            MovesTillLastpot--;
                        }
                        randomx_player++;
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
                        if (c == 'h' && map3[randomy_player][randomx_player - 1] != '#')
                        {
                            move_allow++;
                            if (MovesTillLastpot >= 1)
                            {
                                MovesTillLastpot--;
                            }
                            randomx_player--;
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
                            if (c == 'y' && map3[randomy_player - 1][randomx_player - 1] != '#')
                            {
                                move_allow++;
                                if (MovesTillLastpot >= 1)
                                {
                                    MovesTillLastpot--;
                                }
                                randomy_player--;
                                randomx_player--;
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
                                if (c == 'u' && map3[randomy_player - 1][randomx_player + 1] != '#')
                                {
                                    move_allow++;
                                    if (MovesTillLastpot >= 1)
                                    {
                                        MovesTillLastpot--;
                                    }
                                    randomy_player--;
                                    randomx_player++;
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
                                    if (c == 'b' && map3[randomy_player + 1][randomx_player - 1] != '#')
                                    {
                                        move_allow++;
                                        if (MovesTillLastpot >= 1)
                                        {
                                            MovesTillLastpot--;
                                        }
                                        randomx_player--;
                                        randomy_player++;
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
                                        if (c == 'n' && map3[randomy_player + 1][randomx_player + 1] != '#')
                                        {
                                            move_allow++;
                                            if (MovesTillLastpot >= 1)
                                            {
                                                MovesTillLastpot--;
                                            }
                                            randomx_player++;
                                            randomy_player++;
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
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // Farrange Fight
        if (c == ' ')
        {
            Far_Battle2(randomy_player, randomx_player);
        }
        // Battle 1-1
        if (c == 'j' && Check_Enemy2(randomy_player - 1, randomx_player) == 0)
        {
            battle2((randomy_player - 1), randomx_player);
        }
        else
        {
            if (c == 'k' && Check_Enemy2(y + 1, randomx_player) == 0)
            {
                battle2((randomy_player + 1), (randomx_player));
            }
            else
            {
                if (c == 'l' && Check_Enemy2(y, randomx_player + 1) == 0)
                {
                    battle2(randomy_player, (randomx_player + 1));
                }
                else
                {
                    if (c == 'h' && Check_Enemy2(y, randomx_player - 1) == 0)
                    {
                        battle2(randomy_player, (randomx_player - 1));
                    }
                    else
                    {
                        if (c == 'y' && Check_Enemy2(y - 1, randomx_player - 1) == 0)
                        {
                            battle2((randomy_player - 1), (randomx_player - 1));
                        }
                        else
                        {
                            if (c == 'u' && Check_Enemy2(y - 1, randomx_player + 1) == 0)
                            {
                                battle2((randomy_player - 1), (randomx_player + 1));
                            }
                            else
                            {
                                if (c == 'b' && Check_Enemy2(y + 1, randomx_player - 1) == 0)
                                {
                                    battle2((randomy_player + 1), (randomx_player - 1));
                                }
                                else
                                {
                                    if (c == 'n' && Check_Enemy2(y + 1, randomx_player + 1) == 0)
                                    {
                                        battle2((randomy_player + 1), (randomx_player + 1));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        // Enemy Move
        if (c == 'M' || c == 'm' || c == 'R' || c == 'r' || c == 'P' || c == 'p')
        {
        }
        else
        {
            Enemy_Move2(randomy_player, randomx_player);
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
        // Food Inentory
        if (c == 'E' || c == 'e')
        {
            FoodChoice();
            clear();
        }

        // Messages
        mvprintw(0, 0, "%s", Line1);
        mvprintw(1, 0, "%s", Line2);
        strcpy(Line1, "                                                                   ");
        // Stats
        wchar_t messag1[] = L"💰";
        wchar_t messag2[] = L"❤️";
        wchar_t messag3[] = L"🔑";
        wchar_t messag4[] = L"🏃";
        wchar_t messag5[] = L"🍔";
        wchar_t messag6[] = L"⏫";
        wchar_t messag7[] = L"🚀";
        wchar_t messag8[] = L"💪";
        mvprintw(rows - 2, 0, "                                                                                                                                                                                              ");
        // show HP
        // mvprintw(rows - 2, 0 + 30, "%ls  :", messag2);
        if (playerHP == 1000)
        {
            wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP > 900 && playerHP < 1000)
        {
            wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ 💔";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP == 900)
        {
            wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ 🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP > 800 && playerHP < 900)
        {
            wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ 💔🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP == 800)
        {
            wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ 🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP > 700 && playerHP < 800)
        {
            wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ 💔🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP == 700)
        {
            wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ 🤍🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP > 600 && playerHP < 700)
        {
            wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ ❤️ ❤️ 💔🤍🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP == 600)
        {
            wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ ❤️ ❤️ 🤍🤍🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP > 500 && playerHP < 600)
        {
            wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ ❤️ 💔🤍🤍🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP == 500)
        {
            wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ ❤️ 🤍🤍🤍🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP > 400 && playerHP < 500)
        {
            wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ 💔🤍🤍🤍🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP == 400)
        {
            wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ 🤍🤍🤍🤍🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP > 300 && playerHP < 400)
        {
            wchar_t messag9[] = L"❤️ ❤️ ❤️ 💔🤍🤍🤍🤍🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP == 300)
        {
            wchar_t messag9[] = L"❤️ ❤️ ❤️ 🤍🤍🤍🤍🤍🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP > 200 && playerHP < 300)
        {
            wchar_t messag9[] = L"❤️ ❤️ 💔🤍🤍🤍🤍🤍🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP == 200)
        {
            wchar_t messag9[] = L"❤️ ❤️ 🤍🤍🤍🤍🤍🤍🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP > 100 && playerHP < 200)
        {
            wchar_t messag9[] = L"❤️ 💔🤍🤍🤍🤍🤍🤍🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP == 100)
        {
            wchar_t messag9[] = L"❤️ 🤍🤍🤍🤍🤍🤍🤍🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP > 0 && playerHP < 100)
        {
            wchar_t messag9[] = L"💔🤍🤍🤍🤍🤍🤍🤍🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        if (playerHP == 0)
        {
            wchar_t messag9[] = L"🤍🤍🤍🤍🤍🤍🤍🤍🤍🤍";
            mvprintw(rows - 2, 6 + 30, "%ls", messag9);
        }
        attron(COLOR_PAIR(1) | A_BOLD);
        mvprintw(rows - 2, 28 + 30, "%d%%", playerHP / 10);
        attroff(COLOR_PAIR(1) | A_BOLD);
        // Food
        // mvprintw(rows - 2, 0 + 30 + 50 + 50 - 10, "%ls  :", messag5);
        if (PlayerFood == 10)
        {
            wchar_t messag9[] = L"🍏🍏🍏🍏🍏🍏🍏🍏🍏🍏";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood > 9 && PlayerFood < 10)
        {
            wchar_t messag9[] = L"🍏🍏🍏🍏🍏🍏🍏🍏🍏⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood == 9)
        {
            wchar_t messag9[] = L"🍏🍏🍏🍏🍏🍏🍏🍏🍏⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood > 8 && PlayerFood < 9)
        {
            wchar_t messag9[] = L"🍏🍏🍏🍏🍏🍏🍏🍏⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood == 8)
        {
            wchar_t messag9[] = L"🍏🍏🍏🍏🍏🍏🍏🍏⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood > 7 && PlayerFood < 8)
        {
            wchar_t messag9[] = L"🍏🍏🍏🍏🍏🍏🍏⚪⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood == 7)
        {
            wchar_t messag9[] = L"🍏🍏🍏🍏🍏🍏🍏⚪⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood > 6 && PlayerFood < 7)
        {
            wchar_t messag9[] = L"🍏🍏🍏🍏🍏🍏⚪⚪⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood == 6)
        {
            wchar_t messag9[] = L"🍏🍏🍏🍏🍏🍏⚪⚪⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood > 5 && PlayerFood < 6)
        {
            wchar_t messag9[] = L"🍏🍏🍏🍏🍏⚪⚪⚪⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood == 5)
        {
            wchar_t messag9[] = L"🍏🍏🍏🍏🍏⚪⚪⚪⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood > 4 && PlayerFood < 5)
        {
            wchar_t messag9[] = L"🍏🍏🍏🍏⚪⚪⚪⚪⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood == 4)
        {
            wchar_t messag9[] = L"🍏🍏🍏🍏⚪⚪⚪⚪⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood > 3 && PlayerFood < 4)
        {
            wchar_t messag9[] = L"🍏🍏🍏⚪⚪⚪⚪⚪⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood == 3)
        {
            wchar_t messag9[] = L"🍏🍏🍏⚪⚪⚪⚪⚪⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood > 2 && PlayerFood < 3)
        {
            wchar_t messag9[] = L"🍏🍏⚪⚪⚪⚪⚪⚪⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood == 2)
        {
            wchar_t messag9[] = L"🍏🍏⚪⚪⚪⚪⚪⚪⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood > 1 && PlayerFood < 2)
        {
            wchar_t messag9[] = L"🍏⚪⚪⚪⚪⚪⚪⚪⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood == 1)
        {
            wchar_t messag9[] = L"🍏⚪⚪⚪⚪⚪⚪⚪⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood > 0 && PlayerFood < 1)
        {
            wchar_t messag9[] = L"⚪⚪⚪⚪⚪⚪⚪⚪⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        if (PlayerFood == 0)
        {
            wchar_t messag9[] = L"⚪⚪⚪⚪⚪⚪⚪⚪⚪⚪";
            mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
        }
        attron(COLOR_PAIR(1) | A_BOLD);
        mvprintw(rows - 2, 28 + 30 + 50 + 50 - 10, "%d%%", (int)(PlayerFood * 10));
        attroff(COLOR_PAIR(1) | A_BOLD);
        //
        mvprintw(rows + 1, 0, "%ls  %d                           %d                                                 %ls  %d                                                                              %ls  %d",
                 messag1, playerGOLD, move_allow, messag4, MovesTillLastFood, messag6, level);

        // print map
        for (int i = 0; i < 24; i++)
        {
            for (int j = 0; j < 24; j++)
            {
                mvprintw(i, j, "%c", map3[i][j]);
            }
            printw("\n");
        }
        // print enemy
        for (int carol = 0; carol < Enemycount2; carol++)
        {
            if (EnemyArray2[carol].hp > 0)
            {
                if (EnemyArray2[carol].type == 'd')
                {
                    mvaddch(EnemyArray2[carol].y, EnemyArray2[carol].x, 'D');
                }
                if (EnemyArray2[carol].type == 'f')
                {
                    mvaddch(EnemyArray2[carol].y, EnemyArray2[carol].x, 'F');
                }
                if (EnemyArray2[carol].type == 't')
                {
                    mvaddch(EnemyArray2[carol].y, EnemyArray2[carol].x, 'G');
                }
                if (EnemyArray2[carol].type == 's')
                {
                    mvaddch(EnemyArray2[carol].y, EnemyArray2[carol].x, 'S');
                }
                if (EnemyArray2[carol].type == 'u')
                {
                    mvaddch(EnemyArray2[carol].y, EnemyArray2[carol].x, 'U');
                }
            }
        }
        // print player
        mvaddch(randomy_player, randomx_player, '@');
        attron(A_BLINK);
        mvprintw(rows - 10, 0, "Press Escape to go back!");
        attroff(A_BLINK);
        refresh();
        // Lose
        if (playerHP <= 0)
        {
            Lose();
        }
    } while ((c = getch()) != 27 || move_allow < 10);
    free(EnemyArray2);
    free(Wasted_Weapon_Array2);
    clear();
    Mix_PauseMusic();
    SDL_Delay(1000);
    play_music("1.mp3");
    SDL_Delay(1000);
    if(map[y-1][x] == ' ')
    {
        y--;
    }
    else
    {
        if (map[y + 1][x] == ' ')
        {
            y++;
        }
        else
        {
            if (map[y][x - 1] == ' ')
            {
                x--;
            }
            else
            {
                if (map[y][x + 1] == ' ')
                {
                    x++;
                }
            }
        }
    }
}

struct timeval start, end;
int MakeGame(int c)
{
    // bug fix spped
    if (speed <= 0)
    {
        speed = 0;
    }
    // Generate Map
    if (Room_Placed == 0)
    {
        clear();
        MakeRoom();
    }
    // New Room message
    if (c == 113 && lood != 1)
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
    if (1)
    {
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
    if (playerHP <= 0)
    {
        Lose();
    }
    // Farrange Fight
    if (c == ' ')
    {
        Far_Battle();
    }
    // Inventory
    /*if(c == 'G' || c == 'g')
    {
        Inventory();
    }*/
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
                        UsedKey++;
                        AncientKey[ii] = -1;
                    }
                    else
                    {
                        keypass = 1;
                        UsedKey++;
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
                        UsedKey++;
                        AncientKey[ii] = -1;
                    }
                    else
                    {
                        UsedKey++;
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
                        UsedKey++;
                        AncientKey[ii] = -1;
                    }
                    else
                    {
                        UsedKey++;
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
                        UsedKey++;
                        AncientKey[ii] = -1;
                    }
                    else
                    {
                        UsedKey++;
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
                        UsedKey++;
                        AncientKey[ii] = -1;
                    }
                    else
                    {
                        UsedKey++;
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
                        UsedKey++;
                        AncientKey[ii] = -1;
                    }
                    else
                    {
                        UsedKey++;
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
                        UsedKey++;
                        AncientKey[ii] = -1;
                    }
                    else
                    {
                        UsedKey++;
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
                        UsedKey++;
                        AncientKey[ii] = -1;
                    }
                    else
                    {
                        UsedKey++;
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
                    if ((map[i][x] == ' ' && map[i - 1][x] != ' ') && map[i - 1][x] != 'd' && map[i - 1][x] != 'f' && map[i - 1][x] != 't' && map[i - 1][x] != 's' && map[i - 1][x] != 'u')
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
                    if ((map[i][x] == ' ' && map[i + 1][x] != ' ') && map[i + 1][x] != 'd' && map[i + 1][x] != 'f' && map[i + 1][x] != 't' && map[i + 1][x] != 's' && map[i + 1][x] != 'u')
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
                    if ((map[y][i] == ' ' && map[y][i + 1] != ' ') && map[y][i + 1] != 'd' && map[y][i + 1] != 'f' && map[y][i + 1] != 't' && map[y][i + 1] != 's' && map[y][i + 1] != 'u')
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
                    if ((map[y][i] == ' ' && map[y][i - 1] != ' ') && map[y][i - 1] != 'd' && map[y][i - 1] != 'f' && map[y][i - 1] != 't' && map[y][i - 1] != 's' && map[y][i - 1] != 'u')
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
    // Diactivate enemys in rooms that are not visited yet
    for (int i = 0; i < Room_Placed; i++)
    {
        if (RoomArray[i].show == false)
        {
            for (int j = 0; j < EnemyCount; j++)
            {
                if (EnemyArray[j].y >= RoomArray[i].TopLeft_Y && EnemyArray[j].y <= RoomArray[i].TopLeft_Y + RoomArray[i].Height - 1 &&
                    EnemyArray[j].x >= RoomArray[i].TopLeft_x && EnemyArray[j].x <= RoomArray[i].TopLeft_x + RoomArray[i].Width - 1)
                {
                    if (EnemyArray[j].hp > 0 && EnemyArray[j].move != -1)
                    {
                        EnemyArray[j].move = 0;
                    }
                }
            }
        }
    }
    // step on door(show room) and Diactivate Enemy if we step on door
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
    // Activate Enemy in the room we are in
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
        }
    }
    // Enemy Move
    if (speed == 0 || ((int)(speed * 10)) % 2 == 1)
    {
        if (c == 'M' || c == 'm' || c == 'R' || c == 'r' || c == 'P' || c == 'p')
        {
        }
        else
        {
            Enemy_Move();
        }
    }
    // Save
    if (c == '`')
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
            Won();
        }
        if (c == KEY_RIGHT && level != 1)
        {
            LoadGame_StairCase();
        }
    }
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
        Fight_Room();
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
            {mvprintw(0,0," PassWord Was Wrong! You have %d trys left!", DoorArray[makeriaziyekobastim].trysleft);
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
                            mvprintw(0,0," PassWord Was Wrong! You have %d trys left!", DoorArray[makeriaziyekobastim].trysleft);
                            char ll = getch();
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
                            mvprintw(0,0," PassWord Was Wrong! You have %d trys left!", DoorArray[makeriaziyekobastim].trysleft);
                            char ll = getch();
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
            for (int i = 0; i < Wasted_Weapon_Count; i++)
            {
                if (Wasted_Weapon_Array[i].x == x && Wasted_Weapon_Array[i].y == y)
                {
                    haha = 1;
                    break;
                }
            }
            if (haha == 0)
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
            for (int i = 0; i < Wasted_Weapon_Count; i++)
            {
                if (Wasted_Weapon_Array[i].x == x && Wasted_Weapon_Array[i].y == y)
                {
                    haha = 1;
                    break;
                }
            }
            if (haha == 0)
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
            for (int i = 0; i < Wasted_Weapon_Count; i++)
            {
                if (Wasted_Weapon_Array[i].x == x && Wasted_Weapon_Array[i].y == y)
                {
                    haha = 1;
                    break;
                }
            }
            if (haha == 0)
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
    // Hiden Door
    if (map[y][x] == 'H')
    {
        hiden_door_show = 1;
        if (YesNoChoice_H())
        {
            clear();
            Telesm_Room();
            clear();
        }
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
                if (RoomArray[roomindex].kaboos == 22)
                {
                    check = 5;
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
        if ((RoomArray[index].show == true || m == 1 || RoomArray[index].wdow == 2) && RoomArray[index].kaboos == 22)
        {
            for (int yy = (RoomArray[index].TopLeft_Y - 1); yy <= RoomArray[index].TopLeft_Y + RoomArray[index].Height; yy++)
            {
                for (int xx = (RoomArray[index].TopLeft_x - 1); xx <= RoomArray[index].TopLeft_x + RoomArray[index].Width; xx++)
                {
                    if (yy >= RoomArray[index].TopLeft_Y && yy <= (RoomArray[index].TopLeft_Y + RoomArray[index].Height - 1) &&
                        xx >= RoomArray[index].TopLeft_x && xx <= (RoomArray[index].TopLeft_x + RoomArray[index].Width - 1))
                    {
                        mvaddch(yy, xx, ' ');
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
                                if (map[yy][xx] == 'H' && hiden_door_show == 1)
                                {
                                    attron(COLOR_PAIR(2));
                                    mvaddch(yy, xx, 'H');
                                    attroff(COLOR_PAIR(2));
                                }
                                else
                                {
                                    attron(COLOR_PAIR(7));
                                    wchar_t message[] = L"≡";
                                    mvprintw(yy, xx, "%ls", message);
                                    attroff(COLOR_PAIR(7));
                                }
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
                                if (map[yy][xx] == 'H' && hiden_door_show == 1)
                                {
                                    attron(COLOR_PAIR(2));
                                    mvaddch(yy, xx, 'H');
                                    attroff(COLOR_PAIR(2));
                                }
                                else
                                {
                                    attron(COLOR_PAIR(7));
                                    // ‖⫼
                                    wchar_t message[] = L"‖";
                                    mvprintw(yy, xx, "%ls", message);
                                    attroff(COLOR_PAIR(7));
                                }
                            }
                        }
                        // window∅
                        for (int iji = 0; iji < Window_Placed; iji++)
                        {
                            if (WindowArray[iji].x == xx && WindowArray[iji].y == yy)
                            {
                                attron(COLOR_PAIR(4));
                                // ⊞
                                wchar_t message[] = L"⊞";
                                mvprintw(yy, xx, "%ls", message);
                                attroff(COLOR_PAIR(4));
                            }
                        }
                    }
                }
            }
            for (int yy = (RoomArray[index].TopLeft_Y - 1); yy <= RoomArray[index].TopLeft_Y + RoomArray[index].Height; yy++)
            {
                for (int xx = (RoomArray[index].TopLeft_x - 1); xx <= RoomArray[index].TopLeft_x + RoomArray[index].Width; xx++)
                {
                    if ((yy - y) * (yy - y) + (xx - x) * (xx - x) <= 8)
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
                                            // ∙
                                            attron(COLOR_PAIR(2));
                                            wchar_t message[] = TRP;
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
                                                // ⛀
                                                wchar_t message[] = L"✧";
                                                mvprintw(yy, xx, "%ls", message);
                                                attroff(COLOR_PAIR(7));
                                            }
                                            else
                                            {
                                                if (map[yy][xx] == 'g')
                                                {
                                                    attron(COLOR_PAIR(7));
                                                    wchar_t message[] = L"🞙";
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
                                    if (map[yy][xx] == 'H' && hiden_door_show == 1)
                                    {
                                        attron(COLOR_PAIR(2));
                                        mvaddch(yy, xx, 'H');
                                        attroff(COLOR_PAIR(2));
                                    }
                                    else
                                    {
                                        attron(COLOR_PAIR(7));
                                        wchar_t message[] = L"≡";
                                        mvprintw(yy, xx, "%ls", message);
                                        attroff(COLOR_PAIR(7));
                                    }
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
                                    if (map[yy][xx] == 'H' && hiden_door_show == 1)
                                    {
                                        attron(COLOR_PAIR(2));
                                        mvaddch(yy, xx, 'H');
                                        attroff(COLOR_PAIR(2));
                                    }
                                    else
                                    {
                                        attron(COLOR_PAIR(7));
                                        // ‖⫼
                                        wchar_t message[] = L"‖";
                                        mvprintw(yy, xx, "%ls", message);
                                        attroff(COLOR_PAIR(7));
                                    }
                                }
                            }
                            // window∅
                            for (int iji = 0; iji < Window_Placed; iji++)
                            {
                                if (WindowArray[iji].x == xx && WindowArray[iji].y == yy)
                                {
                                    attron(COLOR_PAIR(4));
                                    // ⊞
                                    wchar_t message[] = L"⊞";
                                    mvprintw(yy, xx, "%ls", message);
                                    attroff(COLOR_PAIR(4));
                                }
                            }
                        }
                    }
                }
            }
        }
        else
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
                                            // ∙
                                            attron(COLOR_PAIR(2));
                                            wchar_t message[] = TRP;
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
                                                // ⛀
                                                wchar_t message[] = L"✧";
                                                mvprintw(yy, xx, "%ls", message);
                                                attroff(COLOR_PAIR(7));
                                            }
                                            else
                                            {
                                                if (map[yy][xx] == 'g')
                                                {
                                                    attron(COLOR_PAIR(7));
                                                    wchar_t message[] = L"🞙";
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
                                    if (map[yy][xx] == 'H' && hiden_door_show == 1)
                                    {
                                        attron(COLOR_PAIR(2));
                                        mvaddch(yy, xx, 'H');
                                        attroff(COLOR_PAIR(2));
                                    }
                                    else
                                    {
                                        attron(COLOR_PAIR(7));
                                        wchar_t message[] = L"≡";
                                        mvprintw(yy, xx, "%ls", message);
                                        attroff(COLOR_PAIR(7));
                                    }
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
                                    if (map[yy][xx] == 'H' && hiden_door_show == 1)
                                    {
                                        attron(COLOR_PAIR(2));
                                        mvaddch(yy, xx, 'H');
                                        attroff(COLOR_PAIR(2));
                                    }
                                    else
                                    {
                                        attron(COLOR_PAIR(7));
                                        // ‖⫼
                                        wchar_t message[] = L"‖";
                                        mvprintw(yy, xx, "%ls", message);
                                        attroff(COLOR_PAIR(7));
                                    }
                                }
                            }
                            // window∅
                            for (int iji = 0; iji < Window_Placed; iji++)
                            {
                                if (WindowArray[iji].x == xx && WindowArray[iji].y == yy)
                                {
                                    attron(COLOR_PAIR(4));
                                    // ⊞
                                    wchar_t message[] = L"⊞";
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
                                                // ∙
                                                attron(COLOR_PAIR(2));
                                                wchar_t message[] = TRP;
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
                                                    wchar_t message[] = L"✧";
                                                    mvprintw(yy, xx, "%ls", message);
                                                    attroff(COLOR_PAIR(7));
                                                }
                                                else
                                                {
                                                    if (map[yy][xx] == 'g')
                                                    {
                                                        attron(COLOR_PAIR(7));
                                                        wchar_t message[] = L"🞙";
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
                                        wchar_t message[] = L"⊞";
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
        // ◉🞹▣
        wchar_t message[] = L"⍟";
        mvprintw(y, x, "%ls", message);
        attroff(COLOR_PAIR(5) | A_BOLD);
    }
    if (color == 1)
    {
        attron(COLOR_PAIR(1) | A_BOLD);
        wchar_t message[] = L"⍟";
        mvprintw(y, x, "%ls", message);
        attroff(COLOR_PAIR(1) | A_BOLD);
    }
    if (color == 2)
    {
        attron(COLOR_PAIR(2) | A_BOLD);
        wchar_t message[] = L"⍟";
        mvprintw(y, x, "%ls", message);
        attroff(COLOR_PAIR(2) | A_BOLD);
    }
    if (color == 3)
    {
        attron(COLOR_PAIR(7) | A_BOLD);
        wchar_t message[] = L"⍟";
        mvprintw(y, x, "%ls", message);
        attroff(COLOR_PAIR(7) | A_BOLD);
    }
    if (color == 4)
    {
        attron(COLOR_PAIR(4) | A_BOLD);
        wchar_t message[] = L"⍟";
        mvprintw(y, x, "%ls", message);
        attroff(COLOR_PAIR(4) | A_BOLD);
    }
    if (color == 5)
    {
        attron(COLOR_PAIR(3) | A_BOLD);
        wchar_t message[] = L"⍟";
        mvprintw(y, x, "%ls", message);
        attroff(COLOR_PAIR(3) | A_BOLD);
    }
    // Messages
    mvprintw(0, 0, "%s", Line1);
    mvprintw(1, 0, "%s", Line2);
    strcpy(Line1, "                                                                   ");
    // Stats
    wchar_t messag1[] = L"💰";
    wchar_t messag2[] = L"❤️";
    wchar_t messag3[] = L"🔑";
    wchar_t messag4[] = L"🏃";
    wchar_t messag5[] = L"🍔";
    wchar_t messag6[] = L"⏫";
    wchar_t messag7[] = L"🚀";
    wchar_t messag8[] = L"💪";
    mvprintw(rows - 2, 0, "                                                                                                                                                                                              ");
    // show HP
    // mvprintw(rows - 2, 0 + 30, "%ls  :", messag2);
    if (playerHP == 1000)
    {
        wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️";
        mvprintw(rows - 2, 6 + 30, "%ls", messag9);
    }
    if (playerHP > 900 && playerHP < 1000)
    {
        wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ 💔";
        mvprintw(rows - 2, 6 + 30, "%ls", messag9);
    }
    if (playerHP == 900)
    {
        wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ 🤍";
        mvprintw(rows - 2, 6 + 30, "%ls", messag9);
    }
    if (playerHP > 800 && playerHP < 900)
    {
        wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ 💔🤍";
        mvprintw(rows - 2, 6 + 30, "%ls", messag9);
    }
    if (playerHP == 800)
    {
        wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ 🤍🤍";
        mvprintw(rows - 2, 6 + 30, "%ls", messag9);
    }
    if (playerHP > 700 && playerHP < 800)
    {
        wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ 💔🤍🤍";
        mvprintw(rows - 2, 6 + 30, "%ls", messag9);
    }
    if (playerHP == 700)
    {
        wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ ❤️ ❤️ ❤️ 🤍🤍🤍";
        mvprintw(rows - 2, 6 + 30, "%ls", messag9);
    }
    if (playerHP > 600 && playerHP < 700)
    {
        wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ ❤️ ❤️ 💔🤍🤍🤍";
        mvprintw(rows - 2, 6 + 30, "%ls", messag9);
    }
    if (playerHP == 600)
    {
        wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ ❤️ ❤️ 🤍🤍🤍🤍";
        mvprintw(rows - 2, 6 + 30, "%ls", messag9);
    }
    if (playerHP > 500 && playerHP < 600)
    {
        wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ ❤️ 💔🤍🤍🤍🤍";
        mvprintw(rows - 2, 6 + 30, "%ls", messag9);
    }
    if (playerHP == 500)
    {
        wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ ❤️ 🤍🤍🤍🤍🤍";
        mvprintw(rows - 2, 6 + 30, "%ls", messag9);
    }
    if (playerHP > 400 && playerHP < 500)
    {
        wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ 💔🤍🤍🤍🤍🤍";
        mvprintw(rows - 2, 6 + 30, "%ls", messag9);
    }
    if (playerHP == 400)
    {
        wchar_t messag9[] = L"❤️ ❤️ ❤️ ❤️ 🤍🤍🤍🤍🤍🤍";
        mvprintw(rows - 2, 6 + 30, "%ls", messag9);
    }
    if (playerHP > 300 && playerHP < 400)
    {
        wchar_t messag9[] = L"❤️ ❤️ ❤️ 💔🤍🤍🤍🤍🤍🤍";
        mvprintw(rows - 2, 6 + 30, "%ls", messag9);
    }
    if (playerHP == 300)
    {
        wchar_t messag9[] = L"❤️ ❤️ ❤️ 🤍🤍🤍🤍🤍🤍🤍";
        mvprintw(rows - 2, 6 + 30, "%ls", messag9);
    }
    if (playerHP > 200 && playerHP < 300)
    {
        wchar_t messag9[] = L"❤️ ❤️ 💔🤍🤍🤍🤍🤍🤍🤍";
        mvprintw(rows - 2, 6 + 30, "%ls", messag9);
    }
    if (playerHP == 200)
    {
        wchar_t messag9[] = L"❤️ ❤️ 🤍🤍🤍🤍🤍🤍🤍🤍";
        mvprintw(rows - 2, 6 + 30, "%ls", messag9);
    }
    if (playerHP > 100 && playerHP < 200)
    {
        wchar_t messag9[] = L"❤️ 💔🤍🤍🤍🤍🤍🤍🤍🤍";
        mvprintw(rows - 2, 6 + 30, "%ls", messag9);
    }
    if (playerHP == 100)
    {
        wchar_t messag9[] = L"❤️ 🤍🤍🤍🤍🤍🤍🤍🤍🤍";
        mvprintw(rows - 2, 6 + 30, "%ls", messag9);
    }
    if (playerHP > 0 && playerHP < 100)
    {
        wchar_t messag9[] = L"💔🤍🤍🤍🤍🤍🤍🤍🤍🤍";
        mvprintw(rows - 2, 6 + 30, "%ls", messag9);
    }
    if (playerHP == 0)
    {
        wchar_t messag9[] = L"🤍🤍🤍🤍🤍🤍🤍🤍🤍🤍";
        mvprintw(rows - 2, 6 + 30, "%ls", messag9);
    }
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(rows - 2, 28 + 30, "%d%%", playerHP / 10);
    attroff(COLOR_PAIR(1) | A_BOLD);
    // Food
    // mvprintw(rows - 2, 0 + 30 + 50 + 50 - 10, "%ls  :", messag5);
    if (PlayerFood == 10)
    {
        wchar_t messag9[] = L"🍏🍏🍏🍏🍏🍏🍏🍏🍏🍏";
        mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
    }
    if (PlayerFood > 9 && PlayerFood < 10)
    {
        wchar_t messag9[] = L"🍏🍏🍏🍏🍏🍏🍏🍏🍏⚪";
        mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
    }
    if (PlayerFood == 9)
    {
        wchar_t messag9[] = L"🍏🍏🍏🍏🍏🍏🍏🍏🍏⚪";
        mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
    }
    if (PlayerFood > 8 && PlayerFood < 9)
    {
        wchar_t messag9[] = L"🍏🍏🍏🍏🍏🍏🍏🍏⚪⚪";
        mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
    }
    if (PlayerFood == 8)
    {
        wchar_t messag9[] = L"🍏🍏🍏🍏🍏🍏🍏🍏⚪⚪";
        mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
    }
    if (PlayerFood > 7 && PlayerFood < 8)
    {
        wchar_t messag9[] = L"🍏🍏🍏🍏🍏🍏🍏⚪⚪⚪";
        mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
    }
    if (PlayerFood == 7)
    {
        wchar_t messag9[] = L"🍏🍏🍏🍏🍏🍏🍏⚪⚪⚪";
        mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
    }
    if (PlayerFood > 6 && PlayerFood < 7)
    {
        wchar_t messag9[] = L"🍏🍏🍏🍏🍏🍏⚪⚪⚪⚪";
        mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
    }
    if (PlayerFood == 6)
    {
        wchar_t messag9[] = L"🍏🍏🍏🍏🍏🍏⚪⚪⚪⚪";
        mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
    }
    if (PlayerFood > 5 && PlayerFood < 6)
    {
        wchar_t messag9[] = L"🍏🍏🍏🍏🍏⚪⚪⚪⚪⚪";
        mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
    }
    if (PlayerFood == 5)
    {
        wchar_t messag9[] = L"🍏🍏🍏🍏🍏⚪⚪⚪⚪⚪";
        mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
    }
    if (PlayerFood > 4 && PlayerFood < 5)
    {
        wchar_t messag9[] = L"🍏🍏🍏🍏⚪⚪⚪⚪⚪⚪";
        mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
    }
    if (PlayerFood == 4)
    {
        wchar_t messag9[] = L"🍏🍏🍏🍏⚪⚪⚪⚪⚪⚪";
        mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
    }
    if (PlayerFood > 3 && PlayerFood < 4)
    {
        wchar_t messag9[] = L"🍏🍏🍏⚪⚪⚪⚪⚪⚪⚪";
        mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
    }
    if (PlayerFood == 3)
    {
        wchar_t messag9[] = L"🍏🍏🍏⚪⚪⚪⚪⚪⚪⚪";
        mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
    }
    if (PlayerFood > 2 && PlayerFood < 3)
    {
        wchar_t messag9[] = L"🍏🍏⚪⚪⚪⚪⚪⚪⚪⚪";
        mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
    }
    if (PlayerFood == 2)
    {
        wchar_t messag9[] = L"🍏🍏⚪⚪⚪⚪⚪⚪⚪⚪";
        mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
    }
    if (PlayerFood > 1 && PlayerFood < 2)
    {
        wchar_t messag9[] = L"🍏⚪⚪⚪⚪⚪⚪⚪⚪⚪";
        mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
    }
    if (PlayerFood == 1)
    {
        wchar_t messag9[] = L"🍏⚪⚪⚪⚪⚪⚪⚪⚪⚪";
        mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
    }
    if (PlayerFood > 0 && PlayerFood < 1)
    {
        wchar_t messag9[] = L"⚪⚪⚪⚪⚪⚪⚪⚪⚪⚪";
        mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
    }
    if (PlayerFood == 0)
    {
        wchar_t messag9[] = L"⚪⚪⚪⚪⚪⚪⚪⚪⚪⚪";
        mvprintw(rows - 2, 6 + 30 + 50 + 50 - 10, "%ls", messag9);
    }
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(rows - 2, 28 + 30 + 50 + 50 - 10, "%d%%", (int)(PlayerFood * 10));
    attroff(COLOR_PAIR(1) | A_BOLD);
    //
    mvprintw(rows + 1, 0, "%ls  %d                                                                                 %ls  %d                                                                              %ls  %d",
             messag1, playerGOLD, messag4, MovesTillLastFood, messag6, level);
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
            if (((int)(speed * 10)) % 2 == 0 && speed > 0)
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
    int c = 113;
    keypad(stdscr, true);
    noecho();
    do
    {
        // Lose
        if (playerHP <= 0)
        {
            Lose();
        }
        MakeGame(c);
        refresh();
        // Lose
        if (playerHP <= 0)
        {
            Lose();
        }
    } while ((c = getch()) != 27);
    endwin();
    exit(0);
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

void Profile()
{
    if (guesting == 0)
    {
        FILE *filer = fopen("Users.txt", "r");
        if (filer == NULL) {
        perror("Error: Cannot open file Users.txt");  // Print the error message
        char apwiduapuoi = getch();
        return;
        }
        int flag = 0;
        char line[100];
        char name[100], pass[100], mail[100];
        while (fgets(line, sizeof(line), filer))
        {
            sscanf(line, "%49[^-]-%49[^-]-%49s", name, pass, mail);

            if (strcmp(name, Player_UserName) == 0)
            {
                strcpy(pass, pass);
                strcpy(mail, mail);
                flag = 1;
                fclose(filer);
                break;
            }
        }
        if (flag == 0)
        {
            fclose(filer);
        }
        printw("--------------------------------------\n");
        printw("|               Profile              |\n");
        printw("|                                    |\n");
        printw("--------------------------------------\n");
        printw("|                                    |\n");
        printw("|        UserName:  %13s    |\n", Player_UserName);
        printw("|                                    |\n");
        printw("|                                    |\n");
        printw("|        Email:  %16s    |\n", mail);
        printw("|                                    |\n");
        printw("|                                    |\n");
        printw("|        PassWord:  %13s    |\n", pass);
        printw("|                                    |\n");
        printw("--------------------------------------\n");
        char an = getch();
    }
    clear();
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
    colss -= 10;
    int u = 0;
    do
    {
        clear();
        refresh();
        attron(A_BOLD);
        wchar_t message1[] = L"╔══════════════════════════════╗";
        mvprintw(4, colss - 10, "%ls", message1);
        wchar_t message2[] = L"║           Game Menu          ║";
        mvprintw(5, colss - 10, "%ls", message2);
        wchar_t message3[] = L"╚══════════════════════════════╝";
        mvprintw(6, colss - 10, "%ls", message3);
        attroff(A_BOLD);

        if (cursor == 0)
        {
            wchar_t message[] = L"🎮 ";
            mvprintw(9, colss - 5, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(9, colss + 4 + 2 - 5, "Create a New Game");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"🎮 ";
            mvprintw(9, colss - 5, "%ls Create a New Game", message);
        }

        if (cursor == 1)
        {
            wchar_t message[] = L"📜 ";
            mvprintw(11, colss - 5, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(11, colss + 4 + 2 - 5, "Continue the last Game");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"📜 ";
            mvprintw(11, colss - 5, "%ls Continue the last Game", message);
        }

        if (cursor == 2)
        {
            wchar_t message[] = L"📊 ";
            mvprintw(13, colss - 5, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(13, colss + 4 + 2 - 5, "Scores Table");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"📊 ";
            mvprintw(13, colss - 5, "%ls Scores Table", message);
        }

        if (cursor == 3)
        {
            wchar_t message[] = L"⚙️ ";
            mvprintw(15, colss - 5, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(15, colss + 4 + 2 - 5, "Settings");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"⚙️ ";
            mvprintw(15, colss - 5, "%ls Settings", message);
        }

        if (cursor == 4)
        {
            wchar_t message[] = L"👤 ";
            mvprintw(17, colss - 5, "%ls", message);
            attron(COLOR_PAIR(77) | A_BOLD);
            mvprintw(17, colss + 4 + 2 - 5, "Profile");
            attroff(COLOR_PAIR(77) | A_BOLD);
        }
        else
        {
            wchar_t message[] = L"👤 ";
            mvprintw(17, colss - 5, "%ls Profile", message);
        }
        c = getch();

        if (c == KEY_UP)
        {
            cursor--;
            if (cursor < 0)
                cursor = 4;
        }

        if (c == KEY_DOWN)
        {
            cursor++;
            if (cursor > 4)
                cursor = 0;
        }

    } while (c != 10);
    // curs_set(1);
    // echo();
    if (cursor == 0)
    {
        clear();
        play_music("1.mp3");
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
    if (cursor == 4)
    {
        clear();
        Profile();
        BeforeGame();
    }
}

int main()
{
    setlocale(LC_ALL, "");
    srand(time(0));
    initscr();
    start_color();
    
    init_pair(1, COLOR_BLUE, COLOR_BLACK);


    init_pair(2, COLOR_GREEN, COLOR_BLACK);


    init_pair(3, COLOR_CYAN, COLOR_BLACK);


    init_pair(4, COLOR_MAGENTA, COLOR_BLACK);


    init_pair(5, COLOR_RED, COLOR_BLACK);


    init_pair(6, COLOR_WHITE, COLOR_BLACK);


    init_pair(7, COLOR_YELLOW, COLOR_BLACK);


    init_pair(11, COLOR_BLUE, COLOR_WHITE);


    init_pair(22, COLOR_GREEN, COLOR_WHITE);


    init_pair(33, COLOR_CYAN, COLOR_WHITE);


    init_pair(44, COLOR_MAGENTA, COLOR_WHITE);


    init_pair(55, COLOR_RED, COLOR_WHITE);


    init_pair(66, COLOR_WHITE, COLOR_WHITE);


    init_pair(77, COLOR_YELLOW, COLOR_WHITE);

    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return 1;
    }

    Rogue_Animation();
    MainMenu();
    BeforeGame();
    refresh();
    endwin();
    return 0;
}