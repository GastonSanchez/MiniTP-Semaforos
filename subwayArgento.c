#include <stdio.h>      // libreria estandar
#include <stdlib.h>     // para usar exit y funciones de la libreria standard
#include <string.h>
#include <pthread.h>    // para usar threads
#include <semaphore.h>  // para usar semaforos
#include <unistd.h>

#define LIMITE 50

//creo estructura de semaforos 
struct semaforos {
    sem_t sem_mezclar;
    sem_t sem_salar;
    sem_t sem_salarEquipo;
    sem_t sem_empanar;
    sem_t sem_freir;
    sem_t sem_freirEquipo;
	sem_t sem_hornear;
	sem_t sem_armar;
    
};

struct paso {
   char accion [LIMITE];
   char ingredientes[4][LIMITE];
   
};
 
struct team{
    int equipo0;
    int equipo2;
    int equipo3;
    int equipo4;
};

struct parametro {
    struct team equipos;
    int equipo_param;
    struct semaforos semaforos_param;
    struct paso pasos_param[8];
};

void* imprimirAccion(void *data, char *accion) {
	struct parametro *mydata = data; 
	int sizeArray = (int)( sizeof(mydata->pasos_param) / sizeof(mydata->pasos_param[0])); 
	for(int i = 0; i < sizeArray; i ++){
		//pregunto si la accion del array es igual a la pasada por parametro
		if(strcmp(mydata->pasos_param[i].accion, accion) == 0){
		printf("\tEquipo %d esta %s \n " , mydata->equipo_param, mydata->pasos_param[i].accion);
		int sizeArrayIngredientes = (int)( sizeof(mydata->pasos_param[i].ingredientes) / sizeof(mydata->pasos_param[i].ingredientes[0]) );
		printf("\tEquipo %d -----------ingredientes----------\n",mydata->equipo_param); 
			for( int h = 0; h < sizeArrayIngredientes; h++) {
				//consulto si la posicion tiene valor porque no se cuantos ingredientes tengo x accion 
				if(strlen(mydata->pasos_param[i].ingredientes[h]) != 0) {
							printf("\tEquipo %d ingrediente  %d : %s \n",mydata->equipo_param,h,mydata->pasos_param[i].ingredientes[h]);
				}
			}
		}
	}
}

void* cortar(void *data) { 
	char *accion = "cortando";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	imprimirAccion(mydata,accion);
	usleep( 2000000 );
	//doy la señal a la siguiente accion (cortar me habilita mezclar)
    sem_post(&mydata->semaforos_param.sem_mezclar);
	
    pthread_exit(NULL);
}

void* mezclar(void *data) { 
	char *accion = "mezclando"; 
	struct parametro *mydata = data;
    sem_wait(&mydata->semaforos_param.sem_mezclar);
	imprimirAccion(mydata,accion);
	usleep( 2000000 );
	//doy la señal a la siguiente accion (mezclar me habilita salar)
    sem_post(&mydata->semaforos_param.sem_salarEquipo);
	
    pthread_exit(NULL);
}

void* salar(void *data) { 
	char *accion = "salando"; 
	struct parametro *mydata = data;
    sem_wait(&mydata->semaforos_param.sem_salarEquipo);
	imprimirAccion(mydata,accion);
	usleep( 5000000 );
	//doy la señal a la siguiente accion (salar me habilita empanar)
    sem_post(&mydata->semaforos_param.sem_empanar);
	
    pthread_exit(NULL);
}

void* empanar(void *data) { 
	char *accion = "empanando";
	struct parametro *mydata = data;
    sem_wait(&mydata->semaforos_param.sem_empanar);
	imprimirAccion(mydata,accion);
	usleep( 1000000 );
	//doy la señal a la siguiente accion (empanar me habilita freir)
    sem_post(&mydata->semaforos_param.sem_freirEquipo);
	
    pthread_exit(NULL);
}

void* freir(void *data) { 
	char *accion = "friendo";
	struct parametro *mydata = data;
    sem_wait(&mydata->semaforos_param.sem_freirEquipo);
	imprimirAccion(mydata,accion);
	usleep( 5000000 );
    sem_post(&mydata->semaforos_param.sem_armar);
    pthread_exit(NULL);
}

void* hornear(void *data) { 
	char *accion = "horneando";
	struct parametro *mydata = data;
    //sem_wait(&mydata->semaforos_param.sem_hornear);
	imprimirAccion(mydata,accion);
	usleep( 3000000 );
    pthread_exit(NULL);
}

void* verduras(void *data) { 
	char *accion = "cortando verduras";
	struct parametro *mydata = data;
	imprimirAccion(mydata,accion);
	usleep( 2000000 );
    pthread_exit(NULL);
}

void* armar(void *data) { 
	char *accion = "armando sandwich";
	struct parametro *mydata = data;
    sem_wait(&mydata->semaforos_param.sem_armar);
	imprimirAccion(mydata,accion);
	usleep( 1000000 );
    pthread_exit(NULL);
}

void* ejecutarReceta(void *data) {
	
    struct parametro *mydata = data;

	//variables semaforos
	sem_t sem_mezclar;
    sem_t sem_salarEquipo;
	sem_t sem_empanar;
    sem_t sem_freirEquipo;
	sem_t sem_armar;
	
	//variables hilos, uno por cada accion para ejecutar la receta
	pthread_t p1;
	pthread_t p2;
	pthread_t p3;
	pthread_t p4;
    pthread_t p5;
	pthread_t p6;
	pthread_t p7;
	pthread_t p8;
	
	//numero del equipo (El primero que ingresa se le asigna el numero 1, al segundo el 2 y asi sucesivamente)
    int p = mydata->equipos.equipo0;
    mydata->equipos.equipo0 ++;   

	printf("Equipo %d inicio\n", p);

	//reservo memoria para el struct
	struct parametro *pthread_data = malloc(sizeof(struct parametro));

	//SETEO LOS VALORES AL STRUCT
	
	//seteo numero de grupo
	pthread_data->equipo_param = p;

	//seteo semaforos
	pthread_data->semaforos_param.sem_mezclar = sem_mezclar;
    pthread_data->semaforos_param.sem_salarEquipo = sem_salarEquipo;
    pthread_data->semaforos_param.sem_empanar = sem_empanar;
    pthread_data->semaforos_param.sem_freirEquipo = sem_freirEquipo;
	pthread_data->semaforos_param.sem_armar = sem_armar;

	//seteo las acciones y los ingredientes
    strcpy(pthread_data->pasos_param[0].accion, "cortando");
	strcpy(pthread_data->pasos_param[0].ingredientes[0], "ajo");
    strcpy(pthread_data->pasos_param[0].ingredientes[1], "perejil");

	strcpy(pthread_data->pasos_param[1].accion, "mezclando");
	strcpy(pthread_data->pasos_param[1].ingredientes[0], "ajo");
    strcpy(pthread_data->pasos_param[1].ingredientes[1], "perejil");
 	strcpy(pthread_data->pasos_param[1].ingredientes[2], "huevo");
	strcpy(pthread_data->pasos_param[1].ingredientes[3], "carne");

	strcpy(pthread_data->pasos_param[2].accion, "salando");
	strcpy(pthread_data->pasos_param[2].ingredientes[0], "mezcla");
    strcpy(pthread_data->pasos_param[2].ingredientes[1], "sal");

	strcpy(pthread_data->pasos_param[3].accion, "empanando");
	strcpy(pthread_data->pasos_param[3].ingredientes[0], "carne");
    strcpy(pthread_data->pasos_param[3].ingredientes[1], "pan rallado");
	
	strcpy(pthread_data->pasos_param[4].accion, "friendo");
	strcpy(pthread_data->pasos_param[4].ingredientes[0], "milanesa");
    strcpy(pthread_data->pasos_param[4].ingredientes[1], "aceite");

	strcpy(pthread_data->pasos_param[5].accion, "horneando");
	strcpy(pthread_data->pasos_param[5].ingredientes[0], "pan");
	
	strcpy(pthread_data->pasos_param[6].accion, "cortando verduras");
	strcpy(pthread_data->pasos_param[6].ingredientes[0], "lechuga");
    strcpy(pthread_data->pasos_param[6].ingredientes[1], "tomate");
 	strcpy(pthread_data->pasos_param[6].ingredientes[2], "cebolla");
	strcpy(pthread_data->pasos_param[6].ingredientes[3], "pepino");

	strcpy(pthread_data->pasos_param[7].accion, "armando sandwich");
	strcpy(pthread_data->pasos_param[7].ingredientes[0], "milanesa");
	strcpy(pthread_data->pasos_param[7].ingredientes[1], "pan");
	strcpy(pthread_data->pasos_param[7].ingredientes[2], "verduras");
	
	//inicializo los semaforos
	sem_init(&(pthread_data->semaforos_param.sem_mezclar),0,0);
    sem_init(&(pthread_data->semaforos_param.sem_salarEquipo),0,0);
    sem_init(&(pthread_data->semaforos_param.sem_empanar),0,0);
    sem_init(&(pthread_data->semaforos_param.sem_freirEquipo),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_armar),0,0);

	//creo los hilos a todos les paso el struct creado (el mismo a todos los hilos) ya que todos comparten los semaforos 
    int rc;
    rc = pthread_create(&p1, NULL, cortar, pthread_data);
	rc = pthread_create(&p2, NULL, mezclar, pthread_data);
    rc = pthread_create(&p3, NULL, verduras, pthread_data);

    sem_wait(&mydata->semaforos_param.sem_salar);
	rc = pthread_create(&p5, NULL, salar, pthread_data);
    sem_post(&mydata->semaforos_param.sem_salar);

    sem_wait(&mydata->semaforos_param.sem_hornear);
	rc = pthread_create(&p4, NULL, hornear, pthread_data);
    sem_post(&mydata->semaforos_param.sem_hornear);

	rc = pthread_create(&p6, NULL, empanar, pthread_data);
    
    sem_wait(&mydata->semaforos_param.sem_freir);
	rc = pthread_create(&p7, NULL, freir, pthread_data);
    sem_post(&mydata->semaforos_param.sem_freir);
	
    rc = pthread_create(&p8, NULL, armar, pthread_data);
	
	
	//join de todos los hilos
	pthread_join (p1,NULL);
	pthread_join (p2,NULL);
	pthread_join (p3,NULL);
	pthread_join (p4,NULL);
    pthread_join (p5,NULL);
	pthread_join (p6,NULL);
	pthread_join (p7,NULL);
	pthread_join (p8,NULL);

    printf("Equipo %d termino!!!\n", p);

	//valido que el hilo se alla creado bien 
    if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1); 
    }

	//destruccion de los semaforos 
	sem_destroy(&sem_mezclar);
    sem_destroy(&sem_salarEquipo);
    sem_destroy(&sem_empanar);
	sem_destroy(&sem_armar); 
	
	//salida del hilo
	 pthread_exit(NULL);
}

int main ()
{
    //Semaforos para las acciones que se hacen de a un equipo a la vez
    sem_t sem_salar;
    sem_t sem_freir;
    sem_t sem_hornear;

    struct parametro *generales = malloc(sizeof(struct parametro));

    generales->semaforos_param.sem_salar = sem_salar;
    generales->semaforos_param.sem_hornear = sem_hornear;
	generales->semaforos_param.sem_freir = sem_freir;

    sem_init(&(generales->semaforos_param.sem_salar),0,1);
    sem_init(&(generales->semaforos_param.sem_freir),0,1);
	sem_init(&(generales->semaforos_param.sem_hornear),0,1);


	//creo los nombres de los equipos 
	int rc;
    int equipoNombre = 1;

	//creo las variables los hilos de los equipos
	pthread_t equipo1; 
	pthread_t equipo2;
    pthread_t equipo3;
    pthread_t equipo4;

    //Creo un equipo generico que van a utilizar todos los hilos
    generales->equipos.equipo0 = equipoNombre;
  
	//inicializo los hilos de los equipos (identificador unico, atributos del thread,funcion a ejecutar)
    rc = pthread_create(&equipo1, NULL, ejecutarReceta, generales); 
    rc = pthread_create(&equipo2, NULL, ejecutarReceta, generales);
	rc = pthread_create(&equipo3, NULL, ejecutarReceta, generales);
	rc = pthread_create(&equipo4, NULL, ejecutarReceta, generales);


   if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
    } 

	//join de todos los hilos
	pthread_join (equipo1,NULL);
	pthread_join (equipo2,NULL);
	pthread_join (equipo3,NULL);
	pthread_join (equipo4,NULL);

    sem_destroy(&sem_salar);
    sem_destroy(&sem_freir);
	sem_destroy(&sem_hornear);

    pthread_exit(NULL);
}

//Para compilar:   gcc subwayArgento.c -o ejecutable -lpthread
//Para ejecutar:   ./ejecutable
