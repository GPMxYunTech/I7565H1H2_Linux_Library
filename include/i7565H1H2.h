/**********************************************************************
 *
 *  i7565H1H2.h
 *
 *  Header for i7565H1H2.c
 *
 *  Since 2009.11.09 by Golden Wang
 *
 **********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include "msw.h"

/* Common Return Code */
#define No_Err			0	//No Error
#define DEV_ModName_Err		1	//The Module Name Error
#define DEV_ModNotExist_Err	2	//The Module doesn't exist in this Port
#define DEV_PortNotOpen_Err	3	//The Port doesn't Open
#define DEV_PortClose_Err	4	//The Port doesn't Open
#define DEV_Reset_Err		5	//Reset I-7565 module failure
#define CAN_ConfigureFail_Err	6	//CAN Hardware Init Fail
#define CAN_Hardware_Err	7	//CAN Hardware Init Fail
#define CAN_PortNo_Err		8	//The Device doesn't support this CAN Port
#define CAN_FIDLength_Err	9	//The CAN Filter-ID Number exceed Max Number
#define CAN_DevDisconnect_Err	10	//The Connection of device is broken
#define CAN_TimeOut_Err		11	//The Config Command Timeout
#define CAN_ConfigCmd_Err	12	//The Config Command doesn't support
#define CAN_ConfigBusy_Err	13	//The Config Command is busy
#define CAN_RxBufEmpty		14	//The CAN Receive Buffer is empty
#define CAN_TxBufFull		15	//The CAN Send Buffer is full
#define CAN_EnableHWCyclicTxMsg_Err 16  //Enable Hardware Cyclic Tx Message Error
#define CreateRxThread_Err 17  		//Create Rx Thread Error
#define RestartRxThread_Err 18		//Restartup Rx Thread Error

/************************      VCI_CAN API Variable      *************************/ 
//Common Define
#define I7565_LIBRARY_VERSION "0.4.5"
#define MAX_I7565_SUPPORT 256
#define CAN1			1
#define CAN2			2
#define CONFIG_MODE		0
#define COMM_MODE		1
#define CAN_BUF_SIZE            65536
#define CAN_BUF_MASK            0xffff

//Device Type
#define I7565H1			1
#define I7565H2			2
	
//CAN Baudrate
#define CANBaud_5K		5000
#define CANBaud_10K		10000
#define CANBaud_20K		20000
#define CANBaud_40K		40000
#define CANBaud_50K		50000
#define CANBaud_80K		80000
#define CANBaud_100K		100000
#define CANBaud_125K		125000
#define CANBaud_200K		200000
#define CANBaud_250K		250000
#define CANBaud_400K		400000
#define CANBaud_500K		500000
#define CANBaud_600K		600000
#define CANBaud_800K		800000
#define CANBaud_1000K		1000000

/* ICPDAS VCICAN Series Message Structure Format */
//[1]. VCICAN - Module Info Format
typedef struct _VCI_MODULE_INFO{
	char	Mod_ID[12];
	char	FW_Ver[12];
	char	HW_SN[16];
} _VCI_MODULE_INFO, *PVCI_MOD_INFO; 

//[2]. VCICAN - CAN Message Format
typedef struct _VCI_CAN_MSG{
        BYTE    Mode;
        BYTE    RTR;
        BYTE    DLC;
        BYTE    Reserved;
        DWORD   ID;
        DWORD   TimeL;
        DWORD   TimeH;
        BYTE    Data[8];
} _VCI_CAN_MSG, *PVCI_CAN_MSG;


//[3]. VCICAN - CAN Parameter (CANBaud: Unit(bps))
typedef struct _VCI_CAN_PARAM{
	BYTE  PortOpen;
	BYTE  DevPort;
	BYTE  DevType;
	DWORD CAN1_Baud;
	DWORD CAN2_Baud;
	//DWORD C12P_MsgBuf_rIdx;
	//DWORD C12P_MsgBuf_wIdx;
	//BYTE  C12P_BufFull;
	_VCI_CAN_MSG C12P_MsgBuf[CAN_BUF_SIZE];
	//DWORD C22P_MsgBuf_rIdx;
        //DWORD C22P_MsgBuf_wIdx;
	//BYTE  C22P_BufFull
	_VCI_CAN_MSG C22P_MsgBuf[CAN_BUF_SIZE];
	DWORD P2C1_MsgBuf[CAN_BUF_SIZE][5];
	DWORD P2C2_MsgBuf[CAN_BUF_SIZE][5];
} _VCI_CAN_PARAM, *PVCI_CAN_PARAM;

typedef struct _VCI_CAN_PARAM_EX{
	_VCI_CAN_PARAM vc;
	BYTE	CAN1_T2Val;
	BYTE	CAN2_T2Val;
	BYTE	Reserved[32];
} _VCI_CAN_PARAM_EX, *PVCI_CAN_PARAM_EX;

//[4]. VCICAN - CAN FilterID Format
typedef struct _VCI_CAN_FilterID{
	WORD	SSFF_Num;
	WORD	GSFF_Num;
	WORD	SEFF_Num;
	WORD	GEFF_Num;
	WORD	SSFF_FID[512];
	DWORD	GSFF_FID[512];
	DWORD	SEFF_FID[512];
	DWORD	GEFF_FID[512];
} _VCI_CAN_FilterID, *PVCI_CAN_FID;

//[5]. VCICAN - CAN Status Format
typedef struct _VCI_CAN_STATUS{
	DWORD	CurCANBaud;
	BYTE	CANReg;
	BYTE	CANTxErrCnt;
	BYTE	CANRxErrCnt;
	BYTE	MODState;
	DWORD	Reserved;
} _VCI_CAN_STATUS, *PVCI_CAN_STATUS;

/************************      VCI_CAN API Function      *************************/ 
/********************     [1] Init Function      ********************/
int VCI_OpenCAN(PVCI_CAN_PARAM pCANPARAM);
int VCI_OpenCAN_NoStruct(BYTE DevPort, BYTE DevType, DWORD CAN1_Baud, DWORD CAN2_Baud);
int VCI_CloseCAN(PVCI_CAN_PARAM pCANPARAM);


/********************     [2] Module Config Function     ********************/
int VCI_Set_CANFID(BYTE DevPort, BYTE CAN_No, PVCI_CAN_FID pCANFID);
int VCI_Set_CANFID_NoStruct(BYTE DevPort, BYTE CAN_No, WORD SSFF_Num, WORD GSFF_Num, WORD SEFF_Num, WORD GEFF_Num, WORD SSFF_FID[512], DWORD GSFF_FID[512], DWORD SEFF_FID[512], DWORD GEFF_FID[512]);
//int VCI_Set_CANFID_AllPass(BYTE DevPort, BYTE CAN_No);
int VCI_Get_CANFID(BYTE DevPort, BYTE CAN_No, PVCI_CAN_FID pCANFID);
int VCI_Get_CANFID_NoStruct(BYTE DevPort, BYTE CAN_No, WORD* SSFF_Num, WORD* GSFF_Num, WORD* SEFF_Num, WORD* GEFF_Num, WORD SSFF_FID[512], DWORD GSFF_FID[512], DWORD SEFF_FID[512], DWORD GEFF_FID[512]);
int VCI_Clr_CANFID(BYTE DevPort, BYTE CAN_No);
int VCI_Get_CANStatus(BYTE DevPort, BYTE CAN_No, PVCI_CAN_STATUS pCANStatus);
int VCI_Get_CANStatus_NoStruct(BYTE DevPort, BYTE CAN_No, DWORD* CurCANBaud, BYTE* CANReg, BYTE* CANTxErrCnt, BYTE* CANRxErrCnt, BYTE* MODState);
int VCI_Clr_BufOverflowLED(BYTE DevPort, BYTE CAN_No);
int VCI_Get_MODInfo(BYTE DevPort, PVCI_MOD_INFO pMODInfo);
int VCI_Get_MODInfo_NoStruct(BYTE DevPort, char Mod_ID[12], char FW_Ver[12], char HW_SN[16]);
int VCI_Rst_MOD(PVCI_CAN_PARAM pCANPARAM);
int VCI_Set_MOD_Ex(BYTE CfgData[512]);

/********************     [3] Communication Function     ********************/
int VCI_SendCANMsg(BYTE DevPort, BYTE CAN_No, PVCI_CAN_MSG pCANMsg);
int VCI_SendCANMsg_NoStruct(BYTE DevPort, BYTE CAN_No, BYTE Mode, BYTE RTR, BYTE DLC, DWORD ID, BYTE Data[8]);
int VCI_RecvCANMsg(BYTE DevPort, BYTE CAN_No, PVCI_CAN_MSG pCANMsg);
int VCI_RecvCANMsg_NoStruct(BYTE DevPort, BYTE CAN_No, BYTE* Mode, BYTE* RTR, BYTE* DLC, DWORD* ID, DWORD* TimeL, DWORD* TimeH, BYTE Data[8]);
int VCI_EnableHWCyclicTxMsg(BYTE DevPort, BYTE CAN_No, PVCI_CAN_MSG pCANMsg, DWORD TimePeriod, DWORD TransmitTimes);
int VCI_EnableHWCyclicTxMsg_NoStruct(BYTE DevPort, BYTE CAN_No, BYTE Mode, BYTE RTR, BYTE DLC, DWORD ID, BYTE Data[8], DWORD TimePeriod, DWORD TransmitTimes);
int VCI_DisableHWCyclicTxMsg(PVCI_CAN_PARAM pCANPARAM);


/********************     [4] Software Buffer Function     ********************/
int VCI_Get_RxMsgCnt(BYTE DevPort, BYTE CAN_No, DWORD* RxMsgCnt);
int VCI_Get_RxMsgBufIsFull(BYTE DevPort, BYTE CAN_No, BYTE* Flag);
int VCI_Clr_RxMsgBuf(BYTE DevPort, BYTE CAN_No);
int VCI_Get_TxMsgCnt(BYTE DevPort, BYTE CAN_No, DWORD* TxMsgCnt);
int VCI_Clr_TxMsgBuf(BYTE DevPort, BYTE CAN_No);
int VCI_Get_TxSentCnt(BYTE DevPort, BYTE CAN_No, DWORD* TxSentCnt);
int VCI_Clr_TxSentCnt(BYTE DevPort, BYTE CAN_No);

/********************     [5] Other Function     ********************/
char * VCI_Get_Library_Version(void);


/********************     [7] Extended Function     ********************/

int VCI_OpenCAN_Ex(PVCI_CAN_PARAM_EX pCANPARAMEx);  //done
int VCI_OpenCAN_NoStruct_Ex(BYTE DevPort, BYTE DevType, DWORD CAN1_Baud, DWORD CAN2_Baud, BYTE CAN1_T2Val, BYTE CAN2_T2Val);  //done
int VCI_Get_CANBaud_BitTime(BYTE DevPort, BYTE CAN_No, BYTE* T1Val, BYTE* T2Val, BYTE* SJWVal);
