////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2008 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (; MStar; Confidential; Information; ) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//

#ifndef _MOONEY_BANDWIDTH_REGTABLE_H_
#define _MOONEY_BANDWIDTH_REGTABLE_H_

typedef enum
{
    BWTABLE_Normal_mode,
    BWTABLE_1080p_mode,
    BWTABLE_HDMI4kx2k_RGB_BwdHigh_mode,
    BWTABLE_HDMI4kx2k_RGB_BwdMed_mode,
    BWTABLE_HDMI4kx2k_RGB_BwdIdle_mode,
    BWTABLE_HDMI4kx2k_YUV444_BwdHigh_mode,
    BWTABLE_HDMI4kx2k_YUV444_BwdMed_mode,
    BWTABLE_HDMI4kx2k_YUV444_BwdIdle_mode,
    BWTABLE_HDMI4kx2k_YUV422_BwdHigh_mode,
    BWTABLE_HDMI4kx2k_YUV422_BwdMed_mode,
    BWTABLE_HDMI4kx2k_YUV422_BwdIdle_mode,
    BWTABLE_dummy_mode,
    BWTABLE_NUMS
}
BWTABLE_Group;

#endif
