#include "i7565H1H2.h"

#ifndef SUCCESS
#define SUCCESS 0
#endif

#ifndef FAILURE
#define FAILURE -1
#endif

/************    VCI_CAN Global Define   **************/
//#define SWAP(x,y)                       {DWORD t; t = x; x = y; y = t;}
//#define Word_SWAP(x,y)          {WORD t; t = x; x = y; y = t;}
#define RULE_MAX	512
#define CAN1_RecvFlag	0

/************    VCI_CAN Global Variable   **************/
DWORD CANBaud_Arr[15]=
{
	CANBaud_5K,	CANBaud_10K,	CANBaud_20K,	CANBaud_40K,
	CANBaud_50K,	CANBaud_80K,	CANBaud_100K,	CANBaud_125K,
	CANBaud_200K,	CANBaud_250K,	CANBaud_400K,	CANBaud_500K,
	CANBaud_600K,	CANBaud_800K,	CANBaud_1000K,
};

/************    VCI_CAN RxBuffer Variable   **************/
//BYTE U2C_Buf[MAX_I7565_SUPPORT][2048]={0};

//#define CAN_BUF_SIZE            65536
//#define CAN_BUF_MASK            0xffff


/* CAN1->PCUSB Define */
DWORD C12P_MsgBuf_rIdx[MAX_I7565_SUPPORT] = {0};
DWORD C12P_MsgBuf_wIdx[MAX_I7565_SUPPORT] = {0};
BYTE C12P_BufFull[MAX_I7565_SUPPORT] = {0};
#define C12P_BUF_WRIDX(index) (CAN_BUF_MASK & C12P_MsgBuf_wIdx[index])
#define C12P_BUF_RDIDX(index) (CAN_BUF_MASK & C12P_MsgBuf_rIdx[index])
#define C12P_BUF_EMPTY(index) (C12P_BUF_RDIDX(index) == C12P_BUF_WRIDX(index))
#define C12P_BUF_FULL(index)  (C12P_BUF_RDIDX(index) == (CAN_BUF_MASK & (C12P_MsgBuf_wIdx[index]+1))) //(C12P_BUF_WRIDX+1))
#define C12P_BUF_COUNT(index) (CAN_BUF_MASK & (C12P_MsgBuf_wIdx[index] - C12P_MsgBuf_rIdx[index]))
//_VCI_CAN_MSG C12P_MsgBuf[MAX_I7565_SUPPORT][CAN_BUF_SIZE];

/* CAN2->PCUSB Define */
DWORD C22P_MsgBuf_rIdx[MAX_I7565_SUPPORT] = {0};
DWORD C22P_MsgBuf_wIdx[MAX_I7565_SUPPORT] = {0};
BYTE C22P_BufFull[MAX_I7565_SUPPORT] = {0};
#define C22P_BUF_WRIDX(index) (CAN_BUF_MASK & C22P_MsgBuf_wIdx[index])
#define C22P_BUF_RDIDX(index) (CAN_BUF_MASK & C22P_MsgBuf_rIdx[index])
#define C22P_BUF_EMPTY(index) (C22P_BUF_RDIDX(index) == C22P_BUF_WRIDX(index))
#define C22P_BUF_FULL(index)  (C22P_BUF_RDIDX(index) == (CAN_BUF_MASK & (C22P_MsgBuf_wIdx[index]+1))) //(C22P_BUF_WRIDX+1))
#define C22P_BUF_COUNT(index) (CAN_BUF_MASK & (C22P_MsgBuf_wIdx[index] - C22P_MsgBuf_rIdx[index]))
//_VCI_CAN_MSG C22P_MsgBuf[MAX_I7565_SUPPORT][CAN_BUF_SIZE];

/* PCUSB->CAN1 Define */
DWORD P2C1_MsgBuf_rIdx[MAX_I7565_SUPPORT] = {0};
DWORD P2C1_MsgBuf_wIdx[MAX_I7565_SUPPORT] = {0};
BYTE P2C1_BufFull[MAX_I7565_SUPPORT] = {0};
#define P2C1_BUF_WRIDX(index) (CAN_BUF_MASK & P2C1_MsgBuf_wIdx[index])
#define P2C1_BUF_RDIDX(index) (CAN_BUF_MASK & P2C1_MsgBuf_rIdx[index])
#define P2C1_BUF_EMPTY(index) (P2C1_BUF_RDIDX(index) == P2C1_BUF_WRIDX(index))
#define P2C1_BUF_FULL(index) (P2C1_BUF_RDIDX(index) == (CAN_BUF_MASK & (P2C1_MsgBuf_wIdx[index]+1))) //(U2C1_BUF_WRIDX+1))
#define P2C1_BUF_COUNT(index) (CAN_BUF_MASK & (P2C1_MsgBuf_wIdx[index] - P2C1_MsgBuf_rIdx[index]))
//DWORD P2C1_MsgBuf[MAX_I7565_SUPPORT][CAN_BUF_SIZE][5];

/* PCUSB->CAN2 Define */
DWORD P2C2_MsgBuf_rIdx[MAX_I7565_SUPPORT] = {0};
DWORD P2C2_MsgBuf_wIdx[MAX_I7565_SUPPORT] = {0};
BYTE P2C2_BufFull[MAX_I7565_SUPPORT] = {0};
#define P2C2_BUF_WRIDX(index) (CAN_BUF_MASK & P2C2_MsgBuf_wIdx[index])
#define P2C2_BUF_RDIDX(index) (CAN_BUF_MASK & P2C2_MsgBuf_rIdx[index])
#define P2C2_BUF_EMPTY(index) (P2C2_BUF_RDIDX(index) == P2C2_BUF_WRIDX(index))
#define P2C2_BUF_FULL(index)  (P2C2_BUF_RDIDX(index) == (CAN_BUF_MASK & (P2C2_MsgBuf_wIdx[index]+1))) //(U2C2_BUF_WRIDX+1))
#define P2C2_BUF_COUNT(index) (CAN_BUF_MASK & (P2C2_MsgBuf_wIdx[index] - P2C2_MsgBuf_rIdx[index]))
//DWORD P2C2_MsgBuf[MAX_I7565_SUPPORT][CAN_BUF_SIZE][5];


/************    VCI_CAN  thread Variable   **************/
typedef struct receivethread rxthread_t;
static rxthread_t *rx_thread[MAX_I7565_SUPPORT] = {0};
static int enable_rxthread[MAX_I7565_SUPPORT] = {0};

