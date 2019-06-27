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
void ReadMatrixFromFile(FILE *fPtr, int matr_pos, t_item** ar, int *num, int *ROWS, int *COLS)

{
    int  i, count = 0;

    // matr_pos - позиция матрицы в файле
    // то есть первая матрица, вторая или третья
    // позиция матрицы не может быть меньше 1.
    // как минимум в файле ОДНА матрица
    if (matr_pos < 1)
    {
        printf("<func::ReadMatrixFromFile>\n");
        // Ошибка: номер матрицы в файлеле меньше 1
        printf("Parametr matr_pos=%d < 1\n", matr_pos);
        _getch();
        exit(1);
    }

    // Просто переходим к нужной нам позиции матрицы в файле
    GoToPosition(fPtr, matr_pos);

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

/*Получение имени файла от пользователя и его открытие*/
void Opening_file(FILE **fPtr, const char *mode)
// Диалоговый режим
// Считываем имя файла от пользователя до тех пор
// Пока он не сможем открыть его
// строка - имя файла
// для fopen_s нужен указаетль на указатель файла FILE **fPtr
{  
    char f_name[30];
    errno_t err;
    for(;;)
    {   
        printf("Enter file name of matrix: ");
        scanf_s("%s", f_name, 30);

  
        //пытаемся открыть файл на чтение
        err = fopen_s(fPtr, f_name, mode);
        if (err)
        {
            printf("Cannot open file: \"%s\"\n", f_name);
            printf("Try again\n");
        }
        else
            break;
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

/*Проверяет файл на корректность*/
// на соответсвие протоколу взаимодействия
// возвращает КОЛИЧЕСТВО матриц в файле
// Если файл НЕ корректный то вернёт 0
int IsCorrectFile(FILE* fPtr)
{
    // число матриц в файле
    int count_matrix;
    // число прочтённых элементов - чисел cur_num
    int count;
    // количество элментов текущей матрицы
    int cur_num;
    // рамзер ТЕКУЩЕЙ матрицы - сколько байт для хранения в файле
    int size_cur_matrix;
    // размер ПРОЧТЁННЫХ матриц
    int size_read_matixs = 0;
    // ПОЛНЫЙ-СУММАРНЫЙ размер файла
    int full_size_file = SizeFile(fPtr);

    // Сначала число матриц и число прочтённых элементов
    // Естественно равно нулю
    count_matrix = count = 0;
    // обходим все матрицы в файле
    while (size_read_matixs < full_size_file)
    {   
        // для простоты вместо того, чтобы считывать всю матрицу
        // мы будем считывать только кол-во элементов матрицы
        // cur_num = ROWS*COLS - число строк умножит на кол-во столбцов
        count += fread(&cur_num, sizeof(int), 1, fPtr);
        // число не прочиталось
        // после прочтения count должен быть больше count_matrix на 1
        if (count == count_matrix)
        {   
            // значит файл не корректный
            return 0;
        }
        // текущее значение count - и есть число матриц в файле
        count_matrix = count;

        // получаем размер под ОДНУ матрицу в байтах
        /*
        размер под ОДНУ матрицу состоит из:
        1) 3-ёх целых чисел 
            cur_num - кол-во элементов матрицы
            ROWS - число строк
            COLS - число стобцов
        2) самих элементов матрицы - их кол-во = cur_num
        */
        size_cur_matrix = 3*sizeof(int) + cur_num*sizeof(t_item);

        // Прибавляем к размерам уже прочитанных матриц
        size_read_matixs += size_cur_matrix;

        // Пытаемся перейти к следующей матрице
        // от начала файла(SEEK_SET) откладываем size_read_matixs байт
        // то есть мы просуммировали предыдущие байты,
        // в которые входят уже известные - КОРРЕКТНЫЕ матрицы
        // и смотрим что там дальше
        if (fseek(fPtr, size_read_matixs, SEEK_SET) != 0)
        {   
            // если fseek - возращает НЕ нулевое значение
            // то файл не корректный
            return 0;
        }
        
    }
    fseek(fPtr, 0L, SEEK_SET); // Возвращаемся в начало файла
    return count_matrix;
}

/*Переходим к матрице которая стоит под номером matr_pos*/
void GoToPosition(FILE* fPtr, int matr_pos)
{
    // число матриц в файле до нужной позиции
    int count_matrix_before;
    // число прочтённых элементов - чисел cur_num
    int count;
    // количество элментов текущей матрицы
    int cur_num;
    // рамзер ТЕКУЩЕЙ матрицы - сколько байт для хранения в файле
    int size_cur_matrix;
    // размер ПРОЧТЁННЫХ матриц
    int size_read_matixs = 0;

    // Сначала число матриц и число прочтённых элементов
    // Естественно равно нулю
    count_matrix_before = count = 0;
    // обходим матрицы в файле до нужной нам
    while (count_matrix_before + 1 < matr_pos)
    {   
        // для простоты вместо того, чтобы считывать всю матрицу
        // мы будем считывать только кол-во элементов матрицы
        // cur_num = ROWS*COLS - число строк умножит на кол-во столбцов
        count += fread(&cur_num, sizeof(int), 1, fPtr);
        // текущее значение count - и есть число ПРОЧИТАННЫХ матриц в файле(ДО)
        count_matrix_before = count;

        // получаем размер под ОДНУ матрицу в байтах
        /*
        размер под ОДНУ матрицу состоит из:
        1) 3-ёх целых чисел 
            cur_num - кол-во элементов матрицы
            ROWS - число строк
            COLS - число стобцов
        2) самих элементов матрицы - их кол-во = cur_num
        */
        size_cur_matrix = 3*sizeof(int) + cur_num*sizeof(t_item);

        // Прибавляем к размерам уже прочитанных матриц
        size_read_matixs += size_cur_matrix;

        // Пытаемся перейти к следующей матрице
        // от начала файла(SEEK_SET) откладываем size_read_matixs байт
        // то есть мы просуммировали предыдущие байты,
        // в которые входят уже известные - КОРРЕКТНЫЕ матрицы
        // и смотрим что там дальше
        fseek(fPtr, size_read_matixs, SEEK_SET);
    }
}

/*Размер файла в байтах*/
int SizeFile(FILE *fPtr)
{
    int size_file;

    fseek(fPtr,0L,SEEK_END);  //перейти в конец файла
    size_file = ftell(fPtr);  //ф-ия ftell() возвращает текущую позицию в файле
    fseek(fPtr, 0L, SEEK_SET); // Возвращаемся в начало файла
    return size_file;
}