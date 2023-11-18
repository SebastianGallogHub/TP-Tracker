/*
 * FreeRTOSConfig.h
 *
 *  Created on: 16 nov. 2023
 *      Author: sebas
 */

#include_next <FreeRTOSConfig.h>

#ifdef configUSE_DAEMON_TASK_STARTUP_HOOK
#undef configUSE_DAEMON_TASK_STARTUP_HOOK
#endif

//Activar el hook Daemon para que ejecute las inicializaciones después que el Scheduler
//inicializa los softTimers
#define configUSE_DAEMON_TASK_STARTUP_HOOK 1
