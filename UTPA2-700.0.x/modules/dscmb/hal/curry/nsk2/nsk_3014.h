/*
Copyright (c) NDS Limited 2007

  P R O P R I E T A R Y & C O N F I D E N T I A L
  
    The copyright of this code and related documentation together with
    any other associated intellectual property rights are vested in
    NDS Limited and may not be used except in accordance with the terms
    of the license that you have entered into with NDS Limited.
    Use of this material without an express license from NDS Limited
    shall be an infringement of copyright and any other intellectual
    property rights that may be incorporated with this material.
*/

#ifndef H_NSK_3014
#define H_NSK_3014

/**
* @mainpage Scrambling and Descrambling Algorithms 
*
* @author Julia Rabinovich
* @date 26/11/2010
*
* @file nsk_3014.h

* @brief Transport and M2M Algorithm Profile codes

* This file contains the definitions algorithm profiles
* Reference document: HWSEC-REG-3014 
*
*/

/** @addtogroup TRANSPORT_ALGORITHMS Transport Algorithms
*@{
*/

#define NSK2HDI_SPROFILE_DVB_CSA2               100U
#define NSK2HDI_SPROFILE_DVB_CSA_CONFORMANCE    101U
#define NSK2HDI_SPROFILE_DVB_CSA3               102U
#define NSK2HDI_SPROFILE_CPCM_LSA_MDI_CBC       103U
#define NSK2HDI_SPROFILE_CPCM_LSA_MDI_RCBC      104U
#define NSK2HDI_SPROFILE_CPCM_LSA_MDD_CBC       105U
#define NSK2HDI_SPROFILE_CPCM_LSA_MDD_RCBC      106U
#define NSK2HDI_SPROFILE_SYNAMEDIA_AES          107U                    
#define NSK2HDI_SPROFILE_AES_ECB_CLEARTAIL      108U                    
#define NSK2HDI_SPROFILE_CIPLUS_AES             109U
#define NSK2HDI_SPROFILE_SCTE41_DES             110U
#define NSK2HDI_SPROFILE_SCTE52_DES             111U
#define NSK2HDI_SPROFILE_TDES_ECB_CLEARTAIL     112U
#define NSK2HDI_SPROFILE_MULTI2_TS				113U
/** @}
* End of TRANSPORT_ALGORITHMS group belonging
*/


/** @addtogroup M2M_ALGORITHMS M2M Algorithms
*@{
*/

#define NSK2HDI_SPROFILE_M2M_DES_ECB_CLR_CLR	     1U
#define NSK2HDI_SPROFILE_M2M_DES_CBC_SCTE52_IV1	     2U
#define NSK2HDI_SPROFILE_M2M_DES_CBC_SCTE52_IV2	     3U
#define NSK2HDI_SPROFILE_M2M_DES_CBC_CLR_CLR	     4U
#define NSK2HDI_SPROFILE_M2M_TDES_ECB_CLR_CLR	     5U
#define NSK2HDI_SPROFILE_M2M_TDES_CBC_SCTE52_IV1	 6U
#define NSK2HDI_SPROFILE_M2M_TDES_CBC_SCTE52_IV2	 7U
#define NSK2HDI_SPROFILE_M2M_TDES_CBC_CLR_CLR	     8U
#define NSK2HDI_SPROFILE_M2M_AES_ECB_CLR_CLR	     9U
#define NSK2HDI_SPROFILE_M2M_AES_CBC_CTS_IV1	    10U
#define NSK2HDI_SPROFILE_M2M_AES_CBC_CTS_CLR	    11U
#define NSK2HDI_SPROFILE_M2M_AES_CBC_SCTE52_IV1	    12U
#define NSK2HDI_SPROFILE_M2M_AES_CBC_SCTE52_CLR	    13U
#define NSK2HDI_SPROFILE_M2M_AES_CBC_CLR_CLR	    14U
#define NSK2HDI_SPROFILE_M2M_RC4_64	                15U
#define NSK2HDI_SPROFILE_M2M_AES_CTR	            16U
#define NSK2HDI_SPROFILE_M2M_AES_CTR64				19U
/** @}
* End of M2M_ALGORITHMS group belonging
*/

#endif /* NSK2HDI_3014_H_ */




