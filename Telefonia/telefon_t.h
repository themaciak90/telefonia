#pragma once
#include <iostream>
#include <string>
#include <mysql.h>
#include <regex>
#include <sstream>


class telefon_t
{
	friend class telefonia_t;
private:
	MYSQL mysql;
	int id_numeru;
	std::string numer_telefonu;
	void dodaj_numer();
	void usun_numer();
	telefon_t();
};

