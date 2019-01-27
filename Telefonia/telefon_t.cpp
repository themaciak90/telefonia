#include "telefon_t.h"



telefon_t::telefon_t()
{
}


void telefon_t::dodaj_numer()
{
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "127.0.0.1", "root", "", "telefonia", 0, NULL, 0);
	MYSQL_ROW row;
	MYSQL_RES* res;
	std::string zapytanie;
	const char* q;
	std::stringstream ss;
	std::cout << "Podaj numer telefonu: ";
	std::cin >> numer_telefonu;
	std::regex reg("^[0-9]{9}$");
	while (!std::regex_match(numer_telefonu, reg))
	{
		std::cout << "Nie prawidlowy numer telefonu. Podaj ponownie: ";
		std::cin >> numer_telefonu;
	}
	ss << "INSERT INTO telefon(numer_telefonu) VALUES ('" << numer_telefonu << "')";
	zapytanie = ss.str();
	q = zapytanie.c_str();
	int qstate, id_number, id_user;
	qstate = mysql_query(&mysql, q);
	if (qstate == 0) std::cout << "Dodano!" << std::endl;
	else std::cout << "Blad " << mysql_error(&mysql);
	ss.str("");
	ss.clear();
	ss << "SELECT id_numeru from telefon where numer_telefonu = '" << numer_telefonu << "'";
	zapytanie = ss.str();
	q = zapytanie.c_str();
	qstate = mysql_query(&mysql, q);
	res = mysql_store_result(&mysql);
	if (qstate == 0)
	{
		while (row = mysql_fetch_row(res))
		{
			id_number = atoi(row[0]);
		}
	}
	else
		std::cout << "Blad " << mysql_error(&mysql);
	std::cout << "Do ktorego klienta przypisac numer? Podaj ID:";
	std::cin >> id_user;
	ss.str("");
	ss.clear();
	ss << "INSERT INTO umowa(id_klienta, id_numeru) VALUES (" << id_user << "," << id_number << ")";
	zapytanie = ss.str();
	q = zapytanie.c_str();
	qstate = mysql_query(&mysql, q);
	if (qstate == 0) std::cout << "Dodano!" << std::endl;
	else std::cout << "Blad " << mysql_error(&mysql);

	mysql_close(&mysql);
}

void telefon_t::usun_numer()
{
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "127.0.0.1", "root", "", "telefonia", 0, NULL, 0);
	MYSQL_ROW row;
	MYSQL_RES* res;
	std::string zapytanie;
	std::stringstream ss;
	const char* q;
	int id, count, qstate;
	std::cout << "Podaj id numeru: ";
	std::cin >> id;
	ss << "SELECT COUNT(*) from telefon where id_numeru = " << id;
	zapytanie = ss.str();
	q = zapytanie.c_str();
	qstate = mysql_query(&mysql, q);
	res = mysql_store_result(&mysql);
	if (qstate == 0)
	{
		while (row = mysql_fetch_row(res))
		{
			count = atoi(row[0]);
		}
	}
	else
		std::cout << "Blad " << mysql_error(&mysql);
	if (count == 0)
	{
		std::cout << "Brak takiego uzytkownika." << std::endl;
		return;
	}
	ss.str("");
	ss.clear();
	ss << "DELETE FROM telefon where id_numeru = " << id;
	zapytanie = ss.str();
	q = zapytanie.c_str();
	mysql_query(&mysql, q);
	ss.str("");
	ss.clear();
	ss << "DELETE FROM umowa where id_numeru = " << id;
	zapytanie = ss.str();
	q = zapytanie.c_str();
	mysql_query(&mysql, q);
	mysql_close(&mysql);
	
}