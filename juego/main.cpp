#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>
#include <list>
using namespace std;

#define ESPACIO 32
#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80

void gotoxy (int x, int y){
    HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;

	SetConsoleCursorPosition(hCon, dwPos);
}

void ocultarCursor(){
    HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 2;
	cci.bVisible = FALSE;

	SetConsoleCursorInfo(hCon, &cci);
}

void pintar_limites(){
    for(int i=2; i<78; i++){
        gotoxy(i, 2); printf("%c", 205);
        gotoxy(i, 33); printf("%c", 205);
    }
    for(int i=3; i<33; i++){
        gotoxy(2, i); printf("%c", 186);
        gotoxy(77, i); printf("%c", 186);
    }
    gotoxy(2,2); printf("%c",201);
    gotoxy(2,33); printf("%c",200);
    gotoxy(77,2); printf("%c",187);
    gotoxy(77,33); printf("%c",188);
}

class NAVE{
    int x, y;
    int corazones;
    int vidas;
public:
    NAVE (int _x, int _y, int _corazones, int _vidas): x(_x), y(_y), corazones(_corazones), vidas(_vidas){}
    int X(){ return x; }
    int Y(){ return y; }
    int VID(){ return vidas; }
    void COR(){ corazones--; }
    void pintar();
    void borrar();
    void mover();
    void pintar_corazones();
    void morir();
};

void NAVE::pintar(){
    gotoxy(x, y);   printf("  %c", 30);
    gotoxy(x, y+1); printf(" %c%c%c", 40,207,41);
    gotoxy(x, y+2); printf("%c%c %c%c",30,190,190,30);
}

void NAVE::borrar(){
    gotoxy(x, y);   printf("        ");
    gotoxy(x, y+1); printf("        ");
    gotoxy(x, y+2); printf("        ");
}

void NAVE::mover(){
       if(kbhit()){
            char tecla = getch();
            borrar();
            if (tecla == IZQUIERDA && x>3) x--;
            if (tecla == DERECHA && x+6 <78) x++;
            if (tecla == ARRIBA && y>3) y--;
            if (tecla == ABAJO && y+3 <33) y++;
            if (tecla == 'e') corazones--;
            pintar();
            pintar_corazones();
        }
}

void NAVE::pintar_corazones(){

    gotoxy(50, 1); printf("VIDAS %d", vidas);
    gotoxy(64,1); printf("Salud");
    gotoxy(70,1); printf("        ");
    for(int i=0; i<corazones; i++){
        gotoxy(70+i,1); printf("%c",3);
    }
}

void NAVE::morir(){
    if(corazones == 0){
        borrar();
        gotoxy(x,y);   printf("   **   ");
        gotoxy(x,y+1); printf("  ****  ");
        gotoxy(x,y+2); printf("   **   ");
        Sleep(300);

        borrar();
        gotoxy(x,y);   printf(" * ** * ");
        gotoxy(x,y+1); printf("  ****  ");
        gotoxy(x,y+2); printf(" * ** * ");
        Sleep(250);
        borrar();

        vidas--;
        corazones = 3;
        pintar_corazones();
        pintar();
    }
}

class AST{
    int x, y;
public:
    AST(int _x, int _y):x(_x), y(_y){}
    void pintar();
    void mover();
    void choque( class NAVE &N);
    int X(){return x;}
    int Y(){return y;}

};

void AST::pintar(){
    gotoxy(x,y); printf("%c",184);
}

void AST::mover(){
    gotoxy(x,y); printf("  ");
    y++;
    if (y > 32){
        x = rand()% 71 + 4;
        y = 4;
    }
    pintar();
}

void AST::choque( class NAVE &N){
    if( x>= N.X() && x< N.X()+6 && y>= N.Y() && y< N.Y()+2 ){
       N.COR();
       N.borrar();
       N.pintar();
       N.pintar_corazones();
       x = rand()% 71 + 4;
       y = 4;
    }
}

class BALA{
    int x, y;
public:
    BALA (int _x, int _y): x(_x), y(_y){}
    int X(){ return x; }
    int Y(){ return y; }
    void mover();
    bool fuera();
};

void BALA::mover(){
    gotoxy(x, y); printf(" ");
    y--;
    gotoxy(x, y); printf("*");
}

bool BALA::fuera(){
    if (y == 4) return true;
    return false;
}
int main()
{
    ocultarCursor();
    pintar_limites();
    NAVE N(37,30,3,3);
    N.pintar();
    N.pintar_corazones();

    list<AST*> aster;
    list<AST*>::iterator iteA;
    for (int i=0; i<5; i++){
        aster.push_back(new AST(rand()%75 + 3, rand()%5 + 4));
    }

    list<BALA*> balas;
    list<BALA*>::iterator ite;


    bool game_over = false;
    int puntos = 0;

    while(!game_over){

    gotoxy(4, 1); printf("Puntos %d", puntos);
    if(kbhit()){
            char tecla = getch();
            if( tecla == ESPACIO)
                balas.push_back( new BALA (N.X()+2, N.Y()-1));
    }
        for(ite= balas.begin(); ite!= balas.end(); ite++)
            {

            (*ite)->mover();
            if((*ite)-> fuera()){
                gotoxy((*ite)-> X(), (*ite)-> Y()); printf(" ");
                delete (*ite);
                ite = balas.erase(ite);
        }
            }

        for(iteA= aster.begin(); iteA!= aster.end(); iteA++){
            (*iteA)->mover();
            (*iteA)->choque(N);
        }

        for(iteA= aster.begin(); iteA!= aster.end(); iteA++){
            for(ite= balas.begin(); ite!=balas.end(); ite++){
                if((*iteA)->X() == (*ite)->X() && ( (*iteA)->Y() + 1 == (*ite)->Y() || (*iteA)->Y() == (*ite)->Y() )){
                    gotoxy((*ite)->X(), (*ite)->Y()); printf(" ");
                    delete(*ite);
                    ite= balas.erase(ite);

                    aster.push_back(new AST(rand()%75 + 3, 4));
                    gotoxy((*iteA)->X(), (*iteA)->Y()); printf(" ");
                    delete(*iteA);
                    iteA= aster.erase(iteA);

                    puntos+=5;

                }
            }
        }
        if(N.VID()== 0){ game_over= true;
        gotoxy(4,10); printf("GAME OVER");
        }
        N.morir();
        N.mover();
        Sleep(30);
    }
    return 0;
}
