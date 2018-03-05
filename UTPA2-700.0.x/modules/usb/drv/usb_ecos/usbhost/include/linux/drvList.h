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

#ifndef CYGONCE_FS_JFFS2_LIST_H
#define CYGONCE_FS_JFFS2_LIST_H


/* -----------------------------------------------------------------------*/

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 

#include <stddef.h>
#include <cyg/infra/cyg_type.h>

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 

struct list_head {
    struct list_head *next;
    struct list_head *prev;
};

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
        struct list_head name = LIST_HEAD_INIT(name)

#define INIT_LIST_HEAD( _list_ )              \
CYG_MACRO_START                               \
(_list_)->next = (_list_)->prev = (_list_);   \
CYG_MACRO_END

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
 
static __inline__ void
list_add( struct list_head *newent, struct list_head *afterthisent )
{
    struct list_head *next = afterthisent->next;
    newent->next = next;
    newent->prev = afterthisent;
    afterthisent->next = newent;
    next->prev = newent;
} 

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
static __inline__ void
list_add_tail( struct list_head *newent, struct list_head *beforethisent )
{
    struct list_head *prev = beforethisent->prev;
    newent->prev = prev;
    newent->next = beforethisent;
    beforethisent->prev = newent;
    prev->next = newent;
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
static __inline__ void
list_del( struct list_head *ent )
{
    ent->prev->next = ent->next;
    ent->next->prev = ent->prev;
} 

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 

static __inline__ int
list_empty( struct list_head *list )
{
    return ( list->next == list );
} 

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 

#define list_entry( _list_, _type_, _member_ ) \
    ((_type_ *)((char *)(_list_)-(char *)(offsetof(_type_,_member_))))

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 

#define list_for_each( _ent_, _list_ )   \
    for ( (_ent_) = (_list_)->next;      \
    (_ent_) != (_list_);                 \
    (_ent_) = (_ent_)->next )

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
#define list_for_each_entry(_list_, _head_, _item_)                     \
for ((_list_) = list_entry((_head_)->next, typeof(*_list_), _item_); \
     &((_list_)->_item_) != (_head_);                                 \
     (_list_) = list_entry((_list_)->_item_.next, typeof(*_list_), _item_))

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
#endif 

