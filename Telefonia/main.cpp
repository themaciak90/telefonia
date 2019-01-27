#include <iostream>
#include <mysql.h>
#include "telefonia_t.h"

int main()
{
	telefonia_t t1;
	int petelka = 1;
	char x;
	while (petelka)
	{
		t1.logowanie();
		std::cout << "Zalogowac jeszcze raz? Kliknik 'q', aby wyjsc" << std::endl;
		std::cin >> x;
		if (x == 'q') petelka = 0;
	}
	getchar();
	return 0;
}