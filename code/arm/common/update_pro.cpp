#include "update_pro.h"
#include "debug.h"
#include "datatype.h"
#include "string.h"
#include "utils.h"
#include "timer.h"
#include "gb101.h"
UpdatePro updp;

char UpdatePro::DealTimeOut ( BUF_MUX *buf, uint16 port_id )
{
	
    if ( member.update_pro_timeout.en == 1 ) {
		PFUNC(TEM_INFO,DEBUG_PROUPDATE," %d\n",port_id);
        if ( member.update_pro_timeout.frame_flag == 1 ) {//if the frame is timeout ,the process is here
            member.update_pro_timeout.frame++;
		//PFUNC(TEM_INFO,DEBUG_PROUPDATE," %d\n",port_id);
            if ( member.update_pro_timeout.frame > 3 ) {
                member.update_pro_timeout.begin69 = 0;
                member.update_pro_timeout.frame = 0;
                member.update_pro_timeout.frame_flag = 0;
                PFUNC ( TEM_WARNING, DEBUG_PROUPDATE, "frame timeout\n" );
            }
        }

        if ( member.update_pro_timeout.byte_flag == 1 ) {
            member.update_pro_timeout.byte_flag = 0;
        }

        if ( member.update_pro_timeout.send_heart_flag == 1 ) {
            member.update_pro_timeout.send_heart_cnt++;

            if ( member.update_pro_timeout.send_heart_cnt >= 5 ) {
				member.update_pro_timeout.send_heart_cnt=0;
                
                member.update_pro_timeout.send_heart++;

                if ( member.update_pro_timeout.send_heart > 3 ) {
					member.update_pro_timeout.send_heart=0;
                    member.update_pro_timeout.send_heart_flag = 0;
					PFUNC ( TEM_WARNING, DEBUG_PROUPDATE, "heart send over %d\n" , member.update_pro_timeout.send_heart_flag);
                }else{
               	 	SendHeart();
               	 	PFUNC ( TEM_WARNING, DEBUG_PROUPDATE,"send heart\n");
                }
            }

        }
    }

    return 1;
}

void UpdatePro::SetSendHeartFlag(uint8 flag)
{
	static uint8 oldflag=0;
	if(flag!=oldflag){
		oldflag=flag;
		//if(member.update_pro_timeout.send_heart_flag!=flag)
		{
			PFUNC(TEM_INFO,DEBUG_PROUPDATE,"send heart frame %d\n",flag);
			member.update_pro_timeout.send_heart_flag=flag;
			member.update_pro_timeout.send_heart_cnt=0;
			member.update_pro_timeout.send_heart=0;
		}
	}
	 
}
char DealUpdatePro ( BUF_MUX *buf, uint16 port_id )
{
    static uint16 timer_id = 0;
    static uint8 flag = 0;

    if ( flag == 0 ) {//
        if ( timer_id == 0 ) {
            timer.SetTimer ( &timer_id, 1, 0 );
        } else {
            timer.KillTimer ( timer_id );
            timer.SetTimer ( &timer_id, 1, 0 );
        }

        flag = 1;
    }

    if ( timer.IsTimesUp ( timer_id ) ) {//
        updp.DealTimeOut ( buf, port_id );
    }
    updp.DealUpdataCom ( buf, port_id );
    return 1;
}

char UpdatePro::SendHeart ( void )
{
    Data<BaseDataType> data;
	BUF_ADDR_REGIST_TABLE *bart=0;
    uint8 TerId[2];
    char ver[7];
    char fr[16];
	memset(ver,0,7);
    user_data.GetData ( PARA_5139, &data );
    memcpy ( ( char* ) TerId, ( char* ) & ( ( ( ComBasePara* ) data.Data )->Data.LocalId ), 2 );
    user_data.GetData ( PARA_5201, &data );
    memcpy ( ( char* ) ver, ( ( SwVersionPara* ) data.Data )->Data.version, 8 );
	PFUNC(TEM_INFO,DEBUG_PROUPDATE,"send heart id=%d,ver=%s\n",TerId[0]+TerId[1]*256,ver);
    fr[0] = 0x4;

    memcpy ( &fr[1], TerId, 2 );
    memcpy ( &fr[3], ver, 7 );
    BuildFrame_6917 ( fr, 10 );
	bart=protocol.SearchBufAddrRegistTable2(PROUPDATE);
	if(bart==0){
		PFUNC(TEM_ERROR,DEBUG_PROUPDATE,"sendbuf not found\n");
		return 0;
	}
	protocol.AddSendBuf(bart->paddr,member.update_frame_buf.ack_buf,member.update_frame_buf.ack_len);
    return 1;
}

char UpdatePro::ReadCfg ( void )
{
    if ( read_file ( UPGRADE_CFG_PARA, &member.update_pro_para, sizeof ( member.update_pro_para ), "rb", 0 ) < 0 ) {
		PFUNC ( TEM_ERROR, DEBUG_PROUPDATE, "%s file read error\n" ,UPGRADE_CFG_PARA);
        return 0;
    } else {
        return 1;
    }
}

char UpdatePro::WriteCfg ( void )
{
    if ( save_file ( UPGRADE_CFG_PARA, &member.update_pro_para, sizeof ( member.update_pro_para ), "wb", 0 ) < 0 ) {
		PFUNC ( TEM_ERROR, DEBUG_PROUPDATE, "%s file write error\n" ,UPGRADE_CFG_PARA);
        return 0;
    } else {
        return 1;
    }
}


UpdatePro::UpdatePro ( void )
{
    memset ( ( char* ) &member.update_pro_timeout, 0, sizeof ( member.update_pro_timeout ) );
    memset ( ( char* ) &member.update_frame_buf, 0, sizeof ( member.update_frame_buf ) );

    if ( !ReadCfg() ) {
        memset ( ( char* ) &member.update_pro_para, 0, sizeof ( member.update_pro_para ) );
        WriteCfg();
    }
	member.update_pro_timeout.en=1;
}

char UpdatePro::BuildFrame_6917 ( char *data, uint16 len )
{
    uint8 i = 0;
    UpdateFrameBuf *ufb = &member.update_frame_buf;
    ufb->ack_buf[i++] = 0x69;
    memcpy ( &ufb->ack_buf[i], data, len );
    i += len;
    ufb->ack_buf[i] = GetSum ( ( uint8 * ) data, len );
    i++;
    ufb->ack_buf[i++] = 0x17;
    ufb->ack_len = i;
    return 1;

}


char UpdatePro::Parser ( char *buf, uint16 len )
{
    uint8 fr_type = 0xff;
    FILE *pfile = 0;
    uint16 crc = 0;
    uint16 cal_crc = 1;
    uint16 fr_seq = 0;
    uint16 data_len = 0;
    char data2[20];
    uint16 data2len = 0;
    uint32 file_offset = 0;
    char pack[UPDATE_DATA_PACK_SIZE];
    uint8 *pp;
    uint16 file_crc = 0;
	
    if ( !CheckFrame ( buf, len ) ) {
        return 0;
    }

    fr_type = ( uint8 ) buf[1];

    switch ( fr_type ) {

    case START_FRAME:
		
        crc = ( uint8 ) buf[len-4] + (( uint8 ) buf[len-3]) * 256;
        fr_seq = ( uint8 ) buf[2] + ( uint8 ) buf[3] * 256;
		PFUNC ( TEM_INFO, DEBUG_PROUPDATE, "start crc =%x fr_seq=%d\n",crc,fr_seq );
        ReadCfg();
		PFUNC ( TEM_INFO, DEBUG_PROUPDATE, "file read crc =%x is_complete=%d,pos=%d\n",
			member.update_pro_para.sw_crc,
			member.update_pro_para.is_complete,
			member.update_pro_para.tran_position);
        if ( member.update_pro_para.sw_crc != crc ) {
			PFUNC ( TEM_INFO, DEBUG_PROUPDATE, "old crc =%x crc=%x\n",member.update_pro_para.sw_crc ,crc);
			remove(UPGRADE_FILE_NAME);
			if( save_file(UPGRADE_FILE_NAME,&buf[8], 1, "wb+", 0)<0){
				PFUNC ( TEM_ERROR, DEBUG_PROUPDATE, "create file %s error\n",UPGRADE_FILE_NAME);
			}
            member.update_pro_para.sw_crc = crc;
            member.update_pro_para.is_complete = 0;
            member.update_pro_para.tran_position = 0;
            WriteCfg();
        }else if(!member.update_pro_para.is_complete){
			PFUNC ( TEM_INFO, DEBUG_PROUPDATE, "continue download from pos %d\n",member.update_pro_para.tran_position);
		}else{
			PFUNC ( TEM_ERROR, DEBUG_PROUPDATE, "the file is equal,%d\n",member.update_pro_para.is_complete);
			data2[data2len] = ERR_FRAME;
            data2len++;
            memcpy ( &data2[data2len], ( char* ) &fr_seq, 2 );
            data2len += 2;
            data2[data2len++] = ERR_NEWEST_VERSION;
            BuildFrame_6917 ( data2, data2len );
			return 0;
		
		}

        data2[data2len] = fr_type;

        data2len++;
        memcpy ( &data2[data2len], ( char* ) &fr_seq, 2 );
        data2len += 2;
        memcpy ( &data2[data2len], ( char* ) &member.update_pro_para.sw_crc, 2 );
        data2len += 2;
        memcpy ( &data2[data2len], ( char* ) &member.update_pro_para.tran_position, 2 );
        data2len += 2;
        BuildFrame_6917 ( data2, data2len );
        break;

    case HARD_START_FRAME:
		PFUNC ( TEM_INFO, DEBUG_PROUPDATE, "hard start\n" );
		remove(UPGRADE_FILE_NAME);
		
		save_file(UPGRADE_FILE_NAME,&buf[8], 1, "wb+", 0);
        crc = ( uint8 ) buf[len-4] + ( uint8 ) buf[len-3] * 256;
        fr_seq = ( uint8 ) buf[2] + ( uint8 ) buf[3] * 256;
		PFUNC ( TEM_INFO, DEBUG_PROUPDATE, "start crc =%x fr_seq=%d\n",crc,fr_seq );        
        member.update_pro_para.sw_crc = crc;
        member.update_pro_para.is_complete = 0;
        member.update_pro_para.tran_position = 0;
        WriteCfg();
        data2[data2len] = fr_type;
        data2len++;
        memcpy ( &data2[data2len], ( char* ) &fr_seq, 2 );
        data2len += 2;
        memcpy ( &data2[data2len], ( char* ) &member.update_pro_para.sw_crc, 2 );
        data2len += 2;
        memcpy ( &data2[data2len], ( char* ) &member.update_pro_para.tran_position, 2 );
        data2len += 2;
        BuildFrame_6917 ( data2, data2len );
        break;

    case DATA_FRAME:
	case DATA_FRAME2:
		//PFUNC ( TEM_INFO, DEBUG_PROUPDATE, "download data\n" );
        fr_seq = ( uint8 ) buf[2] + ( uint8 ) buf[3] * 256;

        if ( fr_seq != member.update_pro_para.tran_position ) {
            PFUNC ( TEM_ERROR, DEBUG_PROUPDATE, "data frame seq error\n" );
            data2[data2len] = ERR_FRAME;
            data2len++;
            memcpy ( &data2[data2len], ( char* ) &fr_seq, 2 );
            data2len += 2;
            data2[data2len++] = ERR_PCK_NO;
            BuildFrame_6917 ( data2, data2len );
			return 0;
        }
        data_len = ( uint8 ) buf[4] + ( uint8 ) buf[5] * 256;
        crc = ( uint8 ) buf[len-4] + ( uint8 ) buf[len-3] * 256;
        cal_crc = crc16 ( ( uint8* ) &buf[6], data_len - 2 );
        if ( cal_crc != crc ) {
			PFUNC ( TEM_ERROR, DEBUG_PROUPDATE, "crc error:cal_crc=%x crc=%x\n",cal_crc,crc );
            data2[data2len] = ERR_FRAME;
            data2len++;
            memcpy ( &data2[data2len], ( char* ) &fr_seq, 2 );
            data2len += 2;
            data2[data2len++] = ERR_CRC;
            BuildFrame_6917 ( data2, data2len );
            return 0;
        } else {
            file_offset = fr_seq * UPDATE_DATA_PACK_SIZE;
			
            if ( save_file ( UPGRADE_FILE_NAME, &buf[6], data_len - 2, "rb+", file_offset ) < 0 ) {
            	if(save_file ( UPGRADE_FILE_NAME, &buf[6], data_len - 2, "wb+", file_offset )<0){
                data2[data2len] = ERR_FRAME;
                data2len++;
                memcpy ( &data2[data2len], ( char* ) &fr_seq, 2 );
                data2len += 2;
                data2[data2len++] = ERR_SAVING_DATA;
            	BuildFrame_6917 ( data2, data2len );
            	}else{
            		PFUNC ( TEM_INFO, DEBUG_PROUPDATE, "save file suc\n" );
            	}
            } else {
                data2[data2len] = fr_type;
                data2len++;
                memcpy ( &data2[data2len], ( char* ) &fr_seq, 2 );
                data2len += 2;
                BuildFrame_6917 ( data2, data2len );
                member.update_pro_para.tran_position += 1;
				WriteCfg();
				PFUNC ( TEM_INFO, DEBUG_PROUPDATE, "save file ok next seq=%d\n", member.update_pro_para.tran_position);
            }
			mdelay(5);
			//return 1;
            break;
		}
    case END_FRAME:
			PFUNC ( TEM_INFO, DEBUG_PROUPDATE, "download end\n" );
            crc = ( uint8 ) buf[len-4] + ( uint8 ) buf[len-3] * 256;

            pfile = fopen ( UPGRADE_FILE_NAME, "rb" );
            file_crc = 0;
            ASSERT ( NULL != pfile );

            while ( ( data_len = fread ( pack, 1, UPDATE_DATA_PACK_SIZE, pfile ) ) > 0 ) {
				//PFUNC ( TEM_INFO, DEBUG_PROUPDATE, "file_len=%d\n" ,data_len);
				pp = (uint8*)pack;
                while ( data_len-- ) {
                    
                    file_crc = _crc_xmodem_update ( file_crc, *pp++ );
                }
            }

            fclose ( pfile );
			PFUNC ( TEM_INFO, DEBUG_PROUPDATE, "crc=%x file_crc=%x\n" ,crc,file_crc);
            if ( file_crc != member.update_pro_para.sw_crc  ) {
                data2[data2len] = ERR_FRAME;
                data2len++;
                memcpy ( &data2[data2len], ( char* ) &fr_seq, 2 );
                data2len += 2;
                data2[data2len++] = ERR_CRC;
                BuildFrame_6917 ( data2, data2len );
                return 0;
            } else {
                member.update_pro_para.is_complete = 1;
				WriteCfg();
				mdelay(100);
				//send_cmd ( STDOUT_FILENO, UPGRADE );
				send_cmd ( process_fp[1], UPGRADE );
            }

            break;

        default:
			PFUNC ( TEM_WARNING, DEBUG_PROUPDATE, "frame type error %d\n" ,ERR_FRAME);
            data2[data2len] = ERR_FRAME;
            data2len++;
            memcpy ( &data2[data2len], ( char* ) &fr_seq, 2 );
            data2len += 2;
            data2[data2len++] = ERR_FRAME_TYPE;
            BuildFrame_6917 ( data2, data2len );
            return 0;
            break;
    }
return 1;    
}

char UpdatePro::CheckFrame ( char *buf , uint16 len )
{

    uint8 sum = 0;
    sum = GetSum ( ( uint8* ) & buf[1], len - 3 );

    if ( sum == ( uint8 ) buf[len-2] ) {
        return 1;
    }

    PFUNC ( TEM_INFO, DEBUG_PROUPDATE, "frame check sum error 0x%x\n", sum );

    return 0;
}

char UpdatePro::DealUpdataCom ( BUF_MUX *buf, uint16 port_id )
{
	uint16 bytes=0;
    //PFUNC ( TEM_INFO,DEBUG_PRO101, "buf->recv.out_cnt=%d, buf->recv.in_cnt=%d,tab[%d]=%x\n", buf->recv.out_cnt, buf->recv.in_cnt ,port_id,tab);
    UPDATE_PRO_TIMEOUT *tab = &member.update_pro_timeout;
    UpdateFrameBuf *ufb = &member.update_frame_buf;

    while ( buf->recv.out_cnt != buf->recv.in_cnt ) {
        if ( tab->begin69 == 0  ) {
            if ( buf->recv.buf[buf->recv.out_cnt] == 0x69 ) {
                tab->begin69 = 1;
                buf->frame.in_cnt = 0;
                //PFUNC ( TEM_INFO, DEBUG_PROUPDATE, "69 frame\n" );
            }
        }

        if ( tab->begin69 == 1 ) {
            ufb->buf[ufb->cnt++] = buf->recv.buf[buf->recv.out_cnt];
            //PFUNC(TEM_WARNING,DEBUG_PRO101,"frame byte %02x begin68=%d,begin10=%d\n",buf->recv.buf[buf->recv.out_cnt],tab->begin68,tab->begin10);

            if ( ufb->cnt == 6 ) {

                if ( ( uint8 ) ufb->buf[1] == 1 || ( uint8 ) ufb->buf[1] == 3  ) {//download begin
                    ufb->len = 10;
                } else if ( ( uint8 ) ufb->buf[1] == 0||( uint8 ) ufb->buf[1] == 6 ) {//downloading
                	
                    //ufb->len = UPDATE_DATA_PACK_SIZE + 10;
					ufb->len = (uint8) ufb->buf[4]+(uint8) ufb->buf[5]*256 + 8;
					
                } else if (  ( uint8 ) ufb->buf[1] == 2 ) {//download complete
                    ufb->len = 10;
                }

                //PFUNC ( TEM_INFO, DEBUG_PROUPDATE, "69 frame len =%d\n" , ufb->len );
            }

            if ( ufb->cnt == ufb->len ) {

                if ( buf->recv.buf[buf->recv.out_cnt] == 0x17 ) {
                    //PFUNC ( TEM_INFO, DEBUG_PROUPDATE, "69 frame ok\n" );
                    tab->begin69 = 0;
                    updp.Parser ( ufb->buf, ufb->len );
                    member.CurPortId = port_id;
                    ufb->cnt = 0;

                    if ( member.update_frame_buf.ack_len != 0 ) {
                        protocol.AddSendBuf ( buf, member.update_frame_buf.ack_buf, member.update_frame_buf.ack_len );
                        member.update_frame_buf.ack_len = 0;
                    }
                }
            }

            if ( ufb->len > UPDATE_DATA_PACK_SIZE + 10 ) {
                tab->begin69 = 0;
                ufb->cnt = 0;
                PFUNC ( TEM_INFO, DEBUG_PROUPDATE, "frame len over flow %d \n" ,UPDATE_DATA_PACK_SIZE + 10);
            }
        }

        buf->recv.out_cnt++;

        if ( buf->recv.out_cnt >= BUFFER_SIZE ) {
            buf->recv.out_cnt = 0;
        }

        bytes++;

        if ( bytes > UPDATE_DATA_PACK_SIZE + 10 ) {
            bytes = 0;
            PFUNC ( TEM_WARNING, DEBUG_PROUPDATE, "break cycle %d\n", bytes );
            break;
        }

        if (  tab->begin69 == 0   ) { //其他情况，由超时处理.
            tab->frame = 0;
            tab->frame_flag = 0;
            PFUNC ( TEM_INFO, DEBUG_PROUPDATE, "update no wait\n" );
        } else {
            tab->frame_flag = 1;
        }
    }

    return 1;
}
