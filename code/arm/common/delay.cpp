#include "datatype.h"
#include "os.h"
#ifdef CONFIG_LINUX
#include <fcntl.h>
#endif
#include "delay.h"
int udelay ( uint32 usecond )
{

    struct timeval tval;
    tval.tv_sec = usecond / 1000000;
    tval.tv_usec = usecond % 1000000;
#ifdef CONFIG_LINUX
    return select ( 0, NULL, NULL, NULL, &tval );
#else
    Task_sleep ( usecond*12 );
    return 1;
#endif
}

int mdelay ( uint32 msecond )
{
    return udelay ( msecond * 1000 );
}

int delay ( uint32 second )
{
    return mdelay ( second * 1000 );
}
