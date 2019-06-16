/**
Autor: Michal Ondrejó
Login: xondre08
Názov: Projekt č.1
Dátum: 9.11.2014
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#define NEDOSTATOK_PARAMETROV 100
#define CHYBNE_PARAMETRE 300
#define CHYBNY_PARAMETER_3 3
#define CHYBNY_PARAMETER_4 4
#define CHYBNY_PARAMETER_5 5
#define CHYBNY_PARAMETER_6 6
#define NESPRAVY_POCET_ARG_ROW 10
#define NESPRAVY_POCET_ARG_COL 11
#define NESPRAVY_POCET_ARG_ROWS 12
#define NESPRAVY_POCET_ARG_COLS 13
#define NESPRAVY_POCET_ARG_RANGE 14
#define CHYBA_ARG 15
#define NEPLATNA_OPERACIA 20
#define NEBOLI_VYBRANE_ZIADNE_BUNKY -250
void help();
long prevodZnakovNaCisla(char *cislo,bool *err);
void nacitajString(char *s,bool *dalsiStlpec,bool *dalsiRiadok,bool *EndOfFile);
long nacitajCislo(bool *err,bool *dalsiStlpec,bool *dalsiRiadok,bool *EndOfFile);
double funkcia (int operacia,int zaciatokRiadku,int zaciatokStlpcov,int konecRiadku,int konecStlpcov);

/** \brief main(int argc, char *argv[])
 *
 * \param argc počet argumentov
 * \param argv pole argumentov
 *
 */
int main(int argc, char *argv[])
{
    //printf("Pocet argumentov: %d \n",argc);
    if(argc==1)
    {
        printf("Neboli zadane ziadne parametre\n");
        exit(NEDOSTATOK_PARAMETROV);
    }
    //printf("Operacia:%s \n",argv[1]);
    if (argc==2)
    {
        if(strcmp(argv[1],"--help") == 0)
        {
            help();
        }
        else
        {
            printf("Chybne zadane parametre!\n");
            exit(CHYBNE_PARAMETRE);
        }
    }
/**
zistenie či sa v argumentoch nenachádza namiesto (kladných) čísiel niečo iné
*/
    bool error=false;
    switch(argc)
    {
        int c;
        case 7:
            c=prevodZnakovNaCisla(argv[6],&error);
            if((c)<=0)
                exit(CHYBNY_PARAMETER_6);
        case 6:if((c=prevodZnakovNaCisla(argv[5],&error))<=0)
                exit(CHYBNY_PARAMETER_5);
        case 5:if((c=prevodZnakovNaCisla(argv[4],&error))<=0)
                exit(CHYBNY_PARAMETER_4);
        case 4:if((c=prevodZnakovNaCisla(argv[3],&error))<=0)
                exit(CHYBNY_PARAMETER_3);
        if(error)
            exit(CHYBNE_PARAMETRE);
    }
    if(argc>2){ /** operacie, ktore sa spravia, ak bolo zadaných viac, ako 2 argumenty*/
    /**vyber buniek*/
    int zaciatokRiadku=0,zaciatokStlpcov=0,konecRiadku=0,konecStlpcov=0;
    if(strcmp(argv[2],"row")==0)
    {
        if(argc!=4)    //zistenie, či je zadany spravny pocet argumentov
        {
            printf("chyba \"row\", nespravny pocet argumentov\n");
            exit(NESPRAVY_POCET_ARG_ROW);
        }
        zaciatokRiadku=prevodZnakovNaCisla(argv[3],&error)-1;
        zaciatokStlpcov=0;
        konecRiadku=prevodZnakovNaCisla(argv[3],&error);
        konecStlpcov=1024;
    }
    else
    if(strcmp(argv[2],"col")==0){
        if(argc!=4)    //zistenie, či je zadany spravny pocet argumentov
        {
            printf("chyba \"col\", nespravny pocet argumentov\n");
            exit(NESPRAVY_POCET_ARG_COL);
        }
        zaciatokRiadku=0;
        zaciatokStlpcov=prevodZnakovNaCisla(argv[3],&error)-1;
        konecRiadku=1024;
        konecStlpcov=prevodZnakovNaCisla(argv[3],&error);
    }else
    if(strcmp(argv[2],"rows")==0){
        if(argc!=5)    //zistenie, či je zadany spravny pocet argumentov
        {
            printf("chyba \"rows\", nespravny pocet argumentov\n");
            exit(NESPRAVY_POCET_ARG_ROWS);
        }
        /** Ošetrenie chybných parametrov*/
        if(prevodZnakovNaCisla(argv[4],&error)<prevodZnakovNaCisla(argv[3],&error))
        {
            printf("3. argument: \"%s\" musi byt mensi ako 4. argument: \"%s\"\n",argv[3],argv[4]);
            exit(CHYBA_ARG);
        }
        zaciatokRiadku=prevodZnakovNaCisla(argv[3],&error)-1;
        zaciatokStlpcov=0;
        konecRiadku=prevodZnakovNaCisla(argv[4],&error);
        konecStlpcov=1024;
    }else
    if(strcmp(argv[2],"cols")==0){
        if(argc!=5)    //zistenie, či je zadany spravny pocet argumentov
        {
            printf("chyba \"cols\", nespravny pocet argumentov\n");
            exit(NESPRAVY_POCET_ARG_COLS);
        }
        /** Ošetrenie chybných parametrov*/
        if(prevodZnakovNaCisla(argv[4],&error)<prevodZnakovNaCisla(argv[3],&error))
        {
            printf("3. argument: \"%s\" musi byt mensi ako 4. argument: \"%s\"\n",argv[3],argv[4]);
            exit(CHYBA_ARG);
        }
        zaciatokRiadku=0;
        zaciatokStlpcov=prevodZnakovNaCisla(argv[3],&error)-1;
        konecRiadku=1024;
        konecStlpcov=prevodZnakovNaCisla(argv[4],&error);
    }else
    if(strcmp(argv[2],"range")==0){
        if(argc!=7)    //zistenie, či je zadany spravny pocet argumentov
        {
            printf("chyba \"range\", nespravny pocet argumentov\n");
            exit(NESPRAVY_POCET_ARG_RANGE);
        }
        /** Ošetrenie chybných parametrov*/
        if(prevodZnakovNaCisla(argv[4],&error)<prevodZnakovNaCisla(argv[3],&error)||prevodZnakovNaCisla(argv[6],&error)<prevodZnakovNaCisla(argv[5],&error))
        {printf("3. argument: \"%s\" musi byt mensi ako 4. argument: \"%s\" a\n5. argument: \"%s\" musi byt mensi ako 6. argument: \"%s\"\n",argv[3],argv[4],argv[5],argv[6]);
            exit(CHYBA_ARG);
        }
        zaciatokRiadku=prevodZnakovNaCisla(argv[3],&error)-1;
        zaciatokStlpcov=prevodZnakovNaCisla(argv[5],&error)-1;
        konecRiadku=prevodZnakovNaCisla(argv[4],&error);
        konecStlpcov=prevodZnakovNaCisla(argv[6],&error);
    }
    /**operace*/
    if (strcmp(argv[1],"select")==0) {
       funkcia(0,zaciatokRiadku,zaciatokStlpcov,konecRiadku,konecStlpcov);
    }else
    if (strcmp(argv[1],"min")==0) {
        //nájdenie min hodnoty
        printf("%.10g\n",funkcia(1,zaciatokRiadku,zaciatokStlpcov,konecRiadku,konecStlpcov));
    }else
    if (strcmp(argv[1],"max")==0) {
        //nájdenie najväčšej hodnoty
       printf("%.10g\n",funkcia(2,zaciatokRiadku,zaciatokStlpcov,konecRiadku,konecStlpcov));
    }else
    if (strcmp(argv[1],"sum")==0) {
        //spocítanie všetkych vybraných čísel
        printf("%.10g\n",funkcia(3,zaciatokRiadku,zaciatokStlpcov,konecRiadku,konecStlpcov));
    }else
    if (strcmp(argv[1],"avg")==0) {
        //aritmetický priemer vybranych čisel
        printf("%.10g\n",funkcia(4,zaciatokRiadku,zaciatokStlpcov,konecRiadku,konecStlpcov));
    }else
    {
        printf("Nesprávny názov operacie!\n");
        exit(NEPLATNA_OPERACIA);
    }
    }
    return 0;
}
/** \brief
 * \param operacia: hodnota 0 - select
 *                          1 - min
 *                          2 - max
 *                          3 - sum
 *                          4 - average
 *
 * \return navráti požadovanú hodnotu
 */
double funkcia (int operacia,int zaciatokRiadku,int zaciatokStlpcov,int konecRiadku,int konecStlpcov)
{
    bool dalsiStlpec,dalsiRiadok,EndOfFile=false;
    long sum=0;
    long min=LONG_MAX ,max=LONG_MIN;
    int pocet=0;
    bool error=false;
    for(int i=0;i<konecRiadku&&!EndOfFile;i++) /**Riadky*/
    {
        for(;i<zaciatokRiadku&&!EndOfFile;i++)              //preskocenie nepoužitích riadkov
        {
            while(!dalsiRiadok)
            {
                switch(operacia)
                {
                    char  p[1024];
                    case 0:  //select
                        nacitajString(p,&dalsiStlpec,&dalsiRiadok,&EndOfFile);
                        break;
                    default: //ostatne operacie
                        nacitajCislo(&error,&dalsiStlpec,&dalsiRiadok,&EndOfFile);
                }
            }
                dalsiRiadok=false;
        }
        for(int j=0;j<konecStlpcov&&!dalsiRiadok&&!EndOfFile;j++) /**Stlpce*/
        {
            for(;j<zaciatokStlpcov&&!EndOfFile;j++)/**preskocenie nevybraných na začiatku riadka*/
            {
                char  p[1024];
                switch(operacia){
                    case 0:  //select
                        nacitajString(p,&dalsiStlpec,&dalsiRiadok,&EndOfFile);
                        break;
                    default: //ostatne operacie
                        nacitajCislo(&error,&dalsiStlpec,&dalsiRiadok,&EndOfFile);
                }
            }
            /**
                Ak bol v predchadzajucom cykle nacitany EOF
            */
            error=false;
            if(!EndOfFile)
            switch(operacia)
            {
                case 0: { /**select*/
                        char  p[1024];
                        nacitajString(p,&dalsiStlpec,&dalsiRiadok,&EndOfFile);
                        long cislo;
                        cislo=prevodZnakovNaCisla(p,&error);
                        if(!error)/** kontrola, či je číslo alebo slovo*/
                        {
                            printf("%.10g\n",(double)cislo);
                        }else
                        {
                            printf("%s\n",p);
                        }
                    }
                    break;
                case 1: /**min*/
                    {
                        long c=nacitajCislo(&error,&dalsiStlpec,&dalsiRiadok,&EndOfFile);
                        if(!error)
                            if(min >c)
                            {
                                min=c;
                            }
                    }
                    break;
                case 2: /**max*/
                    {
                        long c=nacitajCislo(&error,&dalsiStlpec,&dalsiRiadok,&EndOfFile);
                        if(!error)
                            if(max <c)
                            {
                                max=c;
                            }
                    }
                    break;
                case 3: /**sum*/
                    {
                        long c =nacitajCislo(&error,&dalsiStlpec,&dalsiRiadok,&EndOfFile);
                        if(!error)
                            sum+=c;
                    }
                    break;
                case 4: /**avg*/
                    {
                        long c=nacitajCislo(&error,&dalsiStlpec,&dalsiRiadok,&EndOfFile);
                        if(!error)
                        {
                            sum+=c;
                            pocet++;
                        }

                    }
                    break;
                default:
                    printf("CHYBA: nespravna operacia\n");
            }
        }
        while(dalsiRiadok!=true&&!EndOfFile)/**preskocenie nepoužitých  na konci riadka*/
        {
            char  p[1024];
            switch(operacia){
                case 0:  //select
                    nacitajString(p,&dalsiStlpec,&dalsiRiadok,&EndOfFile);
                    break;
                default: //ostatne operacie
                    nacitajCislo(&error,&dalsiStlpec,&dalsiRiadok,&EndOfFile);
            }
        }
        dalsiRiadok=false;
    }
    while(!EndOfFile)//preskocenie nepoužitých  na konci
    {
        char  p[1024];
        switch(operacia){
            case 0:  //select
                nacitajString(p,&dalsiStlpec,&dalsiRiadok,&EndOfFile);
                break;
            default: //ostatne operacie
                nacitajCislo(&error,&dalsiStlpec,&dalsiRiadok,&EndOfFile);
        }
    }
    /**
    návrat výsledkov z operácii
    */
    switch(operacia)
    {
        case 0:
            return 0;
            break;
        case 1:
            return min;
                break;
        case 2:
            return max;
            break;
        case 3:
            return sum;
            break;
        case 4:
            if(pocet<1){
                printf("CHYBA:neboli vybrane ziadne bunky\n");
                exit(NEBOLI_VYBRANE_ZIADNE_BUNKY);
            }
            return sum/(double)pocet;
            break;
        default:
            printf("CHYBA: nespravna operacia\n");
            exit(EXIT_FAILURE);

    }
}
/** Funkcia, na načítanie čísla zo vstupu
 * \param err boolovska premenna do ktorej ulozi hodnotu true, ak bol prevod na cislo uspesny
 * \param dalsiStlpec boolovska premenna do ktorej ulozi hodnotu true, ak bol nacitany prazdny znak
 * \param dalsiRiadok boolovska premenna do ktorej ulozi hodnotu true, ak bol nacitany koniec riadka
 * \param EndOfFile boolovska premenna do ktorej ulozi hodnotu true, ak bol nacitany koniec subora
 */
 long nacitajCislo(bool *err,bool *dalsiStlpec,bool *dalsiRiadok,bool *EndOfFile)
{
    char s[1024];
    nacitajString(s,dalsiStlpec,dalsiRiadok,EndOfFile);
    bool de=false;
    long d =prevodZnakovNaCisla(s,&de);
    *err=de;
     return d;
}
/** Funkcia, na načítanie pola znakov zo vstupu
 * \param s: pole znakov do ktoreho sa uozi nacitany string
 * \param dalsiStlpec boolovska premenna do ktorej ulozi hodnotu true, ak bol nacitany prazdny znak
 * \param dalsiRiadok boolovska premenna do ktorej ulozi hodnotu true, ak bol nacitany koniec riadka
 * \param EndOfFile boolovska premenna do ktorej ulozi hodnotu true, ak bol nacitany koniec subora
 */
void nacitajString(char *s,bool *dalsiStlpec,bool *dalsiRiadok,bool *EndOfFile)
{
    int i=1;
    int c;
    while ((c=getchar()) == ' ' ){ } /**Vynechanie medzier znakov*/
    s[0]=c;
    if(c!=EOF)
    while ((c=getchar())!=EOF && (c != '\n')&& (c != ' '))
    {
         s[i]=c;
         i++;
    }
    s[i]='\0';
    *dalsiRiadok=(c=='\n');
    *dalsiStlpec=(c==' ');
    *EndOfFile=(c==EOF);
}
/** Zmena znakov v retazci na číslo
 * \param cislo retazec v ktorom je ulozena hodnota ktoru prevadzame na cislo
 * \param err do premennej sa ulozi TRUE, ak sa znaky z retazca nedaju zmenit na cislo
 * \return navrati hodnotu cisla ak bolo v retazci "cislo" celociselna hodnota alebo 0 ked sa nepodarilo previest na cislo
 */
long prevodZnakovNaCisla(char *cislo,bool *err)
{
    long i=0;
    char * zvysok;
    i=strtol(cislo,&zvysok,10);
    if(*zvysok!=0){
        *err=true;
        return 0;
    }
    return i;
}
/** vypise napovedu k programu
 */
void help()
{
    printf("Program sa spusti v nasledujucej podobe:\n"
               "(./proj1 znaci umiestnenie a nazov programu: \n"
               "\"./proj1 --help\" alebo \"./proj1 operacia vyber_buniek\"\n"
               "Argumenty programu:\n"
               "- \"--help\" sposoby, ze program vypise napovedu pouzivania programu a skonci.\n"
               "operacia reprezentuje jednu z nasledujicich operacii:\n"
               "- \"select\" znaci operaciu, ktora z danej tabulky len vybere a nasledovne vypise hodnoty danych buniek,\n"
               "- \"min\" znaci vyhladanie a nasledny vypis minimalnej hodnoty z daneho rozsahu buniek,\n"
               "- \"max\" znaci vyhladanie a nasledny vypis maximalnej hodnoty z daneho rozsahu buniek,\n"
               "- \"sum\" znaci vypocet a nasledny vypis sumy hodnot vsetkych vybranych buniek, \n"
               "- \"avg\" znaci vypocet a nasledny vypis aritmetickeho priemeru vybranych buniek.\n"
               "vyber_buniek reprezentuje rozsah tabulky, nad kterym ma byt prevedena dana operacia. Rozsah je definovany jednym z nasledujicich argumentov:\n"
               "- \"row X\" znaci vyber vsetkych buniek na riadku X (X > 0), \n"
               "- \"col X\" znaci vyber vsetkych buniek v stlpci X (X > 0), \n"
               "- \"rows X Y\" znaci vyber vsetkych buniek od riadku X (vcetne) az po Y (vcetne). 0 < X <= Y. \n"
               "- \"cols X Y\" znaci vyber vsetkych buniek od stlpca X (vcetne) az po Y (vcetne). 0 < X <= Y.\n"
               "- \"range A B X Y\" znaci vyber buniek od riadku A po riadok B a od stlpca X po stlpec Y (vcetne danych riadkov a stlpcov). 0 < A <= B, 0 < X <= Y.\n");

}
