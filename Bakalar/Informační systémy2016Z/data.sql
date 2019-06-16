SET NAMES utf8;
SET foreign_key_checks = 0;
SET time_zone = 'SYSTEM';
SET sql_mode = 'NO_AUTO_VALUE_ON_ZERO';

DROP TABLE IF EXISTS Pracovne_pozicie;
DROP TABLE IF EXISTS Benefity;
DROP TABLE IF EXISTS Uradnik;
DROP TABLE IF EXISTS Firma;
/*DROP TABLE IF EXISTS Odbor;*/
DROP TABLE IF EXISTS Klient;
DROP TABLE IF EXISTS Specializacia;
DROP TABLE IF EXISTS Certifikaty;
DROP TABLE IF EXISTS Skolenie;
DROP TABLE IF EXISTS Preukaz;

DROP TABLE IF EXISTS Pozicia_Benefity;
DROP TABLE IF EXISTS Pozicia_Specializacia;
/*DROP TABLE IF EXISTS Firma_Odbor;*/
DROP TABLE IF EXISTS Specializacia_Certifikat;
DROP TABLE IF EXISTS Specializacia_Skolenie;
DROP TABLE IF EXISTS Specializacia_Preukaz;
DROP TABLE IF EXISTS Klient_Pozicia;
/*DROP TABLE IF EXISTS Klient_Odbor;*/
DROP TABLE IF EXISTS Klient_Skolenie;
DROP TABLE IF EXISTS Klient_Certifikat;
DROP TABLE IF EXISTS Klient_Preukaz;

CREATE TABLE Pracovne_pozicie(
ID_PP BIGINT NOT NULL AUTO_INCREMENT,
datumPridania DATE,
datumNastupu DATE,
naplnPrace VARCHAR(255) NOT NULL,
hrubaMzda BIGINT NOT NULL,
miesto VARCHAR(100) NOT NULL,
obsadenost BIGINT NOT NULL,
ID_U BIGINT NOT NULL,
ID_F BIGINT NOT NULL,
PRIMARY KEY(ID_PP)
) ;

CREATE TABLE Benefity(
ID_B BIGINT NOT NULL AUTO_INCREMENT,
PRIMARY KEY(ID_B),
popisBenefitu VARCHAR(40) NOT NULL
);

CREATE TABLE Uradnik(
ID_U BIGINT NOT NULL AUTO_INCREMENT,
PRIMARY KEY(ID_U),
krstneMeno_U VARCHAR(50) NOT NULL,
priezvisko_U VARCHAR(50) NOT NULL,
denNastupu DATE NOT NULL,
datumNarodenia_U DATE NOT NULL,
titul VARCHAR(10) NOT NULL,
email VARCHAR(30) NOT NULL,
mzda BIGINT NOT NULL,
typ BIGINT NOT NULL,
loginU VARCHAR(255) DEFAULT NULL,
hesloU VARCHAR(255) DEFAULT NULL
);

CREATE TABLE Klient(
ID_K BIGINT NOT NULL AUTO_INCREMENT,
PRIMARY KEY(ID_K),
krstneMeno_K VARCHAR(20) NOT NULL,
priezvisko_K VARCHAR(20) NOT NULL,
titul VARCHAR(10) NOT NULL,
datumNarodenia_K DATE NOT NULL,
email VARCHAR(30) NOT NULL,
loginK VARCHAR(255) DEFAULT NULL,
hesloK VARCHAR(255) DEFAULT NULL, 
) ;

CREATE TABLE Firma(
ID_F BIGINT NOT NULL AUTO_INCREMENT,
PRIMARY KEY(ID_F),
nazovFirmy VARCHAR(50) NOT NULL,
zodpovednaOsoba VARCHAR(50) NOT NULL,
adresa VARCHAR(100) NOT NULL,
sidloFirmy VARCHAR(100) NOT NULL,
pocetZamestnancov BIGINT NOT NULL
) ;
/*

IBAN VARCHAR(25) NOT NULL
CREATE TABLE Odbor(
ID_O BIGINT NOT NULL AUTO_INCREMENT,
PRIMARY KEY(ID_O),
nazovOdboru VARCHAR(50) NOT NULL
) ;
*/
CREATE TABLE Specializacia(
ID_SP BIGINT NOT NULL AUTO_INCREMENT,
PRIMARY KEY(ID_SP),
pozadovanyTitul VARCHAR(10),
nazov VARCHAR(50) NOT NULL
) ;

CREATE TABLE Certifikaty(
ID_C BIGINT NOT NULL AUTO_INCREMENT,
PRIMARY KEY(ID_C),
nazovCertifikatu VARCHAR(15) NOT NULL,
informaciaOCertifikatu VARCHAR(100) NOT NULL
) ;
CREATE TABLE Skolenie(
ID_SK BIGINT NOT NULL AUTO_INCREMENT,
PRIMARY KEY(ID_SK),
nazovSkolenia VARCHAR(15) NOT NULL,
informaciaOSkoleni VARCHAR(100) NOT NULL
) ;
CREATE TABLE Preukaz(
ID_PR BIGINT NOT NULL AUTO_INCREMENT,
PRIMARY KEY(ID_PR),
nazovPreukazu VARCHAR(15) NOT NULL,
informaciaOPreukaze VARCHAR(100) NOT NULL
) ;


CREATE TABLE Pozicia_Benefity(
ID_PP BIGINT NOT NULL,
ID_B BIGINT NOT NULL,
PRIMARY KEY(ID_PP, ID_B)
) ;

CREATE TABLE Pozicia_Specializacia(
ID_PP BIGINT NOT NULL,
ID_SP BIGINT NOT NULL,
PRIMARY KEY(ID_PP, ID_SP)
) ;
/*
CREATE TABLE Firma_Odbor(
ID_F BIGINT NOT NULL,
ID_O BIGINT NOT NULL,
PRIMARY KEY(ID_F, ID_O)
) ;
*/
CREATE TABLE Specializacia_Certifikat(
ID_SP BIGINT NOT NULL,
ID_C BIGINT NOT NULL,
PRIMARY KEY(ID_SP,ID_C)
) ;

CREATE TABLE Specializacia_Skolenie(
ID_SP BIGINT NOT NULL,
ID_SK BIGINT NOT NULL,
PRIMARY KEY(ID_SP, ID_SK)
) ;

CREATE TABLE Specializacia_Preukaz(
ID_SP BIGINT NOT NULL,
ID_PR BIGINT NOT NULL,
PRIMARY KEY(ID_SP, ID_PR)
) ;

CREATE TABLE Klient_Pozicia(
ID_K BIGINT NOT NULL,
ID_PP BIGINT NOT NULL,
PRIMARY KEY(ID_K, ID_PP)
) ;
/*
CREATE TABLE Klient_Odbor(
ID_K BIGINT NOT NULL,
ID_O BIGINT NOT NULL,
PRIMARY KEY(ID_K, ID_O)
) ;
*/
CREATE TABLE Klient_Skolenie(
ID_K BIGINT NOT NULL,
ID_SK BIGINT NOT NULL,
DatumPreskolenia DATE NOT NULL,
PRIMARY KEY(ID_K, ID_SK)
) ;

CREATE TABLE Klient_Certifikat(
ID_K BIGINT NOT NULL,
ID_C BIGINT NOT NULL,
PRIMARY KEY(ID_K, ID_C)
) ;

CREATE TABLE Klient_Preukaz(
ID_K BIGINT NOT NULL,
ID_PR BIGINT NOT NULL,
DatumPlatnosti DATE NOT NULL,
PRIMARY KEY(ID_K, ID_PR)
) ;


ALTER TABLE Pozicia_Benefity ADD FOREIGN KEY(ID_B) REFERENCES Benefity(ID_B);
ALTER TABLE Pozicia_Benefity ADD FOREIGN KEY(ID_PP) REFERENCES Pracovne_pozicie(ID_PP);

ALTER TABLE Pozicia_Specializacia ADD FOREIGN KEY(ID_SP) REFERENCES Specializacia(ID_SP);
ALTER TABLE Pozicia_Specializacia ADD FOREIGN KEY(ID_PP) REFERENCES Pracovne_pozicie(ID_PP);
/*
ALTER TABLE Firma_Odbor ADD FOREIGN KEY(ID_F) REFERENCES Firma(ID_F);
ALTER TABLE Firma_Odbor ADD FOREIGN KEY(ID_O) REFERENCES Odbor(ID_O);
*/
ALTER TABLE Specializacia_Certifikat ADD FOREIGN KEY(ID_SP) REFERENCES Specializacia(ID_SP);
ALTER TABLE Specializacia_Certifikat ADD FOREIGN KEY(ID_C) REFERENCES Certifikaty(ID_C);
ALTER TABLE Specializacia_Skolenie ADD FOREIGN KEY(ID_SK) REFERENCES Skolenie(ID_SK);
ALTER TABLE Specializacia_Skolenie ADD FOREIGN KEY(ID_SP) REFERENCES Specializacia(ID_SP);
ALTER TABLE Specializacia_Preukaz ADD FOREIGN KEY(ID_PR) REFERENCES Preukaz(ID_PR);
ALTER TABLE Specializacia_Preukaz ADD FOREIGN KEY(ID_SP) REFERENCES Specializacia(ID_SP);

ALTER TABLE Klient_Pozicia ADD FOREIGN KEY(ID_PP) REFERENCES Pracovne_pozicie(ID_PP);
ALTER TABLE Klient_Pozicia ADD FOREIGN KEY(ID_K) REFERENCES Klient(ID_K);
/*
ALTER TABLE Klient_Odbor ADD FOREIGN KEY(ID_O) REFERENCES Odbor(ID_O);
ALTER TABLE Klient_Odbor ADD FOREIGN KEY(ID_K) REFERENCES Klient(ID_K);
*/
ALTER TABLE Klient_Skolenie ADD FOREIGN KEY(ID_SK) REFERENCES Skolenie(ID_SK);
ALTER TABLE Klient_Skolenie ADD FOREIGN KEY(ID_K) REFERENCES Klient(ID_K);

ALTER TABLE Klient_Certifikat ADD FOREIGN KEY(ID_C) REFERENCES Certifikaty(ID_C);
ALTER TABLE Klient_Certifikat ADD FOREIGN KEY(ID_K) REFERENCES Klient(ID_K);

ALTER TABLE Klient_Preukaz ADD FOREIGN KEY(ID_PR) REFERENCES Preukaz(ID_PR);
ALTER TABLE Klient_Preukaz ADD FOREIGN KEY(ID_K) REFERENCES Klient(ID_K);

INSERT INTO Pracovne_pozicie(ID_PP, datumPridania, datumNastupu,naplnPrace,hrubaMzda,miesto,obsadenost,ID_U, ID_F) VALUES('1','2012-05-01','2012-06-06','Kontrola novych liekov','17000','Brno','1','1','1');
INSERT INTO Pracovne_pozicie(ID_PP, datumPridania, datumNastupu,naplnPrace,hrubaMzda,miesto,obsadenost,ID_U, ID_F) VALUES('2','2013-08-07','2013-09-14','Vyucba','18500','Hradec Kralove','0','1','3');
INSERT INTO Pracovne_pozicie(ID_PP, datumPridania, datumNastupu,naplnPrace,hrubaMzda,miesto,obsadenost,ID_U, ID_F) VALUES('3','2011-04-09','2011-05-12','Vyvoj SW produktov','36000','Praha','1','3','2');
INSERT INTO Pracovne_pozicie(ID_PP, datumPridania, datumNastupu,naplnPrace,hrubaMzda,miesto,obsadenost,ID_U, ID_F) VALUES('4','2012-01-25','2012-03-13','Vyskum','19500','Ostrava','0','4','3');
INSERT INTO Pracovne_pozicie(ID_PP, datumPridania, datumNastupu,naplnPrace,hrubaMzda,miesto,obsadenost,ID_U, ID_F) VALUES('5','2012-05-01','2012-06-08','Vyvoj novych liekov','24500','Brno','0','1','1');
INSERT INTO Pracovne_pozicie(ID_PP, datumPridania, datumNastupu,naplnPrace,hrubaMzda,miesto,obsadenost,ID_U, ID_F) VALUES('6','2012-05-01','2012-06-12','Ochrankar','21600','Brno','0','1','1');
INSERT INTO Pracovne_pozicie(ID_PP, datumPridania, datumNastupu,naplnPrace,hrubaMzda,miesto,obsadenost,ID_U, ID_F) VALUES('7','2011-04-09','2011-05-12','Testovanie SW produktov','25700','Praha','0','3','2');


INSERT INTO Benefity(ID_B,popisBenefitu) VALUES('1','Firemne auto');
INSERT INTO Benefity(ID_B,popisBenefitu) VALUES('2','Firemny mobil');
INSERT INTO Benefity(ID_B,popisBenefitu) VALUES('3','Firemny laptop');
INSERT INTO Benefity(ID_B,popisBenefitu) VALUES('4','Stravenky');

INSERT INTO Uradnik(ID_U,	krstneMeno_U,	priezvisko_U ,denNastupu ,datumNarodenia_U ,email, titul, mzda,typ, loginU, hesloU) VALUES('1','Karolina','Novotna','2015-05-04','1985-02-02','karolina.novotna@gmail.com','Mgr.','26000',1, 'karol.novotna','123456');
INSERT INTO Uradnik(ID_U,	krstneMeno_U,	priezvisko_U ,denNastupu ,datumNarodenia_U ,email, titul, mzda,typ, loginU, hesloU) VALUES('2','Juraj','Kovar','2015-12-01','1989-07-14','kovar.juraj@gmail.com','','26000',1,'juraj.kovar','123456');
INSERT INTO Uradnik(ID_U,	krstneMeno_U,	priezvisko_U ,denNastupu ,datumNarodenia_U ,email, titul, mzda,typ, loginU, hesloU) VALUES('3','Kamil','Kuro','2015-07-04','1991-08-25','kamilk@gmail.com','Bc.','27050',2,'admin','123456');
INSERT INTO Uradnik(ID_U,	krstneMeno_U,	priezvisko_U ,denNastupu ,datumNarodenia_U ,email, titul, mzda,typ, loginU, hesloU) VALUES('4','Otto','Bauer','2015-09-28','1972-03-16','otto@gmail.com','Ing.','27050',1,'otto.bauer','123456');

INSERT INTO Firma(ID_F,nazovFirmy,zodpovednaOsoba,adresa,sidloFirmy,pocetZamestnancov) VALUES('1','Juwex','Karol Horny','Hybesova 28, Brno','Hybesova 28, Brno','45');
INSERT INTO Firma(ID_F,nazovFirmy,zodpovednaOsoba,adresa,sidloFirmy,pocetZamestnancov) VALUES('2','DEXI','Judita Zapotocka','Husova 220, Brno','Bahnweg 127,Vorchdorf, Rakusko','128');
INSERT INTO Firma(ID_F,nazovFirmy,zodpovednaOsoba,adresa,sidloFirmy,pocetZamestnancov) VALUES('3','Solerio','Eva Pivovarnikova','Wanklova 45, Brno','98 AVE NW,Edmonton,Canada','457');
/*
INSERT INTO Odbor(ID_O ,nazovOdboru) VALUES('1','IT - HW');
INSERT INTO Odbor(ID_O ,nazovOdboru) VALUES('2','IT - SW');
INSERT INTO Odbor(ID_O ,nazovOdboru) VALUES('3','Ucetnictvi');
INSERT INTO Odbor(ID_O ,nazovOdboru) VALUES('4','Zdravotnictvi');
*/
INSERT INTO Klient(ID_K,krstneMeno_K ,priezvisko_K ,titul,datumNarodenia_K,email, loginK, hesloK) VALUES('1','Stefan','Adamec','Ing.','1989-07-04','adamec.stefan@gmail.com', 'stefan.adamec','123456'); -- 001
INSERT INTO Klient(ID_K,krstneMeno_K ,priezvisko_K ,titul,datumNarodenia_K,email, loginK, hesloK) VALUES('2','Robert','Horak','Bc.','1995-01-01','robert1995@seznam.cz', 'robert.horak','123456'); -- 002 
INSERT INTO Klient(ID_K,krstneMeno_K ,priezvisko_K ,titul,datumNarodenia_K,email, loginK, hesloK) VALUES('3','Igor','Novak','Ing.','1972-07-04','igornovak@azet.sk', 'igor.novak','123456'); -- 003
INSERT INTO Klient(ID_K,krstneMeno_K ,priezvisko_K ,titul,datumNarodenia_K,email, loginK, hesloK) VALUES('4','Zuzana','Urbancova','Mgr.','1994-09-04','urbancova.zuzana@gmail.com',	 'zuzana.urbancova','123456'); -- 004
INSERT INTO Klient(ID_K,krstneMeno_K ,priezvisko_K ,titul,datumNarodenia_K,email, loginK, hesloK) VALUES('5','Tibor','Artem','MUDr.','1962-08-17','artem81762@gmail.com', 'tibor.artem','123456'); -- 005
INSERT INTO Klient(ID_K,krstneMeno_K ,priezvisko_K ,titul,datumNarodenia_K,email, loginK, hesloK) VALUES('6','Danica','van Braak','MVDr.','1990-03-08','vanBraak.Dani@yahoo.com', 'danica.braak','123456'); -- 006

INSERT INTO Specializacia(ID_SP, pozadovanyTitul, nazov) VALUES('1','Ing.', 'SW vyvojar');
INSERT INTO Specializacia(ID_SP, pozadovanyTitul, nazov) VALUES('2','Bc.','Databazovy administrator');
INSERT INTO Specializacia(ID_SP, nazov) VALUES('3','Spojar');
INSERT INTO Specializacia(ID_SP, pozadovanyTitul, nazov) VALUES('4','Mgr.','Farmaceut');
INSERT INTO Specializacia(ID_SP, pozadovanyTitul, nazov) VALUES('5','MDDr.','Zubar');
INSERT INTO Specializacia(ID_SP, pozadovanyTitul, nazov) VALUES('6','Mgr.','Ucitel');
INSERT INTO Specializacia(ID_SP, pozadovanyTitul, nazov) VALUES('7','Ing.','Laboratorny fyzik');
INSERT INTO Specializacia(ID_SP, nazov) VALUES('8', 'Zvarac');

INSERT INTO Certifikaty(ID_C ,nazovCertifikatu, informaciaOCertifikatu) VALUES('1','CAE1','Uroven C1 z Anglickeho jazyka');
INSERT INTO Certifikaty(ID_C ,nazovCertifikatu, informaciaOCertifikatu) VALUES('2','CAE2','Uroven C2 z Anglickeho jazyka');
INSERT INTO Certifikaty(ID_C ,nazovCertifikatu, informaciaOCertifikatu) VALUES('3','CISSP','Certified Information Systems Security Professional');
INSERT INTO Certifikaty(ID_C ,nazovCertifikatu, informaciaOCertifikatu) VALUES('4','CCNA','Cisco Certified Network Associate');
INSERT INTO Certifikaty(ID_C ,nazovCertifikatu, informaciaOCertifikatu) VALUES('5','MCSE','Microsoft Certified Systems Engineer');

INSERT INTO Skolenie(ID_SK, nazovSkolenia, informaciaOSkoleni) VALUES('1','ADR','Bezpecna preprava nebezpecnych veci');
INSERT INTO Skolenie(ID_SK, nazovSkolenia, informaciaOSkoleni) VALUES('2','DHZ','Hasicske a zachranne jednotky');
INSERT INTO Skolenie(ID_SK, nazovSkolenia, informaciaOSkoleni) VALUES('3','SW-SC','Skolenie v SolidWorks a SolidCAM');
INSERT INTO Skolenie(ID_SK, nazovSkolenia, informaciaOSkoleni) VALUES('4','VZV','Kurz obsluhy vysokozdviznych vozikov');

INSERT INTO Preukaz(ID_PR, nazovPreukazu, informaciaOPreukaze) VALUES('1','VP-B','Vodicsky preukaz typu B');
INSERT INTO Preukaz(ID_PR, nazovPreukazu, informaciaOPreukaze) VALUES('2','VP-C','Vodicsky preukaz typu C');
INSERT INTO Preukaz(ID_PR, nazovPreukazu, informaciaOPreukaze) VALUES('3','VP-D','Vodicsky preukaz typu D');
INSERT INTO Preukaz(ID_PR, nazovPreukazu, informaciaOPreukaze) VALUES('4','ZP','Zbrojny preukaz');
INSERT INTO Preukaz(ID_PR, nazovPreukazu, informaciaOPreukaze) VALUES('5','TZP','Preukaz Tazko-zdravotne postihnuty');


INSERT INTO Pozicia_Benefity(ID_PP, ID_B) VALUES('1','1');
INSERT INTO Pozicia_Benefity(ID_PP, ID_B) VALUES('2','2');
INSERT INTO Pozicia_Benefity(ID_PP, ID_B) VALUES('3','2');
INSERT INTO Pozicia_Benefity(ID_PP, ID_B) VALUES('3','3');

INSERT INTO Pozicia_Specializacia(ID_PP,ID_SP) VALUES('1','4');
INSERT INTO Pozicia_Specializacia(ID_PP,ID_SP) VALUES('2','6');
INSERT INTO Pozicia_Specializacia(ID_PP,ID_SP) VALUES('3','1');
INSERT INTO Pozicia_Specializacia(ID_PP,ID_SP) VALUES('4','7');
/*
INSERT INTO Firma_Odbor(ID_F,ID_O) VALUES('1','4');
INSERT INTO Firma_Odbor(ID_F,ID_O) VALUES('2','2');
INSERT INTO Firma_Odbor(ID_F,ID_O) VALUES('3','1');
*/
INSERT INTO Specializacia_Certifikat(ID_SP,ID_C) VALUES('1','5');
INSERT INTO Specializacia_Certifikat(ID_SP,ID_C) VALUES('1','2');
INSERT INTO Specializacia_Certifikat(ID_SP,ID_C) VALUES('6','1');

INSERT INTO Specializacia_Skolenie(ID_SP,ID_SK) VALUES('8','1');
INSERT INTO Specializacia_Skolenie(ID_SP,ID_SK) VALUES('8','4');
INSERT INTO Specializacia_Skolenie(ID_SP,ID_SK) VALUES('1','3');

INSERT INTO Specializacia_Preukaz(ID_SP,ID_PR) VALUES('3','4');
INSERT INTO Specializacia_Preukaz(ID_SP,ID_PR) VALUES('3','1');
INSERT INTO Specializacia_Preukaz(ID_SP,ID_PR) VALUES('1','1');

INSERT INTO Klient_Pozicia(ID_K,ID_PP) VALUES('4','1');
INSERT INTO Klient_Pozicia(ID_K,ID_PP) VALUES('2','3');
/*
INSERT INTO Klient_Odbor(ID_K,ID_O) VALUES('4','3');
INSERT INTO Klient_Odbor(ID_K,ID_O) VALUES('2','1');
*/
INSERT INTO Klient_Skolenie(ID_K,ID_SK,DatumPreskolenia) VALUES('4','1','1/6/2012');
INSERT INTO Klient_Skolenie(ID_K,ID_SK,DatumPreskolenia) VALUES('2','3','5/4/2012');

INSERT INTO Klient_Certifikat(ID_K,ID_C) VALUES('4','2');
INSERT INTO Klient_Certifikat(ID_K,ID_C) VALUES('2','5');
INSERT INTO Klient_Certifikat(ID_K,ID_C) VALUES('2','2');

INSERT INTO Klient_Preukaz(ID_K,ID_PR,DatumPlatnosti) VALUES('4','1','7/1/2012');
INSERT INTO Klient_Preukaz(ID_K,ID_PR,DatumPlatnosti) VALUES('2','1','9/2/2014');
	