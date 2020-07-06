#include<ctype.h>
#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include "datatype.h"
#include "gps.h"
#include "debug.h"
#include "timer.h"
#include "protocol.h"
#include "sntp.h"
GPS gps;
inline char* strlwr ( char* str )
{
    char* orig = str;
    // process the string

    for ( ;*str != 0;str++ ) {
        *str = tolower ( *str );

        if ( *str == '\0' ) break;
    }

    return orig;
}

GPS::GPS()
{
    buffHead = 0;
    buffEnd = 0;
}


char deal_progps ( BUF_MUX *buf, uint16 port_id )
{
	static int frame_ok=0;
    static uint16 datalen = 0;
    static char data[1024];
    static int begin=0;
	uint8 c=0;
    while ( buf->recv.in_cnt != buf->recv.out_cnt ) {
        c = buf->recv.buf[buf->recv.out_cnt++];
        //fprintf(stderr,"%c",c);
        if ( buf->recv.out_cnt >= BUFFER_SIZE ) {
            buf->recv.out_cnt = 0;
        }

        if ( c == '$' ) {
        	datalen=0;
        	begin=1;
        	//PFUNC ( TEM_INFO, DEBUG_GPS, "Begin:%d Len:%d  \n",begin, datalen );
		}
        if (begin==1){
        	data[datalen++]=c;
        	if(c == 0x0a ) {
                frame_ok=1;
                data[datalen]=0;
            }
		}
        if ( datalen >= 1024 ) {
        	datalen=0;
        	begin=0;
        }
    
	    if ( frame_ok==1 ){
	    	//PFUNC ( TEM_INFO, DEBUG_GPS, "Len:%d  \n", datalen );
	        //PFUNC ( TEM_INFO, DEBUG_GPS, "%s\n", data );
	        if(gps.CheckSum(data)==1)
	        gps.analyseGPSProc ( data, datalen );
		    datalen=0; 
		    frame_ok=0; 
		    begin=0;  
		}
    }
    return 0;
}


/*******************************************************************************
**           Function: Analyse GPS Signal Information                         **
**      $GPRMC,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>,<10>,<11>*hh               **
**       <01>:  [Time]: hhmmss                                                **
**       <02>:  [State]: A is efficiency , V is inefficiency                  **
**       <03>:  [Latitude]: ddmm.mmmm                                         **
**       <04>:  [South or North earth]: N is north, S is south                **
**       <05>:  [Longitude]: ddmm.mmmm                                        **
**       <06>:  [East or West earth]: E is east, W is west                    **
**       <07>:  [High ]:  range from 0.0 to 999.9                             **
**       <08>:  [direction]: range from 000.0 to 359.9                        **
**       <09>:  [date]:  ddmmyy                                               **
**       <10>:  [Magnetic Change]: range from 000.0 to 180.0                  **
**       <11>:  [Magnetic director change]: E or W                            **
**               In:  NULL                                                    **
**               Out: NULL                                                    **
**               Date: 2007-8-1     Author: Owen.Lu                           **
*******************************************************************************/
int GPS::analyseGPSProc ( char *data, int16 len )
{
    char singleType[10] = {'\0'};
    int  knowSignle = 0;
    int i;

    for ( i = 0;i < len;i++ ) {
        gpsMessage[buffHead][0] = data[i];
        gpsMessage[buffHead][1] = 1 ;
        buffHead = ( buffHead + 1 ) % 1024;
    }

    while ( buffHead != buffEnd ) {
        getString ( singleType );

        if ( strcmp ( singleType, "$gpalm" ) == 0 ) {
            knowSignle = 1;
            gpsGPALM();
        }
/*
        else if ( strcmp ( singleType, "$pgrmi" ) == 0 ) {
            knowSignle = 1;
            gpsPGRMI();
        }

        else if ( strcmp ( singleType, "$pgrmo" ) == 0 ) {
            knowSignle = 1;
            gpsPGRMO();
        }
*/
        else if ( strcmp ( singleType, "$gpgga" ) == 0 ) {
            knowSignle = 1;
            gpsGPGGA();
        }
/*
        else if ( strcmp ( singleType, "$gpgsa" ) == 0 ) {
            knowSignle = 1;
            gpsGPGSA();
        }

        else if ( strcmp ( singleType, "$gpgsv" ) == 0 ) {
            knowSignle = 1;
            gpsGPGSV();
        }
*/
        else if ( strcmp ( singleType, "$gprmc" ) == 0 ) {
            knowSignle = 1;
            gpsGPRMC();
        }
/*
        else if ( strcmp ( singleType, "$gpvtg" ) == 0 ) {
            knowSignle = 1;
            gpsGPVTG();
        }

        else if ( strcmp ( singleType, "$lcgll" ) == 0 ) {
            knowSignle = 1;
            gpsLCGLL();
        }

        else if ( strcmp ( singleType, "$lcvtg" ) == 0 ) {
            knowSignle = 1;
            gpsLCVTG();
        }

        else if ( strcmp ( singleType, "$pgrme" ) == 0 ) {
            knowSignle = 1;
            gpsPGRME();
        }

        else if ( strcmp ( singleType, "$pgrmt" ) == 0 ) {
            knowSignle = 1;
            gpsPGRMT();
        }
*/
        if ( knowSignle == 0 ) {
            //fprintf ( stderr, "%s.............\n", singleType );
        }

        knowSignle = 0;
    }

    return 0;
}

/*******************************************************************************
**           Function: Get string from string by char                         **
**               In:                                                          **
**               Out:                                                         **
**               Date: 2007-8-1     Author: Owen.Lu                           **
*******************************************************************************/
void GPS::getBetweenChar ( char input[], char backstr[], char stopcn )
{
    int ii = 0;

    while ( input[ii] != stopcn ) {
        backstr[ii] = input[ii];
        ii++;
    }

    backstr[ii] = '\0';
}

/*******************************************************************************
**           Function: Get system time and date                               **
**               In:                                                          **
**               Out:                                                         **
**               Date: 2007-8-1     Author: Owen.Lu                           **
*******************************************************************************/
void GPS::getSystemTime ( char stime[], char sdate[] )
{
    /*
        SYSTEMTIME  systime;
        char    temp[5];
        GetLocalTime(&systime);
        sprintf(stime, "%02d%02d%02d", systime.wHour, systime.wMinute, systime.wSecond);
        sprintf(sdate, "%02d%02d", systime.wDay, systime.wMonth);
        sprintf(temp, "%d", systime.wYear);
        temp[0] = temp[2];
        temp[1] = temp[3];
        temp[2] = '\0';
        strcat(sdate, temp);
    */
}

/******************************************************************************
**           Function: Get GPALM for buffer                                  **
**               In:  NULL                                                   **
**               Out: GPALM Information                                      **
**               Date: 2007-8-24     Author: Owen.Lu                         **
*******************************************************************************/
void GPS::gpsALM()
{
    // fprintf ( stderr, "$ALM............\n" );
    return;
}

/*******************************************************************************
**           Function: Get PGRMI  for buffer                                  **
**               In: NULL                                                     **
**               Out: PGRMI Information                                       **
**               Date: 2007-8-1     Author: Owen.Lu                           **
*******************************************************************************/
void GPS::gpsPGRMI()
{
    //fprintf ( stderr, "$PGRMI............\n" );
    return;
}

/*******************************************************************************
**           Function: Get GPALM for buffer                                   **
**               In:  NULL                                                    **
**               Out: GPALM   Information                                     **
**               Date: 2007-8-1     Author: Owen.Lu                           **
*******************************************************************************/
void GPS::gpsGPALM()
{
    //fprintf ( stderr, "$GPALM............\n" );
    return;
}

/*******************************************************************************
**           Function: Get PGRMO for buffer                                   **
**               In:  NULL                                                    **
**               Out: PGRMO Information                                       **
**               Date: 2007-8-1     Author: Owen.Lu                           **
*******************************************************************************/
void GPS::gpsPGRMO()
{
    //fprintf ( stderr, "$PGRMO............\n" );
    return;
}

/*******************************************************************************
**           Function: Get GPLM for buffer                                    **
**               In:  NULL                                                    **
**               Out: GPLM Information                                        **
**               Date: 2007-8-1     Author: Owen.Lu                           **
*******************************************************************************/
void GPS::gpsGPLM()
{
    //fprintf ( stderr, "$GPLM............\n" );
    return;
}

/*******************************************************************************
**           Function: Get GPGGA for buffer                                   **
**               In:  NULL                                                    **
**               Out: GPGGA Information                                       **
**               Date: 2007-8-1     Author: Owen.Lu                           **
*******************************************************************************/
void GPS::gpsGPGGA()
{
    //fprintf ( stderr, "$GPGGA............\n" );

    char tempStr[30] = {0};

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsCga.time, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsCga.latitude, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsCga.southornorth, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsCga.longitude, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsCga.eastorwest, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsCga.mode, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsCga.satellites, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsCga.hdop, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsCga.high, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsCga.highunit, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsCga.geosep, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsCga.geosepunit, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsCga.agecorr, tempStr );

//   memset(tempStr, 0, 30);
    //getStringEnd(',', tempStr);
//    strcpy(gpsCga.diffid, tempStr);
#if 0
    fprintf ( stderr, "********************GPGGA**************************************\n" );
    fprintf ( stderr, "Time        : %s\n", gpsCga.time );
    fprintf ( stderr, "Latitude    : %s\n", gpsCga.latitude );
    fprintf ( stderr, "Southornorth: %s\n", gpsCga.southornorth );
    fprintf ( stderr, "Longitude   : %s\n", gpsCga.longitude );
    fprintf ( stderr, "Eastorwest  : %s\n", gpsCga.eastorwest );
    fprintf ( stderr, "Mode        : %s\n", gpsCga.mode );
    fprintf ( stderr, "Satellites  : %s\n", gpsCga.satellites );
    fprintf ( stderr, "Hdop        : %s\n", gpsCga.hdop );
    fprintf ( stderr, "High        : %s\n", gpsCga.high );
    fprintf ( stderr, "HighUnit    : %s\n", gpsCga.highunit );
    fprintf ( stderr, "GeoSep      : %s\n", gpsCga.geosep );
    fprintf ( stderr, "GeoSepUnit  : %s\n", gpsCga.geosepunit );
    fprintf ( stderr, "AgeCorr     : %s\n", gpsCga.agecorr );
#endif
    char time_str[16];
    uint16 len = 0;
    char cell[8];

	SYSTEMTIME gps_time,sys_time,st;
	CP56Time2a _t;
	double span;
	CNtpTime ntp,ntp2;

   if ( atoi ( gpsCga.satellites ) == 0) {//卫星数为零
       PFUNC ( TEM_WARNING, DEBUG_GPS, "$gpgga frame data not valid\n" );
       return ;
   }

   gpsCga.time[10] = 0;

   //PFUNC ( TEM_WARNING, DEBUG_GPS, "%s\n", &gpsCga.time[0] );

   if ( strlen ( &gpsCga.time[0] ) == 0 ) {
       PFUNC ( TEM_WARNING, DEBUG_GPS, "$gpgga time string is invalid\n" );
       return ;
   }

   timer.GetCP56Time ( &_t );
   gps_time.wYear=_t.year+2000;
   gps_time.wMonth=_t.month;
   gps_time.wDay=( _t.day_week) &0x1f;
	//hh
   memset ( cell, 0, 8 );
   memcpy ( cell, &gpsCga.time[0], 2 );
   gps_time.wHour = atoi ( cell ) ;
   //mm
   memset ( cell, 0, 8 );
   memcpy ( cell, &gpsCga.time[2], 2 );
   gps_time.wMinute  = atoi ( cell );
   //ss
   memset ( cell, 0, 8 );
   memcpy ( cell, &gpsCga.time[4], 2 );
   gps_time.wSecond  = atoi ( cell );
   //sss
   memset ( cell, 0, 8 );
   memcpy ( cell, &gpsCga.time[7], 3 );
   gps_time.wMilliseconds = atoi ( cell );


	ntp=CNtpTime(gps_time);
	span=8*3600;
	ntp.AddSpan(span);//加上8小时
	st=ntp;
	
    sys_time.wHour 			= _t.hour_su & 0x1f;
    sys_time.wMinute  		= _t.minute_iv & 0x7f;
    sys_time.wSecond  		= _t.millisecond / 1000;
    sys_time.wMilliseconds 	= _t.millisecond % 1000;
    sys_time.wDay 			= _t.day_week & 0x1f;
    sys_time.wMonth 		= _t.month;
    sys_time.wYear 			= _t.year+2000;
	ntp2=CNtpTime(sys_time);
	
	if ( ( abs ( ntp2.Seconds() - ntp.Seconds() ) >= 2 )  ) {
	    _t.millisecond = st.wMilliseconds + st.wSecond * 1000;
	    _t.minute_iv = st.wMinute | ( _t.minute_iv & 0xc0 );
	    _t.hour_su = st.wHour | ( _t.hour_su & 0xe0 );
	    _t.day_week=(st.wDay|(st.wDayOfWeek<<5));
	    _t.month=st.wMonth;
	    _t.year=st.wYear-2000;
	    
	    ////////////////pack data for prosettime func//////////
	    time_str[0] = 1;//num
	    time_str[1] = 0;//num
	    time_str[2] = 0;//id
	    time_str[3] = 0;//id
	    time_str[4] = 7;//len
	    memcpy ( &time_str[5], ( char* ) &_t, 7 );
	    len = 12;
	    ProSetTime ( time_str, len, time_str, &len );
	    PFUNC ( TEM_INFO, DEBUG_GPS, "sys.s=%d  gps.s=%d\n", sys_time.wSecond , st.wSecond );
	    PFUNC ( TEM_INFO, DEBUG_GPS, "sys.m=%d  gps.m=%d\n", sys_time.wMinute , st.wMinute );
	    PFUNC ( TEM_INFO, DEBUG_GPS, "sys.h=%d  gps.h=%d\n" , sys_time.wHour , st.wHour );
	    PFUNC ( TEM_INFO, DEBUG_GPS, "GPS time in used 1\n" );
	}
	PFUNC ( TEM_INFO, DEBUG_GPS, "$gpgga %02d-%02d-%02d %02d:%02d:%02d.%d\n", st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
    return;
}

/*******************************************************************************
**           Function: Get GPGSA for buffer                                   **
**               In:  NULL                                                    **
**               Out: GPGSA Information                                       **
**               Date: 2007-8-1     Author: Owen.Lu                           **
*******************************************************************************/
void GPS::gpsGPGSA()
{
    //fprintf ( stderr, "$GPGSA............\n" );

    char tempStr[30] = {0};
    double prange = 0.0;

    memset ( &gpsGSA, 0, sizeof ( gpsGSA ) );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsGSA.mode, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsGSA.currentState, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsGSA.PRN01, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsGSA.PRN02, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsGSA.PRN03, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsGSA.PRN04, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsGSA.PRN05, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsGSA.PRN06, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsGSA.PRN07, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsGSA.PRN08, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsGSA.PRN09, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsGSA.PRN10, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsGSA.PRN11, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsGSA.PRN12, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsGSA.pdop, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsGSA.hdop, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( '*', tempStr );
    strcpy ( gpsGSA.vdop, tempStr );

//    memset(tempStr, 0, 30);
//    getStringEnd(',', tempStr);
//    tempStr[2] = '\0';
//    strcpy(gpsGSA.endvalue, tempStr);
#if 0
    fprintf ( stderr, "********************$GPGSA***************************\n" );
    fprintf ( stderr, "Mode:        %s\n", gpsGSA.mode );
    fprintf ( stderr, "Current:     %s\n", gpsGSA.currentState );
    fprintf ( stderr, "PRN01:       %s\n", gpsGSA.PRN01 );
    fprintf ( stderr, "PRN02:       %s\n", gpsGSA.PRN02 );
    fprintf ( stderr, "PRN03:       %s\n", gpsGSA.PRN03 );
    fprintf ( stderr, "PRN04:       %s\n", gpsGSA.PRN04 );
    fprintf ( stderr, "PRN05:       %s\n", gpsGSA.PRN05 );
    fprintf ( stderr, "PRN06:       %s\n", gpsGSA.PRN06 );
    fprintf ( stderr, "PRN07:       %s\n", gpsGSA.PRN07 );
    fprintf ( stderr, "PRN08:       %s\n", gpsGSA.PRN08 );
    fprintf ( stderr, "PRN09:       %s\n", gpsGSA.PRN09 );
    fprintf ( stderr, "PRN10:       %s\n", gpsGSA.PRN10 );
    fprintf ( stderr, "PRN11:       %s\n", gpsGSA.PRN11 );
    fprintf ( stderr, "PRN12:       %s\n", gpsGSA.PRN12 );
    fprintf ( stderr, "PDOP:        %s\n", gpsGSA.pdop );
    fprintf ( stderr, "VDOP:        %s\n", gpsGSA.vdop );
    fprintf ( stderr, "HDOP:        %s\n", gpsGSA.hdop );
    fprintf ( stderr, "EndValue:    %s\n", gpsGSA.endvalue );
#endif

    if ( strcmp ( gpsGSA.currentState, "3" ) != 0 ) {
        return;
    }

    prange = atof ( gpsGSA.pdop );

    if ( prange < 5.0 ) {
        testCycle++;
    }

    //fprintf ( stderr, "DOP:        %f\n", prange );

    return;
}

/*******************************************************************************
**           Function: Get GPGSV for buffer                                   **
**               In:  NULL                                                    **
**               Out: GPGSV Information                                       **
**               Date: 2007-8-1     Author: Owen.Lu                           **
*******************************************************************************/
void GPS::gpsGPGSV()
{
    //fprintf ( stderr, "$GPGSV............\n" );

    char tempStr[30] = {0};
    int starNum = 0;
//    int itemNum = 0;
    int curNum = 0;
    int cycle = 0;

    memset ( &gpsGSV, 0, sizeof ( gpsGSV ) );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
//    itemNum = atoi(tempStr);    /*  save item number */
    strcpy ( gpsGSV.totalnum, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    curNum = atoi ( tempStr ); /*  save current item number */
    strcpy ( gpsGSV.currentnum, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    starNum = atoi ( tempStr ); /*  save star number */
    strcpy ( gpsGSV.totalstar, tempStr );

    cycle = ( starNum - ( ( curNum - 1 ) * 4 ) );   /*check the left star information*/

    if ( cycle > 0 ) {
        memset ( tempStr, 0, 30 );
        getStringEnd ( ',', tempStr );
        strcpy ( gpsGSV.PRV01, tempStr );

        memset ( tempStr, 0, 30 );
        getStringEnd ( ',', tempStr );
        strcpy ( gpsGSV.elevation01, tempStr );

        memset ( tempStr, 0, 30 );
        getStringEnd ( ',', tempStr );
        strcpy ( gpsGSV.depression01, tempStr );

        memset ( tempStr, 0, 30 );

        if ( cycle == 1 ) {                           /*Check is this star last */
            getStringEnd ( '*', tempStr );
        } else {
            getStringEnd ( ',', tempStr );
        }

        strcpy ( gpsGSV.noise01, tempStr );
    }

    if ( cycle > 1 ) {
        memset ( tempStr, 0, 30 );
        getStringEnd ( ',', tempStr );
        strcpy ( gpsGSV.PRV02, tempStr );

        memset ( tempStr, 0, 30 );
        getStringEnd ( ',', tempStr );
        strcpy ( gpsGSV.elevation02, tempStr );

        memset ( tempStr, 0, 30 );
        getStringEnd ( ',', tempStr );
        strcpy ( gpsGSV.depression02, tempStr );

        memset ( tempStr, 0, 30 );

        if ( cycle == 2 ) {              /*Check is this star last */
            getStringEnd ( '*', tempStr );
        } else {
            getStringEnd ( ',', tempStr );
        }

        strcpy ( gpsGSV.noise02, tempStr );
    }

    if ( cycle > 2 ) {
        memset ( tempStr, 0, 30 );
        getStringEnd ( ',', tempStr );
        strcpy ( gpsGSV.PRV03, tempStr );

        memset ( tempStr, 0, 30 );
        getStringEnd ( ',', tempStr );
        strcpy ( gpsGSV.elevation03, tempStr );

        memset ( tempStr, 0, 30 );
        getStringEnd ( ',', tempStr );
        strcpy ( gpsGSV.depression03, tempStr );

        memset ( tempStr, 0, 30 );

        if ( cycle == 3 ) {           /*Check is this star last */
            getStringEnd ( '*', tempStr );
        } else {
            getStringEnd ( ',', tempStr );
        }

        strcpy ( gpsGSV.noise03, tempStr );
    }

    if ( cycle > 3 ) {
        memset ( tempStr, 0, 30 );
        getStringEnd ( ',', tempStr );
        strcpy ( gpsGSV.PRV04, tempStr );

        memset ( tempStr, 0, 30 );
        getStringEnd ( ',', tempStr );
        strcpy ( gpsGSV.elevation04, tempStr );

        memset ( tempStr, 0, 30 );
        getStringEnd ( ',', tempStr );
        strcpy ( gpsGSV.depression04, tempStr );

        memset ( tempStr, 0, 30 );
        getStringEnd ( '*', tempStr );
        strcpy ( gpsGSV.noise04, tempStr );
    }

    //  memset(tempStr, 0, 30);
//    getStringEnd(',', tempStr);
    //tempStr[2] = '\0';
    //strcpy(gpsGSV.endvalue, tempStr);

#if 0
    fprintf ( stderr, "********************$GPGSV***************************\n" );

    fprintf ( stderr, "TOT_Item:        %s\n", gpsGSV.totalnum );

    fprintf ( stderr, "Cur_Item:        %s\n", gpsGSV.currentnum );

    fprintf ( stderr, "TOT_Star:        %s\n", gpsGSV.totalstar );

    fprintf ( stderr, "PRV01:           %s\n", gpsGSV.PRV01 );

    fprintf ( stderr, "Elevation01:     %s\n", gpsGSV.elevation01 );

    fprintf ( stderr, "Depression01:    %s\n", gpsGSV.depression01 );

    fprintf ( stderr, "Noise01:         %s\n", gpsGSV.noise01 );

    fprintf ( stderr, "PRV02:           %s\n", gpsGSV.PRV02 );

    fprintf ( stderr, "Elevation02:     %s\n", gpsGSV.elevation02 );

    fprintf ( stderr, "Depression02:    %s\n", gpsGSV.depression02 );

    fprintf ( stderr, "Noise02:         %s\n", gpsGSV.noise02 );

    fprintf ( stderr, "PRV03:           %s\n", gpsGSV.PRV03 );

    fprintf ( stderr, "Elevation03:     %s\n", gpsGSV.elevation03 );

    fprintf ( stderr, "Depression03:    %s\n", gpsGSV.depression03 );

    fprintf ( stderr, "Noise03:         %s\n", gpsGSV.noise03 );

    fprintf ( stderr, "PRV04:           %s\n", gpsGSV.PRV04 );

    fprintf ( stderr, "Elevation04:     %s\n", gpsGSV.elevation04 );

    fprintf ( stderr, "Depression04:    %s\n", gpsGSV.depression04 );

    fprintf ( stderr, "Noise04:         %s\n", gpsGSV.noise04 );

    fprintf ( stderr, "ENDValue:        %s\n", gpsGSV.endvalue );

#endif
    return;
}

/*******************************************************************************
**           Function: Get GPVTG for buffer                                   **
**               In:  NULL                                                    **
**               Out: GPVTG Information                                       **
**               Date: 2007-8-1     Author: Owen.Lu                           **
*******************************************************************************/
void GPS::gpsGPVTG()
{
    //fprintf ( stderr, "$GPVTG............\n" );
    return;
}

/*******************************************************************************
**           Function: Get LCGLL for buffer                                   **
**               In:  NULL                                                    **
**               Out: LCGLL Information                                       **
**               Date: 2007-8-1     Author: Owen.Lu                           **
*******************************************************************************/
void GPS::gpsLCGLL()
{
    //fprintf ( stderr, "$LCGLL............\n" );
    return;
}

/*******************************************************************************
**           Function: Get LCVTG for buffer                                   **
**               In:  NULL                                                    **
**               Out: LCVTG Information                                       **
**               Date: 2007-8-1     Author: Owen.Lu                           **
*******************************************************************************/
void GPS::gpsLCVTG()
{
    //fprintf ( stderr, "$LCVTG............\n" );
    return;
}

/*******************************************************************************
**           Function: Get PGRME for buffer                                   **
**               In:  NULL                                                    **
**               Out: PGRME Information                                       **
**               Date: 2007-8-1     Author: Owen.Lu                           **
*******************************************************************************/
void GPS::gpsPGRME()
{
    //fprintf ( stderr, "$PGRME............\n" );
    return;
}

/*******************************************************************************
**           Function: Get PGRMT for buffer                                   **
**               In:  NULL                                                    **
**               Out: PGRMT Information                                       **
**               Date: 2007-8-1     Author: Owen.Lu                           **
*******************************************************************************/
void GPS::gpsPGRMT()
{
    //fprintf ( stderr, "$PGRMT............\n" );
    return;
}


/*******************************************************************************
**           Function: Get GPRMC for buffer                                   **
**               In:  NULL                                                    **
**               Out: GPRMC Information                                       **
**               Date: 2007-8-1     Author: Owen.Lu                           **
*******************************************************************************/
void GPS::gpsGPRMC()
{
    //fprintf ( stderr, "$GPRMC............\n" );

    char tempStr[30] = {0};

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsMCSignal.time, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsMCSignal.statu, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsMCSignal.latitude, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsMCSignal.southornorth, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsMCSignal.longitude, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsMCSignal.eastorwest, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsMCSignal.high, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsMCSignal.director, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsMCSignal.date, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsMCSignal.magic, tempStr );

    memset ( tempStr, 0, 30 );
    getStringEnd ( ',', tempStr );
    strcpy ( gpsMCSignal.magicdirtor, tempStr );
#if 0
    fprintf ( stderr, "*********************GPRMC*************************************\n" );
    fprintf ( stderr, "Time        : %s\n", gpsMCSignal.time );
    fprintf ( stderr, "Statue      : %s\n", gpsMCSignal.statu );
    fprintf ( stderr, "Latitude    : %s\n", gpsMCSignal.latitude );
    fprintf ( stderr, "Southornorth: %s\n", gpsMCSignal.southornorth );
    fprintf ( stderr, "Longitude   : %s\n", gpsMCSignal.longitude );
    fprintf ( stderr, "Eastorwest  : %s\n", gpsMCSignal.eastorwest );
    fprintf ( stderr, "High        : %s\n", gpsMCSignal.high );
    fprintf ( stderr, "Director    : %s\n", gpsMCSignal.director );
    fprintf ( stderr, "Date        : %s\n", gpsMCSignal.date );
    fprintf ( stderr, "Magic       : %s\n", gpsMCSignal.magic );
    fprintf ( stderr, "Magicdirtor : %s\n", gpsMCSignal.magicdirtor );
#endif
    int tmp_i;
    float tmp_f1,tmp_f2;

    Data<BaseDataType> yc;
    DataType4 d1, *d2;
    yc.Data = &d1;
    tmp_f1=atof ( gpsMCSignal.latitude );//纬度GPS表示ddmm.mmmm
    tmp_i=tmp_f1/100;//度整数
    tmp_f2=(tmp_f1-tmp_i*100)/60.0;
    d1.Data = tmp_i+tmp_f2;
    user_data.SetData ( YC_4A02, &yc );
 
    tmp_f1=atof ( gpsMCSignal.longitude );//经度表示dddmm.mmmm
    tmp_i=tmp_f1/100;//度整数
    tmp_f2=(tmp_f1-tmp_i*100)/60.0;
    d1.Data = tmp_i+tmp_f2;
    user_data.SetData ( YC_4A01, &yc );

    user_data.GetData ( YC_4A01, &yc );
    d2 = ( DataType4* ) yc.Data;
    PFUNC ( TEM_INFO ,DEBUG_GPS, "longitude:%f \n", d2->Data );
    user_data.GetData ( YC_4A02, &yc );
    d2 = ( DataType4* ) yc.Data;
    PFUNC ( TEM_INFO , DEBUG_GPS, "latitude:%f \n", d2->Data );

    char time_str[16];
    uint16 len = 0;
    char cell[8];

	SYSTEMTIME gps_time,sys_time,st;
	CP56Time2a _t;
	double span;
	CNtpTime ntp,ntp2;

    if ( gpsMCSignal.statu[0] != 'A' ) {
        PFUNC ( TEM_WARNING, DEBUG_GPS, "$gprmc frame data not valid\n" );
        return ;//data not valid
    }

    //gpsMCSignal
    if ( strlen ( &gpsMCSignal.time[0] ) == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_GPS, "$gprmc time string is invalid\n" );
        return ;
    }

    gpsMCSignal.time[10] = 0;

    //hh
    memset ( cell, 0, 8 );
    memcpy ( cell, &gpsMCSignal.time[0], 2 );
    gps_time.wHour = atoi ( cell ) ;
    //mm
    memset ( cell, 0, 8 );
    memcpy ( cell, &gpsMCSignal.time[2], 2 );
    gps_time.wMinute  = atoi ( cell );
    //ss
    memset ( cell, 0, 8 );
    memcpy ( cell, &gpsMCSignal.time[4], 2 );
    gps_time.wSecond  = atoi ( cell );
    //sss
    memset ( cell, 0, 8 );
    memcpy ( cell, &gpsMCSignal.time[7], 3 );
    gps_time.wMilliseconds = atoi ( cell );

    gpsMCSignal.date[7] = 0;
    //DD
    memset ( cell, 0, 8 );
    memcpy ( cell, &gpsMCSignal.date[0], 2 );
    gps_time.wDay = atoi ( cell );

    //MM
    memset ( cell, 0, 8 );
    memcpy ( cell, &gpsMCSignal.date[2], 2 );
    gps_time.wMonth = atoi ( cell );

    //YY
    memset ( cell, 0, 8 );
    memcpy ( cell, &gpsMCSignal.date[4], 2 );
    gps_time.wYear = atoi ( cell )+2000;

	ntp=CNtpTime(gps_time);
	span=8*3600;
	ntp.AddSpan(span);//加上8小时
	st=ntp;
	            		
    timer.GetCP56Time ( &_t );
    sys_time.wHour 			= _t.hour_su & 0x1f;
    sys_time.wMinute  		= _t.minute_iv & 0x7f;
    sys_time.wSecond  		= _t.millisecond / 1000;
    sys_time.wMilliseconds 	= _t.millisecond % 1000;
    sys_time.wDay 			= _t.day_week & 0x1f;
    sys_time.wMonth 		= _t.month;
    sys_time.wYear 			= _t.year+2000;
	ntp2=CNtpTime(sys_time);

	if ( ( abs ( ntp2.Seconds() - ntp.Seconds() ) >= 2 )  ) {
	    _t.millisecond = st.wMilliseconds + st.wSecond * 1000;
	    _t.minute_iv = st.wMinute | ( _t.minute_iv & 0xc0 );
	    _t.hour_su = st.wHour | ( _t.hour_su & 0xe0 );
	    _t.day_week=(st.wDay|(st.wDayOfWeek<<5));
	    _t.month=st.wMonth;
	    _t.year=st.wYear-2000;
	    ////////////////pack data for prosettime func//////////
	    time_str[0] = 1;//num
	    time_str[1] = 0;//num
	    time_str[2] = 0;//id
	    time_str[3] = 0;//id
	    time_str[4] = 7;//len
	    memcpy ( &time_str[5], ( char* ) &_t, 7 );
	    len = 12;
	    ProSetTime ( time_str, len, time_str, &len );
	    PFUNC ( TEM_INFO, DEBUG_GPS, "sys.s=%d  gps.s=%d\n", sys_time.wSecond , st.wSecond );
	    PFUNC ( TEM_INFO, DEBUG_GPS, "sys.m=%d  gps.m=%d\n", sys_time.wMinute , st.wMinute );
	    PFUNC ( TEM_INFO, DEBUG_GPS, "sys.h=%d  gps.h=%d\n" , sys_time.wHour , st.wHour );
	    PFUNC ( TEM_INFO, DEBUG_GPS, "sys.day=%d  gps.day=%d\n" , sys_time.wDay , st.wDay );
	    PFUNC ( TEM_INFO, DEBUG_GPS, "sys.mon=%d  gps.mon=%d\n" , sys_time.wMonth , st.wMonth );
	    PFUNC ( TEM_INFO, DEBUG_GPS, "sys.year=%d  gps.year=%d\n" , sys_time.wYear , st.wYear );
	    PFUNC ( TEM_INFO, DEBUG_GPS, "GPS time in used 2\n" );
	}
	            		    
    PFUNC ( TEM_INFO, DEBUG_GPS, "$gprmc %02d-%02d-%02d %02d:%02d:%02d.%d\n", st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);

}


/*******************************************************************************
**           Function: get a string location                                  **
**               In:   a char buffer                                          **
**               Out: NULL                                                    **
**               Date: 2007-8-1     Author: Owen.Lu                           **
*******************************************************************************/
void GPS::getString ( char strChar[] )
{
    char temp[20] = {0};
    int is_end = 0;

    if ( checkNULL() )
        return;

    while ( gpsMessage[buffEnd][0] != '$' ) {
        gpsMessage[buffEnd][0] = 0;
        gpsMessage[buffEnd][1] = 0;
        buffEnd = ( buffEnd + 1 ) % 1024;

        if ( checkNULL() ) {
            is_end = 1;
            strChar[0] = '\0';
            break;
        }

        //PFUNC(TEM_INFO ,DEBUG_NORMAL,"buf_END:%x %c is_end:%d\n",buffEnd,gpsMessage[buffEnd][0],is_end);
    }

    if ( !is_end ) {
        getStringEnd ( ',' , temp );
        strlwr ( temp );
        strcpy ( strChar, temp );
    }

    return;
}

/*******************************************************************************
**           Function: check next is NULL                                     **
**               In:   a char buffer                                          **
**               Out: NULL                                                    **
**               Date: 2007-8-1     Author: Owen.Lu                           **
*******************************************************************************/
int GPS::checkNULL()
{
    return gpsMessage[buffEnd][1] == 0 ? 1 : 0;
}

/*******************************************************************************
**           Function: get next n char                                        **
**               In:  start position, get length, a char buffer               **
**               Out: a char string                                           **
**               Date: 2007-8-1     Author: Owen.Lu                           **
*******************************************************************************/
void GPS::getNextChar ( int length, char backstr[] )
{
    int ii = 0;

    while ( ii < length ) {
        checkNULL();
        backstr[ii] = gpsMessage[buffEnd][0];
        gpsMessage[buffEnd][0] = 0;
        gpsMessage[buffEnd][1] = 0;
        buffEnd = ( buffEnd + 1 ) % 1024;
        ii++;
    }

    backstr[ii] = 0;
}

void GPS::getStringEnd ( char stopcn, char backstr[] )
{
    int ii = 0;
    int is_end = 0;

    if ( checkNULL() )
        return;

    while ( ( gpsMessage[buffEnd][0] != stopcn ) && ( gpsMessage[buffEnd][0] != '\n' ) ) {
        backstr[ii] = gpsMessage[buffEnd][0];
        gpsMessage[buffEnd][0] = 0;
        gpsMessage[buffEnd][1] = 0;
        buffEnd = ( buffEnd + 1 ) % 1024;
        ii++;

        if ( checkNULL() ) {
            is_end = 1;
            backstr[ii] = '\0';
            break;
        }

    }

    if ( !is_end ) {
        gpsMessage[buffEnd][0] = 0;
        gpsMessage[buffEnd][1] = 0;
        buffEnd = ( buffEnd + 1 ) % 1024;
        backstr[ii] = '\0';
    }
}

//
//	GPS数据包校验和的计算，从$(不包括$)到*(不包括*)之间的数据之和
//	正确，返回1，错误，返回0
//
int GPS::CheckSum( char *sbuff )
{
	int i;
	char *ptr1, *ptr2, cs, css[4];
	ptr1 = &sbuff[1];
	ptr2 = strchr(sbuff, '*');
	cs = 0;
	if(ptr2==NULL)return 0; 
	for(i=0; i< (ptr2-ptr1); i++)
	{
		cs ^= ptr1[i];
	}
	sprintf(css, "%02X", cs);
	ptr2 ++;
	if(strncmp(css, ptr2, 2)!=0)
		return 0;
	else
		return 1;	
}
