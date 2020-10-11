#include <Windows.h>

#include "functions.h"

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	struct matrixStruct* matrix = matrix_definition();
	system("cls");

	while (processing_menu(matrix)) {
		system("cls");
	}

	return 0;
}