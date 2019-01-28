#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <mysql.h>
#include "uzytkownik_t.h"
#include "telefon_t.h"
#include "historia_t.h"


class telefonia_t
{
public:
	MYSQL mysql;
	telefonia_t();
	void logowanie();
	void menu_admina(std::string);
	void menu_uzytkownika(std::string);
	void wyswietl_umowy();
	void wyswietl_klient();
	void wyswietl_po_numerze();
	void wyswietl_klient(int);
	void wyswietl_umowe();
};

