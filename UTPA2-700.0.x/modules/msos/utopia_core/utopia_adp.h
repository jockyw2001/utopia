#ifndef _UTOPIA_ADP_H_
#define _UTOPIA_ADP_H_

//Utopia Adaption layer

//#include <linux/list.h>
#include "MsTypes.h"

#define MODULE_NUM 5


/*
 * Utopia Adaption layer tools
 */
#define UADP_SPT_SELF_SIZE           1
#define UADP_SPT_POINTER_TO_NEXT     2
#define UADP_SPT_END                5

#define UADP_SDT_SELF_SIZE          1
#define UADP_SDT_P2N                2
#define UADP_SDT_AT                 3
#define UADP_SDT_ES                 4
#define UADP_SDT_END                5

typedef struct _UADP_STRUCT_POINTER_TABLE
{
    unsigned int type;
    unsigned int offset;
    unsigned long size_or_next; //size of this structure or pointer to next structure
}UADP_STRUCT_POINTER_TABLE;

#define DEADBEEF 0xdeadbeef
typedef struct _SEQUNCE_MEMORY_MANAGE
{
    unsigned int uFreeIndex;//0~uSlotTotal-1, upper layer also can check this value to know max memory usage
    unsigned int uSlotTotal;//one slot = 4 byte
    unsigned int uFlag;//bit 0: overflow
    unsigned int pMemoryPool[1];
}SEQUNCE_MEMORY_MANAGE;

//-----------------------------------------------------------------------------------------------
//STYLE 0 BASIC MACRO: You need take care most of your SPT description
#define UADP_SPT_BGN(TABLE_ELEMENT,SIZE) {UADPBypassSetSPT(TABLE_ELEMENT,UADP_SPT_SELF_SIZE,0, SIZE)
#define UADP_SPT_NXT(TABLE_ELEMENT,TYPE,MEMBER,NEXT_TABLE) UADPBypassSetSPT(TABLE_ELEMENT,UADP_SPT_POINTER_TO_NEXT,((unsigned long) &((TYPE *)0)->MEMBER), (unsigned long)NEXT_TABLE);
#define UADP_SPT_FIN(TABLE_ELEMENT) UADPBypassSetSPT(TABLE_ELEMENT,UADP_SPT_END , 0, 0);}

#define UADP_SDT_BGN(TABLE_ELEMENT,SIZE) {UADPBypassSetSPT(TABLE_ELEMENT,UADP_SPT_SELF_SIZE,0, SIZE)
#define UADP_SDT_KIND(TABLE_ELEMENT,TYPE,KIND,MEMBER,NEXT_TABLE) UADPBypassSetSPT(TABLE_ELEMENT,KIND,((unsigned long) &((TYPE *)0)->MEMBER), (unsigned long)NEXT_TABLE);
#define UADP_SDT_FIN(TABLE_ELEMENT) UADPBypassSetSPT(TABLE_ELEMENT,UADP_SPT_END , 0, 0);}
//-----------------------------------------------------------------------------------------------
//STYLE 1 NAME-TYPE MACRO: You need take care "NAME", but don't need take care "TABLE_ELEMENTs"
#define UADP_SPT_NAMEnNXT_DEF(NAME,n) UADP_STRUCT_POINTER_TABLE spt_##NAME [2+n]

#define UADP_SPT_NAME0NXT(NAME,TYPE) \
            UADP_SPT_BGN(& spt_##NAME [0],sizeof(TYPE)); \
            UADP_SPT_FIN(& spt_##NAME [1]);
            
#define UADP_SPT_NAME1NXT(NAME,TYPE,MB1,MB1NAME) \
            UADP_SPT_BGN(& spt_##NAME [0],sizeof(TYPE)); \
            UADP_SPT_NXT(& spt_##NAME [1],TYPE,MB1,spt_##MB1NAME); \
            UADP_SPT_FIN(& spt_##NAME [2]);            

#define UADP_SPT_NAME2NXT(NAME,TYPE,MB1,MB1NAME,MB2,MB2NAME) \
            UADP_SPT_BGN(& spt_##NAME [0],sizeof(TYPE)); \
            UADP_SPT_NXT(& spt_##NAME [1],TYPE,MB1,spt_##MB1NAME); \
            UADP_SPT_NXT(& spt_##NAME [2],TYPE,MB2,spt_##MB2NAME); \
            UADP_SPT_FIN(& spt_##NAME [3]);    

#define UADP_SPT_NAME3NXT(NAME,TYPE,MB1,MB1NAME,MB2,MB2NAME,MB3,MB3NAME) \
            UADP_SPT_BGN(& spt_##NAME [0],sizeof(TYPE)); \
            UADP_SPT_NXT(& spt_##NAME [1],TYPE,MB1,spt_##MB1NAME); \
            UADP_SPT_NXT(& spt_##NAME [2],TYPE,MB2,spt_##MB2NAME); \
            UADP_SPT_NXT(& spt_##NAME [3],TYPE,MB3,spt_##MB3NAME); \
            UADP_SPT_FIN(& spt_##NAME [4]);

#define UADP_SPT_NAME4NXT(NAME,TYPE,MB1,MB1NAME,MB2,MB2NAME,MB3,MB3NAME,MB4,MB4NAME) \
            UADP_SPT_BGN(& spt_##NAME [0],sizeof(TYPE)); \
            UADP_SPT_NXT(& spt_##NAME [1],TYPE,MB1,spt_##MB1NAME); \
            UADP_SPT_NXT(& spt_##NAME [2],TYPE,MB2,spt_##MB2NAME); \
            UADP_SPT_NXT(& spt_##NAME [3],TYPE,MB3,spt_##MB3NAME); \
            UADP_SPT_NXT(& spt_##NAME [4],TYPE,MB4,spt_##MB4NAME); \
            UADP_SPT_FIN(& spt_##NAME [5]);
            
#define UADP_SPT_NAME5NXT(NAME,TYPE,MB1,MB1NAME,MB2,MB2NAME,MB3,MB3NAME,MB4,MB4NAME,MB5,MB5NAME) \
            UADP_SPT_BGN(& spt_##NAME [0],sizeof(TYPE)); \
            UADP_SPT_NXT(& spt_##NAME [1],TYPE,MB1,spt_##MB1NAME); \
            UADP_SPT_NXT(& spt_##NAME [2],TYPE,MB2,spt_##MB2NAME); \
            UADP_SPT_NXT(& spt_##NAME [3],TYPE,MB3,spt_##MB3NAME); \
            UADP_SPT_NXT(& spt_##NAME [4],TYPE,MB4,spt_##MB4NAME); \
            UADP_SPT_NXT(& spt_##NAME [5],TYPE,MB5,spt_##MB5NAME); \
            UADP_SPT_FIN(& spt_##NAME [6]);

#define UADP_SPT_NAME6NXT(NAME,TYPE,MB1,MB1NAME,MB2,MB2NAME,MB3,MB3NAME,MB4,MB4NAME,MB5,MB5NAME,MB6,MB6NAME) \
            UADP_SPT_BGN(& spt_##NAME [0],sizeof(TYPE)); \
            UADP_SPT_NXT(& spt_##NAME [1],TYPE,MB1,spt_##MB1NAME); \
            UADP_SPT_NXT(& spt_##NAME [2],TYPE,MB2,spt_##MB2NAME); \
            UADP_SPT_NXT(& spt_##NAME [3],TYPE,MB3,spt_##MB3NAME); \
            UADP_SPT_NXT(& spt_##NAME [4],TYPE,MB4,spt_##MB4NAME); \
            UADP_SPT_NXT(& spt_##NAME [5],TYPE,MB5,spt_##MB5NAME); \
            UADP_SPT_NXT(& spt_##NAME [6],TYPE,MB6,spt_##MB6NAME); \
            UADP_SPT_FIN(& spt_##NAME [7]);

#define UADP_SPT_NAME9NXT(NAME,TYPE,MB1,MB1NAME,MB2,MB2NAME,MB3,MB3NAME,MB4,MB4NAME,MB5,MB5NAME,MB6,MB6NAME,MB7,MB7NAME,MB8,MB8NAME,MB9,MB9NAME) \
            UADP_SPT_BGN(& spt_##NAME [0],sizeof(TYPE)); \
            UADP_SPT_NXT(& spt_##NAME [1],TYPE,MB1,spt_##MB1NAME); \
            UADP_SPT_NXT(& spt_##NAME [2],TYPE,MB2,spt_##MB2NAME); \
            UADP_SPT_NXT(& spt_##NAME [3],TYPE,MB3,spt_##MB3NAME); \
            UADP_SPT_NXT(& spt_##NAME [4],TYPE,MB4,spt_##MB4NAME); \
            UADP_SPT_NXT(& spt_##NAME [5],TYPE,MB5,spt_##MB5NAME); \
            UADP_SPT_NXT(& spt_##NAME [6],TYPE,MB6,spt_##MB6NAME); \
            UADP_SPT_NXT(& spt_##NAME [7],TYPE,MB7,spt_##MB6NAME); \
            UADP_SPT_NXT(& spt_##NAME [8],TYPE,MB8,spt_##MB6NAME); \
            UADP_SPT_NXT(& spt_##NAME [9],TYPE,MB9,spt_##MB6NAME); \
            UADP_SPT_FIN(& spt_##NAME [10]);

#define UADP_SPT_NAME11NXT(NAME,TYPE,MB1,MB1NAME,MB2,MB2NAME,MB3,MB3NAME,MB4,MB4NAME,MB5,MB5NAME,MB6,MB6NAME,MB7,MB7NAME,MB8,MB8NAME,MB9,MB9NAME,MB10,MB10NAME,MB11,MB11NAME) \
            UADP_SPT_BGN(& spt_##NAME [0],sizeof(TYPE)); \
            UADP_SPT_NXT(& spt_##NAME [1],TYPE,MB1,spt_##MB1NAME); \
            UADP_SPT_NXT(& spt_##NAME [2],TYPE,MB2,spt_##MB2NAME); \
            UADP_SPT_NXT(& spt_##NAME [3],TYPE,MB3,spt_##MB3NAME); \
            UADP_SPT_NXT(& spt_##NAME [4],TYPE,MB4,spt_##MB4NAME); \
            UADP_SPT_NXT(& spt_##NAME [5],TYPE,MB5,spt_##MB5NAME); \
            UADP_SPT_NXT(& spt_##NAME [6],TYPE,MB6,spt_##MB6NAME); \
            UADP_SPT_NXT(& spt_##NAME [7],TYPE,MB7,spt_##MB6NAME); \
            UADP_SPT_NXT(& spt_##NAME [8],TYPE,MB8,spt_##MB6NAME); \
            UADP_SPT_NXT(& spt_##NAME [9],TYPE,MB9,spt_##MB6NAME); \
            UADP_SPT_NXT(& spt_##NAME [10],TYPE,MB10,spt_##MB6NAME); \
            UADP_SPT_NXT(& spt_##NAME [11],TYPE,MB11,spt_##MB6NAME); \
            UADP_SPT_FIN(& spt_##NAME [12]);


#define UADP_SDT_NAMEn_DEF(NAME,n) UADP_STRUCT_POINTER_TABLE spt_##NAME [2+n]

#define UADP_SDT_NAME0(NAME,TYPE) \
            UADP_SPT_BGN(& spt_##NAME  [0],sizeof(TYPE)); \
            UADP_SPT_FIN(& spt_##NAME  [1]);
            
#define UADP_SDT_NAME1(NAME,TYPE,MB1KIND,MB1,MB1NAME) \
            UADP_SPT_BGN(& spt_##NAME  [0],sizeof(TYPE)); \
            UADP_SDT_KIND(& spt_##NAME [1],TYPE,MB1KIND,MB1,spt_##MB1NAME); \
            UADP_SPT_FIN(& spt_##NAME  [2]);            

#define UADP_SDT_NAME2(NAME,TYPE,MB1KIND,MB1,MB1NAME,MB2KIND,MB2,MB2NAME) \
            UADP_SPT_BGN(& spt_##NAME  [0],sizeof(TYPE)); \
            UADP_SDT_KIND(& spt_##NAME [1],TYPE,MB1KIND,MB1,spt_##MB1NAME); \
            UADP_SDT_KIND(& spt_##NAME [2],TYPE,MB2KIND,MB2,spt_##MB2NAME); \
            UADP_SPT_FIN(& spt_##NAME  [3]);    

#define UADP_SDT_NAME3(NAME,TYPE,MB1KIND,MB1,MB1NAME,MB2KIND,MB2,MB2NAME,MB3KIND,MB3,MB3NAME) \
            UADP_SPT_BGN(& spt_##NAME  [0],sizeof(TYPE)); \
            UADP_SDT_KIND(& spt_##NAME [1],TYPE,MB1KIND,MB1,spt_##MB1NAME); \
            UADP_SDT_KIND(& spt_##NAME [2],TYPE,MB2KIND,MB2,spt_##MB2NAME); \
            UADP_SDT_KIND(& spt_##NAME [3],TYPE,MB3KIND,MB3,spt_##MB3NAME); \
            UADP_SPT_FIN(& spt_##NAME  [4]);    

#define UADP_SDT_NAME4(NAME,TYPE,MB1KIND,MB1,MB1NAME,MB2KIND,MB2,MB2NAME,MB3KIND,MB3,MB3NAME,MB4KIND,MB4,MB4NAME) \
            UADP_SPT_BGN(& spt_##NAME  [0],sizeof(TYPE)); \
            UADP_SDT_KIND(& spt_##NAME [1],TYPE,MB1KIND,MB1,spt_##MB1NAME); \
            UADP_SDT_KIND(& spt_##NAME [2],TYPE,MB2KIND,MB2,spt_##MB2NAME); \
            UADP_SDT_KIND(& spt_##NAME [3],TYPE,MB3KIND,MB3,spt_##MB3NAME); \
            UADP_SDT_KIND(& spt_##NAME [4],TYPE,MB4KIND,MB4,spt_##MB4NAME); \
            UADP_SPT_FIN(& spt_##NAME  [5]); 

#define UADP_SDT_NAME5(NAME,TYPE,MB1KIND,MB1,MB1NAME,MB2KIND,MB2,MB2NAME,MB3KIND,MB3,MB3NAME,MB4KIND,MB4,MB4NAME,MB5KIND,MB5,MB5NAME) \
            UADP_SPT_BGN(& spt_##NAME  [0],sizeof(TYPE)); \
            UADP_SDT_KIND(& spt_##NAME [1],TYPE,MB1KIND,MB1,spt_##MB1NAME); \
            UADP_SDT_KIND(& spt_##NAME [2],TYPE,MB2KIND,MB2,spt_##MB2NAME); \
            UADP_SDT_KIND(& spt_##NAME [3],TYPE,MB3KIND,MB3,spt_##MB3NAME); \
            UADP_SDT_KIND(& spt_##NAME [4],TYPE,MB4KIND,MB4,spt_##MB4NAME); \
            UADP_SDT_KIND(& spt_##NAME [5],TYPE,MB5KIND,MB5,spt_##MB5NAME); \
            UADP_SPT_FIN(& spt_##NAME  [6]); 

#define UADP_SDT_NAME6(NAME,TYPE,MB1KIND,MB1,MB1NAME,MB2KIND,MB2,MB2NAME,MB3KIND,MB3,MB3NAME,MB4KIND,MB4,MB4NAME,MB5KIND,MB5,MB5NAME,MB6KIND,MB6,MB6NAME) \
            UADP_SPT_BGN(& spt_##NAME  [0],sizeof(TYPE)); \
            UADP_SDT_KIND(& spt_##NAME [1],TYPE,MB1KIND,MB1,spt_##MB1NAME); \
            UADP_SDT_KIND(& spt_##NAME [2],TYPE,MB2KIND,MB2,spt_##MB2NAME); \
            UADP_SDT_KIND(& spt_##NAME [3],TYPE,MB3KIND,MB3,spt_##MB3NAME); \
            UADP_SDT_KIND(& spt_##NAME [4],TYPE,MB4KIND,MB4,spt_##MB4NAME); \
            UADP_SDT_KIND(& spt_##NAME [5],TYPE,MB5KIND,MB5,spt_##MB5NAME); \
            UADP_SDT_KIND(& spt_##NAME [6],TYPE,MB6KIND,MB6,spt_##MB6NAME); \
            UADP_SPT_FIN(& spt_##NAME  [7]); 
#define UADP_SDT_NAME7(NAME,TYPE,MB1KIND,MB1,MB1NAME,MB2KIND,MB2,MB2NAME,MB3KIND,MB3,MB3NAME,MB4KIND,MB4,MB4NAME,MB5KIND,MB5,MB5NAME,MB6KIND,MB6,MB6NAME,MB7KIND,MB7,MB7NAME) \
            UADP_SPT_BGN(& spt_##NAME  [0],sizeof(TYPE)); \
						UADP_SDT_KIND(& spt_##NAME [1],TYPE,MB1KIND,MB1,spt_##MB1NAME); \
						UADP_SDT_KIND(& spt_##NAME [2],TYPE,MB2KIND,MB2,spt_##MB2NAME); \
						UADP_SDT_KIND(& spt_##NAME [3],TYPE,MB3KIND,MB3,spt_##MB3NAME); \
						UADP_SDT_KIND(& spt_##NAME [4],TYPE,MB4KIND,MB4,spt_##MB4NAME); \
						UADP_SDT_KIND(& spt_##NAME [5],TYPE,MB5KIND,MB5,spt_##MB5NAME); \
						UADP_SDT_KIND(& spt_##NAME [6],TYPE,MB6KIND,MB6,spt_##MB6NAME); \
						UADP_SDT_KIND(& spt_##NAME [7],TYPE,MB7KIND,MB7,spt_##MB7NAME); \
            UADP_SPT_FIN(& spt_##NAME  [8]); 
#define UADP_SDT_NAME8(NAME,TYPE,MB1KIND,MB1,MB1NAME,MB2KIND,MB2,MB2NAME,MB3KIND,MB3,MB3NAME,MB4KIND,MB4,MB4NAME,MB5KIND,MB5,MB5NAME,MB6KIND,MB6,MB6NAME,MB7KIND,MB7,MB7NAME,MB8KIND,MB8,MB8NAME) \
            UADP_SPT_BGN(& spt_##NAME  [0],sizeof(TYPE)); \
						UADP_SDT_KIND(& spt_##NAME [1],TYPE,MB1KIND,MB1,spt_##MB1NAME); \
						UADP_SDT_KIND(& spt_##NAME [2],TYPE,MB2KIND,MB2,spt_##MB2NAME); \
						UADP_SDT_KIND(& spt_##NAME [3],TYPE,MB3KIND,MB3,spt_##MB3NAME); \
						UADP_SDT_KIND(& spt_##NAME [4],TYPE,MB4KIND,MB4,spt_##MB4NAME); \
						UADP_SDT_KIND(& spt_##NAME [5],TYPE,MB5KIND,MB5,spt_##MB5NAME); \
						UADP_SDT_KIND(& spt_##NAME [6],TYPE,MB6KIND,MB6,spt_##MB6NAME); \
						UADP_SDT_KIND(& spt_##NAME [7],TYPE,MB7KIND,MB7,spt_##MB7NAME); \
						UADP_SDT_KIND(& spt_##NAME [8],TYPE,MB8KIND,MB8,spt_##MB8NAME); \
            UADP_SPT_FIN(& spt_##NAME  [9]); 

#define UADP_SDT_NAME16(NAME,TYPE,MB1KIND,MB1,MB1NAME,MB2KIND,MB2,MB2NAME,MB3KIND,MB3,MB3NAME,MB4KIND,MB4,MB4NAME,MB5KIND,MB5,MB5NAME,MB6KIND,MB6,MB6NAME,MB7KIND,MB7,MB7NAME,MB8KIND,MB8,MB8NAME,\
    MB9KIND,MB9,MB9NAME,MB10KIND,MB10,MB10NAME,MB11KIND,MB11,MB11NAME,MB12KIND,MB12,MB12NAME,MB13KIND,MB13,MB13NAME,MB14KIND,MB14,MB14NAME,MB15KIND,MB15,MB15NAME,MB16KIND,MB16,MB16NAME) \
            UADP_SPT_BGN(& spt_##NAME  [0],sizeof(TYPE)); \
						UADP_SDT_KIND(& spt_##NAME [1],TYPE,MB1KIND,MB1,spt_##MB1NAME); \
						UADP_SDT_KIND(& spt_##NAME [2],TYPE,MB2KIND,MB2,spt_##MB2NAME); \
						UADP_SDT_KIND(& spt_##NAME [3],TYPE,MB3KIND,MB3,spt_##MB3NAME); \
						UADP_SDT_KIND(& spt_##NAME [4],TYPE,MB4KIND,MB4,spt_##MB4NAME); \
						UADP_SDT_KIND(& spt_##NAME [5],TYPE,MB5KIND,MB5,spt_##MB5NAME); \
						UADP_SDT_KIND(& spt_##NAME [6],TYPE,MB6KIND,MB6,spt_##MB6NAME); \
						UADP_SDT_KIND(& spt_##NAME [7],TYPE,MB7KIND,MB7,spt_##MB7NAME); \
						UADP_SDT_KIND(& spt_##NAME [8],TYPE,MB8KIND,MB8,spt_##MB8NAME); \
                        UADP_SDT_KIND(& spt_##NAME [9],TYPE,MB9KIND,MB9,spt_##MB9NAME); \
                        UADP_SDT_KIND(& spt_##NAME [10],TYPE,MB10KIND,MB10,spt_##MB10NAME); \
                        UADP_SDT_KIND(& spt_##NAME [11],TYPE,MB11KIND,MB11,spt_##MB11NAME); \
                        UADP_SDT_KIND(& spt_##NAME [12],TYPE,MB12KIND,MB12,spt_##MB12NAME); \
                        UADP_SDT_KIND(& spt_##NAME [13],TYPE,MB13KIND,MB13,spt_##MB13NAME); \
                        UADP_SDT_KIND(& spt_##NAME [14],TYPE,MB14KIND,MB14,spt_##MB14NAME); \
                        UADP_SDT_KIND(& spt_##NAME [15],TYPE,MB15KIND,MB15,spt_##MB15NAME); \
                        UADP_SDT_KIND(& spt_##NAME [16],TYPE,MB16KIND,MB16,spt_##MB16NAME); \
            UADP_SPT_FIN(& spt_##NAME  [17]);
//-----------------------------------------------------------------------------------------------
//STYLE 2 TYPE MACRO: You don't need take care "NAME", we will do it.
#define UADP_SPT_0NXT_DEF(TYPE)                                                             UADP_SPT_NAMEnNXT_DEF(TYPE,0)
#define UADP_SPT_1NXT_DEF(TYPE)                                                             UADP_SPT_NAMEnNXT_DEF(TYPE,1)
#define UADP_SPT_2NXT_DEF(TYPE)                                                             UADP_SPT_NAMEnNXT_DEF(TYPE,2)
#define UADP_SPT_3NXT_DEF(TYPE)                                                             UADP_SPT_NAMEnNXT_DEF(TYPE,3)
#define UADP_SPT_4NXT_DEF(TYPE)                                                             UADP_SPT_NAMEnNXT_DEF(TYPE,4)
#define UADP_SPT_5NXT_DEF(TYPE)                                                             UADP_SPT_NAMEnNXT_DEF(TYPE,5)

#define UADP_SPT_0NXT(TYPE)                                                                 UADP_SPT_NAME0NXT(TYPE,TYPE)
#define UADP_SPT_1NXT(TYPE,MB1,MB1TYPE)                                                     UADP_SPT_NAME1NXT(TYPE,TYPE,MB1,MB1TYPE)
#define UADP_SPT_2NXT(TYPE,MB1,MB1TYPE,MB2,MB2TYPE)                                         UADP_SPT_NAME2NXT(TYPE,TYPE,MB1,MB1TYPE,MB2,MB2TYPE)
#define UADP_SPT_3NXT(TYPE,MB1,MB1TYPE,MB2,MB2TYPE,MB3,MB3TYPE)                             UADP_SPT_NAME3NXT(TYPE,TYPE,MB1,MB1TYPE,MB2,MB2TYPE,MB3,MB3TYPE)
#define UADP_SPT_4NXT(TYPE,MB1,MB1TYPE,MB2,MB2TYPE,MB3,MB3TYPE,MB4,MB4TYPE)                 UADP_SPT_NAME4NXT(TYPE,TYPE,MB1,MB1TYPE,MB2,MB2TYPE,MB3,MB3TYPE,MB4,MB4TYPE)
#define UADP_SPT_5NXT(TYPE,MB1,MB1TYPE,MB2,MB2TYPE,MB3,MB3TYPE,MB4,MB4TYPE,MB5,MB5TYPE)     UADP_SPT_NAME5NXT(TYPE,TYPE,MB1,MB1TYPE,MB2,MB2TYPE,MB3,MB3TYPE,MB4,MB4TYPE,MB5,MB5TYPE)

#define UADP_SDT_0_DEF(TYPE)                                                                                                        UADP_SDT_NAMEn_DEF(TYPE,0)
#define UADP_SDT_1_DEF(TYPE)                                                                                                        UADP_SDT_NAMEn_DEF(TYPE,1)
#define UADP_SDT_2_DEF(TYPE)                                                                                                        UADP_SDT_NAMEn_DEF(TYPE,2)
#define UADP_SDT_3_DEF(TYPE)                                                                                                        UADP_SDT_NAMEn_DEF(TYPE,3)
#define UADP_SDT_4_DEF(TYPE)                                                                                                        UADP_SDT_NAMEn_DEF(TYPE,4)
#define UADP_SDT_5_DEF(TYPE)                                                                                                        UADP_SDT_NAMEn_DEF(TYPE,5)
#define UADP_SDT_7_DEF(TYPE)                                                             											UADP_SDT_NAMEn_DEF(TYPE,7)
                                                                                                 
#define UADP_SDT_0(TYPE)                                                                                                            UADP_SDT_NAME0(TYPE,TYPE)
#define UADP_SDT_1(TYPE,MB1KIND,MB1,MB1TYPE)                                                                                        UADP_SDT_NAME1(TYPE,TYPE,MB1KIND,MB1,MB1TYPE)
#define UADP_SDT_2(TYPE,MB1KIND,MB1,MB1TYPE,MB2KIND,MB2,MB2TYPE)                                                                    UADP_SDT_NAME2(TYPE,TYPE,MB1KIND,MB1,MB1TYPE,MB2KIND,MB2,MB2TYPE)
#define UADP_SDT_3(TYPE,MB1KIND,MB1,MB1TYPE,MB2KIND,MB2,MB2TYPE,MB3KIND,MB3,MB3TYPE)                                                UADP_SDT_NAME3(TYPE,TYPE,MB1KIND,MB1,MB1TYPE,MB2KIND,MB2,MB2TYPE,MB3KIND,MB3,MB3TYPE)
#define UADP_SDT_4(TYPE,MB1KIND,MB1,MB1TYPE,MB2KIND,MB2,MB2TYPE,MB3KIND,MB3,MB3TYPE,MB4KIND,MB4,MB4TYPE)                            UADP_SDT_NAME4(TYPE,TYPE,MB1KIND,MB1,MB1TYPE,MB2KIND,MB2,MB2TYPE,MB3KIND,MB3,MB3TYPE,MB4KIND,MB4,MB4TYPE)
#define UADP_SDT_5(TYPE,MB1KIND,MB1,MB1TYPE,MB2KIND,MB2,MB2TYPE,MB3KIND,MB3,MB3TYPE,MB4KIND,MB4,MB4TYPE,MB5KIND,MB5,MB5TYPE)        UADP_SDT_NAME5(TYPE,TYPE,MB1KIND,MB1,MB1TYPE,MB2KIND,MB2,MB2TYPE,MB3KIND,MB3,MB3TYPE,MB4KIND,MB4,MB4TYPE,MB5KIND,MB5,MB5TYPE)
#define UADP_SDT_7(TYPE,MB1KIND,MB1,MB1TYPE,MB2KIND,MB2,MB2TYPE,MB3KIND,MB3,MB3TYPE,MB4KIND,MB4,MB4TYPE,MB5KIND,MB5,MB5TYPE,MB6KIND,MB6,MB6TYPE,MB7KIND,MB7,MB7TYPE)        UADP_SDT_NAME7(TYPE,TYPE,MB1KIND,MB1,MB1TYPE,MB2KIND,MB2,MB2TYPE,MB3KIND,MB3,MB3TYPE,MB4KIND,MB4,MB4TYPE,MB5KIND,MB5,MB5TYPE,MB6KIND,MB6,MB6TYPE,MB7KIND,MB7,MB7TYPE)
//-------------------------------------------------------------------------------------------------

//in/out in kernel_argc, !!!!output need compatible into input structure!!!!!!
int UADPBypassIoctl(void* pInstance, unsigned int cmd, void* arg,const UADP_STRUCT_POINTER_TABLE *pInTable,const UADP_STRUCT_POINTER_TABLE *pOutTable, void* pWorkBuffer, int iWorkBufferSize);
int UADPBypassSetSPT(UADP_STRUCT_POINTER_TABLE* pSpt, unsigned int uiType, unsigned int uiOffset, unsigned long uiSizeOrNext);

char* UADPKQueuePop(MS_U32 module_ID);

void UADPKQueuePush(MS_U8 module_ID,char * pointer);

int UADPKQueueWaitThreadNum(MS_U8 module_ID);

int UADPKQueueDataNum(MS_U8 module_ID);
int UADPKQueueIsempty(MS_U8 module_ID);

void UADPCreateKQueue(MS_U8 module_ID);
void UADPDeleteQueue(MS_U8 module_ID);

#endif /* _UTOPIA_ADP_H_ */

