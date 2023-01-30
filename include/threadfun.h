#include<time.h>
#include "global.h"

extern DWORD C1TxSentCnt, C2TxSentCnt;

//typedef struct receivethread rxthread_t;
struct receivethread
{
        int ReceiveHandle;
        pthread_t RxThread;
        PVCI_CAN_PARAM Interface;
        pthread_mutex_t mutex;
        pthread_cond_t  cond;
        WORD (*thread_func)(struct receivethread *, BOOL);
};

 //0.4.3 V5
#if 1
void new_rxthread(rxthread_t * NewThread, BYTE DevPort);
#else
rxthread_t * new_rxthread(BYTE DevPort);
#endif
WORD rxthread_threadfunc(struct receivethread *thread, BOOL Flage);
void Rx_Thread(PVCI_CAN_PARAM channel);

//0.4.3 V5
#if 1
void new_rxthread(rxthread_t * NewThread, BYTE DevPort)
#else
rxthread_t * new_rxthread(BYTE DevPort)
#endif
{
        int ret;
	int index = DevPort - USB0;

//0.4.3 V5
#if 1
        rxthread_t *thread = NewThread;
#else
        rxthread_t *thread = malloc(sizeof(rxthread_t));
#endif
        memset(thread, 0, sizeof(rxthread_t));

        thread->Interface = malloc(sizeof(_VCI_CAN_PARAM));
        memset(thread->Interface, 0, sizeof(_VCI_CAN_PARAM));

        thread->Interface->PortOpen = 1;
        thread->Interface->DevPort = DevPort;

        pthread_cond_init(&thread->cond, NULL);
        pthread_mutex_init(&thread->mutex, NULL);

        ret = pthread_create(&thread->RxThread, NULL, (void *) Rx_Thread,(void *) thread->Interface);

        if(ret)
	{
                thread->ReceiveHandle = 0;	//create Rx_Thread fail
	}
        else
	{
                enable_rxthread[index] = 1;
                thread->ReceiveHandle = 1;	//create Rx_Thread OK
	}

        thread->thread_func = rxthread_threadfunc;

	//0.4.3 V5
        //return thread;
}

WORD rxthread_threadfunc(struct receivethread *thread, BOOL Flage)
{
        DWORD ret;
	BYTE DevPort = thread->Interface->DevPort;
	int index = DevPort - USB0;

        if(!thread->ReceiveHandle)
                return FAILURE;

        if(Flage)
        {
                pthread_mutex_lock( &thread->mutex );
                enable_rxthread[index] = 1;	//resume Rx_Thread
                ret = pthread_cond_signal( &thread->cond );
                pthread_mutex_unlock( &thread->mutex );

                if (ret)
                        return FAILURE;
        }
        else
        {
                pthread_mutex_lock( &thread->mutex );
                enable_rxthread[index] = 0;	//suspend Rx_Thread
                pthread_mutex_unlock( &thread->mutex );
        }

        return SUCCESS;
}

void Rx_Thread(PVCI_CAN_PARAM channel)
{
	WORD Ret, wt;
        DWORD Sleep_Cnt=0;
        DWORD i=0;//, j=0;
	BYTE DevPort = channel->DevPort;
        BYTE DeviceType = channel->DevType;
	int index = DevPort - USB0;
	BYTE U2C_Buf[2048] = {0};

	while(!rx_thread[index])
	{
		//printf("initialize rx_thread[%d] memory OK\n", index);
	}

	//printf("%s ... DevPort %d : PortOpen %d\n", __FUNCTION__, DevPort, channel->PortOpen);
	
        while(channel->PortOpen)
        {
		if(!enable_rxthread[index])
		{
			pthread_mutex_lock( &rx_thread[index]->mutex );
			pthread_cond_wait( &rx_thread[index]->cond, &rx_thread[index]->mutex );
			pthread_mutex_unlock( &rx_thread[index]->mutex );
		}
		pthread_mutex_lock(&rx_thread[index]->mutex);

		//printf("%s ... DevPort %d : PortOpen %d\n", __FUNCTION__, DevPort,channel->PortOpen);
		//time_t tBeginTime = time(NULL);
		Ret = Receive_Binary(DevPort, (char*)U2C_Buf, 1, 20, &wt);
		//time_t tEndTime = time(NULL);
		//double fCostTime = difftime(tEndTime, tBeginTime);
		//printf( " [time]Cost Time = %fSec\n " , fCostTime);

		if(!Ret)
		{
			if(((U2C_Buf[i*20+3]>>7)&0x01) == CAN1_RecvFlag)
			{
				channel->C12P_MsgBuf[C12P_BUF_WRIDX(index)].Mode = ((U2C_Buf[i*20+3]>>6)&0x1);
				channel->C12P_MsgBuf[C12P_BUF_WRIDX(index)].RTR = ((U2C_Buf[i*20+3]>>5)&0x1);
				channel->C12P_MsgBuf[C12P_BUF_WRIDX(index)].ID = ((*((DWORD*)&U2C_Buf[i*20+0]))&0x1FFFFFFF);
				//printf("Rx_Thread CAN Port 1 Get ID 0x%x\n", channel->C12P_MsgBuf[C12P_BUF_WRIDX].ID);
				channel->C12P_MsgBuf[C12P_BUF_WRIDX(index)].DLC = ((U2C_Buf[i*20+7]>>4)&0xF);
				channel->C12P_MsgBuf[C12P_BUF_WRIDX(index)].TimeH = ((*((DWORD*)&U2C_Buf[i*20+4]))&0x0FFFFFFF);
				channel->C12P_MsgBuf[C12P_BUF_WRIDX(index)].TimeL = *((DWORD*)&U2C_Buf[i*20+8]);
				*((DWORD*)&channel->C12P_MsgBuf[C12P_BUF_WRIDX(index)].Data[0]) = *((DWORD*)&U2C_Buf[i*20+12]);
				//printf("Rx_Thread CAN Port 1 Data[0~3] 0x%x\n", *((DWORD*)&U2C_Buf[i*20+12]));

				*((DWORD*)&channel->C12P_MsgBuf[C12P_BUF_WRIDX(index)].Data[4]) = *((DWORD*)&U2C_Buf[i*20+16]);
				//printf("Rx_Thread CAN Port 1 Data[4~7] 0x%x\n", *((DWORD*)&U2C_Buf[i*20+16]));

#ifndef CAN_EFFICEINCY_TEST
				if(C12P_BUF_FULL(index))
				{
					C12P_BufFull[index] = 1;
				}
				else
				{
					C12P_MsgBuf_wIdx[index]++;
				}
#else
				C12P_MsgBuf_wIdx[index]++;
#endif
			}
			else
			{
				channel->C22P_MsgBuf[C22P_BUF_WRIDX(index)].Mode = ((U2C_Buf[i*20+3]>>6)&0x1);
				channel->C22P_MsgBuf[C22P_BUF_WRIDX(index)].RTR = ((U2C_Buf[i*20+3]>>5)&0x1);
				channel->C22P_MsgBuf[C22P_BUF_WRIDX(index)].ID = ((*((DWORD*)&U2C_Buf[i*20+0]))&0x1FFFFFFF);
				//printf("Rx_Thread CAN Port 2 Get ID 0x%x\n", channel->C22P_MsgBuf[C22P_BUF_WRIDX].ID);

				channel->C22P_MsgBuf[C22P_BUF_WRIDX(index)].DLC = ((U2C_Buf[i*20+7]>>4)&0xF);
				channel->C22P_MsgBuf[C22P_BUF_WRIDX(index)].TimeH = ((*((DWORD*)&U2C_Buf[i*20+4]))&0x0FFFFFFF);
				channel->C22P_MsgBuf[C22P_BUF_WRIDX(index)].TimeL = *((DWORD*)&U2C_Buf[i*20+8]);
				*((DWORD*)&channel->C22P_MsgBuf[C22P_BUF_WRIDX(index)].Data[0]) = *((DWORD*)&U2C_Buf[i*20+12]);
				//printf("Rx_Thread CAN Port 2 Data[0~3] 0x%x\n", *((DWORD*)&U2C_Buf[i*20+12]));

				*((DWORD*)&channel->C22P_MsgBuf[C22P_BUF_WRIDX(index)].Data[4]) = *((DWORD*)&U2C_Buf[i*20+16]);
				//printf("Rx_Thread CAN Port 2 Data[4~7] 0x%x\n", *((DWORD*)&U2C_Buf[i*20+16]));

				if(C22P_BUF_FULL(index))
				{
					C22P_BufFull[index] = 1;
				}
				else
				{
					C22P_MsgBuf_wIdx[index]++;
				}
			}
		}
		//else
		//{
			if(P2C1_BUF_COUNT(index))
			{                                                          
				//printf("Rx_Thread CAN Port 1 ... Send\n");

				Send_Binary(DevPort, "0", 1);
				Send_Binary(DevPort, (char*)&channel->P2C1_MsgBuf[P2C1_BUF_RDIDX(index)][0], 20);
				P2C1_MsgBuf_rIdx[index]++;
				C1TxSentCnt++;
			}

			if(P2C2_BUF_COUNT(index))
			{
				//printf("Rx_Thread CAN Port 2 ... Send\n");
                                        
				Send_Binary(DevPort, "0", 1);
				Send_Binary(DevPort, (char*)&channel->P2C2_MsgBuf[P2C2_BUF_RDIDX(index)][0], 20);
				P2C2_MsgBuf_rIdx[index]++;
				C2TxSentCnt++;
			}
		//}

		pthread_mutex_unlock(&rx_thread[index]->mutex);


                if(((Sleep_Cnt++) & 0x1f) == 0)
                {
			Sleep_Cnt = 0;
                        usleep(1);
                }
        }
}

