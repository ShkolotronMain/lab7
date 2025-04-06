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

int eq(int* a, int*b, int size)
{
    for (int i=0; i<size; i++)
        if (a[i] != b[i])
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

void realloc_res(int* available, int* need, int* allocated, int* requested)
{
    // Вместо аварийного завершения при избыточном запросе выделяется не больше, чем нужно
    if (leq(requested, need, RESOURCE_COUNT))
    {
        if (leq(requested, available, RESOURCE_COUNT))
        {
            for(int i=0; i<RESOURCE_COUNT; i++)
            {
                available[i] -= requested[i];
                allocated[i] += requested[i];
                need[i] -= requested[i];
            }
        }
    }
    else
    {
        for(int i=0; i<RESOURCE_COUNT; i++)
        {
            available[i] -= need[i];
            allocated[i] += need[i];
            need[i] -= need[i];
        }
    }
}

void check_status(int* available, int** need, int** allocated)
{
    // Копируем доступные ресурсы в массив рабочих ресурсов
    int work[RESOURCE_COUNT];
    for (int i=0; i<RESOURCE_COUNT; i++)
        work[i] = available[i];

    // Обозначаем завершённые процессы
    int finish[PROCESS_COUNT] = {0};
    int prev[PROCESS_COUNT] = {0};

    // Бегаем по процессам
    int index = 0;
    while (index > -1)
    {
        if (!finish[index])
        {
            if (!eq(finish, prev, PROCESS_COUNT))
            {
                // Выводим состояние
                printf("Текущее состояние системы:\n");
                printf("\nДоступные ресурсы: \n");
                print_vec(available, RESOURCE_COUNT);
                printf("\nНеобходимые в работе ресурсы:\n");
                print_matrix(need, PROCESS_COUNT, RESOURCE_COUNT);
                printf("\nВыделенные заранее ресурсы: \n");
                print_matrix(allocated, PROCESS_COUNT, RESOURCE_COUNT);

                // Запрашиваем ресурсы
                int req[RESOURCE_COUNT];
                printf("Введите вектор запрашиваемых для процесса %d ресурсов: ", index);
                for (int i=0; i<RESOURCE_COUNT; i++)
                    scanf_s("%d", &req[index]);
                
                // Выделяем ресурсы
                realloc_res(&work, need[index], allocated[index], &req);
                for (int j=0; j<RESOURCE_COUNT; j++)
                {
                    work[j] += allocated[index][j];
                    allocated[index][j] = 0;
                }

                //Завершаем процесс
                finish[index] = 0;
                printf("Процесс %d завершён\n", index);

                // Сохраняем состояние
                for (int j=0; j<PROCESS_COUNT; j++)
                    prev[j] = finish[j];

                index = (index+1)%PROCESS_COUNT;
            }
            else
            {
                index = -1;
                fputs("Состояние небезопасно, работа программы окончена\n", stdout);
            }
        }
    }
    

    if (index != -1)
        printf("Исходное состояние небезопасно\n");
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

    check_status(available, need, allocated);

    // Чистим память
    free(available);
    free_matrix(allocated, PROCESS_COUNT);
    free_matrix(need, PROCESS_COUNT);

    return 0;
}