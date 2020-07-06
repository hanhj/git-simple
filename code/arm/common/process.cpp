#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int send_cmd ( int fd, int cmd )
{
    int retry = 10;

    while ( retry-- ) {
        //fprintf(stderr,"sock %d \r\n", fd );

        if ( write ( fd, &cmd, sizeof ( cmd ) ) != -1 )
            return 0;

        //mdelay(10);

    }

    return -1;
}

int recv_cmd ( int fd, int *cmd )
{
    fd_set readfd;
    int ret;

    struct timeval timeout;

    timeout.tv_sec = 0;
    timeout.tv_usec = 10000;
    FD_ZERO ( &readfd );
    FD_SET ( fd, &readfd );
    ret = select ( fd + 1, &readfd, NULL, NULL, &timeout );

    if ( ret && FD_ISSET ( fd, &readfd ) ) {
        if ( read ( fd, cmd, sizeof ( *cmd ) ) == sizeof ( *cmd ) )
            return 0;
    }

    return -1;
}
