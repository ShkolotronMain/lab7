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

int realloc_res(int* available, int* need, int* allocated, int* requested)
{
    // Вместо аварийного завершения при избыточном запросе ресурсы вообще не выделятся
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
            return 1;
        }
    }
    return 0;
}

void check_status(int* available, int** need, int** allocated, int** max)
{
    // Копируем доступные ресурсы в массив рабочих ресурсов
    int work[RESOURCE_COUNT];
    for (int i=0; i<RESOURCE_COUNT; i++)
        work[i] = available[i];

    // Обозначаем завершённые процессы
    int finish[PROCESS_COUNT] = {0};

    // Бегаем по процессам
    int index = 0;
    while (index > -1)
    {
        if (!finish[index])
        {
            // Выводим состояние
            printf("Текущее состояние системы:\n");
            printf("\nДоступные ресурсы: \n");
            print_vec(work, RESOURCE_COUNT);
            printf("\nНеобходимые в работе ресурсы:\n");
            print_matrix(need, PROCESS_COUNT, RESOURCE_COUNT);
            printf("\nВыделенные заранее ресурсы: \n");
            print_matrix(allocated, PROCESS_COUNT, RESOURCE_COUNT);

            // Запрашиваем ресурсы
            int req[RESOURCE_COUNT];
            for (int k=0; k<RESOURCE_COUNT; k++)
                req[k] = need[index][k];
            // printf("Введите вектор запрашиваемых для процесса %d ресурсов: ", index);
            // for (int i=0; i<RESOURCE_COUNT; i++)
            // {
            //     int buf = 0;
            //     scanf_s("%d", &buf);
            //     req[i] = buf;
            // }
            
            // Выделяем ресурсы
            printf("Выделение процессу %d ресурсов: ", index);
            print_vec(req, RESOURCE_COUNT);
            int reallocated = realloc_res(work, need[index], allocated[index], req);
            
            // Чекаем
            if (leq(max[index], allocated[index], RESOURCE_COUNT))
            {
                for (int j=0; j<RESOURCE_COUNT; j++)
                {
                    work[j] += allocated[index][j];
                    allocated[index][j] = 0;
                    need[index][j] = 0;
                }

                //Завершаем процесс
                finish[index] = 1;
                printf("Процесс %d завершён\n", index);

                if (sum(finish, PROCESS_COUNT) == PROCESS_COUNT)
                {
                    index = -2;
                    fputs("Состояние системы и последовательность безопасны", stdout);
                }
            }
            else
            {
                fputs("Такое состояние системы небезопасно, откат\n", stdout);
                
                if (reallocated)
                    for(int i=0; i<RESOURCE_COUNT; i++)
                    {
                        work[i] += req[i];
                        allocated[index][i] -= req[i];
                        need[index][i] += req[i];
                    }
            }

            index = (index+1)%PROCESS_COUNT;
        }
    }
    

    if (index != -1)
        printf("Исходное состояние небезопасно\n");
}

int main()
{
    // Доступные ресурсы
    int* available = (int*)calloc(RESOURCE_COUNT, sizeof(int));

    // Необходимые для завершения ресурсы
    int** max = (int**)calloc(PROCESS_COUNT, sizeof(int*));
    for (int i=0; i<PROCESS_COUNT; i++)
        max[i] = (int*)calloc(RESOURCE_COUNT, sizeof(int));

    // Запрашиваемые ресурсы
    int** need = (int**)calloc(PROCESS_COUNT, sizeof(int*));
    for (int i=0; i<PROCESS_COUNT; i++)
        need[i] = (int*)calloc(RESOURCE_COUNT, sizeof(int));

    // Выделенные ресурсы
    int** allocated = (int**)calloc(PROCESS_COUNT, sizeof(int*));
    for (int i=0; i<PROCESS_COUNT; i++)
        allocated[i] = (int*)calloc(RESOURCE_COUNT, sizeof(int));

    /// СЕКЦИЯ ТЕСТОВЫХ ДАННЫХ

    available[0] = 2;
    available[1] = 5;
    available[2] = 7;
    available[3] = 3;

    int tmax[PROCESS_COUNT][RESOURCE_COUNT] = 
    {
        {3, 1, 4, 5}, 
        {1, 5, 3, 2}, 
        {2, 4, 6, 1}, 
        {0, 0, 7, 0}, 
        {1, 7, 3, 3}
    };

    int tneed[PROCESS_COUNT][RESOURCE_COUNT] = 
    {
        {3, 0, 4, 1}, 
        {0, 2, 2, 1}, 
        {2, 3, 6, 0}, 
        {0, 0, 6, 0}, 
        {1, 6, 0, 1}
    };

    int tallocated[PROCESS_COUNT][RESOURCE_COUNT] = 
    {
        {0, 1, 0, 4}, 
        {1, 3, 1, 1}, 
        {0, 1, 0, 1}, 
        {0, 0, 1, 0}, 
        {0, 1, 3, 2}
    };

    /// СЕКЦИЯ ТЕСТОВЫХ ДАННЫХ

    // Копируем тестовые данные
    for (int i=0; i<PROCESS_COUNT; i++)
        for (int j=0; j<RESOURCE_COUNT; j++)
        {
            max[i][j] = tmax[i][j];
            need[i][j] = tneed[i][j];
            allocated[i][j] = tallocated[i][j];
        }

    check_status(available, need, allocated, max);

    // Чистим память
    free(available);
    free_matrix(max, PROCESS_COUNT);
    free_matrix(allocated, PROCESS_COUNT);
    free_matrix(need, PROCESS_COUNT);

    return 0;
}