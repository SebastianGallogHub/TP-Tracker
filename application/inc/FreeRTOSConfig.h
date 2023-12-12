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
#define configUSE_DAEMON_TASK_STARTUP_HOOK 		1


#ifdef configTOTAL_HEAP_SIZE
#undef configTOTAL_HEAP_SIZE
#endif

#define configTOTAL_HEAP_SIZE                   ((size_t)(8 * 1024))

#ifdef configUSE_MALLOC_FAILED_HOOK
#undef configUSE_MALLOC_FAILED_HOOK
#endif

#define configUSE_MALLOC_FAILED_HOOK            1
