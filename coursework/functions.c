#include "functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>

struct matrixStruct {
	int** matrix;
	int height;
	int width;
};

struct menuPoint{
	char name[60];
	void (*function)(struct matrixStruct*);
};

struct matrixStruct* matrix_definition() {
	struct matrixStruct* matrix = (struct matrixStruct*)malloc(sizeof(struct matrixStruct));

	while (true) {
		printf_s("%s", "Введите количество строк: ");
		scanf_s("%d", &(matrix->height));
		printf_s("%s", "Введите количество столбцов: ");
		scanf_s("%d", &(matrix->width));

		if (matrix->height > 0 && matrix->width > 0)
			break;

		puts("Введите коректные значения длины массива");
	}
	

	matrix->matrix = (int**)malloc(matrix->height* sizeof(int*));
	for (register int index = 0; index < matrix->height; index++) {
		matrix->matrix[index] = (int*)malloc(matrix->width * sizeof(int));
	}
	
	return matrix;
}

void input_matrix_random(struct matrixStruct* matrix) {
	srand((unsigned int)time(NULL));

	for (register int height_index = 0; height_index < matrix->height; height_index++) {
		for (register int width_index = 0; width_index < matrix->width; width_index++){
			matrix->matrix[height_index][width_index] = rand();
			if ((rand() % 10) == 3) {
				matrix->matrix[height_index][width_index] *= -1;
			}
		}
	}
};

void input_matrix_user(struct matrixStruct *matrix) {
	puts("Заполните матрицу");

	for (register int height_index = 0; height_index < matrix->height; height_index++) {
		for (register int width_index = 0; width_index < matrix->width; width_index++) {
			scanf_s("%d", matrix->matrix[height_index] + width_index);
		}
	}
};

void input_matrix_from_file(struct matrixStruct *matrix) {
	/*Функция считывает бинарный файл с записыннми в него переменными типа int*/
	char file_name[50];
	FILE* fd = NULL;

	while (true) {
		printf_s("%s", "Введите название файла: ");
		scanf_s("%s", file_name, 50);

		fopen_s(fd, file_name, "rb");
		if (fd != NULL) {

			fseek(fd, 0, SEEK_END);
			if (ftell(fd) / sizeof(int) == matrix->height * matrix->width) {
				fseek(fd, 0, SEEK_SET);
				break;
			}

			fclose(fd);
		}

		puts("Ошибка открытия файла");
	}
	
	int buff;

	for (register int height_index = 0; height_index < matrix->height; height_index++) {
		for (register int width_index = 0; width_index < matrix->width; width_index++) {
			fread(matrix->matrix[height_index]+width_index, sizeof(int), 1, fd);
		}
	}

	fclose(fd);
};

void handler_matrix(struct matrixStruct *matrix) {
	register bool there_are_negative;
	register buf = matrix->height * matrix->width;

	for (register int height_index = 0; height_index < matrix->height; height_index++) {
		there_are_negative = false;
		for (register int width_index = 0; width_index < matrix->width; width_index++) {
			if (matrix->matrix[height_index][width_index] < 0) {
				there_are_negative = true;
				break;
			}
		}

		if (there_are_negative) {
			for (register int width_index = 0; width_index < matrix->width; width_index++) {
				matrix->matrix[height_index][width_index] += buf;
			}
		}
	}
};

void find_string_with_negative_numbers(struct matrixStruct *matrix) {
	int line = -1;
	unsigned int max_negative_numbers = 0;
	register unsigned int negative_numbers_on_line;

	for (register int height_index = 0; height_index < matrix->height; height_index++) {
		negative_numbers_on_line = 0;
		for (register int width_index = 0; width_index < matrix->width; width_index++) {
			if (matrix->matrix[height_index][width_index] < 0) {
				negative_numbers_on_line++;
			}
		}

		if (negative_numbers_on_line > max_negative_numbers) {
			max_negative_numbers = negative_numbers_on_line;
			line = height_index;
		}
	}

	if (line == -1) {
		puts("В матрице нет отрицательных чисел\n");
	}
	else {
		printf_s("Строка с максимальным количеством отрицательных чисел: %d\n", line);
	}
};

void sort_matrix_column(struct matrixStruct *matrix) {
	int column_index;
	while (true) {
		printf_s("%s", "Введите колонку для сортировки: ");
		scanf_s("%d", &column_index);

		if (column_index >= 0 && column_index < matrix->width) {
			break;
		}
		puts("Индекс не входит в параметры матрицы");
	}

	int buff;
	for (int index = 0; index < matrix->height - 1; index++) {
		for (int cycle_index = index + 1; cycle_index < matrix->height; cycle_index++) { 
			if (matrix->matrix[cycle_index][column_index] < matrix->matrix[index][column_index]) { 
				buff = matrix->matrix[index][column_index];
				matrix->matrix[index][column_index] = matrix->matrix[cycle_index][column_index];
				matrix->matrix[cycle_index][column_index] = buff;
			}
		}
	}	
};

void execute_task(struct matrixStruct* matrix) {
	puts("Исходная матрица");
	output_matrix(matrix);
	puts("");
	puts("Обрабатываю матрицу");
	handler_matrix(matrix);
	puts("");
	output_matrix(matrix);
	puts("");
	find_string_with_negative_numbers(matrix);
	puts("");
	puts("Выполняю сортировку колонки");
	sort_matrix_column(matrix);
	puts("");
	output_matrix(matrix);
};

void output_matrix(struct matrixStruct *matrix) {
	for (register int height_index = 0; height_index < matrix->height; height_index++) {
		for (register int width_index = 0; width_index < matrix->width; width_index++) {
			printf_s("%d ", matrix->matrix[height_index][width_index]);
		}
		puts("");
	}
};

bool processing_menu(struct matrixStruct* matrix) {
	
	struct menuPoint menu_points[] = {
		{
			"Вывод матрицы",
			output_matrix,
		},
		{
			"Заполнение матрицы случайными числами",
			input_matrix_random,
		},
		{
			"Ввод матрицы с клавиатуры",
			input_matrix_user,
		},
		{
			"Ввод матрицы из файла",
			input_matrix_from_file,
		},
		{
			"Выполнить задание полностью",
			execute_task,
		},
		{
			"Увелечение строк матрицы с отриц. эл. на размер матрицы",
			handler_matrix,
		},
		{
			"Поиск строки с наибольшим кол-во отриц. элементов",
			find_string_with_negative_numbers,
		},
		{
			"Сортировка столбца матрицы согласно варианту 10",
			sort_matrix_column,
		},
	};

	for (register int index_menu_point = 0; index_menu_point < (sizeof(menu_points) / sizeof(struct menuPoint)); index_menu_point++) {
		printf_s("%d. %s\n", index_menu_point+1, menu_points[index_menu_point].name);
	}
	puts("Выход из программы любая иная цифра");
	printf_s("%s", "Введите цифру из меню: ");

	unsigned int menu_point;
	scanf_s("%d", &menu_point);

	if (menu_point > (sizeof(menu_points) / sizeof(struct menuPoint)) || menu_point == 0) {
		return false;
	}

	menu_point--;
	system("cls");
	printf_s("Выполняю функцию: %s\n", menu_points[menu_point].name);

	menu_points[menu_point].function(matrix);

	puts("");
	printf_s("%s", "Функция выполнена, нажмите любую клавишу");
	getch();

	return true;
};
