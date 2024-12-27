#include <stdio.h>
#include <ncurses.h>
#include <string.h>
char LoginorRegister()
{
    printw("Do you want to login or register?\n");
    printw("L for login-R for register\n");
    char input;
    scanw("%c",&input);
    if(input == 'L' || input == 'l')
    {
        clear();
        return 'L';
    }
    else
    {
        if(input == 'R' || input == 'r')
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
void Login()
{

}
void Register()
{
    char Username[100];
    char Password[100];
    char Password_copy[100];
    char Email[100];
    printw("Fill the Following information to Register\n");
    printw("Your password should match these conditions: \n");
    printw("1-Password should be at-least 7 characters\n");
    printw("2-Password should have a lower-case alphabet and an upper-case alphabet and at-least one number\n");
    printw("Username: ");
    scanw("%s",Username);
    printw("Password: ");
    scanw("%s",Password);
    printw("Password Again: ");
    scanw("%s",Password_copy);
    printw("Email: ");
    scanw("%s",Email);
    int length = 0;
    int number = 0;
    int lower = 0;
    int upper = 0;
    int same = 0;
    int emailemail = 0; 
    for(int i = 0;i<strlen(Email);i++)
    {
        if(Email[i] == '@')
        {
            emailemail = 1;
        }
    } 
    if(strlen(Password) >= 7)
    {
        length = 1;
    }
    else
    {
        length = 0;
    }
    if(strcmp(Password,Password_copy) == 0)
    {
        same = 1;
    }
    else
    {
        same = 0;
    }
    for(int i = 0 ; i < strlen(Password);i++)
    {
        if(Password[i] >= '0' && Password[i] <= '9')
        {
            number++;
        }
        if(Password[i] >= 'A' && Password[i] <= 'Z')
        {
            upper++;
        }
        if(Password[i] >= 'a' && Password[i] <= 'z')
        {
            lower++;
        }
    }
    //printw("%d %d %d %d %d %d\n",length,same,upper,lower,number,email);
    if(length * same * upper * lower * number * emailemail > 0)
    {
        clear();
        printw("Welcome!\n");
    }
    else
    {
        clear();
        Register();
    }
}
int main()
{
    initscr();
    char LogorRegi;
    while(1)
    {
        LogorRegi = LoginorRegister();
        if(LogorRegi == 'R' || LogorRegi == 'L')  
        {
            printw("Wellcome\n");
            break;
        }  
    }
    if(LogorRegi == 'R')
    {
        Register();
    }
    else
    {
        Login();
    }
    
    char c = getch();
    refresh();
    endwin();
    return 0;
}