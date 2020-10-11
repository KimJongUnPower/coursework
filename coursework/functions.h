#pragma once
#include <stdbool.h>

struct matrixStruct;
struct menuPoint;

struct matrixStruct* matrix_definition();
void redefine_matrix(struct matrixStruct*);

void input_matrix_random(struct matrixStruct*);
void input_matrix_user(struct matrixStruct*);
void input_matrix_from_file(struct matrixStruct*);

void handler_matrix(struct matrixStruct*);
void sort_matrix_column(struct matrixStruct*);
void find_string_with_negative_numbers(struct matrixStruct*);
void execute_task(struct matrixStruct*);

void output_matrix(struct matrixStruct*);

bool processing_menu(struct matrixStruct*);