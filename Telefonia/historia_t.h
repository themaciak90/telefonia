#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <mysql.h>
#include <regex>

class historia_t
{	std::string czas;
	std::string data;

private:
	MYSQL mysql;
	friend class telefonia_t;
	friend class uzytkownik_t;
	std::string numer_dzwoniacego;
	std::string numer_odbierajacego;
	historia_t();
	void wstaw_historie_admin();
	void historia_od_usera(std::string, std::string ,std::string);
	void wyswietl_historie();
	void wyswietl_historie(std::string);


};

