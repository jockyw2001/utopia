//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010	-	2012 MStar Semiconductor,	Inc. All rights	reserved.
// All software, firmware	and	related	documentation	herein ("MStar Software")	are
// intellectual	property of	MStar	Semiconductor, Inc.	("MStar")	and	protected	by
// law,	including, but not limited to, copyright law and international treaties.
// Any use,	modification,	reproduction,	retransmission,	or republication of	all
// or	part of	MStar	Software is	expressly	prohibited,	unless prior written
// permission	has	been granted by	MStar.
//
// By	accessing, browsing	and/or using MStar Software, you acknowledge that	you
// have	read,	understood,	and	agree, to	be bound by	below	terms	("Terms")	and	to
// comply	with all applicable	laws and regulations:
//
// 1.	MStar	shall	retain any and all right,	ownership	and	interest to	MStar
//		Software and any modification/derivatives	thereof.
//		No right,	ownership, or	interest to	MStar	Software and any
//		modification/derivatives thereof is	transferred	to you under Terms.
//
// 2.	You	understand that	MStar	Software might include,	incorporate	or be
//		supplied together	with third party`s software	and	the	use	of MStar
//		Software may require additional	licenses from	third	parties.
//		Therefore, you hereby	agree	it is	your sole	responsibility to	separately
//		obtain any and all third party right and license necessary for your	use	of
//		such third party`s software.
//
// 3.	MStar	Software and any modification/derivatives	thereof	shall	be deemed	as
//		MStar`s	confidential information and you agree to	keep MStar`s
//		confidential information in	strictest	confidence and not disclose	to any
//		third	party.
//
// 4.	MStar	Software is	provided on	an "AS IS" basis without warranties	of any
//		kind.	Any	warranties are hereby	expressly	disclaimed by	MStar, including
//		without	limitation,	any	warranties of	merchantability, non-infringement	of
//		intellectual property	rights,	fitness	for	a	particular purpose,	error	free
//		and	in conformity	with any international standard.	You	agree	to waive any
//		claim	against	MStar	for	any	loss,	damage,	cost or	expense	that you may
//		incur	related	to your	use	of MStar Software.
//		In no	event	shall	MStar	be liable	for	any	direct,	indirect,	incidental or
//		consequential	damages, including without limitation, lost	of profit	or
//		revenues,	lost or	damage of	data,	and	unauthorized system	use.
//		You	agree	that this	Section	4	shall	still	apply	without	being	affected
//		even if	MStar	Software has been	modified by	MStar	in accordance	with your
//		request	or instruction for your	use, except	otherwise	agreed by	both
//		parties	in writing.
//
// 5.	If requested,	MStar	may	from time	to time	provide	technical	supports or
//		services in	relation with	MStar	Software to	you	for	your use of
//		MStar	Software in	conjunction	with your	or your	customer`s product
//		("Services").
//		You	understand and agree that, except	otherwise	agreed by	both parties in
//		writing, Services	are	provided on	an "AS IS" basis and the warranty
//		disclaimer set forth in	Section	4	above	shall	apply.
//
// 6.	Nothing	contained	herein shall be	construed	as by	implication, estoppels
//		or otherwise:
//		(a)	conferring any license or	right	to use MStar name, trademark,	service
//				mark,	symbol or	any	other	identification;
//		(b)	obligating MStar or	any	of its affiliates	to furnish any person,
//				including	without	limitation,	you	and	your customers,	any	assistance
//				of any kind	whatsoever,	or any information;	or
//		(c)	conferring any license or	right	under	any	intellectual property	right.
//
// 7.	These	terms	shall	be governed	by and construed in	accordance with	the	laws
//		of Taiwan, R.O.C., excluding its conflict	of law rules.
//		Any	and	all	dispute	arising	out	hereof or	related	hereto shall be	finally
//		settled	by arbitration referred	to the Chinese Arbitration Association,
//		Taipei in	accordance with	the	ROC	Arbitration	Law	and	the	Arbitration
//		Rules	of the Association by	three	(3)	arbitrators	appointed	in accordance
//		with the said	Rules.
//		The	place	of arbitration shall be	in Taipei, Taiwan	and	the	language shall
//		be English.
//		The	arbitration	award	shall	be final and binding to	both parties.
//
//******************************************************************************
//<MStar Software>
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor,	Inc.
// All rights	reserved.
//
// Unless	otherwise	stipulated in	writing, any and all information contained
// herein	regardless in	any	format shall remain	the	sole proprietary of
// MStar Semiconductor Inc.	and	be kept	in strict	confidence
// ("MStar Confidential	Information")	by the recipient.
// Any unauthorized	act	including	without	limitation unauthorized	disclosure,
// copying,	use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of	the	contents of	MStar	Confidential
// Information is	unlawful and strictly	prohibited.	MStar	hereby reserves	the
// rights	to any and all damages,	losses,	costs	and	expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
///	file		drvAVD.c
///	@brief	AVD	Driver Interface
///	@author	MStar	Semiconductor	Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//	Include	Files
//-------------------------------------------------------------------------------------------------
// Common	Definition
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#include <stdio.h>
#include <math.h>
#endif
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsOS.h"

// Internal	Definition
//#include "regCHIP.h"
//#include "regAVD.h"
//#include "mapi_tuner.h"
#include "drvSYS.h"
#include "drvDMD_VD_MBX.h"
#include "drvDMD_INTERN_DVBT.h"
#include "drvDMD_INTERN_DVBT_v2.h"

#include "halDMD_INTERN_DVBT.h"
#include "halDMD_INTERN_common.h"
#include "drvSAR.h"	 //	for	Utopia2
#include "utopia.h"

#include "utopia_dapi.h"

#include "drvSAR.h"	 //	for	Utopia2
#include "ULog.h"
//-------------------------------------------------------------------------------------------------
//	Driver Compiler	Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//	Local	Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//	Local	Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//	Global Variables
//-------------------------------------------------------------------------------------------------
#define	DMD_LOCK()			\
		do{													\
				MS_ASSERT(MsOS_In_Interrupt()	== FALSE); \
				if (_u8DMDDbgLevel ==	DMD_DBGLV_DEBUG) ULOGD("DEMOD","%s lock	mutex\n",	__FUNCTION__);\
				MsOS_ObtainMutex(_s32DMD_DVBT_Mutex, MSOS_WAIT_FOREVER);\
				}while(0)

#define	DMD_UNLOCK()			\
		do{													\
				MsOS_ReleaseMutex(_s32DMD_DVBT_Mutex);\
				if (_u8DMDDbgLevel ==	DMD_DBGLV_DEBUG) ULOGD("DEMOD","%s unlock	mutex\n",	__FUNCTION__); \
				}while(0)

//MS_U8		 DVBT_TS_PHASE_EN	=0;
//MS_U8		 DVBT_TS_PHASE_NUM = 0;
//-------------------------------------------------------------------------------------------------
//	Local	Variables
//-------------------------------------------------------------------------------------------------
#if	1
/*static MSIF_Version	_drv_dmd_dvbt_intern_version = {
		.MW	=	{	DMD_DVBT_INTERN_VER, },
};*/
#else
static MSIF_Version	_drv_dmd_dvbt_intern_version;
#endif

//static DMD_DVBT_InitData _sDMD_DVBT_InitData;
//static DMD_DbgLv _u8DMDDbgLevel=DMD_DBGLV_NONE;
//static MS_S32	_s32DMD_DVBT_Mutex=-1;
//static DMD_DVBT_Info sDMD_DVBT_Info;
//static MS_U16	u16DMD_DVBT_TPS_Timeout	=	1500,	u16DMD_DVBT_FEC_Timeout=6000;
static MS_U32	u32DMD_DVBT_IfFrequency	=	36167L,	u32DMD_DVBT_FsFrequency	=	45474L;
static MS_U8 u8DMD_DVBT_IQSwap=0;
//static DMD_RF_CHANNEL_BANDWIDTH	eDMD_DVBT_BandWidth=E_DMD_RF_CH_BAND_8MHz;
static DMD_SQI_CN_NORDIGP1 SqiCnNordigP1[] =
{
		{_QPSK,	 _CR1Y2, 5.1 },
		{_QPSK,	 _CR2Y3, 6.9 },
		{_QPSK,	 _CR3Y4, 7.9 },
		{_QPSK,	 _CR5Y6, 8.9 },
		{_QPSK,	 _CR7Y8, 9.7 },
		{_16QAM, _CR1Y2, 10.8},
		{_16QAM, _CR2Y3, 13.1},
		{_16QAM, _CR3Y4, 14.6},
		{_16QAM, _CR5Y6, 15.6},
		{_16QAM, _CR7Y8, 16.0},
		{_64QAM, _CR1Y2, 16.5},
		{_64QAM, _CR2Y3, 18.7},
		{_64QAM, _CR3Y4, 20.2},
		{_64QAM, _CR5Y6, 21.6},
		{_64QAM, _CR7Y8, 22.5},
};

static DMD_SSI_DBM_NORDIGP1	dvbt_ssi_dbm_nordigp1[]	=
{
	{	_QPSK	,	_CR1Y2,	-93},
	{	_QPSK	,	_CR2Y3,	-91},
	{	_QPSK	,	_CR3Y4,	-90},
	{	_QPSK	,	_CR5Y6,	-89},
	{	_QPSK	,	_CR7Y8,	-88},

	{	_16QAM , _CR1Y2, -87},
	{	_16QAM , _CR2Y3, -85},
	{	_16QAM , _CR3Y4, -84},
	{	_16QAM , _CR5Y6, -83},
	{	_16QAM , _CR7Y8, -82},

	{	_64QAM , _CR1Y2, -82},
	{	_64QAM , _CR2Y3, -80},
	{	_64QAM , _CR3Y4, -78},
	{	_64QAM , _CR5Y6, -77},
	{	_64QAM , _CR7Y8, -76},
	{	_UNKNOW_QAM	,	_UNKNOW_CR,	0.0},
};

//-------------------------------------------------------------------------------------------------
//	Debug	Functions
//-------------------------------------------------------------------------------------------------
#ifdef MS_DEBUG
#define	DMD_DBG(x)					(x)
#else
#define	DMD_DBG(x)					//(x)
#endif

static DMD_DVBT_InitData	 AgcSsi_Para;
//-------------------------------------------------------------------------------------------------
//	Local	Functions
//-------------------------------------------------------------------------------------------------

static		void*	ppDVBTInstant	=	NULL;
static MS_U32	u32DVBTopen	=	0;
		static MS_U8 u8DVBTUtopiaOpen	=	0;	 //for SetStillImagePara is	earlier	called than	Init


static float fViterbiBerFiltered=-1;
//-------------------------------------------------------------------------------------------------
//	Global Functions
//-------------------------------------------------------------------------------------------------
#ifndef MSOS_TYPE_LINUX
#if 1
static float _LogApproxTableX[80] =
{ 1.00, 1.30, 1.69, 2.20, 2.86, 3.71, 4.83, 6.27, 8.16, 10.60, 13.79,
  17.92, 23.30, 30.29, 39.37, 51.19, 66.54, 86.50, 112.46, 146.19,
  190.05, 247.06, 321.18, 417.54, 542.80, 705.64, 917.33, 1192.53,
  1550.29, 2015.38, 2620.00, 3405.99, 4427.79, 5756.13, 7482.97,
  9727.86, 12646.22, 16440.08, 21372.11, 27783.74, 36118.86,
  46954.52, 61040.88, 79353.15, 103159.09, 134106.82, 174338.86,
  226640.52, 294632.68, 383022.48, 497929.22, 647307.99, 841500.39, 1093950.50,
  1422135.65, 1848776.35, 2403409.25, 3124432.03, 4061761.64, 5280290.13,
  6864377.17, 8923690.32, 11600797.42, 15081036.65, 19605347.64, 25486951.94,
  33133037.52, 43072948.77, 55994833.40, 72793283.42, 94631268.45,
  123020648.99, 159926843.68, 207904896.79, 270276365.82, 351359275.57,
  456767058.24, 593797175.72, 771936328.43, 1003517226.96
};

static float _LogApproxTableY[80] =
{ 0.00, 0.11, 0.23, 0.34, 0.46, 0.57, 0.68, 0.80, 0.91, 1.03, 1.14, 1.25,
  1.37, 1.48, 1.60, 1.71, 1.82, 1.94, 2.05, 2.16, 2.28, 2.39, 2.51, 2.62,
  2.73, 2.85, 2.96, 3.08, 3.19, 3.30, 3.42, 3.53, 3.65, 3.76, 3.87, 3.99,
  4.10, 4.22, 4.33, 4.44, 4.56, 4.67, 4.79, 4.90, 5.01, 5.13, 5.24, 5.36,
  5.47, 5.58, 5.70, 5.81, 5.93, 6.04, 6.15, 6.27, 6.04, 6.15, 6.27, 6.38,
  6.49, 6.61, 6.72, 6.84, 6.95, 7.06, 7.18, 7.29, 7.41, 7.52, 7.63, 7.75,
  7.86, 7.98, 8.09, 8.20, 8.32, 8.43, 8.55, 8.66
};

static float Log10Approx(float flt_x)
{
    MS_U8  indx = 0;

    do {
        if (flt_x < _LogApproxTableX[indx])
            break;
        indx++;
    }while (indx < 79);   //stop at indx = 80

    return _LogApproxTableY[indx];
}
#else
static float Log10Approx(float flt_x)
{
    MS_U32       u32_temp = 1;
    MS_U8        indx = 0;

    do {
        u32_temp = u32_temp << 1;
        if (flt_x < (float)u32_temp)
            break;
    }while (++indx < 32);

    // 10*log10(X) ~= 0.3*N, when X ~= 2^N
    return (float)0.3 * indx;
}
#endif
#endif


//bryan	waiting	for	handling
MS_BOOL	MDrv_DMD_DVBT_GetReg(MS_U16	u16Addr, MS_U8 *pu8Data)
{
		MS_BOOL	bRet;

		DVBT_GETREG_PARAM	Drv_DVBT_GETREG_PARAM;
		Drv_DVBT_GETREG_PARAM.u16Addr=u16Addr;
		Drv_DVBT_GETREG_PARAM.pu8Data=pu8Data;

		DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT.c]MDrv_DMD_DVBT_GetReg\n"));

		 bRet=UtopiaIoctl(ppDVBTInstant,DMD_DVBT_DRV_CMD_GetReg,&Drv_DVBT_GETREG_PARAM);
		 return	bRet;
/*
		DMD_LOCK();
		bRet=MDrv_SYS_DMD_VD_MBX_ReadReg(u16Addr,	pu8Data);
		DMD_UNLOCK();

		#ifdef MS_DEBUG
		if (_u8DMDDbgLevel >=	DMD_DBGLV_DEBUG)
		{
				ULOGD("DEMOD","MDrv_DMD_DVBT_GetReg	%x %x\n",	u16Addr, *pu8Data);
		}
		#endif

		return bRet;
*/
}

MS_BOOL	MDrv_DMD_DVBT_SetReg(MS_U16	u16Addr, MS_U8 u8Data)
{
		MS_BOOL	return_val;

	 DVBT_SETREG_PARAM Drv_DVBT_SETREG_PARAM;
	 Drv_DVBT_SETREG_PARAM.u16Addr=u16Addr;
	 Drv_DVBT_SETREG_PARAM.u8Data=u8Data;



		DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT.c]MDrv_DMD_DVBT_SetReg\n"));
		return_val=UtopiaIoctl(ppDVBTInstant,DMD_DVBT_DRV_CMD_SetReg,&Drv_DVBT_SETREG_PARAM);
	return return_val;
/*
		#ifdef MS_DEBUG
		if (_u8DMDDbgLevel >=	DMD_DBGLV_DEBUG)
		{
				ULOGD("DEMOD","MDrv_DMD_DVBT_SetReg	%x %x\n",	u16Addr, u8Data);
		}
		#endif

		DMD_LOCK();
		bRet=MDrv_SYS_DMD_VD_MBX_WriteReg(u16Addr, u8Data);
		DMD_UNLOCK();
		return bRet;
		*/
}



/*bryan	this is	need to	modified*/
MS_BOOL	MDrv_DMD_DVBT_Init(DMD_DVBT_InitData *pDMD_DVBT_InitData,	MS_U32 u32InitDataLen)
{
	 //	MS_BOOL	return_val;


		void*	pAttribte	=	NULL;

		DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT.c]MDrv_DMD_DVBT_Init\n"));

		//bryan	test
		#if(0)
		 DVBT_INIT_PARAM Drv_DVBT_INIT_PARAM;

	 Drv_DVBT_INIT_PARAM.DMD_DVBT_InitData.u8DMD_DVBT_DSPRegInitExt=pDMD_DVBT_InitData->u8DMD_DVBT_DSPRegInitExt;
	 Drv_DVBT_INIT_PARAM.DMD_DVBT_InitData.u8DMD_DVBT_DSPRegInitSize=pDMD_DVBT_InitData->u8DMD_DVBT_DSPRegInitSize;
	 Drv_DVBT_INIT_PARAM.DMD_DVBT_InitData.u8DMD_DVBT_InitExt=pDMD_DVBT_InitData->u8DMD_DVBT_InitExt;
	 Drv_DVBT_INIT_PARAM.DMD_DVBT_InitData.u8SarChannel=pDMD_DVBT_InitData->u8SarChannel;
		Drv_DVBT_INIT_PARAM.u32InitDataLen=sizeof(Drv_DVBT_INIT_PARAM.DMD_DVBT_InitData);
	 #else
				DVBT_INIT_PARAM	Drv_DVBT_INIT_PARAM;
				DMD_DVBT_InitData_Transform	Init_Para_Temp;
	Drv_DVBT_INIT_PARAM.ret=false;

memcpy (&(AgcSsi_Para), pDMD_DVBT_InitData, sizeof(DMD_DVBT_InitData));

	Init_Para_Temp.u8DMD_DVBT_DSPRegInitExt=pDMD_DVBT_InitData->u8DMD_DVBT_DSPRegInitExt;
	 Init_Para_Temp.u8DMD_DVBT_DSPRegInitSize=pDMD_DVBT_InitData->u8DMD_DVBT_DSPRegInitSize;
	 Init_Para_Temp.u8DMD_DVBT_InitExt=pDMD_DVBT_InitData->u8DMD_DVBT_InitExt;
	 Init_Para_Temp.u8SarChannel=pDMD_DVBT_InitData->u8SarChannel;

	 Drv_DVBT_INIT_PARAM.DMD_DVBT_InitData=&Init_Para_Temp;
		Drv_DVBT_INIT_PARAM.u32InitDataLen=sizeof(Init_Para_Temp);
	 #endif

		//Drv_DVBT_INIT_PARAM.pDMD_DVBT_InitData=pDMD_DVBT_InitData;

		if(u8DVBTUtopiaOpen	== 0)	 //	First	time open
		{
//				if(UtopiaOpen(MODULE_DVBT	|KERNEL_MODE,	&ppDVBTInstant,	0, pAttribte)	== UTOPIA_STATUS_SUCCESS)
				if(UtopiaOpen(MODULE_DVBT/*|KERNEL_MODE*/	,	&ppDVBTInstant,	0, pAttribte)	== UTOPIA_STATUS_SUCCESS)	 //kernel	space
//if(UtopiaOpen(MODULE_DVBT	,	&ppDVBTInstant,	0, pAttribte)	== UTOPIA_STATUS_SUCCESS)	 //user	space
				{
						u32DVBTopen	=	1;
			//return_val=true;
						//ULOGD("DEMOD","\r\n	======== DVBT	Open Successful	%x =========", (WORD)u32DVBTopen);
				}
				else
				{
					 //	ULOGD("DEMOD","\r\n	======== DVBT	Open Fail	%x =========", (WORD)u32DVBTopen);
			 //return_val=false;

		 return	false;
				}

				u8DVBTUtopiaOpen = 1;
		}


	UtopiaIoctl(ppDVBTInstant,DMD_DVBT_DRV_CMD_Init,&Drv_DVBT_INIT_PARAM);
	return Drv_DVBT_INIT_PARAM.ret;
 #if(0)
		char pDMD_DVBT_MutexString[16];
		MS_U8	u8ADCIQMode	=	0, u8PadSel	=	0, bPGAEnable	=	0, u8PGAGain = 5;
		MS_BOOL	bRFAGCTristateEnable = 1;
		MS_BOOL	bIFAGCTristateEnable = 0;

		if (_s32DMD_DVBT_Mutex !=	-1)
		{
				DMD_DBG(ULOGD("DEMOD","MDrv_DMD_DVBT_Init	more than	once\n"));
				return FALSE;
		}

		if (NULL ==	strncpy(pDMD_DVBT_MutexString,"Mutex DMD DVBT",16))
		{
				DMD_DBG(ULOGD("DEMOD","MDrv_DMD_DVBT_Init	strcpy Fail\n"));
				return FALSE;
		}
		_s32DMD_DVBT_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, pDMD_DVBT_MutexString,	MSOS_PROCESS_SHARED);
		if (_s32DMD_DVBT_Mutex ==	-1)
		{
				DMD_DBG(ULOGD("DEMOD","MDrv_DMD_DVBT_Init	Create Mutex Fail\n"));
				return FALSE;
		}
		//_u8DMDDbgLevel = DMD_DBGLV_DEBUG;
		#ifdef MS_DEBUG
		if (_u8DMDDbgLevel >=	DMD_DBGLV_INFO)
		{
				ULOGD("DEMOD","MDrv_DMD_DVBT_Init\n");
		}
		#endif

		if ( sizeof(_sDMD_DVBT_InitData) ==	u32InitDataLen)
		{
				memcpy(&_sDMD_DVBT_InitData, pDMD_DVBT_InitData, u32InitDataLen);
		}
		else
		{
				DMD_DBG(ULOGD("DEMOD","MDrv_DMD_DVBT_Init	input	data structure incorrect\n"));
				return FALSE;
		}

		if (_sDMD_DVBT_InitData.u8SarChannel !=	0xFF)
		{
				MDrv_SAR_Adc_Config(_sDMD_DVBT_InitData.u8SarChannel,	TRUE);
		}

		DMD_LOCK();
		MDrv_SYS_DMD_VD_MBX_SetType(E_DMD_VD_MBX_TYPE_DVBT);
		HAL_DMD_RegInit();

		if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt !=	NULL)
		{
				if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[0]>=2)
				{
						bRFAGCTristateEnable = (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[3]	&	(BIT_(0))) ? TRUE	:	FALSE; //	RFAGC	tristate control
						bIFAGCTristateEnable = (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[3]	&	(BIT_(4))) ? TRUE	:	FALSE; //	IFAGC	tristate control
				}
				else
				{
						bRFAGCTristateEnable = 1;
						bIFAGCTristateEnable = 0;
				}
		}
		else
		{
				bRFAGCTristateEnable = 1;
				bIFAGCTristateEnable = 0;
		}

		if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt !=	NULL)
		{
				if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[0]>=3)
				{
						u32DMD_DVBT_IfFrequency	=	_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[4]; //	IF frequency
						u32DMD_DVBT_IfFrequency	=	 (u32DMD_DVBT_IfFrequency<<8)+_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[5]; //	IF frequency
						u32DMD_DVBT_IfFrequency	=	 (u32DMD_DVBT_IfFrequency<<8)+_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[6]; //	IF frequency
						u32DMD_DVBT_IfFrequency	=	 (u32DMD_DVBT_IfFrequency<<8)+_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[7]; //	IF frequency
						u32DMD_DVBT_FsFrequency	=	_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[8]; //	FS frequency
						u32DMD_DVBT_FsFrequency	=	 (u32DMD_DVBT_FsFrequency<<8)+_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[9]; //	FS frequency
						u32DMD_DVBT_FsFrequency	=	 (u32DMD_DVBT_FsFrequency<<8)+_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[10];	// FS	frequency
						u32DMD_DVBT_FsFrequency	=	 (u32DMD_DVBT_FsFrequency<<8)+_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[11];	// FS	frequency
						u8DMD_DVBT_IQSwap	=	_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[12];	// IQ	Swap

						u8ADCIQMode	=	_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[13];	// u8ADCIQMode : 0=I path, 1=Q path, 2=both	IQ
						u8PadSel = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[14]; //	u8PadSel : 0=Normal, 1=analog	pad
						bPGAEnable = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[15]; //	bPGAEnable : 0=disable,	1=enable
						u8PGAGain	=	_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[16];	// u8PGAGain : default 5
				}
				else
				{

				}
		}
		else
		{

		}
		#ifdef MS_DEBUG
		ULOGD("DEMOD","u32DMD_DVBT_IfFrequency %ld\n",u32DMD_DVBT_IfFrequency);
		ULOGD("DEMOD","u32DMD_DVBT_FsFrequency %ld\n",u32DMD_DVBT_FsFrequency);
		ULOGD("DEMOD","u8DMD_DVBT_IQSwap %d\n",u8DMD_DVBT_IQSwap);
		#endif

		u16DMD_DVBT_TPS_Timeout	=	1500;
		u16DMD_DVBT_FEC_Timeout	=	6000;
		if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt !=	NULL)
		{
				if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[0]>=4)
				{
						u16DMD_DVBT_TPS_Timeout	=	_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[17];	// TPS timeout in	ms
						u16DMD_DVBT_TPS_Timeout	=	 (u16DMD_DVBT_TPS_Timeout<<8)+_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[18];
						if (u16DMD_DVBT_TPS_Timeout	<	700) u16DMD_DVBT_TPS_Timeout=700;
						//ULOGD("DEMOD","u16DMD_DVBT_TPS_Timeout %d\n",u16DMD_DVBT_TPS_Timeout);

						u16DMD_DVBT_FEC_Timeout	=	_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[19];	// FEC timeout in	ms
						u16DMD_DVBT_FEC_Timeout	=	 (u16DMD_DVBT_FEC_Timeout<<8)+_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[20];
						if (u16DMD_DVBT_FEC_Timeout	<	2500)	u16DMD_DVBT_FEC_Timeout=2500;
						//ULOGD("DEMOD","u16DMD_DVBT_FEC_Timeout %d\n",u16DMD_DVBT_FEC_Timeout);
				}
				else
				{
				}
		}
		else
		{
		}

		if (bIFAGCTristateEnable)
		{
				MDrv_SYS_SetAGCPadMux(E_SYS_DTV_AGC_PAD_SET_ALL_OFF);
		}
		else
		{
				MDrv_SYS_SetAGCPadMux(E_SYS_DTV_AGC_PAD_SET);
		}


	// oga
	DVBT_TS_PHASE_EN =0;
	DVBT_TS_PHASE_NUM	=	0;
	 if	(_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt	!= NULL)
	 {
/*
		if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[0]>=5)	// version bigger	than 5,	apply	TS phase solution
		{
		 DVBT_TS_PHASE_EN	=	_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[INDEX_T_TS_PHASE_EN];
		 DVBT_TS_PHASE_NUM = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[INDEX_T_TS_PHASE_NUM];
		 ULOGD("DEMOD","##DVBT:TS	check: bTsPhaseEn	=	%d,	u16TsPhaseNum	=	%d\n",DVBT_TS_PHASE_EN,DVBT_TS_PHASE_NUM);
		}
	else
	{
		ULOGD("DEMOD","##DVBT:TS Phase check !!, board version smaller than	4\n");
	}
*/
 }
	 else	// if	init board define	is NULL	TS phase needs check.
	 {
		ULOGD("DEMOD","##DVBT:TS Phase check !!\n");
	 }



		if (_sDMD_DVBT_InitData.u8DMD_DVBT_DSPRegInitExt !=	NULL)
		{
				if (_sDMD_DVBT_InitData.u8DMD_DVBT_DSPRegInitExt[0]>=1)
				{
						INTERN_DVBT_Power_On_Initialization(bRFAGCTristateEnable,	u8ADCIQMode, u8PadSel, bPGAEnable, u8PGAGain,	_sDMD_DVBT_InitData.u8DMD_DVBT_DSPRegInitExt,	_sDMD_DVBT_InitData.u8DMD_DVBT_DSPRegInitSize);
				}
				else
				{
						ULOGD("DEMOD","u8DMD_DVBT_DSPRegInitExt	Error\n");
				}
		}
		else
		{
				INTERN_DVBT_Power_On_Initialization(bRFAGCTristateEnable,	u8ADCIQMode, u8PadSel, bPGAEnable, u8PGAGain,	 NULL, 0);
		}

		INTERN_DVBT_Version(&sDMD_DVBT_Info.u16Version);
		DMD_UNLOCK();
		#ifdef MS_DEBUG
		ULOGD("DEMOD","firmware	version: %x\n",sDMD_DVBT_Info.u16Version);
		#endif
		return TRUE;
#endif
}

MS_BOOL	MDrv_DMD_DVBT_Exit(void)
{
//bryan:return value need	to be	checked
	//MS_BOOL	return_val;
	/*
		#ifdef MS_DEBUG
		if (_u8DMDDbgLevel >=	DMD_DBGLV_DEBUG)
		{
				ULOGD("DEMOD","MDrv_DMD_DVBT_Exit\n");
		}
		#endif

		DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT.c]MDrv_DMD_DVBT_Exit\n"));

		DMD_LOCK();
		INTERN_DVBT_Exit();
		DMD_UNLOCK();
		MsOS_DeleteMutex(_s32DMD_DVBT_Mutex);
		_s32DMD_DVBT_Mutex=	-1;
		return TRUE;
		*/
		DVBT_EXIT_PARAM	Drv_DVBT_EXIT_PARAM;
		Drv_DVBT_EXIT_PARAM.ret=false;
		DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT.c]MDrv_DMD_DVBT_Exit\n"));

		if(u32DVBTopen==1)
			UtopiaIoctl(ppDVBTInstant, DMD_DVBT_DRV_CMD_Exit,	&Drv_DVBT_EXIT_PARAM);
		else
	return false;

	return Drv_DVBT_EXIT_PARAM.ret;
}

MS_U32 MDrv_DMD_DVBT_GetConfig(DMD_DVBT_InitData *pDMD_DVBT_InitData)  
{
	if (!u32DVBTopen) return FALSE;
	
       memcpy (pDMD_DVBT_InitData, &(AgcSsi_Para), sizeof(DMD_DVBT_InitData));	
    
       return UTOPIA_STATUS_SUCCESS;
}

MS_BOOL	MDrv_DMD_DVBT_SetDbgLevel(DMD_DbgLv	u8DbgLevel)
{
	//MS_BOOL	return_val;
	DVBT_SETDBG_LEVEL_PARAM	Drv_DVBT_SETDBG_LEVEL_PARAM;
	Drv_DVBT_SETDBG_LEVEL_PARAM.u8DbgLevel=u8DbgLevel;
	Drv_DVBT_SETDBG_LEVEL_PARAM.ret=false;


		/*
		DMD_LOCK();
		_u8DMDDbgLevel = u8DbgLevel;
		DMD_UNLOCK();
		return TRUE;
		*/
		DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT.c]MDrv_DMD_DVBT_SetDbgLevel\n"));

		 if(u32DVBTopen==1)
			UtopiaIoctl(ppDVBTInstant, DMD_DVBT_DRV_CMD_SetDbgLeve,	&Drv_DVBT_SETDBG_LEVEL_PARAM);
		 else
	return false;

	return Drv_DVBT_SETDBG_LEVEL_PARAM.ret;
}

DMD_DVBT_Info* MDrv_DMD_DVBT_GetInfo(DMD_DVBT_INFO_TYPE	eInfoType)
{
	//MS_BOOL	return_val;
	DVBT_GETINFO_PARAM Drv_DVBT_GETINFO_PARAM;
	Drv_DVBT_GETINFO_PARAM.eInfoType=eInfoType;
	Drv_DVBT_GETINFO_PARAM.pInfo=NULL;

			 DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT.c]MDrv_DMD_DVBT_GetInfo\n"));

			if(u32DVBTopen==1)
			 UtopiaIoctl(ppDVBTInstant,	DMD_DVBT_DRV_CMD_GetInfo,	&Drv_DVBT_GETINFO_PARAM);


	return Drv_DVBT_GETINFO_PARAM.pInfo;
		#if(0)
		DMD_LOCK();
		switch (eInfoType)
		{
				case E_DMD_DVBT_MODULATION_INFO:
						INTERN_DVBT_Show_Modulation_info();
						break;
				case E_DMD_DVBT_DEMOD_INFO:
						INTERN_DVBT_Show_Demod_Info();
						break;
				case E_DMD_DVBT_LOCK_INFO:
						INTERN_DVBT_Show_Lock_Info();
						break;
				case E_DMD_DVBT_PRESFO_INFO:
						INTERN_DVBT_Show_PRESFO_Info();
						break;
				case E_DMD_DVBT_LOCK_TIME_INFO:
						INTERN_DVBT_Show_Lock_Time_Info();
						break;
				case E_DMD_DVBT_BER_INFO:
						INTERN_DVBT_Show_BER_Info();
						break;
				case E_DMD_DVBT_AGC_INFO:
						INTERN_DVBT_Show_AGC_Info();
						break;
				default:
						#ifdef MS_DEBUG
						ULOGD("DEMOD","MDrv_DMD_DVBT_GetInfo %d	Error\n",	eInfoType);
						#endif
						break;
		}
		DMD_UNLOCK();
		return &sDMD_DVBT_Info;

		#endif
}

MS_BOOL	MDrv_DMD_DVBT_GetLibVer(const	MSIF_Version **ppVersion)
{
	DVBT_GETLIBVER_PARAM Drv_DVBT_GETLIBVER_PARAM;
	Drv_DVBT_GETLIBVER_PARAM.ppVersion=ppVersion;
	Drv_DVBT_GETLIBVER_PARAM.ret=false;

			 DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT.c]MDrv_DMD_DVBT_GetLibVer\n"));
	if(u32DVBTopen==1)
		UtopiaIoctl(ppDVBTInstant, DMD_DVBT_DRV_CMD_GetLibVer, &Drv_DVBT_GETLIBVER_PARAM);
	else
		return false;

	return Drv_DVBT_GETLIBVER_PARAM.ret;


		/*
		DMD_LOCK();
		if (!ppVersion)
		{
				return FALSE;
		}

		*ppVersion = &_drv_dmd_dvbt_intern_version;
		DMD_UNLOCK();
		return TRUE;
		*/
}

MS_BOOL	MDrv_DMD_DVBTGetFWVer(MS_U16 *ver)
{
		//MS_BOOL	return_val;
	 DVBT_GETFWVER_PARAM Drv_DVBT_GETFWVER_PARAM;
	 Drv_DVBT_GETFWVER_PARAM.ver=ver;
	 Drv_DVBT_GETFWVER_PARAM.ret=false;

	 DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT.c]MDrv_DMD_DVBTGetFWVer\n"));
	 if(u32DVBTopen==1)
		UtopiaIoctl(ppDVBTInstant, DMD_DVBT_DRV_CMD_GetFWVer,	&Drv_DVBT_GETFWVER_PARAM);
	 else
		return false;

	 return	Drv_DVBT_GETFWVER_PARAM.ret;
		/*
		MS_BOOL	bRet;

		DMD_LOCK();

		bRet = INTERN_DVBT_Version(ver);
		//ULOGD("DEMOD","MDrv_DMD_DVBT_GetFWVer	%x\n",*ver);
		DMD_UNLOCK();

		return bRet;
		*/
}


MS_BOOL	MDrv_DMD_DVBT_SetSerialControl(MS_BOOL bEnable)
{
		//MS_BOOL	return_val;
		DVBT_SetSerialControl_PARAM	Drv_DVBT_SetSerialControl_PARAM;
		Drv_DVBT_SetSerialControl_PARAM.bEnable=bEnable;
		Drv_DVBT_SetSerialControl_PARAM.ret=false;

	 DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT.c]MDrv_DMD_DVBT_SetSerialControl\n"));

	 if(u32DVBTopen==1)
		 UtopiaIoctl(ppDVBTInstant,	DMD_DVBT_DRV_CMD_SetSerialControl, &Drv_DVBT_SetSerialControl_PARAM);
	 else
				return false;

	 return	Drv_DVBT_SetSerialControl_PARAM.ret;
	 /*
		MS_BOOL	bRet;
		MS_U8	u8TSClk;

		#ifdef MS_DEBUG
		if (_u8DMDDbgLevel >=	DMD_DBGLV_DEBUG)
		{
				ULOGD("DEMOD","MDrv_DMD_DVBT_SetSerialControl	%x\n", bEnable);
		}
		#endif

		DMD_LOCK();
		if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt !=	NULL)
		{
				if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[0]>=1)
				{
						u8TSClk	=	_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[2]; //	TS_CLK
				}
				else
				{
						u8TSClk	=	0xFF;	// parallel	mode:	0x0513 =>	ts_clk=288/(2*(0x16+1))=6.26MHz	//@@++--
				}
		}
		else
		{
				u8TSClk	=	0xFF;	// parallel	mode:	0x0513 =>	ts_clk=288/(2*(0x16+1))=6.26MHz	//@@++--
		}
		bRet=INTERN_DVBT_Serial_Control(bEnable, u8TSClk);
		DMD_UNLOCK();
		return bRet;
		*/
}

MS_BOOL	MDrv_DMD_DVBT_SetConfig(DMD_RF_CHANNEL_BANDWIDTH BW, MS_BOOL bSerialTS,	MS_BOOL	bPalBG)
{
		DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT.c]MDrv_DMD_DVBT_SetConfig\n"));

		return MDrv_DMD_DVBT_SetConfigHPLPSetIF(BW,	bSerialTS, bPalBG, 0,	u32DMD_DVBT_IfFrequency, u32DMD_DVBT_FsFrequency,	u8DMD_DVBT_IQSwap);
}


MS_BOOL	MDrv_DMD_DVBT_SetConfigHPLP(DMD_RF_CHANNEL_BANDWIDTH BW, MS_BOOL bSerialTS,	MS_BOOL	bPalBG,	MS_BOOL	bLPSel)
{
		DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT.c]MDrv_DMD_DVBT_SetConfigHPLP\n"));

		return MDrv_DMD_DVBT_SetConfigHPLPSetIF(BW,	bSerialTS, bPalBG, bLPSel, u32DMD_DVBT_IfFrequency,	u32DMD_DVBT_FsFrequency, u8DMD_DVBT_IQSwap);
}


MS_BOOL	MDrv_DMD_DVBT_SetConfigHPLPSetIF(DMD_RF_CHANNEL_BANDWIDTH	BW,	MS_BOOL	bSerialTS, MS_BOOL bPalBG, MS_BOOL bLPSel, MS_U32	u32IFFreq, MS_U32	u32FSFreq, MS_U8 u8IQSwap)
{
	//MS_BOOL	return_val;
	DVBT_SetConfigHPLPSetIF_PARAM	Drv_DVBT_SetConfigHPLPSetIF_PARAM;
	Drv_DVBT_SetConfigHPLPSetIF_PARAM.BW=BW;
			Drv_DVBT_SetConfigHPLPSetIF_PARAM.bSerialTS=bSerialTS;
			Drv_DVBT_SetConfigHPLPSetIF_PARAM.bPalBG=bPalBG;
			Drv_DVBT_SetConfigHPLPSetIF_PARAM.bLPSel=bLPSel;
			Drv_DVBT_SetConfigHPLPSetIF_PARAM.u32IFFreq=u32IFFreq;
			Drv_DVBT_SetConfigHPLPSetIF_PARAM.u32FSFreq=u32FSFreq;
			Drv_DVBT_SetConfigHPLPSetIF_PARAM.u8IQSwap=u8IQSwap;
			Drv_DVBT_SetConfigHPLPSetIF_PARAM.ret=false;

			DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT.c]MDrv_DMD_DVBT_SetConfigHPLPSetIF\n"));

	if(u32DVBTopen==1)
		UtopiaIoctl(ppDVBTInstant, DMD_DVBT_DRV_CMD_SetConfigHPLPSetIF,	&Drv_DVBT_SetConfigHPLPSetIF_PARAM);
	else
		return false;

				return Drv_DVBT_SetConfigHPLPSetIF_PARAM.ret;

/*
		MS_BOOL	bRet;
		MS_U8	u8TSClk;

		DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT.c]MDrv_DMD_DVBT_SetConfigHPLPSetIF\n"));

		#ifdef MS_DEBUG
		if (_u8DMDDbgLevel >=	DMD_DBGLV_DEBUG)
		{
				ULOGD("DEMOD","MDrv_DMD_DVBT_SetConfigHPLPSetIF	%d %d	%d %d	%ld	%ld	%d\n", BW, bSerialTS,	bPalBG,	bLPSel,	u32IFFreq, u32FSFreq,	u8IQSwap);
		}
		#endif

		DMD_LOCK();
		if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt !=	NULL)
		{
				if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[0]>=1)
				{
						u8TSClk	=	_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[2]; //	TS_CLK
				}
				else
				{
						u8TSClk	=	0xFF;	// parallel	mode:	0x0513 =>	ts_clk=288/(2*(0x16+1))=6.26MHz	//@@++--
				}
		}
		else
		{
				u8TSClk	=	0xFF;	// parallel	mode:	0x0513 =>	ts_clk=288/(2*(0x16+1))=6.26MHz	//@@++--
		}

		bRet=INTERN_DVBT_Config(BW,	bSerialTS, bPalBG, bLPSel, u8TSClk,	u32IFFreq, u32FSFreq,	u8IQSwap);
		eDMD_DVBT_BandWidth=BW;
		DMD_UNLOCK();
		return bRet;

		*/
}

MS_BOOL	MDrv_DMD_DVBT_SetActive(MS_BOOL	bEnable)
{
	//MS_BOOL	return_val;
	DVBT_SetActive_PARAM Drv_DVBT_SetActive_PARAM;
	Drv_DVBT_SetActive_PARAM.bEnable=bEnable;
	Drv_DVBT_SetActive_PARAM.ret=false;

			DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT.c]MDrv_DMD_DVBT_SetActive\n"));
	if(u32DVBTopen==1)
		UtopiaIoctl(ppDVBTInstant, DMD_DVBT_DRV_CMD_SetActive, &Drv_DVBT_SetActive_PARAM);
	else
		return false;

				return Drv_DVBT_SetActive_PARAM.ret;
/*
		MS_BOOL	bRet;

		DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT.c]MDrv_DMD_DVBT_SetActive\n"));

		#ifdef MS_DEBUG
		if (_u8DMDDbgLevel >=	DMD_DBGLV_DEBUG)
		{
				ULOGD("DEMOD","MDrv_DMD_DVBT_SetActive %d\n",	bEnable);
		}
		#endif

		DMD_LOCK();
		bRet=INTERN_DVBT_Active(bEnable);
		DMD_UNLOCK();
		return bRet;
 */

}

MS_BOOL	MDrv_DMD_DVBT_GetLock(DMD_DVBT_GETLOCK_TYPE	eType, DMD_LOCK_STATUS *eLockStatus)
{
	//MS_BOOL	return_val;
	DVBT_GetLock_PARAM Drv_DVBT_GetLock_PARAM;
	Drv_DVBT_GetLock_PARAM.eType=eType;
	Drv_DVBT_GetLock_PARAM.eLockStatus=eLockStatus;
	Drv_DVBT_GetLock_PARAM.ret=false;
			 DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT.c]MDrv_DMD_DVBT_GetLock\n"));

	if(u32DVBTopen==1)
		UtopiaIoctl(ppDVBTInstant, DMD_DVBT_DRV_CMD_GetLock, &Drv_DVBT_GetLock_PARAM);
	else
		return false;

				return Drv_DVBT_GetLock_PARAM.ret;
}

#ifndef	MSOS_TYPE_LINUX_KERNEL
MS_BOOL	MDrv_DMD_DVBT_GetSignalStrength(MS_U16 *u16Strength)
{
		DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT.c]MDrv_DMD_DVBT_GetSignalStrength\n"));

		return MDrv_DMD_DVBT_GetSignalStrengthWithRFPower(u16Strength, 200.0f);
}
#endif

#ifndef	MSOS_TYPE_LINUX_KERNEL
MS_BOOL	MDrv_DMD_DVBT_GetSignalStrengthWithRFPower(MS_U16	*u16Strength,	float	fRFPowerDbm)
{

	MS_BOOL	status = true;
	DMD_IFAGC_SSI		*ifagc_ssi;
	DMD_IFAGC_ERR		*ifagc_err;
	float		ch_power_rf=0.0f;
	float		ch_power_db=0.0f,	ch_power_db_rel=0.0f;
	float		ch_power_if=0.0f,	ch_power_ifa = 0.0f, ch_power_ifb	=0.0f;
	float		ch_power_ref = 11.0f;
	MS_U16	if_agc_val =0, if_agc_vala =0, if_agc_valb =0, if_agc_val_lsb	=0,	i;
	float		ch_power_takeover=0.0f;
	MS_U8		ssi_tbl_len	=	0, err_tbl_len = 0;

	MS_U8	ifagc_reg;
	MS_U8	ifagc_reg_lsb;
	MS_U16 ifagc_err_reg;
	MS_U8		u8_index = 0;
	MS_U16	tps_info_qam = 0,tps_info_cr = 0;

	//DVBT_GetSignalStrengthWithRFPower_PARAM	Drv_DVBT_GetSignalStrengthWithRFPower_PARAM;
	//Drv_DVBT_GetSignalStrengthWithRFPower_PARAM.u16Strength=u16Strength;
	//Drv_DVBT_GetSignalStrengthWithRFPower_PARAM.fRFPowerDbm=fRFPowerDbm;

	DVBT_GetIFAGC_PARAM	Drv_DVBT_GetIFAGC_PARAM;
	Drv_DVBT_GetIFAGC_PARAM.ifagc_reg=&ifagc_reg;
	Drv_DVBT_GetIFAGC_PARAM.ifagc_reg_lsb=&ifagc_reg_lsb;
	Drv_DVBT_GetIFAGC_PARAM.ifagc_err_reg=&ifagc_err_reg;
	Drv_DVBT_GetIFAGC_PARAM.ret=false;

	if(u32DVBTopen==1)
	{
		if((AgcSsi_Para.pTuner_IfagcSsi_HiRef	!= NULL) &&	(AgcSsi_Para.pTuner_IfagcSsi_LoRef !=	NULL))
		{
				UtopiaIoctl(ppDVBTInstant,DMD_DVBT_DRV_CMD_GetIFAGC,&Drv_DVBT_GetIFAGC_PARAM);
				status &=	Drv_DVBT_GetIFAGC_PARAM.ret;
				ch_power_rf	=	fRFPowerDbm;

				if_agc_val = ifagc_reg;
				if_agc_val_lsb = ifagc_reg_lsb;

				ifagc_ssi	=	AgcSsi_Para.pTuner_IfagcSsi_LoRef;
				ssi_tbl_len	=	AgcSsi_Para.u16Tuner_IfagcSsi_LoRef_Size;
				ifagc_err	=	AgcSsi_Para.pTuner_IfagcErr_LoRef;
				err_tbl_len	=	AgcSsi_Para.u16Tuner_IfagcErr_LoRef_Size;

				ch_power_if=ifagc_ssi[0].power_db;
				if (if_agc_val >=ifagc_ssi[0].agc_val)
				{
								for(i	=	1; i < ssi_tbl_len;	i++)
								{
										if (if_agc_val < ifagc_ssi[i].agc_val)
										{
												if_agc_valb	=	ifagc_ssi[i].agc_val;
												ch_power_ifb = ifagc_ssi[i].power_db;

												i--;
												if_agc_vala	=	ifagc_ssi[i].agc_val;
												ch_power_ifa=ifagc_ssi[i].power_db;
												while	((i>1) &&	(if_agc_vala==ifagc_ssi[i-1].agc_val))
												{
														ch_power_ifa=ifagc_ssi[i-1].power_db;
														i--;
												}
												ch_power_if	=	ch_power_ifa+(ch_power_ifb-ch_power_ifa)*(float)((if_agc_val-if_agc_vala)*256+if_agc_val_lsb)/((if_agc_valb-if_agc_vala)*256);
												break;
										}
								}
				}
						#ifdef MS_DEBUG
						ULOGD("DEMOD","if	prev %f	%x\n", ch_power_ifa, if_agc_vala);
						ULOGD("DEMOD","if	next %f	%x\n", ch_power_ifb, if_agc_valb);
						#endif

						for(i	=	0; i < ssi_tbl_len;	i++)
						{
								if (ifagc_ssi[i].agc_val <=	ifagc_ssi[i+1].agc_val)
								{
										ch_power_takeover	=	ifagc_ssi[i+1].power_db;
										break;
								}
						}

						#ifdef MS_DEBUG
						ULOGD("DEMOD","ch_power_rf = %f\n",	ch_power_rf);
						ULOGD("DEMOD","ch_power_if = %f\n",	ch_power_if);
						ULOGD("DEMOD","ch_power_takeover = %f\n",	ch_power_takeover);
						#endif

						// ch_power_db = (ch_power_rf	>	ch_power_if)?	ch_power_rf	:	ch_power_if;

						if(ch_power_rf > (ch_power_takeover	+	0.5))
						{
								ch_power_db	=	ch_power_rf;
						}
						else if(ch_power_if	<	(ch_power_takeover - 0.5))
						{
								ch_power_db	=	ch_power_if;
						}
						else
						{
								ch_power_db	=	(ch_power_if + ch_power_rf)/2;
						}

						// ch_power_db = (ch_power_rf	>	ch_power_if)?	ch_power_if	:	ch_power_rf;

				if(if_agc_val	== 0xff)
				{
						for(i	=	0; i < err_tbl_len;	i++)
						{
										if ( ifagc_err_reg <=	ifagc_err[i].agc_err )				// signed	char comparison
										{
												ch_power_db	+= ifagc_err[i].attn_db;
												break;
										}
						}
						#ifdef MS_DEBUG
						ULOGD("DEMOD","if_agc_err	=	0x%x\n", ifagc_err_reg);
					 #endif
				}
		}
		else
		{
				#ifdef MS_DEBUG
				if (fRFPowerDbm>=100.0)	// unreasonable	input	value, get RF	level	from RFAGG
				{
						ULOGD("DEMOD","Error!! please	add	AGC	table\n");
				}
				#endif
				ch_power_db	=	fRFPowerDbm;	// from	tuner
		}


		 if(INTERN_DVBT_Get_TPS_Parameter_Const(&tps_info_qam, TS_MODUL_MODE)	== FALSE)
		 printf("[dvbt]TPS qam parameter retrieve	failure\n");

		 if(INTERN_DVBT_Get_TPS_Parameter_Const(&tps_info_cr,	TS_CODE_RATE)	== FALSE)
		 printf("[dvbt]TPS cr	parameter	retrieve failure\n");

		 while(dvbt_ssi_dbm_nordigp1[u8_index].constel !=	_UNKNOW_QAM)
		{
				if ( (dvbt_ssi_dbm_nordigp1[u8_index].constel	== (DMD_CONSTEL)tps_info_qam)
						&& (dvbt_ssi_dbm_nordigp1[u8_index].code_rate	== (DMD_CODERATE)tps_info_cr))
				{
					 ch_power_ref	=	dvbt_ssi_dbm_nordigp1[u8_index].p_ref;
					 break;
				}
				else
				{
					 u8_index++;
				}
		}

		if (ch_power_ref > 10.0f)
		*u16Strength = 0;
		else
		{
				ch_power_db_rel	=	ch_power_db	-	ch_power_ref;

				if ( ch_power_db_rel < -15.0f	)
				{
						*u16Strength = 0;
				}
				else if	(	ch_power_db_rel	<	0.0f )
				{
						*u16Strength = (MS_U16)(2.0f/3*(ch_power_db_rel	+	15.0f));
				}
				else if	(	ch_power_db_rel	<	20 )
				{
						*u16Strength = (MS_U16)(4.0f*ch_power_db_rel + 10.0f);
				}
				else if	(	ch_power_db_rel	<	35.0f	)
				{
						*u16Strength = (MS_U16)(2.0f/3*(ch_power_db_rel	-	20.0f) + 90.0f);
				}
				else
				{
						*u16Strength = 100;
				}
		}

		DMD_DBG(ULOGD("DEMOD",">>> SSI_CH_PWR(dB)	=	%f , Score = %d<<<\n", ch_power_db,	*u16Strength));
		DMD_DBG(ULOGD("DEMOD",">>> SSI = %d	<<<\n",	(int)*u16Strength));

		return status;
	}
	else
	{
		return false;
	}

	return status;

}
#endif

#ifndef	MSOS_TYPE_LINUX_KERNEL
MS_BOOL	MDrv_DMD_DVBT_GetSignalQuality(MS_U16	*u16Quality)
{
		DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT.c]MDrv_DMD_DVBT_GetSignalQuality\n"));

		return MDrv_DMD_DVBT_GetSignalQualityWithRFPower(u16Quality, 200.0f);
}
#endif

#ifndef	MSOS_TYPE_LINUX_KERNEL
MS_BOOL	MDrv_DMD_DVBT_GetSignalQualityWithRFPower(MS_U16 *u16Quality,	float	fRFPowerDbm)
{
	float		ber_sqi;
	float		fber;
	float		cn_rec = 0;
	float		cn_nordig_p1 = 0;
	float		cn_rel = 0;

	MS_U8		status = true;
	MS_U8		tps_cnstl	=	0, tps_cr	=	0, i = 0;
	MS_U16	u16_tmp;
	DMD_LOCK_STATUS	eLockStatus = E_DMD_CHECKING;

	if(u32DVBTopen==1)
	{
		MDrv_DMD_DVBT_GetLock(E_DMD_COFDM_FEC_LOCK,	&eLockStatus);
		if(E_DMD_LOCK	== eLockStatus)
		{
#if	0
			if ( MsOS_Timer_DiffTimeFromNow(u32FecFirstLockTime) < 300)
	{
		MsOS_DelayTask(300 - MsOS_Timer_DiffTimeFromNow(u32FecFirstLockTime));
	}
#endif
				/////////	Get	Pre-RS (Post-Viterbi)	BER	to determine BER_SQI //////////
				MDrv_DMD_DVBT_GetPostViterbiBer(&fViterbiBerFiltered);
				if(fViterbiBerFiltered<= 0.0)
				{
						if (MDrv_DMD_DVBT_GetPostViterbiBer(&fber) ==	FALSE)
						{
								DMD_DBG(ULOGD("DEMOD","MDrv_DMD_DVBT_GetPostViterbiBer \n	"));
								return FALSE;
						}
						fViterbiBerFiltered	=	fber;
				}
				else
				{
						fber = fViterbiBerFiltered;
				}

				if (fber > 1.0E-3)
						ber_sqi	=	0.0;
				else if	(fber	>	8.5E-7)
#ifdef MSOS_TYPE_LINUX
						ber_sqi	=	(log10f(1.0f/fber))*20.0f	-	22.0f;
#else
						ber_sqi	=	(Log10Approx(1.0f/fber))*20.0f - 22.0f;
#endif
				else
						ber_sqi	=	100.0;

				MDrv_DMD_DVBT_GetSNR(&cn_rec);

				if (cn_rec ==	-1)		//get	SNR	return fail
						status = false;

#if	0	// temp	mark
				/////////	Get	Constellation	and	Code Rate	to determine Ref.	C/N	//////////
				/////////	(refer to	Teracom	min. spec	2.0	4.1.1.7) /////
				tps_cnstl	=	0xff;
				tps_cr = 0xff;
				if(INTERN_DVBT_Get_TPS_Parameter_Const(	&u16_tmp,	TS_MODUL_MODE) ==	TRUE)
						tps_cnstl	=	(MS_U8)u16_tmp&0x07;
				if(INTERN_DVBT_Get_TPS_Parameter_Const(	&u16_tmp,	TS_CODE_RATE)	== TRUE)
						tps_cr = (MS_U8)u16_tmp&0x07;

				for(i	=	0; i < sDMD_DVBT_InitData->u16SqiCnNordigP1_Size;	i++)
				{
						if ( (tps_cnstl	== sDMD_DVBT_InitData->pSqiCnNordigP1[i].constel)
						&& (tps_cr ==	sDMD_DVBT_InitData->pSqiCnNordigP1[i].code_rate) )
						{
								cn_nordig_p1 = sDMD_DVBT_InitData->pSqiCnNordigP1[i].cn_ref;
								break;
						}
				}

				// 0,5,	snr	offset
				cn_rel = cn_rec	-	cn_nordig_p1 + 0.5f;

				// patch....
				// Noridg	SQI,
				// 64QAM,	CR34,	GI14,	SNR	22dB.
				if ( (tps_cnstl	== _64QAM) &&	(tps_cr	== _CR3Y4)
						&& (cn_rel < 2.5f) &&	(cn_rel	>	1.5f))
				{
						cn_rel +=	1.5f;
				}

				if (cn_rel < -7.0f)
				{
						*quality = 0;
				}
				else if	(cn_rel	<	3.0)
						*quality = (MS_U16)(ber_sqi*((cn_rel - 3.0)/10.0 + 1.0));
				else
						*quality = (MS_U16)ber_sqi;
#else
				tps_cnstl	=	0xff;
				tps_cr = 0xff;
				if(INTERN_DVBT_Get_TPS_Parameter_Const(	&u16_tmp,	TS_MODUL_MODE) ==	TRUE)
						tps_cnstl	=	(MS_U8)u16_tmp&0x07;
				if(INTERN_DVBT_Get_TPS_Parameter_Const(	&u16_tmp,	TS_CODE_RATE)	== TRUE)
						tps_cr = (MS_U8)u16_tmp&0x07;

				for(i	=	0; i < (sizeof(SqiCnNordigP1)	/	sizeof(DMD_SQI_CN_NORDIGP1));	i++)
				{
						if ( (tps_cnstl	== SqiCnNordigP1[i].constel)
						&& (tps_cr ==	SqiCnNordigP1[i].code_rate)	)
						{
								cn_nordig_p1 = SqiCnNordigP1[i].cn_ref;
								break;
						}
				}

				cn_rel = cn_rec	-	cn_nordig_p1 + 0.5f;

				// patch....
				// Noridg	SQI,
				// 64QAM,	CR34,	GI14,	SNR	22dB.
				if ( (tps_cnstl	== _64QAM) &&	(tps_cr	== _CR3Y4)
						&& (cn_rel < 2.5f) &&	(cn_rel	>	1.5f))
				{
						cn_rel +=	1.5f;
				}

				if (cn_rel < -7.0f)
				{
						*u16Quality	=	0;
				}
				else if	(cn_rel	<	3.0)
						*u16Quality	=	(MS_U16)(ber_sqi*((cn_rel	-	3.0)/10.0	+	1.0));
				else
						*u16Quality	=	(MS_U16)ber_sqi;

#endif
		}
		else
		{
				*u16Quality	=	0;
		}

		DMD_DBG(ULOGD("DEMOD","BER = %8.3e\n", fber));
		DMD_DBG(ULOGD("DEMOD","Signal	Quility	=	%d\n", *u16Quality));

		return status;
	}

	return status;
}
#endif


#ifndef	MSOS_TYPE_LINUX_KERNEL
MS_BOOL	MDrv_DMD_DVBT_GetSNR(float *fSNR)
{

	MS_U32 noise_power = 0;

	DVBT_GetSNR_PARAM	Drv_DVBT_GetSNR_PARAM;
	Drv_DVBT_GetSNR_PARAM.noise_power_reg	=	&noise_power;
	Drv_DVBT_GetSNR_PARAM.ret	=	false;

			DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT.c]MDrv_DMD_DVBT_GetSNR\n"));

	if(u32DVBTopen==1)
		UtopiaIoctl(ppDVBTInstant, DMD_DVBT_DRV_CMD_GetSNR,	&Drv_DVBT_GetSNR_PARAM);
	else
		return false;

// caluate snr by	noise	power.

			noise_power	=	noise_power/2;
			noise_power	/=1280;

			if (noise_power==0)//protect value 0
				noise_power=1;

#ifdef MSOS_TYPE_LINUX
						*fSNR	=	10*log10f((float)noise_power);
#else
						*fSNR	=	10*Log10Approx((float)noise_power);
#endif

		return Drv_DVBT_GetSNR_PARAM.ret;
}
#endif

#ifndef	MSOS_TYPE_LINUX_KERNEL
MS_BOOL	MDrv_DMD_DVBT_GetPostViterbiBer(float	*ber)
{
			 //DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT.c]MDrv_DMD_DVBT_GetPostViterbiBer\n"));

		//MS_BOOL						 status	=	true;
		//MS_U8							reg=0, reg_frz=0;
		MS_U16						BitErrPeriod;
		MS_U32						BitErr;
		MS_U16						PktErr;

		DVBT_GetPostViterbiBer_PARAM Drv_DVBT_GetPostViterbiBer_PARAM;
		Drv_DVBT_GetPostViterbiBer_PARAM.BitErr_reg	=	 &BitErr;
		Drv_DVBT_GetPostViterbiBer_PARAM.BitErrPeriod_reg	=	&BitErrPeriod;
		Drv_DVBT_GetPostViterbiBer_PARAM.PktErr_reg= &PktErr;
		Drv_DVBT_GetPostViterbiBer_PARAM.ret=false;

	if(u32DVBTopen==1)
	{
		UtopiaIoctl(ppDVBTInstant, DMD_DVBT_DRV_CMD_GetPostViterbiBer, &Drv_DVBT_GetPostViterbiBer_PARAM);

			///////////	Post-Viterbi BER /////////////
#if	0
		if ( MsOS_Timer_DiffTimeFromNow(u32FecFirstLockTime) < 300)
		{
				*ber = (float)-1.0;
				return false;
		}
#endif

		if (BitErrPeriod ==	0	)		 //protect 0
				BitErrPeriod = 1;

		if (BitErr <=0 )
				*ber = 0.5f	/	((float)BitErrPeriod*128*188*8);
		else
				*ber = (float)BitErr / ((float)BitErrPeriod*128*188*8);

		//DBG_GET_SIGNAL(ULOGD("DEMOD","INTERN_DVBT	PostVitBER = %8.3e \n	", *ber));
		//DBG_GET_SIGNAL(ULOGD("DEMOD","INTERN_DVBT	PktErr = %d	\n ",	(int)PktErr));

		return	Drv_DVBT_GetPostViterbiBer_PARAM.ret;

	}
	else
	{
		return false;
	}
}
#endif

#ifndef	MSOS_TYPE_LINUX_KERNEL
MS_BOOL	MDrv_DMD_DVBT_GetPreViterbiBer(float *ber)
{
//	 MS_BOOL return_val;
	 /*bryan temp	mark*/
	 DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT.c]MDrv_DMD_DVBT_GetPreViterbiBer\n"));

	 #if(0)
	 DVBT_GetPreViterbiBer_PARAM Drv_DVBT_GetPreViterbiBer_PARAM;
	 Drv_DVBT_GetPreViterbiBer_PARAM.ber=ber;
	 #endif

	 //bryan temp	mark
	 #if(0)
	 if(u32DVBTopen==1)
		UtopiaIoctl(ppDVBTInstant, DMD_DVBT_DRV_CMD_GetPreViterbiBer,	&Drv_DVBT_GetPreViterbiBer_PARAM);
	 else
		return false;

		 return	Drv_DVBT_GetPreViterbiBer_PARAM.ret;
	 #else
		*ber=0;
		return true;
	 #endif


/*
		MS_BOOL	bRet;

		DMD_LOCK();
		bRet=INTERN_DVBT_GetPreViterbiBer(ber);
		DMD_UNLOCK();

		return bRet;
*/
}
#endif


MS_BOOL	MDrv_DMD_DVBT_GetPacketErr(MS_U16	*pktErr)
{
 //	 MS_BOOL return_val;
	 DVBT_GetPacketErr_PARAM Drv_DVBT_GetPacketErr_PARAM;
	 Drv_DVBT_GetPacketErr_PARAM.pktErr=pktErr;
	 Drv_DVBT_GetPacketErr_PARAM.ret=false;

	 DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT.c]MDrv_DMD_DVBT_GetPacketErr\n"));
		if(u32DVBTopen==1)
		UtopiaIoctl(ppDVBTInstant, DMD_DVBT_DRV_CMD_GetPacketErr,	&Drv_DVBT_GetPacketErr_PARAM);
		else
	return false;

	 return	Drv_DVBT_GetPacketErr_PARAM.ret;

/*
		MS_BOOL	bRet;
		float		fBER;

		DMD_LOCK();
		INTERN_DVBT_GetPostViterbiBer(&fBER);
		bRet=INTERN_DVBT_GetPacketErr(pktErr);
		if ((*pktErr ==1)	&& (fBER<= 0.000001))	// for no	signal case, from	Oga
		{
				*pktErr	=	0x3FF;
		}
		#ifdef MS_DEBUG
		if (_u8DMDDbgLevel >=	DMD_DBGLV_DEBUG)
		{
				ULOGD("DEMOD","MDrv_DMD_DVBT_GetPacketErr	%d\n", *pktErr);
		}
		#endif
		DMD_UNLOCK();

		return bRet;
*/
}

MS_BOOL	MDrv_DMD_DVBT_GetTPSInfo(MS_U16	*u16Info)
{
	 //MS_BOOL return_val;
	 DVBT_GetTPSInfo_PARAM Drv_DVBT_GetTPSInfo_PARAM;
	 Drv_DVBT_GetTPSInfo_PARAM.u16Info=u16Info;
	 Drv_DVBT_GetTPSInfo_PARAM.ret=false;

	 DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT.c]MDrv_DMD_DVBT_GetTPSInfo\n"));
	 if(u32DVBTopen==1)
		UtopiaIoctl(ppDVBTInstant, DMD_DVBT_DRV_CMD_GetTPSInfo,	&Drv_DVBT_GetTPSInfo_PARAM);
	 else
		return false;

	 return	Drv_DVBT_GetTPSInfo_PARAM.ret;
/*
		MS_BOOL	bRet;

		DMD_LOCK();
		bRet=INTERN_DVBT_Get_TPS_Info(u16Info);
		DMD_UNLOCK();

		return bRet;
*/
}

MS_BOOL	MDrv_DMD_DVBT_GetCellID(MS_U16 *u16CellID)
{
 //	 MS_BOOL return_val;

	 DVBT_GetCellID_PARAM	Drv_DVBT_GetCellID_PARAM;
	 Drv_DVBT_GetCellID_PARAM.u16CellID=u16CellID;
	 Drv_DVBT_GetCellID_PARAM.ret=false;

	 DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT.c]MDrv_DMD_DVBT_GetCellID\n"));
	 if(u32DVBTopen==1)
		 UtopiaIoctl(ppDVBTInstant,	DMD_DVBT_DRV_CMD_GetCellID,	&Drv_DVBT_GetCellID_PARAM);
	 else
				return false;

	 return	Drv_DVBT_GetCellID_PARAM.ret;

/*
		MS_BOOL	bRet;

		DMD_LOCK();
		bRet=INTERN_DVBT_Get_CELL_ID(u16CellID);
		DMD_UNLOCK();
		#ifdef MS_DEBUG
		if (_u8DMDDbgLevel >=	DMD_DBGLV_DEBUG)
		{
				ULOGD("DEMOD","MDrv_DMD_DVBT_GetCellID %d\n",	*u16CellID);
		}
		#endif
		return bRet;
*/
}

#ifndef	MSOS_TYPE_LINUX_KERNEL
MS_BOOL	MDrv_DMD_DVBT_GetFreqOffset(float	*pFreqOff)
{
	// MS_BOOL return_val;
	/*bryam	temp mark*/
	 DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT.c]MDrv_DMD_DVBT_GetFreqOffset\n"));
	#if(0)
	 DVBT_GetFreqOffset_PARAM	Drv_DVBT_GetFreqOffset_PARAM;
	 Drv_DVBT_GetFreqOffset_PARAM.pFreqOff=pFreqOff;
		#endif

		/*bryan	temp mark*/
	 #if(0)
	if(u32DVBTopen==1)
		 UtopiaIoctl(ppDVBTInstant,	DMD_DVBT_DRV_CMD_GetFreqOffset,	&Drv_DVBT_GetFreqOffset_PARAM);
	else
		return false;

		 return	Drv_DVBT_GetFreqOffset_PARAM.ret;
	#else
	//Drv_DVBT_GetFreqOffset_PARAM.ret=true;
	return true;
	#endif




/*
		MS_BOOL	bRet=TRUE;
		MS_U8	u8BW=8;

		DMD_LOCK();
		switch (eDMD_DVBT_BandWidth)
		{
				case E_DMD_RF_CH_BAND_6MHz:
						u8BW=6;
						break;

				case E_DMD_RF_CH_BAND_7MHz:
						u8BW=7;
						break;

				case E_DMD_RF_CH_BAND_8MHz:
				default:
						u8BW=8;
						break;
		}
		bRet=INTERN_DVBT_Get_FreqOffset(pFreqOff,	u8BW);
		DMD_UNLOCK();

		#ifdef MS_DEBUG
		if (_u8DMDDbgLevel >=	DMD_DBGLV_DEBUG)
		{
				ULOGD("DEMOD","MDrv_DMD_DVBT_GetStatus %d	%f\n", u8BW, *pFreqOff);
		}
		#endif
		return bRet;
*/
}
#endif


#ifndef	MSOS_TYPE_LINUX_KERNEL
MS_BOOL	MDrv_DMD_DVBT_NORDIG_SSI_Table_Write(DMD_CONSTEL constel,	DMD_CODERATE code_rate,	float	write_value)
{
//	 MS_BOOL return_val;
	 DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT.c]MDrv_DMD_DVBT_NORDIG_SSI_Table_Write\n"));

	 /*bryan temp	mark*/
	 #if(0)
	 DVBT_NORDIGSSITableWrite_PARAM	Drv_DVBT_NORDIGSSITableWrite_PARAM;
	 Drv_DVBT_NORDIGSSITableWrite_PARAM.constel=constel;
	 Drv_DVBT_NORDIGSSITableWrite_PARAM.code_rate=code_rate;
	 Drv_DVBT_NORDIGSSITableWrite_PARAM.write_value=write_value;
	 #endif
	 //bryan temp	mark
	 #if(0)
	 if(u32DVBTopen==1)
		 UtopiaIoctl(ppDVBTInstant,	DMD_DVBT_DRV_CMD_NORDIGSSITableWrite,	&Drv_DVBT_NORDIGSSITableWrite_PARAM);
	 else
		return false;

			return Drv_DVBT_NORDIGSSITableWrite_PARAM.ret;
	 #else
//			 Drv_DVBT_NORDIGSSITableWrite_PARAM.ret=true;
		return true;
	 #endif


		/*
		return INTERN_DVBT_NORDIG_SSI_Table_Write(constel, code_rate,	write_value);
		*/
}
#endif

#ifndef	MSOS_TYPE_LINUX_KERNEL
MS_BOOL	MDrv_DMD_DVBT_NORDIG_SSI_Table_Read(DMD_CONSTEL	constel, DMD_CODERATE	code_rate, float *read_value)
{
		//return INTERN_DVBT_NORDIG_SSI_Table_Read(constel,	code_rate, read_value);
//	MS_BOOL	return_val;
		DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT.c]MDrv_DMD_DVBT_NORDIG_SSI_Table_Read\n"));

	/*bryan	temp mark*/
	#if(0)
	DVBT_NORDIGSSITableRead_PARAM	Drv_DVBT_NORDIGSSITableRead_PARAM;
	Drv_DVBT_NORDIGSSITableRead_PARAM.constel=constel;
	Drv_DVBT_NORDIGSSITableRead_PARAM.code_rate=code_rate;
	Drv_DVBT_NORDIGSSITableRead_PARAM.read_value=read_value;
	#endif

	//bryan	temp mark
	#if(0)
	 if(u32DVBTopen==1)
		UtopiaIoctl(ppDVBTInstant, DMD_DVBT_DRV_CMD_NORDIGSSITableRead,	&Drv_DVBT_NORDIGSSITableRead_PARAM);
	 else
		return false;

			return Drv_DVBT_NORDIGSSITableRead_PARAM.ret;
	 #else
		//Drv_DVBT_NORDIGSSITableRead_PARAM.ret=true;
		return true;
	 #endif



}
#endif
MS_U32 MDrv_DMD_DVBT_SetPowerState(EN_POWER_MODE u16PowerState)
{
//			MS_BOOL	return_val;
		 DVBT_SetPowerState_PARAM	Drv_DVBT_SetPowerState_PARAM;
		 Drv_DVBT_SetPowerState_PARAM.u16PowerState=u16PowerState;
		 Drv_DVBT_SetPowerState_PARAM.ret=false;

		DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT.c]MDrv_DMD_DVBT_SetPowerState\n"));
		 if(u32DVBTopen==1)
	 UtopiaIoctl(ppDVBTInstant,	DMD_DVBT_DRV_CMD_SetPowerState,	&Drv_DVBT_SetPowerState_PARAM);
		 else
	 return	false;

	 return	Drv_DVBT_SetPowerState_PARAM.ret;
	 /*
		static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
		MS_U32 u32Return = UTOPIA_STATUS_FAIL;

		DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT.c]MDrv_DMD_DVBT_SetPowerState\n"));

		u32Return	=	u32Return;
		if (u16PowerState	== E_POWER_SUSPEND)
	{
				MDrv_DMD_DVBT_Exit();
				_prev_u16PowerState	=	u16PowerState;
				u32Return	=	UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
		}
		else if	(u16PowerState ==	E_POWER_RESUME)
		{
				if (_prev_u16PowerState	== E_POWER_SUSPEND)
				{
						MDrv_DMD_DVBT_Init(&_sDMD_DVBT_InitData, sizeof(_sDMD_DVBT_InitData));
						_prev_u16PowerState	=	u16PowerState;
						u32Return	=	UTOPIA_STATUS_SUCCESS;//RESUME_OK;
				}
				else
				{
						ULOGD("DEMOD","[%s,%5d]It	is not suspended yet.	We shouldn't resume\n",__FUNCTION__,__LINE__);
						u32Return	=	UTOPIA_STATUS_FAIL;//SUSPEND_FAILED;
				}
		}
		else
		{
			ULOGD("DEMOD","[%s,%5d]Do	Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
			u32Return	=	FALSE;
		}
		return UTOPIA_STATUS_SUCCESS;
		*/
}
