/**
Autor: Michal Ondrejó
Login: xondre08
Názov: Projekt č.2
Dátum: 30.11.2014
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define POCET_ITERACII 8
#define MAX_VYSKA 100
#define DEFAULT_VYSKA 1.5
#define MAX_POCET_ITERACII 14
#define POL_PI 1.4
void tang(char *argv[]);
void testParam(int argc,char *argv[],bool *chyba);
void testParam2(int argc,char *argv[],bool *chyba);
void vypisHelp();
double taylor_tan(double x, unsigned int n);
double cfrac_tan(double x, unsigned int n);
double prevodZnakovNaCisla(char *cislo,bool *err);
double dAbs(double cislo);
void vzdialenost(int argc,char *argv[]);
double vypocitajVzdialenist(double vyska,double alfa);
double vypocitajVysku(double beta,double vzdialenostOdPredmetu);
void nastavVysku(int *p,char *argv[],double *vyska,bool *konec);
/** \brief main
 *
 * \param argc pocet argumentov
 * \param argv pole argumentov
 * \return
 *
 */

int main(int argc,char *argv[])
{
    bool konec=false;
    testParam(argc,argv,&konec);
    bool tan = false;
    if(strcmp(argv[1],"--tan")==0){
				testParam2(argc,argv,&konec);
				if(!konec)
        tang(argv);
        tan=true;
    }
    if(!tan&&!konec)
			vzdialenost(argc,argv);
    return 0;
}

/** \brief
 * funkcia na otestovanie ci bol zadany jeden alebo ziadny argument
 * \param argc pocet argumentov
 * \param argv pole argumentov
 *
 */
void testParam(int argc,char *argv[],bool *chyba)
{
    if(argc==1)
    {
        printf("Neboli zadane parametre!\n");
        *chyba=true;
    }

    if(argc==2)
    {
        if(strcmp( argv[1],"--help")==0){
            vypisHelp();
            *chyba=true;
        }
        else
        {
            printf("Chybny parameter\n");
            *chyba=true;
        }
    }
}
void testParam2(int argc,char *argv[],bool *chyba)
{
		bool err;
    if(argc!=5)
    {
        printf("chybny pocet parametrov\n");
        *chyba=true;
    }
    double alfa=prevodZnakovNaCisla(argv[2],&err);
    if(*chyba||
       0>=alfa||
       alfa>POL_PI||
       err)
    {
        printf("chybny parameter\n");
        *chyba=true;
    }
    if(!*chyba)
    {
        if(prevodZnakovNaCisla(argv[3],&err)>prevodZnakovNaCisla(argv[4],&err)) //najprv sa overý ze nenastala chyba programu a potom až zistí či je číslo N menšie ako M
        {
            printf("Parameter N musi byt mensi ako parameter M!\n");
            *chyba=true;
        }
        else
        {
            if((prevodZnakovNaCisla(argv[3],&err)<=0||prevodZnakovNaCisla(argv[4],&err)>=MAX_POCET_ITERACII))
            {
                printf("N a M musi byt vacsie ako 0 a mensie ako 14\n");
                *chyba=true;
            }
            else
                *chyba=false;
        }
    }
}
/**  funkcia na vypis helpu
 *
 */
void vypisHelp()
{
    printf("Program se spusta v nasledujucej podobe:\n"
        "./proj2 --help\n"
        "nebo\n"
        "./proj2 --tan A N M\n"
        "nebo\n"
        "./proj2 [-c X] -m A [B]\n"
        "Argumenty programu:\n"
        "--help zposobi, ze program vytiskne napovedu pouzivani programu a skonci.\n"
        "--tan srovna presnosti vypoctu tangens uhlu A (v radianech) mezi volanim tan z matematicke knihovny, a vypoctu tangens pomoci Taylorova polynomu a zretezeneho zlomku. Argumenty N a M udavaji, ve kterych iteracich iteracniho vypoctu ma srovnani probihat. 0 < N <= M < 14\n"
        "-m vypocita a zmeri vzdalenosti.\n"
        "uhel [alfa] je dan argumentem A v radianech. Program vypocita a vypise vzdalenost mereneho objektu. 0 < A <= 1.4 < PI/2.\n"
        "Pokud je zadan, uhel [beta] udava argument B v radianech. Program vypocita a vypise i vysku mereneho objektu. 0 < B <= 1.4 < PI/2\n"
        "Argument -c nastavuje vysku mericiho pristroje c pro vypocet. Vyska c je dana argumentem X (0 < X <= 100). Argument je volitelny - implicitni vyska je 1.5 metru.\n");
}
/** \brief taylor_tan
 *
 * \param x uhol
 * \param n pocet iteraci
 *
 */
/*1,3,5,21,9,55,39,105,17,171,7,253,75,351*/
//1,1,2,
double taylor_tan(double x, unsigned int n)
{
    double vysledok=x;
    double x2=x*x*x;
    unsigned long long int menov=1;
    int menovNasobok[]={1,3,5,21,9,55,39,105,17,171,105,253,75,351};
    //unsigned long long int menovatel[]={ 3, 15, 315, 2835, 155925, 6081075, 638512875, 10854718875, 1856156927625, 194896477400625, 49308808782358125, 3698160658676859375, 1298054391195577640625, 263505041412702261046875};
    long long int citatel[]={1, 1, 2, 17, 62, 1382, 21844, 929569, 6404582, 443861162, 18888466084, 113927491862, 58870668456604/*, 8374643517010684, 689005380505609448, 129848163681107301953, 1736640792209901647222, 418781231495293038913922*/};
    for(unsigned int i=1;i<n&&i<13;i++)
    {
        menov*=menovNasobok[i];
        vysledok+=(citatel[i]*x2)/(menov);
        x2*=x*x;
    }
    return vysledok;
}
/** \brief cfrac_tan
 *
 * \param x uhol
 * \param n pocet iteraci
 *
 */
double cfrac_tan(double x, unsigned int n)
{
    double cf;
    double a=1.0;
    double b=(n*2-1)/x;
    cf=a/b;
    for(;n>=1;n--)
    {
        a=1.0;
        b=(n*2-1)/x;
        cf=a/(b-cf);
    }
    return cf;
}
/** \brief tang funkcia na vypocet tan
 *
 * \param argc pocet argumentov
 * \param argv pole argumentov
 *
 */
void tang(char *argv[])
{
    bool err;
    /*if(argc!=5)    opravene :)
    {
        printf("chybny pocet parametrov\n");
        exit(CHYBA_PARAM);
    }
    if(prevodZnakovNaCisla(argv[3],&err)>prevodZnakovNaCisla(argv[4],&err))
    {
        printf("Parameter N musi byt mensi ako parameter M!\n");
        exit(CHYBA_PARAM);
    }
    else
    {
        if(prevodZnakovNaCisla(argv[3],&err)<=0||prevodZnakovNaCisla(argv[4],&err)>=14)
        {
            printf("N a M musi byt vacsie ako 0 a mensie ako 14\n");
            exit(CHYBA_PARAM);
        }
        else
        {*/
            double uhol=prevodZnakovNaCisla(argv[2],&err);
            double taylor;
            double cfrac;
            double tanMath=tan(uhol); //ulozenie do premennej tanMath hodnotu tangens(uhol), aby sa nemusela stále pocitať
            int Max=prevodZnakovNaCisla(argv[4],&err);
            for(int i=(int)prevodZnakovNaCisla(argv[3],&err);//od poctu iteraci
                    i<=Max;//do poctu iteraci
                    i++)
            {
                        taylor =taylor_tan(uhol,i);
                        cfrac=cfrac_tan(uhol,i);
                        printf("%d %e %e %e %e %e\n",//vypis
                         // printf("%d %.30f %.30f %.30f %.30f %.30f"\n", // na testovanie
                               i, //iteracia
                               tanMath,
                               taylor,
                               dAbs(tanMath-taylor), //presnost taylor. voci matematickej kniznicy
                               cfrac,
                               dAbs(tanMath-cfrac)); //presnost cfrac. voci matematickej kniznicy
            }
       // }
    //}
}
/** \brief prevodZnakovNaCisla
 *
 * \param argc pocet argumentov
 * \param err pointer, ktory ulozi do premennej err hodnotu true ak sa retazec nedal previest na cislo
 *
 */
double prevodZnakovNaCisla(char *cislo,bool *err)
{
    double i=0;
    char * zvysok;
    i=strtod(cislo,&zvysok);
    if(*zvysok!=0){
        *err=true;
        return 0;
    }
    return i;
}
/** \brief dAbs vrati absolutnu hodnotu z cisla
 *
 * \param cislo cislo ktore sa ma previest na absolutnu hodnotu
 *
 */
double dAbs(double cislo)
{
    if(cislo >0)
        return cislo;
    else
        return 0-cislo;
}
/** \brief vzdialenost vypocita vzdialenost od objektu / vysku objektu
 *
 * \param argc pocet argumentov
 * \param argv pole argumentov
 *
 */
void vzdialenost(int argc,char *argv[])
{
    double vyska=DEFAULT_VYSKA;
    int p=1;
    bool err=false;
    nastavVysku(&p,argv,&vyska,&err);
    double alfa;
    double beta;
    double vzdialenostOdPredmetu;
    bool konec=false;
    if(!err)
    {

            if(strcmp(argv[p],"-m")==0)
            {
                p++;
                alfa=prevodZnakovNaCisla(argv[p],&err);//nastavy alfa a err
                if(err)
                {
                    printf("Nespravne parametre!\n");
                    konec=true;
                }
                if((alfa>POL_PI||alfa<=0)&&!konec)
                {
                    printf("Alfa musi byt v rozmedzi 0<A<=1.4\n");
                    konec=true;
                }
                if(!konec)
                vzdialenostOdPredmetu= vypocitajVzdialenist(vyska,alfa);
            }
            else
            {
                printf("Nespravne parametre!\n");
                    konec=true;
            }
            if(p!=argc-1&&!konec)  //zistenie ci za alfou je este nejaky parameter
            {

                if(p+1!=argc-1)//beta
                {
                    printf("Privela parametrov\n");
                    konec=true;
                }

                if(!konec)
                beta=prevodZnakovNaCisla(argv[p+1],&err); //nastavy betu a err
                if(err&&!konec)
                {
                    printf("Nespravne parametre!\n");
                    konec=true;
                }
                if((beta>POL_PI||beta<=0)&&!konec)
                {
                    printf("Beta musi byt v rozmedzi 0<B<=1.4\n");
                    konec=true;
                }

                if(!konec){
									printf(/*Vzdialenost od predmetu je:*/"%.10e\n",vzdialenostOdPredmetu);
									printf(/*"Vyska predmetu je:*/"%.10e\n",vyska+vypocitajVysku(beta,vzdialenostOdPredmetu));
                }
            }
            else
            {

                if(!konec)
                printf(/*Vzdialenost od predmetu je:\t*/"%.10e\n",vzdialenostOdPredmetu);
            }
    }
}
/** \brief vypocitajVysku funkcia vypocita a vrati vysku predmetu
 *
 * \param beta uhol
 * \param vzdialenostOdPredmetu vzdialenost od predmetu
 *
 */
double vypocitajVysku(double beta,double vzdialenostOdPredmetu)
{
    return cfrac_tan(beta,POCET_ITERACII)*vzdialenostOdPredmetu;
}
/** \brief vypocitajVzdialenist funkcia vypocita a vrati vzdialenost od predmetu
 *
 * \param vyska vyska v akej je umiestneny meraci predmet
 * \param alfa uhol
 *
 */
double vypocitajVzdialenist(double vyska,double alfa)
{
    return vyska/cfrac_tan(alfa,POCET_ITERACII);
}

/** \brief nastavVysku ak bol nastaveny
 *
 * \param pl hodnota na ktorej sa zistuje ci sa nachadza prepinac -c
 * \param argv pole argumentov
 * \param vyska premenna do ktorej sa ulozi vyska meracieho predmetu
 *
 */
void nastavVysku(int *pl,char *argv[],double *vyska,bool *konec)
{
    bool err=false;
    if(strcmp(argv[*pl],"-c")==0)
    {
        *vyska=prevodZnakovNaCisla(argv[*pl+1],&err);
        *pl+=2;
        if(err)
        {
            printf("parameter za -c musi byt cislo\n");
            *konec=true;
        }
        if(!*konec&&(*vyska>MAX_VYSKA||*vyska<=0))
        {
            printf("Vyska musi byt zadana v rozmedzi 0< x <=100\n");
            *konec=true;
        }
    }
}
