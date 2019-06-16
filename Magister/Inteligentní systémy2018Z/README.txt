
!!! Na správny beh simuátoru je potrebné mať nainštalovaný program Domoticz (https://www.domoticz.com)

Obsah archívu:
	domoticz.db
	HouseSimulator.jar
	proj/
	README.txt

Spustenie aplikácie
========================================================
1.  Nainštalujte Domoticz
2.  Skopírujte databázu "domoticz.db" do zložky domoticz
    a spustite Domoticz s priloženou databázou
3.  Spustite simulačný program kompiláciou projektu:

        $ cd proj
        proj$ ant run
	
    alebo preloženou verziou .jar:
	
		$ java -jar HouseSimulator.jar
		
4.  Otvorí sa GUI okno so simulátorom




Ovládanie aplikácie:
==============================
1.  Zadajde IP adresu servera do kolónky (defaultne 'localhost')
2.  Zapnite obnovu zariadení (Start Updating)
3.  Namapujte senzory na simulátory označením zariadenia a tlačítkom 'Attach',
    zobrazí sa IDX zariadenia pri tlačítku.
4.  Spustite simuláciu 'Start' a simulátor začne odosielať nové hodnoty na server.
    Server bude reagovať na zmeny a stavy zariadený sa budú meniť v zozname zariadení.


IP (text field) - IP adresa servera Domoticz
Start Updating  - zapnúž/vypnúť automatické, periodické obnovovanie zariadení zo serveru Domoticz
Start           - spustenie simulácie
Stop            - zastavenie simulácie
Attach          - namapovanie simulátorov na jednotlivé senzory

