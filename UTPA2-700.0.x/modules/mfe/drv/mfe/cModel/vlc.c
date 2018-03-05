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


#include "MFE_chip.h"
#include "mfe_common.h"
#include "mfe_type.h"
#include "ms_dprintf.h"

#include "vlc.h"
#include "nalucommon.h"




//Start code and Emulation Prevention need this to be defined in identical manner at encoder and decoder
#define ZEROBYTES_SHORTSTARTCODE 2 //indicates the number of zero bytes in the short start-code prefix


MFE_BYTE NAL_Payload_buffer[MAXRBSPSIZE<<1];


int iabs(int x)
{
    return ((x) < 0) ? -(x) : (x);
}


/*!
************************************************************************
* \brief
*    Converts String Of Data Bits (SODB) to Raw Byte Sequence
*    Packet (RBSP)
* \param currStream
*        Bitstream which contains data bits.
* \return None
* \note currStream is byte-aligned at the end of this function
*
************************************************************************
*/

void SODBtoRBSP(Bitstream *currStream)
{
    currStream->byte_buf <<= 1;
    currStream->byte_buf |= 1;
    currStream->bits_to_go--;
    currStream->byte_buf <<= currStream->bits_to_go;
    currStream->streamBuffer[currStream->byte_pos++] = currStream->byte_buf;
    currStream->bits_to_go = 8;
    currStream->byte_buf = 0;
}

/*!
************************************************************************
*  \brief
*     This function converts a RBSP payload to an EBSP payload
*
*  \param streamBuffer
*       pointer to data bits
*  \param begin_bytepos
*            The byte position after start-code, after which stuffing to
*            prevent start-code emulation begins.
*  \param end_bytepos
*           Size of streamBuffer in bytes.
*  \param min_num_bytes
*           Minimum number of bytes in payload. Should be 0 for VLC entropy
*           coding mode. Determines number of stuffed words for CABAC mode.
*  \return
*           Size of streamBuffer after stuffing.
*  \note
*      NAL_Payload_buffer is used as temporary buffer to store data.
*
*
************************************************************************
*/

int RBSPtoEBSP(MFE_BYTE *streamBuffer, int begin_bytepos, int end_bytepos, int min_num_bytes)
{
    
    int i, j, count;
    
    memcpy(&NAL_Payload_buffer[begin_bytepos],&streamBuffer[begin_bytepos], (end_bytepos - begin_bytepos) * sizeof(unsigned char));
    count = 0;
    j = begin_bytepos;
    for(i = begin_bytepos; i < end_bytepos; i++)
    {
        if(count == ZEROBYTES_SHORTSTARTCODE && !(NAL_Payload_buffer[i] & 0xFC))
        {
            streamBuffer[j] = 0x03;	
            j++;
            count = 0;
        }
        streamBuffer[j] = NAL_Payload_buffer[i];
        if(NAL_Payload_buffer[i] == 0x00)
            count++;
        else
            count = 0;
        j++;
    }
    
    for (i = 0; i< (min_num_bytes - end_bytepos); i+=3 )
    {
        streamBuffer[j]   = 0x00; // CABAC zero word
        streamBuffer[j+1] = 0x00;
        streamBuffer[j+2] = 0x03;
        j += 3;
    }
    return j;
}

/*!
*************************************************************************************
* \brief
*    Converts an RBSP to a NALU
*
* \param rbsp
*    byte buffer with the rbsp
* \param nalu
*    nalu structure to be filled
* \param rbsp_size
*    size of the rbsp in bytes
* \param nal_unit_type
*    as in JVT doc
* \param nal_reference_idc
*    as in JVT doc
* \param min_num_bytes
*    some incomprehensible CABAC stuff
* \param UseAnnexbLongStartcode
*    when 1 and when using AnnexB bytestreams, then use a long startcode prefix
*
* \return
*    length of the NALU in bytes
*************************************************************************************
*/

int RBSPtoNALU (unsigned char *rbsp, NALU_t *nalu, int rbsp_size, int nal_unit_type, int nal_reference_idc,
                int min_num_bytes, int UseAnnexbLongStartcode)
{
    int len;
    
    MFE_ASSERT (nalu != NULL);
    MFE_ASSERT (nal_reference_idc <=3 && nal_reference_idc >=0);
    MFE_ASSERT (nal_unit_type > 0 && nal_unit_type <= 10);
    MFE_ASSERT (rbsp_size < MAXRBSPSIZE);
    
    nalu->startcodeprefix_len = UseAnnexbLongStartcode ? 4 : 3;
    nalu->forbidden_bit       = 0;  
    nalu->nal_reference_idc   = (NalRefIdc) nal_reference_idc;
    nalu->nal_unit_type       = (NaluType) nal_unit_type;    
    nalu->buf[0]              =
        nalu->forbidden_bit << 7      |
        nalu->nal_reference_idc << 5  |
        nalu->nal_unit_type;
    
    memcpy (&nalu->buf[1], rbsp, rbsp_size);
    // printf ("First Byte %x\n", nalu->buf[0]);
    // printf ("RBSPtoNALU: Before: NALU len %d\t RBSP %x %x %x %x\n", rbsp_size, (unsigned) nalu->buf[1], (unsigned) nalu->buf[2], (unsigned) nalu->buf[3], (unsigned) nalu->buf[4]);
    
    len = 1 + RBSPtoEBSP (&nalu->buf[1], 0, rbsp_size, min_num_bytes);
    
    // printf ("RBSPtoNALU: After : NALU len %d\t EBSP %x %x %x %x\n", rbsp_size, (unsigned) nalu->buf[1], (unsigned) nalu->buf[2], (unsigned) nalu->buf[3], (unsigned) nalu->buf[4]);
    // printf ("len %d\n\n", len);
    nalu->len = len * 8;
    
    return len;
}

/*!
************************************************************************
* \brief
*    writes UVLC code to the appropriate buffer
************************************************************************
*/
void  writeUVLC2buffer(SyntaxElement *se, Bitstream *currStream)
{
    unsigned int mask = 1 << (se->len - 1);
    int i;  
    //byte *streamBuffer = &currStream->streamBuffer[currStream->byte_pos];
    //MFE_ASSERT ((se->len-1) < 32);
    
    // Add the new bits to the bitstream.
    // Write out a byte if it is full
    for (i = 0; i < se->len; i++)
    {
        currStream->byte_buf <<= 1;
        
        if (se->bitpattern & mask)
            currStream->byte_buf |= 1;
        
        mask >>= 1;
        
        if ((--currStream->bits_to_go) == 0)
        {
            if(currStream->zerocount == ZEROBYTES_SHORTSTARTCODE && !(currStream->byte_buf & 0xFC))
            {
                //currStream->streamBuffer[currStream->byte_pos++] = 0x03;
                //img->prevention_byte++;
                currStream->zerocount = 0;
            }
            currStream->streamBuffer[currStream->byte_pos++] = currStream->byte_buf;
            if(currStream->byte_buf == 0x00)
                currStream->zerocount++;
            else
                currStream->zerocount = 0;
            
            currStream->bits_to_go = 8;
            currStream->byte_buf = 0;
        }
    }
}

/*!
************************************************************************
* \brief
*    Makes code word and passes it back
*    A code word has the following format: 0 0 0 ... 1 Xn ...X2 X1 X0.
*
* \par Input:
*    Info   : Xn..X2 X1 X0                                             \n
*    Length : Total number of bits in the codeword
************************************************************************
*/
// NOTE this function is called with sym->inf > (1<<(sym->len/2)).  The upper bits of inf are junk
int symbol2uvlc(SyntaxElement *sym)
{
    int suffix_len = sym->len >> 1;
    MFE_ASSERT (suffix_len<32);
    sym->bitpattern = (1<<suffix_len)|(sym->inf & ((1<<suffix_len) - 1));
    return 0;
}

/*!
*************************************************************************************
* \brief
*    ue_v, writes an ue(v) syntax element, returns the length in bits
*
* \param tracestring
*    the string for the trace file
* \param value
*    the value to be coded
*  \param bitstream
*    the target bitstream the value should be coded into
*
* \return
*    Number of bits used by the coded syntax element
*
* \ note
*    This function writes always the bit buffer for the progressive scan flag, and
*    should not be used (or should be modified appropriately) for the interlace crap
*    When used in the context of the Parameter Sets, this is obviously not a
*    problem.
*
*************************************************************************************
*/
int ue_v (char *tracestring, int value, Bitstream *bitstream)
{
    SyntaxElement symbol, *sym=&symbol;
    sym->value1 = value;
    sym->value2 = 0;
    
    MFE_ASSERT (bitstream->streamBuffer != NULL);
    
    ue_linfo(sym->value1,sym->value2,&(sym->len),&(sym->inf));
    symbol2uvlc(sym);
    
    writeUVLC2buffer (sym, bitstream);
    
#if TRACE
    strncpy(sym->tracestring,tracestring,TRACESTRING_SIZE);
    trace2out (sym);
#endif
    
    return (sym->len);
}

/*!
 *************************************************************************************
 * \brief
 *    se_v, writes an se(v) syntax element, returns the length in bits
 *
 * \param tracestring
 *    the string for the trace file
 * \param value
 *    the value to be coded
 *  \param bitstream
 *    the target bitstream the value should be coded into
 *
 * \return
 *    Number of bits used by the coded syntax element
 *
 * \ note
 *    This function writes always the bit buffer for the progressive scan flag, and
 *    should not be used (or should be modified appropriately) for the interlace crap
 *    When used in the context of the Parameter Sets, this is obviously not a
 *    problem.
 *
 *************************************************************************************
 */
int se_v (char *tracestring, int value, Bitstream *bitstream)
{
  SyntaxElement symbol, *sym=&symbol;
  sym->value1 = value;
  sym->value2 = 0;

  MFE_ASSERT (bitstream->streamBuffer != NULL);

  se_linfo(sym->value1,sym->value2,&(sym->len),&(sym->inf));
  symbol2uvlc(sym);

  writeUVLC2buffer (sym, bitstream);

#if TRACE
  strncpy(sym->tracestring,tracestring,TRACESTRING_SIZE);
  trace2out (sym);
#endif

  return (sym->len);
}


/*!
 *************************************************************************************
 * \brief
 *    u_1, writes a flag (u(1) syntax element, returns the length in bits,
 *    always 1
 *
 * \param tracestring
 *    the string for the trace file
 * \param value
 *    the value to be coded
 *  \param bitstream
 *    the target bitstream the value should be coded into
 *
 * \return
 *    Number of bits used by the coded syntax element (always 1)
 *
 * \ note
 *    This function writes always the bit buffer for the progressive scan flag, and
 *    should not be used (or should be modified appropriately) for the interlace crap
 *    When used in the context of the Parameter Sets, this is obviously not a
 *    problem.
 *
 *************************************************************************************
 */
int u_1 (char *tracestring, int value, Bitstream *bitstream)
{
  SyntaxElement symbol, *sym=&symbol;

  sym->bitpattern = value;
  sym->len = 1;
  sym->value1 = value;

  MFE_ASSERT (bitstream->streamBuffer != NULL);

  writeUVLC2buffer(sym, bitstream);

#if TRACE
  strncpy(sym->tracestring,tracestring,TRACESTRING_SIZE);
  trace2out (sym);
#endif

  return (sym->len);
}


/*!
 *************************************************************************************
 * \brief
 *    u_v, writes a n bit fixed length syntax element, returns the length in bits,
 *
 * \param n
 *    length in bits
 * \param tracestring
 *    the string for the trace file
 * \param value
 *    the value to be coded
 *  \param bitstream
 *    the target bitstream the value should be coded into
 *
 * \return
 *    Number of bits used by the coded syntax element
 *
 * \ note
 *    This function writes always the bit buffer for the progressive scan flag, and
 *    should not be used (or should be modified appropriately) for the interlace crap
 *    When used in the context of the Parameter Sets, this is obviously not a
 *    problem.
 *
 *************************************************************************************
 */

int u_v (int n, char *tracestring, int value, Bitstream *bitstream)
{
  SyntaxElement symbol, *sym=&symbol;

  sym->bitpattern = value;
  sym->len = n;
  sym->value1 = value;

  MFE_ASSERT (bitstream->streamBuffer != NULL);

  writeUVLC2buffer(sym, bitstream);

#if TRACE
  strncpy(sym->tracestring,tracestring,TRACESTRING_SIZE);
  trace2out (sym);
#endif

  return (sym->len);
}

/*!
************************************************************************
* \brief
*    mapping for ue(v) syntax elements
* \param ue
*    value to be mapped
* \param dummy
*    dummy parameter
* \param info
*    returns mapped value
* \param len
*    returns mapped value length
************************************************************************
*/
void ue_linfo(int ue, int dummy, int *len,int *info)
{
    int i,nn;
    
    nn=(ue+1)>>1;
    
    for (i=0; i < 33 && nn != 0; i++)
    {
        nn >>= 1;
    }
    *len  = (i << 1) + 1;
    *info = ue + 1 - (1 << i);
}


/*!
************************************************************************
* \brief
*    mapping for se(v) syntax elements
* \param se
*    value to be mapped
* \param dummy
*    dummy parameter
* \param len
*    returns mapped value length
* \param info
*    returns mapped value
************************************************************************
*/
void se_linfo(int se, int dummy, int *len,int *info)
{  
    int sign = (se <= 0) ? 1 : 0;
    int n = iabs(se) << 1;   //  n+1 is the number in the code table.  Based on this we find length and info
    int nn = (n >> 1);
    int i;
    for (i=0; i < 33 && nn != 0; i++)
    {
        nn >>= 1;
    }
    *len  = (i << 1) + 1;
    *info = n - (1 << i) + sign;
}
