#include <string.h>
#include "os.h"
#include "datatype.h"
#include "debug.h"
#include "custom.h"
#include "utils.h"
#include "libxml/parser.h"
/**
* /brief scada数据
*/
UserData user_data;
DATA_BUF data_buf;


#define MK_DSTB(ID) 0x##ID, sizeof(data_buf.d_##ID.Data),&data_buf.d_##ID,0x##ID,1,1,1

static Data<DataType1> YxData[] = {
    {MK_DSTB (0001 )},
    {MK_DSTB (0002 )},
    {MK_DSTB (0003 )},
    {MK_DSTB (0004 )},
    {MK_DSTB (0005 )},
    {MK_DSTB (0006 )},
    {MK_DSTB (0007 )},
    {MK_DSTB (0008 )},
    {MK_DSTB (0009 )},
    {MK_DSTB (000A )},
    {MK_DSTB (000B )},
    {MK_DSTB (000C )},
    {MK_DSTB (000D )},
    {MK_DSTB (000E )},
    {MK_DSTB (000F )},
    {MK_DSTB (0010 )},
    {MK_DSTB (0011 )},
    {MK_DSTB (0012 )},
    {MK_DSTB (0013 )},
    {MK_DSTB (0014 )},
    {MK_DSTB (0015 )},
    {MK_DSTB (0016 )},
    {MK_DSTB (0017 )},
    {MK_DSTB (0018 )},
    {MK_DSTB (0019 )},
    {MK_DSTB (001A )},
    {MK_DSTB (001B )},
    {MK_DSTB (001C )},
    {MK_DSTB (001D )},
    {MK_DSTB (001E )},
    {MK_DSTB (001F )},
    {MK_DSTB (0020 )},
    {MK_DSTB (0021 )},
    {MK_DSTB (0022 )},
    {MK_DSTB (0023 )},
    {MK_DSTB (0024 )},
    {MK_DSTB (0025 )},
    {MK_DSTB (0026 )},
    {MK_DSTB (0027 )},
    {MK_DSTB (0028 )},
    {MK_DSTB (0029 )},
    {MK_DSTB (002A )},
    {MK_DSTB (002B )},
    {MK_DSTB (002C )},
    {MK_DSTB (002D )},
    {MK_DSTB (002E )},
    {MK_DSTB (002F )},
    {MK_DSTB (0030 )},
    {MK_DSTB (0031 )},
    {MK_DSTB (0032 )},
    {MK_DSTB (0033 )},
    {MK_DSTB (0034 )},
    {MK_DSTB (0035 )},
    {MK_DSTB (0036 )},
    {MK_DSTB (0037 )},
    {MK_DSTB (0038 )},
    {MK_DSTB (0039 )},
    {MK_DSTB (003A )},
    {MK_DSTB (003B )},
    {MK_DSTB (003C )},
    {MK_DSTB (003D )},
    {MK_DSTB (003E )},
    {MK_DSTB (003F )},
    {MK_DSTB (0040 )},
    {MK_DSTB (0041 )},
    {MK_DSTB (0042 )},
    {MK_DSTB (0043 )},
    {MK_DSTB (0044 )},
    {MK_DSTB (0045 )},
    {MK_DSTB (0046 )},
    {MK_DSTB (0047 )},
    {MK_DSTB (0048 )},
    {MK_DSTB (0049 )},
    {MK_DSTB (004A )},
    {MK_DSTB (004B )},
    {MK_DSTB (004C )},
    {MK_DSTB (004D )},
    {MK_DSTB (004E )},
    {MK_DSTB (004F )},
    {MK_DSTB (0050 )},
    {MK_DSTB (0051 )},
    {MK_DSTB (0052 )},
    {MK_DSTB (0053 )},
    {MK_DSTB (0054 )},
    {MK_DSTB (0055 )},
    {MK_DSTB (0056 )},
    {MK_DSTB (0057 )},
    {MK_DSTB (0058 )},
    {MK_DSTB (0059 )},
    {MK_DSTB (005A )},
    {MK_DSTB (005B )},
    {MK_DSTB (005C )},
    {MK_DSTB (005D )},
    {MK_DSTB (005E )},
    {MK_DSTB (005F )},
    {MK_DSTB (0060 )},
    {MK_DSTB (0061 )},
    {MK_DSTB (0062 )},
    {MK_DSTB (0063 )},
    {MK_DSTB (0064 )},
    {MK_DSTB (0065 )},
    {MK_DSTB (0066 )},
    {MK_DSTB (0067 )},
    {MK_DSTB (0068 )},
    {MK_DSTB (0069 )},
    {MK_DSTB (006A )},
    {MK_DSTB (006B )},
    {MK_DSTB (006C )},
    {MK_DSTB (006D )},
    {MK_DSTB (006E )},
    {MK_DSTB (006F )},
    {MK_DSTB (0070 )},
    {MK_DSTB (0071 )},
    {MK_DSTB (0072 )},
    {MK_DSTB (0073 )},
    {MK_DSTB (0074 )},
    {MK_DSTB (0075 )},
    {MK_DSTB (0076 )},
    {MK_DSTB (0077 )},
    {MK_DSTB (0078 )},
    {MK_DSTB (0079 )},
    {MK_DSTB (007A )},
    {MK_DSTB (007B )},
    {MK_DSTB (007C )},
    {MK_DSTB (007D )},
    {MK_DSTB (007E )},
    {MK_DSTB (007F )},
    {MK_DSTB (0080 )},
    {MK_DSTB (0081 )},
    {MK_DSTB (0082 )},
    {MK_DSTB (0083 )},
    {MK_DSTB (0084 )},
    {MK_DSTB (0085 )},
    {MK_DSTB (0086 )},
    {MK_DSTB (0087 )},
    {MK_DSTB (0088 )},
    {MK_DSTB (0089 )},
    {MK_DSTB (008A )},
    {MK_DSTB (008B )},
    {MK_DSTB (008C )},
    {MK_DSTB (008D )},
    {MK_DSTB (008E )},
    {MK_DSTB (008F )},
    {MK_DSTB (0090 )},
    {MK_DSTB (0091 )},
    {MK_DSTB (0092 )},
    {MK_DSTB (0093 )},
    {MK_DSTB (0094 )},
    {MK_DSTB (0095 )},
    {MK_DSTB (0096 )},
    {MK_DSTB (0097 )},
    {MK_DSTB (0098 )},
    {MK_DSTB (0099 )},
    {MK_DSTB (009A )},
    {MK_DSTB (009B )},
    {MK_DSTB (009C )},
    {MK_DSTB (009D )},
    {MK_DSTB (009E )},
    {MK_DSTB (009F )},
    {MK_DSTB (00A0 )},
    {MK_DSTB (00A1 )},
    {MK_DSTB (00A2 )},
    {MK_DSTB (00A3 )},
    {MK_DSTB (00A4 )},
    {MK_DSTB (00A5 )},
    {MK_DSTB (00A6 )},
    {MK_DSTB (00A7 )},
    {MK_DSTB (00A8 )},
    {MK_DSTB (00A9 )},
    {MK_DSTB (00AA )},
    {MK_DSTB (00AB )},
    {MK_DSTB (00AC )},
    {MK_DSTB (00AD )},
    {MK_DSTB (00AE )},
    {MK_DSTB (00AF )},
    {MK_DSTB (00B0 )},
    {MK_DSTB (00B1 )},
    {MK_DSTB (00B2 )},
    {MK_DSTB (00B3 )},
    {MK_DSTB (00B4 )},
    {MK_DSTB (00B5 )},
    {MK_DSTB (00B6 )},
    {MK_DSTB (00B7 )},
    {MK_DSTB (00B8 )},
    {MK_DSTB (00B9 )},
    {MK_DSTB (00BA )},
    {MK_DSTB (00BB )},
    {MK_DSTB (00BC )},
    {MK_DSTB (00BD )},
    {MK_DSTB (00BE )},
    {MK_DSTB (00BF )},
    {MK_DSTB (00C0 )},
    {MK_DSTB (00C1 )},
    {MK_DSTB (00C2 )},
    {MK_DSTB (00C3 )},
    {MK_DSTB (00C4 )},
    {MK_DSTB (00C5 )},
    {MK_DSTB (00C6 )},
    {MK_DSTB (00C7 )},
    {MK_DSTB (00C8 )},
    {MK_DSTB (00C9 )},
    {MK_DSTB (00CA )},
    {MK_DSTB (00CB )},
    {MK_DSTB (00CC )},
    {MK_DSTB (00CD )},
    {MK_DSTB (00CE )},
    {MK_DSTB (00CF )},
    {MK_DSTB (00D0 )},
    {MK_DSTB (00D1 )},
    {MK_DSTB (00D2 )},
    {MK_DSTB (00D3 )},
    {MK_DSTB (00D4 )},
    {MK_DSTB (00D5 )},
    {MK_DSTB (00D6 )},
    {MK_DSTB (00D7 )},
    {MK_DSTB (00D8 )},
    {MK_DSTB (00D9 )},
    {MK_DSTB (00DA )},
    {MK_DSTB (00DB )},
    {MK_DSTB (00DC )},
    {MK_DSTB (00DD )},
    {MK_DSTB (00DE )},
    {MK_DSTB (00DF )},
    {MK_DSTB (00E0 )},
    {MK_DSTB (00E1 )},
    {MK_DSTB (00E2 )},
    {MK_DSTB (00E3 )},
    {MK_DSTB (00E4 )},
    {MK_DSTB (00E5 )},
    {MK_DSTB (00E6 )},
    {MK_DSTB (00E7 )},
    {MK_DSTB (00E8 )},
    {MK_DSTB (00E9 )},
    {MK_DSTB (00EA )},
    {MK_DSTB (00EB )},
    {MK_DSTB (00EC )},
    {MK_DSTB (00ED )},
    {MK_DSTB (00EE )},
    {MK_DSTB (00EF )},
    {MK_DSTB (00F0 )},
    {MK_DSTB (00F1 )},
    {MK_DSTB (00F2 )},
    {MK_DSTB (00F3 )},
    {MK_DSTB (00F4 )},
    {MK_DSTB (00F5 )},
    {MK_DSTB (00F6 )},
    {MK_DSTB (00F7 )},
    {MK_DSTB (00F8 )},
    {MK_DSTB (00F9 )},
    {MK_DSTB (00FA )},
    {MK_DSTB (00FB )},
    {MK_DSTB (00FC )},
    {MK_DSTB (00FD )},
    {MK_DSTB (00FE )},
    {MK_DSTB (00FF )},
    {MK_DSTB (0100 )},
    {MK_DSTB (0101 )},
    {MK_DSTB (0102 )},
    {MK_DSTB (0103 )},
    {MK_DSTB (0104 )},
    {MK_DSTB (0105 )},
    {MK_DSTB (0106 )},
    {MK_DSTB (0107 )},
    {MK_DSTB (0108 )},
    {MK_DSTB (0109 )},
    {MK_DSTB (010A )},
    {MK_DSTB (010B )},
    {MK_DSTB (010C )},
    {MK_DSTB (010D )},
    {MK_DSTB (010E )},
    {MK_DSTB (010F )},
    {MK_DSTB (0110 )},
    {MK_DSTB (0111 )},
    {MK_DSTB (0112 )},
    {MK_DSTB (0113 )},
    {MK_DSTB (0114 )},
    {MK_DSTB (0115 )},
    {MK_DSTB (0116 )},
    {MK_DSTB (0117 )},
    {MK_DSTB (0118 )},
    {MK_DSTB (0119 )},
    {MK_DSTB (011A )},
    {MK_DSTB (011B )},
    {MK_DSTB (011C )},
    {MK_DSTB (011D )},
    {MK_DSTB (011E )},
    {MK_DSTB (011F )},
    {MK_DSTB (0120 )},
    {MK_DSTB (0121 )},
    {MK_DSTB (0122 )},
    {MK_DSTB (0123 )},
    {MK_DSTB (0124 )},
    {MK_DSTB (0125 )},
    {MK_DSTB (0126 )},
    {MK_DSTB (0127 )},
    {MK_DSTB (0128 )},
    {MK_DSTB (0129 )},
    {MK_DSTB (012A )},
    {MK_DSTB (012B )},
    {MK_DSTB (012C )},
    {MK_DSTB (012D )},
    {MK_DSTB (012E )},
    {MK_DSTB (012F )},
    {MK_DSTB (0130 )},
    {MK_DSTB (0131 )},
    {MK_DSTB (0132 )},
    {MK_DSTB (0133 )},
    {MK_DSTB (0134 )},
    {MK_DSTB (0135 )},
    {MK_DSTB (0136 )},
    {MK_DSTB (0137 )},
    {MK_DSTB (0138 )},
    {MK_DSTB (0139 )},
    {MK_DSTB (013A )},
    {MK_DSTB (013B )},
    {MK_DSTB (013C )},
    {MK_DSTB (013D )},
    {MK_DSTB (013E )},
    {MK_DSTB (013F )},
    {MK_DSTB (0140 )},
    {MK_DSTB (0141 )},
    {MK_DSTB (0142 )},
    {MK_DSTB (0143 )},
    {MK_DSTB (0144 )},
    {MK_DSTB (0145 )},
    {MK_DSTB (0146 )},
    {MK_DSTB (0147 )},
    {MK_DSTB (0148 )},
    {MK_DSTB (0149 )},
    {MK_DSTB (014A )},
    {MK_DSTB (014B )},
    {MK_DSTB (014C )},
    {MK_DSTB (014D )},
    {MK_DSTB (014E )},
    {MK_DSTB (014F )},
    {MK_DSTB (0150 )},
    {MK_DSTB (0151 )},
    {MK_DSTB (0152 )},
    {MK_DSTB (0153 )},
    {MK_DSTB (0154 )},
    {MK_DSTB (0155 )},
    {MK_DSTB (0156 )},
    {MK_DSTB (0157 )},
    {MK_DSTB (0158 )},
    {MK_DSTB (0159 )},
    {MK_DSTB (015A )},
    {MK_DSTB (015B )},
    {MK_DSTB (015C )},
    {MK_DSTB (015D )},
    {MK_DSTB (015E )},
    {MK_DSTB (015F )},
    {MK_DSTB (0160 )},
    {MK_DSTB (0161 )},
    {MK_DSTB (0162 )},
    {MK_DSTB (0163 )},
    {MK_DSTB (0164 )},
    {MK_DSTB (0165 )},
    {MK_DSTB (0166 )},
    {MK_DSTB (0167 )},
    {MK_DSTB (0168 )},
    {MK_DSTB (0169 )},
    {MK_DSTB (016A )},
    {MK_DSTB (016B )},
    {MK_DSTB (016C )},
    {MK_DSTB (016D )},
    {MK_DSTB (016E )},
    {MK_DSTB (016F )},
    {MK_DSTB (0170 )},
    {MK_DSTB (0171 )},
    {MK_DSTB (0172 )},
    {MK_DSTB (0173 )},
    {MK_DSTB (0174 )},
    {MK_DSTB (0175 )},
    {MK_DSTB (0176 )},
    {MK_DSTB (0177 )},
    {MK_DSTB (0178 )},
    {MK_DSTB (0179 )},
    {MK_DSTB (017A )},
    {MK_DSTB (017B )},
    {MK_DSTB (017C )},
    {MK_DSTB (017D )},
    {MK_DSTB (017E )},
    {MK_DSTB (017F )},
    {MK_DSTB (0180 )},
    {MK_DSTB (0181 )},
    {MK_DSTB (0182 )},
    {MK_DSTB (0183 )},
    {MK_DSTB (0184 )},
    {MK_DSTB (0185 )},
    {MK_DSTB (0186 )},
    {MK_DSTB (0187 )},
    {MK_DSTB (0188 )},
    {MK_DSTB (0189 )},
    {MK_DSTB (018A )},
    {MK_DSTB (018B )},
    {MK_DSTB (018C )},
    {MK_DSTB (018D )},
    {MK_DSTB (018E )},
    {MK_DSTB (018F )},
    {MK_DSTB (0190 )},
    {MK_DSTB (0191 )},
    {MK_DSTB (0192 )},
    {MK_DSTB (0193 )},
    {MK_DSTB (0194 )},
    {MK_DSTB (0195 )},
    {MK_DSTB (0196 )},
    {MK_DSTB (0197 )},
    {MK_DSTB (0198 )},
    {MK_DSTB (0199 )},
    {MK_DSTB (019A )},
    {MK_DSTB (019B )},
    {MK_DSTB (019C )},
    {MK_DSTB (019D )},
    {MK_DSTB (019E )},
    {MK_DSTB (019F )},
    {MK_DSTB (01A0 )},
    {MK_DSTB (01A1 )},


    {MK_DSTB (0201 )},
    {MK_DSTB (0202 )},
    {MK_DSTB (0203 )},
    {MK_DSTB (0204 )},
    {MK_DSTB (0205 )},
    {MK_DSTB (0206 )},
    {MK_DSTB (0207 )},
    {MK_DSTB (0208 )},
    {MK_DSTB (0209 )},
    {MK_DSTB (020A )},
    {MK_DSTB (020B )},
    {MK_DSTB (020C )},
    {MK_DSTB (020D )},
    {MK_DSTB (020E )},
    {MK_DSTB (020F )},
    {MK_DSTB (0210 )},
    {MK_DSTB (0211 )},
    {MK_DSTB (0212 )},
    {MK_DSTB (0213 )},
    {MK_DSTB (0214 )},
    {MK_DSTB (0215 )},
    {MK_DSTB (0216 )},
    {MK_DSTB (0217 )},
    {MK_DSTB (0218 )},
    {MK_DSTB (0219 )},
    {MK_DSTB (021A )},
    {MK_DSTB (021B )},
    {MK_DSTB (021C )},
    {MK_DSTB (021D )},
    {MK_DSTB (021E )},
    {MK_DSTB (021F )},
    {MK_DSTB (0220 )},
    {MK_DSTB (0221 )},
    {MK_DSTB (0222 )},
    {MK_DSTB (0223 )},
    {MK_DSTB (0224 )},
    {MK_DSTB (0225 )},
    {MK_DSTB (0226 )},
    {MK_DSTB (0227 )},
    {MK_DSTB (0228 )},
    {MK_DSTB (0229 )},
    {MK_DSTB (022A )},
    {MK_DSTB (022B )},
    {MK_DSTB (022C )},
    {MK_DSTB (022D )},
    {MK_DSTB (022E )},
    {MK_DSTB (022F )},
    {MK_DSTB (0230 )},
    {MK_DSTB (0231 )},
    {MK_DSTB (0232 )},
    {MK_DSTB (0233 )},
    {MK_DSTB (0234 )},
    {MK_DSTB (0235 )},
    {MK_DSTB (0236 )},
    {MK_DSTB (0237 )},
    {MK_DSTB (0238 )},
    {MK_DSTB (0239 )},
    {MK_DSTB (023A )},
    {MK_DSTB (023B )},
    {MK_DSTB (023C )},
    {MK_DSTB (023D )},
    {MK_DSTB (023E )},
    {MK_DSTB (023F )},
    {MK_DSTB (0240 )},
    {MK_DSTB (0241 )},
    {MK_DSTB (0242 )},
    {MK_DSTB (0243 )},
    {MK_DSTB (0244 )},
    {MK_DSTB (0245 )},
    {MK_DSTB (0246 )},
    {MK_DSTB (0247 )},
    {MK_DSTB (0248 )},
    {MK_DSTB (0249 )},
    {MK_DSTB (024A )},
    {MK_DSTB (024B )},
    {MK_DSTB (024C )},
    {MK_DSTB (024D )},
    {MK_DSTB (024E )},
    {MK_DSTB (024F )},
    {MK_DSTB (0250 )},
    {MK_DSTB (0251 )},
    {MK_DSTB (0252 )},
    {MK_DSTB (0253 )},
    {MK_DSTB (0254 )},
    {MK_DSTB (0255 )},
    {MK_DSTB (0256 )},
    {MK_DSTB (0257 )},
    {MK_DSTB (0258 )},
    {MK_DSTB (0259 )},
    {MK_DSTB (025A )},
    {MK_DSTB (025B )},
    {MK_DSTB (025C )},
    {MK_DSTB (025D )},
    {MK_DSTB (025E )},
    {MK_DSTB (025F )},
    {MK_DSTB (0260 )},
    {MK_DSTB (0261 )},
    {MK_DSTB (0262 )},
    {MK_DSTB (0263 )},
    {MK_DSTB (0264 )},
    {MK_DSTB (0265 )},
    {MK_DSTB (0266 )},
    {MK_DSTB (0267 )},
    {MK_DSTB (0268 )},
    {MK_DSTB (0269 )},
    {MK_DSTB (026A )},
    {MK_DSTB (026B )},
    {MK_DSTB (026C )},
    {MK_DSTB (026D )},
    {MK_DSTB (026E )},
    {MK_DSTB (026F )},
    {MK_DSTB (0270 )},
    {MK_DSTB (0271 )},
    {MK_DSTB (0272 )},
    {MK_DSTB (0273 )},
    {MK_DSTB (0274 )},
    {MK_DSTB (0275 )},
    {MK_DSTB (0276 )},
    {MK_DSTB (0277 )},
    {MK_DSTB (0278 )},
    {MK_DSTB (0279 )},
    {MK_DSTB (027A )},
    {MK_DSTB (027B )},
    {MK_DSTB (027C )},
    {MK_DSTB (027D )},
    {MK_DSTB (027E )},
    {MK_DSTB (027F )},
    {MK_DSTB (0280 )},
    {MK_DSTB (0281 )},
    {MK_DSTB (0282 )},
    {MK_DSTB (0283 )},
    {MK_DSTB (0284 )},
    {MK_DSTB (0285 )},
    {MK_DSTB (0286 )},
    {MK_DSTB (0287 )},
    {MK_DSTB (0288 )},
    {MK_DSTB (0289 )},
    {MK_DSTB (028A )},
    {MK_DSTB (028B )},
    {MK_DSTB (028C )},
    {MK_DSTB (028D )},
    {MK_DSTB (028E )},
    {MK_DSTB (028F )},
    {MK_DSTB (0290 )},
    {MK_DSTB (0291 )},
    {MK_DSTB (0292 )},
    {MK_DSTB (0293 )},
    {MK_DSTB (0294 )},
    {MK_DSTB (0295 )},
    {MK_DSTB (0296 )},
    {MK_DSTB (0297 )},
    {MK_DSTB (0298 )},
    {MK_DSTB (0299 )},
    {MK_DSTB (029A )},
    {MK_DSTB (029B )},
    {MK_DSTB (029C )},
    {MK_DSTB (029D )},
    {MK_DSTB (029E )},
    {MK_DSTB (029F )},
    {MK_DSTB (02A0 )},
    {MK_DSTB (02A1 )},
    {MK_DSTB (02A2 )},
    {MK_DSTB (02A3 )},
    {MK_DSTB (02A4 )},
    {MK_DSTB (02A5 )},
    {MK_DSTB (02A6 )},
    {MK_DSTB (02A7 )},
    {MK_DSTB (02A8 )},
    {MK_DSTB (02A9 )},
    {MK_DSTB (02AA )},
    {MK_DSTB (02AB )},
    {MK_DSTB (02AC )},
    {MK_DSTB (02AD )},
    {MK_DSTB (02AE )},
    {MK_DSTB (02AF )},
    {MK_DSTB (02B0 )},
    {MK_DSTB (02B1 )},
    {MK_DSTB (02B2 )},
    {MK_DSTB (02B3 )},
    {MK_DSTB (02B4 )},
    {MK_DSTB (02B5 )},
    {MK_DSTB (02B6 )},
    {MK_DSTB (02B7 )},
    {MK_DSTB (02B8 )},
    {MK_DSTB (02B9 )},
    {MK_DSTB (02BA )},
    {MK_DSTB (02BB )},
    {MK_DSTB (02BC )},
    {MK_DSTB (02BD )},
    {MK_DSTB (02BE )},
    {MK_DSTB (02BF )},
    {MK_DSTB (02C0 )},
    {MK_DSTB (02C1 )},
    {MK_DSTB (02C2 )},
    {MK_DSTB (02C3 )},
    {MK_DSTB (02C4 )},
    {MK_DSTB (02C5 )},
    {MK_DSTB (02C6 )},
    {MK_DSTB (02C7 )},
    {MK_DSTB (02C8 )},
    {MK_DSTB (02C9 )},
    {MK_DSTB (02CA )},
    {MK_DSTB (02CB )},
    {MK_DSTB (02CC )},
    {MK_DSTB (02CD )},
    {MK_DSTB (02CE )},
    {MK_DSTB (02CF )},
    {MK_DSTB (02D0 )},
    {MK_DSTB (02D1 )},
    {MK_DSTB (02D2 )},
    {MK_DSTB (02D3 )},
    {MK_DSTB (02D4 )},
    {MK_DSTB (02D5 )},
    {MK_DSTB (02D6 )},
    {MK_DSTB (02D7 )},
    {MK_DSTB (02D8 )},
    {MK_DSTB (02D9 )},
    {MK_DSTB (02DA )},
    {MK_DSTB (02DB )},
    {MK_DSTB (02DC )},
    {MK_DSTB (02DD )},
    {MK_DSTB (02DE )},
    {MK_DSTB (02DF )},
    {MK_DSTB (02E0 )},
    {MK_DSTB (02E1 )},
    {MK_DSTB (02E2 )},
    {MK_DSTB (02E3 )},
    {MK_DSTB (02E4 )},
    {MK_DSTB (02E5 )},
    {MK_DSTB (02E6 )},
    {MK_DSTB (02E7 )},
    {MK_DSTB (02E8 )},
    {MK_DSTB (02E9 )},
    {MK_DSTB (02EA )},
    {MK_DSTB (02EB )},
    {MK_DSTB (02EC )},
    {MK_DSTB (02ED )},
    {MK_DSTB (02EE )},
    {MK_DSTB (02EF )},
    {MK_DSTB (02F0 )},
    {MK_DSTB (02F1 )},
    {MK_DSTB (02F2 )},
    {MK_DSTB (02F3 )},
    {MK_DSTB (02F4 )},
    {MK_DSTB (02F5 )},
    {MK_DSTB (02F6 )},
    {MK_DSTB (02F7 )},
    {MK_DSTB (02F8 )},
    {MK_DSTB (02F9 )},
    {MK_DSTB (02FA )},
    {MK_DSTB (02FB )},
    {MK_DSTB (02FC )},
    {MK_DSTB (02FD )},
    {MK_DSTB (02FE )},
    {MK_DSTB (02FF )},
    {MK_DSTB (0300 )},

    {MK_DSTB (0301 )},
    {MK_DSTB (0501 )},
    {MK_DSTB (0701 )},


    {MK_DSTB (0801 )},
    {MK_DSTB (0802 )},
    {MK_DSTB (0803 )},
    {MK_DSTB (0804 )},
    {MK_DSTB (0805 )},
    {MK_DSTB (0806 )},



};
static Data<DataType4> YcData[] = {
    {MK_DSTB (4001 )},
    {MK_DSTB (4002 )},
    {MK_DSTB (4003 )},
    {MK_DSTB (4004 )},
    {MK_DSTB (4005 )},
    {MK_DSTB (4006 )},
    {MK_DSTB (4007 )},
    {MK_DSTB (4008 )},
    {MK_DSTB (4009 )},
    {MK_DSTB (400A )},
    {MK_DSTB (400B )},
    {MK_DSTB (400C )},
    {MK_DSTB (400D )},
    {MK_DSTB (400E )},
    {MK_DSTB (400F )},
    {MK_DSTB (4010 )},
    {MK_DSTB (4011 )},
    {MK_DSTB (4012 )},
    {MK_DSTB (4013 )},
    {MK_DSTB (4014 )},
    {MK_DSTB (4015 )},
    {MK_DSTB (4016 )},
    {MK_DSTB (4017 )},
    {MK_DSTB (4018 )},
    {MK_DSTB (4019 )},
    {MK_DSTB (401A )},
    {MK_DSTB (401B )},
    {MK_DSTB (401C )},
    {MK_DSTB (401D )},
    {MK_DSTB (401E )},
    {MK_DSTB (401F )},
    {MK_DSTB (4020 )},
    {MK_DSTB (4021 )},
    {MK_DSTB (4022 )},
    {MK_DSTB (4023 )},
    {MK_DSTB (4024 )},
    {MK_DSTB (4025 )},
    {MK_DSTB (4026 )},
    {MK_DSTB (4027 )},
    {MK_DSTB (4028 )},
    {MK_DSTB (4029 )},
    {MK_DSTB (402A )},
    {MK_DSTB (402B )},
    {MK_DSTB (402C )},
    {MK_DSTB (402D )},
    {MK_DSTB (402E )},
    {MK_DSTB (402F )},
    {MK_DSTB (4030 )},
    {MK_DSTB (4031 )},
    {MK_DSTB (4032 )},
    {MK_DSTB (4033 )},
    {MK_DSTB (4034 )},
    {MK_DSTB (4035 )},
    {MK_DSTB (4036 )},
    {MK_DSTB (4037 )},
    {MK_DSTB (4038 )},
    {MK_DSTB (4039 )},
    {MK_DSTB (403A )},
    {MK_DSTB (403B )},
    {MK_DSTB (403C )},
    {MK_DSTB (403D )},
    {MK_DSTB (403E )},
    {MK_DSTB (403F )},
    {MK_DSTB (4040 )},
    {MK_DSTB (4041 )},
    {MK_DSTB (4042 )},
    {MK_DSTB (4043 )},
    {MK_DSTB (4044 )},
    {MK_DSTB (4045 )},
    {MK_DSTB (4046 )},
    {MK_DSTB (4047 )},
    {MK_DSTB (4048 )},
    {MK_DSTB (4049 )},
    {MK_DSTB (404A )},
    {MK_DSTB (404B )},
    {MK_DSTB (404C )},
    {MK_DSTB (404D )},
    {MK_DSTB (404E )},
    {MK_DSTB (404F )},
    {MK_DSTB (4050 )},
    {MK_DSTB (4051 )},
    {MK_DSTB (4052 )},
    {MK_DSTB (4053 )},
    {MK_DSTB (4054 )},
    {MK_DSTB (4055 )},
    {MK_DSTB (4056 )},
    {MK_DSTB (4057 )},
    {MK_DSTB (4058 )},
    {MK_DSTB (4059 )},
    {MK_DSTB (405A )},
    {MK_DSTB (405B )},
    {MK_DSTB (405C )},
    {MK_DSTB (405D )},
    {MK_DSTB (405E )},
    {MK_DSTB (405F )},
    {MK_DSTB (4060 )},
    {MK_DSTB (4061 )},
    {MK_DSTB (4062 )},
    {MK_DSTB (4063 )},
    {MK_DSTB (4064 )},
    {MK_DSTB (4065 )},
    {MK_DSTB (4066 )},
    {MK_DSTB (4067 )},
    {MK_DSTB (4068 )},
    {MK_DSTB (4069 )},
    {MK_DSTB (406A )},
    {MK_DSTB (406B )},
    {MK_DSTB (406C )},
    {MK_DSTB (406D )},
    {MK_DSTB (406E )},
    {MK_DSTB (406F )},
    {MK_DSTB (4070 )},
    {MK_DSTB (4071 )},
    {MK_DSTB (4072 )},
    {MK_DSTB (4073 )},
    {MK_DSTB (4074 )},
    {MK_DSTB (4075 )},
    {MK_DSTB (4076 )},
    {MK_DSTB (4077 )},
    {MK_DSTB (4078 )},
    {MK_DSTB (4079 )},
    {MK_DSTB (407A )},
    {MK_DSTB (407B )},
    {MK_DSTB (407C )},
    {MK_DSTB (407D )},
    {MK_DSTB (407E )},
    {MK_DSTB (407F )},
    {MK_DSTB (4080 )},
    {MK_DSTB (4081 )},
    {MK_DSTB (4082 )},
    {MK_DSTB (4083 )},
    {MK_DSTB (4084 )},
    {MK_DSTB (4085 )},
    {MK_DSTB (4086 )},
    {MK_DSTB (4087 )},
    {MK_DSTB (4088 )},
    {MK_DSTB (4089 )},
    {MK_DSTB (408A )},
    {MK_DSTB (408B )},
    {MK_DSTB (408C )},
    {MK_DSTB (408D )},
    {MK_DSTB (408E )},
    {MK_DSTB (408F )},
    {MK_DSTB (4090 )},
    {MK_DSTB (4091 )},
    {MK_DSTB (4092 )},
    {MK_DSTB (4093 )},
    {MK_DSTB (4094 )},
    {MK_DSTB (4095 )},
    {MK_DSTB (4096 )},
    {MK_DSTB (4097 )},
    {MK_DSTB (4098 )},
    {MK_DSTB (4099 )},
    {MK_DSTB (409A )},
    {MK_DSTB (409B )},
    {MK_DSTB (409C )},
    {MK_DSTB (409D )},
    {MK_DSTB (409E )},
    {MK_DSTB (409F )},
    {MK_DSTB (40A0 )},
    {MK_DSTB (40A1 )},
    {MK_DSTB (40A2 )},
    {MK_DSTB (40A3 )},
    {MK_DSTB (40A4 )},
    {MK_DSTB (40A5 )},
    {MK_DSTB (40A6 )},
    {MK_DSTB (40A7 )},
    {MK_DSTB (40A8 )},
    {MK_DSTB (40A9 )},
    {MK_DSTB (40AA )},
    {MK_DSTB (40AB )},
    {MK_DSTB (40AC )},
    {MK_DSTB (40AD )},
    {MK_DSTB (40AE )},
    {MK_DSTB (40AF )},
    {MK_DSTB (40B0 )},
    {MK_DSTB (40B1 )},
    {MK_DSTB (40B2 )},
    {MK_DSTB (40B3 )},
    {MK_DSTB (40B4 )},
    {MK_DSTB (40B5 )},
    {MK_DSTB (40B6 )},
    {MK_DSTB (40B7 )},
    {MK_DSTB (40B8 )},
    {MK_DSTB (40B9 )},
    {MK_DSTB (40BA )},
    {MK_DSTB (40BB )},
    {MK_DSTB (40BC )},
    {MK_DSTB (40BD )},
    {MK_DSTB (40BE )},
    {MK_DSTB (40BF )},
    {MK_DSTB (40C0 )},
    {MK_DSTB (40C1 )},
    {MK_DSTB (40C2 )},
    {MK_DSTB (40C3 )},
    {MK_DSTB (40C4 )},
    {MK_DSTB (40C5 )},
    {MK_DSTB (40C6 )},
    {MK_DSTB (40C7 )},
    {MK_DSTB (40C8 )},
    {MK_DSTB (40C9 )},
    {MK_DSTB (40CA )},
    {MK_DSTB (40CB )},
    {MK_DSTB (40CC )},
    {MK_DSTB (40CD )},
    {MK_DSTB (40CE )},
    {MK_DSTB (40CF )},
    {MK_DSTB (40D0 )},
    {MK_DSTB (40D1 )},
    {MK_DSTB (40D2 )},
    {MK_DSTB (40D3 )},
    {MK_DSTB (40D4 )},
    {MK_DSTB (40D5 )},
    {MK_DSTB (40D6 )},
    {MK_DSTB (40D7 )},
    {MK_DSTB (40D8 )},
    {MK_DSTB (40D9 )},
    {MK_DSTB (40DA )},
    {MK_DSTB (40DB )},
    {MK_DSTB (40DC )},
    {MK_DSTB (40DD )},
    {MK_DSTB (40DE )},
    {MK_DSTB (40DF )},
    {MK_DSTB (40E0 )},
    {MK_DSTB (40E1 )},
    {MK_DSTB (40E2 )},
    {MK_DSTB (40E3 )},
    {MK_DSTB (40E4 )},
    {MK_DSTB (40E5 )},
    {MK_DSTB (40E6 )},
    {MK_DSTB (40E7 )},
    {MK_DSTB (40E8 )},
    {MK_DSTB (40E9 )},
    {MK_DSTB (40EA )},
    {MK_DSTB (40EB )},
    {MK_DSTB (40EC )},
    {MK_DSTB (40ED )},
    {MK_DSTB (40EE )},
    {MK_DSTB (40EF )},
    {MK_DSTB (40F0 )},
    {MK_DSTB (40F1 )},
    {MK_DSTB (40F2 )},
    {MK_DSTB (40F3 )},
    {MK_DSTB (40F4 )},
    {MK_DSTB (40F5 )},
    {MK_DSTB (40F6 )},
    {MK_DSTB (40F7 )},
    {MK_DSTB (40F8 )},
    {MK_DSTB (40F9 )},
    {MK_DSTB (40FA )},
    {MK_DSTB (40FB )},
    {MK_DSTB (40FC )},
    {MK_DSTB (40FD )},
    {MK_DSTB (40FE )},
    {MK_DSTB (40FF )},
    {MK_DSTB (4100 )},
    {MK_DSTB (4101 )},
    {MK_DSTB (4102 )},
    {MK_DSTB (4103 )},
    {MK_DSTB (4104 )},
    {MK_DSTB (4105 )},
    {MK_DSTB (4106 )},
    {MK_DSTB (4107 )},
    {MK_DSTB (4108 )},
    {MK_DSTB (4109 )},
    {MK_DSTB (410A )},
    {MK_DSTB (410B )},
    {MK_DSTB (410C )},
    {MK_DSTB (410D )},
    {MK_DSTB (410E )},
    {MK_DSTB (410F )},
    {MK_DSTB (4110 )},
    {MK_DSTB (4111 )},
    {MK_DSTB (4112 )},
    {MK_DSTB (4113 )},
    {MK_DSTB (4114 )},
    {MK_DSTB (4115 )},
    {MK_DSTB (4116 )},
    {MK_DSTB (4117 )},
    {MK_DSTB (4118 )},
    {MK_DSTB (4119 )},
    {MK_DSTB (411A )},
    {MK_DSTB (411B )},
    {MK_DSTB (411C )},
    {MK_DSTB (411D )},
    {MK_DSTB (411E )},
    {MK_DSTB (411F )},
    {MK_DSTB (4120 )},
    {MK_DSTB (4121 )},
    {MK_DSTB (4122 )},
    {MK_DSTB (4123 )},
    {MK_DSTB (4124 )},
    {MK_DSTB (4125 )},
    {MK_DSTB (4126 )},

    {MK_DSTB (4201 )},
    {MK_DSTB (4401 )},
    {MK_DSTB (4601 )},
    {MK_DSTB (4801 )},

    {MK_DSTB (4A01 )},
    {MK_DSTB (4A02 )},


};
static Data<DataType1> YkData[] = {
    {MK_DSTB (6001 )},
    {MK_DSTB (6002 )},
    {MK_DSTB (6003 )},
    {MK_DSTB (6004 )},
    {MK_DSTB (6005 )},
    {MK_DSTB (6006 )},
    {MK_DSTB (6007 )},
    {MK_DSTB (6008 )},
    {MK_DSTB (6009 )},
    {MK_DSTB (600A )},
    {MK_DSTB (600B )},
    {MK_DSTB (600C )},
    {MK_DSTB (600D )},
    {MK_DSTB (600E )},
    {MK_DSTB (600F )},
    {MK_DSTB (6010 )},

    {MK_DSTB (6064 )},
    {MK_DSTB (6065 )},
    {MK_DSTB (6066 )},

};

static Data<DataType1> StepAdjData[] = {
    {MK_DSTB ( 6601 ) },
    {MK_DSTB ( 6602 ) },
    {MK_DSTB ( 6603 ) },
    {MK_DSTB ( 6604 ) },
    {MK_DSTB ( 6605 ) },
    {MK_DSTB ( 6606 ) },
    {MK_DSTB ( 6607 ) },
};

static Data<BaseDataType> ParaData[] = {
    {MK_DSTB (5001 )},
    {MK_DSTB (5002 )},
    {MK_DSTB (5003 )},
    {MK_DSTB (5004 )},
    {MK_DSTB (5005 )},
    {MK_DSTB (5006 )},
    {MK_DSTB (5007 )},
    {MK_DSTB (5008 )},
    {MK_DSTB (5009 )},
    {MK_DSTB (500A )},
    {MK_DSTB (500B )},
    {MK_DSTB (500C )},
    {MK_DSTB (500D )},
    {MK_DSTB (500E )},
    {MK_DSTB (500F )},
    {MK_DSTB (5010 )},
    {MK_DSTB (5011 )},
    {MK_DSTB (5012 )},
    {MK_DSTB (5013 )},
    {MK_DSTB (5014 )},
    {MK_DSTB (5015 )},
    {MK_DSTB (5016 )},
    {MK_DSTB (5017 )},
    {MK_DSTB (5018 )},
    {MK_DSTB (5019 )},
    {MK_DSTB (501A )},
    {MK_DSTB (501B )},
    {MK_DSTB (501C )},
    {MK_DSTB (501D )},
    {MK_DSTB (501E )},
    {MK_DSTB (501F )},
    {MK_DSTB (5020 )},
    {MK_DSTB (5021 )},
    {MK_DSTB (5022 )},
    {MK_DSTB (5023 )},
    {MK_DSTB (5024 )},
    {MK_DSTB (5025 )},
    {MK_DSTB (5026 )},
    {MK_DSTB (5027 )},
    {MK_DSTB (5028 )},
    {MK_DSTB (5029 )},
    {MK_DSTB (502A )},
    {MK_DSTB (502B )},
    {MK_DSTB (502C )},
    {MK_DSTB (502D )},
    {MK_DSTB (502E )},
    {MK_DSTB (502F )},
    {MK_DSTB (5030 )},
    {MK_DSTB (5031 )},
    {MK_DSTB (5032 )},
    {MK_DSTB (5033 )},
    {MK_DSTB (5034 )},
    {MK_DSTB (5035 )},
    {MK_DSTB (5036 )},
    {MK_DSTB (5037 )},
    {MK_DSTB (5038 )},
    {MK_DSTB (5039 )},
    {MK_DSTB (503A )},
    {MK_DSTB (503B )},
    {MK_DSTB (503C )},
    {MK_DSTB (503D )},
    {MK_DSTB (503E )},
    {MK_DSTB (503F )},
    {MK_DSTB (5040 )},
    {MK_DSTB (5041 )},
    {MK_DSTB (5042 )},
    {MK_DSTB (5043 )},
    {MK_DSTB (5044 )},
    {MK_DSTB (5045 )},
    {MK_DSTB (5046 )},
    {MK_DSTB (5047 )},
    {MK_DSTB (5048 )},
    {MK_DSTB (5049 )},
    {MK_DSTB (504A )},
    {MK_DSTB (504B )},
    {MK_DSTB (504C )},
    {MK_DSTB (504D )},
    {MK_DSTB (504E )},
    {MK_DSTB (504F )},
    {MK_DSTB (5050 )},
    {MK_DSTB (5051 )},
    {MK_DSTB (5052 )},
    {MK_DSTB (5053 )},
    {MK_DSTB (5054 )},
    {MK_DSTB (5055 )},
    {MK_DSTB (5056 )},
    {MK_DSTB (5057 )},
    {MK_DSTB (5058 )},
    {MK_DSTB (5059 )},
    {MK_DSTB (505A )},
    {MK_DSTB (505B )},
    {MK_DSTB (505C )},
    {MK_DSTB (505D )},
    {MK_DSTB (505E )},
    {MK_DSTB (505F )},
    {MK_DSTB (5060 )},
    {MK_DSTB (5061 )},
    {MK_DSTB (5062 )},
    {MK_DSTB (5063 )},
    {MK_DSTB (5064 )},
    {MK_DSTB (5065 )},
    {MK_DSTB (5066 )},

    {MK_DSTB (507F )},
    {MK_DSTB (5080 )},
    {MK_DSTB (5081 )},
    {MK_DSTB (5082 )},
    {MK_DSTB (5083 )},
    {MK_DSTB (5084 )},
    {MK_DSTB (5085 )},
    {MK_DSTB (5086 )},
    {MK_DSTB (5087 )},
    {MK_DSTB (5088 )},
    {MK_DSTB (5089 )},
    {MK_DSTB (508A )},
    {MK_DSTB (508B )},
    {MK_DSTB (508C )},
    {MK_DSTB (508D )},
    {MK_DSTB (508E )},
    {MK_DSTB (508F )},
    {MK_DSTB (5090 )},
    {MK_DSTB (5091 )},
    {MK_DSTB (5092 )},
    {MK_DSTB (5093 )},
    {MK_DSTB (5094 )},
    {MK_DSTB (5095 )},
    {MK_DSTB (5096 )},
    {MK_DSTB (5097 )},
    {MK_DSTB (5098 )},
    {MK_DSTB (5099 )},
    {MK_DSTB (509A )},
    {MK_DSTB (509B )},
    {MK_DSTB (509C )},
    {MK_DSTB (509D )},
    {MK_DSTB (509E )},
    {MK_DSTB (509F )},
    {MK_DSTB (50A0 )},
    {MK_DSTB (50A1 )},
    {MK_DSTB (50A2 )},
    {MK_DSTB (50A3 )},
    {MK_DSTB (50A4 )},
    {MK_DSTB (50A5 )},
    {MK_DSTB (50A6 )},
    {MK_DSTB (50A7 )},
    {MK_DSTB (50A8 )},
    {MK_DSTB (50A9 )},
    {MK_DSTB (50AA )},
    {MK_DSTB (50AB )},
    {MK_DSTB (50AC )},
    {MK_DSTB (50AD )},
    {MK_DSTB (50AE )},
    {MK_DSTB (50AF )},
    {MK_DSTB (50B0 )},
    {MK_DSTB (50B1 )},
    {MK_DSTB (50B2 )},
    {MK_DSTB (50B3 )},
    {MK_DSTB (50B4 )},
    {MK_DSTB (50B5 )},
    {MK_DSTB (50B6 )},
    {MK_DSTB (50B7 )},
    {MK_DSTB (50B8 )},
    {MK_DSTB (50B9 )},
    {MK_DSTB (50BA )},
    {MK_DSTB (50BB )},
    {MK_DSTB (50BC )},
    {MK_DSTB (50BD )},
    {MK_DSTB (50BE )},
    {MK_DSTB (50BF )},
    {MK_DSTB (50C0 )},
    {MK_DSTB (50C1 )},
    {MK_DSTB (50C2 )},
    {MK_DSTB (50C3 )},
    {MK_DSTB (50C4 )},
    {MK_DSTB (50C5 )},
    {MK_DSTB (50C6 )},
    {MK_DSTB (50C7 )},
    {MK_DSTB (50C8 )},
    {MK_DSTB (50C9 )},
    {MK_DSTB (50CA )},
    {MK_DSTB (50CB )},

    {MK_DSTB (5100 )},
    {MK_DSTB (5101 )},
    {MK_DSTB (5102 )},
    {MK_DSTB (5103 )},
    {MK_DSTB (5104 )},
    {MK_DSTB (5105 )},
    {MK_DSTB (5106 )},
    {MK_DSTB (5107 )},
    {MK_DSTB (5108 )},
    {MK_DSTB (5109 )},
    {MK_DSTB (510A )},
    {MK_DSTB (510B )},
    {MK_DSTB (510C )},
    {MK_DSTB (510D )},
    {MK_DSTB (510E )},
    {MK_DSTB (510F )},
    {MK_DSTB (5110 )},
    {MK_DSTB (5111 )},
    {MK_DSTB (5112 )},
    {MK_DSTB (5113 )},
    {MK_DSTB (5114 )},
    {MK_DSTB (5115 )},
    {MK_DSTB (5116 )},
    {MK_DSTB (5117 )},
    {MK_DSTB (5118 )},
    {MK_DSTB (5119 )},
    {MK_DSTB (511A )},
    {MK_DSTB (511B )},
    {MK_DSTB (511C )},
    {MK_DSTB (511D )},
    {MK_DSTB (511E )},
    {MK_DSTB (511F )},
    {MK_DSTB (5120 )},
    {MK_DSTB (5121 )},
    {MK_DSTB (5122 )},
    {MK_DSTB (5123 )},
    {MK_DSTB (5124 )},
    {MK_DSTB (5125 )},
    {MK_DSTB (5126 )},
    {MK_DSTB (5127 )},
    {MK_DSTB (5128 )},
    {MK_DSTB (5129 )},
    {MK_DSTB (512A )},
    {MK_DSTB (512B )},
    {MK_DSTB (512C )},
    {MK_DSTB (512D )},
    {MK_DSTB (512E )},
    {MK_DSTB (512F )},
    {MK_DSTB (5130 )},
    {MK_DSTB (5131 )},
    {MK_DSTB (5132 )},
    {MK_DSTB (5133 )},
    {MK_DSTB (5134 )},
    {MK_DSTB (5135 )},
    {MK_DSTB (5136 )},
    {MK_DSTB (5137 )},
    {MK_DSTB (5138 )},
    {MK_DSTB (5139 )},
    {MK_DSTB (513A )},
    {MK_DSTB (513B )},
    {MK_DSTB (513C )},
    {MK_DSTB (513D )},
    {MK_DSTB (513E )},
    {MK_DSTB (513F )},
    {MK_DSTB (5140 )},
    {MK_DSTB (5141 )},
    {MK_DSTB (5142 )},
    {MK_DSTB (5143 )},
    {MK_DSTB (5144 )},
    {MK_DSTB (5145 )},
    {MK_DSTB (5146 )},
    {MK_DSTB (5147 )},
    {MK_DSTB (5148 )},
    {MK_DSTB (5149 )},
    {MK_DSTB (514A )},



    {MK_DSTB (5200 )},
    {MK_DSTB (5201 )},
    {MK_DSTB (5202 )},
    {MK_DSTB (5203 )},
    {MK_DSTB (5204 )},
    {MK_DSTB (5205 )},
    {MK_DSTB (5206 )},
    {MK_DSTB (5207 )},
    {MK_DSTB (5208 )},
    {MK_DSTB (5209 )},
    {MK_DSTB (520A )},
    {MK_DSTB (520B )},
    {MK_DSTB (520C )},
    {MK_DSTB (520D )},
    {MK_DSTB (520E )},
    {MK_DSTB (520F )},
    {MK_DSTB (5210 )},
    {MK_DSTB (5211 )},
    {MK_DSTB (5212 )},
    {MK_DSTB (5213 )},
    {MK_DSTB (5214 )},
    {MK_DSTB (5215 )},
    {MK_DSTB (5216 )},
	{MK_DSTB (5217 )},

    {MK_DSTB (6801 )},

};



static struct mutex data_mutex;
UserData::UserData()
{
    mutex_init ( &data_mutex, "data mutex" );
    thread_cond_init ( &cond, "cond" );
    UserSetDataFun = NULL;
}

UserData::~UserData()
{
    mutex_destroy ( &data_mutex );
}

int16 UserData::GetData ( int32 Id, Data<BaseDataType>* data )
{
    uint16 i;
    mutex_lock ( &data_mutex );

    if ( Id < 0x4001 ) {
        for ( i = 0;i < sizeof ( YxData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YxData[i].Id ) {
                memcpy ( data, &YxData[i], sizeof ( Data<BaseDataType> ) );
                mutex_unlock ( &data_mutex );
                return 1;
            }
        }
    }

    if ( Id < 0x5001 ) {
        for ( i = 0;i < sizeof ( YcData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YcData[i].Id ) {
                memcpy ( data, &YcData[i], sizeof ( Data<BaseDataType> ) );
                mutex_unlock ( &data_mutex );
                return 1;
            }
        }
    }

    if ( Id < 0x6001 ) {
        for ( i = 0;i < sizeof ( ParaData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == ParaData[i].Id ) {
                memcpy ( data, &ParaData[i], sizeof ( Data<BaseDataType> ) );
                mutex_unlock ( &data_mutex );
                return 1;
            }
        }
    }

    if ( Id < 0x6201 ) {
        for ( i = 0;i < sizeof ( YkData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YkData[i].Id ) {
                memcpy ( data, &YkData[i], sizeof ( Data<BaseDataType> ) );
                mutex_unlock ( &data_mutex );
                return 1;
            }
        }
    }
    
    if ( Id < 0x6700 ) {
        for ( i = 0;i < sizeof ( StepAdjData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == StepAdjData[i].Id ) {
                memcpy ( data, &StepAdjData[i], sizeof ( Data<BaseDataType> ) );
                mutex_unlock ( &data_mutex );
                return 1;
            }
        }
    }
    if ( Id == 0x6801 ) {
        for ( i = 0;i < sizeof ( ParaData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == ParaData[i].Id ) {
                memcpy ( data, &ParaData[i], sizeof ( Data<BaseDataType> ) );
                mutex_unlock ( &data_mutex );
                return 1;
            }
        }
    }
    mutex_unlock ( &data_mutex );

    return 0;
}

int16 UserData::SetData ( int32 Id, Data<BaseDataType>* data )
{

    uint16 i;
    int16 find = 0;
    mutex_lock ( &data_mutex );

    if ( Id < 0x4001 ) {
        for ( i = 0;i < sizeof ( YxData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YxData[i].Id ) {
                memcpy ( & ( YxData[i].Data->Data ), & ( ( ( DataType1* ) ( data->Data ) )->Data ), YxData[i].Size );
                find = 1;
                goto Leave;
            }
        }
    }

    if ( Id < 0x5001 ) {
        for ( i = 0;i < sizeof ( YcData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YcData[i].Id ) {
                memcpy ( & ( YcData[i].Data->Data ), & ( ( ( DataType1* ) ( data->Data ) )->Data ), YcData[i].Size );
                find = 1;
                goto Leave;
            }
        }
    }

    if ( Id < 0x6001 ) {
        for ( i = 0;i < sizeof ( ParaData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == ParaData[i].Id ) {
                memcpy ( & ( ( ( DataType1* ) ( ParaData[i].Data ) )->Data ), & ( ( ( DataType1* ) ( data->Data ) )->Data ), ParaData[i].Size );
                find = 1;
                goto Leave;
            }
        }
    }

    if ( Id < 0x6201 ) {
        for ( i = 0;i < sizeof ( YkData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YkData[i].Id ) {
                memcpy ( & ( YkData[i].Data->Data ), & ( ( ( DataType1* ) ( data->Data ) )->Data ), YkData[i].Size );
                find = 1;
                goto Leave;
            }
        }
    }
     if ( Id < 0x6700 ) {
        for ( i = 0;i < sizeof ( StepAdjData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == StepAdjData[i].Id ) {
                memcpy ( & ( StepAdjData[i].Data->Data ), & ( ( ( DataType1* ) ( data->Data ) )->Data ), StepAdjData[i].Size );
                find = 1;
                goto Leave;
            }
        }
    }
    
    if ( Id == 0x6801 ) {
        for ( i = 0;i < sizeof ( ParaData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == ParaData[i].Id ) {
                memcpy ( & ( ( ( DataType1* ) ( ParaData[i].Data ) )->Data ), & ( ( ( DataType1* ) ( data->Data ) )->Data ), ParaData[i].Size );
                find = 1;
                goto Leave;
            }
        }
    }

Leave:

    if ( UserSetDataFun != NULL && find == 1 )
        ( *UserSetDataFun ) ( Id );

    mutex_unlock ( &data_mutex );

    if ( find == 1 )
        return 1;
    else
        return 0;

}

int16 UserData::GetDataSize ( int32 Id )
{
    uint16 i;

    mutex_lock ( &data_mutex );

    if ( Id < 0x4001 ) {
        for ( i = 0;i < sizeof ( YxData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YxData[i].Id ) {
                mutex_unlock ( &data_mutex );
                return YxData[i].Size;
            }
        }
    }

    if ( Id < 0x5001 ) {
        for ( i = 0;i < sizeof ( YcData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YcData[i].Id ) {
                mutex_unlock ( &data_mutex );
                return YcData[i].Size;
            }
        }
    }

    if ( Id < 0x6001 ) {
        for ( i = 0;i < sizeof ( ParaData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == ParaData[i].Id ) {
                mutex_unlock ( &data_mutex );
                return ParaData[i].Size;
            }
        }
    }

    if ( Id < 0x6201 ) {
        for ( i = 0;i < sizeof ( YkData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YkData[i].Id ) {
                mutex_unlock ( &data_mutex );
                return YkData[i].Size;
            }
        }
    }

    if ( Id < 0x6700 ) {
        for ( i = 0;i < sizeof ( StepAdjData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == StepAdjData[i].Id ) {
                mutex_unlock ( &data_mutex );
                return StepAdjData[i].Size;
            }
        }
    }
    if ( Id == 0x6801 ) {
        for ( i = 0;i < sizeof ( ParaData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == ParaData[i].Id ) {
                mutex_unlock ( &data_mutex );
                return ParaData[i].Size;
            }
        }
    }

    mutex_unlock ( &data_mutex );

    return 0;
}

Data<BaseDataType>* UserData::GetDataPtr ( int32 Id )
{
    uint16 i;

    mutex_lock ( &data_mutex );

    if ( Id < 0x4001 ) {
        for ( i = 0;i < sizeof ( YxData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YxData[i].Id ) {
                mutex_unlock ( &data_mutex );
                return ( Data<BaseDataType>* ) &YxData[i];
            }
        }
    }

    if ( Id < 0x5001 ) {
        for ( i = 0;i < sizeof ( YcData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YcData[i].Id ) {
                mutex_unlock ( &data_mutex );
                return ( Data<BaseDataType>* ) &YcData[i];
            }
        }
    }

    if ( Id < 0x6001 ) {
        for ( i = 0;i < sizeof ( ParaData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == ParaData[i].Id ) {
                mutex_unlock ( &data_mutex );
                return &ParaData[i];
            }
        }
    }

    if ( Id < 0x6201 ) {
        for ( i = 0;i < sizeof ( YkData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YkData[i].Id ) {
                mutex_unlock ( &data_mutex );
                return ( Data<BaseDataType>* ) &YkData[i];
            }
        }
    }
    
    if ( Id < 0x6700 ) {
        for ( i = 0;i < sizeof ( StepAdjData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == StepAdjData[i].Id ) {
                mutex_unlock ( &data_mutex );
                return ( Data<BaseDataType>* ) &StepAdjData[i];
            }
        }
    }
    if ( Id == 0x6801 ) {
        for ( i = 0;i < sizeof ( ParaData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == ParaData[i].Id ) {
                mutex_unlock ( &data_mutex );
                return &ParaData[i];
            }
        }
    }
    mutex_unlock ( &data_mutex );

    return 0;
}


int16 UserData::GetDataTableSize ( int32 tb )
{

    mutex_lock ( &data_mutex );
    switch(tb){
   		case MSG_YX:
            mutex_unlock ( &data_mutex );
   			return  sizeof ( YxData ) / sizeof ( Data<BaseDataType> );
   		case MSG_YC:
            mutex_unlock ( &data_mutex );
   			return sizeof ( YcData ) / sizeof ( Data<BaseDataType> );
   		case MSG_YK:
            mutex_unlock ( &data_mutex );
   			return sizeof ( YkData ) / sizeof ( Data<BaseDataType> );
   		case MSG_PARA:
            mutex_unlock ( &data_mutex );
   			return sizeof ( ParaData ) / sizeof ( Data<BaseDataType> );
		case MSG_STEP:
            mutex_unlock ( &data_mutex );
   			return sizeof ( StepAdjData ) / sizeof ( Data<BaseDataType> );
   			
   		default:
            mutex_unlock ( &data_mutex );
            return 0;
   				
   	}
}

Data<BaseDataType>* UserData::GetDataTablePtr ( int32 tb )	
{
    
    mutex_lock ( &data_mutex );
    switch(tb){
   		case MSG_YX:
            mutex_unlock ( &data_mutex );
   			return ( Data<BaseDataType>* ) &YxData[0];
   		case MSG_YC:
            mutex_unlock ( &data_mutex );
   			return ( Data<BaseDataType>* ) &YcData[0];
   		case MSG_YK:
            mutex_unlock ( &data_mutex );
   			return ( Data<BaseDataType>* ) &YkData[0];
   		case MSG_PARA:
            mutex_unlock ( &data_mutex );
   			return ( Data<BaseDataType>* ) &ParaData[0];
		case MSG_STEP:
            mutex_unlock ( &data_mutex );
   			return ( Data<BaseDataType>* ) &StepAdjData[0];
   		default:
            mutex_unlock ( &data_mutex );
            return NULL;
   				
   	}
}
int16 UserData::GetDataType ( int32 Id )
{

    if ( Id < 0x4001 ) {
        return MSG_YX;
    }

    if ( Id < 0x5001 ) {
        return MSG_YC;
    }

    if ( Id < 0x6001 ) {
        return MSG_PARA;
    }

    if ( Id < 0x6201 ) {
        return MSG_YK;
    }
    if ( Id < 0x6700 ) {
        return MSG_STEP;
    }
    if ( Id == 0x6801 ) {
        return MSG_PARA;
    }

    return 0;
}

int16 UserData::GetData ( int32 Id, Data<BaseDataType>* data, char *r_data )
{
    uint16 i;
    mutex_lock ( &data_mutex );

    if ( Id < 0x4001 ) {
        for ( i = 0;i < sizeof ( YxData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YxData[i].Id ) {
                memcpy ( data, &YxData[i], sizeof ( Data<BaseDataType> ) );
                memcpy ( r_data, & ( YxData[i].Data->Data ), YxData[i].Size );
                mutex_unlock ( &data_mutex );
                return 1;
            }
        }
    }

    if ( Id < 0x5001 ) {
        for ( i = 0;i < sizeof ( YcData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YcData[i].Id ) {
                memcpy ( data, &YcData[i], sizeof ( Data<BaseDataType> ) );
                memcpy ( r_data, & ( YcData[i].Data->Data ), YcData[i].Size );
                mutex_unlock ( &data_mutex );
                return 1;
            }
        }
    }

    if ( Id < 0x6001 ) {
        for ( i = 0;i < sizeof ( ParaData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == ParaData[i].Id ) {
                memcpy ( data, &ParaData[i], sizeof ( Data<BaseDataType> ) );
                memcpy ( r_data, & ( ( ( DataType1* ) ( ParaData[i].Data ) )->Data ), ParaData[i].Size );
                mutex_unlock ( &data_mutex );
                return 1;
            }
        }
    }

    if ( Id < 0x6201 ) {
        for ( i = 0;i < sizeof ( YkData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YkData[i].Id ) {
                memcpy ( data, &YkData[i], sizeof ( Data<BaseDataType> ) );
                memcpy ( r_data, & ( YkData[i].Data->Data ), YkData[i].Size );
                mutex_unlock ( &data_mutex );
                return 1;
            }
        }
    }
    if ( Id < 0x6700 ) {
        for ( i = 0;i < sizeof ( StepAdjData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == StepAdjData[i].Id ) {
                memcpy ( data, &StepAdjData[i], sizeof ( Data<BaseDataType> ) );
                memcpy ( r_data, & ( StepAdjData[i].Data->Data ), StepAdjData[i].Size );
                mutex_unlock ( &data_mutex );
                return 1;
            }
        }
    }
    if ( Id == 0x6801 ) {
        for ( i = 0;i < sizeof ( ParaData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == ParaData[i].Id ) {
                memcpy ( data, &ParaData[i], sizeof ( Data<BaseDataType> ) );
                memcpy ( r_data, & ( ( ( DataType1* ) ( ParaData[i].Data ) )->Data ), ParaData[i].Size );
                mutex_unlock ( &data_mutex );
                return 1;
            }
        }
    }

    mutex_unlock ( &data_mutex );

    return 0;

}

int16 UserData::SetData ( int32 Id, char* data )
{
    uint16 i;
    int16 find = 0;
    mutex_lock ( &data_mutex );

    if ( Id < 0x4001 ) {
        for ( i = 0;i < sizeof ( YxData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YxData[i].Id ) {
                memcpy ( & ( YxData[i].Data->Data ), data, YxData[i].Size );
                find = 1;
                goto Leave;
            }
        }
    }

    if ( Id < 0x5001 ) {
        for ( i = 0;i < sizeof ( YcData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YcData[i].Id ) {
                memcpy ( & ( YcData[i].Data->Data ), data, YcData[i].Size );
                //memcpy(YcData[i].Data,data,YcData[i].Size);
                find = 1;
                goto Leave;
            }
        }
    }

    if ( Id < 0x6001 ) {
        for ( i = 0;i < sizeof ( ParaData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == ParaData[i].Id ) {
                memcpy ( & ( ( ( DataType1* ) ( ParaData[i].Data ) )->Data ), data, ParaData[i].Size );
                find = 1;
                goto Leave;
            }
        }
    }

    if ( Id < 0x6201 ) {
        for ( i = 0;i < sizeof ( YkData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YkData[i].Id ) {
                memcpy ( & ( YkData[i].Data->Data ), data, YkData[i].Size );
                find = 1;
                goto Leave;
            }
        }
    }

    if ( Id < 0x6700 ) {
        for ( i = 0;i < sizeof ( StepAdjData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == StepAdjData[i].Id ) {
                memcpy ( & ( StepAdjData[i].Data->Data ), data, StepAdjData[i].Size );
                find = 1;
                goto Leave;
            }
        }
    }
    if ( Id == 0x6801 ) {
        for ( i = 0;i < sizeof ( ParaData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == ParaData[i].Id ) {
                memcpy ( & ( ( ( DataType1* ) ( ParaData[i].Data ) )->Data ), data, ParaData[i].Size );
                find = 1;
                goto Leave;
            }
        }
    }
Leave:

    if ( UserSetDataFun != NULL && find == 1 )
        ( *UserSetDataFun ) ( Id );

    mutex_unlock ( &data_mutex );

    if ( find == 1 )
        return 1;
    else
        return 0;

}

int16 UserData::LoadPara()
{    
    uint16 i;
	char name_str[20];	
    xmlDocPtr doc;
    xmlNodePtr root;
    
    // load an exist xml file.
    doc = xmlParseFile(PARA_CFG);
    if (doc == NULL ) 
    { 
        PFUNC(TEM_ERROR ,DEBUG_NORMAL,"Not open %s \n",PARA_CFG);
        copy(DEFAULT_PARA_CFG,PARA_CFG);
        doc = xmlParseFile(PARA_CFG);
        if(doc==NULL)
        return 0; 
    } 
    // get root
    root = xmlDocGetRootElement(doc);
    if (root == NULL) 
    { 
        PFUNC(TEM_ERROR ,DEBUG_NORMAL,"empty document\n"); 
        xmlFreeDoc(doc);
        return 0;
    }
    // list all child of root
    xmlNodePtr head = root->children->next;
    while(head != NULL)
    {
        if(head->type == XML_ELEMENT_NODE)
        {
		    for ( i = 0 ; i < sizeof ( ParaData ) / sizeof ( Data<BaseDataType> ) ; i++ ) 
		    {
		    	sprintf(name_str,"d_%lx",ParaData[i].Id);
	            if ((!xmlStrcmp(head->name, (const xmlChar *)name_str)))
	            {
					xmlChar *content = xmlNodeGetContent ( head->children );
					str2hex((char *)& ( ( ( DataType1* ) ( ParaData[i].Data ) )->Data ),(char *)content,ParaData[i].Size );
	                break;
	            }		
		    }
        }
    	head = head->next;
    }
	xmlFreeDoc(doc);
	xmlCleanupParser();
	xmlMemoryDump();//debug memory for regression tests
	

	int Id,tmp;
	float p1,p2,p3;
	char *ptr;
	char buf[500];
	FILE *fp;
	fp=myfopen(PARA_YC,"rt");
	if(fp==NULL)
	{
		copy(DEFAULT_PARA_YC,PARA_YC);
		fp=myfopen(PARA_YC,"rt");	
	}	
	if(fp!=NULL){
		while(!feof(fp)){
			fgets((char*)buf,500,fp);
			ptr=str_find((char*)buf,(char*)"0x");
			if(ptr!=NULL){
				ptr++;
				ptr++;
				sscanf(ptr,"%x",&Id);
				ptr=str_find(ptr,(char*)"0x");
				ptr++;
				ptr++;
				sscanf(ptr,"%x%f%f%f",&tmp,&p1,&p2,&p3);
				for ( i = 0;i < sizeof ( YcData ) / sizeof ( Data<BaseDataType> );i++ ) {
		            if ( Id == YcData[i].Id ) {
		            	YcData[i].UnitaryCoff=p1;
		            	YcData[i].ScaleCoff=p2;
		            	YcData[i].ShorFloCoff=p3;
						//PFUNC ( TEM_INFO , DEBUG_NORMAL, "id:%x p1=%f p2=%f p3=%f\n",Id,p1,p2,p3);
		            	break;
		            }
				}
			}
		}
	fclose(fp);			
	}	
	PFUNC ( TEM_INFO , DEBUG_NORMAL, "finish load para\n" );
    thread_signal_all ( &cond );
return 0;		
}

int16  UserData::SavePara ( int32 Id )
{
    uint16 i, find = 0;
	char name_str[20];	
	char data_str[600];

    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr node = NULL, node1 = NULL;
    
	    for ( i = 0 ; i < sizeof ( ParaData ) / sizeof ( Data<BaseDataType> ) ; i++ ) 
	    {
	    	if(Id==ParaData[i].Id)
	    		{
	    		find=1;
	    		break;
	    		}
	    }
	    if(!find)
	    {
	    PFUNC(TEM_ERROR ,DEBUG_NORMAL,"Not find %x para\n",Id)	;
		return 0;
		}    
    
    find=0;
    // load an exist xml file.
    doc = xmlParseFile(PARA_CFG);
    if (doc == NULL ) 
    { 
        PFUNC(TEM_ERROR ,DEBUG_NORMAL,"Not open %s \n",PARA_CFG)	;
        return 0; 
    } 
    // get root
    root = xmlDocGetRootElement(doc);
    if (root == NULL) 
    { 
        PFUNC(TEM_ERROR ,DEBUG_NORMAL,"empty document\n"); 
        xmlFreeDoc(doc);
        return 0;
    }
    // list all child of root
    xmlNodePtr head = root->children->next;
    while(head != NULL)
    {
        if(head->type == XML_ELEMENT_NODE)
        {
        	//PFUNC(TEM_INFO,DEBUG_NORMAL,"Name:%s Content:%s\n",head->name,xmlNodeGetContent(head->children));
			sprintf(name_str,"d_%lx",Id);
            if ((!xmlStrcmp(head->name, (const xmlChar *)name_str)))
            {
				hex2str(data_str,(char *)& ( ( ( DataType1* ) ( ParaData[i].Data ) )->Data ),ParaData[i].Size );
				data_str[ParaData[i].Size*2]=0;
                xmlNodeSetContent(head->children,(const xmlChar *)data_str);
                find=1;
                break;
            }		
        }
    	head = head->next;
    }
    if(find==0){//新参数,未被保存过
		hex2str(data_str,(char *)& ( ( ( DataType1* ) ( ParaData[i].Data ) )->Data ),ParaData[i].Size );
		data_str[ParaData[i].Size*2]=0;
		node = xmlNewNode(NULL, BAD_CAST name_str);
		node1 = xmlNewText(BAD_CAST data_str);
		xmlNewProp(node, BAD_CAST "attribute", BAD_CAST "yes");
		xmlAddChild(node, node1);
		xmlAddChild(root, node);
    }
	xmlSaveFormatFileEnc(PARA_CFG, doc, "UTF-8", 1);
	xmlFreeDoc(doc);
	xmlCleanupParser();
	xmlMemoryDump();//debug memory for regression tests
return 1;

}

int16  UserData::SaveParaAll ( )
{
	uint16 i;	
	char name_str[20];	
	char data_str[600];
	xmlDocPtr doc = NULL;
	xmlNodePtr root_node = NULL, node = NULL, node1 = NULL;
	// Creates a new document, a node and set it as a root node
	doc = xmlNewDoc(BAD_CAST "1.0");
	root_node = xmlNewNode(NULL, BAD_CAST "root");
	xmlDocSetRootElement(doc, root_node);
	//Here goes another way to create nodes.
	for(i=0;i < sizeof ( ParaData ) / sizeof ( Data<BaseDataType> );i++){
	sprintf(name_str,"d_%lx",ParaData[i].Id);
	hex2str(data_str,(char *)& ( ( ( DataType1* ) ( ParaData[i].Data ) )->Data ),ParaData[i].Size );
	data_str[ParaData[i].Size*2]=0;
	node = xmlNewNode(NULL, BAD_CAST name_str);
	node1 = xmlNewText(BAD_CAST data_str);
	xmlNewProp(node, BAD_CAST "attribute", BAD_CAST "yes");
	xmlAddChild(node, node1);
	xmlAddChild(root_node, node);
	}
	//Dumping document to stdio or file
	xmlSaveFormatFileEnc(PARA_CFG, doc, "UTF-8", 1);
	xmlFreeDoc(doc);
	xmlCleanupParser();
	xmlMemoryDump();//debug memory for regression tests
	return(1);
}

