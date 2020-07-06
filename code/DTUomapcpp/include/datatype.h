/*
 * datatype.h
 *
 *  Created on: 2013-8-6
 *      Author: Administrator
 */

#ifndef DATATYPE_H_
#define DATATYPE_H_
#include "types.h"
#include "user.h"
#include "os.h"
#include "dspdatatype.h"
#define  YXTAB 0
#define  YCTAB 1
#define  PARATAB 2
#define  YKTAB 3
#define MAXYXDATA 1000	//
#define MAXYCDATA 500	//
#define MAXYKDATA 50	//
#define MAXPARADATA 500	//

/**
* /brief 遥信类数据标识
*/
#define YX_0001  0x1   //线路1开关合位
#define YX_0002  0x2   //线路1开关分位
#define YX_0003  0x3   //线路1开关储能位
#define YX_0004  0x4   //线路1备用遥信1
#define YX_0005  0x5   //线路1备用遥信2
#define YX_0006  0x6   //线路1备用遥信3
#define YX_0007  0x7   //线路1一段速断故障信号
#define YX_0008  0x8   //线路1二段过流故障信号
#define YX_0009  0x9   //线路1三段过负荷故障信号
#define YX_000A  0xA   //线路1相间短路故障
#define YX_000B  0xB   //线路1单相接地故障信号
#define YX_000C  0xC   //线路1瞬时故障信号
#define YX_000D  0xD   //线路1永久故障信号
#define YX_000E  0xE   //线路1零序电压故障信号
#define YX_000F  0xF   //线路1零序电流故障信号
#define YX_0010  0x10   //线路1一段保护动作信号
#define YX_0011  0x11   //线路1二段保护动作信号
#define YX_0012  0x12   //线路1三段保护动作信号
#define YX_0013  0x13   //线路1单相接地保护动作信号
#define YX_0014  0x14   //线路1PT断线
#define YX_0015  0x15   //线路1CT断线
#define YX_0016  0x16   //线路1本地合闸
#define YX_0017  0x17   //线路1本地分闸
#define YX_0018  0x18   //线路2开关合位
#define YX_0019  0x19   //线路2开关分位
#define YX_001A  0x1A   //线路2开关储能位
#define YX_001B  0x1B   //线路2备用遥信1
#define YX_001C  0x1C   //线路2备用遥信2
#define YX_001D  0x1D   //线路2备用遥信3
#define YX_001E  0x1E   //线路2一段速断故障信号
#define YX_001F  0x1F   //线路2二段过流故障信号
#define YX_0020  0x20   //线路2三段过负荷故障信号
#define YX_0021  0x21   //线路2相间短路故障
#define YX_0022  0x22   //线路2单相接地故障信号
#define YX_0023  0x23   //线路2瞬时故障信号
#define YX_0024  0x24   //线路2永久故障信号
#define YX_0025  0x25   //线路2零序电压故障信号
#define YX_0026  0x26   //线路2零序电流故障信号
#define YX_0027  0x27   //线路2一段保护动作信号
#define YX_0028  0x28   //线路2二段保护动作信号
#define YX_0029  0x29   //线路2三段保护动作信号
#define YX_002A  0x2A   //线路2单相接地保护动作信号
#define YX_002B  0x2B   //线路2PT断线
#define YX_002C  0x2C   //线路2CT断线
#define YX_002D  0x2D   //线路2本地合闸
#define YX_002E  0x2E   //线路2本地分闸
#define YX_002F  0x2F   //线路3开关合位
#define YX_0030  0x30   //线路3开关分位
#define YX_0031  0x31   //线路3开关储能位
#define YX_0032  0x32   //线路3备用遥信1
#define YX_0033  0x33   //线路3备用遥信2
#define YX_0034  0x34   //线路3备用遥信3
#define YX_0035  0x35   //线路3一段速断故障信号
#define YX_0036  0x36   //线路3二段过流故障信号
#define YX_0037  0x37   //线路3三段过负荷故障信号
#define YX_0038  0x38   //线路3相间短路故障
#define YX_0039  0x39   //线路3单相接地故障信号
#define YX_003A  0x3A   //线路3瞬时故障信号
#define YX_003B  0x3B   //线路3永久故障信号
#define YX_003C  0x3C   //线路3零序电压故障信号
#define YX_003D  0x3D   //线路3零序电流故障信号
#define YX_003E  0x3E   //线路3一段保护动作信号
#define YX_003F  0x3F   //线路3二段保护动作信号
#define YX_0040  0x40   //线路3三段保护动作信号
#define YX_0041  0x41   //线路3单相接地保护动作信号
#define YX_0042  0x42   //线路3PT断线
#define YX_0043  0x43   //线路3CT断线
#define YX_0044  0x44   //线路3本地合闸
#define YX_0045  0x45   //线路3本地分闸
#define YX_0046  0x46   //线路4开关合位
#define YX_0047  0x47   //线路4开关分位
#define YX_0048  0x48   //线路4开关储能位
#define YX_0049  0x49   //线路4备用遥信1
#define YX_004A  0x4A   //线路4备用遥信2
#define YX_004B  0x4B   //线路4备用遥信3
#define YX_004C  0x4C   //线路4一段速断故障信号
#define YX_004D  0x4D   //线路4二段过流故障信号
#define YX_004E  0x4E   //线路4三段过负荷故障信号
#define YX_004F  0x4F   //线路4相间短路故障
#define YX_0050  0x50   //线路4单相接地故障信号
#define YX_0051  0x51   //线路4瞬时故障信号
#define YX_0052  0x52   //线路4永久故障信号
#define YX_0053  0x53   //线路4零序电压故障信号
#define YX_0054  0x54   //线路4零序电流故障信号
#define YX_0055  0x55   //线路4一段保护动作信号
#define YX_0056  0x56   //线路4二段保护动作信号
#define YX_0057  0x57   //线路4三段保护动作信号
#define YX_0058  0x58   //线路4单相接地保护动作信号
#define YX_0059  0x59   //线路4PT断线
#define YX_005A  0x5A   //线路4CT断线
#define YX_005B  0x5B   //线路4本地合闸
#define YX_005C  0x5C   //线路4本地分闸
#define YX_005D  0x5D   //线路5开关合位
#define YX_005E  0x5E   //线路5开关分位
#define YX_005F  0x5F   //线路5开关储能位
#define YX_0060  0x60   //线路5备用遥信1
#define YX_0061  0x61   //线路5备用遥信2
#define YX_0062  0x62   //线路5备用遥信3
#define YX_0063  0x63   //线路5一段速断故障信号
#define YX_0064  0x64   //线路5二段过流故障信号
#define YX_0065  0x65   //线路5三段过负荷故障信号
#define YX_0066  0x66   //线路5相间短路故障
#define YX_0067  0x67   //线路5单相接地故障信号
#define YX_0068  0x68   //线路5瞬时故障信号
#define YX_0069  0x69   //线路5永久故障信号
#define YX_006A  0x6A   //线路5零序电压故障信号
#define YX_006B  0x6B   //线路5零序电流故障信号
#define YX_006C  0x6C   //线路5一段保护动作信号
#define YX_006D  0x6D   //线路5二段保护动作信号
#define YX_006E  0x6E   //线路5三段保护动作信号
#define YX_006F  0x6F   //线路5单相接地保护动作信号
#define YX_0070  0x70   //线路5PT断线
#define YX_0071  0x71   //线路5CT断线
#define YX_0072  0x72   //线路5本地合闸
#define YX_0073  0x73   //线路5本地分闸
#define YX_0074  0x74   //线路6开关合位
#define YX_0075  0x75   //线路6开关分位
#define YX_0076  0x76   //线路6开关储能位
#define YX_0077  0x77   //线路6备用遥信1
#define YX_0078  0x78   //线路6备用遥信2
#define YX_0079  0x79   //线路6备用遥信3
#define YX_007A  0x7A   //线路6一段速断故障信号
#define YX_007B  0x7B   //线路6二段过流故障信号
#define YX_007C  0x7C   //线路6三段过负荷故障信号
#define YX_007D  0x7D   //线路6相间短路故障
#define YX_007E  0x7E   //线路6单相接地故障信号
#define YX_007F  0x7F   //线路6瞬时故障信号
#define YX_0080  0x80   //线路6永久故障信号
#define YX_0081  0x81   //线路6零序电压故障信号
#define YX_0082  0x82   //线路6零序电流故障信号
#define YX_0083  0x83   //线路6一段保护动作信号
#define YX_0084  0x84   //线路6二段保护动作信号
#define YX_0085  0x85   //线路6三段保护动作信号
#define YX_0086  0x86   //线路6单相接地保护动作信号
#define YX_0087  0x87   //线路6PT断线
#define YX_0088  0x88   //线路6CT断线
#define YX_0089  0x89   //线路6本地合闸
#define YX_008A  0x8A   //线路6本地分闸
#define YX_008B  0x8B   //线路7开关合位
#define YX_008C  0x8C   //线路7开关分位
#define YX_008D  0x8D   //线路7开关储能位
#define YX_008E  0x8E   //线路7备用遥信1
#define YX_008F  0x8F   //线路7备用遥信2
#define YX_0090  0x90   //线路7备用遥信3
#define YX_0091  0x91   //线路7一段速断故障信号
#define YX_0092  0x92   //线路7二段过流故障信号
#define YX_0093  0x93   //线路7三段过负荷故障信号
#define YX_0094  0x94   //线路7相间短路故障
#define YX_0095  0x95   //线路7单相接地故障信号
#define YX_0096  0x96   //线路7瞬时故障信号
#define YX_0097  0x97   //线路7永久故障信号
#define YX_0098  0x98   //线路7零序电压故障信号
#define YX_0099  0x99   //线路7零序电流故障信号
#define YX_009A  0x9A   //线路7一段保护动作信号
#define YX_009B  0x9B   //线路7二段保护动作信号
#define YX_009C  0x9C   //线路7三段保护动作信号
#define YX_009D  0x9D   //线路7单相接地保护动作信号
#define YX_009E  0x9E   //线路7PT断线
#define YX_009F  0x9F   //线路7CT断线
#define YX_00A0  0xA0   //线路7本地合闸
#define YX_00A1  0xA1   //线路7本地分闸
#define YX_00A2  0xA2   //线路8开关合位
#define YX_00A3  0xA3   //线路8开关分位
#define YX_00A4  0xA4   //线路8开关储能位
#define YX_00A5  0xA5   //线路8备用遥信1
#define YX_00A6  0xA6   //线路8备用遥信2
#define YX_00A7  0xA7   //线路8备用遥信3
#define YX_00A8  0xA8   //线路8一段速断故障信号
#define YX_00A9  0xA9   //线路8二段过流故障信号
#define YX_00AA  0xAA   //线路8三段过负荷故障信号
#define YX_00AB  0xAB   //线路8相间短路故障
#define YX_00AC  0xAC   //线路8单相接地故障信号
#define YX_00AD  0xAD   //线路8瞬时故障信号
#define YX_00AE  0xAE   //线路8永久故障信号
#define YX_00AF  0xAF   //线路8零序电压故障信号
#define YX_00B0  0xB0   //线路8零序电流故障信号
#define YX_00B1  0xB1   //线路8一段保护动作信号
#define YX_00B2  0xB2   //线路8二段保护动作信号
#define YX_00B3  0xB3   //线路8三段保护动作信号
#define YX_00B4  0xB4   //线路8单相接地保护动作信号
#define YX_00B5  0xB5   //线路8PT断线
#define YX_00B6  0xB6   //线路8CT断线
#define YX_00B7  0xB7   //线路8本地合闸
#define YX_00B8  0xB8   //线路8本地分闸
#define YX_00B9  0xB9   //线路9开关合位
#define YX_00BA  0xBA   //线路9开关分位
#define YX_00BB  0xBB   //线路9开关储能位
#define YX_00BC  0xBC   //线路9备用遥信1
#define YX_00BD  0xBD   //线路9备用遥信2
#define YX_00BE  0xBE   //线路9备用遥信3
#define YX_00BF  0xBF   //线路9一段速断故障信号
#define YX_00C0  0xC0   //线路9二段过流故障信号
#define YX_00C1  0xC1   //线路9三段过负荷故障信号
#define YX_00C2  0xC2   //线路9相间短路故障
#define YX_00C3  0xC3   //线路9单相接地故障信号
#define YX_00C4  0xC4   //线路9瞬时故障信号
#define YX_00C5  0xC5   //线路9永久故障信号
#define YX_00C6  0xC6   //线路9零序电压故障信号
#define YX_00C7  0xC7   //线路9零序电流故障信号
#define YX_00C8  0xC8   //线路9一段保护动作信号
#define YX_00C9  0xC9   //线路9二段保护动作信号
#define YX_00CA  0xCA   //线路9三段保护动作信号
#define YX_00CB  0xCB   //线路9单相接地保护动作信号
#define YX_00CC  0xCC   //线路9PT断线
#define YX_00CD  0xCD   //线路9CT断线
#define YX_00CE  0xCE   //线路9本地合闸
#define YX_00CF  0xCF   //线路9本地分闸
#define YX_00D0  0xD0   //线路10开关合位
#define YX_00D1  0xD1   //线路10开关分位
#define YX_00D2  0xD2   //线路10开关储能位
#define YX_00D3  0xD3   //线路10备用遥信1
#define YX_00D4  0xD4   //线路10备用遥信2
#define YX_00D5  0xD5   //线路10备用遥信3
#define YX_00D6  0xD6   //线路10一段速断故障信号
#define YX_00D7  0xD7   //线路10二段过流故障信号
#define YX_00D8  0xD8   //线路10三段过负荷故障信号
#define YX_00D9  0xD9   //线路10相间短路故障
#define YX_00DA  0xDA   //线路10单相接地故障信号
#define YX_00DB  0xDB   //线路10瞬时故障信号
#define YX_00DC  0xDC   //线路10永久故障信号
#define YX_00DD  0xDD   //线路10零序电压故障信号
#define YX_00DE  0xDE   //线路10零序电流故障信号
#define YX_00DF  0xDF   //线路10一段保护动作信号
#define YX_00E0  0xE0   //线路10二段保护动作信号
#define YX_00E1  0xE1   //线路10三段保护动作信号
#define YX_00E2  0xE2   //线路10单相接地保护动作信号
#define YX_00E3  0xE3   //线路10PT断线
#define YX_00E4  0xE4   //线路10CT断线
#define YX_00E5  0xE5   //线路10本地合闸
#define YX_00E6  0xE6   //线路10本地分闸
#define YX_00E7  0xE7   //线路11开关合位
#define YX_00E8  0xE8   //线路11开关分位
#define YX_00E9  0xE9   //线路11开关储能位
#define YX_00EA  0xEA   //线路11备用遥信1
#define YX_00EB  0xEB   //线路11备用遥信2
#define YX_00EC  0xEC   //线路11备用遥信3
#define YX_00ED  0xED   //线路11一段速断故障信号
#define YX_00EE  0xEE   //线路11二段过流故障信号
#define YX_00EF  0xEF   //线路11三段过负荷故障信号
#define YX_00F0  0xF0   //线路11相间短路故障
#define YX_00F1  0xF1   //线路11单相接地故障信号
#define YX_00F2  0xF2   //线路11瞬时故障信号
#define YX_00F3  0xF3   //线路11永久故障信号
#define YX_00F4  0xF4   //线路11零序电压故障信号
#define YX_00F5  0xF5   //线路11零序电流故障信号
#define YX_00F6  0xF6   //线路11一段保护动作信号
#define YX_00F7  0xF7   //线路11二段保护动作信号
#define YX_00F8  0xF8   //线路11三段保护动作信号
#define YX_00F9  0xF9   //线路11单相接地保护动作信号
#define YX_00FA  0xFA   //线路11PT断线
#define YX_00FB  0xFB   //线路11CT断线
#define YX_00FC  0xFC   //线路11本地合闸
#define YX_00FD  0xFD   //线路11本地分闸
#define YX_00FE  0xFE   //线路12开关合位
#define YX_00FF  0xFF   //线路12开关分位
#define YX_0100  0x100   //线路12开关储能位
#define YX_0101  0x101   //线路12备用遥信1
#define YX_0102  0x102   //线路12备用遥信2
#define YX_0103  0x103   //线路12备用遥信3
#define YX_0104  0x104   //线路12一段速断故障信号
#define YX_0105  0x105   //线路12二段过流故障信号
#define YX_0106  0x106   //线路12三段过负荷故障信号
#define YX_0107  0x107   //线路12相间短路故障
#define YX_0108  0x108   //线路12单相接地故障信号
#define YX_0109  0x109   //线路12瞬时故障信号
#define YX_010A  0x10A   //线路12永久故障信号
#define YX_010B  0x10B   //线路12零序电压故障信号
#define YX_010C  0x10C   //线路12零序电流故障信号
#define YX_010D  0x10D   //线路12一段保护动作信号
#define YX_010E  0x10E   //线路12二段保护动作信号
#define YX_010F  0x10F   //线路12三段保护动作信号
#define YX_0110  0x110   //线路12单相接地保护动作信号
#define YX_0111  0x111   //线路12PT断线
#define YX_0112  0x112   //线路12CT断线
#define YX_0113  0x113   //线路12本地合闸
#define YX_0114  0x114   //线路12本地分闸
#define YX_0115  0x115   //线路13开关合位
#define YX_0116  0x116   //线路13开关分位
#define YX_0117  0x117   //线路13开关储能位
#define YX_0118  0x118   //线路13备用遥信1
#define YX_0119  0x119   //线路13备用遥信2
#define YX_011A  0x11A   //线路13备用遥信3
#define YX_011B  0x11B   //线路13一段速断故障信号
#define YX_011C  0x11C   //线路13二段过流故障信号
#define YX_011D  0x11D   //线路13三段过负荷故障信号
#define YX_011E  0x11E   //线路13相间短路故障
#define YX_011F  0x11F   //线路13单相接地故障信号
#define YX_0120  0x120   //线路13瞬时故障信号
#define YX_0121  0x121   //线路13永久故障信号
#define YX_0122  0x122   //线路13零序电压故障信号
#define YX_0123  0x123   //线路13零序电流故障信号
#define YX_0124  0x124   //线路13一段保护动作信号
#define YX_0125  0x125   //线路13二段保护动作信号
#define YX_0126  0x126   //线路13三段保护动作信号
#define YX_0127  0x127   //线路13单相接地保护动作信号
#define YX_0128  0x128   //线路13PT断线
#define YX_0129  0x129   //线路13CT断线
#define YX_012A  0x12A   //线路13本地合闸
#define YX_012B  0x12B   //线路13本地分闸
#define YX_012C  0x12C   //线路14开关合位
#define YX_012D  0x12D   //线路14开关分位
#define YX_012E  0x12E   //线路14开关储能位
#define YX_012F  0x12F   //线路14备用遥信1
#define YX_0130  0x130   //线路14备用遥信2
#define YX_0131  0x131   //线路14备用遥信3
#define YX_0132  0x132   //线路14一段速断故障信号
#define YX_0133  0x133   //线路14二段过流故障信号
#define YX_0134  0x134   //线路14三段过负荷故障信号
#define YX_0135  0x135   //线路14相间短路故障
#define YX_0136  0x136   //线路14单相接地故障信号
#define YX_0137  0x137   //线路14瞬时故障信号
#define YX_0138  0x138   //线路14永久故障信号
#define YX_0139  0x139   //线路14零序电压故障信号
#define YX_013A  0x13A   //线路14零序电流故障信号
#define YX_013B  0x13B   //线路14一段保护动作信号
#define YX_013C  0x13C   //线路14二段保护动作信号
#define YX_013D  0x13D   //线路14三段保护动作信号
#define YX_013E  0x13E   //线路14单相接地保护动作信号
#define YX_013F  0x13F   //线路14PT断线
#define YX_0140  0x140   //线路14CT断线
#define YX_0141  0x141   //线路14本地合闸
#define YX_0142  0x142   //线路14本地分闸
#define YX_0143  0x143   //线路15开关合位
#define YX_0144  0x144   //线路15开关分位
#define YX_0145  0x145   //线路15开关储能位
#define YX_0146  0x146   //线路15备用遥信1
#define YX_0147  0x147   //线路15备用遥信2
#define YX_0148  0x148   //线路15备用遥信3
#define YX_0149  0x149   //线路15一段速断故障信号
#define YX_014A  0x14A   //线路15二段过流故障信号
#define YX_014B  0x14B   //线路15三段过负荷故障信号
#define YX_014C  0x14C   //线路15相间短路故障
#define YX_014D  0x14D   //线路15单相接地故障信号
#define YX_014E  0x14E   //线路15瞬时故障信号
#define YX_014F  0x14F   //线路15永久故障信号
#define YX_0150  0x150   //线路15零序电压故障信号
#define YX_0151  0x151   //线路15零序电流故障信号
#define YX_0152  0x152   //线路15一段保护动作信号
#define YX_0153  0x153   //线路15二段保护动作信号
#define YX_0154  0x154   //线路15三段保护动作信号
#define YX_0155  0x155   //线路15单相接地保护动作信号
#define YX_0156  0x156   //线路15PT断线
#define YX_0157  0x157   //线路15CT断线
#define YX_0158  0x158   //线路15本地合闸
#define YX_0159  0x159   //线路15本地分闸
#define YX_015A  0x15A   //线路16开关合位
#define YX_015B  0x15B   //线路16开关分位
#define YX_015C  0x15C   //线路16开关储能位
#define YX_015D  0x15D   //线路16备用遥信1
#define YX_015E  0x15E   //线路16备用遥信2
#define YX_015F  0x15F   //线路16备用遥信3
#define YX_0160  0x160   //线路16一段速断故障信号
#define YX_0161  0x161   //线路16二段过流故障信号
#define YX_0162  0x162   //线路16三段过负荷故障信号
#define YX_0163  0x163   //线路16相间短路故障
#define YX_0164  0x164   //线路16单相接地故障信号
#define YX_0165  0x165   //线路16瞬时故障信号
#define YX_0166  0x166   //线路16永久故障信号
#define YX_0167  0x167   //线路16零序电压故障信号
#define YX_0168  0x168   //线路16零序电流故障信号
#define YX_0169  0x169   //线路16一段保护动作信号
#define YX_016A  0x16A   //线路16二段保护动作信号
#define YX_016B  0x16B   //线路16三段保护动作信号
#define YX_016C  0x16C   //线路16单相接地保护动作信号
#define YX_016D  0x16D   //线路16PT断线
#define YX_016E  0x16E   //线路16CT断线
#define YX_016F  0x16F   //线路16本地合闸
#define YX_0170  0x170   //线路16本地分闸
#define YX_0171  0x171   //断路器1双点遥信
#define YX_0172  0x172   //断路器2双点遥信
#define YX_0173  0x173   //断路器3双点遥信
#define YX_0174  0x174   //断路器4双点遥信
#define YX_0175  0x175   //断路器5双点遥信
#define YX_0176  0x176   //断路器6双点遥信
#define YX_0177  0x177   //断路器7双点遥信
#define YX_0178  0x178   //断路器8双点遥信
#define YX_0179  0x179   //断路器9双点遥信
#define YX_017A  0x17A   //断路器10双点遥信
#define YX_017B  0x17B   //断路器11双点遥信
#define YX_017C  0x17C   //断路器12双点遥信
#define YX_017D  0x17D   //断路器13双点遥信
#define YX_017E  0x17E   //断路器14双点遥信
#define YX_017F  0x17F   //断路器15双点遥信
#define YX_0180  0x180   //断路器16双点遥信
#define YX_0181  0x181   //线路1有压
#define YX_0182  0x182   //线路2有压
#define YX_0183  0x183   //线路3有压
#define YX_0184  0x184   //线路4有压
#define YX_0185  0x185   //线路5有压
#define YX_0186  0x186   //线路6有压
#define YX_0187  0x187   //线路7有压
#define YX_0188  0x188   //线路8有压
#define YX_0189  0x189   //线路9有压
#define YX_018A  0x18A   //线路10有压
#define YX_018B  0x18B   //线路11有压
#define YX_018C  0x18C   //线路12有压
#define YX_018D  0x18D   //线路13有压
#define YX_018E  0x18E   //线路14有压
#define YX_018F  0x18F   //线路15有压
#define YX_0190  0x190   //线路16有压
#define YX_0191  0x191   //公共遥信1
#define YX_0192  0x192   //公共遥信2
#define YX_0193  0x193   //公共遥信3
#define YX_0194  0x194   //公共遥信4
#define YX_0195  0x195   //远方/就地位置
#define YX_0196  0x196   //电池低压告警/恢复
#define YX_0197  0x197   //终端直流供电
#define YX_0198  0x198   //装置工作电源异常告警/恢复
#define YX_0199  0x199   //电池活化状态
#define YX_019A  0x19A   //终端主交流供电
#define YX_019B  0x19B   //终端备交流供电
#define YX_019C  0x19C   //终端异常/恢复
#define YX_019D  0x19D   //终端温度告警
#define YX_019E  0x19E   //公共遥控1本地动作
#define YX_019F  0x19F   //公共遥控2本地动作
#define YX_01A0  0x1A0   //公共遥控3本地动作
#define YX_01A1  0x1A1   //公共遥控4本地动作


#define YX_0201  0x201   //线路1 A相电压越上限告警
#define YX_0202  0x202   //线路1 B相电压越上限告警
#define YX_0203  0x203   //线路1 C相电压越上限告警
#define YX_0204  0x204   //线路1 AB线电压越上限告警
#define YX_0205  0x205   //线路1 CB线电压越上限告警
#define YX_0206  0x206   //线路1 零序电压越上限告警
#define YX_0207  0x207   //线路1 A相电压越下限告警
#define YX_0208  0x208   //线路1 B相电压越下限告警
#define YX_0209  0x209   //线路1 C相电压越下限告警
#define YX_020A  0x20A   //线路1 AB线电压越下限告警
#define YX_020B  0x20B   //线路1 CB线电压越下限告警
#define YX_020C  0x20C   //线路1 A相电流越上限告警
#define YX_020D  0x20D   //线路1 B相电流越上限告警
#define YX_020E  0x20E   //线路1 C相电流越上限告警
#define YX_020F  0x20F   //线路1 零序电流越上限告警
#define YX_0210  0x210   //线路1 负荷越上限告警
#define YX_0211  0x211   //线路2 A相电压越上限告警
#define YX_0212  0x212   //线路2 B相电压越上限告警
#define YX_0213  0x213   //线路2 C相电压越上限告警
#define YX_0214  0x214   //线路2 AB线电压越上限告警
#define YX_0215  0x215   //线路2 CB线电压越上限告警
#define YX_0216  0x216   //线路2 零序电压越上限告警
#define YX_0217  0x217   //线路2 A相电压越下限告警
#define YX_0218  0x218   //线路2 B相电压越下限告警
#define YX_0219  0x219   //线路2 C相电压越下限告警
#define YX_021A  0x21A   //线路2 AB线电压越下限告警
#define YX_021B  0x21B   //线路2 CB线电压越下限告警
#define YX_021C  0x21C   //线路2 A相电流越上限告警
#define YX_021D  0x21D   //线路2 B相电流越上限告警
#define YX_021E  0x21E   //线路2 C相电流越上限告警
#define YX_021F  0x21F   //线路2 零序电流越上限告警
#define YX_0220  0x220   //线路2 负荷越上限告警
#define YX_0221  0x221   //线路3 A相电压越上限告警
#define YX_0222  0x222   //线路3 B相电压越上限告警
#define YX_0223  0x223   //线路3 C相电压越上限告警
#define YX_0224  0x224   //线路3 AB线电压越上限告警
#define YX_0225  0x225   //线路3 CB线电压越上限告警
#define YX_0226  0x226   //线路3 零序电压越上限告警
#define YX_0227  0x227   //线路3 A相电压越下限告警
#define YX_0228  0x228   //线路3 B相电压越下限告警
#define YX_0229  0x229   //线路3 C相电压越下限告警
#define YX_022A  0x22A   //线路3 AB线电压越下限告警
#define YX_022B  0x22B   //线路3 CB线电压越下限告警
#define YX_022C  0x22C   //线路3 A相电流越上限告警
#define YX_022D  0x22D   //线路3 B相电流越上限告警
#define YX_022E  0x22E   //线路3 C相电流越上限告警
#define YX_022F  0x22F   //线路3 零序电流越上限告警
#define YX_0230  0x230   //线路3 负荷越上限告警
#define YX_0231  0x231   //线路4 A相电压越上限告警
#define YX_0232  0x232   //线路4 B相电压越上限告警
#define YX_0233  0x233   //线路4 C相电压越上限告警
#define YX_0234  0x234   //线路4 AB线电压越上限告警
#define YX_0235  0x235   //线路4 CB线电压越上限告警
#define YX_0236  0x236   //线路4 零序电压越上限告警
#define YX_0237  0x237   //线路4 A相电压越下限告警
#define YX_0238  0x238   //线路4 B相电压越下限告警
#define YX_0239  0x239   //线路4 C相电压越下限告警
#define YX_023A  0x23A   //线路4 AB线电压越下限告警
#define YX_023B  0x23B   //线路4 CB线电压越下限告警
#define YX_023C  0x23C   //线路4 A相电流越上限告警
#define YX_023D  0x23D   //线路4 B相电流越上限告警
#define YX_023E  0x23E   //线路4 C相电流越上限告警
#define YX_023F  0x23F   //线路4 零序电流越上限告警
#define YX_0240  0x240   //线路4 负荷越上限告警
#define YX_0241  0x241   //线路5 A相电压越上限告警
#define YX_0242  0x242   //线路5 B相电压越上限告警
#define YX_0243  0x243   //线路5 C相电压越上限告警
#define YX_0244  0x244   //线路5 AB线电压越上限告警
#define YX_0245  0x245   //线路5 CB线电压越上限告警
#define YX_0246  0x246   //线路5 零序电压越上限告警
#define YX_0247  0x247   //线路5 A相电压越下限告警
#define YX_0248  0x248   //线路5 B相电压越下限告警
#define YX_0249  0x249   //线路5 C相电压越下限告警
#define YX_024A  0x24A   //线路5 AB线电压越下限告警
#define YX_024B  0x24B   //线路5 CB线电压越下限告警
#define YX_024C  0x24C   //线路5 A相电流越上限告警
#define YX_024D  0x24D   //线路5 B相电流越上限告警
#define YX_024E  0x24E   //线路5 C相电流越上限告警
#define YX_024F  0x24F   //线路5 零序电流越上限告警
#define YX_0250  0x250   //线路5 负荷越上限告警
#define YX_0251  0x251   //线路6 A相电压越上限告警
#define YX_0252  0x252   //线路6 B相电压越上限告警
#define YX_0253  0x253   //线路6 C相电压越上限告警
#define YX_0254  0x254   //线路6 AB线电压越上限告警
#define YX_0255  0x255   //线路6 CB线电压越上限告警
#define YX_0256  0x256   //线路6 零序电压越上限告警
#define YX_0257  0x257   //线路6 A相电压越下限告警
#define YX_0258  0x258   //线路6 B相电压越下限告警
#define YX_0259  0x259   //线路6 C相电压越下限告警
#define YX_025A  0x25A   //线路6 AB线电压越下限告警
#define YX_025B  0x25B   //线路6 CB线电压越下限告警
#define YX_025C  0x25C   //线路6 A相电流越上限告警
#define YX_025D  0x25D   //线路6 B相电流越上限告警
#define YX_025E  0x25E   //线路6 C相电流越上限告警
#define YX_025F  0x25F   //线路6 零序电流越上限告警
#define YX_0260  0x260   //线路6 负荷越上限告警
#define YX_0261  0x261   //线路7 A相电压越上限告警
#define YX_0262  0x262   //线路7 B相电压越上限告警
#define YX_0263  0x263   //线路7 C相电压越上限告警
#define YX_0264  0x264   //线路7 AB线电压越上限告警
#define YX_0265  0x265   //线路7 CB线电压越上限告警
#define YX_0266  0x266   //线路7 零序电压越上限告警
#define YX_0267  0x267   //线路7 A相电压越下限告警
#define YX_0268  0x268   //线路7 B相电压越下限告警
#define YX_0269  0x269   //线路7 C相电压越下限告警
#define YX_026A  0x26A   //线路7 AB线电压越下限告警
#define YX_026B  0x26B   //线路7 CB线电压越下限告警
#define YX_026C  0x26C   //线路7 A相电流越上限告警
#define YX_026D  0x26D   //线路7 B相电流越上限告警
#define YX_026E  0x26E   //线路7 C相电流越上限告警
#define YX_026F  0x26F   //线路7 零序电流越上限告警
#define YX_0270  0x270   //线路7 负荷越上限告警
#define YX_0271  0x271   //线路8 A相电压越上限告警
#define YX_0272  0x272   //线路8 B相电压越上限告警
#define YX_0273  0x273   //线路8 C相电压越上限告警
#define YX_0274  0x274   //线路8 AB线电压越上限告警
#define YX_0275  0x275   //线路8 CB线电压越上限告警
#define YX_0276  0x276   //线路8 零序电压越上限告警
#define YX_0277  0x277   //线路8 A相电压越下限告警
#define YX_0278  0x278   //线路8 B相电压越下限告警
#define YX_0279  0x279   //线路8 C相电压越下限告警
#define YX_027A  0x27A   //线路8 AB线电压越下限告警
#define YX_027B  0x27B   //线路8 CB线电压越下限告警
#define YX_027C  0x27C   //线路8 A相电流越上限告警
#define YX_027D  0x27D   //线路8 B相电流越上限告警
#define YX_027E  0x27E   //线路8 C相电流越上限告警
#define YX_027F  0x27F   //线路8 零序电流越上限告警
#define YX_0280  0x280   //线路8 负荷越上限告警
#define YX_0281  0x281   //线路9 A相电压越上限告警
#define YX_0282  0x282   //线路9 B相电压越上限告警
#define YX_0283  0x283   //线路9 C相电压越上限告警
#define YX_0284  0x284   //线路9 AB线电压越上限告警
#define YX_0285  0x285   //线路9 CB线电压越上限告警
#define YX_0286  0x286   //线路9 零序电压越上限告警
#define YX_0287  0x287   //线路9 A相电压越下限告警
#define YX_0288  0x288   //线路9 B相电压越下限告警
#define YX_0289  0x289   //线路9 C相电压越下限告警
#define YX_028A  0x28A   //线路9 AB线电压越下限告警
#define YX_028B  0x28B   //线路9 CB线电压越下限告警
#define YX_028C  0x28C   //线路9 A相电流越上限告警
#define YX_028D  0x28D   //线路9 B相电流越上限告警
#define YX_028E  0x28E   //线路9 C相电流越上限告警
#define YX_028F  0x28F   //线路9 零序电流越上限告警
#define YX_0290  0x290   //线路9 负荷越上限告警
#define YX_0291  0x291   //线路10 A相电压越上限告警
#define YX_0292  0x292   //线路10 B相电压越上限告警
#define YX_0293  0x293   //线路10 C相电压越上限告警
#define YX_0294  0x294   //线路10 AB线电压越上限告警
#define YX_0295  0x295   //线路10 CB线电压越上限告警
#define YX_0296  0x296   //线路10 零序电压越上限告警
#define YX_0297  0x297   //线路10 A相电压越下限告警
#define YX_0298  0x298   //线路10 B相电压越下限告警
#define YX_0299  0x299   //线路10 C相电压越下限告警
#define YX_029A  0x29A   //线路10 AB线电压越下限告警
#define YX_029B  0x29B   //线路10 CB线电压越下限告警
#define YX_029C  0x29C   //线路10 A相电流越上限告警
#define YX_029D  0x29D   //线路10 B相电流越上限告警
#define YX_029E  0x29E   //线路10 C相电流越上限告警
#define YX_029F  0x29F   //线路10 零序电流越上限告警
#define YX_02A0  0x2A0   //线路10 负荷越上限告警
#define YX_02A1  0x2A1   //线路11 A相电压越上限告警
#define YX_02A2  0x2A2   //线路11 B相电压越上限告警
#define YX_02A3  0x2A3   //线路11 C相电压越上限告警
#define YX_02A4  0x2A4   //线路11 AB线电压越上限告警
#define YX_02A5  0x2A5   //线路11 CB线电压越上限告警
#define YX_02A6  0x2A6   //线路11 零序电压越上限告警
#define YX_02A7  0x2A7   //线路11 A相电压越下限告警
#define YX_02A8  0x2A8   //线路11 B相电压越下限告警
#define YX_02A9  0x2A9   //线路11 C相电压越下限告警
#define YX_02AA  0x2AA   //线路11 AB线电压越下限告警
#define YX_02AB  0x2AB   //线路11 CB线电压越下限告警
#define YX_02AC  0x2AC   //线路11 A相电流越上限告警
#define YX_02AD  0x2AD   //线路11 B相电流越上限告警
#define YX_02AE  0x2AE   //线路11 C相电流越上限告警
#define YX_02AF  0x2AF   //线路11 零序电流越上限告警
#define YX_02B0  0x2B0   //线路11 负荷越上限告警
#define YX_02B1  0x2B1   //线路12 A相电压越上限告警
#define YX_02B2  0x2B2   //线路12 B相电压越上限告警
#define YX_02B3  0x2B3   //线路12 C相电压越上限告警
#define YX_02B4  0x2B4   //线路12 AB线电压越上限告警
#define YX_02B5  0x2B5   //线路12 CB线电压越上限告警
#define YX_02B6  0x2B6   //线路12 零序电压越上限告警
#define YX_02B7  0x2B7   //线路12 A相电压越下限告警
#define YX_02B8  0x2B8   //线路12 B相电压越下限告警
#define YX_02B9  0x2B9   //线路12 C相电压越下限告警
#define YX_02BA  0x2BA   //线路12 AB线电压越下限告警
#define YX_02BB  0x2BB   //线路12 CB线电压越下限告警
#define YX_02BC  0x2BC   //线路12 A相电流越上限告警
#define YX_02BD  0x2BD   //线路12 B相电流越上限告警
#define YX_02BE  0x2BE   //线路12 C相电流越上限告警
#define YX_02BF  0x2BF   //线路12 零序电流越上限告警
#define YX_02C0  0x2C0   //线路12 负荷越上限告警
#define YX_02C1  0x2C1   //线路13 A相电压越上限告警
#define YX_02C2  0x2C2   //线路13 B相电压越上限告警
#define YX_02C3  0x2C3   //线路13 C相电压越上限告警
#define YX_02C4  0x2C4   //线路13 AB线电压越上限告警
#define YX_02C5  0x2C5   //线路13 CB线电压越上限告警
#define YX_02C6  0x2C6   //线路13 零序电压越上限告警
#define YX_02C7  0x2C7   //线路13 A相电压越下限告警
#define YX_02C8  0x2C8   //线路13 B相电压越下限告警
#define YX_02C9  0x2C9   //线路13 C相电压越下限告警
#define YX_02CA  0x2CA   //线路13 AB线电压越下限告警
#define YX_02CB  0x2CB   //线路13 CB线电压越下限告警
#define YX_02CC  0x2CC   //线路13 A相电流越上限告警
#define YX_02CD  0x2CD   //线路13 B相电流越上限告警
#define YX_02CE  0x2CE   //线路13 C相电流越上限告警
#define YX_02CF  0x2CF   //线路13 零序电流越上限告警
#define YX_02D0  0x2D0   //线路13 负荷越上限告警
#define YX_02D1  0x2D1   //线路14 A相电压越上限告警
#define YX_02D2  0x2D2   //线路14 B相电压越上限告警
#define YX_02D3  0x2D3   //线路14 C相电压越上限告警
#define YX_02D4  0x2D4   //线路14 AB线电压越上限告警
#define YX_02D5  0x2D5   //线路14 CB线电压越上限告警
#define YX_02D6  0x2D6   //线路14 零序电压越上限告警
#define YX_02D7  0x2D7   //线路14 A相电压越下限告警
#define YX_02D8  0x2D8   //线路14 B相电压越下限告警
#define YX_02D9  0x2D9   //线路14 C相电压越下限告警
#define YX_02DA  0x2DA   //线路14 AB线电压越下限告警
#define YX_02DB  0x2DB   //线路14 CB线电压越下限告警
#define YX_02DC  0x2DC   //线路14 A相电流越上限告警
#define YX_02DD  0x2DD   //线路14 B相电流越上限告警
#define YX_02DE  0x2DE   //线路14 C相电流越上限告警
#define YX_02DF  0x2DF   //线路14 零序电流越上限告警
#define YX_02E0  0x2E0   //线路14 负荷越上限告警
#define YX_02E1  0x2E1   //线路15 A相电压越上限告警
#define YX_02E2  0x2E2   //线路15 B相电压越上限告警
#define YX_02E3  0x2E3   //线路15 C相电压越上限告警
#define YX_02E4  0x2E4   //线路15 AB线电压越上限告警
#define YX_02E5  0x2E5   //线路15 CB线电压越上限告警
#define YX_02E6  0x2E6   //线路15 零序电压越上限告警
#define YX_02E7  0x2E7   //线路15 A相电压越下限告警
#define YX_02E8  0x2E8   //线路15 B相电压越下限告警
#define YX_02E9  0x2E9   //线路15 C相电压越下限告警
#define YX_02EA  0x2EA   //线路15 AB线电压越下限告警
#define YX_02EB  0x2EB   //线路15 CB线电压越下限告警
#define YX_02EC  0x2EC   //线路15 A相电流越上限告警
#define YX_02ED  0x2ED   //线路15 B相电流越上限告警
#define YX_02EE  0x2EE   //线路15 C相电流越上限告警
#define YX_02EF  0x2EF   //线路15 零序电流越上限告警
#define YX_02F0  0x2F0   //线路15 负荷越上限告警
#define YX_02F1  0x2F1   //线路16 A相电压越上限告警
#define YX_02F2  0x2F2   //线路16 B相电压越上限告警
#define YX_02F3  0x2F3   //线路16 C相电压越上限告警
#define YX_02F4  0x2F4   //线路16 AB线电压越上限告警
#define YX_02F5  0x2F5   //线路16 CB线电压越上限告警
#define YX_02F6  0x2F6   //线路16 零序电压越上限告警
#define YX_02F7  0x2F7   //线路16 A相电压越下限告警
#define YX_02F8  0x2F8   //线路16 B相电压越下限告警
#define YX_02F9  0x2F9   //线路16 C相电压越下限告警
#define YX_02FA  0x2FA   //线路16 AB线电压越下限告警
#define YX_02FB  0x2FB   //线路16 CB线电压越下限告警
#define YX_02FC  0x2FC   //线路16 A相电流越上限告警
#define YX_02FD  0x2FD   //线路16 B相电流越上限告警
#define YX_02FE  0x2FE   //线路16 C相电流越上限告警
#define YX_02FF  0x2FF   //线路16 零序电流越上限告警
#define YX_0300  0x300   //线路16 负荷越上限告警

#define YX_0301  0x301   //故障指示器

#define YX_0501  0x501   //备自投

#define YX_0701  0x701   //状态监测

#define YX_0801  0x801   //配电房水位告警
#define YX_0802  0x802   //配电房火灾告警
#define YX_0803  0x803   //配电房环境温度告警
#define YX_0804  0x804   //电缆温度告警
#define YX_0805  0x805   //直流母线电压越上限告警
#define YX_0806  0x806   //直流母线电压越下限告警

//遥信总数381+256+6=643

/**
* /brief 遥测类数据标识
*/
#define YC_4001  0x4001   //线路1 A相电压
#define YC_4002  0x4002   //线路1 B相电压
#define YC_4003  0x4003   //线路1 C相电压
#define YC_4004  0x4004   //线路1 AB线电压
#define YC_4005  0x4005   //线路1 CB线电压
#define YC_4006  0x4006   //线路1 零序电压
#define YC_4007  0x4007   //线路1 A相测量电流
#define YC_4008  0x4008   //线路1 B相测量电流
#define YC_4009  0x4009   //线路1 C相测量电流
#define YC_400A  0x400A   //线路1 零序电流
#define YC_400B  0x400B   //线路1 A相保护电流
#define YC_400C  0x400C   //线路1 B相保护电流
#define YC_400D  0x400D   //线路1 C相保护电流
#define YC_400E  0x400E   //线路1 总有功功率
#define YC_400F  0x400F   //线路1 总无功功率
#define YC_4010  0x4010   //线路1 总视在功率
#define YC_4011  0x4011   //线路1 总功率因数
#define YC_4012  0x4012   //线路1 频率
#define YC_4013  0x4013   //线路2 A相电压
#define YC_4014  0x4014   //线路2 B相电压
#define YC_4015  0x4015   //线路2 C相电压
#define YC_4016  0x4016   //线路2 AB线电压
#define YC_4017  0x4017   //线路2 CB线电压
#define YC_4018  0x4018   //线路2 零序电压
#define YC_4019  0x4019   //线路2 A相测量电流
#define YC_401A  0x401A   //线路2 B相测量电流
#define YC_401B  0x401B   //线路2 C相测量电流
#define YC_401C  0x401C   //线路2 零序电流
#define YC_401D  0x401D   //线路2 A相保护电流
#define YC_401E  0x401E   //线路2 B相保护电流
#define YC_401F  0x401F   //线路2 C相保护电流
#define YC_4020  0x4020   //线路2 总有功功率
#define YC_4021  0x4021   //线路2 总无功功率
#define YC_4022  0x4022   //线路2 总视在功率
#define YC_4023  0x4023   //线路2 总功率因数
#define YC_4024  0x4024   //线路2 频率
#define YC_4025  0x4025   //线路3 A相电压
#define YC_4026  0x4026   //线路3 B相电压
#define YC_4027  0x4027   //线路3 C相电压
#define YC_4028  0x4028   //线路3 AB线电压
#define YC_4029  0x4029   //线路3 CB线电压
#define YC_402A  0x402A   //线路3 零序电压
#define YC_402B  0x402B   //线路3 A相测量电流
#define YC_402C  0x402C   //线路3 B相测量电流
#define YC_402D  0x402D   //线路3 C相测量电流
#define YC_402E  0x402E   //线路3 零序电流
#define YC_402F  0x402F   //线路3 A相保护电流
#define YC_4030  0x4030   //线路3 B相保护电流
#define YC_4031  0x4031   //线路3 C相保护电流
#define YC_4032  0x4032   //线路3 总有功功率
#define YC_4033  0x4033   //线路3 总无功功率
#define YC_4034  0x4034   //线路3 总视在功率
#define YC_4035  0x4035   //线路3 总功率因数
#define YC_4036  0x4036   //线路3 频率
#define YC_4037  0x4037   //线路4 A相电压
#define YC_4038  0x4038   //线路4 B相电压
#define YC_4039  0x4039   //线路4 C相电压
#define YC_403A  0x403A   //线路4 AB线电压
#define YC_403B  0x403B   //线路4 CB线电压
#define YC_403C  0x403C   //线路4 零序电压
#define YC_403D  0x403D   //线路4 A相测量电流
#define YC_403E  0x403E   //线路4 B相测量电流
#define YC_403F  0x403F   //线路4 C相测量电流
#define YC_4040  0x4040   //线路4 零序电流
#define YC_4041  0x4041   //线路4 A相保护电流
#define YC_4042  0x4042   //线路4 B相保护电流
#define YC_4043  0x4043   //线路4 C相保护电流
#define YC_4044  0x4044   //线路4 总有功功率
#define YC_4045  0x4045   //线路4 总无功功率
#define YC_4046  0x4046   //线路4 总视在功率
#define YC_4047  0x4047   //线路4 总功率因数
#define YC_4048  0x4048   //线路4 频率
#define YC_4049  0x4049   //线路5 A相电压
#define YC_404A  0x404A   //线路5 B相电压
#define YC_404B  0x404B   //线路5 C相电压
#define YC_404C  0x404C   //线路5 AB线电压
#define YC_404D  0x404D   //线路5 CB线电压
#define YC_404E  0x404E   //线路5 零序电压
#define YC_404F  0x404F   //线路5 A相测量电流
#define YC_4050  0x4050   //线路5 B相测量电流
#define YC_4051  0x4051   //线路5 C相测量电流
#define YC_4052  0x4052   //线路5 零序电流
#define YC_4053  0x4053   //线路5 A相保护电流
#define YC_4054  0x4054   //线路5 B相保护电流
#define YC_4055  0x4055   //线路5 C相保护电流
#define YC_4056  0x4056   //线路5 总有功功率
#define YC_4057  0x4057   //线路5 总无功功率
#define YC_4058  0x4058   //线路5 总视在功率
#define YC_4059  0x4059   //线路5 总功率因数
#define YC_405A  0x405A   //线路5 频率
#define YC_405B  0x405B   //线路6 A相电压
#define YC_405C  0x405C   //线路6 B相电压
#define YC_405D  0x405D   //线路6 C相电压
#define YC_405E  0x405E   //线路6 AB线电压
#define YC_405F  0x405F   //线路6 CB线电压
#define YC_4060  0x4060   //线路6 零序电压
#define YC_4061  0x4061   //线路6 A相测量电流
#define YC_4062  0x4062   //线路6 B相测量电流
#define YC_4063  0x4063   //线路6 C相测量电流
#define YC_4064  0x4064   //线路6 零序电流
#define YC_4065  0x4065   //线路6 A相保护电流
#define YC_4066  0x4066   //线路6 B相保护电流
#define YC_4067  0x4067   //线路6 C相保护电流
#define YC_4068  0x4068   //线路6 总有功功率
#define YC_4069  0x4069   //线路6 总无功功率
#define YC_406A  0x406A   //线路6 总视在功率
#define YC_406B  0x406B   //线路6 总功率因数
#define YC_406C  0x406C   //线路6 频率
#define YC_406D  0x406D   //线路7 A相电压
#define YC_406E  0x406E   //线路7 B相电压
#define YC_406F  0x406F   //线路7 C相电压
#define YC_4070  0x4070   //线路7 AB线电压
#define YC_4071  0x4071   //线路7 CB线电压
#define YC_4072  0x4072   //线路7 零序电压
#define YC_4073  0x4073   //线路7 A相测量电流
#define YC_4074  0x4074   //线路7 B相测量电流
#define YC_4075  0x4075   //线路7 C相测量电流
#define YC_4076  0x4076   //线路7 零序电流
#define YC_4077  0x4077   //线路7 A相保护电流
#define YC_4078  0x4078   //线路7 B相保护电流
#define YC_4079  0x4079   //线路7 C相保护电流
#define YC_407A  0x407A   //线路7 总有功功率
#define YC_407B  0x407B   //线路7 总无功功率
#define YC_407C  0x407C   //线路7 总视在功率
#define YC_407D  0x407D   //线路7 总功率因数
#define YC_407E  0x407E   //线路7 频率
#define YC_407F  0x407F   //线路8 A相电压
#define YC_4080  0x4080   //线路8 B相电压
#define YC_4081  0x4081   //线路8 C相电压
#define YC_4082  0x4082   //线路8 AB线电压
#define YC_4083  0x4083   //线路8 CB线电压
#define YC_4084  0x4084   //线路8 零序电压
#define YC_4085  0x4085   //线路8 A相测量电流
#define YC_4086  0x4086   //线路8 B相测量电流
#define YC_4087  0x4087   //线路8 C相测量电流
#define YC_4088  0x4088   //线路8 零序电流
#define YC_4089  0x4089   //线路8 A相保护电流
#define YC_408A  0x408A   //线路8 B相保护电流
#define YC_408B  0x408B   //线路8 C相保护电流
#define YC_408C  0x408C   //线路8 总有功功率
#define YC_408D  0x408D   //线路8 总无功功率
#define YC_408E  0x408E   //线路8 总视在功率
#define YC_408F  0x408F   //线路8 总功率因数
#define YC_4090  0x4090   //线路8 频率
#define YC_4091  0x4091   //线路9 A相电压
#define YC_4092  0x4092   //线路9 B相电压
#define YC_4093  0x4093   //线路9 C相电压
#define YC_4094  0x4094   //线路9 AB线电压
#define YC_4095  0x4095   //线路9 CB线电压
#define YC_4096  0x4096   //线路9 零序电压
#define YC_4097  0x4097   //线路9 A相测量电流
#define YC_4098  0x4098   //线路9 B相测量电流
#define YC_4099  0x4099   //线路9 C相测量电流
#define YC_409A  0x409A   //线路9 零序电流
#define YC_409B  0x409B   //线路9 A相保护电流
#define YC_409C  0x409C   //线路9 B相保护电流
#define YC_409D  0x409D   //线路9 C相保护电流
#define YC_409E  0x409E   //线路9 总有功功率
#define YC_409F  0x409F   //线路9 总无功功率
#define YC_40A0  0x40A0   //线路9 总视在功率
#define YC_40A1  0x40A1   //线路9 总功率因数
#define YC_40A2  0x40A2   //线路9 频率
#define YC_40A3  0x40A3   //线路10 A相电压
#define YC_40A4  0x40A4   //线路10 B相电压
#define YC_40A5  0x40A5   //线路10 C相电压
#define YC_40A6  0x40A6   //线路10 AB线电压
#define YC_40A7  0x40A7   //线路10 CB线电压
#define YC_40A8  0x40A8   //线路10 零序电压
#define YC_40A9  0x40A9   //线路10 A相测量电流
#define YC_40AA  0x40AA   //线路10 B相测量电流
#define YC_40AB  0x40AB   //线路10 C相测量电流
#define YC_40AC  0x40AC   //线路10 零序电流
#define YC_40AD  0x40AD   //线路10 A相保护电流
#define YC_40AE  0x40AE   //线路10 B相保护电流
#define YC_40AF  0x40AF   //线路10 C相保护电流
#define YC_40B0  0x40B0   //线路10 总有功功率
#define YC_40B1  0x40B1   //线路10 总无功功率
#define YC_40B2  0x40B2   //线路10 总视在功率
#define YC_40B3  0x40B3   //线路10 总功率因数
#define YC_40B4  0x40B4   //线路10 频率
#define YC_40B5  0x40B5   //线路11 A相电压
#define YC_40B6  0x40B6   //线路11 B相电压
#define YC_40B7  0x40B7   //线路11 C相电压
#define YC_40B8  0x40B8   //线路11 AB线电压
#define YC_40B9  0x40B9   //线路11 CB线电压
#define YC_40BA  0x40BA   //线路11 零序电压
#define YC_40BB  0x40BB   //线路11 A相测量电流
#define YC_40BC  0x40BC   //线路11 B相测量电流
#define YC_40BD  0x40BD   //线路11 C相测量电流
#define YC_40BE  0x40BE   //线路11 零序电流
#define YC_40BF  0x40BF   //线路11 A相保护电流
#define YC_40C0  0x40C0   //线路11 B相保护电流
#define YC_40C1  0x40C1   //线路11 C相保护电流
#define YC_40C2  0x40C2   //线路11 总有功功率
#define YC_40C3  0x40C3   //线路11 总无功功率
#define YC_40C4  0x40C4   //线路11 总视在功率
#define YC_40C5  0x40C5   //线路11 总功率因数
#define YC_40C6  0x40C6   //线路11 频率
#define YC_40C7  0x40C7   //线路12 A相电压
#define YC_40C8  0x40C8   //线路12 B相电压
#define YC_40C9  0x40C9   //线路12 C相电压
#define YC_40CA  0x40CA   //线路12 AB线电压
#define YC_40CB  0x40CB   //线路12 CB线电压
#define YC_40CC  0x40CC   //线路12 零序电压
#define YC_40CD  0x40CD   //线路12 A相测量电流
#define YC_40CE  0x40CE   //线路12 B相测量电流
#define YC_40CF  0x40CF   //线路12 C相测量电流
#define YC_40D0  0x40D0   //线路12 零序电流
#define YC_40D1  0x40D1   //线路12 A相保护电流
#define YC_40D2  0x40D2   //线路12 B相保护电流
#define YC_40D3  0x40D3   //线路12 C相保护电流
#define YC_40D4  0x40D4   //线路12 总有功功率
#define YC_40D5  0x40D5   //线路12 总无功功率
#define YC_40D6  0x40D6   //线路12 总视在功率
#define YC_40D7  0x40D7   //线路12 总功率因数
#define YC_40D8  0x40D8   //线路12 频率
#define YC_40D9  0x40D9   //线路13 A相电压
#define YC_40DA  0x40DA   //线路13 B相电压
#define YC_40DB  0x40DB   //线路13 C相电压
#define YC_40DC  0x40DC   //线路13 AB线电压
#define YC_40DD  0x40DD   //线路13 CB线电压
#define YC_40DE  0x40DE   //线路13 零序电压
#define YC_40DF  0x40DF   //线路13 A相测量电流
#define YC_40E0  0x40E0   //线路13 B相测量电流
#define YC_40E1  0x40E1   //线路13 C相测量电流
#define YC_40E2  0x40E2   //线路13 零序电流
#define YC_40E3  0x40E3   //线路13 A相保护电流
#define YC_40E4  0x40E4   //线路13 B相保护电流
#define YC_40E5  0x40E5   //线路13 C相保护电流
#define YC_40E6  0x40E6   //线路13 总有功功率
#define YC_40E7  0x40E7   //线路13 总无功功率
#define YC_40E8  0x40E8   //线路13 总视在功率
#define YC_40E9  0x40E9   //线路13 总功率因数
#define YC_40EA  0x40EA   //线路13 频率
#define YC_40EB  0x40EB   //线路14 A相电压
#define YC_40EC  0x40EC   //线路14 B相电压
#define YC_40ED  0x40ED   //线路14 C相电压
#define YC_40EE  0x40EE   //线路14 AB线电压
#define YC_40EF  0x40EF   //线路14 CB线电压
#define YC_40F0  0x40F0   //线路14 零序电压
#define YC_40F1  0x40F1   //线路14 A相测量电流
#define YC_40F2  0x40F2   //线路14 B相测量电流
#define YC_40F3  0x40F3   //线路14 C相测量电流
#define YC_40F4  0x40F4   //线路14 零序电流
#define YC_40F5  0x40F5   //线路14 A相保护电流
#define YC_40F6  0x40F6   //线路14 B相保护电流
#define YC_40F7  0x40F7   //线路14 C相保护电流
#define YC_40F8  0x40F8   //线路14 总有功功率
#define YC_40F9  0x40F9   //线路14 总无功功率
#define YC_40FA  0x40FA   //线路14 总视在功率
#define YC_40FB  0x40FB   //线路14 总功率因数
#define YC_40FC  0x40FC   //线路14 频率
#define YC_40FD  0x40FD   //线路15 A相电压
#define YC_40FE  0x40FE   //线路15 B相电压
#define YC_40FF  0x40FF   //线路15 C相电压
#define YC_4100  0x4100   //线路15 AB线电压
#define YC_4101  0x4101   //线路15 CB线电压
#define YC_4102  0x4102   //线路15 零序电压
#define YC_4103  0x4103   //线路15 A相测量电流
#define YC_4104  0x4104   //线路15 B相测量电流
#define YC_4105  0x4105   //线路15 C相测量电流
#define YC_4106  0x4106   //线路15 零序电流
#define YC_4107  0x4107   //线路15 A相保护电流
#define YC_4108  0x4108   //线路15 B相保护电流
#define YC_4109  0x4109   //线路15 C相保护电流
#define YC_410A  0x410A   //线路15 总有功功率
#define YC_410B  0x410B   //线路15 总无功功率
#define YC_410C  0x410C   //线路15 总视在功率
#define YC_410D  0x410D   //线路15 总功率因数
#define YC_410E  0x410E   //线路15 频率
#define YC_410F  0x410F   //线路16 A相电压
#define YC_4110  0x4110   //线路16 B相电压
#define YC_4111  0x4111   //线路16 C相电压
#define YC_4112  0x4112   //线路16 AB线电压
#define YC_4113  0x4113   //线路16 CB线电压
#define YC_4114  0x4114   //线路16 零序电压
#define YC_4115  0x4115   //线路16 A相测量电流
#define YC_4116  0x4116   //线路16 B相测量电流
#define YC_4117  0x4117   //线路16 C相测量电流
#define YC_4118  0x4118   //线路16 零序电流
#define YC_4119  0x4119   //线路16 A相保护电流
#define YC_411A  0x411A   //线路16 B相保护电流
#define YC_411B  0x411B   //线路16 C相保护电流
#define YC_411C  0x411C   //线路16 总有功功率
#define YC_411D  0x411D   //线路16 总无功功率
#define YC_411E  0x411E   //线路16 总视在功率
#define YC_411F  0x411F   //线路16 总功率因数
#define YC_4120  0x4120   //线路16 频率
#define YC_4121  0x4121   //直流电源电压
#define YC_4122  0x4122   //环境温度
#define YC_4123  0x4123   //终端温度
#define YC_4124  0x4124   //电缆温度
#define YC_4125  0x4125   //直流量1
#define YC_4126  0x4126   //直流量2

#define YC_4201  0x4201   //其他数据1

#define YC_4401  0x4401   //其他数据2

#define YC_4601  0x4601   //其他数据3

#define YC_4801  0x4801   //其他数据4
//其他数据5
#define YC_4A01  0x4A01  // GPS经度
#define YC_4A02  0x4A02  //GPS纬度


#define YC_6401  0x6401   //累积量


//遥测总数294

/**
* /brief 遥控类数据标识
*/
#define YK_6001  0x6001   //断路器1（开/合）
#define YK_6002  0x6002   //断路器2（开/合）
#define YK_6003  0x6003   //断路器3（开/合）
#define YK_6004  0x6004   //断路器4（开/合）
#define YK_6005  0x6005   //断路器5（开/合）
#define YK_6006  0x6006   //断路器6（开/合）
#define YK_6007  0x6007   //断路器7（开/合）
#define YK_6008  0x6008   //断路器8（开/合）
#define YK_6009  0x6009   //断路器9（开/合）
#define YK_600A  0x600A   //断路器10（开/合）
#define YK_600B  0x600B   //断路器11（开/合）
#define YK_600C  0x600C   //断路器12（开/合）
#define YK_600D  0x600D   //断路器13（开/合）
#define YK_600E  0x600E   //断路器14（开/合）
#define YK_600F  0x600F   //断路器15（开/合）
#define YK_6010  0x6010   //断路器16（开/合）

#define YK_6064  0x6064   //电池活化
#define YK_6065  0x6065   //公共遥控1(开合,风机启停)
#define YK_6066  0x6066   //公共遥控2(开合,水泵启停)
//遥控总数19




/**
* /brief 参数类数据标识
*/
#define PARA_5001  0x5001   //第1套 I段电流定值（瞬时电流速断保护）
#define PARA_5002  0x5002   //第1套定值II段电流定值（定时限电流速断保护）
#define PARA_5003  0x5003   //第1套定值III段电流定值（定时限过电流保护）
#define PARA_5004  0x5004   //第1套定值零序I段电流保护定值
#define PARA_5005  0x5005   //第1套定值零序II段电流保护定值
#define PARA_5006  0x5006   //第1套定值零序III段电流保护定值
#define PARA_5007  0x5007   //第1套定值零序电压保护定值
#define PARA_5008  0x5008   //第1套定值电流方向保护投入定值
#define PARA_5009  0x5009   //第1套定值零序电流方向保护投入定值
#define PARA_500A  0x500A   //第2套定值I段电流定值（瞬时电流速断保护）
#define PARA_500B  0x500B   //第2套定值II段电流定值（定时限电流速断保护）
#define PARA_500C  0x500C   //第2套定值III段电流定值（定时限过电流保护）
#define PARA_500D  0x500D   //第2套定值零序I段电流保护定值
#define PARA_500E  0x500E   //第2套定值零序II段电流保护定值
#define PARA_500F  0x500F   //第2套定值零序III段电流保护定值
#define PARA_5010  0x5010   //第2套定值零序电压保护定值
#define PARA_5011  0x5011   //第2套定值电流方向保护投入定值
#define PARA_5012  0x5012   //第2套定值零序电流方向保护投入定值
#define PARA_5013  0x5013   //第3套定值I段电流定值（瞬时电流速断保护）
#define PARA_5014  0x5014   //第3套定值II段电流定值（定时限电流速断保护）
#define PARA_5015  0x5015   //第3套定值III段电流定值（定时限过电流保护）
#define PARA_5016  0x5016   //第3套定值零序I段电流保护定值
#define PARA_5017  0x5017   //第3套定值零序II段电流保护定值
#define PARA_5018  0x5018   //第3套定值零序III段电流保护定值
#define PARA_5019  0x5019   //第3套定值零序电压保护定值
#define PARA_501A  0x501A   //第3套定值电流方向保护投入定值
#define PARA_501B  0x501B   //第3套定值零序电流方向保护投入定值
#define PARA_501C  0x501C   //第4套定值I段电流定值（瞬时电流速断保护）
#define PARA_501D  0x501D   //第4套定值II段电流定值（定时限电流速断保护）
#define PARA_501E  0x501E   //第4套定值III段电流定值（定时限过电流保护）
#define PARA_501F  0x501F   //第4套定值零序I段电流保护定值
#define PARA_5020  0x5020   //第4套定值零序II段电流保护定值
#define PARA_5021  0x5021   //第4套定值零序III段电流保护定值
#define PARA_5022  0x5022   //第4套定值零序电压保护定值
#define PARA_5023  0x5023   //第4套定值电流方向保护投入定值
#define PARA_5024  0x5024   //第4套定值零序电流方向保护投入定值
#define PARA_5025  0x5025   //第5套定值I段电流定值（瞬时电流速断保护）
#define PARA_5026  0x5026   //第5套定值II段电流定值（定时限电流速断保护）
#define PARA_5027  0x5027   //第5套定值III段电流定值（定时限过电流保护）
#define PARA_5028  0x5028   //第5套定值零序I段电流保护定值
#define PARA_5029  0x5029   //第5套定值零序II段电流保护定值
#define PARA_502A  0x502A   //第5套定值零序III段电流保护定值
#define PARA_502B  0x502B   //第5套定值零序电压保护定值
#define PARA_502C  0x502C   //第5套定值电流方向保护投入定值
#define PARA_502D  0x502D   //第5套定值零序电流方向保护投入定值
#define PARA_502E  0x502E   //第6套定值I段电流定值（瞬时电流速断保护）
#define PARA_502F  0x502F   //第6套定值II段电流定值（定时限电流速断保护）
#define PARA_5030  0x5030   //第6套定值III段电流定值（定时限过电流保护）
#define PARA_5031  0x5031   //第6套定值零序I段电流保护定值
#define PARA_5032  0x5032   //第6套定值零序II段电流保护定值
#define PARA_5033  0x5033   //第6套定值零序III段电流保护定值
#define PARA_5034  0x5034   //第6套定值零序电压保护定值
#define PARA_5035  0x5035   //第6套定值电流方向保护投入定值
#define PARA_5036  0x5036   //第6套定值零序电流方向保护投入定值
#define PARA_5037  0x5037   //第7套定值I段电流定值（瞬时电流速断保护）
#define PARA_5038  0x5038   //第7套定值II段电流定值（定时限电流速断保护）
#define PARA_5039  0x5039   //第7套定值III段电流定值（定时限过电流保护）
#define PARA_503A  0x503A   //第7套定值零序I段电流保护定值
#define PARA_503B  0x503B   //第7套定值零序II段电流保护定值
#define PARA_503C  0x503C   //第7套定值零序III段电流保护定值
#define PARA_503D  0x503D   //第7套定值零序电压保护定值
#define PARA_503E  0x503E   //第7套定值电流方向保护投入定值
#define PARA_503F  0x503F   //第7套定值零序电流方向保护投入定值
#define PARA_5040  0x5040   //第8套定值I段电流定值（瞬时电流速断保护）
#define PARA_5041  0x5041   //第8套定值II段电流定值（定时限电流速断保护）
#define PARA_5042  0x5042   //第8套定值III段电流定值（定时限过电流保护）
#define PARA_5043  0x5043   //第8套定值零序I段电流保护定值
#define PARA_5044  0x5044   //第8套定值零序II段电流保护定值
#define PARA_5045  0x5045   //第8套定值零序III段电流保护定值
#define PARA_5046  0x5046   //第8套定值零序电压保护定值
#define PARA_5047  0x5047   //第8套定值电流方向保护投入定值
#define PARA_5048  0x5048   //第8套定值零序电流方向保护投入定值
#define PARA_5049  0x5049   //线路1~8保护定值区号设置
#define PARA_504A  0x504A   //线路9~16保护定值区号设置
#define PARA_504B  0x504B   //<重合闸设置
#define PARA_504C  0x504C   //<FA功能设置
#define PARA_504D  0x504D   //<分段开关设置
#define PARA_504E  0x504E   //<联络开关设置
#define PARA_504F  0x504F   //<FA功能设置
#define PARA_5050  0x5050   //<分段开关设置
#define PARA_5051  0x5051   //<联络开关设置
#define PARA_5052  0x5052   //<线路1 FA定值区号设置
#define PARA_5053  0x5053   //<线路2 FA定值区号设置
#define PARA_5054  0x5054   //保护公共定值
#define PARA_5055  0x5055   //保护投入与退出
#define PARA_5056  0x5056   //故障信号复归
#define PARA_5057  0x5057   //故障信号关联出口（1~4回）
#define PARA_5058  0x5058   //故障信号关联出口（5~8回）
#define PARA_5059  0x5059   //故障信号关联出口（9~12回）
#define PARA_505A  0x505A   //故障信号关联出口（13~16回）
#define PARA_505B  0x505B   //故障信号关联出口（17~20回）
#define PARA_505C  0x505C   //故障信号关联出口功能允许
#define PARA_505D  0x505D   //出口继电器软压板
#define PARA_505E  0x505E   //分回线远方就地开关设置（1~4回）
#define PARA_505F  0x505F   //分回线远方就地开关设置（5~8回）
#define PARA_5060  0x5060   //分回线远方就地开关设置（9~12回）
#define PARA_5061  0x5061   //分回线远方就地开关设置（13~16回）
#define PARA_5062  0x5062   //分回线远方就地开关设置（17~20回）
#define PARA_5063  0x5063   //分回线远方就地开关设置允许
#define PARA_5064  0x5064   //出口继电器1~16类型
#define PARA_5065  0x5065   //出口继电器17~32类型
#define PARA_5066  0x5066   //出口继电器33~48类型


#define PARA_507F  0x507F   //<基波算法还是全波算法
#define PARA_5080  0x5080   //模拟输入量的硬件端口配置
#define PARA_5081  0x5081   //模拟输入量的硬件端口配置
#define PARA_5082  0x5082   //模拟输入量的硬件端口配置
#define PARA_5083  0x5083   //模拟输入量的硬件端口配置
#define PARA_5084  0x5084   //模拟输入量的硬件端口配置
#define PARA_5085  0x5085   //开关输入量的硬件端口配置
#define PARA_5086  0x5086   //开关输入量的硬件端口配置
#define PARA_5087  0x5087   //开关输入量的硬件端口配置
#define PARA_5088  0x5088   //开关输入量的硬件端口配置
#define PARA_5089  0x5089   //双点遥信硬件端口配置
#define PARA_508A  0x508A   //开关输出量的硬件端口配置
#define PARA_508B  0x508B   //功率计算方法配置
#define PARA_508C  0x508C   //直流量配置
#define PARA_508D  0x508D   //电流标称值是5A还是1A,
#define PARA_508E  0x508E   //电压标称值 220V还是110V
#define PARA_508F  0x508F   //遥测死区值设置
#define PARA_5090  0x5090   //遥信防抖时间
#define PARA_5091  0x5091   //遥控返校失败取消遥控时间
#define PARA_5092  0x5092   //遥控保持时间
#define PARA_5093  0x5093   //遥控脉冲宽度
#define PARA_5094  0x5094   //延时断开操作电源时间
#define PARA_5095  0x5095   //PT                                               
#define PARA_5096  0x5096   //CT                                                
#define PARA_5097  0x5097   //标定
#define PARA_5098  0x5098   //遥测信息体属性配置
#define PARA_5099  0x5099   //遥测信息体属性配置
#define PARA_509A  0x509A   //遥测信息体属性配置
#define PARA_509B  0x509B   //遥测信息体属性配置
#define PARA_509C  0x509C   //遥测信息体属性配置
#define PARA_509D  0x509D   //遥测信息体属性配置
#define PARA_509E  0x509E   //遥测信息体属性配置
#define PARA_509F  0x509F   //遥测信息体属性配置
#define PARA_50A0  0x50A0   //遥测信息体属性配置
#define PARA_50A1  0x50A1   //遥测信息体属性配置
#define PARA_50A2  0x50A2   //遥测信息体属性配置
#define PARA_50A3  0x50A3   //遥测信息体属性配置
#define PARA_50A4  0x50A4   //遥测信息体属性配置
#define PARA_50A5  0x50A5   //遥测信息体属性配置
#define PARA_50A6  0x50A6   //遥测信息体属性配置
#define PARA_50A7  0x50A7   //遥测信息体属性配置
#define PARA_50A8  0x50A8   //遥测信息体属性配置
#define PARA_50A9  0x50A9   //遥测信息体属性配置
#define PARA_50AA  0x50AA   //遥测信息体属性配置
#define PARA_50AB  0x50AB   //遥测信息体属性配置
#define PARA_50AC  0x50AC   //遥测信息体属性配置
#define PARA_50AD  0x50AD   //遥信信息体属性配置
#define PARA_50AE  0x50AE   //遥信信息体属性配置
#define PARA_50AF  0x50AF   //遥信信息体属性配置
#define PARA_50B0  0x50B0   //遥信信息体属性配置
#define PARA_50B1  0x50B1   //遥信信息体属性配置
#define PARA_50B2  0x50B2   //遥信信息体属性配置
#define PARA_50B3  0x50B3   //遥信信息体属性配置
#define PARA_50B4  0x50B4   //遥信信息体属性配置
#define PARA_50B5  0x50B5   //遥信信息体属性配置
#define PARA_50B6  0x50B6   //遥信信息体属性配置
#define PARA_50B7  0x50B7   //遥信信息体属性配置
#define PARA_50B8  0x50B8   //遥信信息体属性配置
#define PARA_50B9  0x50B9   //遥信信息体属性配置
#define PARA_50BA  0x50BA   //遥信信息体属性配置
#define PARA_50BB  0x50BB   //遥信信息体属性配置
#define PARA_50BC  0x50BC   //遥信信息体属性配置
#define PARA_50BD  0x50BD   //遥信信息体属性配置
#define PARA_50BE  0x50BE   //遥信信息体属性配置
#define PARA_50BF  0x50BF   //遥信信息体属性配置
#define PARA_50C0  0x50C0   //遥信信息体属性配置
#define PARA_50C1  0x50C1   //遥信信息体属性配置
#define PARA_50C2  0x50C2   //遥信信息体属性配置
#define PARA_50C3  0x50C3   //遥信信息体属性配置
#define PARA_50C4  0x50C4   //遥信信息体属性配置
#define PARA_50C5  0x50C5   //遥信信息体属性配置
#define PARA_50C6  0x50C6   //遥信信息体属性配置
#define PARA_50C7  0x50C7   //遥信信息体属性配置
#define PARA_50C8  0x50C8   //遥信信息体属性配置
#define PARA_50C9  0x50C9   //遥信信息体属性配置
#define PARA_50CA  0x50CA   //遥信信息体属性配置
#define PARA_50CB  0x50CB   //遥控信息体属性配置

#define PARA_5100  0x5100   //101相关参数：
#define PARA_5101  0x5101   //104相关参数
#define PARA_5102  0x5102   //以太网1参数：
#define PARA_5103  0x5103   //以太网2参数：
#define PARA_5104  0x5104   //以太网3参数：
#define PARA_5105  0x5105   //以太网4参数：
#define PARA_5106  0x5106   //无线通讯参数：
#define PARA_5107  0x5107   //网络1通讯参数
#define PARA_5108  0x5108   //网络2通讯参数
#define PARA_5109  0x5109   //网络3通讯参数
#define PARA_510A  0x510A   //网络4通讯参数
#define PARA_510B  0x510B   //网络5通讯参数
#define PARA_510C  0x510C   //网络6通讯参数
#define PARA_510D  0x510D   //网络7通讯参数
#define PARA_510E  0x510E   //网络8通讯参数
#define PARA_510F  0x510F   //网络9通讯参数
#define PARA_5110  0x5110   //网络10通讯参数
#define PARA_5111  0x5111   //网络11通讯参数
#define PARA_5112  0x5112   //网络12通讯参数
#define PARA_5113  0x5113   //网络13通讯参数
#define PARA_5114  0x5114   //网络14通讯参数
#define PARA_5115  0x5115   //网络15通讯参数
#define PARA_5116  0x5116   //网络16通讯参数
#define PARA_5117  0x5117   //网络17通讯参数
#define PARA_5118  0x5118   //网络18通讯参数
#define PARA_5119  0x5119   //网络19通讯参数
#define PARA_511A  0x511A   //网络20通讯参数
#define PARA_511B  0x511B   //网络21通讯参数
#define PARA_511C  0x511C   //网络22通讯参数
#define PARA_511D  0x511D   //网络23通讯参数
#define PARA_511E  0x511E   //网络24通讯参数
#define PARA_511F  0x511F   //网络25通讯参数
#define PARA_5120  0x5120   //网络26通讯参数
#define PARA_5121  0x5121   //网络27通讯参数
#define PARA_5122  0x5122   //网络28通讯参数
#define PARA_5123  0x5123   //网络29通讯参数
#define PARA_5124  0x5124   //网络30通讯参数
#define PARA_5125  0x5125   //网络31通讯参数
#define PARA_5126  0x5126   //网络32通讯参数
#define PARA_5127  0x5127   //网络33通讯参数
#define PARA_5128  0x5128   //网络34通讯参数
#define PARA_5129  0x5129   //网络35通讯参数
#define PARA_512A  0x512A   //网络36通讯参数
#define PARA_512B  0x512B   //网络37通讯参数
#define PARA_512C  0x512C   //网络38通讯参数
#define PARA_512D  0x512D   //网络39通讯参数
#define PARA_512E  0x512E   //网络40通讯参数
#define PARA_512F  0x512F   //串行口1配置
#define PARA_5130  0x5130   //串行口2配置
#define PARA_5131  0x5131   //串行口3配置
#define PARA_5132  0x5132   //串行口4配置
#define PARA_5133  0x5133   //串行口5配置
#define PARA_5134  0x5134   //串行口6配置
#define PARA_5135  0x5135   //串行口7配置
#define PARA_5136  0x5136   //串行口8配置
#define PARA_5137  0x5137   //串行口9配置
#define PARA_5138  0x5138   //串行口10配置
#define PARA_5139  0x5139   //通讯公共参数
#define PARA_513A  0x513A   //转发口1设置
#define PARA_513B  0x513B   //转发口2设置
#define PARA_513C  0x513C   //转发口3设置
#define PARA_513D  0x513D   //转发口4设置
#define PARA_513E  0x513E   //转发口5设置
#define PARA_513F  0x513F   //转发口6设置
#define PARA_5140  0x5140   //转发口7设置
#define PARA_5141  0x5141   //转发口8设置
#define PARA_5142  0x5142   //转发口9设置
#define PARA_5143  0x5143   //转发口10设置
#define PARA_5144  0x5144   //转发口11设置
#define PARA_5145  0x5145   //转发口12设置
#define PARA_5146  0x5146   //转发口13设置
#define PARA_5147  0x5147   //转发口14设置
#define PARA_5148  0x5148   //转发口15设置
#define PARA_5149  0x5149   //转发口16设置
#define PARA_514A  0x514A   //项目参数


#define PARA_5200  0x5200   ///**密码*/
#define PARA_5201  0x5201   ///**版本*/
#define PARA_5202  0x5202   ///**温度定值*/
#define PARA_5203  0x5203   ///**湿度定值*/
#define PARA_5204  0x5204   ///**经度坐标*/
#define PARA_5205  0x5205   ///**纬度坐标*/
#define PARA_5206  0x5206   ///**恢复出厂设置*/
#define PARA_5207  0x5207   ///**蓄电池欠压切除阈值*/
#define PARA_5208  0x5208   ///**蓄电池低压告警阈值*/
#define PARA_5209  0x5209   ///**蓄电池活化充放电间隔时间*/
#define PARA_520A  0x520A   ///*电压上限告警阈值，恢复阈值，持续时间*/
#define PARA_520B  0x520B   ///*电压下限告警阈值，恢复阈值，持续时间*/
#define PARA_520C  0x520C   ///*电流上限阈值，恢复阈值，持续时间*/
#define PARA_520D  0x520D   ///*负荷越上限阈值，恢复阈值，持续时间*/
#define PARA_520E  0x520E   //零序电压上限告警参数
#define PARA_520F  0x520F   //零序电流上限告警参数
#define PARA_5210  0x5210   ///*遥测历史数据项定义*/
#define PARA_5211  0x5211   ///*遥测历史数据存储间隔*/
#define PARA_5212  0x5212   ///*遥测极值数据项定义*/
#define PARA_5213  0x5213   ///*人工值设置使能*/
#define PARA_5214  0x5214   ///*人工设置数据*/
#define PARA_5215  0x5215   ///*CP56Time2a格式终端时间*/
#define PARA_5216  0x5216   //终端类型,三遥型：D30；二遥标准型：D21；二遥动作型：D22
#define PARA_6801  0x6801/*终端32比特状态*/
//参数类数据99+77+75+24=共326个



#define SYS_7001  0x7001/*链路断开*/
#define SYS_7002  0x7002/*链路建立*/
#define SYS_7003  0x7003/*交流标定系数*/
#define SYS_7004  0x7003/*直流标定系数*/

#define SYS_7010  0x7010/*硬遥信事件记录*/
#define SYS_7011  0x7011/*软遥信事件记录*/
#define SYS_7012  0x7012/*掉电事件记录*/
#define SYS_7013  0x7013/*动作事件记录*/
#define SYS_7014  0x7014/*终端故障事件记录*/



/**
* /brief 基础应用数据类型
*/

class BaseDataType
{

public :
    int32 DataType ;///<数据类型ﺿ-类型1,2-类型2.。⾿
};

/**
* /brief 数据类型1 ：int8
*/

class DataType1: public BaseDataType
{

public:
    int8   Data ;
    DataType1() {
        DataType = 1;
    }
};

class DataType7: public BaseDataType
{

public:
    uint8   Data ;
    DataType7() {
        DataType = 7;
    }
};

/**
* /brief 数据类型2 ：int16
*/

class DataType2: public BaseDataType
{

public:
    int16 __attribute__ ((packed)) Data ;
    DataType2() {
        DataType = 2;
    }
};

/**
* /brief 数据类型3 ：int32
*/

class DataType3: public BaseDataType
{

public:
    int32 __attribute__ ((packed)) Data ;
    DataType3() {
        DataType = 3;
    }
};

/**
* /brief 数据类型4 ：float
*/

class DataType4: public BaseDataType
{

public:
    float __attribute__ ((packed)) Data ;
    DataType4() {
        DataType = 4;
        Data = 0;
    }
};

/**
* /brief 数据类型5 ：double
*/

class DataType5: public BaseDataType
{

public:
    double __attribute__ ((packed)) Data ;
    DataType5() {
        DataType = 5;
    }
};

/**
* /brief 数据类型6 ：int64
*/

class DataType6: public BaseDataType
{

public:
    int64 __attribute__ ((packed)) Data ;
    DataType6() {
        DataType = 37;
    }
};
///////////////////////////////////////////////////////////////////////////////
//参数类
/**
* /brief 数据类型6：保护参数
*/

class ProtectSet: public BaseDataType
{

public:
    ProtectSet() {
        DataType = 6;
    }

    struct {
        float LimitValue;///<限值
        int32 LimitTime;///<时间限值
        int8 Enable;///<是否投入保护(告警压板)
        int8  EnableOut;///<是否允许出口
    }  __attribute__ ((packed)) Data;
};

/**
* /brief 数据类型7：保护方向参数
*/

class ProtectDirSet: public BaseDataType
{

public:
    ProtectDirSet() {
        DataType = 7;
    }

    struct {
        int8 Enable;
        int8 Dir;
        int8 Anagle;
    }Data  ;
};

/**
* /brief 数据类型8：重合闸参数
*/

class RecloseSet: public BaseDataType
{

public:
    RecloseSet() {
        DataType = 8;
    }

    struct {
        int8  Times;///<重合闸次数
        int8  Type;///<重合闸方式
        int32 Time[3];///<三次重合闸时间
        int8  Enable;///<后加速允许
        int32 AcceTimes[4];///<后加速时间
        int32 LockTime;///<闭锁2次重合延时
        float DiffVol;///<开关两侧电压差
        float DiffPhase;///<相位差设置
        int16 DiffVolDefine[2];///<开关两侧电压端口指定

    }__attribute__ ((packed)) Data ;

};

/**
* /brief 数据类型9：FA功能设置参数
*/

class FaFunSet: public BaseDataType
{

public:
    FaFunSet() {
        DataType = 10;
    }

    struct {
        float NoShutOffCurr;///<非遮断电流
        int8  Type;///<开关方式
    }__attribute__ ((packed)) Data ;
};

/**
* /brief 数据类型11：FA功能中分段开关参数
*/

class FaIsolationSet: public BaseDataType
{

public:
    FaIsolationSet() {
        DataType = 11;
    }

    struct {
        int32 LosePowerTime;///<双侧失电延时时间（T时限）
        int8  EnableOpenSwitch;///<双侧失电延时分闸功能压板
        int32 GetPowerTime;///<单侧得电延时时间（X时限）
        int8  EnableCloseSwitch;///<单侧得电延时合闸功能压板
        int32 LockCloseSwitchTime;///<闭锁合闸时间（Y时限）
        int8  EnableLockCloseSwitch;///<闭锁合闸功能压板
        float FaultCurrent;///<闭锁合闸故障电流定值
        int8  EnableChkFaultCurrent;///<闭锁合闸功能中是否检测故障电流功能允许(如果不允许,则只检测无压)
        int32 LockOpenSwitchTime;///<无故障闭锁分闸时限
        int32  LockOpenSwitchResetTime;///<无故障闭锁分闸复归时限
        int8  EnableLockOpenSwitch;///<无故障闭锁分闸功能压板
        float RemainVol;///<残压定值
        int8  EnableChkRemainVol;///<残压检测功能允许
        float LimitU0;///<零序电压保护定值U0
        int32 LimitU0Time;///<零序电压保护时间定值
        int8  EnableU0;///<零序电压保护投入定值
    }__attribute__ ((packed)) Data ;

};

/**
* /brief 数据类型12：FA功能中联络开关参数
*/

class FaConnectionSet: public BaseDataType
{

public:
    FaConnectionSet() {
        DataType = 12;
    }

    struct {
        int32 CloseSwitchOneSideTime;///<单侧失压延时后合闸设定时间（XL时限）
        int8  EnableCloseSwitchOneSide;///<单侧失压延时后合闸功能允许
        int32 LockCloseSwitchTime;///<双侧得电闭锁开关合闸时间
        int8  EnableLockCloseSwitch;///<双侧得电闭锁开关合闸功能允许
    }__attribute__ ((packed)) Data ;

};

/**
* /brief 数据类型13：FA功能中公共参数
*/
#define BIGCURRENT 2
#define SMALLCURRENT 1
class FaCommSet: public BaseDataType
{

public:
    FaCommSet() {
        DataType = 13;
    }

    struct {
        float StartI;
        float StartU;
        float StopU;
		int32 InstantFaultChkTime;
        int32 ResetFaTime;
        int8 ResetFaType;
        int8 GroundLinkType;
    }__attribute__ ((packed)) Data ;
};

/**
* /brief 数据类型14：FA功能中投退参数
*/

class FaDevotionSet: public BaseDataType
{

public:
    FaDevotionSet() {
        DataType = 14;
    }

    struct {
        int8 EnableYCEvent;     ///<遥测越限检测功能投退
        int8 EnableCurrentProtect;  ///<基本保护功能投退
        int8 EnableFa;              ///<FA功能投退
        int8 EnableReclose;         ///<重合闸投退
        int8 EnablePtFun;           ///<当PT断线时是否自动退出方向元件以及检同期元件,FA功能
    }Data ;
};


/**
* /brief SCADA相关参数
*/
/**
* /brief 数据类型15：单点硬件配置
*/
#define YX_PORT_PARA_NUM 4
#define YC_PORT_PARA_NUM 5
#define SINGLE_DIGIT_SET_LEN 50
class SingleDigitSet: public BaseDataType
{

public:
    SingleDigitSet() {
        DataType = 15;
    }

    struct {
        int8 Num;

        struct {
            int16 Id;//对应信息体地址设置参数中的DataId
            int16 Port;
        }Define[SINGLE_DIGIT_SET_LEN];
    }__attribute__ ((packed)) Data ;
};
#define DB_DIGIT_SET_LEN 35 
/**
* /brief 数据类型16：双点硬件配置
*/
class DbDigitSet: public BaseDataType
{

public:
    DbDigitSet() {
        DataType = 16;
    }

    struct {
        int8 Num;

        struct {
            int16 Id;//对应信息体地址设置参数中的DataId
            int16 ClosePort;
            int16 OpenPort;
        }Define[DB_DIGIT_SET_LEN];
    }__attribute__ ((packed)) Data ;
};

/**
* /brief 数据类型18：遥测死区配置
*/

class DeadAreaSet: public BaseDataType
{

public:
    DeadAreaSet() {
        DataType = 18;
    }

    struct {
    	int16 DeadTime;//遥测死区时间设置
        float DeadU;   ///<电压死区设置
        float DeadI;      ///<电流死区设置
        float DeadP;      ///<功率死区设置
        float DeadFreq;   ///<频率死区设置
        float DeadAngle;///<角度死区设置
        float DeadHarmoicValue;///<谐波有效值死区设置
        float DeadHarmoicCount;///<   谐波含有率死区设置
        float DeadTemp;   ///<温度死区设置
        float DeadHumidity;///<湿度死区设置
    }__attribute__ ((packed)) Data ;
};

/**
* /brief 数据类型19：遥测信息体配置
*/

class YCMsgSet: public BaseDataType
{

public:
    YCMsgSet() {
        DataType = 19;
    }

    struct {
        int8 Num;

        struct {
            int16 DataId;
            int16 MsgId;
        }Define[50];
    }__attribute__ ((packed)) Data ;

};

/**
* /brief 数据类型20：遥信信息体配置
*/
#define YxNatureFrames 30
#define YxNaturePerNum 35
class YXMsgSet: public BaseDataType
{

public:
    YXMsgSet() {
        DataType = 20;
    }

    struct {
        int8 Num;

        struct {
            int16 DataId;
            int16 MsgId;
            int16 Type;
        }Define[YxNaturePerNum];
    }__attribute__ ((packed)) Data ;
};
/**
* /brief 数据类型21：遥控信息体配置
*/

class YKMsgSet: public BaseDataType
{

public:
    YKMsgSet() {
        DataType = 21;
    }

    struct {
        int8 Num;

        struct {
            int16 DataId;
            int16 MsgId;
        }Define[50];
    }__attribute__ ((packed)) Data ;
};




/**通讯相关参数**/
/**
* /brief 数据类型23：101规约参数
*/

class Pro101Para: public BaseDataType
{

public:
    Pro101Para() {
        DataType = 23;
    }

    struct {
        int8  TransMode;///<传输方式
        int8  AddressLen;///<地址域
        int16 RepeatTimes;///<重复帧传输时间间隔
        int8  TransCauseLen;///<传送原因
        int8  CommAddresLen;///<应用数据单元公共地址
        int8  MsgAddrLen;///<信息对象地址
    }__attribute__ ((packed)) Data ;
};

/**
* /brief 数据类型24：104规约参数
*/

class Pro104Para: public BaseDataType
{

public:
    Pro104Para() {
        DataType = 24;
    }

    struct {
        int8  CommAddresLen;///<应用数据单元公共地址
        int8  MsgAddrLen;///<信息对象地址
        int16 T0;///<连接建立的超时t0
        int16 T1;///<发送或测试 APDU 的超时t1
        int16 T2;///<无数据报文 t2<t1时确认的超时t2
        int16 T3;///<长期空闲 t3 >t1状态下发送测试帧的超时 t3
        int16 T4;///<应用报文确认超时t4
        int16 K;///<发送方最大发送未被确认的I格式报文数k
        int16 W;///<接收方最大接受未被确认I格式报文数w
    }__attribute__ ((packed)) Data ;
};

/**
* /brief 数据类型25：以太网参数
*/

class EthPara : public BaseDataType
{

public:
    EthPara() {
        DataType = 25;
    }

    struct {
        uint8 LocalIp[4];/**<  本地ip  */
        uint8 Mac[6];/**<  mac地址  */
        uint8 Mask[4];/**<  子网掩码  */
        uint8 Gate[4];/**<  网关  */
        uint8 Use;/**<  是否使用本网口  */
        uint8 LinkState;/**<  连接状态  */
        uint32 FlowSend;/**<  当前流量  */
        uint32 FlowRcv;/**<  当前流量  */
    }__attribute__ ((packed)) Data ;
} ;

class SockPara : public BaseDataType
{

public:
    SockPara() {
        DataType = 35;
    }

    struct {
    	int16	channel;//通道号
    	int16	isMaster;//是否是主通道
        int16 	Mode;/**工作模式1-服务器模式，2-客户端模式*/
        int16	Use; /**启用标志*/
        int16	TransPro; /**传输方式1:TCP;2:UDP 默认tcp*/
        uint16 	LocalPort;/**<  本地端口  */
        uint8 	HostIp[4];/*主站ip  **/
        uint16 	HostPort;/**<  主站端口  */
        int8 	ProSel;/**<  通道规约选择  */
    }__attribute__ ((packed)) Data ;
} ;
/**
* /brief 数据类型26：无线通讯参数
*/

class WirelessPara : public BaseDataType
{

public:
    WirelessPara() {
        DataType = 26;
    }

    struct {
        uint8 Vpn[32];/**<  cmnet etc. ascii 码 */
        uint8 UserName[32];/**<  用户名称  ascii 码*/
        uint8 Passwd[32];/**<  密码ascii 码 */
        uint8 LocalIp[4];/**<  本地ip  */
        
        int32 MonFlowSet;/**<  月流量限制 上下行 单位KB  */
        int8 WireLessType;/**<  网络选择 */
        uint8 Use;/**<  是否使用本网口  */
        uint8 LinkState;/**<  连接状态  */
        uint32 FlowSend;/**<  当前流量  */
        uint32 FlowRcv;/**<  当前流量  */
    }__attribute__ ((packed)) Data ;
};

/**
* /brief 数据类型27：串口参数
*/

class UartPara : public BaseDataType
{

public:
    UartPara() {
        DataType = 27;
    }

    struct {
    	int16	channel;//通道号
        int32 BaudRate;/**<  波特率:300,600,1200,2400,4800,9600,115200,38400 */
        int8 DataBit;/**<  数据位 7,8*/
        int8 StopBit;/**<  停止位 1-1位停止位,2-2位停止位,3-1。5位停止位*/
        int8 CheckBit;/**<  校验位 0:无校验，1 奇校验 2 偶校验*/
        int8  ProSel;/**<  通道规约选择  */
    }__attribute__ ((packed)) Data ;


} ;

/**
* /brief 数据类型28：基本通讯参数
*/

class ComBasePara: public BaseDataType
{

public:
    ComBasePara() {
        DataType = 28;
    }

    struct {
        int16 LocalId;/**<终端地址*/
        int16 HostId;/**<主站地址*/
        int8  YcDataType;/**<上送遥测数据类型：标度化还是归一化，短浮点数传输.默认标度化.*/
        int16 RelinkTime;/**<  断线重连间隔时间 单位 秒 */
		int16 PointsYXGroup;///<每组遥信点数
		int16 PointsYCGroup;///<每组遥测点数
    }__attribute__ ((packed)) Data ;
};
class RetransPortSet:public BaseDataType{
public:
	RetransPortSet(){DataType = 36;}
	struct{
		int16 No;//转发口设置序号,指明第几个转发口对
		uint16 TransPort;//转发通讯口,指定转发通讯口,高8位指定主口，低8位指定目标口
        int16 ByteOverTime;/**<  转发口超时等待时间 单位 ms  */
	}__attribute__ ((packed)) Data;
};
/**
* /brief 数据类型29：密码
*/

class PassWdPara: public BaseDataType
{

public:
    PassWdPara() {
        DataType = 29;
    }

    struct {
        uint8 pass[6];
    }Data  ;
};
class SwVersionPara: public BaseDataType
{

public:
    SwVersionPara() {
        DataType = 37;
    }

    struct {
        uint8 version[8];
    }Data  ;
};
/**
* /brief 数据类型30：告警参数
*/

class WarnPara: public BaseDataType
{

public:
    WarnPara() {
        DataType = 30;
    }

    struct {
        float Limit;
        float Recover;
        int32 ContinueTime;
    }__attribute__ ((packed)) Data ;
};

/**
* /brief 数据类型31：历史数据项定义参数
*/

class HisDataDefinePara: public BaseDataType
{

public:
    HisDataDefinePara() {
        DataType = 31;
    }

    struct {
        int8 Num;
        int8 DefineData[30];
    }Data  ;
};

/**
* /brief 数据类型32：人工置数参数
*/

typedef struct _BaseSet {
    float BaseValue;
    float BaseHz;
    int16 HarmoDgree;
    float HarmContent;
}__attribute__ ((packed)) BaseSet;

class UserRealDataDefPara: public BaseDataType
{
public:
    UserRealDataDefPara() {
        DataType = 32;
    }
    struct {
        BaseSet Ua;
        BaseSet Ub;
        BaseSet Uc;
        BaseSet Ia;
        BaseSet Ib;
        BaseSet Ic;
        float   IaAngle;
        float   IbAngle;
        float   IcAngle;
    }__attribute__ ((packed)) Data ;
    
};
/*直流量配置*/
class DcPara:public BaseDataType{
public:
	DcPara(){DataType = 33;}
	struct{
		int8 DcType;
		int16 NominalValue;
		
	}__attribute__ ((packed)) Data;
};

class TeminalTime:public BaseDataType{
public:
	TeminalTime(){DataType = 34;}
	struct{
		CP56Time2a time;
	}__attribute__ ((packed)) Data;
};

class TianJinProjPara:public BaseDataType{
public:
	TianJinProjPara(){DataType = 40;}
	struct{
		int16 DC_1;//直流屏电源上限
		int16 DC_2;//直流屏电源下限
	}__attribute__ ((packed)) Data;
};



/**
* /brief 应用数据类
*/
template  <class T>  class Data
{

public:
    int32 Id;//数据标识
    int16 Size;//Data成员中的Data成员（实际数值）数据长度
    T* Data;
    int32 MsgAddr;//信息体地址
    float UnitaryCoff;///<归一化系数
    float ScaleCoff;///<标度化系数
    float ShorFloCoff;///<短浮点系数
};

/**
* /brief 消息数据类
*/
#define MSG_YX    1
#define MSG_YC    2
#define MSG_YK    3
#define MSG_PARA   4
#define MSG_STEP 6


template  <class T>  class MessageData
{

public:
    uint32 Owner;//消息所有者
    int32 MessageId;//1代表遥信变化,2代表遥测变化,3代表遥控变化,4代表参数变化
    int32 Id;//数据标识
    int16 Size;//Data成员中的Data成员（实际数值）数据长度
    T* Data;
    CP56Time2a Time;//年月日时分毫秒
}__attribute__ ((packed)) ;

class UserData
{
private:		
		int16(*UserSetDataFun)(int32 );//用户调用的当调用SetData后需要调用的函数
public:
	struct cond cond;
	UserData();
	~UserData();
	int16 Init();

	int16 GetData(int32 Id,Data<BaseDataType>* data);
	int16 SetData(int32 Id,Data<BaseDataType>*data);
	int16 GetData ( int32 Id, Data<BaseDataType>* data,char * r_data);
	int16 SetData ( int32 Id, char* data );
        
  int16 GetDataSize ( int32 Id );//返回数据长度
  int16 GetDataType ( int32 Id );//返回数据类型
	Data<BaseDataType>* GetDataPtr(int32 Id);//返回数据指针
    int16 GetDataTableSize ( int32 tb );//返回数据表大小
	Data<BaseDataType>* GetDataTablePtr ( int32 tb );//返回数据表指针
	Data<BaseDataType> *RecallData(uint16 DataType, uint16 DataPosInTab);
	void SetUserDataFun ( int16(*fun)(int32 Id) );
};
extern  UserData user_data;
typedef struct da {
 DataType1 d_0001;   //线路1开关合位
 DataType1 d_0002;   //线路1开关分位
 DataType1 d_0003;   //线路1开关储能位
 DataType1 d_0004;   //线路1备用遥信1
 DataType1 d_0005;   //线路1备用遥信2
 DataType1 d_0006;   //线路1备用遥信3
 DataType1 d_0007;   //线路1一段速断故障信号
 DataType1 d_0008;   //线路1二段过流故障信号
 DataType1 d_0009;   //线路1三段过负荷故障信号
 DataType1 d_000A;   //线路1相间短路故障
 DataType1 d_000B;   //线路1单相接地故障信号
 DataType1 d_000C;   //线路1瞬时故障信号
 DataType1 d_000D;   //线路1永久故障信号
 DataType1 d_000E;   //线路1零序电压故障信号
 DataType1 d_000F;   //线路1零序电流故障信号
 DataType1 d_0010;   //线路1一段保护动作信号
 DataType1 d_0011;   //线路1二段保护动作信号
 DataType1 d_0012;   //线路1三段保护动作信号
 DataType1 d_0013;   //线路1单相接地保护动作信号
 DataType1 d_0014;   //线路1PT断线
 DataType1 d_0015;   //线路1CT断线
 DataType1 d_0016;   //线路1本地合闸
 DataType1 d_0017;   //线路1本地分闸
 DataType1 d_0018;   //线路2开关合位
 DataType1 d_0019;   //线路2开关分位
 DataType1 d_001A;   //线路2开关储能位
 DataType1 d_001B;   //线路2备用遥信1
 DataType1 d_001C;   //线路2备用遥信2
 DataType1 d_001D;   //线路2备用遥信3
 DataType1 d_001E;   //线路2一段速断故障信号
 DataType1 d_001F;   //线路2二段过流故障信号
 DataType1 d_0020;   //线路2三段过负荷故障信号
 DataType1 d_0021;   //线路2相间短路故障
 DataType1 d_0022;   //线路2单相接地故障信号
 DataType1 d_0023;   //线路2瞬时故障信号
 DataType1 d_0024;   //线路2永久故障信号
 DataType1 d_0025;   //线路2零序电压故障信号
 DataType1 d_0026;   //线路2零序电流故障信号
 DataType1 d_0027;   //线路2一段保护动作信号
 DataType1 d_0028;   //线路2二段保护动作信号
 DataType1 d_0029;   //线路2三段保护动作信号
 DataType1 d_002A;   //线路2单相接地保护动作信号
 DataType1 d_002B;   //线路2PT断线
 DataType1 d_002C;   //线路2CT断线
 DataType1 d_002D;   //线路2本地合闸
 DataType1 d_002E;   //线路2本地分闸
 DataType1 d_002F;   //线路3开关合位
 DataType1 d_0030;   //线路3开关分位
 DataType1 d_0031;   //线路3开关储能位
 DataType1 d_0032;   //线路3备用遥信1
 DataType1 d_0033;   //线路3备用遥信2
 DataType1 d_0034;   //线路3备用遥信3
 DataType1 d_0035;   //线路3一段速断故障信号
 DataType1 d_0036;   //线路3二段过流故障信号
 DataType1 d_0037;   //线路3三段过负荷故障信号
 DataType1 d_0038;   //线路3相间短路故障
 DataType1 d_0039;   //线路3单相接地故障信号
 DataType1 d_003A;   //线路3瞬时故障信号
 DataType1 d_003B;   //线路3永久故障信号
 DataType1 d_003C;   //线路3零序电压故障信号
 DataType1 d_003D;   //线路3零序电流故障信号
 DataType1 d_003E;   //线路3一段保护动作信号
 DataType1 d_003F;   //线路3二段保护动作信号
 DataType1 d_0040;   //线路3三段保护动作信号
 DataType1 d_0041;   //线路3单相接地保护动作信号
 DataType1 d_0042;   //线路3PT断线
 DataType1 d_0043;   //线路3CT断线
 DataType1 d_0044;   //线路3本地合闸
 DataType1 d_0045;   //线路3本地分闸
 DataType1 d_0046;   //线路4开关合位
 DataType1 d_0047;   //线路4开关分位
 DataType1 d_0048;   //线路4开关储能位
 DataType1 d_0049;   //线路4备用遥信1
 DataType1 d_004A;   //线路4备用遥信2
 DataType1 d_004B;   //线路4备用遥信3
 DataType1 d_004C;   //线路4一段速断故障信号
 DataType1 d_004D;   //线路4二段过流故障信号
 DataType1 d_004E;   //线路4三段过负荷故障信号
 DataType1 d_004F;   //线路4相间短路故障
 DataType1 d_0050;   //线路4单相接地故障信号
 DataType1 d_0051;   //线路4瞬时故障信号
 DataType1 d_0052;   //线路4永久故障信号
 DataType1 d_0053;   //线路4零序电压故障信号
 DataType1 d_0054;   //线路4零序电流故障信号
 DataType1 d_0055;   //线路4一段保护动作信号
 DataType1 d_0056;   //线路4二段保护动作信号
 DataType1 d_0057;   //线路4三段保护动作信号
 DataType1 d_0058;   //线路4单相接地保护动作信号
 DataType1 d_0059;   //线路4PT断线
 DataType1 d_005A;   //线路4CT断线
 DataType1 d_005B;   //线路4本地合闸
 DataType1 d_005C;   //线路4本地分闸
 DataType1 d_005D;   //线路5开关合位
 DataType1 d_005E;   //线路5开关分位
 DataType1 d_005F;   //线路5开关储能位
 DataType1 d_0060;   //线路5备用遥信1
 DataType1 d_0061;   //线路5备用遥信2
 DataType1 d_0062;   //线路5备用遥信3
 DataType1 d_0063;   //线路5一段速断故障信号
 DataType1 d_0064;   //线路5二段过流故障信号
 DataType1 d_0065;   //线路5三段过负荷故障信号
 DataType1 d_0066;   //线路5相间短路故障
 DataType1 d_0067;   //线路5单相接地故障信号
 DataType1 d_0068;   //线路5瞬时故障信号
 DataType1 d_0069;   //线路5永久故障信号
 DataType1 d_006A;   //线路5零序电压故障信号
 DataType1 d_006B;   //线路5零序电流故障信号
 DataType1 d_006C;   //线路5一段保护动作信号
 DataType1 d_006D;   //线路5二段保护动作信号
 DataType1 d_006E;   //线路5三段保护动作信号
 DataType1 d_006F;   //线路5单相接地保护动作信号
 DataType1 d_0070;   //线路5PT断线
 DataType1 d_0071;   //线路5CT断线
 DataType1 d_0072;   //线路5本地合闸
 DataType1 d_0073;   //线路5本地分闸
 DataType1 d_0074;   //线路6开关合位
 DataType1 d_0075;   //线路6开关分位
 DataType1 d_0076;   //线路6开关储能位
 DataType1 d_0077;   //线路6备用遥信1
 DataType1 d_0078;   //线路6备用遥信2
 DataType1 d_0079;   //线路6备用遥信3
 DataType1 d_007A;   //线路6一段速断故障信号
 DataType1 d_007B;   //线路6二段过流故障信号
 DataType1 d_007C;   //线路6三段过负荷故障信号
 DataType1 d_007D;   //线路6相间短路故障
 DataType1 d_007E;   //线路6单相接地故障信号
 DataType1 d_007F;   //线路6瞬时故障信号
 DataType1 d_0080;   //线路6永久故障信号
 DataType1 d_0081;   //线路6零序电压故障信号
 DataType1 d_0082;   //线路6零序电流故障信号
 DataType1 d_0083;   //线路6一段保护动作信号
 DataType1 d_0084;   //线路6二段保护动作信号
 DataType1 d_0085;   //线路6三段保护动作信号
 DataType1 d_0086;   //线路6单相接地保护动作信号
 DataType1 d_0087;   //线路6PT断线
 DataType1 d_0088;   //线路6CT断线
 DataType1 d_0089;   //线路6本地合闸
 DataType1 d_008A;   //线路6本地分闸
 DataType1 d_008B;   //线路7开关合位
 DataType1 d_008C;   //线路7开关分位
 DataType1 d_008D;   //线路7开关储能位
 DataType1 d_008E;   //线路7备用遥信1
 DataType1 d_008F;   //线路7备用遥信2
 DataType1 d_0090;   //线路7备用遥信3
 DataType1 d_0091;   //线路7一段速断故障信号
 DataType1 d_0092;   //线路7二段过流故障信号
 DataType1 d_0093;   //线路7三段过负荷故障信号
 DataType1 d_0094;   //线路7相间短路故障
 DataType1 d_0095;   //线路7单相接地故障信号
 DataType1 d_0096;   //线路7瞬时故障信号
 DataType1 d_0097;   //线路7永久故障信号
 DataType1 d_0098;   //线路7零序电压故障信号
 DataType1 d_0099;   //线路7零序电流故障信号
 DataType1 d_009A;   //线路7一段保护动作信号
 DataType1 d_009B;   //线路7二段保护动作信号
 DataType1 d_009C;   //线路7三段保护动作信号
 DataType1 d_009D;   //线路7单相接地保护动作信号
 DataType1 d_009E;   //线路7PT断线
 DataType1 d_009F;   //线路7CT断线
 DataType1 d_00A0;   //线路7本地合闸
 DataType1 d_00A1;   //线路7本地分闸
 DataType1 d_00A2;   //线路8开关合位
 DataType1 d_00A3;   //线路8开关分位
 DataType1 d_00A4;   //线路8开关储能位
 DataType1 d_00A5;   //线路8备用遥信1
 DataType1 d_00A6;   //线路8备用遥信2
 DataType1 d_00A7;   //线路8备用遥信3
 DataType1 d_00A8;   //线路8一段速断故障信号
 DataType1 d_00A9;   //线路8二段过流故障信号
 DataType1 d_00AA;   //线路8三段过负荷故障信号
 DataType1 d_00AB;   //线路8相间短路故障
 DataType1 d_00AC;   //线路8单相接地故障信号
 DataType1 d_00AD;   //线路8瞬时故障信号
 DataType1 d_00AE;   //线路8永久故障信号
 DataType1 d_00AF;   //线路8零序电压故障信号
 DataType1 d_00B0;   //线路8零序电流故障信号
 DataType1 d_00B1;   //线路8一段保护动作信号
 DataType1 d_00B2;   //线路8二段保护动作信号
 DataType1 d_00B3;   //线路8三段保护动作信号
 DataType1 d_00B4;   //线路8单相接地保护动作信号
 DataType1 d_00B5;   //线路8PT断线
 DataType1 d_00B6;   //线路8CT断线
 DataType1 d_00B7;   //线路8本地合闸
 DataType1 d_00B8;   //线路8本地分闸
 DataType1 d_00B9;   //线路9开关合位
 DataType1 d_00BA;   //线路9开关分位
 DataType1 d_00BB;   //线路9开关储能位
 DataType1 d_00BC;   //线路9备用遥信1
 DataType1 d_00BD;   //线路9备用遥信2
 DataType1 d_00BE;   //线路9备用遥信3
 DataType1 d_00BF;   //线路9一段速断故障信号
 DataType1 d_00C0;   //线路9二段过流故障信号
 DataType1 d_00C1;   //线路9三段过负荷故障信号
 DataType1 d_00C2;   //线路9相间短路故障
 DataType1 d_00C3;   //线路9单相接地故障信号
 DataType1 d_00C4;   //线路9瞬时故障信号
 DataType1 d_00C5;   //线路9永久故障信号
 DataType1 d_00C6;   //线路9零序电压故障信号
 DataType1 d_00C7;   //线路9零序电流故障信号
 DataType1 d_00C8;   //线路9一段保护动作信号
 DataType1 d_00C9;   //线路9二段保护动作信号
 DataType1 d_00CA;   //线路9三段保护动作信号
 DataType1 d_00CB;   //线路9单相接地保护动作信号
 DataType1 d_00CC;   //线路9PT断线
 DataType1 d_00CD;   //线路9CT断线
 DataType1 d_00CE;   //线路9本地合闸
 DataType1 d_00CF;   //线路9本地分闸
 DataType1 d_00D0;   //线路10开关合位
 DataType1 d_00D1;   //线路10开关分位
 DataType1 d_00D2;   //线路10开关储能位
 DataType1 d_00D3;   //线路10备用遥信1
 DataType1 d_00D4;   //线路10备用遥信2
 DataType1 d_00D5;   //线路10备用遥信3
 DataType1 d_00D6;   //线路10一段速断故障信号
 DataType1 d_00D7;   //线路10二段过流故障信号
 DataType1 d_00D8;   //线路10三段过负荷故障信号
 DataType1 d_00D9;   //线路10相间短路故障
 DataType1 d_00DA;   //线路10单相接地故障信号
 DataType1 d_00DB;   //线路10瞬时故障信号
 DataType1 d_00DC;   //线路10永久故障信号
 DataType1 d_00DD;   //线路10零序电压故障信号
 DataType1 d_00DE;   //线路10零序电流故障信号
 DataType1 d_00DF;   //线路10一段保护动作信号
 DataType1 d_00E0;   //线路10二段保护动作信号
 DataType1 d_00E1;   //线路10三段保护动作信号
 DataType1 d_00E2;   //线路10单相接地保护动作信号
 DataType1 d_00E3;   //线路10PT断线
 DataType1 d_00E4;   //线路10CT断线
 DataType1 d_00E5;   //线路10本地合闸
 DataType1 d_00E6;   //线路10本地分闸
 DataType1 d_00E7;   //线路11开关合位
 DataType1 d_00E8;   //线路11开关分位
 DataType1 d_00E9;   //线路11开关储能位
 DataType1 d_00EA;   //线路11备用遥信1
 DataType1 d_00EB;   //线路11备用遥信2
 DataType1 d_00EC;   //线路11备用遥信3
 DataType1 d_00ED;   //线路11一段速断故障信号
 DataType1 d_00EE;   //线路11二段过流故障信号
 DataType1 d_00EF;   //线路11三段过负荷故障信号
 DataType1 d_00F0;   //线路11相间短路故障
 DataType1 d_00F1;   //线路11单相接地故障信号
 DataType1 d_00F2;   //线路11瞬时故障信号
 DataType1 d_00F3;   //线路11永久故障信号
 DataType1 d_00F4;   //线路11零序电压故障信号
 DataType1 d_00F5;   //线路11零序电流故障信号
 DataType1 d_00F6;   //线路11一段保护动作信号
 DataType1 d_00F7;   //线路11二段保护动作信号
 DataType1 d_00F8;   //线路11三段保护动作信号
 DataType1 d_00F9;   //线路11单相接地保护动作信号
 DataType1 d_00FA;   //线路11PT断线
 DataType1 d_00FB;   //线路11CT断线
 DataType1 d_00FC;   //线路11本地合闸
 DataType1 d_00FD;   //线路11本地分闸
 DataType1 d_00FE;   //线路12开关合位
 DataType1 d_00FF;   //线路12开关分位
 DataType1 d_0100;   //线路12开关储能位
 DataType1 d_0101;   //线路12备用遥信1
 DataType1 d_0102;   //线路12备用遥信2
 DataType1 d_0103;   //线路12备用遥信3
 DataType1 d_0104;   //线路12一段速断故障信号
 DataType1 d_0105;   //线路12二段过流故障信号
 DataType1 d_0106;   //线路12三段过负荷故障信号
 DataType1 d_0107;   //线路12相间短路故障
 DataType1 d_0108;   //线路12单相接地故障信号
 DataType1 d_0109;   //线路12瞬时故障信号
 DataType1 d_010A;   //线路12永久故障信号
 DataType1 d_010B;   //线路12零序电压故障信号
 DataType1 d_010C;   //线路12零序电流故障信号
 DataType1 d_010D;   //线路12一段保护动作信号
 DataType1 d_010E;   //线路12二段保护动作信号
 DataType1 d_010F;   //线路12三段保护动作信号
 DataType1 d_0110;   //线路12单相接地保护动作信号
 DataType1 d_0111;   //线路12PT断线
 DataType1 d_0112;   //线路12CT断线
 DataType1 d_0113;   //线路12本地合闸
 DataType1 d_0114;   //线路12本地分闸
 DataType1 d_0115;   //线路13开关合位
 DataType1 d_0116;   //线路13开关分位
 DataType1 d_0117;   //线路13开关储能位
 DataType1 d_0118;   //线路13备用遥信1
 DataType1 d_0119;   //线路13备用遥信2
 DataType1 d_011A;   //线路13备用遥信3
 DataType1 d_011B;   //线路13一段速断故障信号
 DataType1 d_011C;   //线路13二段过流故障信号
 DataType1 d_011D;   //线路13三段过负荷故障信号
 DataType1 d_011E;   //线路13相间短路故障
 DataType1 d_011F;   //线路13单相接地故障信号
 DataType1 d_0120;   //线路13瞬时故障信号
 DataType1 d_0121;   //线路13永久故障信号
 DataType1 d_0122;   //线路13零序电压故障信号
 DataType1 d_0123;   //线路13零序电流故障信号
 DataType1 d_0124;   //线路13一段保护动作信号
 DataType1 d_0125;   //线路13二段保护动作信号
 DataType1 d_0126;   //线路13三段保护动作信号
 DataType1 d_0127;   //线路13单相接地保护动作信号
 DataType1 d_0128;   //线路13PT断线
 DataType1 d_0129;   //线路13CT断线
 DataType1 d_012A;   //线路13本地合闸
 DataType1 d_012B;   //线路13本地分闸
 DataType1 d_012C;   //线路14开关合位
 DataType1 d_012D;   //线路14开关分位
 DataType1 d_012E;   //线路14开关储能位
 DataType1 d_012F;   //线路14备用遥信1
 DataType1 d_0130;   //线路14备用遥信2
 DataType1 d_0131;   //线路14备用遥信3
 DataType1 d_0132;   //线路14一段速断故障信号
 DataType1 d_0133;   //线路14二段过流故障信号
 DataType1 d_0134;   //线路14三段过负荷故障信号
 DataType1 d_0135;   //线路14相间短路故障
 DataType1 d_0136;   //线路14单相接地故障信号
 DataType1 d_0137;   //线路14瞬时故障信号
 DataType1 d_0138;   //线路14永久故障信号
 DataType1 d_0139;   //线路14零序电压故障信号
 DataType1 d_013A;   //线路14零序电流故障信号
 DataType1 d_013B;   //线路14一段保护动作信号
 DataType1 d_013C;   //线路14二段保护动作信号
 DataType1 d_013D;   //线路14三段保护动作信号
 DataType1 d_013E;   //线路14单相接地保护动作信号
 DataType1 d_013F;   //线路14PT断线
 DataType1 d_0140;   //线路14CT断线
 DataType1 d_0141;   //线路14本地合闸
 DataType1 d_0142;   //线路14本地分闸
 DataType1 d_0143;   //线路15开关合位
 DataType1 d_0144;   //线路15开关分位
 DataType1 d_0145;   //线路15开关储能位
 DataType1 d_0146;   //线路15备用遥信1
 DataType1 d_0147;   //线路15备用遥信2
 DataType1 d_0148;   //线路15备用遥信3
 DataType1 d_0149;   //线路15一段速断故障信号
 DataType1 d_014A;   //线路15二段过流故障信号
 DataType1 d_014B;   //线路15三段过负荷故障信号
 DataType1 d_014C;   //线路15相间短路故障
 DataType1 d_014D;   //线路15单相接地故障信号
 DataType1 d_014E;   //线路15瞬时故障信号
 DataType1 d_014F;   //线路15永久故障信号
 DataType1 d_0150;   //线路15零序电压故障信号
 DataType1 d_0151;   //线路15零序电流故障信号
 DataType1 d_0152;   //线路15一段保护动作信号
 DataType1 d_0153;   //线路15二段保护动作信号
 DataType1 d_0154;   //线路15三段保护动作信号
 DataType1 d_0155;   //线路15单相接地保护动作信号
 DataType1 d_0156;   //线路15PT断线
 DataType1 d_0157;   //线路15CT断线
 DataType1 d_0158;   //线路15本地合闸
 DataType1 d_0159;   //线路15本地分闸
 DataType1 d_015A;   //线路16开关合位
 DataType1 d_015B;   //线路16开关分位
 DataType1 d_015C;   //线路16开关储能位
 DataType1 d_015D;   //线路16备用遥信1
 DataType1 d_015E;   //线路16备用遥信2
 DataType1 d_015F;   //线路16备用遥信3
 DataType1 d_0160;   //线路16一段速断故障信号
 DataType1 d_0161;   //线路16二段过流故障信号
 DataType1 d_0162;   //线路16三段过负荷故障信号
 DataType1 d_0163;   //线路16相间短路故障
 DataType1 d_0164;   //线路16单相接地故障信号
 DataType1 d_0165;   //线路16瞬时故障信号
 DataType1 d_0166;   //线路16永久故障信号
 DataType1 d_0167;   //线路16零序电压故障信号
 DataType1 d_0168;   //线路16零序电流故障信号
 DataType1 d_0169;   //线路16一段保护动作信号
 DataType1 d_016A;   //线路16二段保护动作信号
 DataType1 d_016B;   //线路16三段保护动作信号
 DataType1 d_016C;   //线路16单相接地保护动作信号
 DataType1 d_016D;   //线路16PT断线
 DataType1 d_016E;   //线路16CT断线
 DataType1 d_016F;   //线路16本地合闸
 DataType1 d_0170;   //线路16本地分闸
 DataType1 d_0171;   //断路器1双点遥信
 DataType1 d_0172;   //断路器2双点遥信
 DataType1 d_0173;   //断路器3双点遥信
 DataType1 d_0174;   //断路器4双点遥信
 DataType1 d_0175;   //断路器5双点遥信
 DataType1 d_0176;   //断路器6双点遥信
 DataType1 d_0177;   //断路器7双点遥信
 DataType1 d_0178;   //断路器8双点遥信
 DataType1 d_0179;   //断路器9双点遥信
 DataType1 d_017A;   //断路器10双点遥信
 DataType1 d_017B;   //断路器11双点遥信
 DataType1 d_017C;   //断路器12双点遥信
 DataType1 d_017D;   //断路器13双点遥信
 DataType1 d_017E;   //断路器14双点遥信
 DataType1 d_017F;   //断路器15双点遥信
 DataType1 d_0180;   //断路器16双点遥信
 DataType1 d_0181;   //线路1有压
 DataType1 d_0182;   //线路2有压
 DataType1 d_0183;   //线路3有压
 DataType1 d_0184;   //线路4有压
 DataType1 d_0185;   //线路5有压
 DataType1 d_0186;   //线路6有压
 DataType1 d_0187;   //线路7有压
 DataType1 d_0188;   //线路8有压
 DataType1 d_0189;   //线路9有压
 DataType1 d_018A;   //线路10有压
 DataType1 d_018B;   //线路11有压
 DataType1 d_018C;   //线路12有压
 DataType1 d_018D;   //线路13有压
 DataType1 d_018E;   //线路14有压
 DataType1 d_018F;   //线路15有压
 DataType1 d_0190;   //线路16有压
 DataType1 d_0191;   //公共遥信1
 DataType1 d_0192;   //公共遥信2
 DataType1 d_0193;   //公共遥信3
 DataType1 d_0194;   //公共遥信4
 DataType1 d_0195;   //远方/就地位置
 DataType1 d_0196;   //电池低压告警/恢复
 DataType1 d_0197;   //终端直流供电
 DataType1 d_0198;   //装置工作电源异常告警/恢复
 DataType1 d_0199;   //电池活化状态
 DataType1 d_019A;   //终端主交流供电
 DataType1 d_019B;   //终端备交流供电
 DataType1 d_019C;   //终端异常/恢复
 DataType1 d_019D;   //终端温度告警
 DataType1 d_019E;   //公共遥控1本地动作
 DataType1 d_019F;   //公共遥控2本地动作
 DataType1 d_01A0;   //公共遥控3本地动作
 DataType1 d_01A1;   //公共遥控4本地动作


 DataType1 d_0201;   //线路1 A相电压越上限告警
 DataType1 d_0202;   //线路1 B相电压越上限告警
 DataType1 d_0203;   //线路1 C相电压越上限告警
 DataType1 d_0204;   //线路1 AB线电压越上限告警
 DataType1 d_0205;   //线路1 CB线电压越上限告警
 DataType1 d_0206;   //线路1 零序电压越上限告警
 DataType1 d_0207;   //线路1 A相电压越下限告警
 DataType1 d_0208;   //线路1 B相电压越下限告警
 DataType1 d_0209;   //线路1 C相电压越下限告警
 DataType1 d_020A;   //线路1 AB线电压越下限告警
 DataType1 d_020B;   //线路1 CB线电压越下限告警
 DataType1 d_020C;   //线路1 A相电流越上限告警
 DataType1 d_020D;   //线路1 B相电流越上限告警
 DataType1 d_020E;   //线路1 C相电流越上限告警
 DataType1 d_020F;   //线路1 零序电流越上限告警
 DataType1 d_0210;   //线路1 负荷越上限告警
 DataType1 d_0211;   //线路2 A相电压越上限告警
 DataType1 d_0212;   //线路2 B相电压越上限告警
 DataType1 d_0213;   //线路2 C相电压越上限告警
 DataType1 d_0214;   //线路2 AB线电压越上限告警
 DataType1 d_0215;   //线路2 CB线电压越上限告警
 DataType1 d_0216;   //线路2 零序电压越上限告警
 DataType1 d_0217;   //线路2 A相电压越下限告警
 DataType1 d_0218;   //线路2 B相电压越下限告警
 DataType1 d_0219;   //线路2 C相电压越下限告警
 DataType1 d_021A;   //线路2 AB线电压越下限告警
 DataType1 d_021B;   //线路2 CB线电压越下限告警
 DataType1 d_021C;   //线路2 A相电流越上限告警
 DataType1 d_021D;   //线路2 B相电流越上限告警
 DataType1 d_021E;   //线路2 C相电流越上限告警
 DataType1 d_021F;   //线路2 零序电流越上限告警
 DataType1 d_0220;   //线路2 负荷越上限告警
 DataType1 d_0221;   //线路3 A相电压越上限告警
 DataType1 d_0222;   //线路3 B相电压越上限告警
 DataType1 d_0223;   //线路3 C相电压越上限告警
 DataType1 d_0224;   //线路3 AB线电压越上限告警
 DataType1 d_0225;   //线路3 CB线电压越上限告警
 DataType1 d_0226;   //线路3 零序电压越上限告警
 DataType1 d_0227;   //线路3 A相电压越下限告警
 DataType1 d_0228;   //线路3 B相电压越下限告警
 DataType1 d_0229;   //线路3 C相电压越下限告警
 DataType1 d_022A;   //线路3 AB线电压越下限告警
 DataType1 d_022B;   //线路3 CB线电压越下限告警
 DataType1 d_022C;   //线路3 A相电流越上限告警
 DataType1 d_022D;   //线路3 B相电流越上限告警
 DataType1 d_022E;   //线路3 C相电流越上限告警
 DataType1 d_022F;   //线路3 零序电流越上限告警
 DataType1 d_0230;   //线路3 负荷越上限告警
 DataType1 d_0231;   //线路4 A相电压越上限告警
 DataType1 d_0232;   //线路4 B相电压越上限告警
 DataType1 d_0233;   //线路4 C相电压越上限告警
 DataType1 d_0234;   //线路4 AB线电压越上限告警
 DataType1 d_0235;   //线路4 CB线电压越上限告警
 DataType1 d_0236;   //线路4 零序电压越上限告警
 DataType1 d_0237;   //线路4 A相电压越下限告警
 DataType1 d_0238;   //线路4 B相电压越下限告警
 DataType1 d_0239;   //线路4 C相电压越下限告警
 DataType1 d_023A;   //线路4 AB线电压越下限告警
 DataType1 d_023B;   //线路4 CB线电压越下限告警
 DataType1 d_023C;   //线路4 A相电流越上限告警
 DataType1 d_023D;   //线路4 B相电流越上限告警
 DataType1 d_023E;   //线路4 C相电流越上限告警
 DataType1 d_023F;   //线路4 零序电流越上限告警
 DataType1 d_0240;   //线路4 负荷越上限告警
 DataType1 d_0241;   //线路5 A相电压越上限告警
 DataType1 d_0242;   //线路5 B相电压越上限告警
 DataType1 d_0243;   //线路5 C相电压越上限告警
 DataType1 d_0244;   //线路5 AB线电压越上限告警
 DataType1 d_0245;   //线路5 CB线电压越上限告警
 DataType1 d_0246;   //线路5 零序电压越上限告警
 DataType1 d_0247;   //线路5 A相电压越下限告警
 DataType1 d_0248;   //线路5 B相电压越下限告警
 DataType1 d_0249;   //线路5 C相电压越下限告警
 DataType1 d_024A;   //线路5 AB线电压越下限告警
 DataType1 d_024B;   //线路5 CB线电压越下限告警
 DataType1 d_024C;   //线路5 A相电流越上限告警
 DataType1 d_024D;   //线路5 B相电流越上限告警
 DataType1 d_024E;   //线路5 C相电流越上限告警
 DataType1 d_024F;   //线路5 零序电流越上限告警
 DataType1 d_0250;   //线路5 负荷越上限告警
 DataType1 d_0251;   //线路6 A相电压越上限告警
 DataType1 d_0252;   //线路6 B相电压越上限告警
 DataType1 d_0253;   //线路6 C相电压越上限告警
 DataType1 d_0254;   //线路6 AB线电压越上限告警
 DataType1 d_0255;   //线路6 CB线电压越上限告警
 DataType1 d_0256;   //线路6 零序电压越上限告警
 DataType1 d_0257;   //线路6 A相电压越下限告警
 DataType1 d_0258;   //线路6 B相电压越下限告警
 DataType1 d_0259;   //线路6 C相电压越下限告警
 DataType1 d_025A;   //线路6 AB线电压越下限告警
 DataType1 d_025B;   //线路6 CB线电压越下限告警
 DataType1 d_025C;   //线路6 A相电流越上限告警
 DataType1 d_025D;   //线路6 B相电流越上限告警
 DataType1 d_025E;   //线路6 C相电流越上限告警
 DataType1 d_025F;   //线路6 零序电流越上限告警
 DataType1 d_0260;   //线路6 负荷越上限告警
 DataType1 d_0261;   //线路7 A相电压越上限告警
 DataType1 d_0262;   //线路7 B相电压越上限告警
 DataType1 d_0263;   //线路7 C相电压越上限告警
 DataType1 d_0264;   //线路7 AB线电压越上限告警
 DataType1 d_0265;   //线路7 CB线电压越上限告警
 DataType1 d_0266;   //线路7 零序电压越上限告警
 DataType1 d_0267;   //线路7 A相电压越下限告警
 DataType1 d_0268;   //线路7 B相电压越下限告警
 DataType1 d_0269;   //线路7 C相电压越下限告警
 DataType1 d_026A;   //线路7 AB线电压越下限告警
 DataType1 d_026B;   //线路7 CB线电压越下限告警
 DataType1 d_026C;   //线路7 A相电流越上限告警
 DataType1 d_026D;   //线路7 B相电流越上限告警
 DataType1 d_026E;   //线路7 C相电流越上限告警
 DataType1 d_026F;   //线路7 零序电流越上限告警
 DataType1 d_0270;   //线路7 负荷越上限告警
 DataType1 d_0271;   //线路8 A相电压越上限告警
 DataType1 d_0272;   //线路8 B相电压越上限告警
 DataType1 d_0273;   //线路8 C相电压越上限告警
 DataType1 d_0274;   //线路8 AB线电压越上限告警
 DataType1 d_0275;   //线路8 CB线电压越上限告警
 DataType1 d_0276;   //线路8 零序电压越上限告警
 DataType1 d_0277;   //线路8 A相电压越下限告警
 DataType1 d_0278;   //线路8 B相电压越下限告警
 DataType1 d_0279;   //线路8 C相电压越下限告警
 DataType1 d_027A;   //线路8 AB线电压越下限告警
 DataType1 d_027B;   //线路8 CB线电压越下限告警
 DataType1 d_027C;   //线路8 A相电流越上限告警
 DataType1 d_027D;   //线路8 B相电流越上限告警
 DataType1 d_027E;   //线路8 C相电流越上限告警
 DataType1 d_027F;   //线路8 零序电流越上限告警
 DataType1 d_0280;   //线路8 负荷越上限告警
 DataType1 d_0281;   //线路9 A相电压越上限告警
 DataType1 d_0282;   //线路9 B相电压越上限告警
 DataType1 d_0283;   //线路9 C相电压越上限告警
 DataType1 d_0284;   //线路9 AB线电压越上限告警
 DataType1 d_0285;   //线路9 CB线电压越上限告警
 DataType1 d_0286;   //线路9 零序电压越上限告警
 DataType1 d_0287;   //线路9 A相电压越下限告警
 DataType1 d_0288;   //线路9 B相电压越下限告警
 DataType1 d_0289;   //线路9 C相电压越下限告警
 DataType1 d_028A;   //线路9 AB线电压越下限告警
 DataType1 d_028B;   //线路9 CB线电压越下限告警
 DataType1 d_028C;   //线路9 A相电流越上限告警
 DataType1 d_028D;   //线路9 B相电流越上限告警
 DataType1 d_028E;   //线路9 C相电流越上限告警
 DataType1 d_028F;   //线路9 零序电流越上限告警
 DataType1 d_0290;   //线路9 负荷越上限告警
 DataType1 d_0291;   //线路10 A相电压越上限告警
 DataType1 d_0292;   //线路10 B相电压越上限告警
 DataType1 d_0293;   //线路10 C相电压越上限告警
 DataType1 d_0294;   //线路10 AB线电压越上限告警
 DataType1 d_0295;   //线路10 CB线电压越上限告警
 DataType1 d_0296;   //线路10 零序电压越上限告警
 DataType1 d_0297;   //线路10 A相电压越下限告警
 DataType1 d_0298;   //线路10 B相电压越下限告警
 DataType1 d_0299;   //线路10 C相电压越下限告警
 DataType1 d_029A;   //线路10 AB线电压越下限告警
 DataType1 d_029B;   //线路10 CB线电压越下限告警
 DataType1 d_029C;   //线路10 A相电流越上限告警
 DataType1 d_029D;   //线路10 B相电流越上限告警
 DataType1 d_029E;   //线路10 C相电流越上限告警
 DataType1 d_029F;   //线路10 零序电流越上限告警
 DataType1 d_02A0;   //线路10 负荷越上限告警
 DataType1 d_02A1;   //线路11 A相电压越上限告警
 DataType1 d_02A2;   //线路11 B相电压越上限告警
 DataType1 d_02A3;   //线路11 C相电压越上限告警
 DataType1 d_02A4;   //线路11 AB线电压越上限告警
 DataType1 d_02A5;   //线路11 CB线电压越上限告警
 DataType1 d_02A6;   //线路11 零序电压越上限告警
 DataType1 d_02A7;   //线路11 A相电压越下限告警
 DataType1 d_02A8;   //线路11 B相电压越下限告警
 DataType1 d_02A9;   //线路11 C相电压越下限告警
 DataType1 d_02AA;   //线路11 AB线电压越下限告警
 DataType1 d_02AB;   //线路11 CB线电压越下限告警
 DataType1 d_02AC;   //线路11 A相电流越上限告警
 DataType1 d_02AD;   //线路11 B相电流越上限告警
 DataType1 d_02AE;   //线路11 C相电流越上限告警
 DataType1 d_02AF;   //线路11 零序电流越上限告警
 DataType1 d_02B0;   //线路11 负荷越上限告警
 DataType1 d_02B1;   //线路12 A相电压越上限告警
 DataType1 d_02B2;   //线路12 B相电压越上限告警
 DataType1 d_02B3;   //线路12 C相电压越上限告警
 DataType1 d_02B4;   //线路12 AB线电压越上限告警
 DataType1 d_02B5;   //线路12 CB线电压越上限告警
 DataType1 d_02B6;   //线路12 零序电压越上限告警
 DataType1 d_02B7;   //线路12 A相电压越下限告警
 DataType1 d_02B8;   //线路12 B相电压越下限告警
 DataType1 d_02B9;   //线路12 C相电压越下限告警
 DataType1 d_02BA;   //线路12 AB线电压越下限告警
 DataType1 d_02BB;   //线路12 CB线电压越下限告警
 DataType1 d_02BC;   //线路12 A相电流越上限告警
 DataType1 d_02BD;   //线路12 B相电流越上限告警
 DataType1 d_02BE;   //线路12 C相电流越上限告警
 DataType1 d_02BF;   //线路12 零序电流越上限告警
 DataType1 d_02C0;   //线路12 负荷越上限告警
 DataType1 d_02C1;   //线路13 A相电压越上限告警
 DataType1 d_02C2;   //线路13 B相电压越上限告警
 DataType1 d_02C3;   //线路13 C相电压越上限告警
 DataType1 d_02C4;   //线路13 AB线电压越上限告警
 DataType1 d_02C5;   //线路13 CB线电压越上限告警
 DataType1 d_02C6;   //线路13 零序电压越上限告警
 DataType1 d_02C7;   //线路13 A相电压越下限告警
 DataType1 d_02C8;   //线路13 B相电压越下限告警
 DataType1 d_02C9;   //线路13 C相电压越下限告警
 DataType1 d_02CA;   //线路13 AB线电压越下限告警
 DataType1 d_02CB;   //线路13 CB线电压越下限告警
 DataType1 d_02CC;   //线路13 A相电流越上限告警
 DataType1 d_02CD;   //线路13 B相电流越上限告警
 DataType1 d_02CE;   //线路13 C相电流越上限告警
 DataType1 d_02CF;   //线路13 零序电流越上限告警
 DataType1 d_02D0;   //线路13 负荷越上限告警
 DataType1 d_02D1;   //线路14 A相电压越上限告警
 DataType1 d_02D2;   //线路14 B相电压越上限告警
 DataType1 d_02D3;   //线路14 C相电压越上限告警
 DataType1 d_02D4;   //线路14 AB线电压越上限告警
 DataType1 d_02D5;   //线路14 CB线电压越上限告警
 DataType1 d_02D6;   //线路14 零序电压越上限告警
 DataType1 d_02D7;   //线路14 A相电压越下限告警
 DataType1 d_02D8;   //线路14 B相电压越下限告警
 DataType1 d_02D9;   //线路14 C相电压越下限告警
 DataType1 d_02DA;   //线路14 AB线电压越下限告警
 DataType1 d_02DB;   //线路14 CB线电压越下限告警
 DataType1 d_02DC;   //线路14 A相电流越上限告警
 DataType1 d_02DD;   //线路14 B相电流越上限告警
 DataType1 d_02DE;   //线路14 C相电流越上限告警
 DataType1 d_02DF;   //线路14 零序电流越上限告警
 DataType1 d_02E0;   //线路14 负荷越上限告警
 DataType1 d_02E1;   //线路15 A相电压越上限告警
 DataType1 d_02E2;   //线路15 B相电压越上限告警
 DataType1 d_02E3;   //线路15 C相电压越上限告警
 DataType1 d_02E4;   //线路15 AB线电压越上限告警
 DataType1 d_02E5;   //线路15 CB线电压越上限告警
 DataType1 d_02E6;   //线路15 零序电压越上限告警
 DataType1 d_02E7;   //线路15 A相电压越下限告警
 DataType1 d_02E8;   //线路15 B相电压越下限告警
 DataType1 d_02E9;   //线路15 C相电压越下限告警
 DataType1 d_02EA;   //线路15 AB线电压越下限告警
 DataType1 d_02EB;   //线路15 CB线电压越下限告警
 DataType1 d_02EC;   //线路15 A相电流越上限告警
 DataType1 d_02ED;   //线路15 B相电流越上限告警
 DataType1 d_02EE;   //线路15 C相电流越上限告警
 DataType1 d_02EF;   //线路15 零序电流越上限告警
 DataType1 d_02F0;   //线路15 负荷越上限告警
 DataType1 d_02F1;   //线路16 A相电压越上限告警
 DataType1 d_02F2;   //线路16 B相电压越上限告警
 DataType1 d_02F3;   //线路16 C相电压越上限告警
 DataType1 d_02F4;   //线路16 AB线电压越上限告警
 DataType1 d_02F5;   //线路16 CB线电压越上限告警
 DataType1 d_02F6;   //线路16 零序电压越上限告警
 DataType1 d_02F7;   //线路16 A相电压越下限告警
 DataType1 d_02F8;   //线路16 B相电压越下限告警
 DataType1 d_02F9;   //线路16 C相电压越下限告警
 DataType1 d_02FA;   //线路16 AB线电压越下限告警
 DataType1 d_02FB;   //线路16 CB线电压越下限告警
 DataType1 d_02FC;   //线路16 A相电流越上限告警
 DataType1 d_02FD;   //线路16 B相电流越上限告警
 DataType1 d_02FE;   //线路16 C相电流越上限告警
 DataType1 d_02FF;   //线路16 零序电流越上限告警
 DataType1 d_0300;   //线路16 负荷越上限告警

 DataType1 d_0301;   ////故障指示器
 DataType1 d_0501;   ////备自投
 DataType1 d_0701;   ////状态监测

 DataType1 d_0801;   //配电房水位告警
 DataType1 d_0802;   //配电房火灾告警
 DataType1 d_0803;   //配电房环境温度告警
 DataType1 d_0804;   //电缆温度告警
 DataType1 d_0805;   //直流母线电压越上限告警
 DataType1 d_0806;   //直流母线电压越下限告警


 DataType4 d_4001;   //线路1 A相电压
 DataType4 d_4002;   //线路1 B相电压
 DataType4 d_4003;   //线路1 C相电压
 DataType4 d_4004;   //线路1 AB线电压
 DataType4 d_4005;   //线路1 CB线电压
 DataType4 d_4006;   //线路1 零序电压
 DataType4 d_4007;   //线路1 A相测量电流
 DataType4 d_4008;   //线路1 B相测量电流
 DataType4 d_4009;   //线路1 C相测量电流
 DataType4 d_400A;   //线路1 零序电流
 DataType4 d_400B;   //线路1 A相保护电流
 DataType4 d_400C;   //线路1 B相保护电流
 DataType4 d_400D;   //线路1 C相保护电流
 DataType4 d_400E;   //线路1 总有功功率
 DataType4 d_400F;   //线路1 总无功功率
 DataType4 d_4010;   //线路1 总视在功率
 DataType4 d_4011;   //线路1 总功率因数
 DataType4 d_4012;   //线路1 频率
 DataType4 d_4013;   //线路2 A相电压
 DataType4 d_4014;   //线路2 B相电压
 DataType4 d_4015;   //线路2 C相电压
 DataType4 d_4016;   //线路2 AB线电压
 DataType4 d_4017;   //线路2 CB线电压
 DataType4 d_4018;   //线路2 零序电压
 DataType4 d_4019;   //线路2 A相测量电流
 DataType4 d_401A;   //线路2 B相测量电流
 DataType4 d_401B;   //线路2 C相测量电流
 DataType4 d_401C;   //线路2 零序电流
 DataType4 d_401D;   //线路2 A相保护电流
 DataType4 d_401E;   //线路2 B相保护电流
 DataType4 d_401F;   //线路2 C相保护电流
 DataType4 d_4020;   //线路2 总有功功率
 DataType4 d_4021;   //线路2 总无功功率
 DataType4 d_4022;   //线路2 总视在功率
 DataType4 d_4023;   //线路2 总功率因数
 DataType4 d_4024;   //线路2 频率
 DataType4 d_4025;   //线路3 A相电压
 DataType4 d_4026;   //线路3 B相电压
 DataType4 d_4027;   //线路3 C相电压
 DataType4 d_4028;   //线路3 AB线电压
 DataType4 d_4029;   //线路3 CB线电压
 DataType4 d_402A;   //线路3 零序电压
 DataType4 d_402B;   //线路3 A相测量电流
 DataType4 d_402C;   //线路3 B相测量电流
 DataType4 d_402D;   //线路3 C相测量电流
 DataType4 d_402E;   //线路3 零序电流
 DataType4 d_402F;   //线路3 A相保护电流
 DataType4 d_4030;   //线路3 B相保护电流
 DataType4 d_4031;   //线路3 C相保护电流
 DataType4 d_4032;   //线路3 总有功功率
 DataType4 d_4033;   //线路3 总无功功率
 DataType4 d_4034;   //线路3 总视在功率
 DataType4 d_4035;   //线路3 总功率因数
 DataType4 d_4036;   //线路3 频率
 DataType4 d_4037;   //线路4 A相电压
 DataType4 d_4038;   //线路4 B相电压
 DataType4 d_4039;   //线路4 C相电压
 DataType4 d_403A;   //线路4 AB线电压
 DataType4 d_403B;   //线路4 CB线电压
 DataType4 d_403C;   //线路4 零序电压
 DataType4 d_403D;   //线路4 A相测量电流
 DataType4 d_403E;   //线路4 B相测量电流
 DataType4 d_403F;   //线路4 C相测量电流
 DataType4 d_4040;   //线路4 零序电流
 DataType4 d_4041;   //线路4 A相保护电流
 DataType4 d_4042;   //线路4 B相保护电流
 DataType4 d_4043;   //线路4 C相保护电流
 DataType4 d_4044;   //线路4 总有功功率
 DataType4 d_4045;   //线路4 总无功功率
 DataType4 d_4046;   //线路4 总视在功率
 DataType4 d_4047;   //线路4 总功率因数
 DataType4 d_4048;   //线路4 频率
 DataType4 d_4049;   //线路5 A相电压
 DataType4 d_404A;   //线路5 B相电压
 DataType4 d_404B;   //线路5 C相电压
 DataType4 d_404C;   //线路5 AB线电压
 DataType4 d_404D;   //线路5 CB线电压
 DataType4 d_404E;   //线路5 零序电压
 DataType4 d_404F;   //线路5 A相测量电流
 DataType4 d_4050;   //线路5 B相测量电流
 DataType4 d_4051;   //线路5 C相测量电流
 DataType4 d_4052;   //线路5 零序电流
 DataType4 d_4053;   //线路5 A相保护电流
 DataType4 d_4054;   //线路5 B相保护电流
 DataType4 d_4055;   //线路5 C相保护电流
 DataType4 d_4056;   //线路5 总有功功率
 DataType4 d_4057;   //线路5 总无功功率
 DataType4 d_4058;   //线路5 总视在功率
 DataType4 d_4059;   //线路5 总功率因数
 DataType4 d_405A;   //线路5 频率
 DataType4 d_405B;   //线路6 A相电压
 DataType4 d_405C;   //线路6 B相电压
 DataType4 d_405D;   //线路6 C相电压
 DataType4 d_405E;   //线路6 AB线电压
 DataType4 d_405F;   //线路6 CB线电压
 DataType4 d_4060;   //线路6 零序电压
 DataType4 d_4061;   //线路6 A相测量电流
 DataType4 d_4062;   //线路6 B相测量电流
 DataType4 d_4063;   //线路6 C相测量电流
 DataType4 d_4064;   //线路6 零序电流
 DataType4 d_4065;   //线路6 A相保护电流
 DataType4 d_4066;   //线路6 B相保护电流
 DataType4 d_4067;   //线路6 C相保护电流
 DataType4 d_4068;   //线路6 总有功功率
 DataType4 d_4069;   //线路6 总无功功率
 DataType4 d_406A;   //线路6 总视在功率
 DataType4 d_406B;   //线路6 总功率因数
 DataType4 d_406C;   //线路6 频率
 DataType4 d_406D;   //线路7 A相电压
 DataType4 d_406E;   //线路7 B相电压
 DataType4 d_406F;   //线路7 C相电压
 DataType4 d_4070;   //线路7 AB线电压
 DataType4 d_4071;   //线路7 CB线电压
 DataType4 d_4072;   //线路7 零序电压
 DataType4 d_4073;   //线路7 A相测量电流
 DataType4 d_4074;   //线路7 B相测量电流
 DataType4 d_4075;   //线路7 C相测量电流
 DataType4 d_4076;   //线路7 零序电流
 DataType4 d_4077;   //线路7 A相保护电流
 DataType4 d_4078;   //线路7 B相保护电流
 DataType4 d_4079;   //线路7 C相保护电流
 DataType4 d_407A;   //线路7 总有功功率
 DataType4 d_407B;   //线路7 总无功功率
 DataType4 d_407C;   //线路7 总视在功率
 DataType4 d_407D;   //线路7 总功率因数
 DataType4 d_407E;   //线路7 频率
 DataType4 d_407F;   //线路8 A相电压
 DataType4 d_4080;   //线路8 B相电压
 DataType4 d_4081;   //线路8 C相电压
 DataType4 d_4082;   //线路8 AB线电压
 DataType4 d_4083;   //线路8 CB线电压
 DataType4 d_4084;   //线路8 零序电压
 DataType4 d_4085;   //线路8 A相测量电流
 DataType4 d_4086;   //线路8 B相测量电流
 DataType4 d_4087;   //线路8 C相测量电流
 DataType4 d_4088;   //线路8 零序电流
 DataType4 d_4089;   //线路8 A相保护电流
 DataType4 d_408A;   //线路8 B相保护电流
 DataType4 d_408B;   //线路8 C相保护电流
 DataType4 d_408C;   //线路8 总有功功率
 DataType4 d_408D;   //线路8 总无功功率
 DataType4 d_408E;   //线路8 总视在功率
 DataType4 d_408F;   //线路8 总功率因数
 DataType4 d_4090;   //线路8 频率
 DataType4 d_4091;   //线路9 A相电压
 DataType4 d_4092;   //线路9 B相电压
 DataType4 d_4093;   //线路9 C相电压
 DataType4 d_4094;   //线路9 AB线电压
 DataType4 d_4095;   //线路9 CB线电压
 DataType4 d_4096;   //线路9 零序电压
 DataType4 d_4097;   //线路9 A相测量电流
 DataType4 d_4098;   //线路9 B相测量电流
 DataType4 d_4099;   //线路9 C相测量电流
 DataType4 d_409A;   //线路9 零序电流
 DataType4 d_409B;   //线路9 A相保护电流
 DataType4 d_409C;   //线路9 B相保护电流
 DataType4 d_409D;   //线路9 C相保护电流
 DataType4 d_409E;   //线路9 总有功功率
 DataType4 d_409F;   //线路9 总无功功率
 DataType4 d_40A0;   //线路9 总视在功率
 DataType4 d_40A1;   //线路9 总功率因数
 DataType4 d_40A2;   //线路9 频率
 DataType4 d_40A3;   //线路10 A相电压
 DataType4 d_40A4;   //线路10 B相电压
 DataType4 d_40A5;   //线路10 C相电压
 DataType4 d_40A6;   //线路10 AB线电压
 DataType4 d_40A7;   //线路10 CB线电压
 DataType4 d_40A8;   //线路10 零序电压
 DataType4 d_40A9;   //线路10 A相测量电流
 DataType4 d_40AA;   //线路10 B相测量电流
 DataType4 d_40AB;   //线路10 C相测量电流
 DataType4 d_40AC;   //线路10 零序电流
 DataType4 d_40AD;   //线路10 A相保护电流
 DataType4 d_40AE;   //线路10 B相保护电流
 DataType4 d_40AF;   //线路10 C相保护电流
 DataType4 d_40B0;   //线路10 总有功功率
 DataType4 d_40B1;   //线路10 总无功功率
 DataType4 d_40B2;   //线路10 总视在功率
 DataType4 d_40B3;   //线路10 总功率因数
 DataType4 d_40B4;   //线路10 频率
 DataType4 d_40B5;   //线路11 A相电压
 DataType4 d_40B6;   //线路11 B相电压
 DataType4 d_40B7;   //线路11 C相电压
 DataType4 d_40B8;   //线路11 AB线电压
 DataType4 d_40B9;   //线路11 CB线电压
 DataType4 d_40BA;   //线路11 零序电压
 DataType4 d_40BB;   //线路11 A相测量电流
 DataType4 d_40BC;   //线路11 B相测量电流
 DataType4 d_40BD;   //线路11 C相测量电流
 DataType4 d_40BE;   //线路11 零序电流
 DataType4 d_40BF;   //线路11 A相保护电流
 DataType4 d_40C0;   //线路11 B相保护电流
 DataType4 d_40C1;   //线路11 C相保护电流
 DataType4 d_40C2;   //线路11 总有功功率
 DataType4 d_40C3;   //线路11 总无功功率
 DataType4 d_40C4;   //线路11 总视在功率
 DataType4 d_40C5;   //线路11 总功率因数
 DataType4 d_40C6;   //线路11 频率
 DataType4 d_40C7;   //线路12 A相电压
 DataType4 d_40C8;   //线路12 B相电压
 DataType4 d_40C9;   //线路12 C相电压
 DataType4 d_40CA;   //线路12 AB线电压
 DataType4 d_40CB;   //线路12 CB线电压
 DataType4 d_40CC;   //线路12 零序电压
 DataType4 d_40CD;   //线路12 A相测量电流
 DataType4 d_40CE;   //线路12 B相测量电流
 DataType4 d_40CF;   //线路12 C相测量电流
 DataType4 d_40D0;   //线路12 零序电流
 DataType4 d_40D1;   //线路12 A相保护电流
 DataType4 d_40D2;   //线路12 B相保护电流
 DataType4 d_40D3;   //线路12 C相保护电流
 DataType4 d_40D4;   //线路12 总有功功率
 DataType4 d_40D5;   //线路12 总无功功率
 DataType4 d_40D6;   //线路12 总视在功率
 DataType4 d_40D7;   //线路12 总功率因数
 DataType4 d_40D8;   //线路12 频率
 DataType4 d_40D9;   //线路13 A相电压
 DataType4 d_40DA;   //线路13 B相电压
 DataType4 d_40DB;   //线路13 C相电压
 DataType4 d_40DC;   //线路13 AB线电压
 DataType4 d_40DD;   //线路13 CB线电压
 DataType4 d_40DE;   //线路13 零序电压
 DataType4 d_40DF;   //线路13 A相测量电流
 DataType4 d_40E0;   //线路13 B相测量电流
 DataType4 d_40E1;   //线路13 C相测量电流
 DataType4 d_40E2;   //线路13 零序电流
 DataType4 d_40E3;   //线路13 A相保护电流
 DataType4 d_40E4;   //线路13 B相保护电流
 DataType4 d_40E5;   //线路13 C相保护电流
 DataType4 d_40E6;   //线路13 总有功功率
 DataType4 d_40E7;   //线路13 总无功功率
 DataType4 d_40E8;   //线路13 总视在功率
 DataType4 d_40E9;   //线路13 总功率因数
 DataType4 d_40EA;   //线路13 频率
 DataType4 d_40EB;   //线路14 A相电压
 DataType4 d_40EC;   //线路14 B相电压
 DataType4 d_40ED;   //线路14 C相电压
 DataType4 d_40EE;   //线路14 AB线电压
 DataType4 d_40EF;   //线路14 CB线电压
 DataType4 d_40F0;   //线路14 零序电压
 DataType4 d_40F1;   //线路14 A相测量电流
 DataType4 d_40F2;   //线路14 B相测量电流
 DataType4 d_40F3;   //线路14 C相测量电流
 DataType4 d_40F4;   //线路14 零序电流
 DataType4 d_40F5;   //线路14 A相保护电流
 DataType4 d_40F6;   //线路14 B相保护电流
 DataType4 d_40F7;   //线路14 C相保护电流
 DataType4 d_40F8;   //线路14 总有功功率
 DataType4 d_40F9;   //线路14 总无功功率
 DataType4 d_40FA;   //线路14 总视在功率
 DataType4 d_40FB;   //线路14 总功率因数
 DataType4 d_40FC;   //线路14 频率
 DataType4 d_40FD;   //线路15 A相电压
 DataType4 d_40FE;   //线路15 B相电压
 DataType4 d_40FF;   //线路15 C相电压
 DataType4 d_4100;   //线路15 AB线电压
 DataType4 d_4101;   //线路15 CB线电压
 DataType4 d_4102;   //线路15 零序电压
 DataType4 d_4103;   //线路15 A相测量电流
 DataType4 d_4104;   //线路15 B相测量电流
 DataType4 d_4105;   //线路15 C相测量电流
 DataType4 d_4106;   //线路15 零序电流
 DataType4 d_4107;   //线路15 A相保护电流
 DataType4 d_4108;   //线路15 B相保护电流
 DataType4 d_4109;   //线路15 C相保护电流
 DataType4 d_410A;   //线路15 总有功功率
 DataType4 d_410B;   //线路15 总无功功率
 DataType4 d_410C;   //线路15 总视在功率
 DataType4 d_410D;   //线路15 总功率因数
 DataType4 d_410E;   //线路15 频率
 DataType4 d_410F;   //线路16 A相电压
 DataType4 d_4110;   //线路16 B相电压
 DataType4 d_4111;   //线路16 C相电压
 DataType4 d_4112;   //线路16 AB线电压
 DataType4 d_4113;   //线路16 CB线电压
 DataType4 d_4114;   //线路16 零序电压
 DataType4 d_4115;   //线路16 A相测量电流
 DataType4 d_4116;   //线路16 B相测量电流
 DataType4 d_4117;   //线路16 C相测量电流
 DataType4 d_4118;   //线路16 零序电流
 DataType4 d_4119;   //线路16 A相保护电流
 DataType4 d_411A;   //线路16 B相保护电流
 DataType4 d_411B;   //线路16 C相保护电流
 DataType4 d_411C;   //线路16 总有功功率
 DataType4 d_411D;   //线路16 总无功功率
 DataType4 d_411E;   //线路16 总视在功率
 DataType4 d_411F;   //线路16 总功率因数
 DataType4 d_4120;   //线路16 频率
 DataType4 d_4121;   //直流电源电压
 DataType4 d_4122;   //环境温度
 DataType4 d_4123;   //箱体温度
 DataType4 d_4124;   //电缆温度
 DataType4 d_4125;   //直流量1
 DataType4 d_4126;   //直流量2

 DataType4 d_4201;   //其他数据1
 DataType4 d_4401;   //其他数据2
 DataType4 d_4601;   //其他数据3
 DataType4 d_4801;   //其他数据4

 DataType4 d_4A01;   // GPS经度
 DataType4 d_4A02;   //GPS纬度

 DataType1 d_6001;   //断路器1（开/合）
 DataType1 d_6002;   //断路器2（开/合）
 DataType1 d_6003;   //断路器3（开/合）
 DataType1 d_6004;   //断路器4（开/合）
 DataType1 d_6005;   //断路器5（开/合）
 DataType1 d_6006;   //断路器6（开/合）
 DataType1 d_6007;   //断路器7（开/合）
 DataType1 d_6008;   //断路器8（开/合）
 DataType1 d_6009;   //断路器9（开/合）
 DataType1 d_600A;   //断路器10（开/合）
 DataType1 d_600B;   //断路器11（开/合）
 DataType1 d_600C;   //断路器12（开/合）
 DataType1 d_600D;   //断路器13（开/合）
 DataType1 d_600E;   //断路器14（开/合）
 DataType1 d_600F;   //断路器15（开/合）
 DataType1 d_6010;   //断路器16（开/合）

 DataType1 d_6064;   //电池活化
 DataType1 d_6065;   //公共遥控1(开合,风机启停)
 DataType1 d_6066;   //公共遥控2(开合,水泵启停)

 DataType1 d_6601;   //步调节数据
 DataType1 d_6602;   //步调节数据
 DataType1 d_6603;   //步调节数据
 DataType1 d_6604;   //步调节数据
 DataType1 d_6605;   //步调节数据
 DataType1 d_6606;   //步调节数据
 DataType1 d_6607;   //步调节数据

ProtectSet d_5001;   //第1套 I段电流定值（瞬时电流速断保护）
ProtectSet d_5002;   //第1套定值II段电流定值（定时限电流速断保护）
ProtectSet d_5003;   //第1套定值III段电流定值（定时限过电流保护）
ProtectSet d_5004;   //第1套定值零序I段电流保护定值
ProtectSet d_5005;   //第1套定值零序II段电流保护定值
ProtectSet d_5006;   //第1套定值零序III段电流保护定值
ProtectSet d_5007;   //第1套定值零序电压保护定值
ProtectDirSet  d_5008;   //第1套定值电流方向保护投入定值
ProtectDirSet  d_5009;   //第1套定值零序电流方向保护投入定值
ProtectSet d_500A;   //第2套定值I段电流定值（瞬时电流速断保护）
ProtectSet d_500B;   //第2套定值II段电流定值（定时限电流速断保护）
ProtectSet d_500C;   //第2套定值III段电流定值（定时限过电流保护）
ProtectSet d_500D;   //第2套定值零序I段电流保护定值
ProtectSet d_500E;   //第2套定值零序II段电流保护定值
ProtectSet d_500F;   //第2套定值零序III段电流保护定值
ProtectSet d_5010;   //第2套定值零序电压保护定值
ProtectDirSet  d_5011;   //第2套定值电流方向保护投入定值
ProtectDirSet  d_5012;   //第2套定值零序电流方向保护投入定值
ProtectSet d_5013;   //第3套定值I段电流定值（瞬时电流速断保护）
ProtectSet d_5014;   //第3套定值II段电流定值（定时限电流速断保护）
ProtectSet d_5015;   //第3套定值III段电流定值（定时限过电流保护）
ProtectSet d_5016;   //第3套定值零序I段电流保护定值
ProtectSet d_5017;   //第3套定值零序II段电流保护定值
ProtectSet d_5018;   //第3套定值零序III段电流保护定值
ProtectSet d_5019;   //第3套定值零序电压保护定值
ProtectDirSet  d_501A;   //第3套定值电流方向保护投入定值
ProtectDirSet  d_501B;   //第3套定值零序电流方向保护投入定值
ProtectSet d_501C;   //第4套定值I段电流定值（瞬时电流速断保护）
ProtectSet d_501D;   //第4套定值II段电流定值（定时限电流速断保护）
ProtectSet d_501E;   //第4套定值III段电流定值（定时限过电流保护）
ProtectSet d_501F;   //第4套定值零序I段电流保护定值
ProtectSet d_5020;   //第4套定值零序II段电流保护定值
ProtectSet d_5021;   //第4套定值零序III段电流保护定值
ProtectSet d_5022;   //第4套定值零序电压保护定值
ProtectDirSet  d_5023;   //第4套定值电流方向保护投入定值
ProtectDirSet  d_5024;   //第4套定值零序电流方向保护投入定值
ProtectSet d_5025;   //第5套定值I段电流定值（瞬时电流速断保护）
ProtectSet d_5026;   //第5套定值II段电流定值（定时限电流速断保护）
ProtectSet d_5027;   //第5套定值III段电流定值（定时限过电流保护）
ProtectSet d_5028;   //第5套定值零序I段电流保护定值
ProtectSet d_5029;   //第5套定值零序II段电流保护定值
ProtectSet d_502A;   //第5套定值零序III段电流保护定值
ProtectSet d_502B;   //第5套定值零序电压保护定值
ProtectDirSet  d_502C;   //第5套定值电流方向保护投入定值
ProtectDirSet  d_502D;   //第5套定值零序电流方向保护投入定值
ProtectSet d_502E;   //第6套定值I段电流定值（瞬时电流速断保护）
ProtectSet d_502F;   //第6套定值II段电流定值（定时限电流速断保护）
ProtectSet d_5030;   //第6套定值III段电流定值（定时限过电流保护）
ProtectSet d_5031;   //第6套定值零序I段电流保护定值
ProtectSet d_5032;   //第6套定值零序II段电流保护定值
ProtectSet d_5033;   //第6套定值零序III段电流保护定值
ProtectSet d_5034;   //第6套定值零序电压保护定值
ProtectDirSet  d_5035;   //第6套定值电流方向保护投入定值
ProtectDirSet  d_5036;   //第6套定值零序电流方向保护投入定值
ProtectSet d_5037;   //第7套定值I段电流定值（瞬时电流速断保护）
ProtectSet d_5038;   //第7套定值II段电流定值（定时限电流速断保护）
ProtectSet d_5039;   //第7套定值III段电流定值（定时限过电流保护）
ProtectSet d_503A;   //第7套定值零序I段电流保护定值
ProtectSet d_503B;   //第7套定值零序II段电流保护定值
ProtectSet d_503C;   //第7套定值零序III段电流保护定值
ProtectSet d_503D;   //第7套定值零序电压保护定值
ProtectDirSet  d_503E;   //第7套定值电流方向保护投入定值
ProtectDirSet  d_503F;   //第7套定值零序电流方向保护投入定值
ProtectSet d_5040;   //第8套定值I段电流定值（瞬时电流速断保护）
ProtectSet d_5041;   //第8套定值II段电流定值（定时限电流速断保护）
ProtectSet d_5042;   //第8套定值III段电流定值（定时限过电流保护）
ProtectSet d_5043;   //第8套定值零序I段电流保护定值
ProtectSet d_5044;   //第8套定值零序II段电流保护定值
ProtectSet d_5045;   //第8套定值零序III段电流保护定值
ProtectSet d_5046;   //第8套定值零序电压保护定值
ProtectDirSet  d_5047;   //第8套定值电流方向保护投入定值
ProtectDirSet  d_5048;   //第8套定值零序电流方向保护投入定值
DataType3  d_5049;   //线路1~8保护定值区号设置
DataType3  d_504A;   //线路9~16保护定值区号设置
RecloseSet d_504B;   //<重合闸设置
FaFunSet d_504C;   //<FA功能设置
FaIsolationSet d_504D;   //<分段开关设置
FaConnectionSet d_504E;   //<联络开关设置
FaFunSet d_504F;   //<FA功能设置
FaIsolationSet d_5050;   //<分段开关设置
FaConnectionSet d_5051;   //<联络开关设置
DataType1 d_5052;   //<线路1 FA定值区号设置
DataType1 d_5053;   //<线路2 FA定值区号设置
FaCommSet d_5054;   //保护公共定值
FaDevotionSet d_5055;   //保护投入与退出
DataType3 d_5056;   //故障信号复归
DataType3 d_5057;   //故障信号关联出口（1~4回）
DataType3 d_5058;   //故障信号关联出口（5~8回）
DataType3 d_5059;   //故障信号关联出口（9~12回）
DataType3 d_505A;   //故障信号关联出口（13~16回）
DataType3 d_505B;   //故障信号关联出口（17~20回）
DataType3 d_505C;   //故障信号关联出口功能允许
DataType3 d_505D;   //软压板
DataType3 d_505E;   //分回线远方就地开关设置（1~4回）
DataType3 d_505F;   //分回线远方就地开关设置（5~8回）
DataType3 d_5060;   //分回线远方就地开关设置（9~12回）
DataType3 d_5061;   //分回线远方就地开关设置（13~16回）
DataType3 d_5062;   //分回线远方就地开关设置（17~20回）
DataType3 d_5063;   //分回线远方就地开关设置允许
DataType3 d_5064;   //出口继电器1~16类型
DataType3 d_5065;   //出口继电器17~32类型
DataType3 d_5066;   //出口继电器33~48类型


DataType1 d_507F;   //<基波算法还是全波算法
SingleDigitSet d_5080;   //模拟输入量的硬件端口配置
SingleDigitSet d_5081;   //模拟输入量的硬件端口配置
SingleDigitSet d_5082;   //模拟输入量的硬件端口配置
SingleDigitSet d_5083;   //模拟输入量的硬件端口配置
SingleDigitSet d_5084;   //模拟输入量的硬件端口配置
SingleDigitSet d_5085;   //开关输入量的硬件端口配置
SingleDigitSet d_5086;   //开关输入量的硬件端口配置
SingleDigitSet d_5087;   //开关输入量的硬件端口配置
SingleDigitSet d_5088;   //开关输入量的硬件端口配置
DbDigitSet d_5089;   //双点遥信硬件端口配置
DbDigitSet d_508A;   //开关输出量的硬件端口配置
DataType1 d_508B;   //功率计算方法配置
DcPara d_508C;   //直流量配置
DataType2 d_508D;   //电流标称值是5A还是1A,
DataType4 d_508E;   //电压标称值 220V还是110V
DeadAreaSet d_508F;   //遥测死区值设置
DataType3 d_5090;   //遥信防抖时间
DataType3 d_5091;   //遥控返校失败取消遥控时间
DataType3 d_5092;   //遥控保持时间
DataType3 d_5093;   //遥控脉冲宽度
DataType3 d_5094;   //延时断开操作电源时间
DataType2 d_5095;   //PT                                               
DataType2 d_5096;   //CT                                                
DataType2 d_5097;   //标定
YCMsgSet d_5098;   //遥测信息体属性配置
YCMsgSet d_5099;   //遥测信息体属性配置
YCMsgSet d_509A;   //遥测信息体属性配置
YCMsgSet d_509B;   //遥测信息体属性配置
YCMsgSet d_509C;   //遥测信息体属性配置
YCMsgSet d_509D;   //遥测信息体属性配置
YCMsgSet d_509E;   //遥测信息体属性配置
YCMsgSet d_509F;   //遥测信息体属性配置
YCMsgSet d_50A0;   //遥测信息体属性配置
YCMsgSet d_50A1;   //遥测信息体属性配置
YCMsgSet d_50A2;   //遥测信息体属性配置
YCMsgSet d_50A3;   //遥测信息体属性配置
YCMsgSet d_50A4;   //遥测信息体属性配置
YCMsgSet d_50A5;   //遥测信息体属性配置
YCMsgSet d_50A6;   //遥测信息体属性配置
YCMsgSet d_50A7;   //遥测信息体属性配置
YCMsgSet d_50A8;   //遥测信息体属性配置
YCMsgSet d_50A9;   //遥测信息体属性配置
YCMsgSet d_50AA;   //遥测信息体属性配置
YCMsgSet d_50AB;   //遥测信息体属性配置
YCMsgSet d_50AC;   //遥测信息体属性配置
YXMsgSet d_50AD;   //遥信信息体属性配置
YXMsgSet d_50AE;   //遥信信息体属性配置
YXMsgSet d_50AF;   //遥信信息体属性配置
YXMsgSet d_50B0;   //遥信信息体属性配置
YXMsgSet d_50B1;   //遥信信息体属性配置
YXMsgSet d_50B2;   //遥信信息体属性配置
YXMsgSet d_50B3;   //遥信信息体属性配置
YXMsgSet d_50B4;   //遥信信息体属性配置
YXMsgSet d_50B5;   //遥信信息体属性配置
YXMsgSet d_50B6;   //遥信信息体属性配置
YXMsgSet d_50B7;   //遥信信息体属性配置
YXMsgSet d_50B8;   //遥信信息体属性配置
YXMsgSet d_50B9;   //遥信信息体属性配置
YXMsgSet d_50BA;   //遥信信息体属性配置
YXMsgSet d_50BB;   //遥信信息体属性配置
YXMsgSet d_50BC;   //遥信信息体属性配置
YXMsgSet d_50BD;   //遥信信息体属性配置
YXMsgSet d_50BE;   //遥信信息体属性配置
YXMsgSet d_50BF;   //遥信信息体属性配置
YXMsgSet d_50C0;   //遥信信息体属性配置
YXMsgSet d_50C1;   //遥信信息体属性配置
YXMsgSet d_50C2;   //遥信信息体属性配置
YXMsgSet d_50C3;   //遥信信息体属性配置
YXMsgSet d_50C4;   //遥信信息体属性配置
YXMsgSet d_50C5;   //遥信信息体属性配置
YXMsgSet d_50C6;   //遥信信息体属性配置
YXMsgSet d_50C7;   //遥信信息体属性配置
YXMsgSet d_50C8;   //遥信信息体属性配置
YXMsgSet d_50C9;   //遥信信息体属性配置
YXMsgSet d_50CA;   //遥信信息体属性配置
YKMsgSet d_50CB;   //遥控信息体属性配置

Pro101Para d_5100;   //101相关参数：
Pro104Para d_5101;   //104相关参数
EthPara d_5102;   //以太网1参数：
EthPara d_5103;   //以太网2参数：
EthPara d_5104;   //以太网3参数：
EthPara d_5105;   //以太网4参数：
WirelessPara d_5106;   //无线通讯参数：
SockPara d_5107;   //网络1通讯参数
SockPara d_5108;   //网络2通讯参数
SockPara d_5109;   //网络3通讯参数
SockPara d_510A;   //网络4通讯参数
SockPara d_510B;   //网络5通讯参数
SockPara d_510C;   //网络6通讯参数
SockPara d_510D;   //网络7通讯参数
SockPara d_510E;   //网络8通讯参数
SockPara d_510F;   //网络9通讯参数
SockPara d_5110;   //网络10通讯参数
SockPara d_5111;   //网络11通讯参数
SockPara d_5112;   //网络12通讯参数
SockPara d_5113;   //网络13通讯参数
SockPara d_5114;   //网络14通讯参数
SockPara d_5115;   //网络15通讯参数
SockPara d_5116;   //网络16通讯参数
SockPara d_5117;   //网络17通讯参数
SockPara d_5118;   //网络18通讯参数
SockPara d_5119;   //网络19通讯参数
SockPara d_511A;   //网络20通讯参数
SockPara d_511B;   //网络21通讯参数
SockPara d_511C;   //网络22通讯参数
SockPara d_511D;   //网络23通讯参数
SockPara d_511E;   //网络24通讯参数
SockPara d_511F;   //网络25通讯参数
SockPara d_5120;   //网络26通讯参数
SockPara d_5121;   //网络27通讯参数
SockPara d_5122;   //网络28通讯参数
SockPara d_5123;   //网络29通讯参数
SockPara d_5124;   //网络30通讯参数
SockPara d_5125;   //网络31通讯参数
SockPara d_5126;   //网络32通讯参数
SockPara d_5127;   //网络33通讯参数
SockPara d_5128;   //网络34通讯参数
SockPara d_5129;   //网络35通讯参数
SockPara d_512A;   //网络36通讯参数
SockPara d_512B;   //网络37通讯参数
SockPara d_512C;   //网络38通讯参数
SockPara d_512D;   //网络39通讯参数
SockPara d_512E;   //网络40通讯参数
UartPara d_512F;   //串行口1配置
UartPara d_5130;   //串行口2配置
UartPara d_5131;   //串行口3配置
UartPara d_5132;   //串行口4配置
UartPara d_5133;   //串行口5配置
UartPara d_5134;   //串行口6配置
UartPara d_5135;   //串行口7配置
UartPara d_5136;   //串行口8配置
UartPara d_5137;   //串行口9配置
UartPara d_5138;   //串行口10配置
ComBasePara d_5139;   //通讯公共参数
RetransPortSet d_513A;   //转发口1设置
RetransPortSet d_513B;   //转发口2设置
RetransPortSet d_513C;   //转发口3设置
RetransPortSet d_513D;   //转发口4设置
RetransPortSet d_513E;   //转发口5设置
RetransPortSet d_513F;   //转发口6设置
RetransPortSet d_5140;   //转发口7设置
RetransPortSet d_5141;   //转发口8设置
RetransPortSet d_5142;   //转发口9设置
RetransPortSet d_5143;   //转发口10设置
RetransPortSet d_5144;   //转发口11设置
RetransPortSet d_5145;   //转发口12设置
RetransPortSet d_5146;   //转发口13设置
RetransPortSet d_5147;   //转发口14设置
RetransPortSet d_5148;   //转发口15设置
RetransPortSet d_5149;   //转发口16设置

TianJinProjPara d_514A;   //项目参数



PassWdPara d_5200;   ///**密码*/
SwVersionPara d_5201;   ///**版本*/
DataType4 d_5202;   ///**温度定值*/
DataType4 d_5203;   ///**湿度定值*/
DataType4 d_5204;   ///**经度坐标*/
DataType4 d_5205;   ///**纬度坐标*/
DataType1 d_5206;   ///**恢复出厂设置*/
DataType4 d_5207;   ///**蓄电池欠压切除阈值*/
DataType4 d_5208;   ///**蓄电池低压告警阈值*/
DataType2 d_5209;   ///**蓄电池活化充放电间隔时间*/
WarnPara  d_520A;   ///*电压上限告警阈值，恢复阈值，持续时间*/
WarnPara d_520B;   ///*电压下限告警阈值，恢复阈值，持续时间*/
WarnPara d_520C;   ///*电流上限阈值，恢复阈值，持续时间*/
WarnPara d_520D;   ///*负荷越上限阈值，恢复阈值，持续时间*/
WarnPara d_520E;   //零序电压上限告警参数
WarnPara d_520F;   //零序电流上限告警参数
HisDataDefinePara d_5210;   ///*遥测历史数据项定义*/
DataType2 d_5211;   ///*遥测历史数据存储间隔*/
HisDataDefinePara d_5212;   ///*遥测极值数据项定义*/
DataType1 d_5213;   ///*人工值设置使能*/
UserRealDataDefPara d_5214;   ///*人工设置数据*/
TeminalTime d_5215;   ///*CP56Time2a格式终端时间*/
PassWdPara d_5216;   //终端类型,三遥型：D30；二遥标准型：D21；二遥动作型：D22

DataType3 d_6801;   //32比特串

}DATA_BUF;
extern DATA_BUF DataBuf;
extern  UserData user_data;
#endif
