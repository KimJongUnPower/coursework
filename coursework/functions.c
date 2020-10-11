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
		printf_s("%s", "������� ���������� �����: ");
		scanf_s("%d", &(matrix->height));
		printf_s("%s", "������� ���������� ��������: ");
		scanf_s("%d", &(matrix->width));

		if (matrix->height > 0 && matrix->width > 0)
			break;

		puts("������� ��������� �������� ����� �������");
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
	puts("��������� �������");

	for (register int height_index = 0; height_index < matrix->height; height_index++) {
		for (register int width_index = 0; width_index < matrix->width; width_index++) {
			scanf_s("%d", matrix->matrix[height_index] + width_index);
		}
	}
};

void input_matrix_from_file(struct matrixStruct *matrix) {
	/*������� ��������� �������� ���� � ���������� � ���� ����������� ���� int*/
	char file_name[50];
	FILE* fd = NULL;

	while (true) {
		printf_s("%s", "������� �������� �����: ");
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

		puts("������ �������� �����");
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
		puts("� ������� ��� ������������� �����\n");
	}
	else {
		printf_s("������ � ������������ ����������� ������������� �����: %d\n", line);
	}
};

void sort_matrix_column(struct matrixStruct *matrix) {
	int column_index;
	while (true) {
		printf_s("%s", "������� ������� ��� ����������: ");
		scanf_s("%d", &column_index);

		if (column_index >= 0 && column_index < matrix->width) {
			break;
		}
		puts("������ �� ������ � ��������� �������");
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
	puts("�������� �������");
	output_matrix(matrix);
	puts("");
	puts("����������� �������");
	handler_matrix(matrix);
	puts("");
	output_matrix(matrix);
	puts("");
	find_string_with_negative_numbers(matrix);
	puts("");
	puts("�������� ���������� �������");
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
			"����� �������",
			output_matrix,
		},
		{
			"���������� ������� ���������� �������",
			input_matrix_random,
		},
		{
			"���� ������� � ����������",
			input_matrix_user,
		},
		{
			"���� ������� �� �����",
			input_matrix_from_file,
		},
		{
			"��������� ������� ���������",
			execute_task,
		},
		{
			"���������� ����� ������� � �����. ��. �� ������ �������",
			handler_matrix,
		},
		{
			"����� ������ � ���������� ���-�� �����. ���������",
			find_string_with_negative_numbers,
		},
		{
			"���������� ������� ������� �������� �������� 10",
			sort_matrix_column,
		},
	};

	for (register int index_menu_point = 0; index_menu_point < (sizeof(menu_points) / sizeof(struct menuPoint)); index_menu_point++) {
		printf_s("%d. %s\n", index_menu_point+1, menu_points[index_menu_point].name);
	}
	puts("����� �� ��������� ����� ���� �����");
	printf_s("%s", "������� ����� �� ����: ");

	unsigned int menu_point;
	scanf_s("%d", &menu_point);

	if (menu_point > (sizeof(menu_points) / sizeof(struct menuPoint)) || menu_point == 0) {
		return false;
	}

	menu_point--;
	system("cls");
	printf_s("�������� �������: %s\n", menu_points[menu_point].name);

	menu_points[menu_point].function(matrix);

	puts("");
	printf_s("%s", "������� ���������, ������� ����� �������");
	getch();

	return true;
};
