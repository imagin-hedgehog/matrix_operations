#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <errno.h>
#include "matrio.h"
#include "mytype.h"

/*Открывает файл матрицы с перехватом ошибок*/
void OpenMatrixFile(FILE** fPtr, const char *path, const char *mode)
// FILE** fPtr Нужен для fopen_s
{
    errno_t err = fopen_s(fPtr, path, mode);
        if (err)
        {   
            // Ошибка при открытии файла!
            puts("File could not be open\n");
            _getch();   //задержка после вывода сообщения на консоль    
            exit(1);            
        }
}


/*Считываем матрицу из файла*/
// Перед использование ОБЯЗАТЕЛЬНО проверяйте
// освободили ли вы память
// иначе будет утечка
// Внутри просто используется malloc
void ReadMatrixFromFile(FILE *fPtr, t_item** ar, int *num, int *ROWS, int *COLS)
{  
    int  i, count = 0;
    // По интерфейсу сначала сичтываем
    // Вспомогательную информацию о матрице

    // Размер всей матрицы - число её элементов
    count += fread(num, sizeof(int), 1, fPtr);

    if (count != 1)
    {   printf("<func::ReadMatrixFromFile>\n");
        // Ошибка: Не смогли даже прочитать размер матрицы
        printf("Error: couldn't even read the matrix size\n");
        _getch();
        exit(2);
    }

    // ПОЗЖЕ ДОБАВИТЬ НОВУЮ ПРОВЕРКУ НА РАЗМЕР ФАЙЛА
    // ТАК КАК В ОДНОМ ФАЙЛЕ МОГУТ НАХОДТЬСЯ ДВЕ МАТРЦЫ

    // Размерность
    count += fread(ROWS, sizeof(int), 1, fPtr);
    count += fread(COLS, sizeof(int), 1, fPtr);

     // Проверка всё ли прочиталось
    if (count != 3)
    {   
        printf("<func::ReadMatrixFromFile>\n");
        // Ошибка при чтении данных о матрице(размерность)
        printf("Error when reading data on the matrix(the dimension)\n");
        _getch();
        exit(2);
    }

    if (*num != (*ROWS) * (*COLS))
    {   
        printf("<func::ReadMatrixFromFile>\n");
        // Ошибка: неверный размер матрицы
        printf("Error: wrong matrix size\n");
        // Получено num =
        printf("got num = %d\n", *num);
        // Ожидал 
        printf("expected ROWS * COLS = %d\n", (*ROWS) * (*COLS));
        _getch();
        exit(3);
    }

    //и выделяем память под новую матрицу
    *ar  = (t_item*)calloc(*num, sizeof(t_item));
    // Наконец считываем сами элементы матрицы
    count = 0;
         for(i = 0; i < *num; i++)
    {
        count += fread((*ar+i), sizeof(t_item), 1, fPtr);
    }

    // Прочитались не все элементы матрицы
    if (count != *num)
    {   
        printf("<func::ReadMatrixFromFile>\n");
        // Не смогли прочитать ВСЕ элементы матрицы
        printf("Couldn't read all the matrix elements");
        _getch();
        exit(4);
    }
}

/*Записываем матрицу и информацию о ней в файл*/
void WriteMatrixToFile(FILE *fPtr, t_item* ar, int num, int ROWS, int COLS)
 
{
    int i;

    int c = 0; // начальное значение счётчика записей

    if (num != ROWS * COLS)
    {   
        printf("<func::WriteMatrixToFile>\n");
        // Ошибка: неверный размер матрицы
        printf("Error: wrong matrix size\n");
        // Получено num =
        printf("got num = %d\n", num);
        // Ожидалось
        printf("expected ROWS * COLS = %d \n", ROWS * COLS);
        _getch();
        exit(3);
    }

    // Сначала записываем в файл информацию о матрице

    // Число элементов матрицы
    c += fwrite(&num, sizeof(int), 1, fPtr);
    // размерность
    c += fwrite(&ROWS, sizeof(int), 1, fPtr);     // Число строк
    c += fwrite(&COLS, sizeof(int), 1, fPtr);     // Число столбцов
 
    if (c != 3)
    {   
        printf("<func::WriteMatrixToFile>\n");
        //Не смогли записать информацию о матрице
        printf("We can not write information about the matrix\n");
        _getch();
        exit(2);
    }
 
    c = 0;
    for( i = 0; i < num; i++)
        c += fwrite((ar + i), sizeof(t_item), 1, fPtr);
 
    if (c != num )
    {   
        printf("<func::WriteMatrixToFile>\n");
        //Записались НЕ ВСЕ элементы матрицы
        printf("Not all element of the matrix are enrolled.\n");
        _getch();
        exit(4);
    }
}


/*заполняем "двумерный" массив случайными числами*/
// Причом n - числе ВСЕХ элементом "матрицы"
void SetAr(t_item* ar,  int n)
{
    int q;
 
    for(q = 0; q < n; q++)
    {
        ar[q] = rand() % 10;
    }
 
}


/*Выводим "матрицу" на экран*/
void PrintAr(t_item* ar, int ROWS, int COLS)
{
    int i, j;
 
    for( i = 0; i < ROWS; i++)
    {
        for( j = 0; j < COLS; j++)
            // если t_item = int
            // printf("%5d",*(ar + i*COLS+ j));
            // если t_item = float ил double
            printf("%6.2f",*(ar + i*COLS+ j));
        printf("\n");
    }
}


/* "умное" закрытие файла */
// Выводит информацию при ошибке
 void SmartCloseFile(FILE *fPtr)
{
   if  (fclose(fPtr) == EOF)
    {
      printf("Error closing file\n");//Ошибка при закрытии файла
      _getch();
      exit(6);
    }
}


// Находим размер файла в байтах
int SizeFile(FILE *fPtr)
{
    int size_file;

    fseek(fPtr,0L,SEEK_END);  //перейти в конец файла
    size_file = ftell(fPtr);  //ф-ия ftell() возвращает текущую позицию в файле
    fseek(fPtr, 0L, SEEK_SET); // Возвращаемся в начало файла
    return size_file;
}