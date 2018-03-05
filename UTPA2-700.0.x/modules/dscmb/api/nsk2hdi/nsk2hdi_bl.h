/*
Copyright (c) NDS Limited 2010

P R O P R I E T A R Y & C O N F I D E N T I A L

The copyright of this code and related documentation together with
any other associated intellectual property rights are vested in
NDS Limited and may not be used except in accordance with the terms
of the license that you have entered into with NDS Limited.
Use of this material without an express license from NDS Limited
shall be an infringement of copyright and any other intellectual
property rights that may be incorporated with this material.
*/

#ifndef NSK2HDI_BL_H_
#define NSK2HDI_BL_H_

/**
* @mainpage NSK2 API - HDI Bootloader API for NSK2 Chips
*
* @author Reuben Sumner, Julia Rabinovich
* @date 28/06/2011
* @version 4.01
*
* @file nsk2hdi_bl.h

* @brief NSK2 API - HDI Bootloader API for NSK2 Chips

* This file contains the definitions and functions of the NSK2 Bootloader supplement

*
*/

#include "ndstypes.h"
//#include "nsk2hdi_3014.h"
//#include "nsk2hdi.h"
#include "nsk_3014.h"
#include "nsk_282.h"
/**
** @addtogroup NSK2HDI_BL  BL Device
** @{
*/


/**
** @addtogroup NSK2HDI_BL_REQUEST  BL Request IDs
** @{
*/
#define NSK2HDI_BL_ALL_DESCRIPTORS_REQUEST 0x00U /*!< @brief request for reading all BL descriptors*/
/** @}
* End of NSK2HDI_BL_REQUEST group belonging
*/


/**
** @addtogroup NSK2HDI_BL_HASH  BL Hashing Algorithms
** @{
*/
#define NSK2HDI_BL_SHA1_HASH_ALG_TYPE                    0x01U   /*!< @brief SHA1 hash type*/
#define NSK2HDI_BL_SHA256_HASH_ALG_TYPE                  0x02U   /*!< @brief SHA256 hash type*/
#define NSK2HDI_BL_MD5_HASH_ALG_TYPE                     0x03U   /*!< @brief MD5 hash type */
/** @}
* End of NSK2HDI_BL_HASH group belonging
*/

/**
** @addtogroup NSK2HDI_BL_DESCRIPTOR_TAGS  BL Descriptor Tags
** @{
*/
#define NSK2HDI_BL_ALGORITHM_DESC_TAG        0x01U  /*!< @brief Algorithm descriptor*/
#define NSK2HDI_BL_CAPABILITY_DESC_TAG        0x02U  /*!< @brief Device capability descriptor*/
/** @}
* End of NSK2HDI_BL_DESCRIPTOR_TAGS group belonging
*/


/*===========================================================================*/
/** @brief This function is used to query the capabilities of the provided hashing engine.

@param[in] request_id - the ID of the request. See \ref NSK2HDI_BL_REQUEST for the list of the possible values.
@param[in, out] desc_size - size of the buffer allocated by the caller for the device properties as an input, actual size of the device properties returned by the device as an output.
@param[out] desc - pointer to the buffer receiving the descriptors with device properties. See \ref NSK2HDI_BL_DESCRIPTOR_TAGS

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.

*/
NSK2HDI_STATUS NSK2HDI_BL_GetHashCapabilities(
                                        NDS_ULONG   request_id,
                                        NDS_ULONG  *desc_size,
                                        NDS_UBYTE  *desc
                                        );

/*===========================================================================*/
/** @brief This function is used to initialize the hash operation cycle.

@param[in] desc_size - Size of the buffer carrying the descriptors.
@param[in] desc - Buffer carrying the descriptors. Should contain #NSK2HDI_BL_ALGORITHM_DESC_TAG descriptor.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.

*/
NSK2HDI_STATUS NSK2HDI_BL_InitializeHashOperation(
                                                  NDS_ULONG	desc_size,
                                                  NDS_UBYTE	*desc
                                                  );

/*===========================================================================*/
/** @brief This function is used to perform the hashing of the data block.

@param[in] data_size - Size of data block to be hashed in bytes. Must be a multiple of underlying hash block size (generally 64-bytes) and the minimum granularity as specified by #NSK2HDI_BL_CAPABILITY_DESC_TAG.
@param[in] data - Pointer to the data to be hashed. The data must be aligned according to the device capability as reported by #NSK2HDI_BL_CAPABILITY_DESC_TAG.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/

NSK2HDI_STATUS NSK2HDI_BL_PerformHashOperation(
                                        NDS_ULONG           data_size,
                                        const NDS_UBYTE     *data
                                        );

/*===========================================================================*/
/** @brief This function is used to commit the completion of the hash operation cycle
 and retrieve the digest.

@param[in] data - Pointer to the last block of data to be hashed. The data must be aligned according to the device capability as reported by #NSK2HDI_BL_CAPABILITY_DESC_TAG. This value may be NULL if and only if data_size is 0.
@param[in] data_size - Size of the final data in bytes. Size in bytes of the final block of data to hash. Must be a multiple of the minimum granularity as reported by #NSK2HDI_BL_CAPABILITY_DESC_TAG. This does not need to be a multiple of the underlying hash block size.
@param[in, out] digest_size - size of the buffer allocated by the caller for the hash digest as an input, actual digest size written to the buffer as an output.
@param[out] digest - pointer to the buffer receiving the digest

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
NSK2HDI_STATUS NSK2HDI_BL_CompleteHashOperation(
                                        NDS_ULONG           data_size,
                                        const NDS_UBYTE     *data,
                                        NDS_ULONG           *digest_size,
                                        NDS_UBYTE           *digest
                                        );

/** @}
* End of NSK2HDI_BL group belonging
*/

#endif /*NSK2HDI_BL_H_*/
