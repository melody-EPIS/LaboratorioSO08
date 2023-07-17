#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

union semun {
    int val;
    struct semid_ds* buf;
    unsigned short int* array;
    struct seminfo* __buf;
};

int obtener_semaforo(key_t clave) {
    int id_semaforo = semget(clave, 1, 0);
    if (id_semaforo == -1) {
        printf("No se pudo obtener el semáforo\n");
        exit(1);
    }
    return id_semaforo;
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
    int i;
    clave = ftok("/bin/ls", 33);
    
    if (clave == (key_t) -1) {
        printf("No se pudo obtener la clave del semáforo\n");
        exit(1);
    }
    id_semaforo = obtener_semaforo(clave);
    
    for (i = 0; i < 10; i++) {
        printf("Levanto semáforo\n");
        desbloquear_semaforo(id_semaforo);
        sleep(1);
    }

    return 0;
}
