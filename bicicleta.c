#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

const int N = 5;   // Espacio para almacenar piezas para OP2 y OP3
const int M = 10;  // Espacio para almacenar piezas para OP1

// Declaración de semáforos
sem_t sem_OP1;
sem_t sem_OP2;
sem_t sem_OP3;
sem_t sem_Montador;

// Función de operario OP1
void OP1() {
    while (true) {
        // Producción de ruedas
        // ...

        sem_wait(&sem_OP1);      // Adquirir una ranura de almacenamiento
        sem_post(&sem_Montador); // Liberar al montador para que pueda ensamblar la bicicleta
    }
}

// Función de operario OP2
void OP2() {
    while (true) {
        sem_wait(&sem_OP2);      // Esperar a tener una pieza disponible para el montaje del cuadro
        // Montaje del cuadro
        // ...
        sem_post(&sem_Montador); // Liberar al montador para que pueda ensamblar la bicicleta
    }
}

// Función de operario OP3
void OP3() {
    while (true) {
        sem_wait(&sem_OP3);      // Esperar a tener una pieza disponible para el montaje del manillar
        // Montaje del manillar
        // ...
        sem_post(&sem_Montador); // Liberar al montador para que pueda ensamblar la bicicleta
    }
}

// Función del montador
void Montador() {
    while (true) {
        sem_wait(&sem_Montador); // Esperar a tener todas las piezas necesarias para ensamblar la bicicleta
        // Ensamblar la bicicleta
        // ...
        sem_post(&sem_OP1);      // Liberar espacio en OP1 para almacenar más ruedas
        sem_post(&sem_OP2);      // Liberar espacio en OP2 para almacenar más cuadros
        sem_post(&sem_OP3);      // Liberar espacio en OP3 para almacenar más manillares
    }
}

int main() {
    // Inicializar semáforos
    sem_init(&sem_OP1, 0, M);
    sem_init(&sem_OP2, 0, 0);
    sem_init(&sem_OP3, 0, 0);
    sem_init(&sem_Montador, 0, 0);

    // Crear los hilos para cada operario
    std::thread t_OP1(OP1);
    std::thread t_OP2(OP2);
    std::thread t_OP3(OP3);
    std::thread t_Montador(Montador);

    // Esperar a que los hilos terminen (esto no se alcanzará nunca en este ejemplo)
    t_OP1.join();
    t_OP2.join();
    t_OP3.join();
    t_Montador.join();

    // Destruir los semáforos
    sem_destroy(&sem_OP1);
    sem_destroy(&sem_OP2);
    sem_destroy(&sem_OP3);
    sem_destroy(&sem_Montador);

    return 0;
}