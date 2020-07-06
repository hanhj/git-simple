#ifndef __GPS_H__
#define __GPS_H__
#include "types.h"

typedef struct tagGPRMC {
    char time[20];
    char statu[5];
    char latitude[20];
    char southornorth[5];
    char longitude[20];
    char eastorwest[5];
    char high[10];
    char director[10];
    char date[20];
    char magic[10];
    char magicdirtor[5];
}__attribute__ ( ( packed ) )GPRMCHead, * LP_GPRMC_Head;

typedef struct tagGPGSA {
    char mode[5];
    char currentState[5];
    char PRN01[5];
    char PRN02[5];
    char PRN03[5];
    char PRN04[5];
    char PRN05[5];
    char PRN06[5];
    char PRN07[5];
    char PRN08[5];
    char PRN09[5];
    char PRN10[5];
    char PRN11[5];
    char PRN12[5];
    char pdop[10];
    char vdop[10];
    char hdop[10];
    char endvalue[50];
}__attribute__ ( ( packed ) )GPGSAHead, *LP_GPGSA_Head;

typedef struct tagGPGSV {
    char totalnum[5];
    char currentnum[5];
    char totalstar[5];
    char PRV01[5];
    char elevation01[10];
    char depression01[10];
    char noise01[10];
    char PRV02[5];
    char elevation02[10];
    char depression02[10];
    char noise02[10];
    char PRV03[5];
    char elevation03[10];
    char depression03[10];
    char noise03[10];
    char PRV04[5];
    char elevation04[10];
    char depression04[10];
    char noise04[10];
    char endvalue[10];
}__attribute__ ( ( packed ) )GPGSVHead, *LP_GPGSV_Head;

typedef struct tagGPCGA {
    char time[20];
    char latitude[20];
    char southornorth[5];
    char longitude[20];
    char eastorwest[5];
    char mode[5];
    char satellites[10];
    char hdop[10];
    char high[10];
    char highunit[5];
    char geosep [10];
    char geosepunit [5];
    char agecorr[10];

}__attribute__ ( ( packed ) )GPCGAHead, *LP_GPCGA_Head;

class GPS
{

public:
   
	 GPCGAHead gpsCga;
	 GPGSAHead gpsGSA;
	 GPGSVHead gpsGSV;
	 GPRMCHead gpsMCSignal;
	  GPS();
    int analyseGPSProc ( char *, int16 );
    int  CheckSum( char *sbuff );
private:
    char gpsMessage[1030][2] ;
    unsigned int testCycle ;
    volatile int buffHead ;
    volatile int buffEnd ;
    void getNextChar ( int length, char backstr[] );
    void getString ( char strChar[] );
    int checkNULL();
    void getStringEnd ( char stopcn, char backstr[] );
    void getBetweenChar ( char input[], char backstr[], char stopcn );
    void getSystemTime ( char stime[], char sdate[] );

    void gpsALM();
    void gpsPGRMI();
    void gpsGPALM();
    void gpsPGRMO();
    void gpsGPLM();
    void gpsGPGGA();
    void gpsGPGSA();
    void gpsGPGSV();
    void gpsGPRMC();
    void gpsGPVTG();
    void gpsLCGLL();
    void gpsLCVTG();
    void gpsPGRME();
    void gpsPGRMT();
    void showPass();
};
extern char deal_progps( BUF_MUX *buf, uint16 port_id );

extern GPS gps;
#endif
