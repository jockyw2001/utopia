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
 typedef U8   __u8;     // 1 byte
 typedef  S8   __s8;     // 1 byte

/// data type unsigned short, data length 2 byte
 typedef U16  __u16;    // 2 bytes
 typedef  S16  __s16;    // 2 bytes

/// data type unsigned int, data length 4 byte
typedef U32    __u32;    // 4 bytes
typedef  S32    __s32;    // 4 bytes

/// data type signed char, data length 1 byte


/*
 * HID report item format
 */

#define HID_ITEM_FORMAT_SHORT	0
#define HID_ITEM_FORMAT_LONG	1

/*
 * Special tag indicating long items
 */

#define HID_ITEM_TAG_LONG	15

/*
 * HID report descriptor item type (prefix bit 2,3)
 */

#define HID_ITEM_TYPE_MAIN		0
#define HID_ITEM_TYPE_GLOBAL		1
#define HID_ITEM_TYPE_LOCAL		2
#define HID_ITEM_TYPE_RESERVED		3

/*
 * HID report descriptor main item tags
 */

#define HID_MAIN_ITEM_TAG_INPUT			8
#define HID_MAIN_ITEM_TAG_OUTPUT		9
#define HID_MAIN_ITEM_TAG_FEATURE		11
#define HID_MAIN_ITEM_TAG_BEGIN_COLLECTION	10
#define HID_MAIN_ITEM_TAG_END_COLLECTION	12

/*
 * HID report descriptor main item contents
 */

#define HID_MAIN_ITEM_CONSTANT		0x001
#define HID_MAIN_ITEM_VARIABLE		0x002
#define HID_MAIN_ITEM_RELATIVE		0x004
#define HID_MAIN_ITEM_WRAP		0x008
#define HID_MAIN_ITEM_NONLINEAR		0x010
#define HID_MAIN_ITEM_NO_PREFERRED	0x020
#define HID_MAIN_ITEM_NULL_STATE	0x040
#define HID_MAIN_ITEM_VOLATILE		0x080
#define HID_MAIN_ITEM_BUFFERED_BYTE	0x100

/*
 * HID report descriptor collection item types
 */

#define HID_COLLECTION_PHYSICAL		0
#define HID_COLLECTION_APPLICATION	1
#define HID_COLLECTION_LOGICAL		2

/*
 * HID report descriptor global item tags
 */

#define HID_GLOBAL_ITEM_TAG_USAGE_PAGE		0
#define HID_GLOBAL_ITEM_TAG_LOGICAL_MINIMUM	1
#define HID_GLOBAL_ITEM_TAG_LOGICAL_MAXIMUM	2
#define HID_GLOBAL_ITEM_TAG_PHYSICAL_MINIMUM	3
#define HID_GLOBAL_ITEM_TAG_PHYSICAL_MAXIMUM	4
#define HID_GLOBAL_ITEM_TAG_UNIT_EXPONENT	5
#define HID_GLOBAL_ITEM_TAG_UNIT		6
#define HID_GLOBAL_ITEM_TAG_REPORT_SIZE		7
#define HID_GLOBAL_ITEM_TAG_REPORT_ID		8
#define HID_GLOBAL_ITEM_TAG_REPORT_COUNT	9
#define HID_GLOBAL_ITEM_TAG_PUSH		10
#define HID_GLOBAL_ITEM_TAG_POP			11

/*
 * HID report descriptor local item tags
 */

#define HID_LOCAL_ITEM_TAG_USAGE		0
#define HID_LOCAL_ITEM_TAG_USAGE_MINIMUM	1
#define HID_LOCAL_ITEM_TAG_USAGE_MAXIMUM	2
#define HID_LOCAL_ITEM_TAG_DESIGNATOR_INDEX	3
#define HID_LOCAL_ITEM_TAG_DESIGNATOR_MINIMUM	4
#define HID_LOCAL_ITEM_TAG_DESIGNATOR_MAXIMUM	5
#define HID_LOCAL_ITEM_TAG_STRING_INDEX		7
#define HID_LOCAL_ITEM_TAG_STRING_MINIMUM	8
#define HID_LOCAL_ITEM_TAG_STRING_MAXIMUM	9
#define HID_LOCAL_ITEM_TAG_DELIMITER		10

/*
 * HID usage tables
 */

#define HID_USAGE_PAGE		0xffff0000

#define HID_UP_GENDESK 		0x00010000
#define HID_UP_KEYBOARD 	0x00070000
#define HID_UP_LED 		0x00080000
#define HID_UP_BUTTON 		0x00090000
#define HID_UP_CONSUMER		0x000c0000
#define HID_UP_DIGITIZER 	0x000d0000
#define HID_UP_PID 		0x000f0000

#define HID_USAGE		0x0000ffff

#define HID_GD_POINTER		0x00010001
#define HID_GD_MOUSE		0x00010002
#define HID_GD_JOYSTICK		0x00010004
#define HID_GD_GAMEPAD		0x00010005
#define HID_GD_HATSWITCH	0x00010039

/*
 * HID report types --- Ouch! HID spec says 1 2 3!
 */

#define HID_INPUT_REPORT	0
#define HID_OUTPUT_REPORT	1
#define HID_FEATURE_REPORT	2

/*
 * HID device quirks.
 */

#define HID_QUIRK_INVERT	0x01
#define HID_QUIRK_NOTOUCH	0x02
#define HID_QUIRK_IGNORE	0x04
#define HID_QUIRK_NOGET		0x08

#define  USEAGE_X                   0x30
#define  USEAGE_Y                   0x31
#define  USEAGE_Z                   0x32
#define  USEAGE_UNDEFINED   0
#define  MAX_AXIS_NUM          6

#define  USEAGE_PAGE_BUTTON         9
/*
 * We parse each description item into this structure. Short items data
 * values are expanded to 32-bit signed int, long items contain a pointer
 * into the data area.
 */

struct hid_item {
	__u8      format;
	__u8      size;
	__u8      type;
	__u8      tag;
	union {
	    __u8   u8;
	    __s8   s8;
	    __u16  u16;
	    __s16  s16;
	    __u32  u32;
	    __s32  s32;
 	    __u8  *longdata;
	} zdata;
};
struct hid_global {
	__u32        usage_page;
	__s32       logical_minimum;
	__s32       logical_maximum;
	__s32       physical_minimum;
	__s32       physical_maximum;
	__u8        unit_exponent;
	__u8        unit;
	__u8        report_id;
	__u8        report_size;
	__u8        report_count;
};
/*
 * This is the collection stack. We climb up the stack to determine
 * application and function of each field.
 */

struct hid_collection {
	__u8    type;
	__u8    usage;
};


/*
 * This is the local enviroment. It is resistent up the next main-item.
 */

#define HID_MAX_DESCRIPTOR_SIZE		4096
#define HID_MAX_USAGES			16
#define HID_MAX_APPLICATIONS		16

struct hid_local {
	__u8    usage[HID_MAX_USAGES]; /* usage array */
	__u8    usage_index;
	__u8    usage_minimum;
	__u8    delimiter_depth;
	__u8    delimiter_branch;
};

#define HID_GLOBAL_STACK_SIZE 4
#define HID_COLLECTION_STACK_SIZE 4
struct hid_parser {
	struct hid_global     global;
	struct hid_global     global_stack[HID_GLOBAL_STACK_SIZE];
	unsigned              global_stack_ptr;
	struct hid_local      local;
	struct hid_collection collection_stack[HID_COLLECTION_STACK_SIZE];
	unsigned              collection_stack_ptr;
	//struct hid_device    *device;
};
#define   TYPE_NOEVENT        0
#define   TYPE_AXIS_X           1
#define   TYPE_AXIS_Y           2
#define   TYPE_BUTTON          3

struct axis_struct{
        __u8   size;
        __u8    val;
        __u16  startbit;
        __u8    type;
};
struct  button_struct {                 //for all one bit button
        __u8   count;
        __u16    val;
        __u16  startbit;
};

__u32 GetValue_Port4(__u8 *buf, __u16 offset,__u8 size);

