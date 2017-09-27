/*Программа для умножения матриц с использованием OpenMP
- матрицы целых чисел, 
- динамические массивы, 
- ввод с клавиатуры -> закомментировано, переделано -> заполнение без клавиатуры,
- кол-во потоков по умолчанию 4
- schedule(static, chunk) в цикле с вычислением ячеек результирующей матрицы,
- schedule(static, chunk) в цикле с инициализацией без заполнения с клавиатуры
- вывод результата на экран*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main()
{
	int mat1_1d, mat2_1d, mat1_2d, mat2_2d;  //размеры двух матриц
	double ** matr1, ** matr2, ** result_matr;  //сами матрицы + результирующая
	int i, j, k;                             //счетчики для циклов
	int chunk = 1;                           //для schedule

	printf("This programme will multiply matrices\nUsing OpenMP\n\n");
	printf("Enter the size of the first matrix (n m): ");
	scanf("%d", &mat1_1d);
	scanf("%d", &mat1_2d);
	printf("Enter the size of the second matrix (n m): ");
	scanf("%d", &mat2_1d);
	scanf("%d", &mat2_2d);

	//проверка размеров матриц
	if (mat1_2d != mat2_1d || mat1_1d <= 0 || mat1_2d <= 0 || mat2_1d <= 0 || mat2_2d <= 0)
	{
		printf("M1 row length must be equal to M2 column length!\nAnd only positive numbers, please...\n\n");
		getch();
	}
	else
	{
		////for debug
		//printf("1st matrix is %d x %d\n", mat1_1d, mat1_2d);
		//printf("2nd is %d x %d\n\n", mat2_1d, mat2_2d);

		//создание матриц
		matr1 = malloc(mat1_1d * sizeof(double*));
		for (i = 0; i < mat1_1d; i++) {
			matr1[i] = malloc(mat1_2d * sizeof(double));
		}
		matr2 = malloc(mat2_1d * sizeof(double*));
		for (i = 0; i < mat2_1d; i++) {
			matr2[i] = malloc(mat2_2d * sizeof(double));
		}

		//заполнение матриц с клавиатуры
		printf("Fill in the 1st matrix (A1 ... An):\n");
		for (i = 0; i < mat1_1d; i++)
		{
			for (j = 0; j < mat1_2d; j++)
			{
				scanf("%lf", &matr1[i][j]);
			}
		}
		printf("Fill in the 2nd matrix (B1 ... Bn):\n");
		for (i = 0; i < mat2_1d; i++)
		{
			for (j = 0; j < mat2_2d; j++)
			{
				scanf("%lf", &matr2[i][j]);
			}
		}

		//создание рез. матрицы
		result_matr = malloc(mat1_1d * sizeof(double*));
		for (i = 0; i < mat1_1d; i++) {
			result_matr[i] = malloc(mat2_2d * sizeof(double));
		}
		
		//начало параллельной области
		#pragma omp parallel shared(matr1, matr2, result_matr) private (i,j,k)
		{
			int tid = omp_get_thread_num();

			////for debug инициализация матриц (без ввода с клавиатуры)
			////первая
			//#pragma omp for schedule (static, chunk) 
			//for (i = 0; i < mat1_1d; i++)
			//{
			//	for (j = 0; j < mat1_2d; j++)
			//	{
			//		matr1[i][j] = i * j - j;
			//	}
			//}
			////вторая
			//#pragma omp for schedule (static, chunk)
			//for (i = 0; i < mat2_1d; i++)
			//{
			//	for (j = 0; j < mat2_2d; j++)
			//	{
			//		matr2[i][j] = (i + j)/(j+1);
			//	}
			//}

			//инициализация рез.матрицы нулями
			#pragma omp for schedule (static, chunk)
			for (i = 0; i < mat1_1d; i++) {
				printf("Thread %d initializes %d row\n", tid, i+1);
				for (j = 0; j < mat2_2d; j++) {
					result_matr[i][j] = 0;
				}
			}

			//заполнение новой матрицы
			#pragma omp for schedule (static, chunk)
			for (i = 0; i < mat1_1d; i++) {
				printf("Thread %d computes %d row\n", tid, i + 1);
				for (j = 0; j < mat2_2d; j++) {
					for (k = 0; k < mat1_2d; k++) {
						result_matr[i][j] += matr1[i][k] * matr2[k][j];
					}
				}
			}
		}
		//конец параллельной области
		
		//for debug вывод матриц
		printf("Matrix 1:\n");
		for (i = 0; i < mat1_1d; i++)
		{
			for (j = 0; j < mat1_2d; j++)
			{
				printf("%4.2f ", matr1[i][j]);
				if (j == mat1_2d - 1)
					printf("\n");
			}
		}
		printf("\n");
		printf("Matrix 2:\n");
		for (i = 0; i < mat2_1d; i++)
		{
			for (j = 0; j < mat2_2d; j++)
			{
				printf("%4.2f ", matr2[i][j]);
				if (j == mat2_2d - 1)
					printf("\n");
			}
		}
		printf("\n");

		//вывод результирующей матрицы
		printf("\nMatrix M1 x M2:\n");
		for (i = 0; i < mat1_1d; i++)
		{
			for (j = 0; j < mat2_2d; j++)
			{
				printf("%4.2f ", result_matr[i][j]);
				if (j == mat2_2d - 1)
					printf("\n");
			}
		}
		printf("\n");
		printf("Press any key to exit...\n");
		getch();

		//удаление матриц из памяти
		for (i = 0; i < mat1_1d; i++) {
			free(matr1[i]);
		}
		free(matr1);
		for (i = 0; i < mat2_1d; i++) {
			free(matr2[i]);
		}
		free(matr2);
		for (i = 0; i < mat1_1d; i++) {
			free(result_matr[i]);
		}
		free(result_matr);
	}

	return 0;
}