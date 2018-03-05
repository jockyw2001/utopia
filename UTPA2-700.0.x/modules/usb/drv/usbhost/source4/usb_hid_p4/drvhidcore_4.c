//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
#if defined(MSOS_TYPE_NOS)
#include "MsVersion.h"
#include <stdio.h>
#include <string.h>
#include "../USBHostConfig_4.h"
#include "../../include/hal_arch.h"
#include "../../include/hal_cache.h"

#include "../../include/datatype.h"

#include "../../include/_drvUSB.h"
#include "../drvhid_4.h"
#include "../drvHostLib_4.h"
#include "../drvUsbMain_4.h"

#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"

#define USB_HID_DBG(x) // x;
#define USB_HID_MSG(x)  MS_CRITICAL_MSG(x)

#if 0
U8 code USBHID_P4_VBuf[32] = {'M', 'S', 'V', 'C', '0', '0',			    // 0, 0 fixed
                        'U', '3',						            // Library ID

                        'A', '0', '0', '1', '0', '2',				// build number

                        '0', '0', '0', '0', '0', '0', '0', '0',		// change list 46058
                        '0', '0', '0', '0', '0', '0', '0', '0', '0',// A4: Saturn, A:LG
                        'T'};
#endif

#define USB_HID_P4_DRV_VERSION                  /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    {'U','L','H','4'},                  /* IP__                                             */  \
    {'0','1'},                          /* 0.0 ~ Z.Z                                        */  \
    {'0','3'},                          /* 00 ~ 99                                          */  \
    {'0','0','2','5','9','5','9','0'},  /* CL#                                              */  \
    MSIF_OS

static MSIF_Version _drv_usb_hid_p4_version = {
    .DDI = { USB_HID_P4_DRV_VERSION },
};

/*
 * Read data value from item.
 */
U8 UsbIntrData_Port4[16] __attribute__ ((aligned (16))) ;
U8 *pUsbIntrData_Port4;
extern UINT8  pUsbData_Port4[1024];
struct   axis_struct  axis_Port4[MAX_AXIS_NUM];
__u8     axis_idx_Port4;
struct   button_struct  buttons_Port4;
__u16  gOffsetBit_Port4;

U8 MDrv_USB_HID_GetLibVer_Port4(const MSIF_Version **ppVersion)
{
    if(!ppVersion)
        return 1;

    *ppVersion = &_drv_usb_hid_p4_version;
    return 0;
}

__u32 item_udata_Port4(struct hid_item *item)
{
//    DO_PRINT("@@@ >>> Enter hid-core.c file --> item_udata function \n");

	switch (item->size) {
		case 1: return item->zdata.u8;
		case 2: return item->zdata.u16;
		case 4: return item->zdata.u32;
	}
	return 0;
}

static  __s32 item_sdata_Port4(struct hid_item *item)
{
    //DO_PRINT("@@@ >>> Enter hid-core.c file --> item_sdata function \n");

	switch (item->size) {
		case 1: return item->zdata.s8;
		case 2: return item->zdata.s16;
		case 4: return item->zdata.s32;
	}
	return 0;
}


/*
 * Parse a report description into a hid_device structure. Reports are
 * enumerated, fields are attached to these reports.
 */


static __u8 *fetch_item_Port4(__u8 *start, __u8 *end, struct hid_item *item)
{
    //printf("@@@ >>> Enter hid-core.c file --> fetch_item function \n");

       //printf("start: %X, end: %X ", start, end);
	if ((end - start) > 0) {

		__u8 b = *start++;
             //printf("b:%02X ",b);
		item->type = (b >> 2) & 3;
             //printf("type: %X ", item->type);
		item->tag  = (b >> 4) & 15;
             //printf("tag:%02X ",item->tag);

		if (item->tag == HID_ITEM_TAG_LONG) {

			item->format = HID_ITEM_FORMAT_LONG;

			if ((end - start) >= 2) {

				item->size = *start++;
				item->tag  = *start++;

				if ((end - start) >= item->size) {
					item->zdata.longdata = start;
					start += item->size;
					return start;
				}
			}
		} else {

			item->format = HID_ITEM_FORMAT_SHORT;
			item->size = b & 3;
                    //printf("size:%02X ",item->size);
			switch (item->size) {

				case 0:
                                  //printf("\n");
					return start;

				case 1:
					if ((end - start) >= 1) {
						item->zdata.u8 = *start++;
                                        //printf("U8:%02X\n",item->zdata.u8);
                                        //  printf("return %x\n",(U16)start);

						return start;
					}
					break;

				case 2:
					if ((end - start) >= 2) {
						//item->zdata.u16 = *((__u16*)start);
						item->zdata.u16 = (((U16)*(start+1))<<8) | *start;
                                        //printf("U16:%02X\n",item->zdata.u16);
                                        start+=2;
						return start;
					}
					break;

				case 3:
					item->size++;
					if ((end - start) >= 4) {
						//item->zdata.u32 = *((__u32*)start);
						item->zdata.u32 = (((U32)*(start+3))<<24) | (((U32)*(start+2))<<16) |
						                            (((U32)*(start+1))<<8) | *start;
                                        //printf("U32:%02X\n",item->zdata.u32);
                                        start+=4;
						return start;
					}
					break;
			}
		}
	}
	return NULL;
}

static int open_collection_Port4(struct hid_parser *parser, unsigned type)
{
	struct hid_collection *collection;
	unsigned usage;

	usage = parser->local.usage[0];

	if (parser->collection_stack_ptr == HID_COLLECTION_STACK_SIZE) {
		USB_HID_MSG(printf("collection stack overflow\n"));
		return -1;
	}

	collection = parser->collection_stack + parser->collection_stack_ptr++;
	collection->type = type;
	collection->usage = usage;

	return 0;
}

static int close_collection_Port4(struct hid_parser *parser)
{
	if (!parser->collection_stack_ptr) {
		USB_HID_MSG(printf("collection stack underflow\n"));
		return -1;
	}
	parser->collection_stack_ptr--;
	return 0;
}

static int hid_chk_variable_Port4(struct hid_parser *parser, unsigned report_type)
{
    U8  ii, usg_count, rpt_count;

    usg_count = parser->local.usage_index;
    rpt_count = parser->global.report_count;

    USB_HID_DBG(printf("hid_chk_variable: type: %X\n", report_type));
    USB_HID_DBG(printf("usage count: %X, size: %X, count: %X\n",
            usg_count, parser->global.report_size, rpt_count));

    if (parser->global.usage_page == USEAGE_PAGE_BUTTON)
    {
        buttons_Port4.startbit=gOffsetBit_Port4;
        buttons_Port4.count=parser->global.report_size * rpt_count;
        USB_HID_DBG(printf("Usage Page (buttons): %X size: %X\n", buttons_Port4.startbit, buttons_Port4.count));

        gOffsetBit_Port4+=buttons_Port4.count;
        goto Func_Done;
    }

    if (usg_count > 0)
    {
        for(ii=usg_count-rpt_count; ii<usg_count; ii++)
        {
            USB_HID_DBG(printf("local usage %X: %X\n", ii, parser->local.usage[ii]));
            if (report_type == HID_INPUT_REPORT)
            {
                if (parser->local.usage[ii] == USEAGE_X)
                {
                     axis_Port4[axis_idx_Port4].startbit=gOffsetBit_Port4;
                     axis_Port4[axis_idx_Port4].size=parser->global.report_size;
                     axis_Port4[axis_idx_Port4].type=TYPE_AXIS_X;
                     gOffsetBit_Port4+=axis_Port4[axis_idx_Port4].size;
                     USB_HID_DBG(printf("Usage(X): %X, size: %X\n",axis_Port4[axis_idx_Port4].startbit, axis_Port4[axis_idx_Port4].size));

                     axis_idx_Port4++;
                }
                else if (parser->local.usage[ii] == USEAGE_Y)
                {
                     axis_Port4[axis_idx_Port4].startbit=gOffsetBit_Port4;
                     axis_Port4[axis_idx_Port4].size=parser->global.report_size;
                     axis_Port4[axis_idx_Port4].type=TYPE_AXIS_Y;
                     gOffsetBit_Port4+=axis_Port4[axis_idx_Port4].size;
                     USB_HID_DBG(printf("Usage(Y): %X, size: %X\n",axis_Port4[axis_idx_Port4].startbit, axis_Port4[axis_idx_Port4].size));

                     axis_idx_Port4++;
                }
                else
                {
                    USB_HID_DBG(printf("Unsupport usage: %X, size: %X\n", gOffsetBit_Port4, parser->global.report_size));
                    gOffsetBit_Port4+= parser->global.report_size;
                }
            }
            else
            {
                USB_HID_DBG(printf("Unsupport usage: %X, size: %X\n", gOffsetBit_Port4, parser->global.report_size));
                gOffsetBit_Port4 += parser->global.report_size;
            }
        }
    }
    else
    {
        USB_HID_DBG(printf("Unsupport data: %X, size: %X\n", gOffsetBit_Port4, parser->global.report_size * rpt_count));
        gOffsetBit_Port4 += parser->global.report_size * rpt_count;
    }

Func_Done:
    return 0;
}

/*
 * Process a main item.
 */
 int hid_parser_main_Port4(struct hid_parser *parser, struct hid_item *item)
{
	__u32 adata;
	int ret=0;

   // printf("  hid_parser_main   \n");

	adata = item_udata_Port4(item);
      // printf("TAG:%02bx\n",item->tag);
	switch (item->tag) {
		case HID_MAIN_ITEM_TAG_BEGIN_COLLECTION:
#if 0
                     if (adata==HID_COLLECTION_APPLICATION)
                        {
				//printf("application\n");
                            gOffsetBit=0;
                            axis_idx=0;
                            for (i=0 ; i < MAX_AXIS_NUM ; i ++)
                            {
                                axis[i].size=0;
                                axis[i].val=0;
                            }
                        }
#endif
			ret = open_collection_Port4(parser, adata & 3);
			break;
		case HID_MAIN_ITEM_TAG_END_COLLECTION:
			ret = close_collection_Port4(parser);
			break;
		case HID_MAIN_ITEM_TAG_INPUT:
                        ret = hid_chk_variable_Port4(parser, HID_INPUT_REPORT);
			//ret = hid_add_field(parser, HID_INPUT_REPORT, data);
			break;
		case HID_MAIN_ITEM_TAG_OUTPUT:
                        ret = hid_chk_variable_Port4(parser, HID_OUTPUT_REPORT);
			//ret = hid_add_field(parser, HID_OUTPUT_REPORT, data);
			break;
		case HID_MAIN_ITEM_TAG_FEATURE:
                        ret = hid_chk_variable_Port4(parser, HID_FEATURE_REPORT);
		//	ret = hid_add_field(parser, HID_FEATURE_REPORT, data);
			break;
		default:
			//printf("unknown main item tag 0x%02bx", item->tag);
			ret = 0;
	}

	memset(&parser->local, 0, sizeof(parser->local));	/* Reset the local parser environment */

	return ret;
}


/*
 * Process a global item.
 */

 int hid_parser_global_Port4(struct hid_parser *parser, struct hid_item *item)
{
   // printf("  hid_parser_global   \n");
	switch (item->tag) {

		case HID_GLOBAL_ITEM_TAG_PUSH:

			//if (parser->global_stack_ptr == HID_GLOBAL_STACK_SIZE) {
			//	printf("global enviroment stack overflow");
				//return -1;
			//}

			//memcpy(parser->global_stack + parser->global_stack_ptr++,
				//&parser->global, sizeof(struct hid_global));
			return 0;

		case HID_GLOBAL_ITEM_TAG_POP:

			//if (!parser->global_stack_ptr) {
			//	printf("global enviroment stack underflow");
			//	return -1;
			//}

			//memcpy(&parser->global, parser->global_stack + --parser->global_stack_ptr,
				//sizeof(struct hid_global));
			return 0;

		case HID_GLOBAL_ITEM_TAG_USAGE_PAGE:
			parser->global.usage_page = item_udata_Port4(item);

                    USB_HID_DBG(printf("Find a usage page: %lX\n", parser->global.usage_page));
#if 0
                     if (parser->global.usage_page==USEAGE_PAGE_BUTTON)
                        {
                            buttons.startbit=gOffsetBit;
                            printf("buttons start:%x\n",buttons.startbit);
                            buttons.count=parser->global.report_count;
                            printf("buttons count:%02bx\n",buttons.count);

                            gOffsetBit+=buttons.count;
                        }
#endif
			return 0;

		case HID_GLOBAL_ITEM_TAG_LOGICAL_MINIMUM:
			parser->global.logical_minimum = item_sdata_Port4(item);
			return 0;

		case HID_GLOBAL_ITEM_TAG_LOGICAL_MAXIMUM:
			if (parser->global.logical_minimum < 0)
				parser->global.logical_maximum = item_sdata_Port4(item);
			else
				parser->global.logical_maximum = item_udata_Port4(item);
			return 0;

		case HID_GLOBAL_ITEM_TAG_PHYSICAL_MINIMUM:
			parser->global.physical_minimum = item_sdata_Port4(item);
			return 0;

		case HID_GLOBAL_ITEM_TAG_PHYSICAL_MAXIMUM:
			if (parser->global.physical_minimum < 0)
				parser->global.physical_maximum = item_sdata_Port4(item);
			else
				parser->global.physical_maximum = item_udata_Port4(item);
			return 0;

		case HID_GLOBAL_ITEM_TAG_UNIT_EXPONENT:
			parser->global.unit_exponent = item_udata_Port4(item);
			return 0;

		case HID_GLOBAL_ITEM_TAG_UNIT:
			parser->global.unit = item_udata_Port4(item);
			return 0;

		case HID_GLOBAL_ITEM_TAG_REPORT_SIZE:
			if ((parser->global.report_size = item_udata_Port4(item)) > 32) {
			//	printf("invalid report_size %d", parser->global.report_size);
				return -1;
			}
			return 0;

		case HID_GLOBAL_ITEM_TAG_REPORT_COUNT:
			if ((parser->global.report_count = item_udata_Port4(item)) > HID_MAX_USAGES) {
				//printf("invalid report_count %d", parser->global.report_count);
				return -1;
			}
			return 0;

		case HID_GLOBAL_ITEM_TAG_REPORT_ID:
			if ((parser->global.report_id = item_udata_Port4(item)) == 0) {
				//printf("report_id 0 is invalid");
				return -1;
			}
			return 0;

		default:
			//printf("unknown global tag 0x%02bx", item->tag);
			return -1;
	}
}

 int hid_parser_local_Port4(struct hid_parser *parser, struct hid_item *item)
{
	__u32 adata;

      //printf("  hid_parser_local   \n");

	if (item->size == 0) {
		//printf("item data expected for local item");
		return -1;
	}

	adata = item_udata_Port4(item);
      //printf("local tag:%02bx\n",item->tag);
	switch (item->tag) {

		case HID_LOCAL_ITEM_TAG_DELIMITER:

			if (adata) {
				/*
				 * We treat items before the first delimiter
				 * as global to all usage sets (branch 0).
				 * In the moment we process only these global
				 * items and the first delimiter set.
				 */
				if (parser->local.delimiter_depth != 0) {
					//printf("nested delimiters");
					return -1;
				}
				parser->local.delimiter_depth++;
				parser->local.delimiter_branch++;
			} else {
				if (parser->local.delimiter_depth < 1) {
					//printf("bogus close delimiter");
					return -1;
				}
				parser->local.delimiter_depth--;
			}
			return 0;

		case HID_LOCAL_ITEM_TAG_USAGE:

			if (parser->local.delimiter_branch > 1) {
				//printf("alternative usage ignored");
				return 0;
			}

			//if (item->size <= 2)
				//adata = (parser->global.usage_page << 16) + adata;
			USB_HID_DBG(printf("Find a usage: idx: %X, val: %lX\n", parser->local.usage_index, adata));
                    parser->local.usage[parser->local.usage_index++] = adata;
                  //   printf("useage:%lx\n",adata);
#if 0
                     if (adata==USEAGE_X)
                        {
                             axis[axis_idx].startbit=gOffsetBit;
                             axis[axis_idx].size=parser->global.report_size;
                             axis[axis_idx].type=TYPE_AXIS_X;
                             gOffsetBit+=axis[axis_idx].size;
                             printf("Usage(X):%x\n",axis[axis_idx].startbit);

                             axis_idx++;
                        }
                     else if (adata==USEAGE_Y)
                        {
                             axis[axis_idx].startbit=gOffsetBit;
                             axis[axis_idx].size=parser->global.report_size;
                             axis[axis_idx].type=TYPE_AXIS_Y;
                             gOffsetBit+=axis[axis_idx].size;
                             printf("Usage(Y):%x\n",axis[axis_idx].startbit);

                             axis_idx++;
                        }
                     else if (adata==USEAGE_UNDEFINED)
                        {
                            printf("undefine usage :%02bx\n",parser->global.report_size);
                            gOffsetBit+=parser->global.report_size;
                        }
#endif
			return 0;// hid_add_usage(parser, data);

		case HID_LOCAL_ITEM_TAG_USAGE_MINIMUM:

			if (parser->local.delimiter_branch > 1) {
				//printf("alternative usage ignored");
				return -1;
			}

			if (item->size <= 2)
				adata = (parser->global.usage_page << 16) + adata;

			parser->local.usage_minimum = adata;
                    //  printf("logical minimum:%02bx\n",adata);
			return 0;

		case HID_LOCAL_ITEM_TAG_USAGE_MAXIMUM:

			if (parser->local.delimiter_branch > 1) {
				//printf("alternative usage ignored");
				return -1;
			}

			if (item->size <= 2)
				adata = (parser->global.usage_page << 16) + adata;
                    //  printf("logical maximum:%02bx\n",adata);

			//for (n = parser->local.usage_minimum; n <= data; n++)
				//if (hid_add_usage(parser, n)) {
				//	printf("hid_add_usage failed\n");
				//	return -1;
				//}
			return 0;

		default:

			//printf("unknown local item tag 0x%x", item->tag);
			return -1;
	}
	return 0;
}

void hid_parse_report_Port4(__u8 *start, unsigned size)
{
	__u8 *end;
       int result = 0;
      struct hid_item xdata gitem;
     struct hid_parser xdata gparser;


  // printf("@@@ >>> Enter hid-core.c file --> hid_parse_report function \n");

	memset((void*)&gparser, 0, sizeof(struct hid_parser));
	//parser->device = device;

	end = start + size;
	while ((start = fetch_item_Port4(start, end, &gitem)) != 0)
        {
//            printf("gitem: start: %X, fmt: %X, size: %d, type: %X, tag: %X data: %X\n",
//                start, gitem.format, gitem.size, gitem.type, gitem.tag, gitem.zdata.u8);

		if (gitem.format != HID_ITEM_FORMAT_SHORT)
             {
			printf("unexpected long global item");
			return ;
		}
        if (gitem.type==0)
            result=hid_parser_main_Port4(&gparser,&gitem);
        else if (gitem.type==1)
            result=hid_parser_global_Port4(&gparser,&gitem);
        else if (gitem.type==2)
            result=hid_parser_local_Port4(&gparser,&gitem);

		if (result)
             {
			//printf("item %u %u %u %u parsing failed\n",
				//gitem.format, (unsigned)gitem.size, (unsigned)gitem.type, (unsigned)gitem.tag);
			//hid_free_device(device);
			//kfree(parser);
			return ;
		}

		if (start == end) {
		//	if (parser.collection_stack_ptr) {
				//printf("unbalanced collection at end of report description");
				//return ;
			//}
			if (gparser.local.delimiter_depth) {
				//printf("unbalanced delimiter at end of report description");
				return ;
			}
			return;
		}
	}

	//printf("item fetching failed at offset %d\n", (int)(end - start));
	return;
}
U16 Parse_Joystick_Report_Port4(__u8 *report)
{
__u8 i,tmp;
__u16  tmp2,result;
 //printf("t1");

         //   for (i=0 ; i < 8 ; i++)
		//printf("%02bx ", report[i]);
            for (i=0 ; i < MAX_AXIS_NUM ; i ++)
           {
               if (axis_Port4[i].size)                //this AXIS exist
                {
                        tmp=GetValue_Port4(report,axis_Port4[i].startbit,axis_Port4[i].size);
                        if (axis_Port4[i].val!=tmp )
                            {
                                 USB_HID_DBG(printf(" axis idx %x, offset:%x ,val %02x  tmp:%02x\n", i,axis_Port4[i].startbit,axis_Port4[i].val,tmp));
                                 axis_Port4[i].val=tmp;
                                 result=(axis_Port4[i].type << 12) | tmp;
                                 return result;

                            }
                }

            }
// printf("t2");
		//printf("button start:%x",buttons.startbit);
            tmp2=GetValue_Port4(report,buttons_Port4.startbit,buttons_Port4.count);
           // printf("tmp2:%x\n",tmp2);
            if (buttons_Port4.val!=tmp2)
                {
                    USB_HID_DBG(printf("buttons:%x %x\n",buttons_Port4.val, tmp2));
                    buttons_Port4.val=tmp2;
                    tmp2 &=0x0fff;              //only maximum 12 bits allowed
                    result=(TYPE_BUTTON << 12) | tmp2;
                    return result;

                }
      return 0;         //no event
}
__u32 GetValue_Port4(__u8 *buf, __u16 offset,__u8 size)
{

        __u8 *ptr;
        __u32  x,y;
        __u8 i;

        ptr=buf+offset/8;
        x= *ptr + (*(ptr+1)<<8) + (*(ptr+2)<< 16) + (*(ptr+3) <<24);
        x>>=offset % 8;
        y=0;
        for (i=0 ; i < size ; i++)
       {
            y=y << 1;
            y|=1;

       }
        x= x & y;       //mask unwanted bits
        return x;
}
code UINT8  GET_REPORT_DESCRIPTOR_PORT4[]        = {0x81,0x06,0x00,0x22,0x00,0x00,0x65,0x00};

BOOLEAN GetReportDescriptor_Port4(void)
{
    UINT8 bCMD[8];
    UINT8 i;

   memcpy(bCMD,GET_REPORT_DESCRIPTOR_PORT4,8);
   if ( flib_Host20_Issue_Control_Port4 (1,bCMD,0x65,pUsbData_Port4)>0)
       return FALSE;

    gOffsetBit_Port4=0;
    axis_idx_Port4=0;
    for (i=0 ; i < MAX_AXIS_NUM ; i++)
    {
        axis_Port4[i].size=0;
        axis_Port4[i].val=0;
    }

    buttons_Port4.count = 0;
    buttons_Port4.startbit = 0;
    buttons_Port4.val = 0;

   hid_parse_report_Port4(pUsbData_Port4, 0x65);

   return TRUE;

}
U8 drvUSBHost_HID_Initial_Port4(void)
{
    USB_HID_DBG(printf("HID device\n"));

    GetReportDescriptor_Port4();

    pUsbIntrData_Port4=(U8*) KSEG02KSEG1(UsbIntrData_Port4);
    MY_HAL_DCACHE_FLUSH((U32)UsbIntrData_Port4, 16);
    flib_Host20_Interrupt_Init_Port4(3);

    return 0;
}
U16  MDrv_GET_JOYSTICK_STATUS_Port4(void)
{
    __u16 tmp=0;

    //printf("x1");

    memset(pUsbIntrData_Port4, 0, 8);
     if (flib_Host20_Issue_Interrupt_Port4((U32)pUsbIntrData_Port4,8)==HOST20_OK)
         tmp=Parse_Joystick_Report_Port4(pUsbIntrData_Port4);
// printf("x3");

     return tmp;
}

U8  ReportData_Port4[8];
U8*  MDrv_Get_HID_Report_Raw_Data_Port4(void)
{
    U8  i;

    memset(ReportData_Port4, 0, 8);
    if (flib_Host20_Issue_Interrupt_Port4((U32)pUsbIntrData_Port4,8)==HOST20_OK)
    {
        for (i=0 ; i <8 ; i++)
        {
            ReportData_Port4[i]=pUsbIntrData_Port4[i];
            // printf("%02bx ",ReportData[i]);
        }
    }

    return ReportData_Port4;
}
#endif //#if defined(MSOS_TYPE_NOS)


