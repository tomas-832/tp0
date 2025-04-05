#include "client.h"

t_log* iniciar_logger(void)
{
	t_log* logger = log_create ("tp0.log", "tp0", true, LOG_LEVEL_INFO); // logger es el puntero
	if (logger == NULL){
		printf("error al crear el logger");
		abort();
	}
	return logger;
}

t_config* iniciar_config(void)
{
	t_config* config = config_create("cliente.config");
	if (config == NULL){
		//Termino el programa
		printf("error al crear la configuración");
		abort();
	}
	return config;
}

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* clave;

	t_log* logger = iniciar_logger();
	t_config* config = iniciar_config();

	/* ---------------- LOGGING ---------------- */		

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger, "Soy un log");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	ip = config_get_string_value(config, "IP");
    puerto = config_get_string_value(config, "PUERTO");
    clave = config_get_string_value(config, "CLAVE");

	// Loggeamos el valor de config
	if (ip == NULL || puerto == NULL || clave == NULL ){
		printf("error faltan claves");
	} else{
		log_info(logger, "la ip es: %s", ip);
		log_info(logger, "El puerto es: %s", puerto);
		log_info(logger, "El clave es: %s", clave);
	}

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(clave, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

}

void leer_consola(t_log* logger)
{
    char* leido;
    leido = readline("> ");
    while(strcmp(leido,"") != 0){
        free(leido);
        leido = readline("> ");
        log_info(logger, "Ingresaste: %s", leido);
    }

        free(leido);
}

void paquete(int conexion)
{
    // Ahora toca lo divertido!
    char* leido;
    t_paquete* paquete = crear_paquete();

    // Leemos y esta vez agregamos las lineas al paquete
    leido = readline("> ");
    while(strcmp(leido,"") != 0){
        agregar_a_paquete(paquete, leido, strlen(leido) + 1);
        free(leido);
        leido = readline("> ");
    }

    free(leido);

    enviar_paquete(paquete, conexion);

    eliminar_paquete(paquete);


    // ¡No te olvides de liberar las líneas y el paquete antes de regresar!
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
}