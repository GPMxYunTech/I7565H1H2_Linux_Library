/* Example of I-7565-H1/H2 module

   Author: Golden Wang

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

/* File level history (record changes for this file here.)

   v 0.0.0 1 Nov 2009 by Golden Wang
     create, blah blah... */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include "i7565H1H2.h"
#include <pthread.h>

#define kbhit()   (int)getchar()
char Kbhit(void);

char Kbhit(void)
{
  int n=0;

  ioctl(0,FIONREAD,&n);
  if(n) return kbhit();

  return n;
}

int main(void)
{
	int usb, devtype, can1_baud = 0, can2_baud = 0, init = 0;
	char command, count;
	int i, wRetVal, j = 0, x;
        DWORD RxMsgCnt = 0;
        DWORD temp;
        WORD Ret;
        //_VCI_CAN_PARAM vcparam_ex.vc;
	_VCI_CAN_PARAM_EX vcparam_ex;
        _VCI_MODULE_INFO vminfo;
	char buf[16][10], *find = 0;
	FILE *fd;
	char ls_cmd[30], grep_cmd[100];
	BYTE CfgData[512];

	printf("----------------------\n");
	printf("SDK version %s\n",VCI_Get_Library_Version());
	printf("----------------------\n");

	sprintf(ls_cmd,"ls /dev | grep ttyUSB",0);  //get ttyUSBx in /dev

	if ((fd = popen(ls_cmd, "r")) == NULL) {  
		printf("popen() error!\n");  
		exit(1);  
	}  

	i = 0;
	printf("I7565-H1/H2 device name:\n");
	while (fgets(buf[i], sizeof buf, fd)) {  //get all ttyUSB device name.
		if ((find = strrchr(buf[i], '\n')))  //replace \n to \0
			*find = '\0';

		sprintf(grep_cmd,"grep 1b5c/20 /sys/bus/usb-serial/devices/%s/../uevent > /dev/null",buf[i]);  //get I7565 device name
		if(!system(grep_cmd))
		{
			for(x = 6; x <= 8; x++)
				buf[j][x] = buf[i][x];
			j++;
			printf("%d   %s\n", j, buf[i]);
		}
		i++;
	}
	pclose(fd);

	printf("USB-Serial Device Name :");
	scanf("%d",&usb);
	if (usb > j || usb <= 0)
	{
		printf("error number\n");
		return 0;
	}	
	count = strlen(buf[usb-1]);  //map variable j

	switch(count)
	{
		case 7:
		  vcparam_ex.vc.DevPort = buf[usb-1][6] - 48;
		  break;
		case 8:
		  vcparam_ex.vc.DevPort = (buf[usb-1][6] - 48) * 10 + buf[usb-1][7] - 48;
		  break;
		case 9:
		  vcparam_ex.vc.DevPort = (buf[usb-1][6] - 48) * 100 + (buf[usb-1][7] - 48) * 10 + buf[usb-1][8] - 48;
		  break;
		default:
		  printf("USB-Serial Device Name Error\r\n");
		  return 0;
	}

	sprintf(grep_cmd,"grep 1b5c/201 /sys/bus/usb-serial/devices/%s/../uevent > /dev/null",buf[usb-1]);

	if(!system(grep_cmd))
		vcparam_ex.vc.DevType = I7565H1;
	else
		vcparam_ex.vc.DevType = I7565H2;

	printf("You choose \"%s\", it's a I7565H%d module.\n",buf[usb-1], vcparam_ex.vc.DevType);
	
	printf("Configure I-7565-H1/H2 CAN Port Baudrate\r\n");
	printf("1  :    5 Kbps\r\n");
        printf("2  :   10 Kbps\r\n");
        printf("3  :   20 Kbps\r\n");
        printf("4  :   40 Kbps\r\n");
        printf("5  :   50 Kbps\r\n");
        printf("6  :   80 Kbps\r\n");
        printf("7  :  100 Kbps\r\n");
        printf("8  :  125 Kbps\r\n");
        printf("9  :  200 Kbps\r\n");
        printf("10 :  250 Kbps\r\n");
        printf("11 :  400 Kbps\r\n");
        printf("12 :  500 Kbps\r\n");
        printf("13 :  600 Kbps\r\n");
        printf("14 :  800 Kbps\r\n");
        printf("15 : 1000 Kbps\r\n");

	if(vcparam_ex.vc.DevType == I7565H1)
	{
		printf("I-7565-H1 CAN1 baud(1~15):");
		scanf("%d",&can1_baud);
	}
	else if(vcparam_ex.vc.DevType == I7565H2)
	{
		printf("I-7565-H2 CAN1 baud(1~15):");
		scanf("%d",&can1_baud);

		printf("I-7565-H2 CAN2 baud(1~15):");
		scanf("%d",&can2_baud);

	}

	if(can1_baud > 0)
	{
		switch(can1_baud)
                {
                        case   1: vcparam_ex.vc.CAN1_Baud = CANBaud_5K; break;
                        case   2: vcparam_ex.vc.CAN1_Baud = CANBaud_10K; break;
                        case   3: vcparam_ex.vc.CAN1_Baud = CANBaud_20K; break;
                        case   4: vcparam_ex.vc.CAN1_Baud = CANBaud_40K; break;
                        case   5: vcparam_ex.vc.CAN1_Baud = CANBaud_50K; break;
                        case   6: vcparam_ex.vc.CAN1_Baud = CANBaud_80K; break;
                        case   7: vcparam_ex.vc.CAN1_Baud = CANBaud_100K; break;
                        case   8: vcparam_ex.vc.CAN1_Baud = CANBaud_125K; break;
                        case   9: vcparam_ex.vc.CAN1_Baud = CANBaud_200K; break;
                        case   10: vcparam_ex.vc.CAN1_Baud = CANBaud_250K; break;
                        case   11: vcparam_ex.vc.CAN1_Baud = CANBaud_400K; break;
                        case   12: vcparam_ex.vc.CAN1_Baud = CANBaud_500K; break;
                        case   13: vcparam_ex.vc.CAN1_Baud = CANBaud_600K; break;
                        case   14: vcparam_ex.vc.CAN1_Baud = CANBaud_800K; break;
                        case   15: vcparam_ex.vc.CAN1_Baud = CANBaud_1000K; break;
                        default  : vcparam_ex.vc.CAN1_Baud = CANBaud_1000K; break;
                }
	}

	if(can2_baud > 0)
        {
                switch(can2_baud)
                {
                        case   1: vcparam_ex.vc.CAN2_Baud = CANBaud_5K; break;
                        case   2: vcparam_ex.vc.CAN2_Baud = CANBaud_10K; break;
                        case   3: vcparam_ex.vc.CAN2_Baud = CANBaud_20K; break;
                        case   4: vcparam_ex.vc.CAN2_Baud = CANBaud_40K; break;
                        case   5: vcparam_ex.vc.CAN2_Baud = CANBaud_50K; break;
                        case   6: vcparam_ex.vc.CAN2_Baud = CANBaud_80K; break;
                        case   7: vcparam_ex.vc.CAN2_Baud = CANBaud_100K; break;
                        case   8: vcparam_ex.vc.CAN2_Baud = CANBaud_125K; break;
                        case   9: vcparam_ex.vc.CAN2_Baud = CANBaud_200K; break;
                        case   10: vcparam_ex.vc.CAN2_Baud = CANBaud_250K; break;
                        case   11: vcparam_ex.vc.CAN2_Baud = CANBaud_400K; break;
                        case   12: vcparam_ex.vc.CAN2_Baud = CANBaud_500K; break;
                        case   13: vcparam_ex.vc.CAN2_Baud = CANBaud_600K; break;
                        case   14: vcparam_ex.vc.CAN2_Baud = CANBaud_800K; break;
                        case   15: vcparam_ex.vc.CAN2_Baud = CANBaud_1000K; break;
                        default  : vcparam_ex.vc.CAN2_Baud = CANBaud_1000K; break;
                }
        }

	/* Set Tseg2 Value */
        printf("Set Tseg2 Value\n");
        if(vcparam_ex.vc.DevType == I7565H1)
        {
                printf("I-7565-H1 Tesg2 Value(2~6):");
                scanf("%d",&vcparam_ex.CAN1_T2Val);
        }
        else if(vcparam_ex.vc.DevType == I7565H2)
        {
                printf("I-7565-H2 CAN1 Tesg2 Value(2~6):");
		scanf("%d",&vcparam_ex.CAN1_T2Val);

                printf("I-7565-H2 CAN2 Tesg2 Value(2~6):");
		scanf("%d",&vcparam_ex.CAN2_T2Val);
        }

	/* Set the module parameters of new functions */
        printf("Set Listen Only Mode:\n");
        if(vcparam_ex.vc.DevType == I7565H1)
        {
                printf("I-7565-H1 CAN1 Listen Only Mode(0 disable, 1 enable):");
                scanf("%d",&CfgData[0]);  //CAN1 => 0:Disable, 1:Enable
        }
        else if(vcparam_ex.vc.DevType == I7565H2)
        {
                printf("I-7565-H2 CAN1 Listen Only Mode(0 disable, 1 enable):");
		scanf("%d",&CfgData[0]);  //CAN1 => 0:Disable, 1:Enable	

                printf("I-7565-H2 CAN2 Listen Only Mode(0 disable, 1 enable):");
		scanf("%d",&CfgData[1]);  //CAN2 => 0:Disable, 1:Enable

        }

        VCI_Set_MOD_Ex(CfgData);

	getchar();

	wRetVal = VCI_OpenCAN_Ex(&vcparam_ex);

        if (wRetVal > 0) 
	{
                printf("open %d virtual COM port failed! Erro : %d\r\n",  vcparam_ex.vc.DevPort, wRetVal);
                return 0;
        }

	printf("a. Get I-7565-H1/H2 CAN State:\r\n");	//done
	printf("b. Get I-7565-H1/H2 Module Information:\r\n");  //done
	printf("c. Set I-7565-H1/H2 CAN Filter ID:\r\n"); //done
	printf("d. Get I-7565-H1/H2 CAN Filter ID:\r\n"); //done
	printf("e. Clear I-7565-H1/H2 CAN Filter ID:\r\n"); //done
	printf("f. Clear I-7565-H1/H2 Overflow LED:\r\n"); //done
	printf("g. Reset I-7565-H1/H2 Module:\r\n");//done
	printf("h. Send CAN Message:\r\n");//done
	printf("i. Receive CAN Message:\r\n");    //done
	printf("j. Enable Hardware Cyclic Send CAN Message:\r\n");//done
	printf("k. Disable Hardware Cyclic Send CAN Message:\r\n");//done
	printf("l. Get I-7565-H1/H2 Rx Message Count:\r\n");	//done
	printf("m. Get I-7565-H1/H2 Rx Message Buffer State:\r\n");	//done
	printf("n. Clear I-7565-H1/H2 Rx Message Buffer:\r\n");  //done
	printf("o. Get or Clear I-7565-H1/H2 Tx sent count:\r\n");  //done
	printf("p. Show All I-7565-H1/H2function:\r\n");//done
	printf("q. Shutdown and exit:\r\n"); //done
	printf("r. Get I-7565-H1/H2 Tseg1, Tseg2, SJWval value:\r\n");
    
	while(1)
	{
		command = Kbhit();
		//scanf("%c",&command);

		if(command)
		{
			switch(command)
			{
				case 'a':
				{

					int port;
					_VCI_CAN_STATUS vcstatus;

					if (vcparam_ex.vc.DevType == I7565H2)
					{
						printf("CAN Port : Press CAN Port number(1~2):");
						//port = CAN1 or CAN2
	                                        scanf("%d",&port);
        	                                getchar();
					}
					else
					{
						port = CAN1;	//CAN1 = 1
        	                                getchar();
					}
	
					printf("get CAN Status : DevPort %d\n",vcparam_ex.vc.DevPort);
					Ret = VCI_Get_CANStatus(vcparam_ex.vc.DevPort, port, &vcstatus);
					if(Ret)
                                        {
                                                printf("I-7565-H1/H2 Error : Error Code = %d\r\n",Ret);
                                        }
                                        else
                                        {
                                                printf("I-7565-H1/H2 CAN Port Status\r\n");
						printf("CAN Port %d : Baudrate %d K\r\n", port, (vcstatus.CurCANBaud/1000));
						printf("CAN Port %d : Error Count Tx %d Rx %d\r\n", port, vcstatus.CANTxErrCnt, vcstatus.CANRxErrCnt);
						printf("CAN Port %d : Module State %d\r\n", port, vcstatus.MODState);
                                        }
      				}
      				break;

				case 'b':
				{
					getchar();

					Ret = VCI_Get_MODInfo(vcparam_ex.vc.DevPort, &vminfo);

					if(Ret)
                                        {
                                                printf("I-7565-H1/H2 Error : Error Code = %d\r\n",Ret);
                                        }
                                        else
                                        {
						printf("Module ID : %s\r\n",vminfo.Mod_ID);
						printf("Module Firmware : %s\r\n",vminfo.FW_Ver);
						printf("Hareware Serial Number : %s\r\n",vminfo.HW_SN);
                                        }

      				}
      				break;

				case 'c':
				{
					int port;
					_VCI_CAN_FilterID CANFID;

					//Single Standard FID (11-bit)
					WORD  SSFID[3]={0x0124, 0x0122, 0x0123};

					//Group  Standard FID (11-bit)
					DWORD GSFID[2]={0x00300040, 0x00100020};
					
					//Single Extended FID (29-bit)
					DWORD SEFID[3]={0x00000013, 0x00000012,0x00000011};

					//Group  Extended FID (29-bit)
					DWORD GEFID[4]={0x00000300, 0x00000400, 0x00000200, 0x00000100};

					if (vcparam_ex.vc.DevType == I7565H2)
                                        {
                                                printf("CAN Port : Press CAN Port number(1~2):");
						//port = CAN1 or CAN2
                                                scanf("%d",&port);
                                                getchar();
                                        }
                                        else
                                        {
                                                port = CAN1;	//CAN1 = 1
                                                getchar();
                                        }

					CANFID.SSFF_Num = sizeof(SSFID)/sizeof(WORD);
					CANFID.GSFF_Num = sizeof(GSFID)/sizeof(DWORD);
					CANFID.SEFF_Num = sizeof(SEFID)/sizeof(DWORD);
					CANFID.GEFF_Num = sizeof(GEFID)/sizeof(DWORD);

					memcpy(CANFID.SSFF_FID, SSFID, CANFID.SSFF_Num*2);
					memcpy(CANFID.GSFF_FID, GSFID, CANFID.GSFF_Num*4);
					memcpy(CANFID.SEFF_FID, SEFID, CANFID.SEFF_Num*4);
					memcpy(CANFID.GEFF_FID, GEFID, CANFID.GEFF_Num*4);

                                        Ret = VCI_Set_CANFID(vcparam_ex.vc.DevPort, port, &CANFID);
					if(Ret)
                                        {
                                                printf("I-7565-H1/H2 Error : Error Code = %d\r\n",Ret);
                                        }
                                        else
                                        {
                                                printf("Set CAN%d Filter ID OK\r\n",port);
                                        }

					usleep(100000);


      				}
				break;
	
				case 'd':
				{
					int port, i;
					_VCI_CAN_FilterID CANFID;
					WORD SID11_EndNum=0, GID11_EndNum=0, SID29_EndNum=0, GID29_EndNum=0;

					if (vcparam_ex.vc.DevType == I7565H2)
                                        {
                                                printf("CAN Port : Press CAN Port number(1~2):");
						//port = CAN1 or CAN2
                                                scanf("%d",&port);
                                                getchar();
                                        }
                                        else
                                        {
                                                port = CAN1;	//CAN1 = 1
                                                getchar();
                                        }

                                        Ret = VCI_Get_CANFID(vcparam_ex.vc.DevPort, port, &CANFID);	
                                        if(Ret)
                                        {
                                                printf("I-7565-H1/H2 Error : Error Code = %d\r\n",Ret);
                                        }
                                        else
                                        {
						SID11_EndNum = CANFID.SSFF_Num;
				                GID11_EndNum = CANFID.GSFF_Num;
				                SID29_EndNum = CANFID.SEFF_Num;
				                GID29_EndNum = CANFID.GEFF_Num;

                                                printf("I-7565-H1/H2 CAN Port %d Filter State\r\n",port);
						//show 11-bits Single Standard filter ID
                				printf("CAN%d 11-bits Single Standard FID Number = %d\r\n", port, SID11_EndNum);
						for(i = 0; i < SID11_EndNum; i++)
						{
							printf("11-bits SSFF_FID : 0x%04x\r\n", CANFID.SSFF_FID[i]); 
						}

						//show Group Standard filter ID
                				printf("\nCAN%d 11-bits Group Standard FID Number = %d\r\n", port, GID11_EndNum);
						for(i = 0; i < GID11_EndNum; i++)
                                                {
							printf("11-bits GSFF_FID : 0x%04x ~ 0x%04x\n", ((CANFID.GSFF_FID[i]>>16)&0xffff), (CANFID.GSFF_FID[i]&0x0000ffff) );
                                                }
                				
						//show Single Extended filter ID
						printf("\nCAN%d 29-bits Single Extended FID Number = %d\r\n", port, SID29_EndNum);

						for(i = 0; i < SID29_EndNum; i++)
                                                {
                                                        printf("29-bits SEFF_FID : 0x%08x\r\n", CANFID.SEFF_FID[i]);
                                                }

						//show Group Extended filter ID
						printf("\nCAN%d 29-bits Group Extended FID Number = %d\r\n", port, (GID29_EndNum/2));
						for(i = 0; i < (GID29_EndNum/2); i++)
                                                {
							if ( CANFID.GEFF_FID[i*2] >  CANFID.GEFF_FID[(i*2)+1])
							{
                                                        	printf("29-bits GEFF_FID : 0x%08x ~ 0x%08x\r\n",  CANFID.GEFF_FID[(i*2)+1], CANFID.GEFF_FID[i*2]);
							}
							else
							{
                                                        	printf("29-bits GEFF_FID : 0x%08x ~ 0x%08x\r\n",  CANFID.GEFF_FID[i*2], CANFID.GEFF_FID[(i*2)+1]);
	
							}
                                                }
                                        }
      				}
      				break;

				case 'e':
				{
					int port;

                                        if (vcparam_ex.vc.DevType == I7565H2)
                                        {
                                                printf("CAN Port : Press CAN Port number(1~2):");
						//port = CAN1 or CAN2
                                                scanf("%d",&port);
                                                getchar();
                                        }
                                        else
                                        {
                                                port = CAN1; //CAN1 = 1
                                                getchar();
                                        }

                                        Ret = VCI_Clr_CANFID(vcparam_ex.vc.DevPort, port);

                                        if(Ret)
                                        {
                                                printf("I-7565-H1/H2 Error : Error Code = %d\r\n",Ret);
                                        }
                                        else
                                        {
                                                printf("CAN Port %d : Clear Filter ID OK\r\n",port);
                                        }

				}
				break;
	
				case 'f':
				{
					int port;

					if (vcparam_ex.vc.DevType == I7565H2)
					{
						printf("CAN Port : Press CAN Port number(1~2):");
						//port = CAN1 or CAN2
                                                scanf("%d",&port);
						getchar();
					}
					else
					{
						port = CAN1;	//CAN1 = 1
						getchar();
					}

					Ret = VCI_Clr_BufOverflowLED(vcparam_ex.vc.DevPort, port);
					usleep(100000);

					if(Ret)
                                        {
                                                printf("I-7565-H1/H2 Error : Error Code = %d\r\n",Ret);
                                        }
                                        else
                                        {
                                                printf("CAN Port %d : Clear Buffer Overflow LED OK\r\n",port);
                                        }
      				}
				break;

				case 'g':
				{
					getchar();

					Ret = VCI_Rst_MOD(&vcparam_ex.vc);

					if(Ret)
                                        {
                                                printf("I-7565-H1/H2 Error : Error Code = %d\r\n",Ret);
                                        }
                                        else
                                        {
                                                printf("Reset I-7565-H1/H2 OK\r\n");
                                        }
      				}
				break;

				case 'h':
                                {
					char option;
					int port, id, mode, rtr, len, data;
					_VCI_CAN_MSG CANTx;


					printf("Use Default CAN Message (y/n):");
                                        scanf("%s",&option);
					getchar();

					memset(&CANTx, 0, sizeof(CANTx));

					if( option == 'n' )
					{
						if (vcparam_ex.vc.DevType == I7565H2)
                                        	{
                                                	printf("CAN Port : Press CAN Port number(1~2):");
							//port = CAN1 or CAN2
	                                                scanf("%d",&port);
                	                        }
                        	                else
                                	        {
							port = CAN1;	//CAN1 = 1
                                        	}

						printf("CAN Message ID :");
                        	                scanf("%x",&id);

						printf("CAN Message Mode :");
                                        	scanf("%d",&mode);

						printf("CAN Message RTR :");
        	                                scanf("%d",&rtr);

						printf("CAN Message Length :");
                        	                scanf("%d",&len);

						CANTx.Mode = mode;
                                        	CANTx.ID = id;
	                                        CANTx.RTR = rtr;
        	                                CANTx.DLC = len;

						for(i = 0; i < 8; i++)
							CANTx.Data[i] = 0;

						for(i = 0; i < CANTx.DLC; i++)
						{
							printf("CAN Message Data[%d] :",i);
	                                	        scanf("%x",&data);
							CANTx.Data[i] = data;
						}

        	                                getchar();

					}
					else  //option == y
					{
						if (vcparam_ex.vc.DevType == I7565H2)
                                        	{
                                                	printf("CAN Port : Press CAN Port number(1~2):");
							//port = CAN1 or CAN2
	                                                scanf("%d",&port);
        	                                        getchar();
                	                        }
                        	                else
                                	        {
                                        	        port = CAN1;	//CAN1 = 1
                                        	}

						CANTx.Mode = 0;
						CANTx.ID = 0x120;
						CANTx.RTR = 0;
						CANTx.DLC = 8;
						CANTx.Data[0] = 0x01;
						CANTx.Data[1] = 0x02;
						CANTx.Data[2] = 0x03;
						CANTx.Data[3] = 0x04;
						CANTx.Data[4] = 0x05;
						CANTx.Data[5] = 0x06;
						CANTx.Data[6] = 0x07;
						CANTx.Data[7] = 0x08;
					}


//					printf("CANTx.ID : 0x%x\n", CANTx.ID);

//					printf("CANTx.Mode : %d\n", CANTx.Mode);
//					printf("CANTx.RTR : %d\n", CANTx.RTR);
//					printf("CANTx.DLC : %d\n", CANTx.DLC);

//					for( i = 0; i < CANTx.DLC; i++)
//					{
//						printf("CANTx.Data[%d] : 0x%x\n",i ,CANTx.Data[i]);
//					}

					Ret = VCI_SendCANMsg(vcparam_ex.vc.DevPort, port, &CANTx);
					if(Ret)
                                       	{
						printf("I-7565-H1/H2 Error : Error Code = %d\r\n",Ret);
					}
                                       	else
                                       	{
						printf("Send CAN Message(Mode %d ID(Hex) %x RTR %d DLC %d D1 %x D2 %x D3 %x D4 %x D5 %x D6 %x D7 %x D8 %x) OK\r\n", CANTx.Mode, CANTx.ID, CANTx.RTR, CANTx.DLC, CANTx.Data[0], CANTx.Data[1], CANTx.Data[2], CANTx.Data[3], CANTx.Data[4], CANTx.Data[5], CANTx.Data[6], CANTx.Data[7]);
					}
				}
				break;

				case 'i':
                                {
					_VCI_CAN_MSG CANRx;
                                        int port, rx_count = 0, i=0;

					memset(&CANRx, 0, sizeof(CANRx));
					if (vcparam_ex.vc.DevType == I7565H2)
                                        {
                                                printf("CAN Port : Press CAN Port number(1~2):");
						//port = CAN1 or CAN2
                                                scanf("%d",&port);
                                                getchar();
                                        }
                                        else
                                        {
                                                port = CAN1;	//CAN1 = 1
                                                getchar();
                                        }
		
					Ret = VCI_Get_RxMsgCnt(vcparam_ex.vc.DevPort, port, &rx_count);

					if(Ret)
                                        {
                                                printf("I-7565-H1/H2 Error : Error Code = %d\r\n",Ret);
                                        }
					else
					{
						if ( rx_count > 0 )
						{
							for( i = 0; i < rx_count; i++)
							{
				        			Ret = VCI_RecvCANMsg(vcparam_ex.vc.DevPort, port, &CANRx);

			               	                        if(Ret)
                		        	                {
                        		        	                printf("I-7565-H1/H2 Error : Error Code = %d\r\n",Ret);
                                		        	}
                                        			else
                                        			{
                                                			printf("CAN %d Receive Message(Mode %d ID(Hex) %x RTR %d DLC %d D1 %x D2 %x D3 %x D4 %x D5 %x D6 %x D7 %x D8 %x) OK\r\n", port, CANRx.Mode, CANRx.ID, CANRx.RTR, CANRx.DLC, CANRx.Data[0], CANRx.Data[1], CANRx.Data[2], CANRx.Data[3], CANRx.Data[4], CANRx.Data[5], CANRx.Data[6], CANRx.Data[7]);
                                        			}

								memset(&CANRx,0,sizeof(CANRx));
							}
						}
						else
						{
							printf("CAN %d : No Data\n", port);
						}
					}
                                }
                                break;

				case 'j':
                                {
                                        char option;
                                        int port, id, mode, rtr, len, data, count, time;
                                        _VCI_CAN_MSG CANTx;

                                        printf("Use Default Config to Cyclic Send CAN Message (y/n):");
                                        scanf("%s",&option);
                                        getchar();

                                        memset(&CANTx, 0, sizeof(CANTx));

                                        if( option == 'n' )
                                        {
                                                if (vcparam_ex.vc.DevType == I7565H2)
                                                {
                                                        printf("CAN Port : Press CAN Port number(1~2):");
							//port = CAN1 or CAN2
                                                        scanf("%d",&port);
                                                }
                                                else
                                                {
                                                        port = CAN1;	//CAN1 = 1
                                                }

						printf("HW CAN Message Count :");
                                                scanf("%d",&count);

						printf("HW CAN Message Time Period(ms):");
						scanf("%d",&time);

                                                printf("CAN Message ID :");
                                                scanf("%x",&id);

                                                printf("CAN Message Mode :");
                                                scanf("%d",&mode);

                                                printf("CAN Message RTR :");
                                                scanf("%d",&rtr);

                                                printf("CAN Message Length :");
                                                scanf("%d",&len);

                                                CANTx.Mode = mode;
                                                CANTx.ID = id;
                                                CANTx.RTR = rtr;
                                                CANTx.DLC = len;

                                                for(i = 0; i < 8; i++)
                                                        CANTx.Data[i] = 0;

						for(i = 0; i < CANTx.DLC; i++)
                                                {
                                                        printf("CAN Message Data[%d] :",i);
                                                        scanf("%x",&data);
                                                        CANTx.Data[i] = data;
                                                }

                                                getchar();

                                        }
                                        else
                                        {
                                                if (vcparam_ex.vc.DevType == I7565H2)
                                                {
                                                        printf("CAN Port : Press CAN Port number(1~2):");
							//port = CAN1 or CAN2
                                                        scanf("%d",&port);
                                                        getchar();
                                                }
                                                else
                                                {
                                                        port = CAN1;	//CAN1 = 1
                                                }
						
						count = 10;
						time = 10;

						CANTx.Mode = 0;
                                                CANTx.ID = 0x120;
                                                CANTx.RTR = 0;
                                                CANTx.DLC = 8;
                                                CANTx.Data[0] = 0x01;
                                                CANTx.Data[1] = 0x02;
                                                CANTx.Data[2] = 0x03;
                                                CANTx.Data[3] = 0x04;
                                                CANTx.Data[4] = 0x05;
                                                CANTx.Data[5] = 0x06;
                                                CANTx.Data[6] = 0x07;
                                                CANTx.Data[7] = 0x08;
                                        }
                                        //Send 10 CANMsg and Stop from CAN1
                                        Ret = VCI_EnableHWCyclicTxMsg(vcparam_ex.vc.DevPort, port, &CANTx, time, count);

                                        if(Ret)
                                        {
                                                printf("I-7565-H1/H2 Error : Error Code = %d\r\n",Ret);
                                        }
                                        else
                                        {
                                                printf("Send CAN Message(Mode %d ID(Hex) %x RTR %d DLC %d D1 %x D2 %x D3 %x D4 %x D5 %x D6 %x D7 %x D8 %x) OK\r\n", CANTx.Mode, CANTx.ID, CANTx.RTR, CANTx.DLC, CANTx.Data[0], CANTx.Data[1], CANTx.Data[2], CANTx.Data[3], CANTx.Data[4], CANTx.Data[5], CANTx.Data[6], CANTx.Data[7]);
                                        }
				}
				break;

				case 'k':
				{
					getchar();
                                        Ret = VCI_DisableHWCyclicTxMsg(&vcparam_ex.vc);

                                        if(Ret)
                                                printf("I-7565-H1/H2 Error : Error Code = %d\r\n",Ret);
                                        else
                                                printf("Disable Hardware Cyclic Tx Message OK\r\n");
				}
				break;

				case 'l':
                                {
					int port, rx_count = 0;

					if (vcparam_ex.vc.DevType == I7565H2)
                                        {
                                                printf("CAN Port : Press CAN Port number(1~2):");
						//port = CAN1 or CAN2
                                                scanf("%d",&port);
                                                getchar();
                                        }
                                        else
                                        {
                                                port = CAN1;	//CAN1 = 1
                                                getchar();
                                        }

                                        Ret = VCI_Get_RxMsgCnt(vcparam_ex.vc.DevPort, port, &rx_count);

                                        if(Ret)
                                        {
						printf("I-7565-H1/H2 Error : Error Code = %d\r\n",Ret);
                                        }
                                        else
                                        {
						printf("CAN%d RxMsgCnt = %d\r\n", port, rx_count);
                                        }
                                }
                                break;

				case 'm':
                                {
                                        int port;
					BYTE buf_full;

                                        if (vcparam_ex.vc.DevType == I7565H2)
                                        {
                                                printf("CAN Port : Press CAN Port number(1~2):");
						//port = CAN1 or CAN2
                                                scanf("%d",&port);
                                                getchar();
                                        }
                                        else
                                        {
                                                port = CAN1;	//CAN1 = 1
                                                getchar();
                                        }

                                        Ret = VCI_Get_RxMsgBufIsFull(vcparam_ex.vc.DevPort, port, &buf_full);

                                        if(Ret)
                                        {
                                                printf("I-7565-H1/H2 Error : Error Code = %d\r\n",Ret);
                                        }
                                        else
                                        {
						if ( buf_full)
						{
                                                	printf("CAN Port %d : Rx Message Buffer is Full\r\n",port);
                                        	}
						else
						{
                                                	printf("CAN Port %d : Rx Message Buffer isn't full\r\n",port);
						}
					}
                                }
				break;

				case 'n':
                                {
                                        int port;

                                        if (vcparam_ex.vc.DevType == I7565H2)
                                        {
                                                printf("CAN Port : Press CAN Port number(1~2):");
						//port = CAN1 or CAN2
                                                scanf("%d",&port);
                                                getchar();
                                        }
                                        else
                                        {
                                                port = CAN1;	//CAN1 = 1
                                                getchar();
                                        }

                                        Ret = VCI_Clr_RxMsgBuf(vcparam_ex.vc.DevPort, port);

                                        if(Ret)
                                        {
                                                printf("I-7565-H1/H2 Error : Error Code = %d\r\n",Ret);
                                        }
                                        else
                                        {
						printf("CAN Port %d : Clear Rx Message Buffer OK\r\n",port);
                                        }
				}
                                break;

				case 'o':
                                {
                                        int port, tmp;
					DWORD TxSentCnt;

					printf("Get Tx sent count press 0\n");
					printf("Clear Tx sent count press 1\n");
					scanf("%d",&tmp);

                                        if (vcparam_ex.vc.DevType == I7565H2)
                                        {
                                                printf("CAN Port : Press CAN Port number(1~2):");
                                                //port = CAN1 or CAN2
                                                scanf("%d",&port);
                                                getchar();
                                        }
                                        else
                                        {
                                                port = CAN1;    //CAN1 = 1
                                                getchar();
                                        }
					
					if(tmp == 0)
					{
						Ret = VCI_Get_TxSentCnt(vcparam_ex.vc.DevPort, port, &TxSentCnt);
						printf("TxSentCnt %d\n",TxSentCnt);

						if(Ret)
                                                	printf("I-7565-H1/H2 Error : Error Code = %d\r\n",Ret);
					}
					else
					{
						Ret = VCI_Clr_TxSentCnt(vcparam_ex.vc.DevPort, port);

						if(Ret)
                                 	               printf("I-7565-H1/H2 Error : Error Code = %d\r\n",Ret);
                                        	else
                                        	        printf("CAN Port %d : Clear Tx sent count OK\r\n",port);
					}

                                }
                                break;

				case 'p':
				{
					getchar();

					printf("a. Get I-7565-H1/H2 CAN State:\r\n");
					printf("b. Get I-7565-H1/H2 Module Information:\r\n");
					printf("c. Set I-7565-H1/H2 CAN Filter ID:\r\n");
					printf("d. Get I-7565-H1/H2 CAN Filter ID:\r\n");
					printf("e. Clear I-7565-H1/H2 CAN Filter ID:\r\n");
					printf("f. Clear I-7565-H1/H2 Overflow LED:\r\n");
					printf("g. Reset I-7565-H1/H2 Module:\r\n");
					printf("h. Send CAN Message:\r\n");
					printf("i. Receive CAN Message:\r\n");
					printf("j. Enable Hardware Cyclic Send CAN Message:\r\n");
					printf("k. Disable Hardware Cyclic Send CAN Message:\r\n");
					printf("l. Get I-7565-H1/H2 Rx Message Count:\r\n");
					printf("m. Get I-7565-H1/H2 Rx Message State:\r\n");    
					printf("n. Clear I-7565-H1/H2 Rx Message Buffer:\r\n");
					printf("o. Get or Clear I-7565-H1/H2 Tx sent count:\r\n");
					printf("p. Show All I-7565-H1/H2function:\r\n");
					printf("q. Shutdown and exit:\r\n");
					printf("r. Get I-7565-H1/H2 Tseg1, Tseg2, SJWval value:\r\n");
				}
				break;

				case 'q':
				{

					getchar();
					Ret = VCI_CloseCAN(&vcparam_ex.vc);
				
					if(Ret)
					{
						printf("ShutdownMaster Error Code = %d\r\n",Ret);
					}
					else
					{
						printf("ShutdownMaster OK\n");
						return 0;
					}

				}
				break;

				case 'r':
                                {
                                        int port;
                                        BYTE T1Val = 0, T2Val = 0, SJWVal = 0;


                                        if(vcparam_ex.vc.DevType == I7565H2)
                                        {
                                                printf("CAN Port : Press CAN Port number(1~2):");
                                                //port = CAN1 or CAN2
                                                scanf("%d",&port);
                                                getchar();
                                        }
                                        else
                                        {
                                                port = CAN1;    //CAN1 = 1
                                                getchar();
                                        }

                                        VCI_Get_CANBaud_BitTime(vcparam_ex.vc.DevPort, port, &T1Val, &T2Val, &SJWVal);
                                        printf("T1Val %d T2Val %d SJWVal %d\n",T1Val, T2Val, SJWVal);
                                }
                                break;
				default:
				        printf("Unknow command = %X\r\n",command);
			        break;
			}
		}
	}  //end while
}
