#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*
1- Ingresar por teclado
1.1.- El tamaño de la memoria real en unidad de MB.
1.2.- El tamaño de la memoria real destinada al SO en unidad de KB.
1.2.- El tamaño del frame o marco en unidad de KB.

2- Ingresar por teclado (este es un ciclo que se repite mientras el identificador del proceso sea distinto de 0)
2.1.- Identificador numérico del proceso
2.2.- Tamaño del proceso en unidad de KB
2.2.- Asignarle al proceso un set de 3 frames libres

3- Ingresar por teclado (este es un ciclo que se repite mientras el identificador del proceso sea distinto de 0)
3.1.- Identificador del proceso y mostrar su tabla de páginas

4- Mostrar por pantalla La tabla de frames

5- Ingresar por teclado (este es un ciclo que se repite mientras el identificador del proceso sea distinto de 0)
5.1.- Identificador de un proceso y suponiendo que el proceso genera la siguiente serie de acceso a las siguientes páginas: 1 2 3 4 2 5 3, por cada fallo de página muestre la tabla de páginas, aplique el algoritmo LRU local para la selección de la página víctima

6- Mostrar por pantalla en función de los datos ingresados
6.1.- La cantidad de bits necesarios para una dirección lógica
6.2.- La cantidad de bits necesarios para una dirección física

7- Ingresar por teclado (este es un ciclo que se repite mientras el identificador del proceso sea distinto de 0)
7.1.- Un Identificador de proceso y una dirección virtual relativa a 0, de 0 a N-1 siendo N el límite del proceso y mostrar su dirección lógica de paginación y su dirección física en binario y decimal.


plus -> pagina de tabla invertidas con processid, para kill process

Importante: Hay que tener en cuenta que la siguiente actividad propone una Simulación, esto quiere decir que todas las estructuras de datos que utilice su simulación están dentro del espacio de direcciones de su proceso (programa de simulación en ejecución).

 */

typedef struct
{
    int real_memory_mb;
    int memory_so_kb;
    int memory_frame_kb;
    int virtual_memory; // asumimos que la memoria virtual es 3 veces mas grande que la memoria fisica
} MemoryConfig;

typedef struct
{
    int id_frame;
    int present_absent_bit;
    int last_access_time;
} Page;

typedef struct
{
    int id;
    int size_kb;
    int frames[3];
    int num_pages;
    Page *table_of_pages;
} Process;

typedef struct
{
    int id;
    int occupied;
    int process_id;
} Frame;

int num_process = 0;
int current_time = 0;

void memory_menu(MemoryConfig *memoryConfig);
void add_process(int id, int size_kb, int max_pages, Process *process, Frame *frames, int max_frames);
int find_free_frame(Frame *frames, int max_frames);
void initialize_pages(Process *process, int max_pages);
void access_page(Process *process, int num_page, Frame *frames, int max_frames);
void show_table_pages(Process *process);
Process *find_process_by_id(Process *processes, int num_processes, int id);
void line();

int main()
{
    int max_frames, max_process;

    MemoryConfig memoryConfig;
    memory_menu(&memoryConfig);
    memoryConfig.virtual_memory = memoryConfig.real_memory_mb * 3;

    line();

    // asigno la cantidad de frames disponibles en primera instancia
    max_frames = ((memoryConfig.real_memory_mb * 1024) - memoryConfig.memory_so_kb) / memoryConfig.memory_frame_kb;
    // creo la estructura de frames e inicializo cada uno con un id y ocupado = falso
    Frame frames[max_frames];

    for (int i = 0; i < max_frames; i++)
    {
        frames[i].id = i;
        frames[i].occupied = 0;
    }

    Process process[max_frames / 3];

    // ciclo para almacenar los procesos
    int id_process, size_process, max_pages;

    // PUNTO 2
    printf("PUNTO 2\n");
    printf("2.1 - Ingrese el id numerico del proceso: ");
    scanf("%d", &id_process);

    while (id_process != 0)
    {
        printf("2.2 - Ingrese el tamano en kb del proceso: ");
        scanf("%d", &size_process);
        max_pages = (size_process + memoryConfig.memory_frame_kb - 1) / memoryConfig.memory_frame_kb;
        add_process(id_process, size_process, max_pages, process, frames, max_frames);

        printf("\n2.1 - Ingrese un nuevo id numerico del proceso: ");
        scanf("%d", &id_process);
    }

    line();

    // PUNTO 3
    printf("PUNTO 3\n");
    printf("3.1 - Ingrese el id numerico del proceso: ");
    scanf("%d", &id_process);

    while (id_process != 0)
    {
        Process *processSearched = find_process_by_id(process, num_process, id_process);

        if (processSearched == NULL)
        {
            printf("El proceso buscado no existe.\n");
            break;
        }
        else
        {
            show_table_pages(processSearched);
        }

        printf("\n3.1 - Ingrese el id numerico del proceso: ");
        scanf("%d", &id_process);
    }

    line();
    // PUNTO 4
    printf("PUNTO 4\n");
    printf("Tabla de frames:\n");
    printf("Nro frame | ocupado\n");
    printf("----------|----------\n");
    for (int i = 0; i < max_frames; i++)
    {
        printf("%8d | %7d\n", i, frames[i].occupied);
    }

    line();

    // PUNTO 5
    int arr_sequence[] = {1, 2, 3, 4, 2, 5, 3};
    printf("PUNTO 5\n");
    printf("5.1 - Ingrese el id numerico del proceso: ");
    scanf("%d", &id_process);

    while (id_process != 0)
    {
        Process *processSearched = find_process_by_id(process, num_process, id_process);

        if (processSearched == NULL)
        {
            printf("El proceso buscado no existe.\n");
        }
        else
        {
            for (int i = 0; i < sizeof(arr_sequence) / sizeof(arr_sequence[0]); i++)
            {
                access_page(processSearched, arr_sequence[i], frames, max_frames);
            }

            printf("\nTabla del proceso luego de aplicar LRU\n");
            show_table_pages(processSearched);
        }

        printf("\n3.1 - Ingrese el id numerico del proceso: ");
        scanf("%d", &id_process);
    }

    line();
    // PUNTO 6
    printf("PUNTO 6\n");

    int bits_logic_address = (int)ceil(log2(ceil((memoryConfig.real_memory_mb * 3 * 1024) / memoryConfig.memory_frame_kb))) + (int)log2(ceil(memoryConfig.memory_frame_kb * 1024));
    printf("6.1 Bits necesarios para una direccion logica: %d\n", bits_logic_address);

    int bits_physical_address = (int)ceil(log2(max_frames)) + (int)log2(ceil(memoryConfig.memory_frame_kb * 1024));
    printf("6.2 Bits necesarios para una direccion fisica: %d\n", bits_physical_address);

    line();
    // PUNTO 7
    printf("PUNTO 7\n");
    /*
    7- Ingresar por teclado (este es un ciclo que se repite mientras el identificador del proceso sea distinto de 0)
    7.1.- Un Identificador de proceso y una dirección virtual relativa a 0, de 0 a N-1 siendo N el límite del proceso y mostrar su dirección lógica de paginación y su dirección física en binario y decimal.
*/
    printf("7.1 - Ingrese el id numerico del proceso: ");
    scanf("%d", &id_process);

    while (id_process != 0)
    {
        Process *processSearched = find_process_by_id(process, num_process, id_process);

        if (processSearched == NULL)
        {
            printf("El proceso buscado no existe.\n");
        }
        else
        {
        }

        printf("\n3.1 - Ingrese el id numerico del proceso: ");
        scanf("%d", &id_process);
    }

    return 0;
}

void memory_menu(MemoryConfig *memoryConfig)
{
    printf("PUNTO 1\n");
    printf("1.1 - Ingrese el tamano de la memoria real en MB: ");
    scanf("%d", &memoryConfig->real_memory_mb);

    printf("1.2 - Ingrese el tamano de la memoria real destinada al SO en KB: ");
    scanf("%d", &memoryConfig->memory_so_kb);

    printf("1.3 - Ingrese el tamano del frame en KB: ");
    scanf("%d", &memoryConfig->memory_frame_kb);
};

void add_process(int id, int size_kb, int max_pages, Process *process, Frame *frames, int max_frames)
{
    process[num_process].id = id;
    process[num_process].size_kb = size_kb;
    process[num_process].num_pages = max_pages;

    // funcion para inicializar la tabla de paginas
    // todas comienzan como el bit de validez en 0
    initialize_pages(&process[num_process], max_pages);

    int frames_to_assign = 3;
    for (int i = 0; i < max_pages; i++)
    {
        if (frames_to_assign > 0)
        {

            // si la funcion de buscar un frame retorna -1 quiere decir que no hay mas frames disponibles
            int id_frame = find_free_frame(frames, max_frames);
            if (id_frame != -1)
            {
                // le asignamos un frame libre al proceso
                process[num_process].frames[i] = id_frame;
                // se asigna el frame a cada posicion de la tabla de paginas, por un total de 3
                process[num_process].table_of_pages[i].id_frame = id_frame;
                // damos por hecho que los 3 frames asignados estan memoria
                process[num_process].table_of_pages[i].present_absent_bit = 1;
            }
            frames_to_assign--;
        }
        else
        {
            // inicializamos en -1 para indicar que no tienen ningun frame asociado
            process[num_process].table_of_pages[i].id_frame = -1;
        }
    }
    num_process++;
}

void initialize_pages(Process *process, int max_pages)
{
    // asignar memoria para la tabla de paginas
    process->table_of_pages = malloc(max_pages * sizeof(Page));
    if (process->table_of_pages == NULL)
    {
        printf("Error: No se pudo asignar memoria para la tabla de páginas.\n");
        exit(1);
    }

    // inicializamos cada pagina
    for (int i = 0; i < max_pages; i++)
    {
        process->table_of_pages[i].present_absent_bit = 0;
    }
}

int find_free_frame(Frame *frames, int max_frames)
{
    for (int i = 0; i < max_frames; i++)
    {
        if (frames[i].occupied == 0)
        {
            frames[i].occupied = 1;
            return frames[i].id;
        }
    }
    return -1;
}

Process *find_process_by_id(Process *processes, int num_processes, int id)
{

    for (int i = 0; i < num_processes; i++)
    {
        if (processes[i].id == id)
        {
            return &processes[i];
        }
    }
    return NULL;
}

void access_page(Process *process, int num_page, Frame *frames, int max_frames)
{
    current_time++;
    // chequeamos si la pagina esta presente o no en la memoria
    // num_page - 1 = nro pagina solicitado - 1 para encontrar su verdadera posicion
    // ejemplo: pag 1 esta en la posicion 0, y asi sucesivamente
    if (!process->table_of_pages[num_page - 1].present_absent_bit)
    {
        printf("\nFallo de pagina\n");
        show_table_pages(process);

        // aplicamos LRU
        int lru_index = 0;
        int lru_time = current_time++;
        for (int i = 0; i < process->num_pages; i++)
        {
            if (process->table_of_pages[i].present_absent_bit && process->table_of_pages[i].last_access_time < lru_time)
            {
                lru_time = process->table_of_pages[i].last_access_time;
                // indice de la pagina con el acceso a memoria mas viejo
                lru_index = i;
            }
        }

        // actualizar tabla
        process->table_of_pages[num_page - 1].id_frame = process->table_of_pages[lru_index].id_frame;
        process->table_of_pages[num_page - 1].present_absent_bit = 1;
        process->table_of_pages[num_page - 1].last_access_time = current_time;

        process->table_of_pages[lru_index].id_frame = -1;
        process->table_of_pages[lru_index].present_absent_bit = 0;
    }
    else
    {
        // actualizamos el tiempo del ultimo ingreso a la pagina
        process->table_of_pages[num_page - 1].last_access_time = current_time;
    }
};

void show_table_pages(Process *process)
{
    printf("\nTabla del proceso:\n");
    printf("Pagina | Frame | valido\n");
    printf("-------|-------|-------\n");
    for (int i = 0; i < process->num_pages; i++)
    {
        printf("%6d | %5d | %3d\n", i, process->table_of_pages[i].id_frame, process->table_of_pages[i].present_absent_bit);
    }
}

void line()
{
    printf("\n");
    printf("*****************************************************\n");
    printf("\n");
}
