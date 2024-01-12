#include <fstream>
#include <winbgim.h>
#include <windows.h>
#include <string.h>

using namespace std;

#define MAX 20
#define latime 75

#define SUS 72
#define JOS 80
#define STG 75
#define DRP 77
#define ESC 27
#define SPC 32
#define TAB 9
#define BKS 8
#define SPACE 32

int imario, jmario, ipostaza;
char tasta;
string directie;
bool inJump = false;
int jumpHeight = 0;



int scor, nrstelute;

int nrLinii, nrColoane, i, j;
char harta[MAX][MAX];
char car;

ifstream fisier("harta.txt");
ofstream gameLog("gameLog.txt");

void showEndGameMenu();

void afiseazaMario()
{
    int y = imario * latime, x = jmario * latime;

    switch (ipostaza) {
    case -10: readimagefile("mario_spate1.jpg", x, y, x + latime, y + latime); break;
    case 10: readimagefile("mario_spate2.jpg", x, y, x + latime, y + latime); break;
    case 1:
        readimagefile("mario11.jpg", x, y, x + latime, y + latime); break;
    case 2:
        readimagefile("mario22.jpg", x, y, x + latime, y + latime); break;
    case -1:
        readimagefile("mario55.jpg", x, y, x + latime, y + latime); break;
    case -2:
        readimagefile("mario66.jpg", x, y, x + latime, y + latime); break;
    }
}

void afiseazaPoza(char c, int i, int j)
{
    if (c == '@')
        readimagefile("iarba.jpg", latime * j, latime * i, latime * (j + 1), latime * (i + 1));
    if (c == '#')
        readimagefile("scara.jpg", latime * j, latime * i, latime * (j + 1), latime * (i + 1));
    if (c == '*')
        readimagefile("stea.jpg", latime * j, latime * i, latime * (j + 1), latime * (i + 1));
    if (c == '.')
        readimagefile("fundal.jpg", latime * j, latime * i, latime * (j + 1), latime * (i + 1));
    if (c == ',')
        readimagefile("fundal2.jpg", latime * j, latime * i, latime * (j + 1), latime * (i + 1));
    if (c == '!')
        readimagefile("fundal3.jpg", latime * j, latime * i, latime * (j + 1), latime * (i + 1));
    if (c == '?')
        readimagefile("fundal4.jpg", latime * j, latime * i, latime * (j + 1), latime * (i + 1));
}

void stergeMario()
{
    afiseazaPoza(harta[imario][jmario], imario, jmario);
}

void afiseazaScor()
{
    int i;
    for (i = 1; i <= scor; i++)
        readimagefile("stea.jpg", 30 * i, 0, 30 * i + 30, 30);
    afiseazaMario();
    if (scor < nrstelute)
    {
        for (i = 800; i <= 1200; i += 30) Beep(i, 20);
    }
    else
    {
        //PlaySound("7.wav", NULL, SND_SYNC);
        //PlaySound("8.wav", NULL, SND_SYNC);
        showEndGameMenu();
        closegraph();
        exit(0);
    }
}

void urmatoareaIpostaza() {
    stergeMario();

    // Verifica gorizontal pozitia
    if (directie == "stanga" && jmario > 0 && harta[imario][jmario - 1] != '@') {
        jmario--;
        if (abs(ipostaza) == 10) ipostaza = -1;
        if (ipostaza > 0) ipostaza = -ipostaza;
        ipostaza--; if (ipostaza == -2) ipostaza = -1;
    } else if (directie == "dreapta" && jmario < nrColoane - 1 && harta[imario][jmario + 1] != '@') {
        jmario++;
        if (abs(ipostaza) == 10) ipostaza = 1;
        if (ipostaza < 0) ipostaza = -ipostaza;
        ipostaza++; if (ipostaza == 2) ipostaza = 1;
    }

   // să cadă dacă Mario nu sare și nu există pământ sau scări sub el
    if (!inJump && harta[imario + 1][jmario] != '@' && harta[imario + 1][jmario] != '#') {
        inJump = true;
        jumpHeight = 2;
    }



    //// Sari logica ////
    if (inJump) {
        if (jumpHeight < 2 && harta[imario - 1][jmario] != '@') {
            jumpHeight++;
            imario--;
        } else {
            inJump = false;
            jumpHeight = 0;
            // Repetăm ​​logica căderii la pământ
            while (imario < nrLinii - 1 && harta[imario + 1][jmario] != '@') {
                imario++;
            }
        }
    }




    // Manipularea mișcării sus/jos fără sărituri pentu scari!
    if (!inJump) {
        if (directie == "sus" && harta[imario - 1][jmario] == '#') {
            imario--;
            ipostaza = 10;
        } else if (directie == "jos" && harta[imario + 1][jmario] == '#') {
            imario++;
            ipostaza = -10;
        }
    }

    // Colectarea articolelor și actualizarea scorului
    if (harta[imario][jmario] == '*') {
        scor++;
        harta[imario][jmario] = '.';
        afiseazaScor();
    }

    afiseazaMario();
}


void incarcaHarta()
{

    fisier >> nrLinii >> nrColoane;
    initwindow(latime * nrColoane, latime * nrLinii, "Mario pe scari");
    setbkcolor(WHITE); cleardevice();
    nrstelute = 0;
    for (i = 0; i < nrLinii; i++)
    {
        for (j = 0; j < nrColoane; j++)
        {
            fisier >> car;
            if (car == '*') nrstelute++;
            if (car == 'M') { imario = i; jmario = j; car = '.'; }
            harta[i][j] = car;
            afiseazaPoza(harta[i][j], i, j);
        }
    }
    fisier.close();

    scor = 0;
}

void showMenu(int &incepeX1, int &incepeY1, int &incepeX2, int &incepeY2,
              int &inchideX1, int &inchideY1, int &inchideX2, int &inchideY2) {
    cleardevice();
    setbkcolor(WHITE);
    setcolor(BLACK);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);

    int windowWidth = getmaxx();
    int windowHeight = getmaxy();

    int buttonWidth = 200;
    int buttonHeight = 50;

    incepeX1 = (windowWidth - buttonWidth) / 2;
    incepeY1 = (windowHeight - buttonHeight) / 2 - 60;
    incepeX2 = incepeX1 + buttonWidth;
    incepeY2 = incepeY1 + buttonHeight;

    rectangle(incepeX1, incepeY1, incepeX2, incepeY2);
    outtextxy(incepeX1 + 70, incepeY1 + 15, "Incepe");

    inchideX1 = incepeX1;
    inchideY1 = incepeY2 + 20;
    inchideX2 = inchideX1 + buttonWidth;
    inchideY2 = inchideY1 + buttonHeight;

    rectangle(inchideX1, inchideY1, inchideX2, inchideY2);
    outtextxy(inchideX1 + 70, inchideY1 + 15, "Inchide");
}

bool isInsideButton(int x, int y, int left, int top, int right, int bottom) {
    return (x >= left && x <= right && y >= top && y <= bottom);
}

void showEndGameMenu() {

    cleardevice();
    setbkcolor(WHITE);
    setcolor(BLACK);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);

    int windowWidth = getmaxx();
    int windowHeight = getmaxy();

    outtextxy(windowWidth / 2 - 100, windowHeight / 2 - 100, "Ati terminat jocul");

    int exitButtonX1 = (windowWidth - 200) / 2;
    int exitButtonY1 = (windowHeight / 2) + 30;
    int exitButtonX2 = exitButtonX1 + 200;
    int exitButtonY2 = exitButtonY1 + 50;

    rectangle(exitButtonX1, exitButtonY1, exitButtonX2, exitButtonY2);
    outtextxy(exitButtonX1 + 35, exitButtonY1 + 15, "Iesire");

    bool running = true;
    while (running) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);

            if (isInsideButton(x, y, exitButtonX1, exitButtonY1, exitButtonX2, exitButtonY2)) {
                running = false;
            }
        }
    }
}

bool canJump() {
    return harta[imario + 1][jmario] == '@' && harta[imario - 1][jmario] != '@';
}

void jump() {
    if (canJump() && !inJump) {
        inJump = true;
        jumpHeight = 0;
    }
}



int main()
{
    initwindow(1000, 1000, "Mario");

    int incepeX1, incepeY1, incepeX2, incepeY2;
    int inchideX1, inchideY1, inchideX2, inchideY2;
    showMenu(incepeX1, incepeY1, incepeX2, incepeY2, inchideX1, inchideY1, inchideX2, inchideY2);

    bool running = true;
    while (running) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);

            if (isInsideButton(x, y, incepeX1, incepeY1, incepeX2, incepeY2)) {
                cleardevice();
                break;
            } else if (isInsideButton(x, y, inchideX1, inchideY1, inchideX2, inchideY2)) {
                running = false;
                break;
            }
        }
    }

if (running) {
    cleardevice();
    incarcaHarta();
    ipostaza = 1;
    afiseazaMario();
    //PlaySound("SuperMarioBros.wav", NULL, SND_ASYNC);           ////////////
    directie = "none";

    do {
        if (kbhit()) {
            tasta = getch(); if (tasta == 0) tasta = getch();
            if (tasta == STG && jmario > 0) directie = "stanga";
            if (tasta == DRP && jmario < nrColoane - 1) directie = "dreapta";
            if (tasta == SUS && harta[imario - 1][jmario] == '#') directie = "sus";
            if (tasta == JOS && harta[imario + 1][jmario] == '#') directie = "jos";
            if (tasta == SPACE && !inJump) jump();

            urmatoareaIpostaza();
            directie = "none";
        } else if (inJump) {
            urmatoareaIpostaza();
        }

        delay(30);
    } while (tasta != ESC);
}



    closegraph();
    gameLog.close();
    return 0;
}
