# Sistemas Operativos y Redes
## Trabajo Práctico Semáforos primer semestre año 2022
En este trabajo se busco representar una competencia entre 4 equipos llamada *Subway Argento*, la cual tiene como dificultad que varios elementos de la competencia deben ser compartidos por los equipos por los que deben turnarse para utilizarlos.

Al ejecutar el programa se indica el inicio de los equipos y luego la actividad que estan realizando con sus respectivos ingredientes, las tareas tienen diferentes tiempos de duración para facilitar la comprención de lo que esta ocurriendo en cada momento de la ejecución y al finalizar se da a conocer en que orden los equipos finalizaron la preparación.

### Dificultades
- El manejo de los semaforos requirio una busqueda extensa de informacion sobre el tema para utilizarlos junto con Threads.
- La sincronizacion dentro del equipo para no realizar tareas que primero requieran la finalizacion de una tarea previa.
- La división entre los semaforos internos del equipo y los semaforos que se encargan de la utilización de las herramientas comunes entre los equipos requirio una organización precisa para no mezclar la ejecución de las tareas.
- Poder enviar a una funcion más de un parametro por medio de un Thread requirio varias modificaciones del codigo hasta dar con la solución.

### Pseudocodigo
```
main (){
    //declaracion de los semaforos e hilos
    sem_t sem_sarten;
    sem_t sem_salero;
    sem_t sem_horno;

    sem_init(&(sem_sarten),0,1);
    sem_init(&(sem_salero),0,1);
    sem_init(&(sem_horno),0,1);

    pthread_t hilo_equipo1;
    pthread_t hilo_equipo2;
    pthread_t hilo_equipo3;
    pthread_t hilo_equipo4;

    pthread_create(&hilo_equipo1, NULL, receta, semaforos_equipos);
    pthread_create(&hilo_equipo2, NULL, receta, semaforos_equipos);
    pthread_create(&hilo_equipo3, NULL, receta, semaforos_equipos);
    pthread_create(&hilo_equipo4, NULL, receta, semaforos_equipos);

    pthread_join (hilo_equipo1,NULL);
	pthread_join (hilo_equipo2,NULL);
	pthread_join (hilo_equipo3,NULL);
	pthread_join (hilo_equipo4,NULL);

    sem_destroy(&sem_1);
    sem_destroy(&sem_2);
	sem_destroy(&sem_3);

    pthread_exit(NULL);
}

void receta(hilos){
    sem_t tarea_2;
    sem_t tarea_3;
    sem_t tarea_4;

    pthread_t tarea1;
    pthread_t tarea2;
    pthread_t tarea3;
    pthread_t tarea4;

    //cada tarea se inicia al mismo tiempor pero se le envian los semaforos de las tareas para habilitar a la siguente tarea
    pthread_create(&tarea1, NULL, cortar, semaforos_tareas);
    pthread_create(&tarea2, NULL, mezclar, semaforos_tareas);
    pthread_create(&tarea3, NULL, empanar, semaforos_tareas);

    //la tarea cocinar no se realiza si la sarten no esta disponible y hasta que el semaforo interno del equipo le permita cocinar, luego de ser usada se da la señal que habilita a otro equipo a usarla la sarten
    //esto es similar con las demas actividades que requieren usar elementos compartidos
    sem_wait((&(sem_sarten))
    pthread_create(&tarea4, NULL, cocinar, semaforos_tareas);
    sem_post((&(sem_sarten))

    //Una vez finalizadas todas las tareas el equipo avisa que finalizo
    printf("Equipo X, termino!!!\n");



}
```