#include "telefonia_t.h"


unsigned long hashowanko(std::string str)
{
	unsigned long hash = 5381;

	for (auto c : str)
		hash = ((hash << 5) + hash) + c;

	return hash;
}


telefonia_t::telefonia_t()
{
}


void telefonia_t::logowanie()
{
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "127.0.0.1", "root", "", "telefonia", 0, NULL, 0);
	MYSQL_ROW row;
	MYSQL_RES* res;
	std::string username, pass, odcz_pass, zapytanie, result , acces;
	int check, qstate;
	unsigned long hashed;
	const char* q;
	std::stringstream ss;
	std::cout << "Podaj login: ";
	std::cin >> username;
	std::cout << "Podaj haslo: ";
	std::cin >> pass;
	ss << "SELECT COUNT(*) from uzytkownik where Login = '" << username << "'";
	zapytanie = ss.str();
	q = zapytanie.c_str();
	qstate = mysql_query(&mysql, q);
	res = mysql_store_result(&mysql);
	if (qstate == 0)
	{
		while (row = mysql_fetch_row(res))
		{
			check = atoi(row[0]);
		}
	}
	else
		std::cout << "Blad " << mysql_error(&mysql);
	if (check == 0)
	{
		std::cout << "Brak takiego uzytkownika." << std::endl;
		getchar();
		return;
	}

	ss.str("");
	ss.clear();
	ss << "Select Haslo,uprawnienia from uzytkownik where login = '" << username << "'";
	zapytanie = ss.str();
	q = zapytanie.c_str();
	qstate = mysql_query(&mysql, q);
	res = mysql_store_result(&mysql);
	if (qstate == 0)
	{
		while (row = mysql_fetch_row(res))
		{
			odcz_pass = row[0];
			acces = row[1];
		}
	}
	else
		std::cout << "Blad " << mysql_error(&mysql);
	ss.str("");
	ss.clear();
	hashed = hashowanko(pass);
	ss << std::hex << hashed;
	result = ss.str();
	
	if (result == odcz_pass)
	{
		std::cout << "Witaj " << username << "." << std::endl;
		if (acces == "Admin")
			menu_admina(username);
		else
			menu_uzytkownika(username);
	}
	else
	{
		std::cout << "Niepoprawne haslo." << std::endl;
		getchar();
		return;
	}
	getchar();

}




void telefonia_t::menu_admina(std::string helper)
{
	while (1)
	{
		std::cout << "~~~~~~~~~~~~~~~~~~~~~~ ADMIN ~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
		std::cout << "1.Dodaj uzytkownika." << std::endl;
		std::cout << "2.Dodaj numer." << std::endl;
		std::cout << "3.Wyswietl umowy." << std::endl;
		std::cout << "4.Wyswietl klientow." << std::endl;
		std::cout << "5.Wyswietl po numerze." << std::endl;
		std::cout << "6.Wyswietl klienta." << std::endl;
		std::cout << "7.Wyswietl umowe." << std::endl;
		std::cout << "8.Usun klienta." << std::endl;
		std::cout << "9.Usun numer." << std::endl;
		std::cout << "10.Wstaw historie." << std::endl;
		std::cout << "11.Wyswietl historie." << std::endl;
		std::cout << "12.Wyswietl statystyki." << std::endl;
		std::cout << "13.Zadzwon." << std::endl;
		std::cout << "14.Edytuj uzytkownika." << std::endl;
		std::cout << "15.Zakoncz sesje." << std::endl;
		std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
		int wybor, numer;
		uzytkownik_t u1;
		telefon_t t1;
		historia_t h1;
		std::cin >> wybor;
		switch (wybor)
		{
		case 1:
			u1.dodaj_konto();
			break;
		case 2:
			t1.dodaj_numer();
			break;
		case 3:
			wyswietl_umowy();
			break;
		case 4:
			wyswietl_klient();
			break;
		case 5:
			wyswietl_po_numerze();
			break;
		case 6:
			std::cout << "Podaj id klienta: ";
			std::cin >> numer;
			wyswietl_klient(numer);
			break;
		case 7:
			wyswietl_umowe();
			break;
		case 8:
			u1.usun_konto();
			break;
		case 9:
			t1.usun_numer();
			break;
		case 10:
			h1.wstaw_historie_admin();
			break;
		case 11:
			h1.wyswietl_historie();
			break;
		case 12:
			u1.sredni_wiek();
			u1.rozklad_plci();
			u1.srednia_ilosc_numerow();
			break;
		case 13:
			u1.zadzwon(helper);
			break;
		case 14:
			u1.edytuj();
			break;
		case 15:
			return;
			break;

		default:
			std::cout << "Brak takiej opcji" << std::endl;
		}
	}
}

void telefonia_t::menu_uzytkownika(std::string helper)
{
	uzytkownik_t u1;
	telefon_t t1;
	historia_t h1;
	while (1)
	{
		std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
		std::cout << "1.Wyswietl swoja umowe." << std::endl;
		std::cout << "2.Zadzwon." << std::endl;
		std::cout << "3.Wyswietl historie." << std::endl;
		std::cout << "4.Zmien haslo." << std::endl;
		std::cout << "5.Zakoncz sesje." << std::endl;
		std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
		int wybor;
		std::cin >> wybor;
		switch (wybor)
		{
		case 1:
			u1.wyswietl_swoje_numery(helper);
			break;
		case 2:
			u1.zadzwon(helper);
			break;
		case 3:
			h1.wyswietl_historie(helper);
			break;
		case 4:
			u1.zmien_haslo(helper);
			break;
		case 5:
			return;
			break;

		default:
			std::cout << "Brak takiej opcji" << std::endl;
		}
	}

}






void telefonia_t::wyswietl_umowy()
{
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "127.0.0.1", "root", "", "telefonia", 0, NULL, 0);
	MYSQL_ROW row;
	MYSQL_RES* res;
	mysql_query(&mysql, "SELECT imie, nazwisko, numer_telefonu from telefon t, umowa um, uzytkownik u WHERE t.id_numeru = um.id_numeru and um.id_klienta = u.id_uzytkownika order by nazwisko");
	res = mysql_store_result(&mysql);
	while (row = mysql_fetch_row(res))
	{
		std::cout << row[0] << "\t | \t" << row[1] << "\t | \t" << row[2] << std::endl << std::endl;
	}

	mysql_close(&mysql);

}


void telefonia_t::wyswietl_klient()
{
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "127.0.0.1", "root", "", "telefonia", 0, NULL, 0);
	MYSQL_ROW row;
	MYSQL_RES* res;
	mysql_query(&mysql, "SELECT imie, nazwisko, wiek, plec uprawnienia from uzytkownik");
	res = mysql_store_result(&mysql);
	while (row = mysql_fetch_row(res))
	{
		std::cout << row[0] << "\t | \t" << row[1] << "\t | \t" << row[2] << "\t | \t" << row[3] << "\t | \t" << row[4] << std::endl << std::endl;
	}

	mysql_close(&mysql);

}


void telefonia_t::wyswietl_po_numerze()
{
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "127.0.0.1", "root", "", "telefonia", 0, NULL, 0);
	MYSQL_ROW row;
	MYSQL_RES* res;
	int id;
	std::string numer, zapytanie;
	std::stringstream ss;
	const char* q;
	std::cout << "Podaj numer telefonu: ";
	std::cin >> numer;
	std::regex reg("^[0-9]{9}$");
	while (!std::regex_match(numer, reg))
	{
		std::cout << "Nie prawidlowy numer telefonu. Podaj ponownie: ";
		std::cin >> numer;
	}

	ss << "SELECT id_klienta from umowa, telefon where umowa.Id_numeru = telefon.id_numeru and numer_telefonu = '" << numer << "'";
	zapytanie = ss.str();
	q = zapytanie.c_str();
	mysql_query(&mysql, q);
	res = mysql_store_result(&mysql);
	while (row = mysql_fetch_row(res))
	{
		id = atoi(row[0]);
	}
	ss.str("");
	ss.clear();
	ss << "SELECT imie, nazwisko, wiek, plec uprawnienia from uzytkownik where id_uzytkownika = " << id;
	zapytanie = ss.str();
	q = zapytanie.c_str();
	mysql_query(&mysql, q);
	res = mysql_store_result(&mysql);
	while (row = mysql_fetch_row(res))
	{
		std::cout << row[0] << "\t | \t" << row[1] << "\t | \t" << row[2] << "\t | \t" << row[3] << "\t | \t" << row[4] << std::endl << std::endl;
	}

	mysql_close(&mysql);
}


void telefonia_t::wyswietl_klient(int id)
{
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "127.0.0.1", "root", "", "telefonia", 0, NULL, 0);
	MYSQL_ROW row;
	MYSQL_RES* res;
	std::string zapytanie;
	std::stringstream ss;
	const char* q;
	ss << "SELECT imie, nazwisko, wiek, plec uprawnienia from uzytkownik where id_uzytkownika = " << id;
	zapytanie = ss.str();
	q = zapytanie.c_str();
	mysql_query(&mysql, q);
	res = mysql_store_result(&mysql);
	while (row = mysql_fetch_row(res))
	{
		std::cout << row[0] << "\t | \t" << row[1] << "\t | \t" << row[2] << "\t | \t" << row[3] << "\t | \t" << row[4] << std::endl << std::endl;
	}

	mysql_close(&mysql);
}


void telefonia_t::wyswietl_umowe()
{
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "127.0.0.1", "root", "", "telefonia", 0, NULL, 0);
	MYSQL_ROW row;
	MYSQL_RES* res;
	int id, qstate;
	std::string zapytanie, imie, nazwisko;
	std::stringstream ss;
	const char* q;
	std::cout << "Podaj id klienta: ";
	std::cin >> id;
	ss << "SELECT imie, nazwisko from uzytkownik where id_uzytkownika = " << id;
	zapytanie = ss.str();
	q = zapytanie.c_str();
	mysql_query(&mysql, q);
	res = mysql_store_result(&mysql);
	while (row = mysql_fetch_row(res))
	{
		imie = row[0];
		nazwisko = row[1];
	}
	ss.str("");
	ss.clear();
	ss << "SELECT numer_telefonu from telefon t, umowa um, uzytkownik u WHERE t.id_numeru = um.id_numeru and um.id_klienta = u.id_uzytkownika and id_uzytkownika = " << id;
	zapytanie = ss.str();
	q = zapytanie.c_str();
	qstate = mysql_query(&mysql, q);
	if (qstate == 0)
	{
		res = mysql_store_result(&mysql);
		std::cout << "UMOWA: " << imie << " " << nazwisko << std::endl;
		while (row = mysql_fetch_row(res))
		{
			std::cout << row[0] << std::endl << std::endl;
		}
	}
	else std::cout << "Blad " << mysql_error(&mysql);

	mysql_close(&mysql);
}