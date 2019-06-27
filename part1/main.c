// Открываем файлы и записываем туда матрицы
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <errno.h>
#include "matrio.h"
#include "mytype.h"


int main(void)
{
   
    FILE *fPtr1;// указатель файла matrix1.dat
    FILE *fPtr2;// указатель файла matrix2.dat

    //  строки,столбцы,кол-во элементов матриц
    int ROWS1, COLS1, num1,ROWS2, COLS2, num2; 
    
    //указатели, на две матрицы
    // хорошая привычка обнулять указатели при объявлении
    t_item *matr1=NULL;
    t_item *matr2=NULL;
  
    //Создаем файлы для записи.Если файлы уже существуют, их содержимое пропадает
    OpenMatrixFile(&fPtr1, "matrix1.dat", "wb");
    OpenMatrixFile(&fPtr2, "matrix2.dat", "wb");
    

    // Получаем информацию о матрицах от пользователя

            // для МАТРИЦЫ 1
    printf("[Matr1] number of lines: ");
    scanf_s("%d", &ROWS1, 1);
    printf("[Matr1] number of columns: ");
    scanf_s("%d", &COLS1, 1);
    printf("\n");
    num1 = ROWS1 * COLS1;  //Кол-во элементов массива 1
 
 
           // для МАТРИЦЫ 2
    printf("[Matr2] number of lines: ");
    scanf_s("%d", &ROWS2, 1);
    printf("[Matr2] number of columns: ");
    scanf_s("%d", &COLS2, 1);
    printf("\n");
    num2 = ROWS2 * COLS2;  //Кол-во элементов массива 2
 
    //Динамическое выделение памяти под матрицы
    matr1  = (t_item*)calloc(num1, sizeof(t_item));
    matr2  = (t_item*)calloc(num2, sizeof(t_item));
 

    SetAr(matr1, num1);  //Заполняет  матрицу 1
    SetAr(matr2, num2);  //Заполняет  матрицу 2
   
 
    PrintAr(matr1, ROWS1, COLS1);  //Выводим матрицу 1 на экран
    printf("\n\n");
    PrintAr(matr2, ROWS2, COLS2);  //Выводим матрицу 2 на экран

    /*Записываем в 1-ый файл информацию о матрице 1 */
    WriteMatrixToFile(fPtr1, matr1, num1, ROWS1, COLS1);
    /*Записываем во 2-ой файл информацию о матрице 2 */
    WriteMatrixToFile(fPtr2, matr2, num2, ROWS2, COLS2);
        
    /*закрыть файл 1*/
    SmartCloseFile(fPtr1);
    /*закрыть файл 2*/
    SmartCloseFile(fPtr2);

    // освобождаем память
    // Т.к. далее вызывается ReadMatrixFromFile
    free(matr1);
    free(matr2);

    // Записали матрицы в файлы. Теперь проверим их - прочитав
    printf("Wrote the matrixs in files. Now to check read them.\n");
    printf("\nEnter\n");
    _getch();

    //Открываем файлы на чтение
    OpenMatrixFile(&fPtr1, "matrix1.dat", "rb");
    OpenMatrixFile(&fPtr2, "matrix2.dat", "rb");

    ReadMatrixFromFile(fPtr1, &matr1, &num1, &ROWS1, &COLS1);
    ReadMatrixFromFile(fPtr2, &matr2, &num2, &ROWS2, &COLS2);

    PrintAr(matr1, ROWS1, COLS1);  //Выводим матрицу 1 на экран
    printf("\n\n");
    PrintAr(matr2, ROWS2, COLS2);  //Выводим матрицу 2 на экран

    /*закрыть файл 1*/
    SmartCloseFile(fPtr1);
    /*закрыть файл 2*/
    SmartCloseFile(fPtr2);

    // ещё раз освобождаем память
    free(matr1);
    free(matr2);

    printf("[OK]\n");
    _getch();
   
    return 0;
 
}