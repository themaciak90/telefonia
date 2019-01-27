#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <mysql.h>
#include <regex>
#include <vector>
#include "historia_t.h"

class uzytkownik_t
{
private:
	friend class telefonia_t;
	MYSQL mysql;
	std::string imie;
	std::string nazwisko;
	int wiek;
	std::string uprawnienia;
	std::string plec;
	std::string login;
	std::string haslo;
	uzytkownik_t();
	//funkcje admina
	void dodaj_konto();
	void usun_konto();
	void sredni_wiek();
	void rozklad_plci();


	//funkcje usera
	void wyswietl_swoje_numery(std::string);
	void zadzwon(std::string);

		
};

