----------- IDS Projekt -----------
-- Július Kéry 		[xkeryj00] --
-- Michal Ondrejó 	[xondre08] --
-----------------------------------

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DROP TABLE Pracovne_pozicie CASCADE CONSTRAINTS;
DROP TABLE Plny_uvazok CASCADE CONSTRAINTS;
DROP TABLE Ciastocny_uvazok CASCADE CONSTRAINTS;
DROP TABLE Benefity CASCADE CONSTRAINTS;
DROP TABLE Uradnik CASCADE CONSTRAINTS;
DROP TABLE Firma CASCADE CONSTRAINTS;
DROP TABLE Odbor CASCADE CONSTRAINTS;
DROP TABLE Klient CASCADE CONSTRAINTS;
DROP TABLE Specializacia CASCADE CONSTRAINTS;
DROP TABLE Certifikaty CASCADE CONSTRAINTS;
DROP TABLE Skolenie CASCADE CONSTRAINTS;
DROP TABLE Preukaz CASCADE CONSTRAINTS;

DROP TABLE Pozicia_Benefity CASCADE CONSTRAINTS;
DROP TABLE Pozicia_Specializacia CASCADE CONSTRAINTS;
DROP TABLE Firma_Odbor CASCADE CONSTRAINTS;
DROP TABLE Specializacia_Certifikat CASCADE CONSTRAINTS;
DROP TABLE Specializacia_Skolenie CASCADE CONSTRAINTS;
DROP TABLE Specializacia_Preukaz CASCADE CONSTRAINTS;
DROP TABLE Klient_Pozicia CASCADE CONSTRAINTS;
DROP TABLE Klient_Odbor CASCADE CONSTRAINTS;
DROP TABLE Klient_Skolenie CASCADE CONSTRAINTS;
DROP TABLE Klient_Certifikat CASCADE CONSTRAINTS;
DROP TABLE Klient_Preukaz CASCADE CONSTRAINTS;

DROP SEQUENCE incID;
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

--------------------
--Vytvorenie Entit--
--------------------

CREATE TABLE Pracovne_pozicie(
ID_PP NUMBER NOT NULL,
datumPridania DATE NOT NULL,
datumNastupu DATE NOT NULL,
naplnPrace VARCHAR2(100) NOT NULL,
hrubaMzda NUMBER NOT NULL,
miesto VARCHAR2(40) NOT NULL,
obsadenost NUMBER NOT NULL,
ID_U NUMBER NOT NULL,
ID_F NUMBER NOT NULL
);

CREATE TABLE Plny_uvazok(
ID_PP NUMBER NOT NULL
);

CREATE TABLE Ciastocny_uvazok(
ID_PP NUMBER NOT NULL,
dlzkaPracovnejDoby NUMBER
);

CREATE TABLE Benefity(
ID_B NUMBER NOT NULL,
popisBenefitu VARCHAR2(40) NOT NULL
);

CREATE TABLE Uradnik(
ID_U NUMBER NOT NULL,
krstneMeno_U VARCHAR2(20) NOT NULL,
priezvisko_U VARCHAR2(20) NOT NULL,
denNastupu DATE NOT NULL,
datumNarodenia_U DATE NOT NULL,
mzda NUMBER NOT NULL
);

CREATE TABLE Firma(
ID_F NUMBER NOT NULL,
nazovFirmy VARCHAR2(50) NOT NULL UNIQUE,
zodpovednaOsoba VARCHAR2(50) NOT NULL,
adresa VARCHAR2(100) NOT NULL,
sidloFirmy VARCHAR2(100) NOT NULL,
pocetZamestnancov NUMBER NOT NULL
);

CREATE TABLE Odbor(
ID_O NUMBER NOT NULL,
nazovOdboru VARCHAR2(50) NOT NULL UNIQUE
);

-- bankovy ucet je v tvare IBAN
CREATE TABLE Klient(
ID_K NUMBER NOT NULL,
krstneMeno_K VARCHAR2(20) NOT NULL,
priezvisko_K VARCHAR2(20) NOT NULL,
titul VARCHAR2(10) NOT NULL,
datumNarodenia_K DATE NOT NULL,
email VARCHAR2(30) NOT NULL CHECK (email LIKE '%_@__%.__%'),
bankovyUcet VARCHAR(25) NOT NULL UNIQUE 
);

CREATE TABLE Specializacia(
ID_SP NUMBER NOT NULL,
pozadovanyTitul VARCHAR2(10),
nazov VARCHAR2(50) NOT NULL
);

CREATE TABLE Certifikaty(
ID_C NUMBER NOT NULL,
nazovCertifikatu VARCHAR2(15) NOT NULL UNIQUE,
informaciaOCertifikatu VARCHAR2(100) NOT NULL
);
CREATE TABLE Skolenie(
ID_SK NUMBER NOT NULL,
nazovSkolenia VARCHAR2(15) NOT NULL UNIQUE,
informaciaOSkoleni VARCHAR2(100) NOT NULL
);
CREATE TABLE Preukaz(
ID_PR NUMBER NOT NULL,
nazovPreukazu VARCHAR2(15) NOT NULL UNIQUE,
informaciaOPreukaze VARCHAR2(100) NOT NULL
);

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

---------------------------
-- Vztahy medzi entitami --
---------------------------

CREATE TABLE Pozicia_Benefity(
ID_PP NUMBER NOT NULL,
ID_B NUMBER NOT NULL
);
CREATE TABLE Pozicia_Specializacia(
ID_PP NUMBER NOT NULL,
ID_SP NUMBER NOT NULL
);
CREATE TABLE Firma_Odbor(
ID_F NUMBER NOT NULL,
ID_O NUMBER NOT NULL
);
CREATE TABLE Specializacia_Certifikat(
ID_SP NUMBER NOT NULL,
ID_C NUMBER NOT NULL
);
CREATE TABLE Specializacia_Skolenie(
ID_SP NUMBER NOT NULL,
ID_SK NUMBER NOT NULL
);
CREATE TABLE Specializacia_Preukaz(
ID_SP NUMBER NOT NULL,
ID_PR NUMBER NOT NULL
);
CREATE TABLE Klient_Pozicia(
ID_K NUMBER NOT NULL,
ID_PP NUMBER NOT NULL
);
CREATE TABLE Klient_Odbor(
ID_K NUMBER NOT NULL,
ID_O NUMBER NOT NULL
);
CREATE TABLE Klient_Skolenie(
ID_K NUMBER NOT NULL,
ID_SK NUMBER NOT NULL,
DatumPreskolenia DATE NOT NULL
);
CREATE TABLE Klient_Certifikat(
ID_K NUMBER NOT NULL,
ID_C NUMBER NOT NULL
);
CREATE TABLE Klient_Preukaz(
ID_K NUMBER NOT NULL,
ID_PR NUMBER NOT NULL,
DatumPlatnosti DATE NOT NULL
);

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

---------------
--	TRIGGERY --
---------------

-- 1. Autoincrement ID --
CREATE SEQUENCE incID;
ALTER SESSION SET nls_date_format='dd.mm.yyyy';
CREATE OR REPLACE TRIGGER autoincrement
	BEFORE INSERT ON Klient
	FOR EACH ROW
BEGIN
	:new.ID_K := incID.nextval;
END autoincrement;
/
show errors
ALTER session SET nls_date_format='dd.mm.yyyy';

-- 2. IBAN Checker --
ALTER SESSION set nls_date_format='dd.mm.yyyy';
SET serveroutput ON;
CREATE OR REPLACE TRIGGER UcetCheck
	BEFORE INSERT OR UPDATE OF bankovyUcet ON Klient
	FOR EACH ROW
DECLARE
	acc Klient.bankovyUcet%TYPE;
	country VARCHAR2(2);
	bankNumber VARCHAR2(22);
	account VARCHAR2(10);
	foo NUMBER;
BEGIN
	acc := :NEW.bankovyUcet;
	country := SUBSTR(acc, 1, 2); 
	bankNumber := SUBSTR(acc, 3, 22); 
	account := SUBSTR(acc, 15, 10);
	foo := TO_NUMBER(SUBSTR(account, 1,1), '9') + TO_NUMBER(SUBSTR(account, 2,1), '9') + TO_NUMBER(SUBSTR(account, 3,1), '9') +
			TO_NUMBER(SUBSTR(account, 4,1), '9') + TO_NUMBER(SUBSTR(account, 5,1), '9') + TO_NUMBER(SUBSTR(account, 6,1), '9') +
			TO_NUMBER(SUBSTR(account, 7,1), '9') + TO_NUMBER(SUBSTR(account, 8,1), '9') + TO_NUMBER(SUBSTR(account, 9,1), '9') +
			TO_NUMBER(SUBSTR(account, 10,1), '9');
	-- kazdy cesky a slovensky IBAN ma 24 znakov		
	IF (LENGTH(acc) != 24) 
		THEN 
			RAISE_APPLICATION_ERROR(-22222,'!IBAN is too short. Must have 24 characters!');
	END IF;
	-- overenie ci prve 2 znaky su pismena
	IF (LENGTH(TRIM(TRANSLATE(country,'ABCDEFGHIJKLMNOPQRSTUVWXYZ',' '))) != NULL) 
		THEN 
			RAISE_APPLICATION_ERROR(-20333, '!IBAN must begin with 2 letters!');
	END IF;
	-- overenie ci ostatne znaky(3 - 22) su cisla
	IF (LENGTH(TRIM(TRANSLATE(bankNumber,'0123456789',' '))) != NULL) 
		THEN 
			RAISE_APPLICATION_ERROR(-20444, '!IBAN must have 22 numbers after 2 letters!');
	END IF;
	-- IBAN je validny, ak vysledok algoritmu je delitelny 11
	IF MOD(foo, 2) != 0 
		THEN 
			RAISE_APPLICATION_ERROR(-20555, '!IBAN is invalid!');
	END IF;
END UcetCheck;
/
show errors
ALTER SESSION set nls_date_format='dd.mm.yyyy';

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

---------------- 
--	PROCEDURY --
----------------
--	1. Procedura, ktora vypise ci specializacia vyzaduje titul alebo nie
--ALTER SESSION SET nls_date_format='dd.mm.yyyy'
SET serveroutput ON;
CREATE OR REPLACE PROCEDURE pozad_Titul(spID Specializacia.ID_SP%TYPE)
IS
	title Specializacia.pozadovanyTitul%TYPE;
BEGIN
	SELECT pozadovanyTitul
	INTO title
	FROM Specializacia
	WHERE Specializacia.ID_SP = spID;
	
	IF title IS NULL THEN
		dbms_output.put_line('Dana specializacia nevyzaduje ziaden titul.');
	ELSE
		dbms_output.put_line('Dana specializacia vyzaduje titul: *' || title || '*.');
  END IF;
EXCEPTION
	WHEN NO_DATA_FOUND
	THEN
		dbms_output.put_line('Specializacia s tymto ID nebola najdena.');
END;
/
show errors
--ALTER SESSION SET nls_date_format='dd.mm.yyyy'

-- 2. Vypise, ktory uradnici maju rovnaky, mensi alebo vacsi plat ako zadany uradnik
--ALTER SESSION SET nls_date_format='dd.mm.yyyy'
SET serveroutput ON;
CREATE OR REPLACE PROCEDURE uradnici(uradnikID Uradnik.ID_U%TYPE)
IS
	CURSOR Uinfo IS SELECT * FROM Uradnik;
	UnoUradnik Uinfo%ROWTYPE;
	menoUradnika Uradnik.priezvisko_U%TYPE;
	mzdaUradnika Uradnik.mzda%TYPE;
BEGIN
	SELECT mzda into mzdaUradnika FROM Uradnik WHERE uradnikID = ID_U;
	SELECT priezvisko_U into menoUradnika FROM Uradnik WHERE uradnikID = ID_U;
	OPEN Uinfo;
	LOOP
		FETCH Uinfo INTO UnoUradnik;
		EXIT WHEN Uinfo%NOTFOUND;
      IF (UnoUradnik.priezvisko_U = menoUradnika)
        THEN
          dbms_output.put_line('Uradnik s menom ' || menoUradnika || ' bol zadaný do porovnania.');
			ELSIF (UnoUradnik.mzda = mzdaUradnika)
				THEN
					dbms_output.put_line('Uradnik s menom ' || UnoUradnik.priezvisko_U || ' ma rovnaky plat ako zadany uradnik ' || menoUradnika || '.');
			ELSIF (UnoUradnik.mzda < mzdaUradnika)
				THEN
					dbms_output.put_line('Uradnik s menom ' || UnoUradnik.priezvisko_U || ' ma mensi plat ako zadany uradnik ' || menoUradnika || '.');
			ELSIF (UnoUradnik.mzda > mzdaUradnika)
				THEN
					dbms_output.put_line('Uradnik s menom ' || UnoUradnik.priezvisko_U || ' ma vacsi plat ako zadany uradnik ' || menoUradnika || '.');
			END IF;
	END LOOP;
EXCEPTION
	WHEN NO_DATA_FOUND
		THEN
			dbms_output.put_line('Uradnik s danym ID nieje evidovany.');
END;
/
show error
--ALTER SESSION SET nls_date_format='dd.mm.yyyy'

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-------------------------------
-- Priradenie <<PK>> entitam --
-------------------------------

ALTER TABLE Pracovne_pozicie ADD CONSTRAINT PK_PracovnePozicie PRIMARY KEY (ID_PP);
ALTER TABLE Plny_uvazok ADD CONSTRAINT PK_PlnyUvazok PRIMARY KEY (ID_PP);
ALTER TABLE Ciastocny_uvazok ADD CONSTRAINT PK_CiastocnyUvazok PRIMARY KEY (ID_PP);

ALTER TABLE Benefity ADD CONSTRAINT PK_Benefity PRIMARY KEY (ID_B);
ALTER TABLE Uradnik ADD CONSTRAINT PK_Uradnik PRIMARY KEY (ID_U);
ALTER TABLE Firma ADD CONSTRAINT PK_Firma PRIMARY KEY (ID_F);
ALTER TABLE Odbor ADD CONSTRAINT PK_Odbor PRIMARY KEY (ID_O);
ALTER TABLE Klient ADD CONSTRAINT PK_Klient PRIMARY KEY (ID_K);
ALTER TABLE Specializacia ADD CONSTRAINT PK_Specializacia PRIMARY KEY (ID_SP);
ALTER TABLE Certifikaty ADD CONSTRAINT PK_Certifikaty PRIMARY KEY (ID_C);
ALTER TABLE Skolenie ADD CONSTRAINT PK_Skolenie PRIMARY KEY (ID_SK);
ALTER TABLE Preukaz ADD CONSTRAINT PK_Preukaz PRIMARY KEY (ID_PR);

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-------------------------------
-- Prepojenia medzi entitami --
-------------------------------

ALTER TABLE Pozicia_Benefity ADD CONSTRAINT Benefity_FK FOREIGN KEY(ID_B) REFERENCES Benefity;
ALTER TABLE Pozicia_Benefity ADD CONSTRAINT PoziciaB_FK FOREIGN KEY(ID_PP) REFERENCES Pracovne_pozicie;

ALTER TABLE Pozicia_Specializacia ADD CONSTRAINT Spec_FK FOREIGN KEY(ID_SP) REFERENCES Specializacia;
ALTER TABLE Pozicia_Specializacia ADD CONSTRAINT PoziciaS_FK FOREIGN KEY(ID_PP) REFERENCES Pracovne_pozicie;

ALTER TABLE Firma_Odbor ADD CONSTRAINT FirmaO_FK FOREIGN KEY(ID_F) REFERENCES Firma;
ALTER TABLE Firma_Odbor ADD CONSTRAINT Odbor_FK FOREIGN KEY(ID_O) REFERENCES Odbor;

ALTER TABLE Specializacia_Certifikat ADD CONSTRAINT SpecC_FK FOREIGN KEY(ID_SP) REFERENCES Specializacia;
ALTER TABLE Specializacia_Certifikat ADD CONSTRAINT CertS_FK FOREIGN KEY(ID_C) REFERENCES Certifikaty;
ALTER TABLE Specializacia_Skolenie ADD CONSTRAINT SkolS_FK FOREIGN KEY(ID_SK) REFERENCES Skolenie;
ALTER TABLE Specializacia_Skolenie ADD CONSTRAINT SpecS_FK FOREIGN KEY(ID_SP) REFERENCES Specializacia;
ALTER TABLE Specializacia_Preukaz ADD CONSTRAINT PreukazS_FK FOREIGN KEY(ID_PR) REFERENCES Preukaz;
ALTER TABLE Specializacia_Preukaz ADD CONSTRAINT SpecP_FK FOREIGN KEY(ID_SP) REFERENCES Specializacia;

ALTER TABLE Klient_Pozicia ADD CONSTRAINT PoziciaK_FK FOREIGN KEY(ID_PP) REFERENCES Pracovne_pozicie;
ALTER TABLE Klient_Pozicia ADD CONSTRAINT KlientPP_FK FOREIGN KEY(ID_K) REFERENCES Klient;

ALTER TABLE Klient_Odbor ADD CONSTRAINT OdborK_FK FOREIGN KEY(ID_O) REFERENCES Odbor;
ALTER TABLE Klient_Odbor ADD CONSTRAINT KlientO_FK FOREIGN KEY(ID_K) REFERENCES Klient;

ALTER TABLE Klient_Skolenie ADD CONSTRAINT SkolK_FK FOREIGN KEY(ID_SK) REFERENCES Skolenie;
ALTER TABLE Klient_Skolenie ADD CONSTRAINT KlientS_FK FOREIGN KEY(ID_K) REFERENCES Klient;

ALTER TABLE Klient_Certifikat ADD CONSTRAINT CertK_FK FOREIGN KEY(ID_C) REFERENCES Certifikaty;
ALTER TABLE Klient_Certifikat ADD CONSTRAINT KlientC_FK FOREIGN KEY(ID_K) REFERENCES Klient;

ALTER TABLE Klient_Preukaz ADD CONSTRAINT PreukazK_FK FOREIGN KEY(ID_PR) REFERENCES Preukaz;
ALTER TABLE Klient_Preukaz ADD CONSTRAINT KlientP_FK FOREIGN KEY(ID_K) REFERENCES Klient;

ALTER TABLE Plny_uvazok ADD CONSTRAINT Plny_uvazokFK FOREIGN KEY(ID_PP) REFERENCES Pracovne_pozicie;
ALTER TABLE Ciastocny_uvazok ADD CONSTRAINT Ciastocny_uvazokFK FOREIGN KEY(ID_PP) REFERENCES Pracovne_pozicie;

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-------------------------
-- Priradenie do entit --
-------------------------
INSERT INTO Pracovne_pozicie(ID_PP, datumPridania, datumNastupu,naplnPrace,hrubaMzda,miesto,obsadenost,ID_U, ID_F) VALUES('0001',TO_DATE('01-05-2012', 'dd-mm-yyyy'),TO_DATE('06-06-2012','dd-mm-yyyy'),'Kontrola nových liekov','17000','Brno','0','0101','1001');
INSERT INTO Pracovne_pozicie(ID_PP, datumPridania, datumNastupu,naplnPrace,hrubaMzda,miesto,obsadenost,ID_U, ID_F) VALUES('0005',TO_DATE('01-05-2012', 'dd-mm-yyyy'),TO_DATE('08-06-2012','dd-mm-yyyy'),'Vývoj nových liekov','24500','Brno','0','0101','1001');
INSERT INTO Pracovne_pozicie(ID_PP, datumPridania, datumNastupu,naplnPrace,hrubaMzda,miesto,obsadenost,ID_U, ID_F) VALUES('0006',TO_DATE('01-05-2012', 'dd-mm-yyyy'),TO_DATE('12-06-2012','dd-mm-yyyy'),'Ochrankár','21600','Brno','0','0101','1001');
INSERT INTO Pracovne_pozicie(ID_PP, datumPridania, datumNastupu,naplnPrace,hrubaMzda,miesto,obsadenost,ID_U, ID_F) VALUES('0002',TO_DATE('07-08-2013', 'dd-mm-yyyy'),TO_DATE('14-09-2013','dd-mm-yyyy'),'Vyuèba','18500','Hradec Kralove','0','0101','1003');
INSERT INTO Pracovne_pozicie(ID_PP, datumPridania, datumNastupu,naplnPrace,hrubaMzda,miesto,obsadenost,ID_U, ID_F) VALUES('0003',TO_DATE('09-04-2011', 'dd-mm-yyyy'),TO_DATE('12-05-2011','dd-mm-yyyy'),'Vývoj SW produktov','36000','Praha','0','0103','1002');
INSERT INTO Pracovne_pozicie(ID_PP, datumPridania, datumNastupu,naplnPrace,hrubaMzda,miesto,obsadenost,ID_U, ID_F) VALUES('0007',TO_DATE('09-04-2011', 'dd-mm-yyyy'),TO_DATE('12-05-2011','dd-mm-yyyy'),'Testovanie SW produktov','25700','Praha','0','0103','1002');
INSERT INTO Pracovne_pozicie(ID_PP, datumPridania, datumNastupu,naplnPrace,hrubaMzda,miesto,obsadenost,ID_U, ID_F) VALUES('0004',TO_DATE('25-01-2012', 'dd-mm-yyyy'),TO_DATE('13-03-2012','dd-mm-yyyy'),'Výskum','19500','Ostrava','0','0104','1003');


INSERT INTO Benefity(ID_B,popisBenefitu) VALUES('0011','Firemne auto');
INSERT INTO Benefity(ID_B,popisBenefitu) VALUES('0012','Firemny mobil');
INSERT INTO Benefity(ID_B,popisBenefitu) VALUES('0013','Firemny laptop');
INSERT INTO Benefity(ID_B,popisBenefitu) VALUES('0014','stravenky');

INSERT INTO Uradnik(ID_U,	krstneMeno_U,	priezvisko_U ,denNastupu ,datumNarodenia_U ,mzda) VALUES('0101','Karolína','Novotná',TO_DATE('04-05-2015','dd-mm-yyyy'),TO_DATE('02-02-1985','dd-mm-yyyy'),'26000');
INSERT INTO Uradnik(ID_U,	krstneMeno_U,	priezvisko_U ,denNastupu ,datumNarodenia_U ,mzda) VALUES('0102','Juraj','Kovár',TO_DATE('01-12-2015','dd-mm-yyyy'),TO_DATE('14-07-1989','dd-mm-yyyy'),'26000');
INSERT INTO Uradnik(ID_U,	krstneMeno_U,	priezvisko_U ,denNastupu ,datumNarodenia_U ,mzda) VALUES('0103','Kamil','Kuro',TO_DATE('04-07-2015','dd-mm-yyyy'),TO_DATE('25-08-1991','dd-mm-yyyy'),'27050');
INSERT INTO Uradnik(ID_U,	krstneMeno_U,	priezvisko_U ,denNastupu ,datumNarodenia_U ,mzda) VALUES('0104','Otto','Bauer',TO_DATE('28-09-2015','dd-mm-yyyy'),TO_DATE('16-03-1972','dd-mm-yyyy'),'27050');

INSERT INTO Firma(ID_F,nazovFirmy,zodpovednaOsoba,adresa,sidloFirmy,pocetZamestnancov) VALUES('1001','Juwex','Karol Horný','Hybešová 28, Brno','Hybešová 28, Brno','45');
INSERT INTO Firma(ID_F,nazovFirmy,zodpovednaOsoba,adresa,sidloFirmy,pocetZamestnancov) VALUES('1002','DEXI','Judita Zapotocká','Husová 220, Brno','Bahnweg 127,Vorchdorf, Rakúsko','128');
INSERT INTO Firma(ID_F,nazovFirmy,zodpovednaOsoba,adresa,sidloFirmy,pocetZamestnancov) VALUES('1003','Solerio','Eva Pivovarniková','Wanklová 45, Brno','98 AVE NW,Edmonton,Canada','457');

INSERT INTO Odbor(ID_O ,nazovOdboru) VALUES('0021','IT - HW');
INSERT INTO Odbor(ID_O ,nazovOdboru) VALUES('0022','IT - SW');
INSERT INTO Odbor(ID_O ,nazovOdboru) VALUES('0023','Úèetnictví');
INSERT INTO Odbor(ID_O ,nazovOdboru) VALUES('0024','Zdravotnictví');

INSERT INTO Klient(krstneMeno_K ,priezvisko_K ,titul,datumNarodenia_K,email,bankovyUcet) VALUES('Štefan','Adamec','Ing.',TO_DATE('04-07-1989','dd-mm-yyyy'),'adamec.stefan@gmail.com',				'CZ6508000000192001145399'); -- 001
INSERT INTO Klient(krstneMeno_K ,priezvisko_K ,titul,datumNarodenia_K,email,bankovyUcet) VALUES('Róbert','Hoøák','Bc.',TO_DATE('01-01-1995','dd-mm-yyyy'),'robert1995@seznam.cz',     				'SK3112000000198742537541'); -- 002 
INSERT INTO Klient(krstneMeno_K ,priezvisko_K ,titul,datumNarodenia_K,email,bankovyUcet) VALUES('Igor','Novák','Ing.',TO_DATE('04-07-1972','dd-mm-yyyy'),'igornovak@azet.sk',         				'CZ6508000000192345782355'); -- 003
INSERT INTO Klient(krstneMeno_K ,priezvisko_K ,titul,datumNarodenia_K,email,bankovyUcet) VALUES('Zuzana','Urbancová','Mgr.',TO_DATE('04-09-1994','dd-mm-yyyy'),'urbancova.zuzana@gmail.com',		'CZ6907000000232245634271'); -- 004
INSERT INTO Klient(krstneMeno_K ,priezvisko_K ,titul,datumNarodenia_K,email,bankovyUcet) VALUES('Tibor','Artem','MUDr.',TO_DATE('17-08-1962','dd-mm-yyyy'),'artem81762@gmail.com',					'SK3112000000457865227922'); -- 005
INSERT INTO Klient(krstneMeno_K ,priezvisko_K ,titul,datumNarodenia_K,email,bankovyUcet) VALUES('Danica','van Braak','MVDr.',TO_DATE('08-03-1990','dd-mm-yyyy'),'vanBraak.Dani@yahoo.com',			'SK3112000000147855692345'); -- 006

INSERT INTO Specializacia(ID_SP, pozadovanyTitul, nazov) VALUES('2001','Ing.', 'SW vývojár');
INSERT INTO Specializacia(ID_SP, pozadovanyTitul, nazov) VALUES('2002','Bc.','Databázový administrátor');
INSERT INTO Specializacia(ID_SP, nazov) VALUES('2003','Spojár');
INSERT INTO Specializacia(ID_SP, pozadovanyTitul, nazov) VALUES('2004','Mgr.','Farmaceut');
INSERT INTO Specializacia(ID_SP, pozadovanyTitul, nazov) VALUES('2005','MDDr.','Zubár');
INSERT INTO Specializacia(ID_SP, pozadovanyTitul, nazov) VALUES('2006','Mgr.','Uèite¾');
INSERT INTO Specializacia(ID_SP, pozadovanyTitul, nazov) VALUES('2007','Ing.','Laboratórny fyzik');
INSERT INTO Specializacia(ID_SP, nazov) VALUES('2008', 'Zváraè');

INSERT INTO Certifikaty(ID_C ,nazovCertifikatu, informaciaOCertifikatu) VALUES('0031','CAE1','Uroveò C1 z Anglického jazyka');
INSERT INTO Certifikaty(ID_C ,nazovCertifikatu, informaciaOCertifikatu) VALUES('0032','CAE2','Uroveò C2 z Anglického jazyka');
INSERT INTO Certifikaty(ID_C ,nazovCertifikatu, informaciaOCertifikatu) VALUES('0033','CISSP','Certified Information Systems Security Professional');
INSERT INTO Certifikaty(ID_C ,nazovCertifikatu, informaciaOCertifikatu) VALUES('0034','CCNA','Cisco Certified Network Associate');
INSERT INTO Certifikaty(ID_C ,nazovCertifikatu, informaciaOCertifikatu) VALUES('0035','MCSE','Microsoft Certified Systems Engineer');

INSERT INTO Skolenie(ID_SK, nazovSkolenia, informaciaOSkoleni) VALUES('0301','ADR','Bezpeèná preprava nebezpeèných vecí');
INSERT INTO Skolenie(ID_SK, nazovSkolenia, informaciaOSkoleni) VALUES('0302','DHZ','Hasièské a záchranné jednotky');
INSERT INTO Skolenie(ID_SK, nazovSkolenia, informaciaOSkoleni) VALUES('0303','SW-SC','Školenie v SolidWorks a SolidCAM');
INSERT INTO Skolenie(ID_SK, nazovSkolenia, informaciaOSkoleni) VALUES('0304','VZV','Kurz obsluhy vysokozdvižných vozíkov');

INSERT INTO Preukaz(ID_PR, nazovPreukazu, informaciaOPreukaze) VALUES('3001','VP-B','Vodièský preukaz typu B');
INSERT INTO Preukaz(ID_PR, nazovPreukazu, informaciaOPreukaze) VALUES('3002','VP-C','Vodièský preukaz typu C');
INSERT INTO Preukaz(ID_PR, nazovPreukazu, informaciaOPreukaze) VALUES('3003','VP-D','Vodièský preukaz typu D');
INSERT INTO Preukaz(ID_PR, nazovPreukazu, informaciaOPreukaze) VALUES('3004','ZP','Zbrojný preukaz');
INSERT INTO Preukaz(ID_PR, nazovPreukazu, informaciaOPreukaze) VALUES('3005','ZP','Preukaz azko-zdravotne postihnutý');

-------------------------------------------------------------------------------------------------------------------------------------
INSERT INTO Pozicia_Benefity(ID_PP, ID_B) VALUES('0001','0011');
INSERT INTO Pozicia_Benefity(ID_PP, ID_B) VALUES('0002','0012');
INSERT INTO Pozicia_Benefity(ID_PP, ID_B) VALUES('0003','0012');
INSERT INTO Pozicia_Benefity(ID_PP, ID_B) VALUES('0003','0013');

INSERT INTO Pozicia_Specializacia(ID_PP,ID_SP) VALUES('0001','2004');
INSERT INTO Pozicia_Specializacia(ID_PP,ID_SP) VALUES('0002','2006');
INSERT INTO Pozicia_Specializacia(ID_PP,ID_SP) VALUES('0003','2001');
INSERT INTO Pozicia_Specializacia(ID_PP,ID_SP) VALUES('0003','2007');

INSERT INTO Firma_Odbor(ID_F,ID_O) VALUES('1001','0024');
INSERT INTO Firma_Odbor(ID_F,ID_O) VALUES('1002','0022');
INSERT INTO Firma_Odbor(ID_F,ID_O) VALUES('1003','0021');

INSERT INTO Specializacia_Certifikat(ID_SP,ID_C) VALUES('2001','0035');
INSERT INTO Specializacia_Certifikat(ID_SP,ID_C) VALUES('2001','0032');
INSERT INTO Specializacia_Certifikat(ID_SP,ID_C) VALUES('2006','0031');

INSERT INTO Specializacia_Skolenie(ID_SP,ID_SK) VALUES('2008','0301');
INSERT INTO Specializacia_Skolenie(ID_SP,ID_SK) VALUES('2008','0304');
INSERT INTO Specializacia_Skolenie(ID_SP,ID_SK) VALUES('2001','0303');

INSERT INTO Specializacia_Preukaz(ID_SP,ID_PR) VALUES('2003','3004');
INSERT INTO Specializacia_Preukaz(ID_SP,ID_PR) VALUES('2003','3001');
INSERT INTO Specializacia_Preukaz(ID_SP,ID_PR) VALUES('2001','3001');
--
INSERT INTO Klient_Pozicia(ID_K,ID_PP) VALUES('004','0001');
INSERT INTO Klient_Pozicia(ID_K,ID_PP) VALUES('002','0003');
--
INSERT INTO Klient_Odbor(ID_K,ID_O) VALUES('004','0023');
INSERT INTO Klient_Odbor(ID_K,ID_O) VALUES('002','0021');
--
INSERT INTO Klient_Skolenie(ID_K,ID_SK,DatumPreskolenia) VALUES('004','0301','1/6/2012');
INSERT INTO Klient_Skolenie(ID_K,ID_SK,DatumPreskolenia) VALUES('002','0303','5/4/2012');
--
INSERT INTO Klient_Certifikat(ID_K,ID_C) VALUES('004','0032');
INSERT INTO Klient_Certifikat(ID_K,ID_C) VALUES('002','0035');
INSERT INTO Klient_Certifikat(ID_K,ID_C) VALUES('002','0032');
--
INSERT INTO Klient_Preukaz(ID_K,ID_PR,DatumPlatnosti) VALUES('004','3001','7/1/2012');
INSERT INTO Klient_Preukaz(ID_K,ID_PR,DatumPlatnosti) VALUES('002','3001','9/2/2014');

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

--------------
--- SELECT ---
--------------

-- 2x Spojenie 2 tabuliek
-------------------------

-- ** Aka je napln prace, hruba mzda a miesto, o ktoru je zodpovedny clovek s menom Otto
SELECT P.naplnPrace, P.hrubaMzda, P.miesto
FROM Pracovne_pozicie P, Uradnik U
WHERE P.ID_U = U.ID_U and U.krstneMeno_U='Otto';

-- ** nazov, adresa a sidlo firmy, ktora ponuka pracovnu poziciu s mzdou vyssou ako 20000
SELECT F.nazovFirmy, F.adresa, F.sidloFirmy
FROM Pracovne_pozicie P, Firma F
WHERE P.ID_F = F.ID_F and P.hrubaMzda > 20000;
-- 1x spojenie 3 tabuliek
-------------------------
-- ** vypis certifikaty, ktore dana specializacia vyzaduje
SELECT S.nazov, C.nazovCertifikatu
FROM Specializacia S, Certifikaty C, Specializacia_Certifikat SC
WHERE S.ID_SP = SC.ID_SP and C.ID_C = SC.ID_C;
-- 2x Group By
--------------
-- ** pocet klientov, ktory maju dany titul
SELECT K.titul, count(K.titul)
FROM Klient K
GROUP BY K.titul;

-- ** vypis max. mzdu od uradnikov a pocet, kolki ju maju
SELECT COUNT(MAX(u.mzda)) AS Pocet_uradnikov, MAX(u.mzda) AS Najvyssi_plat
FROM Uradnik u
GROUP BY u.mzda;
-- 1x EXISTS // NOT EXISTS
------------
-- ** vypis vsetky specializacie, kde neexistuje poziadavka certifikatu
SELECT SP.nazov
FROM Specializacia SP
WHERE NOT EXISTS
(
	SELECT SC.ID_SP
	FROM Specializacia_Certifikat SC
	WHERE SP.ID_SP = SC.ID_SP
	
);
-- 1x Vnoreny IN
----------------
-- ** vypis vsetky info o pracovnej pozicii, ktorej Firma ma medzi 100 - 200 zamestnancov
SELECT *
FROM Pracovne_pozicie Pp
WHERE Pp.ID_F
IN
(
	SELECT F.ID_F
	FROM Firma F
	WHERE F.pocetZamestnancov BETWEEN '100' AND '200'
);

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-------------------------
--	Zavolanie procedur --
-------------------------
exec pozad_Titul(2005);
exec uradnici(0102);

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

--DROP INDEX ExIndex;

---------------------------
--	EXPLAIN PLAN & INDEX --
---------------------------
EXPLAIN PLAN FOR
SELECT priezvisko_U, COUNT(ID_PP)
FROM Uradnik NATURAL JOIN Pracovne_pozicie
GROUP By priezvisko_U, ID_PP;
SELECT * FROM TABLE(DBMS_XPLAN.display);

CREATE INDEX ExIndex ON Uradnik(priezvisko_U);

EXPLAIN PLAN FOR
SELECT /*+ INDEX(Uradnik ExIndex)*/ priezvisko_U, COUNT(ID_PP)
FROM Uradnik NATURAL JOIN Pracovne_pozicie
GROUP By priezvisko_U, ID_PP;
SELECT * FROM TABLE(DBMS_XPLAN.display);

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-----------------------------------------------
--	Udelenie práv pre 2. èlena týmu xkeryj00 --
-----------------------------------------------
GRANT ALL ON Pracovne_pozicie TO xkeryj00;
GRANT ALL ON Plny_uvazok TO xkeryj00;
GRANT ALL ON Ciastocny_uvazok TO xkeryj00;
GRANT ALL ON Benefity TO xkeryj00;
GRANT ALL ON Uradnik TO xkeryj00;
GRANT ALL ON Firma TO xkeryj00;
GRANT ALL ON Odbor TO xkeryj00;
GRANT ALL ON Klient TO xkeryj00;
GRANT ALL ON Specializacia TO xkeryj00;
GRANT ALL ON Certifikaty TO xkeryj00;
GRANT ALL ON Skolenie TO xkeryj00;
GRANT ALL ON Preukaz TO xkeryj00;

GRANT EXECUTE ON pozad_Titul TO xkeryj00;
GRANT EXECUTE ON uradnici TO xkeryj00;

COMMIT;

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-----------------------------
--	Materializovany pohlad --
-----------------------------
DROP MATERIALIZED VIEW newKlient;

CREATE MATERIALIZED VIEW LOG ON Klient WITH PRIMARY KEY,ROWID(titul) INCLUDING NEW VALUES;

CREATE MATERIALIZED VIEW newKlient
CACHE
BUILD IMMEDIATE -- immediately after creation
REFRESH FAST ON COMMIT -- optimization of reading from view
ENABLE QUERY REWRITE -- used by optimalizator
AS SELECT Kl.titul, count(Kl.titul) as PocetTitulov
FROM Klient Kl
GROUP BY Kl.titul;

GRANT ALL PRIVILEGES ON newKlient TO xondre08;

SELECT * from xondre08.newKlient;
INSERT INTO Klient(krstneMeno_K ,priezvisko_K ,titul,datumNarodenia_K,email,bankovyUcet) VALUES('Vladimír','Oravec','Ing.',TO_DATE('17-11-1991','dd-mm-yyyy'),'vladoOravec@gmail.com',				'SK3204000000112358447812');
INSERT INTO Klient(krstneMeno_K ,priezvisko_K ,titul,datumNarodenia_K,email,bankovyUcet) VALUES('Peter','Hlinár','Bc.',TO_DATE('14-08-1992','dd-mm-yyyy'),'p.hlinar@azet.sk',				'SK3204000000558947123295');
COMMIT;
SELECT * from newKlient;