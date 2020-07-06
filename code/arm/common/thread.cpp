
#include <string.h>
#include "debug.h"
#include "os.h"

int thread_cond_init ( struct cond *cond, const char *name )
{
#ifdef CONFIG_LINUX
    strncpy ( cond->name, name, 31 );

    if ( pthread_mutex_init ( &cond->mutex, NULL ) < 0 )
        return -1;

    return pthread_cond_init ( &cond->cond, NULL );

#endif
}

int thread_signal ( struct cond *cond )
{
#ifdef CONFIG_LINUX
    pthread_mutex_lock ( &cond->mutex );
    pthread_cond_signal ( &cond->cond );
    pthread_mutex_unlock ( &cond->mutex );
    return 0;
#endif
}

int thread_signal_all ( struct cond *cond )
{
#ifdef CONFIG_LINUX
    pthread_mutex_lock ( &cond->mutex );
    pthread_cond_broadcast ( &cond->cond );
    pthread_mutex_unlock ( &cond->mutex );
    return 0;
#endif
}

int thread_wait_cond ( struct cond *cond )
{
#ifdef CONFIG_LINUX
    pthread_mutex_lock ( &cond->mutex );
    pthread_cond_wait ( &cond->cond, &cond->mutex );
    pthread_mutex_unlock ( &cond->mutex );
    return 0;
#endif
}

int thread_cond_destroy ( struct cond *cond )
{
#ifdef CONFIG_LINUX
    pthread_mutex_destroy ( &cond->mutex );
    pthread_cond_destroy ( &cond->cond );
    return 0;
#endif
}

int thread_init()
{

#ifdef CONFIG_LINUX
    pthread_setcancelstate ( PTHREAD_CANCEL_ENABLE, NULL );
    pthread_setcanceltype ( PTHREAD_CANCEL_ASYNCHRONOUS, NULL );
    return 0;
#endif
}

int mutex_init ( struct mutex *m, const char *name )
{
#ifdef CONFIG_LINUX
    pthread_mutexattr_t attr;
    pthread_mutexattr_init ( &attr );
    pthread_mutexattr_settype ( &attr, PTHREAD_MUTEX_RECURSIVE );
    m->lock_count = 0;
    strncpy ( m->name, name, 31 );
    return pthread_mutex_init ( ( pthread_mutex_t* ) m->lock, &attr );
#else
    m->sem = Semaphore_create ( 1, NULL, NULL );
    return 1;
#endif
}

int mutex_destroy ( struct mutex *m )
{
#ifdef CONFIG_LINUX
    return pthread_mutex_destroy ( ( pthread_mutex_t* ) m->lock );
#else
    Semaphore_delete ( &m->sem );
    return 1;
#endif
}

#ifdef CONFIG_LINUX
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
#endif
int mutex_lock ( struct mutex *m )
{
#ifdef CONFIG_LINUX
    int ret;
    ret = pthread_mutex_lock ( ( pthread_mutex_t* ) m->lock );
    pthread_mutex_lock ( &lock );

    if ( ++m->lock_count >= 2 )
        PFUNC ( TEM_WARNING ,DEBUG_NORMAL, "Recursive lock happens %s %d\n", m->name, m->lock_count );

    pthread_mutex_unlock ( &lock );

    return ret;

#else
    return Semaphore_pend ( m->sem, BIOS_WAIT_FOREVER );

#endif
}

int mutex_unlock ( struct mutex *m )
{
#ifdef CONFIG_LINUX
    int ret;
    pthread_mutex_lock ( &lock );

    if ( --m->lock_count < 0 )
        PFUNC ( TEM_WARNING ,DEBUG_NORMAL, "Unmatch lock/unlock %s %d\n", m->name, m->lock_count );

    pthread_mutex_unlock ( &lock );

    ret = pthread_mutex_unlock ( ( pthread_mutex_t* ) m->lock );

    return ret;

#else
    Semaphore_post ( m->sem );

    return 1;

#endif
}

