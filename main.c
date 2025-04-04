#include <stdlib.h>

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

int check_safety(int* aval, int** need, int** alloc)
{
    int work[RESOURCE_COUNT];
    for (int i=0; i<RESOURCE_COUNT; i++)
        work[i] = aval[i];

    int finish[PROCESS_COUNT] = {0};
    
    for (int i=0; i<PROCESS_COUNT; i++)
    {
        if (!(finish[i]) && leq(need[i], work, RESOURCE_COUNT))
        {
            for (int j=0; j<RESOURCE_COUNT; j++)
                work[j] += alloc[i][j];
            finish[i] = 1;
        }
    }

    return sum(finish, PROCESS_COUNT)==PROCESS_COUNT;
}

int main()
{
    // Доступные ресурсы
    int available[RESOURCE_COUNT];

    // Запрашиваемые ресурсы
    int need[PROCESS_COUNT][RESOURCE_COUNT];

    // Выделенные ресурсы
    int allocated[PROCESS_COUNT][RESOURCE_COUNT];

    available[0] = 2;
    available[1] = 5;
    available[2] = 7;
    available[3] = 3;

    
}