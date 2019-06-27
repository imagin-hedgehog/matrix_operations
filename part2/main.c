#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <errno.h>
#include "matrio.h"
#include "matrcomp.h"
#include "mytype.h"

// прототипы функций
int menu_select();
void LoadOne(t_item** m, int* num, int* ROWS, int* COLS);
void ViewFile();
void multMatrix();
void sumMatrix();
void rankMatrix();
void determinant();

// Делаем эти переменные глобальными, чтобы они были видны в других функциях
//  строки,столбцы,кол-во элементов матриц
int ROWS1, COLS1, num1, ROWS2, COLS2, num2, ROWS, COLS, num;
// сами матрицы
t_item* matr1=NULL;
t_item* matr2=NULL;
t_item* m=NULL;

int main()
{
        /*Интерактивное меню*/

    int choice; // хранит выбранный пункт меню

    for(;;)
    {
        choice = menu_select();
        switch(choice)
        {
            case 1:
                // Поочерёдно загружаем матрицу 1 и матрицу 2
                printf("[Matr1]\n");
                LoadOne(&matr1, &num1, &ROWS1, &COLS1);
                printf("[Matr2]\n");
                LoadOne(&matr2, &num2, &ROWS2, &COLS2);
                break;

            case 2:
                // Загружаем только одну матрицу
                printf("[Matr]\n");
                LoadOne(&m, &num, &ROWS, &COLS);
                break;
            case 3:
                // Посмотреть только один файл
                ViewFile();
                break;
            case 4:
                // Умножить матрицы
                multMatrix();
                break;
            case 5:
                // Сложить матрицы
                sumMatrix();
                break;
            case 6:
                // Определитель матрицы
                determinant();
                break;
            case 7:
                rankMatrix();
                break;
            case 8:
                free(m);
                free(matr1);
                free(matr2);
                exit(0);  // выход
        }
    }

    return 0;
}

/*Интерактивное меню*/
// возвращает номер выбранного пункта
int menu_select()
{
    int c; // ответ от пользователя

    // Выводим само меню
    printf("\n\n");
    printf("\t1 - Load TWO matrix\n");
    printf("\t2 - Load ONE matrix\n");
    printf("\t3 - View one file\n");
    printf("\t4 - Multiply TWO matrix\n");
    printf("\t5 - Sum TWO matrix\n");
    printf("\t6 - Culculate determinant\n");
    printf("\t7 - Culculate rank of matrix\n");
    printf("\t8 - Exit\n");

    // получаем выбор пункта меню до тех пор
    // пользователь вводит некоректный ответ
    do
    {
        printf("\nEnter number: ");
        scanf_s("%d", &c, 1);
    } while(c <1 || c>8);

    return c;
}

/*Загрузить ОДНУ матрицу из файла на диске*/
// записываться всё будет в переданные аргументы
// Которые будут ГЛОБАЛЬНЫМИ переменными
// вызов LoadOne(&matr1, &num1, &ROWS1, &COLS1);
void LoadOne(t_item** C, int* numC, int* rowsC, int* colsC)
{
    FILE* fPtr;
    // количество матриц в файле
    int count_matrix ;
    // ответ пользователя
    // номер выбранной им матрицы
    int matr_pos;

    // все действия после объявления переменных
    // Перед загрузкой матрицы нужно освбодить память
    // Конечно если матрица сама не пустая
    if (*C != NULL)
    {
        free(*C);
        *C = NULL;
    }

    // Диалог ввода имени файла
    Opening_file(&fPtr, "rb");

    // Проверяем файл на корректность
    count_matrix = IsCorrectFile(fPtr);
    // Число матриц в файле 0 значит файл повреждён
    if (count_matrix == 0)
    {
        printf("Bad file\n");
        printf("Press Enter to return in menu\n");
        _getch();
        return;
    }

    // В файле может быть больше одной матрицы
    // Но мы можем загрузить только одну из них
    // если матрица всё же одна, то просто её и загружаем
    matr_pos = 1;
    // если больше одной матрицы в файле то
    if (count_matrix > 1)
    {   
        // спрашиваем у пользователя какую матрицу загрузить
        // первую вторую или тд
        printf("File has a %d matrices\n", count_matrix);
        do
        {
            printf("\nEnter the number you need(1/%d): ", count_matrix);
            scanf_s("%d", &matr_pos, 1);
        } while(matr_pos < 1 || matr_pos > count_matrix);
    }
    // Записываем матрицу из файла в ПЕРЕДАННЫЕ аргументы
    // ИЗМЕНЯТСЯ ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ
    ReadMatrixFromFile(fPtr, matr_pos, C, numC, rowsC, colsC);
    // Закртыие с перехватом ошибок
    SmartCloseFile(fPtr);
    // Выводим матрицу на экран
    PrintAr(*C, *rowsC, *colsC);

    // Здесь важно отметить что
    // НЕ НУЖНО особождать память по типу free(*C)
    // это происходит в функции main
    // при выходе из программы
    // case: 8 ...

    _getch();
}

/*Посмотреть один файл на диске*/
// здесь НЕ используются глобальные переменные
// служит для проверки изенений на диске
// правильно ли всё записалось
void ViewFile()
{
    // здесь память освобождать не надо
    // т.к. C и так равна NULL
    // То есть под локальную переменную память не выделялась ещё
    t_item *C=NULL;
    int numC, rowsC, colsC;
    FILE* fPtr;
    int count_matrix;
    int matr_pos;

    // Диалог ввода имени файла
    Opening_file(&fPtr, "rb");

    // Проверяем файл на корректность
    count_matrix = IsCorrectFile(fPtr);
    // Число матриц в файле 0 значит файл повреждён
    if (count_matrix == 0)
    {
        printf("Bad file\n");
        printf("Press Enter to return in menu\n");
        _getch();
        return;
    }
    
    // В файле может быть больше одной матрицы
    // Но мы можем загрузить только одну из них
    // если матрица всё же одна, то просто её и загружаем
    matr_pos = 1;
    // если больше одной матрицы в файле то
    if (count_matrix > 1)
    {   
        // спрашиваем у пользователя какую матрицу загрузить
        // первую вторую или тд
        printf("File has a %d matrices\n", count_matrix);
        do
        {
            printf("\nEnter the number you need(1/%d): ", count_matrix);
            scanf_s("%d", &matr_pos, 1);
        } while(matr_pos < 1 || matr_pos > count_matrix);
    }

    // Записываем матрицу из файла в ЛОКАЛЬНЫЕ переменные
    ReadMatrixFromFile(fPtr, matr_pos, &C, &numC, &rowsC, &colsC);
    // Закрываем файл с перехватом ошибок
    SmartCloseFile(fPtr);
    // Выводим мтрицу
    PrintAr(C, rowsC, colsC);

    // Не забываем осовбодить память, так как после
    // завершения работы функции
    // C - уничтожится
    // Но выделенная память останется
    free(C);
    _getch();
}

/*Произведение матриц*/
void multMatrix()
{   
    // matr1, ROWS1, COLS1    глобальные переменные
    // matr2, ROWS2, COLS2

    t_item* C;
    int rowsC, colsC;
    int cArB;
    FILE* fPtr;

    // под матрицы не выделена память, значит их не загрузили
    if (matr1 == NULL || matr2 == NULL)
    {
        printf("Matr1 or Matr2 is not loaded\n");
        printf("Go back to the menu and download them(select 1)");
        _getch();

        return;
    }

    // Поочерёдно выводим матрицы на экран
    printf("\n[Matr1]\n");
    PrintAr(matr1, ROWS1, COLS1);
    printf("\n[Matr2]\n");
    PrintAr(matr2, ROWS2, COLS2);

    // из определения умножеия матриц
    // число строк матрицы перемножения
    rowsC = ROWS1;
    // число столбцов матрицы перемножения
    colsC = COLS2;

    // Проверка при умножении матриц
    // если число столбцов первой матрицы не равно числу строк второй матрицы
    // то такие матрицы нельзя умножать
    if (COLS1 != ROWS2)
    {
        // Такие матрицы нельзя перемножать
        printf("Such matrices cannot be multiplied\n");
        printf("COLS1=%d is not ROWS2=%d\n", COLS1, ROWS2);
        printf("\nPress Enter to return in menu");

        _getch();
        return; // возвращаеся в меню

    }

    // кол-во столбцов первой матрцы
    // и ОДНОВРЕМЕННО число строк второй матрицы
    cArB = COLS1; // или ROWS2

    // перед тем как передать рузультирующую матрицу в функцию умножения
    // нужно выделить под неё память
    C = (t_item*)calloc(rowsC*colsC, sizeof(t_item));

    // вычисленное произведение записываем в C
    mult(matr1, matr2, rowsC, colsC, cArB, C);

    // выводим матрицу произведения
    printf("\nMultiply matrix:\n");
    PrintAr(C, rowsC, colsC);

    // Сохраняем её в файл
    printf("Save her on the disk\n");
    // режим "ab" - говорит что мы записываем начиная с конца файла
    // если таковой имеется
    // и записываем с начала если такого нет
    Opening_file(&fPtr, "ab");
    WriteMatrixToFile(fPtr, C, rowsC*colsC, rowsC, colsC);
    SmartCloseFile(fPtr);
    printf("[OK] Press Enter to return in menu");

    // Т.к. С - локальная переменная
    // и ВНУТРИ функции под неё выделяется память
    // то и освобождать память надо ВНУТРИ этой функции 
    free(C);
    _getch();
}

/*Сумма матриц*/
void sumMatrix()
{   
    // matr1, ROWS1, COLS1    глобальные переменные
    // matr2, ROWS2, COLS2 

    t_item* C;
    int rowsC, colsC;
    FILE* fPtr;

    // под матрицы не выделена память, значит их не загрузили
    if (matr1 == NULL || matr2 == NULL)
    {
        printf("Matr1 or Matr2 is not loaded\n");
        printf("Go back to the menu and download them(select 1)");
        _getch();

        return;
    }

    //Поочерёдно выводим 1-ую и 2-ую матрицы
    printf("\n[Matr1]\n");
    PrintAr(matr1, ROWS1, COLS1);
    printf("\n[Matr2]\n");
    PrintAr(matr2, ROWS2, COLS2);

    // Предполагаем, что число строк и столбцов   24,11,18   пока что без проверок на корректность
    // у обоих мариц одинаковое количество по ровну

    // Прверку на число строк и столбцов можно сделать по разному
    /*
    1) можно переписать функцию суммирования матриц
       передавать в неё строки и столбцы и ВНУТРИ неё делать проверку
       тогда её ВЫЗОВ должен выглядеть примерно так

       sum(A, rowsA, colsA, B, rowsB, colsB, C, &rowsC, &colsC)

       &rowsC и &colsC нужны чтобы после работы функции
       задать число строк и столбцов результирующей матрицы

    2) Делать проверку вне функции, что легче и менее громоздко
       ЕГО И БУДЕМ ДЕЛАТЬ
    */

    // если матрицы суммироватать нельзя(их размерности не совпадают)
    // Причём важно || (или) а не && (и)
    // т.к. может быть что строки равны, а столбцы нет
    // если будет && то логическое выражение будет ложным
    // и ПРОВЕРКА НЕ БУДЕТ РАБОТАТЬ 
    if (ROWS1!=ROWS2 || COLS2!=COLS2)
    {   
        // матрицы нельзя суммировать так как не совпадают размерности
        printf("Matr1 and Matr2 cannot be summed, because the dimensions do not match\n");
        printf("\nPress Enter to return in menu");
        _getch();
        // причом не обязательно закрывать программу
        // можно просто выйти из функции здесь
        return;
        // и таким образом вернуться снова в меню
    }

    // Предполагаем что число строк 1-ой матрицы = числу строк 2-ой
    // Аналогично и столбцы

    // значит строки и стобцы результирующей матрицы(суммы) будут
    rowsC = ROWS1; // или ROWS2
    colsC = COLS2; // или COLS1

    // перед тем как записать сумму матриц в результирующую
    // нужно выделить под неё память
    C = (t_item*)calloc(rowsC*colsC, sizeof(t_item));
    // записываем результат в C
    sum(matr1, matr2, rowsC, colsC, C);

    // Выводи матрицу суммы
    printf("\nSum matrix:\n");
    PrintAr(C, rowsC, colsC);

    // Сохраняем её в файл
    printf("Save her on the disk\n");
    // режим "ab" - говорит что мы записываем начиная с конца файла
    // если таковой имеется
    // и записываем с начала если такого нет
    Opening_file(&fPtr, "ab");
    WriteMatrixToFile(fPtr, C, rowsC*colsC, rowsC, colsC);
    SmartCloseFile(fPtr);
    printf("[OK] Press Enter to return in menu");

    // Т.к. С - локальная переменная
    // и ВНУТРИ функции под неё выделяется память
    // то и освобождать память надо ВНУТРИ этой функции
    free(C);
    _getch();
}

/*Выводит определитель ЗАГРУЖЕННОЙ матрицы*/
void determinant()
{   
    // m, ROWS, COLS - глобальные переменные

    // под ЕДИНСТВЕННУЮ матрицу не выделена память, значит её не загрузили
    if (m==NULL)
    {
        printf("ONE matrix is not loaded\n");
        printf("Go back to the menu and download her(select 2)");
        _getch();

        return;
    }

    // выводим матрицу
    printf("The matrix\n");
    PrintAr(m, ROWS, COLS);
    // выводим её определитель
    // если t_item = int
    // printf("\nHer determinant = %d\n", det(m, ROWS, COLS));
    // если t_item = float
    printf("\nHer determinant = %6.2f\n", det(m, ROWS, COLS));

    _getch();
}

/*Выводит ранг ЗАГРУЖЕННОЙ матрицы*/
void rankMatrix()
{
    // m, ROWS, COLS - глобальные переменные

    // под ЕДИНСТВЕННУЮ матрицу не выделена память, значит её не загрузили
    if (m==NULL)
    {
        printf("ONE matrix is not loaded\n");
        printf("Go back to the menu and download her(select 2)");
        _getch();

        return;
    }

    // выводим матрицу
    printf("The matrix\n");
    PrintAr(m, ROWS, COLS);
    // выводим её ранг
    printf("\nHer rank = %d\n", rank(m, ROWS, COLS));
    _getch();
}
