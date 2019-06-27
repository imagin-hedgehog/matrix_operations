/* 
Макрос 'MATRIO_H' указывает на то,что
файл уже однажды включался
В подсключаемых файлах пользователя
макрос не должен начинатьяс с симновола '_'
В системных подключаемых файлах его имя не должно
начинаться с симнвола '_' во избежание возникновения
конфликтов с программами пользователя
Каким бы ни был файл, имя макроса должно содержать
имя файла и некоторый дополнительый текст
во избежание возникновения конфликтов
с другими подкдючаемыми файлами

Тое есть всё что внутри конструкции 
#ifndef
  ...
#endif
выполняется лишь один раз
даже если данный заголовочный файл подключался несколько раз
*/

#ifndef MATRIO_H
#define MATRIO_H
#include <stdio.h>  // нужен чтобы не выдавлао ошибку при "FILE *"
#include "mytype.h" // нужно для типа матриц

// Наши прототипы функций
void SetAr(t_item* ar,  int n);
void PrintAr(t_item* ar, int ROWS, int COLS);
int SizeFile(FILE *fPtr);
void GoToPosition(FILE* fPtr, int matr_pos);
int IsCorrectFile(FILE* fPtr);
void OpenMatrixFile(FILE** fPtr, const char *path, const char *mode);
void Opening_file(FILE** fPtr, const char *mode);
void ReadMatrixFromFile(FILE *fPtr, int matr_pos, t_item** ar, int *num, int *ROWS, int *COLS);
void WriteMatrixToFile(FILE *fPtr, t_item* ar, int num, int ROWS, int COLS);
void SmartCloseFile(FILE *fPtr);

#endif