#include "uzytkownik_t.h"



unsigned long hashowanko2(std::string str)
{
	unsigned long hash = 5381;

	for (auto c : str)
		hash = ((hash << 5) + hash) + c;

	return hash;
}


uzytkownik_t::uzytkownik_t()
{
}


void uzytkownik_t::dodaj_konto()
{
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "127.0.0.1", "root", "", "telefonia", 0, NULL, 0);
	std::string zapytanie, haselko, numer;
	std::cout << "Podaj imie: ";
	std::cin >> imie;
	std::cout << "Podaj nazwisko: ";
	std::cin >> nazwisko;
	std::cout << "Podaj wiek: ";
	std::cin >> wiek;
	std::cout << "Podaj plec: ";
	std::cin >> plec;
	std::cout << "Podaj login: ";
	std::cin >> login;
	std::cout << "Podaj haslo: ";
	std::cin >> haslo;
	std::cout << "Podaj uprawnienia: ";
	std::cin >> uprawnienia;
	std::cout << "Podaj numer telefonu: ";
	std::cin >> numer;
	std::regex reg("^[0-9]{9}$");
	while (!std::regex_match(numer, reg))
	{
		std::cout << "Nie prawidlowy numer telefonu. Podaj ponownie: ";
		std::cin >> numer;
	}
	std::stringstream ss;
	int qstate, id_user, id_number;
	unsigned long pass;
	pass = hashowanko2(haslo);
	ss << std::hex << pass;
	haselko = ss.str();
	ss << std::dec << wiek;
	ss.str("");
	ss.clear();
	if (uprawnienia == "Admin")
		ss << "INSERT INTO uzytkownik (imie, nazwisko, wiek, plec, uprawnienia, login, haslo) VALUES ('" << imie << "','" << nazwisko << "'," << wiek << ",'" << plec << "','" << uprawnienia << "','" << login << "','" << haselko << "')";
	else 
		ss << "INSERT INTO uzytkownik (imie, nazwisko, wiek, plec, login, haslo) VALUES ('" << imie << "','" << nazwisko << "'," << wiek << ",'" << plec << "','" << login << "','" << haselko << "')";
	zapytanie = ss.str();
	const char* q = zapytanie.c_str();
	qstate = mysql_query(&mysql, q);
	if (qstate == 0) std::cout << "Dodano!" << std::endl;
	else std::cout << "Blad " << mysql_error(&mysql);
	ss.str("");
	ss.clear();
	ss << "INSERT INTO telefon(numer_telefonu) VALUES ('" << numer << "')";
	zapytanie = ss.str();
	q = zapytanie.c_str();
	qstate = mysql_query(&mysql, q);
	if (qstate == 0) std::cout << "Dodano!" << std::endl;
	else std::cout << "Blad " << mysql_error(&mysql);
	ss.str("");
	ss.clear();
	ss << "SELECT id_uzytkownika from uzytkownik where login = '" << login << "'";
	MYSQL_ROW row;
	MYSQL_RES* res;
	zapytanie = ss.str();
	q = zapytanie.c_str();
	qstate = mysql_query(&mysql, q);
	res = mysql_store_result(&mysql);
	if (qstate == 0)
	{
		while (row = mysql_fetch_row(res))
		{
			id_user = atoi(row[0]);
		}
	}
	else
		std::cout << "Blad " << mysql_error(&mysql);
	mysql_close(&mysql);
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "127.0.0.1", "root", "", "telefonia", 0, NULL, 0);
	ss.str("");
	ss.clear();
	ss << "SELECT id_numeru from telefon where numer_telefonu = '" << numer << "'";
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




void uzytkownik_t::usun_konto()
{
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "127.0.0.1", "root", "", "telefonia", 0, NULL, 0);
	MYSQL_ROW row;
	MYSQL_RES* res;
	std::string zapytanie;
	std::stringstream ss;
	const char* q;
	int id, count, qstate;
	std::cout << "Podaj id uzytkownika: ";
	std::cin >> id;
	ss << "SELECT COUNT(*) from uzytkownik where id_uzytkownika = " << id;
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
	ss << "DELETE FROM uzytkownik where id_uzytkownika = " << id;
	zapytanie = ss.str();
	q = zapytanie.c_str();
	mysql_query(&mysql, q);
	ss.str("");
	ss.clear();
	ss << "SELECT id_numeru FROM umowa where id_klienta = " << id;
	zapytanie = ss.str();
	q = zapytanie.c_str();
	mysql_query(&mysql, q);
	res = mysql_store_result(&mysql);
	while (row = mysql_fetch_row(res))
	{
		count = atoi(row[0]);
		ss.str("");
		ss.clear();
		ss << "DELETE FROM telefon where id_numeru = " << count;
		zapytanie = ss.str();
		q = zapytanie.c_str();
		mysql_query(&mysql, q);

	}
	ss.str("");
	ss.clear();
	ss << "DELETE FROM umowa where id_klienta = " << id;
	zapytanie = ss.str();
	q = zapytanie.c_str();
	mysql_query(&mysql, q);
	mysql_close(&mysql);
}


void uzytkownik_t::wyswietl_swoje_numery(std::string user)
{
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "127.0.0.1", "root", "", "telefonia", 0, NULL, 0);
	MYSQL_ROW row;
	MYSQL_RES* res;
	std::string zapytanie;
	std::stringstream ss;
	login = user;
	const char* q;
	ss << "SELECT numer_telefonu from uzytkownik u, telefon t, umowa um where u.Id_uzytkownika = um.Id_klienta and t.id_numeru = um.Id_numeru and login = '" << login << "'";
	zapytanie = ss.str();
	q = zapytanie.c_str();
	int qstate = mysql_query(&mysql, q);
	res = mysql_store_result(&mysql);
	if (qstate == 0)
	{
		std::cout << "Numery: " << std::endl;
		while (row = mysql_fetch_row(res))
		{
			std::cout << row[0] << std::endl << std::endl;
		}
	}
	else
		std::cout << "Blad " << mysql_error(&mysql);
	mysql_close(&mysql);

}



void uzytkownik_t::zadzwon(std::string user)
{
	historia_t h1;
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "127.0.0.1", "root", "", "telefonia", 0, NULL, 0);
	MYSQL_ROW row;
	MYSQL_RES* res;
	std::string zapytanie, numer, do_kogo, data, data2;
	std::stringstream ss;
	login = user;
	const char* q;
	ss << "SELECT numer_telefonu from uzytkownik u, telefon t, umowa um where u.Id_uzytkownika = um.Id_klienta and t.id_numeru = um.Id_numeru and login = '" << login << "'";
	zapytanie = ss.str();
	q = zapytanie.c_str();
	int qstate = mysql_query(&mysql, q);
	res = mysql_store_result(&mysql);
	if (qstate == 0)
	{
		int i = 0, choice;
		std::vector<std::string> numery;
		std::cout << "Wybierz numer, z ktorego chcesz zadzownic: " << std::endl;
		while (row = mysql_fetch_row(res))
		{
			numer = row[0];
			std::cout << i << ". " << row[0] << std::endl << std::endl;
			numery.push_back(numer);
			i++;
		}
		std::cin >> choice;
		while (choice < 0 || choice > i)
		{
			std::cout << "Wybierz istniejacy numer." << std::endl;
			std::cin >> choice;
		}
		std::cout << "Podaj numer na jaki chcesz zadzwonic: " << std::endl;
		std::cin >> do_kogo;
		std::regex reg("^[0-9]{9}$");
		while (!std::regex_match(do_kogo, reg))
		{
			std::cout << "Nie prawidlowy numer telefonu. Podaj ponownie: ";
			std::cin >> do_kogo;
		}
		mysql_query(&mysql, "SELECT CURRENT_TIMESTAMP() from dual");
		res = mysql_store_result(&mysql);
		while (row = mysql_fetch_row(res))
		{
			data = row[0];
		}
		
		std::cout << "Dzwonienie. Nacisnij 'x' zeby rozlaczyc" << std::endl;
		int petla = 1;
		char znak;
		while (petla)
		{
			std::cin >> znak;
			if (znak == 'x') break;
		}
		ss.str("");
		ss.clear();
		ss << "SELECT timediff(CURRENT_TIMESTAMP(),'" << data << "') from dual";
		zapytanie = ss.str();
		q = zapytanie.c_str();
		mysql_query(&mysql, q);
		res = mysql_store_result(&mysql);
		while (row = mysql_fetch_row(res))
		{
			data2 = row[0];
		}
		ss.str("");
		ss.clear();
		h1.historia_od_usera(numery.at(choice), do_kogo,data2);
		
	}
	else
		std::cout << "Blad " << mysql_error(&mysql);
	mysql_close(&mysql);

}


void uzytkownik_t::sredni_wiek()
{
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "127.0.0.1", "root", "", "telefonia", 0, NULL, 0);
	MYSQL_ROW row;
	MYSQL_RES* res;
	int helper, suma = 0;
	std::vector<int> wiek_vector;
	mysql_query(&mysql, "SELECT wiek from uzytkownik");
	res = mysql_store_result(&mysql);
	while (row = mysql_fetch_row(res))
	{
		helper = atoi(row[0]);
		wiek_vector.push_back(helper);
	}
	std::for_each(wiek_vector.begin(), wiek_vector.end(), [&](int x) {suma += x;  });
	helper = wiek_vector.size();
	float avg = (float)suma /(float)helper;
	std::cout << "Sredni wiek uzytkownikow wynosi: " << avg << "lat." << std::endl;
	mysql_close(&mysql);
	

}

void uzytkownik_t::rozklad_plci()
{
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "127.0.0.1", "root", "", "telefonia", 0, NULL, 0);
	MYSQL_ROW row;
	MYSQL_RES* res;
	int liczba_mezczyzn = 0 , liczba_kobiet = 0;
	std::string helper;
	mysql_query(&mysql, "SELECT plec from uzytkownik");
	res = mysql_store_result(&mysql);
	while (row = mysql_fetch_row(res))
	{
		helper = row[0];
		if (helper == "Kobieta") liczba_kobiet++;
		else liczba_mezczyzn++;
	}
	int suma = liczba_kobiet + liczba_mezczyzn;
	float procent_kobiet = ((float)liczba_kobiet / (float)suma)*100;
	float procent_mezczyzn = 100 - procent_kobiet;

	std::cout << "Procent kobiet: " << procent_kobiet << "%" << std::endl;
	std::cout << "Procent mezczyzn: " << procent_mezczyzn << "%" << std::endl;

	mysql_close(&mysql);

}