/**
Autor: Michal Ondrejó
Login: xondre08
Názov: Projekt c.3
Dátum: 14.12.2014
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#define error -1
typedef struct {
  int rows;
  int cols;
  unsigned char *cells;
} Map;
typedef struct {
  int rows;
  int cols;
  unsigned int *cells;
} MapDistance;
void vypisHelp();
int testParam(int argc,char *argv[]);
int prevodZnakovNaCisla(char *cislo,bool *err);
unsigned char *hodnotaBunky(Map *map,int x,int y);
void vypis(Map *mapa);
void zapisOkraje(Map *mapa,char znak);
void zapisOkrajeDistance(MapDistance *mapa,int znak);
void nacitajString(FILE *in, char *s,bool *dalsiStlpec,bool *dalsiRiadok,bool *EndOfFile);
int nacitajMapu(Map *mapa,char *subor);
int otestujMapu(Map *mapa,char *subor);
bool otestovanieOkolitychBuniek(Map *mapa);
bool isborder(Map *map, int r, int c, int border);
bool najdenieVychodov(Map *mapa);
void posun(int smer,int *x,int *y);
int start_border(Map *map, int x, int y, int leftright);
void NajdenieCesty(Map *mapa,int pravidlo,int x,int y);
bool testPosun(Map *mapa,int *last,int *x,int *y,int a,int b,int c);
void freeMap(Map *mapa);
void freeMapDist(MapDistance *mapa);
void NajdenieNajkratsejCesty(Map *mapa,int x,int y);
void vynulovanieMapy(MapDistance *mapa,int znak);
void vypisMapyINT(MapDistance *mapa);
unsigned int *hodnotaBunkyDistance(MapDistance *map,int x,int y);
/** \brief
 *
 * \param pocet argumentov
 * \param pole argumentov
 * \return
 *
 */
int main(int argc,char *argv[])
{
    Map map1;
    Map *mapa=&map1;
    switch(testParam(argc,argv))
    {
    case 0://help
        break;
    case 1://test
        switch(otestujMapu(mapa,argv[2]))
        {
        case 0:
            printf("valid\n");
            break;
        case 1:
            printf("invalid\n");
            break;
        case 3: //chyba
            return error;
        }
        freeMap(mapa);
        break;
    case 2://rpath
        if(!otestujMapu(mapa,argv[4]))
        {
            bool err=false;
            int x=prevodZnakovNaCisla(argv[2],&err);
            int y=prevodZnakovNaCisla(argv[3],&err);
            if(!err)
            {
                if(x>0&& x<=map1.rows && y>0&& y<=map1.cols)
                    NajdenieCesty(mapa,1,x,y);
                else
                {
                    printf("chyba parametrov X a Y\n");
                    return error;
                }
            }
            else
            {
                printf("chyba parametrov X a Y\n");
            return error;
            }
        }
        else
        {
            printf("zadane bludisko je neplatne!\n");
            return error;
        }
    freeMap(mapa);
        break;
    case 3://lpath
        if(!otestujMapu(mapa,argv[4]))
        {
            bool err=false;
            int x=prevodZnakovNaCisla(argv[2],&err);
            int y=prevodZnakovNaCisla(argv[3],&err);
            if(!err)
            {
                if(x>0&& x<=map1.rows && y>0&& y<=map1.cols)
                NajdenieCesty(mapa,2,x,y);
                else
                {
                    printf("chyba parametrov X a Y\n");
                    return error;
                }
            }
            else
            {
                printf("chyba parametrov X a Y\n");
            return error;
            }
        }
        else
        {
            printf("zadane bludisko je neplatne!\n");
            return error;
        }
    freeMap(mapa);
        break;
    case 4://shortest
        if(!otestujMapu(mapa,argv[4]))
        {
            bool err=false;
            int x=prevodZnakovNaCisla(argv[2],&err);
            int y=prevodZnakovNaCisla(argv[3],&err);
            if(!err)
            {

                if(x>0&& x<=map1.rows && y>0&& y<=map1.cols)
                    NajdenieNajkratsejCesty(mapa,x,y);
                else
                {
                    printf("chyba parametrov X a Y\n");
                    return error;
                }
            }
            else
            {
                printf("chyba parametrov X a Y\n");
                return error;
            }
        }
        else
        {
            printf("zadane bludisko je neplatne!\n");
            return error;
        }
    freeMap(mapa);
        break;
        case 5: //chyba
            return error;
    }
    return 0;
}
/** \brief uvolnenie mapy MapDistance
 *
 * \param mapa struktora MapDistance v ktorej je ulozena mapa
 * \return
 *
 */
void freeMapDist(MapDistance *mapa)
{
    free(mapa->cells);
    mapa->cells=NULL;
}
/** \brief uvolnenie mapy Map
 *
 * \param mapa struktora Map v ktorej je ulozena mapa
 * \return
 *
 */
void freeMap(Map *mapa)
{
    free(mapa->cells);
    mapa->cells=NULL;
}
/** \brief otestovanie parametrov
 *
 * \param pocet argumentov
 * \param pole argumentov
 * \return
 *
 */
int testParam(int argc,char *argv[])
{
    if(argc==1)
    {
        printf("neboli zadane parametre\n");
        return 5;
    }
    if(argc==2&&strcmp(argv[1],"--help")==0)
    {
        vypisHelp();
        return 0;
    }
    if(argc==3&&strcmp(argv[1],"--test")==0)
    {
        return 1;
    }
    if(argc==5)
    {
        bool pozicia=false;
        prevodZnakovNaCisla(argv[3],&pozicia); //overenie èi je hodnota v R a C èíslo
        prevodZnakovNaCisla(argv[4],&pozicia);
        if(pozicia)
        {
            if(strcmp(argv[1],"--rpath")==0)
            {
                return 2;
            }
            if(strcmp(argv[1],"--lpath")==0)
            {
                return 3;
            }
            if(strcmp(argv[1],"--shortest")==0)
            {
                return 4;
            }
        }
    }
    printf("chyba argumentov!\n");
    return 5;
}
/** \brief
 *
 * \param mapa struktura do ktorej sa ulozi mapa
 * \param subor nazov suboru, v ktorom sa nachadza mapa
 * \return vrati false ak nenastala ziadna chyba mapy
 *
 */
int otestujMapu(Map *mapa,char *subor)
{
    int test;
    if((test=nacitajMapu(mapa,subor))!=0)
        return test;
    else
    {
        zapisOkraje(mapa,'x');
        if(!otestovanieOkolitychBuniek(mapa))
            return true;
        else
            return !najdenieVychodov(mapa);//vrati hodnotu true ak sa nasla cesta von z bludiska
    }
}
/*
col=3
row=4
. 0 1 2 3 4
0 x x x x x
1 x 0 0 0 x
2 x 0 0 0 x
3 x 0 0 0 x
4 x 0 0 0 x
5 x x x x x
*/
/** \brief
 *
 * \param mapa struktura do ktorej sa ulozi mapa
 * \param subor nazov suboru, v ktorom sa nachadza mapa
 * \return vrati true ak nenastala chyba pri nanacitavani mapy
 *
 */
int nacitajMapu(Map *mapa,char *subor)
{
    FILE *in;
    if((in = fopen(subor,"r"))!=NULL)
    {
        char c[21];//velkost pola aby sa do neho zmestilo int
        bool err=false;
        bool dalsiRiadok=false,dalsiStlpec=false,endOfFile=false;
        nacitajString(in,c,&dalsiStlpec,&dalsiRiadok,&endOfFile);//riadky
        mapa->rows=prevodZnakovNaCisla(c,&err);
        if(dalsiRiadok)
        {
            return 1;
        }
        else
        {
            nacitajString(in,c,&dalsiStlpec,&dalsiRiadok,&endOfFile);//stlpce
            mapa->cols=prevodZnakovNaCisla(c,&err);
            if(dalsiRiadok)
            {
                if(err)
                {
                    printf("chybne zadany pocet riadkov alebo stlpcov\n");

                    return 1;

                }
                else
                {
                    if((mapa->cells=(unsigned char*)malloc((mapa->cols+2)*(mapa->rows+2)*sizeof(unsigned char)))!=NULL)
                    {
                        dalsiRiadok=false;
                        for(int i=1;i<=(mapa->rows);i++)
                        {
                            for(int j=1;j<=mapa->cols;j++)
                            {
                                if(dalsiRiadok)
                                {
                                    printf("Chyba nacitavania mapy %d %d \n",i,j);
                                    return 1;
                                }
                                nacitajString(in,c,&dalsiStlpec,&dalsiRiadok,&endOfFile);
                                int cislo=prevodZnakovNaCisla(c,&err);
                                if(cislo<=7&&cislo>=0&& !err)
                                {
                                    *hodnotaBunky(mapa,i, j)=cislo+48;
                                }
                                else
                                {
                                    printf("Chyba nacitavania mapy %d %d \n",i,j);
                                    return 1;
                                }
                            }
                            dalsiRiadok=false;
                        }
                        nacitajString(in,c,&dalsiStlpec,&dalsiRiadok,&endOfFile);
                        if(!endOfFile)
                            return 1;
                    }
                    else
                    {
                        printf("Nedostatok pamate\n");
                        return 3;
                    }
                }
            }
            else
            {
                printf("Na prvom riadku musia byt zadane 2 cisla oznacujuce velkost bludiska!\n");
                return 1;
            }
            //uzatvorenie suboru
            fclose(in);
        }
    }
    else
    {
        printf("subor nenajdeny\n");
        return 3;
    }
    return 0;
}
/** \brief vypise mapu
 *
 * \param mapa struktora Map v ktorej je ulozena mapa
 * \return
 *
 */
void vypis(Map *mapa)
{
    for(int i=0;i<=mapa->rows+1;i++)
    {
        for(int j=0;j<=mapa->cols+1;j++)
        {
            printf("%c ", /**(mapa->cells + ((j)*(mapa->rows+2)) + (i)) */ *hodnotaBunky(mapa,i, j)+16+32);
        }
        printf("\n");
    }
        printf("\n");
}
/** \brief zapise okraj mapy
 *
 * \param mapa struktora MapDistance v ktorej je ulozena mapa
 * \param znak znak ktory sa zapise na okraj mapy
 * \return
 *
 */
void zapisOkrajeDistance(MapDistance *mapa,int znak)
{

    int i=0;
    int j=0;
    for(;i<mapa->rows+2;i++)//1.stlpec
    {
        *hodnotaBunkyDistance(mapa,i, j)=znak;

    }
    i=0;
    j=mapa->cols+1;
    for(;i<mapa->rows+2;i++)//posledny stlpec
    {
        *hodnotaBunkyDistance(mapa,i, j)=znak;

    }
    i=0;
    j=1;
    for(;j<mapa->cols+1;j++)//horny riadok
    {
        *hodnotaBunkyDistance(mapa,i, j)=znak;
    }
    i=mapa->rows+1;
    j=1;
    for(;j<mapa->cols+1;j++)//dolny riadok
    {
       *hodnotaBunkyDistance(mapa,i, j)=znak;
    }
}
/** \brief zapise okraj mapy
 *
 * \param mapa struktora Map v ktorej je ulozena mapa
 * \param znak znak ktory sa zapise na okraj mapy
 * \return
 *
 */
void zapisOkraje(Map *mapa,char znak)
{
    int i=0;
    int j=0;
    for(;i<mapa->rows+2;i++)//1.stlpec
    {
        *hodnotaBunky(mapa,i, j)=znak;

    }
    i=0;
    j=mapa->cols+1;
    for(;i<mapa->rows+2;i++)//posledny stlpec
    {
        *hodnotaBunky(mapa,i, j)=znak;

    }
    i=0;
    j=1;
    for(;j<mapa->cols+1;j++)//horny riadok
    {
        *hodnotaBunky(mapa,i, j)=znak;
    }
    i=mapa->rows+1;
    j=1;
    for(;j<mapa->cols+1;j++)//dolny riadok
    {
       *hodnotaBunky(mapa,i, j)=znak;
    }
}
/** \brief otestovanie hran
 *
 * \param mapa struktora Map v ktorej je ulozena mapa
 * \return
 *
 */
bool otestovanieOkolitychBuniek(Map *mapa)
{
    //stred
    int j=1;
    int i=1;
    for(i=1;(i<(mapa->rows));i++)
    {
        for(j=1;((mapa->cols+1)>j);j++)
        {
            if(i%2!=j%2)
            {//4
                    if(isborder(mapa, i, j, 3)!=isborder(mapa, i+1, j, 3))
                    {
                        return false;
                    }
            }
            if(j<mapa->cols)
            {//sikme
                if(isborder(mapa, i, j, 2)!=isborder(mapa, i, j+1, 1))
                    {
                        return false;
                    }
            }
        }
    }
    return true;
}
/** \brief zistenie pociatocneho smeru
 *
 * \param mapa struktora Map v ktorej je ulozena mapa
 * \param x zaciatocna pozicia X
 * \param y zaciatocna pozicia Y
 * \param leftright pravidlo lavej - 1
 *                          pravej - 2
 * \return
 *
 */
int start_border(Map *map, int x, int y, int leftright)
{
    //corners
    if(x==1 && y==1)
    {
        if(!isborder(map,x,y,3))
        {
            return 3;
        }
        else
            return 2;
    }
    if(x==1 && y==map->cols)
    {
        if(y%2!=0)
        {
            if(!isborder(map,x,y,3))
            {
                return 3;
            }
            if(!isborder(map,x,y,2))
            {
                return 1;
            }
        }
        else
            return 1;
    }
    if(x==map->rows && y==1)
    {
        if(x%2==0)
        {
            if(!isborder(map,x,y,3))
            {
                return 3;
            }
            if(!isborder(map,x,y,1))
            {
                return 2;
            }
        }
        else
            return 2;
    }
    if(x==map->rows && y==map->cols)
    {
        if(y%2!=x%2)
        {
            if(!isborder(map,x,y,3))
            {
                return 3;
            }
            if(!isborder(map,x,y,2))
            {
                return 1;
            }
        }
        return 1;
    }
    //lines
    if(y==1)
    {
        return 2;
    }
    if(y==map->cols)
    {
        return 1;
    }
    return 3;
    if(leftright==1)
    {//kedze pri tomto rieseni nepotrebujem leftright, tak aby nebol error ju musim nejak pouzit
    }
}
/** \brief vrati hodnota bunky v tabulke
 *
 * \param mapa struktora Map v ktorej je ulozena mapa
 * \param x  pozicia X
 * \param y  pozicia Y
 * \return
 *
 */
unsigned char *hodnotaBunky(Map *map,int x,int y)
{
    return (map->cells + ((x)*(map->cols+2)) + (y));
}
/** \brief vrati hodnota bunky v tabulke
 *
 * \param mapa struktora MapDistance v ktorej je ulozena mapa
 * \param x  pozicia X
 * \param y  pozicia Y
 * \return
 *
 */
unsigned int *hodnotaBunkyDistance(MapDistance *map,int x,int y)
{
    return (map->cells + ((x)*(map->cols+2)) + (y));
}
/** \brief zistenie ci sa na danej pozicii nachadza stena
 *
 * \param mapa struktora Map v ktorej je ulozena mapa
 * \param r  pozicia x
 * \param c  pozicia Y
 * \param border strana na ktorej sa hlada stena
 * \return true ak sa nasla stena
 *
 */
bool isborder(Map *map, int r, int c, int border)
{
    if(border ==3)
    {//hore/dole
        if((*hodnotaBunky(map,r,c)&4)!=0)
        {
           return true;
        }
    }
    if(border ==2)
    {//vpravo
        if((*hodnotaBunky(map,r,c)&2)!=0)
        {
           return true;
        }
    }
    if(border ==1)
    {//vlavo
        if((*hodnotaBunky(map,r,c)&1)!=0)
        {
           return true;
        }
    }
    return false;
}
/** \brief zistenie ci mapa ma vychod
 *
 * \param mapa struktora Map v ktorej je ulozena mapa
 * \return true ak sa nasiel vychod z mapy
 *
 */
bool najdenieVychodov(Map *mapa)
{
    int i=1;
    int j=1;
    for(;i<mapa->rows+1;i++)//1.stlpec
    {
        if(isborder(mapa,i,j,1))
            return true;
    }
    i=1;
    j=mapa->cols;
    for(;i<mapa->rows+1;i++)//posledny stlpec
    {
        if(isborder(mapa,i,j,2))
            return true;
    }
    i=1;
    j=1;
    for(;j<mapa->cols;j++)//horny riadok
    {
        if(j%2!=0)
            if(isborder(mapa,i,j,3))
                return true;
    }
    i=mapa->rows;
    j=1;
    for(;j<mapa->cols;j++)//dolny riadok
    {
        if(mapa->rows%2!=0)//ak nieje parny pocet riadkov
        {//tak sa testuju parne
            if(j%2==0)
            if(isborder(mapa,i,j,3))
                return true;
        }
        else
        {//inak sa testuju neparne
            if(j%2!=0)
            if(isborder(mapa,i,j,3))
                return true;
        }

    }
    return false;
}
/////////////////////////////////////////////////////////////////////////////
/** \brief funkcia najde cestu s pouzitim pravej alebo lavej ruky
 *
 * \param mapa musi obsahovat platnu mapu
 * \param pravidlo pravej ruky 1
 *                 lavej ruky 2
 * \param x pozicia X
 * \param y pozicia Y
 * \return
 *
 */
void NajdenieCesty(Map *mapa,int pravidlo,int x,int y)
{
    int last=start_border(mapa,x,y, pravidlo);
    while(*hodnotaBunky(mapa,x,y)!='x')
    {
        printf("%d,%d\n",x,y);

            if(last==3)
            {
                if(x%2==y%2)
                {//dole
                    if(pravidlo==1)
                    {
                        testPosun(mapa,&last,&x,&y,1,2,3);
                    }
                    else //pravidlo 2
                    {
                        testPosun(mapa,&last,&x,&y,2,1,3);
                    }
                }
                else
                {//hore
                    if(pravidlo==2)
                    {
                        testPosun(mapa,&last,&x,&y,1,2,3);
                    }
                    else //pravidlo 1
                    {
                        testPosun(mapa,&last,&x,&y,2,1,3);
                    }
                }
            }else
            if(last==2)
            {

                if(x%2!=y%2)
                {//hore
                    if(pravidlo==1)
                    {
                        testPosun(mapa,&last,&x,&y,3,2,1);
                    }
                    else //pravidlo 2
                    {
                        testPosun(mapa,&last,&x,&y,2,3,1);
                    }
                }
                else
                {//dole
                    if(pravidlo==2)
                    {
                        testPosun(mapa,&last,&x,&y,3,2,1);
                    }
                    else //pravidlo 1
                    {
                        testPosun(mapa,&last,&x,&y,2,3,1);
                    }
                }
            }else
            if(last==1)
            {
                if(x%2!=y%2)
                {//hore
                    if(pravidlo==1)
                    {
                        testPosun(mapa,&last,&x,&y,1,3,2);
                    }
                    else //pravidlo 2
                    {
                        testPosun(mapa,&last,&x,&y,3,1,2);

                    }
                }
                else
                {//dole
                    if(pravidlo!=2)
                    {
                        testPosun(mapa,&last,&x,&y,3,1,2);
                    }
                    else //pravidlo 2
                    {
                        testPosun(mapa,&last,&x,&y,1,3,2);
                    }
                }
            }
    }
}
/** \brief funkcia zisti ci sa nachadzaju steny na bunke
 *
 * \param mapa struktora Map v ktorej je ulozena mapa
 * \param last pozicia z ktorej sme dosli do bunky
 * \param x pozicia x
 * \param y pozicia Y
 * \param a smer ktory sa ma testovat ako prvy
 * \param b smer ktory sa ma testovat ako druhy
 * \param c smer ktory sa ma testovat ako treti
 *
 */
 bool testPosun(Map *mapa,int *last,int *x,int *y,int a,int b,int c)
{
    if(!isborder(mapa,*x,*y,a))
    {
        posun(a,x,y);
        *last=a;
    }else
    if(!isborder(mapa,*x,*y,b))
    {
        posun(b,x,y);
        *last=b;
    }else
    if(!isborder(mapa,*x,*y,c))
    {
        posun(c,x,y);
        *last=c;
    }
    else{return 1;}
    return 0;
}
/** \brief posuvanie sa medzi bunkami
 *
 * \param smer na ktory sa ma posunut
 * \param x pozicia x
 * \param y pozicia Y
 *
 */
void posun(int smer,int *x,int *y)
{
    if(smer==1)
    {
        *y=*y-1;
    }
    if(smer==2)
    {
        *y=*y+1;
    }
    if(smer==3)
    {
        if(*x%2==*y%2)
        {
            *x=*x-1;
        }
        else
        {
            *x=*x+1;
        }
    }
}
/////////////////////////////////////////////////////////////////////////////
/** Funkcia, na nacítanie pola znakov zo vstupu
 * \param s: pole znakov do ktoreho sa uozi nacitany string
 * \param dalsiStlpec boolovska premenna do ktorej ulozi hodnotu true, ak bol nacitany prazdny znak
 * \param dalsiRiadok boolovska premenna do ktorej ulozi hodnotu true, ak bol nacitany koniec riadka
 * \param EndOfFile boolovska premenna do ktorej ulozi hodnotu true, ak bol nacitany koniec subora
 */
void nacitajString(FILE *in, char *s,bool *dalsiStlpec,bool *dalsiRiadok,bool *EndOfFile)
{
    int i=1;
    int c;
    while ((c=fgetc(in)) == ' ' ){ } /**Vynechanie medzier znakov*/
    s[0]=c;
    if(c!=EOF)
    while ((c=fgetc(in))!=EOF && (c != '\n')&& (c != ' '))
    {
         s[i]=c;
         i++;
    }
    s[i]='\0';
    *dalsiRiadok=(c=='\n');
    *dalsiStlpec=(c==' ');
    *EndOfFile=(c==EOF);
}
/** Funkcia prevedie cislo v char* na int
 * \param cislo cislo ktore sa nachadza v char *
 * \param err ak nastala chyba nastavy hodnotu na true
 * \return vrati prevedenu hodnotu
 */
int prevodZnakovNaCisla(char *cislo,bool *err)
{
    int i=0;
    char * zvysok;
    i=strtol(cislo,&zvysok,10);
    if(*zvysok!=0){
        *err=true;
        return 0;
    }
    return i;
}
/** \brief Funkcia, na najdenie najkratsej cesty
 *
 * \param mapa struktora Map v ktorej je ulozena mapa
 * \param x pozicia x
 * \param y pozicia Y
 *
 */
void NajdenieNajkratsejCesty(Map *mapa,int x,int y)
{
    MapDistance mapaVzdialenost;
    mapaVzdialenost.cells=(unsigned int*)malloc((mapa->cols+2)*(mapa->rows+2)*sizeof(unsigned int));
    mapaVzdialenost.cols=mapa->cols;
    mapaVzdialenost.rows=mapa->rows;
    vynulovanieMapy(&mapaVzdialenost,LONG_MAX);
    zapisOkrajeDistance(&mapaVzdialenost,500);
    int koniecX=x,koniecY=y,pocet=1;
    bool najdenyKoniec=false;
    unsigned int vzdialenost=0;
    bool nenajdenyVychod=false;
    *hodnotaBunkyDistance(&mapaVzdialenost,x,y)=vzdialenost;
    while(!najdenyKoniec)
    {
        for(int i=1;i<mapa->rows+1;i++)
        {
            for(int j=1;j<mapa->cols+1;j++)
            {
                if(*hodnotaBunkyDistance(&mapaVzdialenost,i,j)==vzdialenost-1)
                {
                    if(!isborder(mapa,i,j,1))
                    {
                        if(*hodnotaBunky(mapa,i,j-1)=='x')
                        {
                            najdenyKoniec=true;
                            koniecX=i;
                            koniecY=j;
                        }
                        //else
                        {
                            if(*hodnotaBunkyDistance(&mapaVzdialenost,i,j-1)>=vzdialenost)
                            {
                                *hodnotaBunkyDistance(&mapaVzdialenost,i,j-1)=vzdialenost;
       // vypisMapyINT(&mapaVzdialenost);
                            }


                        }
                    }
                    if(!isborder(mapa,i,j,2))
                    {
                        if(*hodnotaBunky(mapa,i,j+1)=='x')
                        {
                            najdenyKoniec=true;
                            koniecX=i;
                            koniecY=j;
                        }
                       // else
                        {
                            if(*hodnotaBunkyDistance(&mapaVzdialenost,i,j+1)>=vzdialenost)
                            {
                                *hodnotaBunkyDistance(&mapaVzdialenost,i,j+1)=vzdialenost;
        //vypisMapyINT(&mapaVzdialenost);
                            }
                        }
                    }
                    if(!isborder(mapa,i,j,3))
                    {
                        if(i%2!=j%2)
                        {
                            if(*hodnotaBunky(mapa,i+1,j)=='x')
                            {
                                najdenyKoniec=true;
                                koniecX=i;
                                koniecY=j;
                            }
                            //else
                            {
                                if(*hodnotaBunkyDistance(&mapaVzdialenost,i+1,j)>=vzdialenost)
                                {
                                    *hodnotaBunkyDistance(&mapaVzdialenost,i+1,j)=vzdialenost;
       //vypisMapyINT(&mapaVzdialenost);
                                }
                            }
                        }
                        else
                        {
                            if(*hodnotaBunky(mapa,i-1,j)=='x')
                            {
                                najdenyKoniec=true;
                                koniecX=i;
                                koniecY=j;
                            }
                            //else
                            {
                                if(*hodnotaBunkyDistance(&mapaVzdialenost,i-1,j)>=vzdialenost)
                                {
                                    *hodnotaBunkyDistance(&mapaVzdialenost,i-1,j)=vzdialenost;
       // vypisMapyINT(&mapaVzdialenost);
                                }
                            }
                        }
                    }
                    pocet++;
                }
            }
        }
        if(pocet==0)
        {
            najdenyKoniec=true;
            nenajdenyVychod=true;
        }
        else{
        pocet=0;
        /*printf("%d [%d,%d]\n",vzdialenost,koniecX,koniecY);
        vypisMapyINT(&mapaVzdialenost);*/
        vzdialenost++;
        }

    }
    if(nenajdenyVychod)
    {
        printf("Z pozicie [%d,%d] nebolo mozne najst ziadny vychod\n",koniecX,koniecY);
        return;
    }
    int d=0;
    int *reverzneX, *reverzneY;

    const int array_size = vzdialenost * sizeof(int);
    reverzneX = (int *) malloc(array_size);
    reverzneY = (int *) malloc(array_size);
    for( int i=vzdialenost-2;i>=0;i--)
    {//cyklus na vratenie sa do zaciatocneho bodu a ulozenie reverzne do pola
        if(!isborder(mapa,koniecX,koniecY,1))
        {
            if(*hodnotaBunkyDistance(&mapaVzdialenost,koniecX,koniecY-1)==(unsigned)i-1)
            {
                reverzneX[d]=koniecX;
                reverzneY[d]=koniecY;
                koniecY--;
            }
        }
        if(!isborder(mapa,koniecX,koniecY,2))
        {
            if(*hodnotaBunkyDistance(&mapaVzdialenost,koniecX,koniecY+1)==(unsigned)i-1)
            {
                reverzneX[d]=koniecX;
                reverzneY[d]=koniecY;
                koniecY++;
            }
        }
        if(!isborder(mapa,koniecX,koniecY,3))
        {
            if(koniecX%2!=koniecY%2)
            {

                if(*hodnotaBunkyDistance(&mapaVzdialenost,koniecX+1,koniecY)==(unsigned)i-1)
                {
                    reverzneX[d]=koniecX;
                    reverzneY[d]=koniecY;
                    koniecX++;
                }

            }
            else
            {
                if(*hodnotaBunkyDistance(&mapaVzdialenost,koniecX-1,koniecY)==(unsigned)i-1)
                {
                    reverzneX[d]=koniecX;
                    reverzneY[d]=koniecY;
                    koniecX--;
                }
            }
        }
d++;
    }
    d--;
    reverzneX[d]=koniecX;
    reverzneY[d]=koniecY;
    for(int i=vzdialenost-2;i>=0;i--)
    {
        printf("%d,%d\n",reverzneX[i],reverzneY[i]);
    }
    free(reverzneX);
    reverzneX=NULL;
    free(reverzneY);
    reverzneY=NULL;
    freeMapDist(&mapaVzdialenost);
}
/** \brief vypise mapu
 *
 * \param mapa struktora MapDistance v ktorej je ulozena mapa
 * \return
 *
 */
void vypisMapyINT(MapDistance *mapa)
{
    for(int i=0;i<=mapa->rows+1;i++)
    {
        for(int j=0;j<=mapa->cols+1;j++)
        {
            printf("%d ", *hodnotaBunkyDistance(mapa,i, j));
        }
        printf("\n");
    }
        printf("\n");
}
/** \brief nastavy hodnotu buniek v mape
 *
 * \param mapa struktora MapDistance v ktorej sa bunky nastavia na hodnotu znak
 * \param znak znak ktory sa ulozi do buniek
 * \return
 *
 */
void vynulovanieMapy(MapDistance *mapa,int znak)
{
    for(int i=0;i<(mapa->rows+2);i++)
    {
        for(int j=0;j<mapa->cols+2;j++)
        {
                *hodnotaBunkyDistance(mapa,i, j)=znak;
        }

    }
}
/** \brief vypise napovedu
 *
 */
void vypisHelp()
{
    printf("Program se spousti v nasledujici podobe:\n"
"\t./proj3 --help\n"
"nebo\n"
"\t./proj3 --test soubor.txt\n"
"nebo\n"
"\t./proj3 --rpath R C bludiste.txt\n"
"nebo\n"
"\t./proj3 --lpath R C bludiste.txt\n"
"nebo\n"
"\t./proj3 --shortest R C bludiste.txt\n"
"Argumenty programu:\n"
"\t--help zpusobi, ze program vytiskne napovedu pouzivani programu a skonci.\n"
"\t--test pouze zkontroluje, ze soubor dany druhym argumentem programu obsahuje radnou definici mapy bludiste. "
"V pripade, ze format obrazku odpovida definici (viz nize), vytiskne Valid. "
"V opacnem pripade (napr. ilegalnich znaku, chybejicich udaju nebo spatnych hodnot) program tiskne Invalid.\n"
"\t--rpath hleda pruchod bludistem na vstupu na radku R a sloupci C. Pruchod hleda pomoci pravidla prave ruky (prava ruka vzdy na zdi).\n"
"\t--lpath hleda pruchod bludistem na vstupu na radku R a sloupci C, ale za pomoci pravidla leve ruky.\n"
"\t--shortest hleda nejkratsi cestu z bludiste pri vstupu na radku R a sloupci C.y\n\n");
}
