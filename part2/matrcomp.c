#include <stdlib.h>
#include "matrcomp.h"
#include "mytype.h"

// НЕ ЗАБЫВАЕМ что в данном случае ВО ВСЕХ функциях
// 'матрица' - это ОДНОМЕРНЫЙ массив

/*Сложение двух матриц*/
void sum(t_item* A, t_item* B, int rowC, int colC, t_item* C)
{
    int i,j;

    
    for (i = 0; i < rowC; ++i)
    {
        for (j = 0; j < colC; ++j)
        {
            *(C + i*colC + j) = *(A + i*colC + j) + *(B + i*colC + j);
        }
    }

}

/*Умножение двух матриц*/
// A*B записывается в матрицу С
void mult (t_item* A, t_item* B, int rowA, int colB, int cArB, t_item* C)
{
  int i,j,k;
  t_item* A_i_k;
  t_item* B_k_j;
  t_item* C_i_j;

  // обходим все элменты матрицы C
  // по правилу умножения матриц
  // число строк в С = числу строк в A
  // число столбцов в C = числу столбцов в B

  for (i = 0; i < rowA; i++)
    for (j = 0; j < colB; j++)
    {
        // умножаем строку матрицы A на столбце матрицы B
        // получаем адрес элемента матрицы "C" расположенный в позиции [i,j]
        C_i_j = (C + colB*i+j);
        *C_i_j = 0;
        // C[i][j] = 0;
        for (k = 0; k < cArB; k++)
        {
            A_i_k = (A + cArB*i+k);
            B_k_j = (B + colB*k+j);
            // C[i][j] += A[i][k] * B [k][j];
            *C_i_j += (*A_i_k) * (*B_k_j);
        }

    }

}

/*Минор матрицы стоящей на пересечении строки 'r' и столбца 'c'*/
// Т.е. вычЁркиваем стоку 'r' и столбец 'c'
t_item minor_det(t_item* m, int ROWS, int COLS, int r, int c)
{   
    int i, j;
    t_item res;
    // По условию понятно, что матрица одного элемента никогда не попадёт в эту функцию
    // поэтому здесь не надо делать проверок на количество строк и столбцов

    // По рекурсии решаем через миноры определители(определители миноров и тд.)


    // Создаём Минорную матрицу
    t_item* matrix_minors = NULL;
    // Число ВСЕХ элементов матрицы миноров
    // Чтобы легче было записывать в эту матрицу элементы
    int count_mm;

    // Выделяем память под матрицу миноров
    matrix_minors = (t_item*)calloc((ROWS-1)*(COLS-1), sizeof(t_item));

    // заполняем её элементами главной метрицы,
    // не включая (вычёркивая) строку 'r' и столбуц 'c'
    count_mm = 0;
    for (i = 0; i < ROWS; ++i)
    {   
        if (i == r) continue; // строка 'r' - нам не нужна

        for (j = 0; j < COLS; ++j)
        {
            if (j == c) continue; // столбец 'c' - тоже не нужен

            *(matrix_minors + count_mm) = *(m + i*COLS + j);
            count_mm++;
        }
    }

    // после получения минорной матрицы: находим её определитель
    // это и есть Минор - результат функции
    res = det(matrix_minors, ROWS-1, COLS-1);

    // не забываем осободить память
    // выделенную под вспомогательную минорную матрицу
    free(matrix_minors);

    // важно это сделать ПОСЛЕ вычесления определителя
    // и ДО возвращения результата

    return res;
}



/*Собственная функция возведения в степень*/
// так как были траблы по приведению типов в вычислении алгебраического дополнения
int my_pow(int num, int step)
{
    int res = 1;
    int i;
    for (i = 0; i < step; ++i)
    {
        res *= num;
    }

    return res;
}

/*Определиетль матрицы*/
t_item det(t_item* m, int ROWS, int COLS)
{   
    t_item res; // Результат работы функции
    int j;
    t_item A; // Алгебраическое дополнение
    // определитель матрицы одного элемента - сам этот элемент
    if (ROWS == 1) return m[0];

    // Вычисляем определитель, раскладывая его по первой строке (i=0)
    // det(m) = m[0][0]*A_0_0 + m[0][1]*A_0_1 + ... + m[0][N]*A_0_N
    res = 0; // det(m)
    for(j=0; j < COLS; j++)
    {   
        // Вычисляем Алгебраическое дополнение для каждого элемента строки
        // по формуле
        // A_i_j = (-1)^(i+j) * minor_i_j
        A = my_pow(-1, 0+j) * minor_det(m, ROWS, COLS, 0, j);
        // *(m+j) - j-ый элемент i=0-ой строки матрицы
        res += (*(m+j))*A;
    }
    return res;

}

/*Вычисляем окаймляющий минор*/
// n- число строк и столбцов окаймляющего минора
// По сути ранг матрицы
t_item bounding_minor(t_item* m, int ROWS, int COLS, int r, int c, int n)
{   
    t_item res; // результат работы функции
    int i,j;
    // Создаём Минорную матрицу
    t_item* matrix_minors = NULL;
    // Число ВСЕХ элементов матрицы окаймляющего минора
    // Чтобы легче было записывать в эту матрицу элементы
    int count_bm;

    // Выделяем память под матрицу минора
    // Она квадратная
    matrix_minors = (t_item*)calloc(n*n, sizeof(t_item));

    // Заполняем её элементами так:
    // со строки "r" и столбца "c" сроим квадратик
    // со стороной "n"
    count_bm = 0;
    for (i = r; i < r+n; ++i)
    {   
        for (j = c; j < c+n; ++j)
        {
            *(matrix_minors + count_bm) = *(m + i*COLS + j);
            count_bm++;
        }
    }

    // Берём от неё определитель - это и называется ОКАЙМЛЯЮЩИЙ МИНОР
    res = det(matrix_minors, n, n);

    // не забываем осободить память
    // выделенную под вспомогательную минорную матрицу
    free(matrix_minors);

    // важно это сделать ПОСЛЕ вычесления определителя
    // и ДО возвращения результата

    return res;
}

/*Вычисляем ранг матрицы*/
// Методом окаймляющих миноров
int rank(t_item* m, int ROWS, int COLS)
{
    int k,i,j;
    int minor_is_NULL = 1; // флаг события

    // Минор - определитель от квадратной матрицы
    // Порядок максимального минора матрицы(число строк и столбцов)
    // Должен быть равен одной из сторон матрицы
    // Представьте как вы вписываете квадратик в прямоуголник
    // Квадрат можно вписать только когда его сторона равно меньшей стороне прямоугльника
    // если окаймляющий минор не равен 0(определитель квадратика)
    // То его порядок равен рангу матрицы
    // Следовательно максимальный ранг матрицы МОЖЕТ(т.е. не всегда)
    // может быть равен ПОРЯДКУ самого большого квадратика вписанного в прямоугольик
    // А это как мы знаем - меньшая сторона прямоугольника
    int max_rank = (ROWS <= COLS) ? ROWS : COLS;

    // Перебираем все возможно квадратики со стороной = k
    for(k=1; k <= max_rank; k++)
    {
        for(i = 0; i <= ROWS-k; i++)
        {
            minor_is_NULL = 1;
            for(j = 0; j <= COLS-k; j++)
            {
                if (bounding_minor(m, ROWS, COLS, i, j, k)!=0)
                {
                    // Если окаймляющий минор - НЕ равен нулю
                    // То не имеет смысла перебирать остальные Квадратики
                    // Поэтому выходим из цикла и УВЕЛИЧИВАЕМ СТОРОНУ КВАДРАТА
                    // т.е. возвращаемся в цикл где перебираем стороны
                    // for(k=1; k<=max_rank; k++) и тд
                    // Но проблема в том что "breek" завершает только внутренний цикл
                    // Где перебираем столбцы
                    // Но сверху есть ещё один цикл,где перебираем строки
                    // И из него надо тоже выйти
                    // Для этого "сообщаем ему" - циклу строк
                    // Что работать дальше не надо через логичскую переменную
                    // Которую он проверит и завершит работу
                    minor_is_NULL = 0;
                    break;
                }
            }

            // В цикле строк проверяем а нужно ли дальше работать
            // Если во внутреннем цикле нашили минор, который НЕ равен нулю
            // То и другие миноры проверять нам не надо
            if (!minor_is_NULL)
            {   
                // Поправка если мы нашли уже такой минор
                // - квадратик с максимальной стороной
                // То и другие такие миноры проверять не надо
                // А просто возвращяем длину стороны
                // И выходим из функции
                if (k == max_rank)
                {
                    return k;
                }
                // Если минор не равен нулю, но он ещё меньше максимальной
                // возвращаемся в цикл с беребором сторон квадратиков
                // for(k=1; k<=max_rank; k++)
                // Потому что вдруг минор с большей сторой окажется тоже не нулевым
                else
                {
                    break;
                }
            }
        }

        // Если мы прошли все циклы и оказались здесь
        // Без break
        // А значит minor_is_NULL = true
        // То все миноры со сторой k - оказались нулевыми
        // Значит не имеет смысла искать миноры с большей сторой
        // Значит ранг матрицы меньше длины квадратика минора на еденицу
        // То есть мы нашли ответ
        // Поэтому просто возвращаем результат и выходим из функции
        if (minor_is_NULL)
        {   
            return k-1;
        }
    }

    return k;
}