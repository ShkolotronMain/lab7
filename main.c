#include <stdlib.h>
#include <stdio.h>

// 5 процессов
// 4 ресурса (2, 5 ,7, 3)

#define PROCESS_COUNT 5
#define RESOURCE_COUNT 4

int leq(int* a, int *b, int size)
{
    for (int i=0; i<size; i++)
        if (a[i] > b[i])
            return 0;
    return 1;
}

int sum(int* vec, int size)
{
    int res = 0;
    for (int i=0; i<size; i++)
        res += vec[i];
    return res;
}

void print_vec(int* vec, int size)
{
    fputs("[", stdout);
    for (int i=0; i<size; i++)
    {
        printf("%-3.d", vec[i]);
        if (i<size-1)
            fputs(", ", stdout);
    }
    fputs("]\n", stdout);
}

void print_matrix(int** mat, int lines, int cols)
{
    fputs("[\n", stdout);
    for (int i=0; i<lines; i++)
    {
        fputs(" ", stdout);
        print_vec(mat[i], cols);
    }
    fputs("]\n", stdout);
}

void free_matrix(int** mat, int lines)
{
    for (int i=0; i<lines; i++)
        free(mat[i]);
    free(mat);
}

int main()
{
    // Доступные ресурсы
    int* available = (int*)calloc(RESOURCE_COUNT, sizeof(int));
    // available[0] = 2;
    // available[1] = 5;
    // available[2] = 7;
    // available[3] = 3;

    // Запрашиваемые ресурсы
    int** need = (int**)calloc(PROCESS_COUNT, sizeof(int*));
    for (int i=0; i<PROCESS_COUNT; i++)
        need[i] = (int*)calloc(RESOURCE_COUNT, sizeof(int));

    // Выделенные ресурсы
    int** allocated = (int**)calloc(PROCESS_COUNT, sizeof(int*));
    for (int i=0; i<PROCESS_COUNT; i++)
        allocated[i] = (int*)calloc(RESOURCE_COUNT, sizeof(int));
    
    // Запрашиваемые ресурсы
    int** requested = (int**)calloc(PROCESS_COUNT, sizeof(int*));
    for (int i=0; i<PROCESS_COUNT; i++)
        requested[i] = (int*)calloc(RESOURCE_COUNT, sizeof(int));

    // Копируем доступные ресурсы в массив рабочих ресурсов
    int work[RESOURCE_COUNT];
    for (int i=0; i<RESOURCE_COUNT; i++)
        work[i] = available[i];

    // Обозначаем завершённые процессы
    int finish[PROCESS_COUNT] = {0};
    
    int running = 1;

    // Основной цикл алгоритма
    for (int k=0; k<PROCESS_COUNT && running; k++)
        for (int i=0; i<PROCESS_COUNT && running; i++)
        {
            if (!(finish[i]) && leq(need[i], work, RESOURCE_COUNT))
            {
                for (int j=0; j<RESOURCE_COUNT; j++)
                {
                    work[j] += allocated[i][j];
                    allocated[i][j] = 0;
                }
                finish[i] = 1;
                printf("Завершён процесс %d\n", i);

                printf("\nДоступные ресурсы: \n");
                print_vec(available, RESOURCE_COUNT);
                printf("\nНеобходимые в работе ресурсы:\n");
                print_matrix(need, PROCESS_COUNT, RESOURCE_COUNT);
                printf("\nВыделенные заранее ресурсы: \n");
                print_matrix(allocated, PROCESS_COUNT, RESOURCE_COUNT);                 
            }

            if (sum(finish, PROCESS_COUNT) < PROCESS_COUNT)
            {
                printf("Исходное состояние безопасно\n");
                running = 0;
            }
        }

    if (running)
    printf("Исходное состояние небезопасно\n");

    // Чистим память
    free(available);
    free_matrix(allocated, PROCESS_COUNT);
    free_matrix(need, PROCESS_COUNT);
    free_matrix(requested, PROCESS_COUNT);

    return 0;
}