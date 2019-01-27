#include "historia_t.h"



historia_t::historia_t()
{
}


void historia_t::wstaw_historie_admin()
{
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "127.0.0.1", "root", "", "telefonia", 0, NULL, 0);
	MYSQL_ROW row;
	MYSQL_RES* res;
	std::string zapytanie;
	std::stringstream ss;
	const char* q;
	int count1, count2, qstate;
	std::cout << "Podaj numer dzwoniacego: ";
	std::cin >> numer_dzwoniacego;
	std::regex reg("^[0-9]{9}$");
	while (!std::regex_match(numer_dzwoniacego, reg))
	{
		std::cout << "Nie prawidlowy numer telefonu. Podaj ponownie: ";
		std::cin >> numer_dzwoniacego;
	}
	std::cout << "Podaj numer odbierajacego: ";
	std::cin >> numer_odbierajacego;
	while (!std::regex_match(numer_odbierajacego, reg))
	{
		std::cout << "Nie prawidlowy numer telefonu. Podaj ponownie: ";
		std::cin >> numer_odbierajacego;
	}

	ss << "SELECT COUNT(*) from telefon where numer_telefonu = '" << numer_dzwoniacego << "'";
	zapytanie = ss.str();
	q = zapytanie.c_str();
	qstate = mysql_query(&mysql, q);
	res = mysql_store_result(&mysql);
	if (qstate == 0)
	{
		while (row = mysql_fetch_row(res))
		{
			count1 = atoi(row[0]);
		}
	}
	else
		std::cout << "Blad " << mysql_error(&mysql);
	ss.str("");
	ss.clear();
	ss << "SELECT COUNT(*) from telefon where numer_telefonu = '" << numer_odbierajacego << "'";
	zapytanie = ss.str();
	q = zapytanie.c_str();
	qstate = mysql_query(&mysql, q);
	res = mysql_store_result(&mysql);
	if (qstate == 0)
	{
		while (row = mysql_fetch_row(res))
		{
			count2 = atoi(row[0]);
		}
	}
	else
		std::cout << "Blad " << mysql_error(&mysql);
	if (count1 == 0 && count2 == 0)
	{
		std::cout << "Przynajmniej jeden numer musi pochodzic z bazy danych." << std::endl;
		return;
	}
	std::cout << "Podaj czas rozmowy w formiacie HH:MM:SS:  ";
	std::cin >> czas;


	ss.str("");
	ss.clear();
	ss << "INSERT INTO historia(dzwoniacy, rozmowca, czas) VALUES('" << numer_dzwoniacego << "','" << numer_odbierajacego << "','" << czas << "')";
	zapytanie = ss.str();
	q = zapytanie.c_str();
	qstate =mysql_query(&mysql, q);
	if (qstate == 0) std::cout << "DODANO!" << std::endl;
	else std::cout << "Blad " << mysql_error(&mysql) << std::endl;
	mysql_close(&mysql);
}


void historia_t::historia_od_usera(std::string dzwoniacy, std::string odbierajacy, std::string czas)
{
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "127.0.0.1", "root", "", "telefonia", 0, NULL, 0);
	int qstate;
	std::string zapytanie;
	std::stringstream ss;
	const char* q;
	ss.str("");
	ss.clear();
	ss << "INSERT INTO historia(dzwoniacy, rozmowca, czas) VALUES('" << dzwoniacy << "','" << odbierajacy << "','" <<  czas << "')";
	zapytanie = ss.str();
	q = zapytanie.c_str();
	qstate = mysql_query(&mysql, q);
	if (qstate == 0) std::cout << "DODANO!" << std::endl;
	else std::cout << "Blad " << mysql_error(&mysql) << std::endl;
	mysql_close(&mysql);

}


void historia_t::wyswietl_historie()
{
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "127.0.0.1", "root", "", "telefonia", 0, NULL, 0);
	MYSQL_ROW row;
	MYSQL_RES* res;	
	mysql_query(&mysql, "SELECT * from historia");
	res = mysql_store_result(&mysql);
	while (row = mysql_fetch_row(res))
	{
		std::cout << row[1] << "\t | \t" << row[2] << "\t | \t" << row[3] << "\t | \t" << row[4] << std::endl << std::endl;
	}

	mysql_close(&mysql);
}

void historia_t::wyswietl_historie(std::string user)
{
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "127.0.0.1", "root", "", "telefonia", 0, NULL, 0);
	MYSQL_ROW row;
	MYSQL_RES* res;
	MYSQL_RES* res2;
	std::string zapytanie, numer;
	std::stringstream ss;
	int qstate;
	const char* q;
	ss << "SELECT numer_telefonu from uzytkownik u, umowa um, telefon t where u.id_uzytkownika = um.id_klienta and um.id_numeru = t.id_numeru and login = '" << user << "'";
	zapytanie = ss.str();
	q = zapytanie.c_str();
	qstate = mysql_query(&mysql, q);
	res = mysql_store_result(&mysql);
	if (qstate == 0)
	{
		while (row = mysql_fetch_row(res))
		{
			numer = row[0];
			ss.str("");
			ss.clear();
			ss << "SELECT * from historia where dzwoniacy = '" << numer << "' or rozmowca = '" << numer << "'";
			zapytanie = ss.str();
			q = zapytanie.c_str();
			mysql_query(&mysql, q);
			res2 = mysql_store_result(&mysql);
			while (row = mysql_fetch_row(res2))
			{
				std::cout << row[1] << "\t | \t" << row[2] << "\t | \t" << row[3] << "\t | \t" << row[4] << std::endl << std::endl;
			}

		}
	}
	else
		std::cout << "Blad " << mysql_error(&mysql) << std::endl;
	mysql_close(&mysql);

}