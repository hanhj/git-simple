#ifndef UPDATE_PRO__H_
#define UPDATE_PRO__H_
#include "types.h"
#include "user.h"
#include "custom.h"
#define UPDATE_DATA_PACK_SIZE 1000
typedef struct {
    uint16 sw_crc;
    uint16 is_complete;
    uint32 tran_position;  // pck number which got

}__attribute__ ( ( packed ) ) UpDateProPara;


enum UPGRADE_ERR_CODE {
    NO_ERROR_TYPE,
    ERR_NEWEST_VERSION,
    ERR_OLDER_VERSION,
    ERR_FRAME_TYPE,
    ERR_SAVING_DATA,
    ERR_CRC,
    ERR_PCK_NO,
};
#define START_FRAME 0x01
#define HARD_START_FRAME 0x03
#define DATA_FRAME 0x00
#define DATA_FRAME2 0x06
#define END_FRAME 0x02
#define ERR_FRAME 0x15
#define HEART_FRAME 0x04

typedef struct up_timeout {
    uint16 en;
    uint8 frame;
    uint8 frame_flag;
    uint8 byte;
    uint8 byte_flag;
    uint8 begin69;
    uint8 begin17;
    uint8 send_heart_cnt;
    uint8 send_heart_flag;
    uint8 send_heart;
}__attribute__ ( ( packed ) ) UPDATE_PRO_TIMEOUT;

#define MAX_UPDATE_FRAME_SIZE 1024
#define MAX_ACK_FRAME_SIZE 128

typedef struct update_fr {
    uint16 cnt;
    uint16 len;
    char buf[MAX_UPDATE_FRAME_SIZE];
    uint16 ack_len;
    char ack_buf[MAX_ACK_FRAME_SIZE];
} __attribute__ ( ( packed ) ) UpdateFrameBuf;

class UpdatePro
{

public:

    struct {
        UpDateProPara update_pro_para;
        UPDATE_PRO_TIMEOUT update_pro_timeout;
        UpdateFrameBuf update_frame_buf;
        uint16 CurPortId;

    }__attribute__ ( ( packed ) ) member;

    char ReadCfg ( void );
    char WriteCfg ( void );
    UpdatePro ( void );
    char SendHeart ( void );
    char BuildFrame_6917 ( char *data, uint16 len );
    char Parser ( char *buf, uint16 len );
    char CheckFrame ( char *buf , uint16 len );
    char DealUpdataCom ( BUF_MUX *buf, uint16 port_id );
	char DealTimeOut( BUF_MUX *buf, uint16 port_id );
	void SetSendHeartFlag(uint8 flag);

};

extern char DealUpdatePro ( BUF_MUX *buf, uint16 port_id );

#endif
