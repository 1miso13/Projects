	
/* c206.c **********************************************************}
{* T�ma: Dvousm�rn� v�zan� line�rn� seznam
**
**                   N�vrh a referen�n� implementace: Bohuslav K�ena, ��jen 2001
**                            P�epracovan� do jazyka C: Martin Tu�ek, ��jen 2004
**                                            �pravy: Bohuslav K�ena, ��jen 2015
**
** Implementujte abstraktn� datov� typ dvousm�rn� v�zan� line�rn� seznam.
** U�ite�n�m obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datov� abstrakce reprezentov�n prom�nnou
** typu tDLList (DL znamen� Double-Linked a slou�� pro odli�en�
** jmen konstant, typ� a funkc� od jmen u jednosm�rn� v�zan�ho line�rn�ho
** seznamu). Definici konstant a typ� naleznete v hlavi�kov�m souboru c206.h.
**
** Va��m �kolem je implementovat n�sleduj�c� operace, kter� spolu
** s v��e uvedenou datovou ��st� abstrakce tvo�� abstraktn� datov� typ
** obousm�rn� v�zan� line�rn� seznam:
**
**      DLInitList ...... inicializace seznamu p�ed prvn�m pou�it�m,
**      DLDisposeList ... zru�en� v�ech prvk� seznamu,
**      DLInsertFirst ... vlo�en� prvku na za��tek seznamu,
**      DLInsertLast .... vlo�en� prvku na konec seznamu, 
**      DLFirst ......... nastaven� aktivity na prvn� prvek,
**      DLLast .......... nastaven� aktivity na posledn� prvek, 
**      DLCopyFirst ..... vrac� hodnotu prvn�ho prvku,
**      DLCopyLast ...... vrac� hodnotu posledn�ho prvku, 
**      DLDeleteFirst ... zru�� prvn� prvek seznamu,
**      DLDeleteLast .... zru�� posledn� prvek seznamu, 
**      DLPostDelete .... ru�� prvek za aktivn�m prvkem,
**      DLPreDelete ..... ru�� prvek p�ed aktivn�m prvkem, 
**      DLPostInsert .... vlo�� nov� prvek za aktivn� prvek seznamu,
**      DLPreInsert ..... vlo�� nov� prvek p�ed aktivn� prvek seznamu,
**      DLCopy .......... vrac� hodnotu aktivn�ho prvku,
**      DLActualize ..... p�ep�e obsah aktivn�ho prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na dal�� prvek seznamu,
**      DLPred .......... posune aktivitu na p�edchoz� prvek seznamu, 
**      DLActive ........ zji��uje aktivitu seznamu.
**
** P�i implementaci jednotliv�ch funkc� nevolejte ��dnou z funkc�
** implementovan�ch v r�mci tohoto p��kladu, nen�-li u funkce
** explicitn� uvedeno n�co jin�ho.
**
** Nemus�te o�et�ovat situaci, kdy m�sto leg�ln�ho ukazatele na seznam 
** p�ed� n�kdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodn� komentujte!
**
** Terminologick� pozn�mka: Jazyk C nepou��v� pojem procedura.
** Proto zde pou��v�me pojem funkce i pro operace, kter� by byly
** v algoritmick�m jazyce Pascalovsk�ho typu implemenov�ny jako
** procedury (v jazyce C procedur�m odpov�daj� funkce vracej�c� typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozorn�n� na to, �e do�lo k chyb�.
** Tato funkce bude vol�na z n�kter�ch d�le implementovan�ch operac�.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* glob�ln� prom�nn� -- p��znak o�et�en� chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L p�ed jeho prvn�m pou�it�m (tzn. ��dn�
** z n�sleduj�c�ch funkc� nebude vol�na nad neinicializovan�m seznamem).
** Tato inicializace se nikdy nebude prov�d�t nad ji� inicializovan�m
** seznamem, a proto tuto mo�nost neo�et�ujte. V�dy p�edpokl�dejte,
** �e neinicializovan� prom�nn� maj� nedefinovanou hodnotu.
**/
    L->First=NULL;					//inicializacia
	L->Last=NULL;
	L->Act=NULL;	
 //solved = FALSE;                   /* V p��pad� �e�en�, sma�te tento ��dek! */
}

void DLDisposeList (tDLList *L) {
/*
** Zru�� v�echny prvky seznamu L a uvede seznam do stavu, v jak�m
** se nach�zel po inicializaci. Ru�en� prvky seznamu budou korektn�
** uvoln�ny vol�n�m operace free. 
**/
	tDLElemPtr prvok=NULL;
	while(L->First!=NULL)
	{								//posun a uvolnenie prvov
		prvok=L->First;				
		L->First=prvok->rptr;		
		free(prvok);				
	}
	L->Act=NULL;					//Last a Act na NULL
	L->Last=NULL;					
// solved = FALSE;                   /* V p��pad� �e�en�, sma�te tento ��dek! */
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vlo�� nov� prvek na za��tek seznamu L.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/
	tDLElemPtr next=L->First;
	if((L->First= (tDLElemPtr) malloc(sizeof(struct tDLElem)))==NULL) //alokacia
	{
		DLError();
	}
	else
	{	
		L->First->data=val;			//data
		L->First->rptr=next;		//ukazatel na dalsi prvok
		L->First->lptr=NULL;
		if(L->Last==NULL)			//ak bol prazdny a pridavame prvu hodnotu
		{
			
			//L->First->lptr=NULL;
			L->Last=L->First;		
		}
		else
		next->lptr=L->First;		//2. prvok ukazuje na 1.
	}
	
 //solved = FALSE;                   /* V p��pad� �e�en�, sma�te tento ��dek! */
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vlo�� nov� prvek na konec seznamu L (symetrick� operace k DLInsertFirst).
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/ 	
	tDLElemPtr least=L->Last;
	if((L->Last= (tDLElemPtr) malloc(sizeof(struct tDLElem)))==NULL) //alokacia
	{
		DLError();
	}
	else
	{	
		L->Last->data=val;			//data
		L->Last->lptr=least;		//ukazatel na predchadzajuci prvok
		L->Last->rptr=NULL;
		if(L->First==NULL)			//ak bol prazdny
		{
			L->First=L->Last;
		}
		else
		least->rptr=L->Last;		//predposledny prvok ukazuje na posledny
	}
	
//solved = FALSE;                   /* V p��pad� �e�en�, sma�te tento ��dek! */
}

void DLFirst (tDLList *L) {
/*
** Nastav� aktivitu na prvn� prvek seznamu L.
** Funkci implementujte jako jedin� p��kaz (nepo��t�me-li return),
** ani� byste testovali, zda je seznam L pr�zdn�.
**/
	L->Act=L->First;

//solved = FALSE;                   /* V p��pad� �e�en�, sma�te tento ��dek! */
}

void DLLast (tDLList *L) {
/*
** Nastav� aktivitu na posledn� prvek seznamu L.
** Funkci implementujte jako jedin� p��kaz (nepo��t�me-li return),
** ani� byste testovali, zda je seznam L pr�zdn�.
**/
	L->Act=L->Last;	
//solved = FALSE;                   /* V p��pad� �e�en�, sma�te tento ��dek! */
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prost�ednictv�m parametru val vr�t� hodnotu prvn�ho prvku seznamu L.
** Pokud je seznam L pr�zdn�, vol� funkci DLError().
**/
	if(L->First==NULL)				//test ci je prazdny
		DLError();
	else
		*val=L->First->data;
	
 //solved = FALSE;                   /* V p��pad� �e�en�, sma�te tento ��dek! */
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prost�ednictv�m parametru val vr�t� hodnotu posledn�ho prvku seznamu L.
** Pokud je seznam L pr�zdn�, vol� funkci DLError().
**/
	if(L->Last==NULL)				//test ci je prazdny
		DLError();
	else
		*val=L->Last->data;
 //solved = FALSE;                   /* V p��pad� �e�en�, sma�te tento ��dek! */
}

void DLDeleteFirst (tDLList *L) {
/*
** Zru�� prvn� prvek seznamu L. Pokud byl prvn� prvek aktivn�, aktivita 
** se ztr�c�. Pokud byl seznam L pr�zdn�, nic se ned�je.
**/
	if(L->First != NULL)				//test ci je prazdny
	{
		if(L->First == L->Act)			//ak je aktivny prvy prvok tak sa zrusi aktivnost
		{
			L->Act=NULL;
		}
		
		tDLElemPtr prvok =L->First;		//pointer "prvok" na prvy prvok
		L->First=L->First->rptr;		//First na 2. prvok
		if(L->Last==prvok)				//ak bol posledny prvok
		{
			L->Last=NULL;
		}
		else
		L->First->lptr=NULL;			//kedze sme prvy prvok odstranili, musime nastavit pointer na druhom prvku na NULL
		
		free(prvok);					//uvlonenie
	}
	
 //solved = FALSE;                   /* V p��pad� �e�en�, sma�te tento ��dek! */
}	

void DLDeleteLast (tDLList *L) {
/*
** Zru�� posledn� prvek seznamu L. Pokud byl posledn� prvek aktivn�,
** aktivita seznamu se ztr�c�. Pokud byl seznam L pr�zdn�, nic se ned�je.
**/ 
	if(L->Last != NULL)				//test ci je prazdny
	{
		if(L->Last == L->Act)		//ak je aktivny posledny prvok tak sa zrusi aktivnost
		{
			L->Act=NULL;
		}
		
		tDLElemPtr prvok =L->Last;	//pointer "prvok" na posledny prvok
		L->Last=L->Last->lptr;		//Last na predposledny prvok
		if(L->First==prvok)			//ak bol prvy prvok
		{
			L->First=NULL;
		}
		else
		L->Last->rptr=NULL;			//kedze sme posledny prvok odstranili, musime nastavit pointer na predposlednom prvku na NULL
		free(prvok);				//uvlonenie
	}
	
 //solved = FALSE;                   /* V p��pad� �e�en�, sma�te tento ��dek! */
}

void DLPostDelete (tDLList *L) {
/*
** Zru�� prvek seznamu L za aktivn�m prvkem.
** Pokud je seznam L neaktivn� nebo pokud je aktivn� prvek
** posledn�m prvkem seznamu, nic se ned�je.
**/
	if((L->Act != NULL) && (L->Act->rptr != NULL)) //testovanie ci je aktivny a ci nieje posledny prvok
	{
		tDLElemPtr prvok = L->Act->rptr;
		L->Act->rptr=L->Act->rptr->rptr;	//aktivny prvok musi ukazovat na dalsi prvok
		if(L->Last==prvok)					//Ak je prvok posledny
			L->Last=L->Act;					//kedze aktivny prvok je posledny, tak Last nan musi odkazovat
		else
			L->Act->rptr->lptr=L->Act;		//dalsi prvok musi odkazovat sp�t na aktivny prvok
		free(prvok);
	}
		
 //solved = FALSE;                   /* V p��pad� �e�en�, sma�te tento ��dek! */
}

void DLPreDelete (tDLList *L) {
/*
** Zru�� prvek p�ed aktivn�m prvkem seznamu L .
** Pokud je seznam L neaktivn� nebo pokud je aktivn� prvek
** prvn�m prvkem seznamu, nic se ned�je.
**/
	if((L->Act != NULL) && (L->Act->lptr != NULL))//testovanie ci je aktivny a ci nieje prvy prvok
	{
		tDLElemPtr prvok = L->Act->lptr;
		L->Act->lptr=L->Act->lptr->lptr;	//aktivny prvok musi ukazovat na predchadzajuci prvok
		if(L->First==prvok)					//Ak je prvok posledny
			L->First=L->Act;
		else
			L->Act->lptr->rptr=L->Act;		//dalsi prvok musi odkazovat sp�t na aktivny prvok
		free(prvok);
	}
			
 //solved = FALSE;                   /* V p��pad� �e�en�, sma�te tento ��dek! */
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vlo�� prvek za aktivn� prvek seznamu L.
** Pokud nebyl seznam L aktivn�, nic se ned�je.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/
	if(L->Act!=NULL) 
	{
		tDLElemPtr prvok;
		if((prvok= (tDLElemPtr) malloc(sizeof(struct tDLElem)))==NULL) //alokacia
			DLError();
		else
		{
			prvok->data=val;			//hodnota
			prvok->rptr=L->Act->rptr;	//vlozeny prvok bude ukazovat na prvok za aktivnim prvkom
			L->Act->rptr=prvok;			//aktivny prvok zasa ukazuje na vlozeny prvok
			prvok->lptr=L->Act;			//sp�tny ukazatel z vlozeneho prvku na aktivny prvok
			if(L->Act==L->Last)			//Ak bol aktivny prvok na konci
				L->Last=prvok;			//tak musi Last nan ukazovat
			else
			prvok->rptr->lptr=prvok;	//dalsi prvok musi odkazovat sp�t na prvok
		}
	}
	
 //solved = FALSE;                   /* V p��pad� �e�en�, sma�te tento ��dek! */
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vlo�� prvek p�ed aktivn� prvek seznamu L.
** Pokud nebyl seznam L aktivn�, nic se ned�je.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/
	if(L->Act!=NULL)
	{
		tDLElemPtr prvok;
		if((prvok= (tDLElemPtr) malloc(sizeof(struct tDLElem)))==NULL)//alokacia
			DLError();
		else
		{
			prvok->data=val;
			prvok->lptr=L->Act->lptr;	//vlozeny prvok bude ukazovat na prvok pred aktivnim prvkom
			L->Act->lptr=prvok;			//aktivny prvok zasa ukazuje na vlozeny prvok
			
			prvok->rptr=L->Act;			//ukazatel z vlozeneho prvku na aktivny
			if(L->Act==L->First)		//Ak bol aktivny prvok na zaciatku
				L->First=prvok;			//tak musi First nan ukazovat
			else
			prvok->lptr->rptr=prvok;	//predchadzajuci prvok musi odkazovat na prvok
		}
	}
	
// solved = FALSE;                   /* V p��pad� �e�en�, sma�te tento ��dek! */
}

void DLCopy (tDLList *L, int *val) {
/*
** Prost�ednictv�m parametru val vr�t� hodnotu aktivn�ho prvku seznamu L.
** Pokud seznam L nen� aktivn�, vol� funkci DLError ().
**/
	if(L->Act!=NULL)		//test aktivnosti
	{
		*val=L->Act->data;
	}
	else
		DLError();
	
	
 //solved = FALSE;                   /* V p��pad� �e�en�, sma�te tento ��dek! */
}

void DLActualize (tDLList *L, int val) {
/*
** P�ep�e obsah aktivn�ho prvku seznamu L.
** Pokud seznam L nen� aktivn�, ned�l� nic.
**/
	if(L->Act!=NULL)		//test aktivnosti
		L->Act->data=val;	//data
	
 //solved = FALSE;                   /* V p��pad� �e�en�, sma�te tento ��dek! */
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na n�sleduj�c� prvek seznamu L.
** Nen�-li seznam aktivn�, ned�l� nic.
** V�imn�te si, �e p�i aktivit� na posledn�m prvku se seznam stane neaktivn�m.
**/
	if(L->Act!=NULL)		//test aktivnosti
		L->Act=L->Act->rptr;//posun o jedno dalej
	
 //solved = FALSE;                   /* V p��pad� �e�en�, sma�te tento ��dek! */
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na p�edchoz� prvek seznamu L.
** Nen�-li seznam aktivn�, ned�l� nic.
** V�imn�te si, �e p�i aktivit� na prvn�m prvku se seznam stane neaktivn�m.
**/
	if(L->Act!=NULL)		//test aktivnosti
		L->Act=L->Act->lptr;//posun o jedno vpred
	
 //solved = FALSE;                   /* V p��pad� �e�en�, sma�te tento ��dek! */
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivn�, vrac� nenulovou hodnotu, jinak vrac� 0.
** Funkci je vhodn� implementovat jedn�m p��kazem return.
**/
	return L->Act==NULL ? 0 : 1 ;//test aktivnosti
	
 //solved = FALSE;                   /* V p��pad� �e�en�, sma�te tento ��dek! */
}

/* Konec c206.c*/
