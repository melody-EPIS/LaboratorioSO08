#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>

union semun {
    int val;
    struct semid_ds* buf;
    unsigned short int* array;
    struct seminfo* __buf;
};

int crear_semaforo(key_t clave) {
    int id_semaforo = semget(clave, 1, 0600 | IPC_CREAT);
    if (id_semaforo == -1) {
        printf("No se pudo crear el semáforo\n");
        exit(1);
    }
    return id_semaforo;
}

void eliminar_semaforo(int id_semaforo) {
    if (semctl(id_semaforo, 0, IPC_RMID) == -1) {
        printf("No se pudo eliminar el semáforo\n");
        exit(1);
    }
}

void bloquear_semaforo(int id_semaforo) {
    struct sembuf operacion;
    operacion.sem_num = 0;
    operacion.sem_op = -1;
    operacion.sem_flg = 0;
    if (semop(id_semaforo, &operacion, 1) == -1) {
        printf("No se pudo bloquear el semáforo\n");
        exit(1);
    }
}

void desbloquear_semaforo(int id_semaforo) {
    struct sembuf operacion;
    operacion.sem_num = 0;
    operacion.sem_op = 1;
    operacion.sem_flg = 0;
    if (semop(id_semaforo, &operacion, 1) == -1) {
        printf("No se pudo desbloquear el semáforo\n");
        exit(1);
    }
}

int main() {
    key_t clave;
    int id_semaforo;
    int i = 0;
    clave = ftok("/bin/ls", 33);
    
    if (clave == (key_t) -1) {
        printf("No se pudo obtener la clave del semáforo\n");
        exit(1);
    }

    id_semaforo = crear_semaforo(clave);
    
    while (1) {
        printf("Esperando semáforo %d\n", i);
        bloquear_semaforo(id_semaforo);
        printf("Salgo del semáforo %d\n", i);
        i++;
    }

    eliminar_semaforo(id_semaforo);

    return 0;
}
