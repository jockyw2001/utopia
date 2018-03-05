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

#include "MsCommon.h"
#define ehci_dbg(ehci, fmt, ...) \
  dev_dbg ((ehci)->hcd.controller , fmt , __VA_ARGS__ )
#define ehci_err(ehci, fmt, ...) \
  dev_err ((ehci)->hcd.controller , fmt , __VA_ARGS__ )
#define ehci_info(ehci, fmt, ...) \
  dev_info ((ehci)->hcd.controller , fmt , __VA_ARGS__ )
#define ehci_warn(ehci, fmt, ...) \
  dev_warn ((ehci)->hcd.controller , fmt , __VA_ARGS__ )

#ifdef EHCI_VERBOSE_DEBUG
#define ehci_vdbg ehci_dbg
#else
#define ehci_vdbg(ehci, fmt, args...) do { } while (0)
#endif

#ifdef  CONFIG_USB_DEBUG

static void dbg_hcs_params (struct ehci_hcd *ehci, char *label)
{
	char tmpStr[10];
	U32  params = ehci_readl (&ehci->caps->hcs_params);


	dev_dbg ((ehci)->hcd.controller,
		"%s hcs_params 0x%x dbg=%d%s cc=%d pcc=%d%s%s ports=%d\n",
		label, params,
		HCS_DEBUG_PORT (params),
		HCS_INDICATOR (params) ? " ind" : "",
		HCS_N_CC (params),
		HCS_N_PCC (params),
		HCS_PORTROUTED (params) ? "" : " ordered",
		HCS_PPC (params) ? "" : " !ppc", HCS_N_PORTS (params) );

	if (HCS_PORTROUTED (params)) {
		int i;
		char buf [46], tmp [7], byte;

		buf[0] = 0;
#ifndef CONFIG_FARADAY_FOTG200
		for (i = 0; i < HCS_N_PORTS (params); i++) {
			byte = ehci_readb (&ehci->caps->portroute[(i>>1)]);
			USB_sprintf(tmp, "%d ",
				((i & 0x1) ? ((byte)&0xf) : ((byte>>4)&0xf)));
			strcat(buf, tmp);
		}
		ehci_dbg (ehci, "%s portroute %s\n",
				label, buf);
#endif
	}
}
#else

static __inline__ void dbg_hcs_params (struct ehci_hcd *ehci, char *label) {}

#endif

#ifdef  DEBUG

static void dbg_hcc_params (struct ehci_hcd *ehci, char *label)
{
	U32  params = ehci_readl (&ehci->caps->hcc_params);

	if (HCC_ISOC_CACHE (params)) {
		dev_dbg ((ehci)->hcd.controller,
			"%s hcc_params 0x%04x caching frame %s%s%s\n",
			label,
			params,
			HCC_PGM_FRAMELISTLEN (params) ? "256/512/1024" : "1024",
			HCC_CANPARK (params) ? " park" : "",
			HCC_64BIT_ADDR (params) ? " 64 bit addr" : "");
	} else {
		dev_dbg ((ehci)->hcd.controller,
			"%s hcc_params 0x%04x thresh %d uframes %s%s%s\n",
			label,
			params,
			HCC_ISOC_THRES (params),
			HCC_PGM_FRAMELISTLEN (params) ? "256/512/1024" : "1024",
			HCC_CANPARK (params) ? " park" : "",
			HCC_64BIT_ADDR (params) ? " 64 bit addr" : "");
	}
}
#else

static __inline__ void dbg_hcc_params (struct ehci_hcd *ehci, char *label) {}

#endif

#ifdef  DEBUG

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static void dbg_qtd (char *label, struct ehci_hcd *ehci, struct ehci_qtd *qtd)
{
	dev_dbg ((ehci)->hcd.controller, "%s qtd %p next%08x %08x token%08x buf=%08x\n", label, qtd,
		cpu_to_le32p (&qtd->hw_next),
		cpu_to_le32p (&qtd->hw_alt_next),
		cpu_to_le32p (&qtd->hw_token),
		cpu_to_le32p (&qtd->hw_buf [0]));
  if (qtd->hw_buf [1])
	dev_dbg ((ehci)->hcd.controller, "  buf1=%08x buf2=%08x buf3=%08x bufp4=%08x\n",
		cpu_to_le32p (&qtd->hw_buf [1]),
		cpu_to_le32p (&qtd->hw_buf [2]),
		cpu_to_le32p (&qtd->hw_buf [3]),
		cpu_to_le32p (&qtd->hw_buf [4]));
}

static void dbg_qh (char *label, struct ehci_hcd *ehci, struct ehci_qh *qh)
{
	dev_dbg ((ehci)->hcd.controller, "%s qh %p next%08x info %x %x qtd %x\n", label,
			qh, qh->hw_next, qh->hw_info1, qh->hw_info2,
			qh->hw_current);
	dbg_qtd ("qh overlay", ehci, (struct ehci_qtd *) &qh->hw_qtd_next);
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
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static int dbg_status_buf (char *buf, unsigned len, char *label, U32 status)
{
	return snprintf (buf, len,
			"%s%sstatus %04x%s%s%s%s%s%s%s%s%s%s",
			label, label [0] ? " " : "", status,
			(status & STS_ASS) ? " Async" : "",
			(status & STS_PSS) ? " Periodic" : "",
			(status & STS_RECL) ? " Recl" : "",
			(status & STS_HALT) ? " Halt" : "",
			(status & STS_IAA) ? " IAA" : "",
			(status & STS_FATAL) ? " FATAL" : "",
			(status & STS_FLR) ? " FLR" : "",
			(status & STS_PCD) ? " PCD" : "",
			(status & STS_ERR) ? " ERR" : "",
			(status & STS_INT) ? " INT" : ""
			);
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
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

static int dbg_intr_buf (char *buf, unsigned len, char *label, U32 enable)
{
  return snprintf (buf, len,
    "%s%sintrenable %02x%s%s%s%s%s%s",
    label, label [0] ? " " : "", enable,
    (enable & STS_IAA) ? " IAA" : "",
    (enable & STS_FATAL) ? " FATAL" : "",
    (enable & STS_FLR) ? " FLR" : "",
    (enable & STS_PCD) ? " PCD" : "",
    (enable & STS_ERR) ? " ERR" : "",
    (enable & STS_INT) ? " INT" : ""
    );
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
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

static const char *const fls_strings [] =
    { "1024", "512", "256", "??" };

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static int dbg_command_buf (char *buf, unsigned len, char *label, U32 command)
{
  return snprintf (buf, len,
    "%s%scmd %06x %s=%d ithresh=%d%s%s%s%s period=%s%s %s",
    label, label [0] ? " " : "", command,
    (command & CMD_PARK) ? "park" : "(park)",
    CMD_PARK_CNT (command),
    (command >> 16) & 0x3f,
    (command & CMD_LRESET) ? " LReset" : "",
    (command & CMD_IAAD) ? " IAAD" : "",
    (command & CMD_ASE) ? " Async" : "",
    (command & CMD_PSE) ? " Periodic" : "",
    fls_strings [(command >> 2) & 0x3],
    (command & CMD_RESET) ? " Reset" : "",
    (command & CMD_RUN) ? "RUN" : "HALT"
    );
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
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

static int
dbg_port_buf (char *buf, unsigned len, char *label, int port, U32 status)
{
  char  *sig;

  /* signaling state */
  switch (status & (3 << 10)) {
  case 0 << 10: sig = "se0"; break;
  case 1 << 10: sig = "k"; break;    /* low speed */
  case 2 << 10: sig = "j"; break;
  default: sig = "?"; break;
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
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  return snprintf (buf, len,
    "%s%sport %d status %06x%s%s sig=%s %s%s%s%s%s%s%s%s%s",
    label, label [0] ? " " : "", port, status,
    (status & PORT_POWER) ? " POWER" : "",
    (status & PORT_OWNER) ? " OWNER" : "",
    sig,
    (status & PORT_RESET) ? " RESET" : "",
    (status & PORT_SUSPEND) ? " SUSPEND" : "",
    (status & PORT_RESUME) ? " RESUME" : "",
    (status & PORT_OCC) ? " OCC" : "",
    (status & PORT_OC) ? " OC" : "",
    (status & PORT_PEC) ? " PEC" : "",
    (status & PORT_PE) ? " PE" : "",
    (status & PORT_CSC) ? " CSC" : "",
    (status & PORT_CONNECT) ? " CONNECT" : ""
      );
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
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

#else
static inline void dbg_qh (char *label, struct ehci_hcd *ehci, struct ehci_qh *qh)
{}

static inline int dbg_status_buf (char *buf, unsigned len, char *label, U32 status)
{ return 0; }

static inline int dbg_command_buf (char *buf, unsigned len, char *label, U32 command)
{ return 0; }

static inline int dbg_intr_buf (char *buf, unsigned len, char *label, U32 enable)
{ return 0; }

static inline int dbg_port_buf (char *buf, unsigned len, char *label, int port, U32 status)
{ return 0; }

#endif  /* CONFIG_USB_DEBUG */

#define dbg_status(ehci, label, status) { \
  char _buf [80]; \
  dbg_status_buf (_buf, sizeof _buf, label, status); \
  ehci_dbg (ehci, "%s\n", _buf); \
}

#define dbg_cmd(ehci, label, command) { \
  char _buf [80]; \
  dbg_command_buf (_buf, sizeof _buf, label, command); \
  ehci_dbg (ehci, "%s\n", _buf); \
}

#define dbg_port(ehci, label, port, status) { \
  char _buf [80]; \
  dbg_port_buf (_buf, sizeof _buf, label, port, status); \
  ehci_dbg (ehci, "%s\n", _buf); \
}

/*-------------------------------------------------------------------------*/

#ifdef CONFIG_USB_DEBUG

static inline char speed_char(int info1)
{
    char tmp;
    switch (info1 & (3 << 12)) {
    case 0 << 12:
      tmp = 'f';
      break;
    case 1 << 12:
      tmp = 'l';
      break;
    case 2 << 12:
      tmp = 'h';
      break;
    default:
      tmp = '?';
      break;
    }
    return tmp;
}

static inline char token_mark (U32 token)
{
  token = le32_to_cpu (token);
  if (token & QTD_STS_ACTIVE)
    return '*';
  if (token & QTD_STS_HALT)
    return '-';
  if (!IS_SHORT_READ (token))
    return ' ';
  return '/';
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
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static void qh_lines (
  struct ehci_hcd *ehci,
  struct ehci_qh *qh,
  char **nextp,
  unsigned *sizep
)
{
  U32      scratch;
  U32      hw_curr;
  struct list_head  *entry;
  struct ehci_qtd    *td;
  unsigned    temp;
  unsigned    size = *sizep;
  char      *next = *nextp;
  char      mark;
  char *tmpStr[8];

  if (qh->hw_qtd_next == EHCI_LIST_END)
    mark = '@';
  else
    mark = token_mark (qh->hw_token);
  if (mark == '/') {
    if ((qh->hw_alt_next & QTD_MASK) == ehci->async->hw_alt_next)
      mark = '#';
    else if (qh->hw_alt_next == EHCI_LIST_END)
      mark = '.';
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
  scratch = cpu_to_le32p (&qh->hw_info1);
  hw_curr = (mark == '*') ? cpu_to_le32p (&qh->hw_current) : 0;
  temp = snprintf (next, size,
      "qh/%p dev%d %cs ep%d %08x %08x (%08x%c %s nak%d)",
      qh, scratch & 0x007f,
      speed_char (scratch),
      (scratch >> 8) & 0x000f,
      scratch, cpu_to_le32p (&qh->hw_info2),
      cpu_to_le32p (&qh->hw_token), mark,
      (__constant_cpu_to_le32 (QTD_TOGGLE) & qh->hw_token)
        ? "data0" : "data1",
      (cpu_to_le32p (&qh->hw_alt_next) >> 1) & 0x0f);
  size -= temp;
  next += temp;

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
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  list_for_each (entry, &qh->qtd_list) {
    //td = list_entry (entry, struct ehci_qtd, qtd_list);
    list_entry (entry,struct ehci_qtd,qtd_list,struct list_head,td);
    scratch = cpu_to_le32p (&td->hw_token);
    mark = ' ';
    if (hw_curr == td->qtd_dma)
      mark = '*';
    else if (qh->hw_qtd_next == td->qtd_dma)
      mark = '+';
    else if (QTD_LENGTH (scratch)) {
      if (td->hw_alt_next == ehci->async->hw_alt_next)
        mark = '#';
      else if (td->hw_alt_next != EHCI_LIST_END)
        mark = '/';
    }
    switch ((scratch>>8)&0x03) {
    case 0:
      strcpy(tmpStr,"out");
      break;
    case 1:
      strcpy(tmpStr,"in");
      break;
    case 2:
      strcpy(tmpStr,"setup");
      break;
    default:
      strcpy(tmpStr, "?");
      break;
    }
    temp = snprintf (next, size,
        "\n\t%p%c%s len=%d %08x urb %p",
        td, mark,
        tmpStr,
        (scratch >> 16) & 0x7fff,
        scratch,
        td->urb);
    if ((signed int)temp < 0)
      temp = 0;
    else if (size < temp)
      temp = size;
    size -= temp;
    next += temp;
    if (temp == size)
      goto done;
  }

  temp = snprintf (next, size, "\n");
  if ((signed int)temp < 0)
    temp = 0;
  else if (size < temp)
    temp = size;
  size -= temp;
  next += temp;

done:
  *sizep = size;
  *nextp = next;
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
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static ssize_t show_async (struct usb_bus *bus, char *buf)
{
  struct usb_hcd    *hcd;
  struct ehci_hcd    *ehci;
  unsigned long    flags;
  unsigned    temp, size;
  char      *next;
  struct ehci_qh    *qh;

  hcd = bus->hcpriv;
  ehci = hcd_to_ehci (hcd);
  next = buf;
  size = PAGE_SIZE;

  spin_lock_irqsave (&ehci->lock, flags);
  for (qh = ehci->async->qh_next.qh; size > 0 && qh; qh = qh->qh_next.qh)
    qh_lines (ehci, qh, &next, &size);
  if (ehci->reclaim && size > 0) {
    temp = snprintf (next, size, "\nreclaim =\n");
    size -= temp;
    next += temp;

    for (qh = ehci->reclaim; size > 0 && qh; qh = qh->reclaim)
      qh_lines (ehci, qh, &next, &size);
  }
  spin_unlock_irqrestore (&ehci->lock, flags);

  return strlen (buf);
}

#define DBG_SCHED_LIMIT 64

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
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static ssize_t show_periodic (struct usb_bus *bus, char *buf)
{
  struct usb_hcd    *hcd;
  struct ehci_hcd    *ehci;
  unsigned long    flags;
  union ehci_shadow  p, *seen;
  unsigned    temp, size, seen_count;
  char      *next;
  unsigned    i, tag;
  struct ehci_qtd *__mptr;

  if (!(seen = kmalloc (DBG_SCHED_LIMIT * sizeof *seen, SLAB_ATOMIC)))
    return 0;
  seen_count = 0;

  hcd = bus->hcpriv;
  ehci = hcd_to_ehci (hcd);
  next = buf;
  size = PAGE_SIZE;

  temp = snprintf (next, size, "size = %d\n", ehci->periodic_size);
  size -= temp;
  next += temp;

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
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  spin_lock_irqsave (&ehci->lock, flags);
  for (i = 0; i < ehci->periodic_size; i++) {
    p = ehci->pshadow [i];
    if (likely (!p.ptr))
      continue;
    tag = Q_NEXT_TYPE (ehci->periodic [i]);

    temp = snprintf (next, size, "%4d: ", i);
    size -= temp;
    next += temp;

    do {
      switch (tag) {
      case Q_TYPE_QH:
        temp = snprintf (next, size, " qh%d-%04x/%p",
            p.qh->period,
            le32_to_cpup (&p.qh->hw_info2)
              /* uframe masks */
              & 0xffff,
            p.qh);
        size -= temp;
        next += temp;
        for (temp = 0; temp < seen_count; temp++) {
          if (seen [temp].ptr != p.ptr)
            continue;
          if (p.qh->qh_next.ptr)
            temp = snprintf (next, size,
              " ...");
          p.ptr = 0;
          break;
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
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

        if (temp == seen_count && p.ptr) {
          U32  scratch = cpu_to_le32p (
              &p.qh->hw_info1);
          struct ehci_qtd  *qtd;
          char    *type = "";

          temp = 0;
          list_entry((&p.qh->qtd_list)->next,struct ehci_qtd, qtd_list, struct list_head, qtd);
          for( ;&qtd->qtd_list != (&p.qh->qtd_list);
                __mptr = qtd->qtd_list.next,
                qtd = (struct ehci_qtd *)((char *)__mptr - offsetof(struct ehci_qtd, qtd_list))
          ){
          /*
          list_for_each_entry (qtd,
              &p.qh->qtd_list,
              qtd_list) {
          */
            temp++;
            switch (0x03 & (le32_to_cpu (
              qtd->hw_token) >> 8)) {
            case 0: type = "out"; continue;
            case 1: type = "in"; continue;
            }
          }

          temp = snprintf (next, size,
            " (%c%d ep%d%s "
            "[%d/%d] q%d p%d)",
            speed_char (scratch),
            scratch & 0x007f,
            (scratch >> 8) & 0x000f, type,
            p.qh->usecs, p.qh->c_usecs,
            temp,
            0x7ff & (scratch >> 16));

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
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
          if (seen_count < DBG_SCHED_LIMIT)
            seen [seen_count++].qh = p.qh;
        } else
          temp = 0;
        if (p.qh) {
          tag = Q_NEXT_TYPE (p.qh->hw_next);
          p = p.qh->qh_next;
        }
        break;
      case Q_TYPE_FSTN:
        temp = snprintf (next, size,
          " fstn-%8x/%p", p.fstn->hw_prev,
          p.fstn);
        tag = Q_NEXT_TYPE (p.fstn->hw_next);
        p = p.fstn->fstn_next;
        break;
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
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
      case Q_TYPE_ITD:
        temp = snprintf (next, size,
          " itd/%p", p.itd);
        tag = Q_NEXT_TYPE (p.itd->hw_next);
        p = p.itd->itd_next;
        break;
      case Q_TYPE_SITD:
        temp = snprintf (next, size,
          " sitd/%p", p.sitd);
        tag = Q_NEXT_TYPE (p.sitd->hw_next);
        p = p.sitd->sitd_next;
        break;
      }
      size -= temp;
      next += temp;
    } while (p.ptr);

    temp = snprintf (next, size, "\n");
    size -= temp;
    next += temp;
  }
  spin_unlock_irqrestore (&ehci->lock, flags);
  kfree (seen);

  return PAGE_SIZE - size;
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
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

#undef DBG_SCHED_LIMIT

static ssize_t
show_registers (struct usb_bus *bus, char *buf)
{
  struct usb_hcd    *hcd;
  struct ehci_hcd    *ehci;
  unsigned long    flags;
  unsigned    temp, size, i;
  char      *next, scratch [80];
  static char    fmt [] = "%*s\n";
  static char    label [] = "";

  hcd = bus->hcpriv;
  ehci = hcd_to_ehci (hcd);
  next = buf;
  size = PAGE_SIZE;

  spin_lock_irqsave (&ehci->lock, flags);

  i = ehci_readw (&ehci->caps->hci_version);
  temp = snprintf (next, size,
    "AMBA Device \nEHCI %x.%02x, hcd state %d (driver " DRIVER_VERSION ")\n",
     i >> 8, i & 0x0ff, ehci->hcd.state);
  size -= temp;
  next += temp;

  i = ehci_readl (&ehci->caps->hcs_params);
  temp = snprintf (next, size, "structural params 0x%08x\n", i);
  size -= temp;
  next += temp;

  i = ehci_readl (&ehci->caps->hcc_params);
  temp = snprintf (next, size, "capability params 0x%08x\n", i);
  size -= temp;
  next += temp;

  temp = dbg_status_buf (scratch, sizeof scratch, label,
      ehci_readl (&ehci->regs->status));
  temp = snprintf (next, size, fmt, temp, scratch);
  size -= temp;
  next += temp;

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
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  temp = dbg_command_buf (scratch, sizeof scratch, label,
      ehci_readl (&ehci->regs->command));
  temp = snprintf (next, size, fmt, temp, scratch);
  size -= temp;
  next += temp;

  temp = dbg_intr_buf (scratch, sizeof scratch, label,
      ehci_readl (&ehci->regs->intr_enable));
  temp = snprintf (next, size, fmt, temp, scratch);
  size -= temp;
  next += temp;

  temp = snprintf (next, size, "uframe %04x\n",
      ehci_readl (&ehci->regs->frame_index));
  size -= temp;
  next += temp;

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
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  for (i = 0; i < HCS_N_PORTS (ehci->hcs_params); i++) {
    temp = dbg_port_buf (scratch, sizeof scratch, label, i,
        ehci_readl (&ehci->regs->port_status [i]));
    temp = snprintf (next, size, fmt, temp, scratch);
    size -= temp;
    next += temp;
  }

  if (ehci->reclaim) {
    temp = snprintf (next, size, "reclaim qh %p%s\n",
        ehci->reclaim,
        ehci->reclaim_ready ? " ready" : "");
    size -= temp;
    next += temp;
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
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#ifdef EHCI_STATS
  temp = snprintf (next, size,
    "irq normal %ld err %ld reclaim %ld (lost %ld)\n",
    ehci->stats.normal, ehci->stats.error, ehci->stats.reclaim,
    ehci->stats.lost_iaa);
  size -= temp;
  next += temp;

  temp = snprintf (next, size, "complete %ld unlink %ld\n",
    ehci->stats.complete, ehci->stats.unlink);
  size -= temp;
  next += temp;
#endif

  spin_unlock_irqrestore (&ehci->lock, flags);

  return PAGE_SIZE - size;
}
#endif /* CONFIG_USB_DEBUG */

