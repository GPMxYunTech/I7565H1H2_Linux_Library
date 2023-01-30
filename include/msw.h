/**********************************************************************
 *
 *  msw.h
 *
 *  Header for msw.c (Microsoft Windows Warp)
 *
 *  Since 1999.10.27 by Reed Lai
 *
 **********************************************************************/
/* *INDENT-OFF* */
#ifndef _MSW_H
#define _MSW_H

#include "i7k.h"
#include "common.h"

#define DEFAULT_READ_BUF_SIZE 250

#define MAX_CMD_SIZE  250

#ifndef I7565H1H2
#define MAX_COM_PORTS 4
#define COM1          1
#define COM2          2
#define COM3          3
#define COM4          4
#else
#define MAX_COM_PORTS 256
#define USB0          0
#define USB1          1
#define USB2          2
#define USB3          3
#define USB4          4
#define USB5          5
#define USB6          6
#define USB7          7
#define USB8          8
#define USB9          9
#define USB10         10
#define USB11         11
#define USB12         12
#define USB13         13
#define USB14         14
#define USB15         15
#define USB16         16
#define USB17         17
#define USB18         18
#define USB19         19
#define USB20         20
#define USB21         21
#define USB22         22
#define USB23         23
#define USB24         24
#define USB25         25
#define USB26         26
#define USB27         27
#define USB28         28
#define USB29         29
#define USB30         30
#define USB31         31
#define USB32         32
#define USB33         33
#define USB34         34
#define USB35         35
#define USB36         36
#define USB37         37
#define USB38         38
#define USB39         39
#define USB40         40
#define USB41         41
#define USB42         42
#define USB43         43
#define USB44         44
#define USB45         45
#define USB46         46
#define USB47         47
#define USB48         48
#define USB49         49
#define USB50         50
#define USB51         51
#define USB52         52
#define USB53         53
#define USB54         54
#define USB55         55
#define USB56         56
#define USB57         57
#define USB58         58
#define USB59         59
#define USB60         60
#define USB61         61
#define USB62         62
#define USB63         63
#define USB64         64

#define USB65         65
#define USB66         66
#define USB67         67
#define USB68         68
#define USB69         69
#define USB70         70
#define USB71         71
#define USB72         72
#define USB73         73
#define USB74         74
#define USB75         75
#define USB76         76
#define USB77         77
#define USB78         78
#define USB79         79
#define USB80         80
#define USB81         81
#define USB82         82
#define USB83         83
#define USB84         84
#define USB85         85
#define USB86         86
#define USB87         87
#define USB88         88
#define USB89         89
#define USB90         90
#define USB91         91
#define USB92         92
#define USB93         93
#define USB94         94
#define USB95         95
#define USB96         96
#define USB97         97
#define USB98         98
#define USB99         99
#define USB100        100
#define USB101        101
#define USB102        102
#define USB103        103
#define USB104        104
#define USB105        105
#define USB106        106
#define USB107        107
#define USB108        108
#define USB109        109
#define USB110        110
#define USB111        111
#define USB112        112
#define USB113        113
#define USB114        114
#define USB115        115
#define USB116        116
#define USB117        117
#define USB118        118
#define USB119        119
#define USB120        120
#define USB121        121
#define USB122        122
#define USB123        123
#define USB124        124
#define USB125        125
#define USB126        126
#define USB127        127
#define USB128        128

#define USB129        129
#define USB130        130
#define USB131        131
#define USB132        132
#define USB133        133
#define USB134        134
#define USB135        135
#define USB136        136
#define USB137        137
#define USB138        138
#define USB139        139
#define USB140        140
#define USB141        141
#define USB142        142
#define USB143        143
#define USB144        144
#define USB145        145
#define USB146        146
#define USB147        147
#define USB148        148
#define USB149        149
#define USB150        150
#define USB151        151
#define USB152        152
#define USB153        153
#define USB154        154
#define USB155        155
#define USB156        156
#define USB157        157
#define USB158        158
#define USB159        159
#define USB160        160
#define USB161        161
#define USB162        162
#define USB163        163
#define USB164        164
#define USB165        165
#define USB166        166
#define USB167        167
#define USB168        168
#define USB169        169
#define USB170        170
#define USB171        171
#define USB172        172
#define USB173        173
#define USB174        174
#define USB175        175
#define USB176        176
#define USB177        177
#define USB178        178
#define USB179        179
#define USB180        180
#define USB181        181
#define USB182        182
#define USB183        183
#define USB184        184
#define USB185        185
#define USB186        186
#define USB187        187
#define USB188        188
#define USB189        189
#define USB190        190
#define USB191        191
#define USB192        192

#define USB193        193
#define USB194        194
#define USB195        195
#define USB196        196
#define USB197        197
#define USB198        198
#define USB199        199
#define USB200        200
#define USB201        201
#define USB202        202
#define USB203        203
#define USB204        204
#define USB205        205
#define USB206        206
#define USB207        207
#define USB208        208
#define USB209        209
#define USB210        210
#define USB211        211
#define USB212        212
#define USB213        213
#define USB214        214
#define USB215        215
#define USB216        216
#define USB217        217
#define USB218        218
#define USB219        219
#define USB220        220
#define USB221        221
#define USB222        222
#define USB223        223
#define USB224        224
#define USB225        225
#define USB226        226
#define USB227        227
#define USB228        228
#define USB229        229
#define USB230        230
#define USB231        231
#define USB232        232
#define USB233        233
#define USB234        234
#define USB235        235
#define USB236        236
#define USB237        237
#define USB238        238
#define USB239        239
#define USB240        240
#define USB241        241
#define USB242        242
#define USB243        243
#define USB244        244
#define USB245        245
#define USB246        246
#define USB247        247
#define USB248        248
#define USB249        249
#define USB250        250
#define USB251        251
#define USB252        252
#define USB253        253
#define USB254        254
#define USB255        255
#endif

#define EXPORTS
#define CALLBACK
#define WINAPI
#define FAR
#define NEAR

#define Data5Bit	5
#define Data6Bit	6
#define Data7Bit	7
#define Data8Bit	8

#define NonParity	0
#define OddParity	1
#define EvenParity	2

#define OneStopBit	0
#define One5StopBit	1
#define TwoStopBit	2


typedef __bool_t BOOL;

typedef u_byte_t BYTE;
typedef u_word_t WORD;
typedef u_dword_t DWORD;

typedef unsigned char UCHAR;

extern int _fd[];

extern EXPORTS WORD CALLBACK Receive_Cmd(char cPort, char szResult[],										 WORD wTimeOut, WORD wChksum);

/* Macros to warp MS-Windows functions */
#define Sleep(a)

/* Macros to warp Turbo C functions */
#define getch()   (int)getchar()
#define getche()  (int)getchar()
#define kbhit()   (int)getchar()

/* Macros to warp DOS functions */
#define RECEIVE_CMD(a, b, c, d) (int)Receive_Cmd((char)(a), (b), (long)(c), (int)(d))
#define SEND_CMD(a,b,c,d) (int)Send_Cmd((char)(a), (b), (WORD)(d))
#define OPEN_COM(p,b)     (int)Open_Com((char)(p), (DWORD)(b), Data8Bit, NonParity, OneStopBit)
#define CLOSE_COM(p)      (int)Close_Com((char)(p))

WORD Send_Receive_Cmd(char cPort, char szCmd[], char szResult[], WORD wTimeOut, WORD wChksum, WORD * wT);
//WORD Receive_Cmd(char cPort, char szResult[], WORD wTimeOut, WORD wChksum);
WORD Receive_Binary(char cPort, char szResult[], WORD wTimeOut, DWORD wLen, WORD *wT);
WORD Send_Cmd(char cPort, char szCmd[], WORD wChksum);
WORD Send_Binary(char cPort, char szCmd[],int iLen);
extern BOOL Close_Com(char port);
extern WORD Open_Com(char port, DWORD baudrate, char cData, char cParity, char cStop);


WORD SetLineStatus(BYTE cPort, BYTE DTR_Trig, BYTE RTS_Trig);
WORD FW_C2U_SendTrig(BYTE cPort, BYTE Trig);

#include "i7000.h"

/*#ifndef I7565H1H2
#include "i8000.h"
#include "i87000.h"
#endif
*/
#endif							/* _MSW_H */
