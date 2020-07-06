#include <string.h>
#include <stdlib.h>
#include "datatype.h"
#include "hisdata.h"
#include "custom.h"
#include "utils.h"
#include "debug.h"
#define FILE_INDEX_TABLE_NAME EVENT_ROOT "fileindex.dat"
static FILE_INDEX_TABLE his_db[] = {
    {HARD_YX_EVENT,                  SYS_7010 ,      EVENT_ROOT "yx.dat", 0,0,256},
    {SOFT_YX_EVENT,                  SYS_7011 ,   EVENT_ROOT "warning.dat", 0,0,256},
    {POWER_OFF_EVENT,                SYS_7012 ,     EVENT_ROOT "power_off.dat", 0,0,10},
    {ACT_EVENT,                      SYS_7013 ,   EVENT_ROOT "act_event.dat", 0,0,50},
    {TERMINAL_FAULT_EVENT ,           SYS_7014 ,      EVENT_ROOT "terminal_fault.dat", 0,0,10},
};
HisData::HisData()
{

    	read_file(FILE_INDEX_TABLE_NAME,&his_db,sizeof(his_db),"rb",0);
}

FILE_INDEX_TABLE* HisData::FindHisDbById ( int32 Id )
{
    uint16 i;

    for ( i = 0;i < sizeof ( his_db ) / sizeof ( FILE_INDEX_TABLE );i++ ) {
        if ( his_db[i].Id == Id )
            return &his_db[i];
    }

    return NULL;
}

FILE_INDEX_TABLE* HisData::FindHisDbByType ( int16 type )
{
    uint16 i;

    for ( i = 0;i < sizeof ( his_db ) / sizeof ( FILE_INDEX_TABLE );i++ ) {
        if ( his_db[i].type == type )
            return &his_db[i];
    }

    return NULL;
}

int16 HisData::CleanHisData ( int32 Id )
{
    FILE_INDEX_TABLE*db;
    db = FindHisDbById ( Id );
    remove(db->Name);
    return 0;
}

int HisData::SaveHisRecord ( void *p, uint16 len, int16 type )
{
    uint32 offset;
    int ret;
    FILE_INDEX_TABLE *his_db_index;
    his_db_index = FindHisDbByType ( type );

    if ( his_db_index != NULL ) {
    	offset=his_db_index->Count*len;
    	if((ret=save_file(his_db_index->Name,p,len,"r+",offset))<0)
    		{
    			ret=save_file(his_db_index->Name,p,len,"wb",offset);
    			if(ret<0)
		    		{
		    		return 0;
		    		}
    		}
    	his_db_index->Len = file_size(his_db_index->Name);
    	PFUNC ( TEM_INFO , DEBUG_STATIS, "write:%s len:%d count:%d\n", his_db_index->Name ,his_db_index->Len,his_db_index->Count) ;
    	his_db_index->Count+=1;
    	if(his_db_index->Count>=his_db_index->MaxCount)
    		his_db_index->Count=0;
    		
    	ret=save_file(FILE_INDEX_TABLE_NAME,&his_db,sizeof(his_db),"wb",0);
		if(ret<0){
    		return 0;
    	}
    	return 1;
  }
    PFUNC ( TEM_WARNING , DEBUG_STATIS, "can't find :%s\n", his_db_index->Name ) ;
   return 0;
}

int16 HisData::ReadHisRecord ( void *p, int32 offset, uint16 len, int32 Id )
{
    FILE_INDEX_TABLE *his_db_index;
    int16 ret;
    his_db_index = FindHisDbById ( Id );

    if ( his_db_index != NULL ) {
    	if((ret=read_file(his_db_index->Name,p,len,"rb",offset))>=0){
    	return ret;
    	}
		else
    PFUNC ( TEM_WARNING , DEBUG_STATIS, "can't read :%s\n", his_db_index->Name ) ;
    return -1;
  	}
    PFUNC ( TEM_WARNING , DEBUG_STATIS, "can't find :%s\n", his_db_index->Name ) ;
  	return -1;

}
