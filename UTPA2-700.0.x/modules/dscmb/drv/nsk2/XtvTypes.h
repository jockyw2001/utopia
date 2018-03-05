/*****************************************************************************\
  Copyright (C) 2004  : NDS Ltd.

  P R O P R I E T A R Y        C O N F I D E N T I A L

  The copyright of this document is vested in NDS Ltd. without whose prior
  written permission its contents must not be published,adapted
  or reproduced in any form or disclosed or issued to any third party.

  ===========================================================================
  \Purpose: XTV services common types - for 3rd parties

  ===========================================================================
  VSS file:    $Workfile: XtvTypes.h $    $Revision: 23 $
  VSS last change:    $Author: Ozecharia $    $Date: 7/21/04 3:00p $    $Modtime: 7/21/04 2:55p $

\*****************************************************************************/

#ifndef _INC_XTVTYPES
#define _INC_XTVTYPES

/*****************************************************************************/

/* public, package and private functions prefix */
#define PUBFUNC /* public function */
#define PKGFUNC /* package function - no way in standard C to specify "package only" visibility during compilation */
#define PRVFUNC static /* private function */

/* function in and out parameters prefix */
#define _in_
#define _out_
#define _in_out_


/*****************************************************************************/
/* basic types */

#ifndef XNULL
#define XNULL ((void *)0)
#endif

typedef   signed char    XCHAR;      /* Signed    8 bit quantity  */
typedef unsigned char    XUCHAR;     /* Unsigned  8 bit quantity  */
typedef   signed short   XSHORT;     /* Signed   16 bit quantity  */
typedef unsigned short   XUSHORT;    /* Unsigned 16 bit quantity  */
typedef   signed long    XLONG;      /* Signed   32 bit quantity  */
typedef unsigned long    XULONG;     /* Unsigned 32 bit quantity  */
typedef unsigned long    F_RASP_EV_MASK;

typedef enum xbool
{
    XFALSE = 0,
    XTRUE  = 1
} XBOOL;

typedef enum trigger_executor
{
		F_TRIGGER_BY_EVENT = 0,
		F_TRIGGER_BY_TIMER = 1,
} TRIGGER_EXECUTOR;


typedef XUCHAR*     XSTRING  ; /* null-terminated string */

typedef  XULONG             XHANDLE; /* general handle */

/* data buffer + its length */
typedef struct XDATA_
{
    XUCHAR  *data;
  XUSHORT length;
} XDATA;

/* const data buffer + its length */
typedef struct XCDATA_
{
    const XUCHAR  *data;
    XUSHORT length;
} XCDATA;

typedef struct
{
    F_RASP_EV_MASK     		Supported_Event;
    XULONG         				Event_Timer_Rate;
    XUSHORT				        Num_Rasps;
    XUSHORT        				Max_Event_Num_In_Fifo;
    XUSHORT        				Num_of_Payload_Buffers;
    XUSHORT        				Num_of_Filters;
    XBOOL             		Event_Generation_Every_Slot_Time;
    XBOOL             		PCR_Stamp;
    XBOOL             		Local_Clock_Stamp;
    XBOOL             		Trigger_by_Evnet;
    XBOOL             		Trigger_by_Time ;

}RASP_CAPS;

typedef struct
{
    XULONG        Event_Descriptor;
    XUSHORT       Pid;
    XUSHORT       Reserved;
    XULONG        PacketNum;
    XULONG        Timer;
    XULONG        PCR;

}RASP_EVENT;


/* time and duration */
typedef struct XTIME_
{
    XULONG time;    /* standard ANSI time in seconds (from Jan 1970) */
    XULONG milli;   /* where TOD clock supports it else 0            */
} XTIME;

#define XTIME_NULL  {0,0} /* null value for XTIME */

typedef XLONG       XDURATION;      /* time duration */

#define XDURATION_NULL  (0)         /* null value for XDURATION*/

#define XDURATION_MAX   (0x7FFFFFFF)/* maximum duration value */

typedef unsigned short  (*RASP_EVENT_NOTIFY_FOUNCTION)
(
    XULONG      		Xconn,
    XUSHORT  				NumEvent,
    RASP_EVENT      RaspEvents[]
);

typedef unsigned short  (*RASP_OVERFLOW_NOTIFY_FUNCTION)
(
    XULONG      Xconn
);


/* file position offset relative to */
typedef enum XPOSITION_OFFSET_
{
    XPOSITION_OFFSET_START=0,   /* relative to file start */
    XPOSITION_OFFSET_END,   /* relative to file end */
    XPOSITION_OFFSET_CURRENT    /* relative to current position */
} XPOSITION_OFFSET;


/* playback file EOF types */
typedef enum XEOF_POSITION_
{
    XEOF_POSITION_BEGIN= 0,             /* Playback has reached the beginning of file */
    XEOF_POSITION_END                   /* Playback has reached the end of file */
} XEOF_POSITION;


/* playback stop mode */
typedef enum XSTOP_MODE_
{
    XSTOP_MODE_DEFAULT=0,
    XSTOP_MODE_FREEZE,
    XSTOP_MODE_BLACK
} XSTOP_MODE;


/* languages data length */
#define XLANGUAGES_MAX_LEN  (15) /* list of languages max length */


/*****************************************************************************/
/* XCONNECTION */

typedef  XULONG             XCONNECTION;

#define XCONNECTION_INVALID     (0L) /* invalid XCONNECTION value */

/* connection id */
typedef enum XCONNID_
{
    XCONNID_T1_LIVE=501,    /* Live display from tuner 1 */
    XCONNID_T2_LIVE,        /* Live display from tuner 2 */
    XCONNID_T1_RECORD,      /* Record from tuner 1 to disk */
    XCONNID_T2_RECORD,      /* Record from tuner 2 to disk */
    XCONNID_PLAY1           /* Play from disk to display */
} XCONNID;


/*****************************************************************************/
/* XSTATUS - standard return status */

typedef XUSHORT             XSTATUS;

typedef XULONG              XLSTATUS; /* long return status */


/* general purpose XSTATUS values */

#define XSTATUS_OK          (XSTATUS)0      /* success */
#define XSTATUS_ERROR       (XSTATUS)1      /* general error */
#define XSTATUS_PARAM_ERROR     (XSTATUS)2      /* parameter error */
#define XSTATUS_IN_USE          (XSTATUS)3      /* the required resources are in use */
#define XSTATUS_NOT_AVAILABLE   (XSTATUS)4      /* the required resources are not available */
#define XSTATUS_DISK_FULL       (XSTATUS)5      /* disk full */
#define XSTATUS_ASYNCHRONOUS    (XSTATUS)6      /* the function execution is asynchronous */
#define XSTATUS_TUNER_CONFLICT  (XSTATUS)7      /* tuner can not be allocated */
#define XSTATUS_OK_TUNER_CHANGED (XSTATUS)8     /* tuner was changed */
#define XSTATUS_OK_NO_RETUNE     (XSTATUS)9     /* retuning was not necessary */
#define XSTATUS_NOT_INITIALIZED  (XSTATUS)10        /* the data requested had not been initialized */

#define X_OK        XSTATUS_OK
#define X_ERROR     XSTATUS_ERROR


/* XSTATUS warning/error creation
  parametrs:
    mod - Name of module, generating Warning / error
    num - Number of error
    txt - Error description
*/
#define X_WARN_CREATE(mod,num,txt)      ((XSTATUS)(((mod & 0x7f)<< 8) | (num & 0xff)))
#define X_ERR_CREATE(mod,num,txt)       ((XSTATUS)(0x8000 | X_WARN_CREATE(mod,num,txt)))


/* XSTATUS warning/error and module information retriving macros */
#define X_IS_ERR(s)                     ((s & 0x8000) != 0)
#define X_IS_WARN(s)                    (!X_IS_ERR(s))
#define X_GET_MODULE(s)                 ((s>>8) & 0x7f)


/*****************************************************************************/
/* general bit set macros */

/* Check bit at offset */
#define  X_IS_BIT_SET(val,off)   (((val)&(1U<<(off)))!=0)

/* Set bit at offset */
#define  X_SET_BIT(val,off)     {(val)|=(1U<<(off));}

/* Clear bit at offset */
#define  X_CLR_BIT(val,off)     {(val)&=~(1U<<(off));}


/*****************************************************************************/
/* 64 bits API
Poor men OO 64 bit long platform independent interface.
Implementation is platform dependent.
*/

/*  Private Implementation. Don't build on it !!! */
typedef struct _XI64{
    long DontUseIt[2];
} XI64;

#ifdef XBOOL64INT
#define XBOOL64 int
#else
#define XBOOL64 XBOOL
#endif

/*  Public: Use it */
void    XI64Set(XI64* ptarget, XI64* porigin);
XBOOL64 XI64Equal(XI64* ptarget, XI64* porigin);
XBOOL64 XI64GE(XI64* ptarget, XI64* porigin);
XBOOL64 XI64Equal0(XI64* ptarget);
XBOOL64 XI64Negative(XI64* ptarget);
void    XI64SetLong(XI64* ptarget, XLONG high, XULONG low);
void    XI64GetLong(XI64* ptarget, XLONG* phigh, XULONG* plow);
void    XI64Add(XI64* presult, XI64* porigin, XI64* poffset); /* *presult = *porigin + *poffset */
void    XI64Sub(XI64* presult, XI64* porigin, XI64* poffset); /* *presult = *porigin - *poffset */
void    XI64Div(XI64* presult, XI64* porigin, XI64* poffset); /* *presult = *porigin / *poffset */
void    XI64Mul(XI64* presult, XI64* porigin, XI64* poffset); /* *presult = *porigin * (*poffset) */
void    XI64ShiftL(XI64* presult, XI64* porigin, int numBits); /* *presult = *porigin << *numBits */
void    XI64ShiftR(XI64* presult, XI64* porigin, int numBits); /* *presult = *porigin >> *numBits */
char*   XI64ToChar(XI64* ptarget);                              /* character representation, 10 based */
#define XI64GR(X,Y) !(XI64GE(Y,X))


#endif /* _INC_XTVTYPES */

