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

#ifndef H_NSK_282
#define H_NSK_282

/**
* @mainpage NSK2 API - HDI CA API for NSK2 Chips
*
* @author Reuben Sumner, Julia Rabinovich, Zvika Naparstek
* @date 01/01/2014
* @version 7.00
*
* @file nsk_282.h

* @brief NSK2 API - HDI CA API for NSK2 Chips

* This file contains the definitions and functions of the NSK2 High Level Driver Interface
*
*/

#include "ndstypes.h"
#include "nsk_3014.h"

/**
* @addtogroup NSK2HDI_COMMON Common Definitions
* @{
*/
/**
* @addtogroup NSK2HDI_TYPEDEFS Typedefs
* @{
*/
typedef NDS_ULONG   NSK2HDI_STATUS; 			/*!< @brief NSK2 interface return status */
typedef void*       NSK2HDI_HANDLE; 			/*!< @brief NSK2 device handle */
typedef NDS_ULONG   NSK2HDI_DEVICE_ID; 		    /*!< @brief NSK2 device ID */

/** @brief The NSK2HDI_CHILD_DEVICE structure is used to get/set child device information
*/
typedef struct _NSK2HDI_CHILD_DEVICE
{
    NSK2HDI_DEVICE_ID 	device_id;    /*!< @brief Child device ID. Must be unique in the entire system*/
    NDS_ULONG	    	device_type;  /*!< @brief Child device type. See possible values in \ref NSK2HDI_DEVICE_TYPE. */
} NSK2HDI_CHILD_DEVICE;

/**
* @addtogroup NSK2HDI_STATUS Return Values
* @{
*/

#define NSK2HDI_STATUS_OK					  0x00000000U /*!< @brief Success status*/
#define NSK2HDI_STATUS_INVALID_REQUEST  	  0x00000001U /*!< @brief Failure status - one ore more input parameters are invalid*/
#define NSK2HDI_STATUS_FAILED				  0x000000FFU	  /*!< @brief  Failure status - general error */
/** @}
* End of NSK2HDI_STATUS group belonging
*/

/**
* @addtogroup NSK2HDI_DEFINES Special Values
* @{
*/
#define NSK2HDI_ILLEGAL_HANDLE          (NSK2HDI_HANDLE)NULL    /*!< @brief The valid handle value should always be different from this value */
#define NSK2HDI_CHILD_DEVICE_ID_MASK    0xFF000000U             /*!< @brief The mask used for creating child device IDs */
#define NSK2HDI_INVALID_PID             0xFFFF
#define NSK2HDI_INVALID_CONNECTION      0xFFFFFFFFL
/** @}
* End of NSK2HDI_DEFINES group belonging
*/

/** 
* @addtogroup NSK2HDI_DEVICE_TYPE Device types
* @{
*/
#define NSK2HDI_CA_SOC_DEVICE_TYPE							0x00000001U /*!< @brief CA SoC device */
#define NSK2HDI_OTP_DEVICE_TYPE								0x00000002U /*!< @brief OTP device */
#define NSK2HDI_NSK_DEVICE_TYPE								0x00000003U /*!< @brief NSK device */
#define NSK2HDI_CMCHANNEL_TABLE_DEVICE_TYPE			        0x00000004U /*!< @brief CM Channel Table device */
#define NSK2HDI_CMCHANNEL_GROUP_DEVICE_TYPE			        0x00000005U /*!< @brief CM Channel Group device */
#define NSK2HDI_CMCHANNEL_DEVICE_TYPE				        0x00000006U /*!< @brief CM Channel device */
#define NSK2HDI_CA_PID_PROXY_DEVICE_TYPE				    0x00000007U /*!< @brief CA PID Proxy device */
#define NSK2HDI_M2MTABLE_DEVICE_TYPE			            0x00000008U /*!< @brief M2M Table device */
#define NSK2HDI_M2MCHANNEL_GROUP_DEVICE_TYPE			    0x00000009U /*!< @brief M2M Channel Group device */
#define NSK2HDI_M2MCHANNEL_DEVICE_TYPE				        0x0000000AU /*!< @brief M2M Channel device */
#define NSK2HDI_DMA_DEVICE_TYPE				                0x0000000BU /*!< @brief DMA device */

/** @}
* End of NSK2HDI_DEVICE_TYPE group belonging
*/

/** @brief NSK2 HDI drivers device configuration callback function 
*/
typedef void HDINSK2_ENUMERATION_CHANGE_CB_FN ( NSK2HDI_HANDLE device_handle, void *cookie); 

/** @}
* End of NSK2HDI_TYPEDEFS group belonging
*/
/** @}
* End of NSK2HDI_COMMON group belonging
*/

/**
* @addtogroup NSK2HDI_CA_DEVICES CA Devices
* @{
*/


/** 
** @addtogroup NSK2HDI_CA_SoC CA SoC Group Device
** @{
*/

/*===========================================================================*/
/** @brief Creates an instance of the CA SoC Group device and returns its handle to the caller.

@param[in] CaSoCGroup_ID - the ID of the CA SoC Group device to open as provided during the SoC device registration.
@param[out] CaSoCGroup_handle - pointer to the device instance handle to be returned to a caller.

@retval #NSK2HDI_STATUS_OK if device instance is created successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if request failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_CaSoCGroupDevice_Open (NSK2HDI_DEVICE_ID      CaSoCGroup_ID, 
NSK2HDI_HANDLE        *CaSoCGroup_handle);


/*===========================================================================*/
/** @brief Destroys the specified instance of the CA SoC Group device.
Releases all the resources used by this device instance.

@param[in] CaSoCGroup_handle - handle of the device instance to be destroyed.

@retval #NSK2HDI_STATUS_OK if the device instance was destroyed or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if an invalid device handle was passed.
*/
typedef NSK2HDI_STATUS NSK2HDI_CaSoCGroupDevice_Close ( NSK2HDI_HANDLE CaSoCGroup_handle);


/*===========================================================================*/
/** @brief Enumerates CA SoC Group device children. Retrieves number of children and associated info for each child.
If child_device_num is zero, the function is used to retrieve the number of children only and the child_device parameter is ignored.

@param[in] CaSoCGroup_handle - a handle to the opened CA SoC Group device.
@param[in, out] child_device_num - the number of elements in the child_device array allocated by the caller as an input,the actual number of the CA SoC Group child devices as an output
@param[out] child_device - array that receives the child info for the CA SoC Group child devices.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/


typedef NSK2HDI_STATUS NSK2HDI_CaSoCGroupDevice_EnumerateChildren( NSK2HDI_HANDLE     CaSoCGroup_handle, 
                                                                    NDS_ULONG              *child_device_num, 
                                                                    NSK2HDI_CHILD_DEVICE   *child_device);


/*===========================================================================*/
/** @brief Registers the callback function for CA SoC Group device configuration changes.
This method can be called once during the device instance life cycle.

@param[in] CaSoCGroup_handle - a handle to the opened CA SoC Group device.
@param[in] cb_routine - pointer to the #HDINSK2_ENUMERATION_CHANGE_CB_FN function.
@param[in] cookie -  a pointer that will be returned to the caller when the cb_routine is called.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_CaSoCGroupDevice_RegisterEnumerationChangeCB ( NSK2HDI_HANDLE             CaSoCGroup_handle, 
                                                                            HDINSK2_ENUMERATION_CHANGE_CB_FN *cb_routine, 
                                                                            void	                            *cookie);

/** @}
* End of NSK2HDI_CA_SoC group belonging
*/


/** 
** @addtogroup NSK2HDI_OTP OTP Device
** @{
*/
/**
* @addtogroup NSK2HDI_OTP_COMMON OTP Definitions
* @{
*/

/** 
** @addtogroup NSK2HDI_OTP_GET_PROPERTIES_REQUEST  OTP Request IDs
** @{
*/
#define NSK2HDI_OTP_READ_ALL_DESCRIPTORS_REQUEST     0x00U /*!< @brief request for reading all OTP descriptors*/
#define NSK2HDI_OTP_READ_ALL_DESCRIPTORS_REQUEST_ALT 0x01U /*!< @brief request for reading all OTP descriptors*/
#define NSK2HDI_OTP_READ_FULLCHIPCONFIGURATION	     0x02U /*!< @brief request for reading OTP hash configuration */
#define NSK2HDI_OTP_MAX_NVCOUNTER_UPDATES            0x03U /*!< @brief request for reading OTP max NV counter updates*/
/** @}
* End of NSK2HDI_OTP_GET_PROPERTIES_REQUEST group belonging
*/

/** 
** @addtogroup NSK2HDI_OTP_DESCRIPTOR_TAGS  OTP Descriptor Tags
** @{
*/
#define NSK2HDI_OTP_NDS_ID_DESC_TAG         0x20U  /*!< @brief NDS serialization info descriptor*/
#define NSK2HDI_OTP_NDS_BULK128_0_DESC_TAG  0x21U  /*!< @brief First 128 bit bulk OTP read descriptor */
#define NSK2HDI_OTP_NDS_BULK128_1_DESC_TAG  0x22U  /*!< @brief Second 128 bit bulk OTP read descriptor */
#define NSK2HDI_OTP_NDS_BULK128_2_DESC_TAG  0x23U  /*!< @brief Third 128 bit bulk OTP read descriptor */
#define NSK2HDI_OTP_NDS_BULK128_3_DESC_TAG  0x24U  /*!< @brief Fourth 128 bit bulk OTP read descriptor */
#define NSK2HDI_OTP_NVCOUNTER_DESC_TAG  	0x25U  
/** @}
* End of NSK2HDI_OTP_DESCRIPTOR_TAGS group belonging
*/
/** @}
* End of NSK2HDI_OTP_COMMON group belonging
*/

/*===========================================================================*/
/** @brief Creates an instance of the OTP device and returns its handle to the caller.

@param[in] Otp_ID - the ID of the OTP device to open as returned by #NSK2HDI_CaSoCGroupDevice_EnumerateChildren function.
@param[out] Otp_handle - pointer to the device instance handle to be returned to a caller.

@retval #NSK2HDI_STATUS_OK if device instance is created successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if request failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_OtpDevice_Open (NSK2HDI_DEVICE_ID     Otp_ID, 
                                                NSK2HDI_HANDLE	    *Otp_handle);


/*===========================================================================*/
/** @brief Destroys the specified instance of the OTP device.
Releases all the resources used by this device instance.

@param[in] Otp_handle - handle of the device instance to be destroyed.

@retval #NSK2HDI_STATUS_OK if the device instance was destroyed or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if an invalid device handle was passed.
*/
typedef NSK2HDI_STATUS NSK2HDI_OtpDevice_Close (NSK2HDI_HANDLE Otp_handle);


/*===========================================================================*/
/** @brief Retrieves the Properties of the OTP device.
If desc_size is zero, the function is used to retrieve the size of the buffer to be allocated for the properties, only and the desc parameter is ignored.

@param[in] Otp_handle - a handle to the opened OTP device.
@param[in] request_id - the ID of the properties retrieval request.See \ref NSK2HDI_OTP_GET_PROPERTIES_REQUEST for possible values.
@param[in, out] desc_size - size of the buffer allocated by the caller for the device properties.
@param[out] desc - pointer to the buffer receiving the descriptors with device properties. See \ref NSK2HDI_OTP_DESCRIPTOR_TAGS

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_OtpDevice_GetProperties( NSK2HDI_HANDLE  Otp_handle, 
                                                        NDS_ULONG        request_id, 
                                                        NDS_ULONG        *desc_size, 
                                                        NDS_UBYTE	     *desc);

/** @}
* End of NSK2HDI_OTP group belonging
*/

/** 
** @addtogroup NSK2HDI_NSK NSK Device
** @{
*/

/**
* @addtogroup NSK2HDI_NSK_COMMON NSK Definitions
* @{
*/

/** @brief NSK2 HDI drivers event callback function 
*/
typedef void HDINSK2_NSKEVENT_CB_FN (NSK2HDI_HANDLE     Nsk_handle, 
                                    void               *cookie, 
                                    NDS_ULONG		 reason, 
                                    NDS_ULONG		 acpuError);

/** 
** @addtogroup NSK2HDI_INITIALIZATION_STAGE NSK Device initialization stage
** @{
    */
#define NSK2HDI_INITIALIZATION_STAGE_ONE         0x01U  /*!< @brief NDS serialization info*/

/** @}
* End of NSK2HDI_INITIALIZATION_STAGE group belonging
*/
/** @}
* End of NSK2HDI_NSK_COMMON group belonging
*/

/*===========================================================================*/
/** @brief Creates an instance of the NSK device and returns its handle to the caller.

@param[in] Nsk_ID - the ID of the NSK device to open as returned by #NSK2HDI_CaSoCGroupDevice_EnumerateChildren function.
@param[out] Nsk_handle - pointer to the device instance handle to be returned to a caller.

@retval #NSK2HDI_STATUS_OK if device instance is created successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if request failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_NskDevice_Open (NSK2HDI_DEVICE_ID     Nsk_ID, 
                                                NSK2HDI_HANDLE       *Nsk_handle);

/*===========================================================================*/
/** @brief Destroys the specified instance of the NSK device. 
Releases all the resources used by this device instance.

@param[in] Nsk_handle- handle of the device instance to be destroyed.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or 
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_NskDevice_Close (NSK2HDI_HANDLE Nsk_handle);


/*===========================================================================*/
/** @brief Reads data from registers.

@param[in] Nsk_handle - a handle to the opened NSK device.
@param[in] register_offset - the offset within the device memory map at which to start the read.
@param[in] data_size - number of bytes to read, must be a positive multiple of 4.
@param[out] data - pointer to the buffer receiving the read data.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or 
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/

typedef NSK2HDI_STATUS NSK2HDI_NskDevice_Read8 (NSK2HDI_HANDLE      Nsk_handle, 
                                                NDS_ULONG           register_offset,
                                                NDS_ULONG           data_size,
                                                NDS_UBYTE            *data);


/*===========================================================================*/
/** @brief Writes data to the registers

@param[in] Nsk_handle - a handle to the opened NSK device.
@param[in] register_offset - the offset within the device memory map at which to start the read.
@param[in] data_size - number of bytes to write, must be a positive multiple of 4.
@param[out] data - pointer to the buffer containing the data to be written.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or 
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_NskDevice_Write8 (NSK2HDI_HANDLE     Nsk_handle, 
                                                NDS_ULONG            register_offset,
                                                NDS_ULONG            data_size,
                                                const NDS_UBYTE	    *data);  



/*===========================================================================*/
/** @brief Reads data from registers

@param[in] Nsk_handle - a handle to the opened NSK device.
@param[in] register_offset - the offset within the device memory map at which to start the read, must be a multiple of 4.
@param[in] data_size - number of 32-bits words to read.
@param[out] data - pointer to the buffer receiving the read data.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or 
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_NskDevice_Read32 (NSK2HDI_HANDLE     Nsk_handle, 
                                                NDS_ULONG           register_offset,
                                                NDS_ULONG           data_size,
                                                NDS_ULONG            *data);

/*===========================================================================*/
/** @brief Writes data to the registers

@param[in] Nsk_handle - a handle to the opened NSK device.
@param[in] register_offset - the offset within the device memory map at which to start writing,must be a multiple of 4.
@param[in] data_size - number of bytes to write.
@param[out] data - pointer to the buffer containing the data to be written.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or 
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_NskDevice_Write32 (NSK2HDI_HANDLE     Nsk_handle, 
                                                NDS_ULONG           register_offset,
                                                NDS_ULONG           data_size,
                                                const NDS_ULONG	    *data);  


/*===========================================================================*/
/** @brief Executes a single command in the NSK.

@param[in] Nsk_handle - a handle to the opened NSK device.
@param[in] command - data to be loaded into the NSK command register.
@param[in] control - data to be loaded into the NSK control register.
@param[in] register_offset - the offset within the device memory map at which to start data loading,must be a multiple of 4.
@param[in] data_size - size of the data to be loaded into the NSK registers. Must be a multiply of 4.
@param[in] data - buffer containing the data to be loaded into the NSK registers.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or 
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_NskDevice_ExecuteCmd (NSK2HDI_HANDLE     Nsk_handle,
                                                    NDS_ULONG           command,  
                                                    NDS_ULONG		    control,  
                                                    NDS_ULONG           register_offset,
                                                    NDS_ULONG           data_size,  
                                                    const NDS_UBYTE	    *data);  


/*===========================================================================*/
/** @brief Registers the callback function for NSK events.
This method can be called once during the device instance life cycle.

@param[in] Nsk_handle - a handle to the opened NSK device.
@param[in] cb_routine - pointer to the #HDINSK2_NSKEVENT_CB_FN function.
@param[in] cookie - a pointer that will be returned to the caller when the cb_routine is called.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or 
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_NskDevice_RegisterEventCB (NSK2HDI_HANDLE            Nsk_handle, 
                                                          HDINSK2_NSKEVENT_CB_FN	*cb_routine, 
                                                          void						*cookie);


/*===========================================================================*/
/** @brief Sends an acknowledgment signal to the NSK that the NSK chip has finished using the KTE contents.

@param[in] Nsk_handle - a handle to the opened NSK device.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or 
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_NskDevice_AcknowledgeKte(NSK2HDI_HANDLE Nsk_handle);

/*===========================================================================*/
/** @brief Called to indicate that NSK basic initialization is complete.

@param[in] Nsk_handle - a handle to the opened NSK device.
@param[in] stage - initialization stage. See \ref NSK2HDI_INITIALIZATION_STAGE for possible values.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or 
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_NskDevice_BasicInitializationComplete(NSK2HDI_HANDLE Nsk_handle, NDS_ULONG stage);

/*===========================================================================*/
/** @brief Called to indicate that NSK Update NV Counter.

@param[in] Nsk_handle - a handle to the opened NSK device.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or 
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_NskDevice_UpdateNvCounter(NSK2HDI_HANDLE Nsk_handle);

/*===========================================================================*/
/** @brief Called to indicate that NSK Write OTP Key.

@param[in] Nsk_handle - a handle to the opened NSK device.
@param[in] desc_size - Size in bytes of the desc buffer.
@param[in] desc - Additional configuration information for WriteOtpKey provided in a series of descriptors.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or 
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_NskDevice_WriteOtpKey(NSK2HDI_HANDLE Nsk_handle, NDS_ULONG desc_size, const NDS_UBYTE *desc);

/*===========================================================================*/
/** @brief Called to indicate that NSK Set One Shot GenInBits.

@param[in] Nsk_handle - a handle to the opened NSK device.
@param[in] bits - Bit mask of bits to set.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or 
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_NskDevice_SetOneShotGenInBits(NSK2HDI_HANDLE Nsk_handle, NDS_ULONG bits);

/** @}
* End of NSK2HDI_NSK group belonging
*/



/** 
** @addtogroup NSK2HDI_CM Content Module Devices
** @{
*/
/** 
** @addtogroup NSK2HDI_CMCHANNELTABLE CmChannel Table Device
** @{
*/

/*===========================================================================*/
/** @brief Creates an instance of the CmChannel Table device and returns its handle to the caller.

@param[in] CmChannelTable_ID - the ID of theCmChannel Table device to open as returned by #NSK2HDI_CaSoCGroupDevice_EnumerateChildren function.
@param[out] CmChannelTable_handle - pointer to the device instance handle to be returned to a caller.

@retval #NSK2HDI_STATUS_OK if device instance is created successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if request failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_CmChannelTableDevice_Open (  NSK2HDI_DEVICE_ID        CmChannelTable_ID, 
                                                            NSK2HDI_HANDLE	        *CmChannelTable_handle);

/*===========================================================================*/
/** @brief Destroys the specified instance of the CmChannel Table device.
Releases all the resources used by this device instance.

@param[in] CmChannelTable_handle - handle of the device instance to be destroyed.

@retval #NSK2HDI_STATUS_OK if the device instance was destroyed or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if an invalid device handle was passed.
*/
typedef NSK2HDI_STATUS NSK2HDI_CmChannelTableDevice_Close ( NSK2HDI_HANDLE CmChannelTable_handle);


/*===========================================================================*/
/** @brief Enumerates CmChannel Table device children. Retrieves number of children and associated info for each child.
If child_device_num is zero, the function is used to retrieve the number of children only and the child_device parameter is ignored.

@param[in] CmChannelTable_handle - a handle to the opened CmChannel Table device.
@param[in, out] child_device_num - the number of elements in the child_device array allocated by the caller as an input, the actual number of the CmChannel Table child devices as an output.
@param[out] child_device - array that receives the child info for the CmChannel Table child devices.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/

typedef NSK2HDI_STATUS NSK2HDI_CmChannelTableDevice_EnumerateChildren( NSK2HDI_HANDLE     CmChannelTable_handle, 
                                                                        NDS_ULONG              *child_device_num, 
                                                                        NSK2HDI_CHILD_DEVICE   *child_device);


/** @}
* End of NSK2HDI_CMCHANNELTABLE group belonging
*/
/** 
** @addtogroup NSK2HDI_CMCHANNELGROUP CmChannel Group Device
** @{
*/
/** 
** @addtogroup NSK2HDI_CMCHANNELGROUP_DEFS CmChannel Group Definitions
** @{
*/
#define NSK2HDI_CMCHANNELGROUP_MAX_USER_CONTEXT_LENGTH 0x08U   /*!< @brief the size of the user context field  */    
/** 
** @addtogroup NSK2HDI_CMCHANNEL_PARITY_FLAGS  CmChannel Parity Flags
** @{
*/
#define NSK2HDI_CMCHANNEL_CLEAR_PARITY             0x01U      /*!< @brief the key can be set for the clear parity */
#define NSK2HDI_CMCHANNEL_EVEN_PARITY              0x02U      /*!< @brief the key can be set for the even parity */
#define NSK2HDI_CMCHANNEL_EVEN_CLEAR_PARITY        0x04U      /*!< @brief the same key can be set for the clear  and even parity */
#define NSK2HDI_CMCHANNEL_ODD_PARITY               0x08U      /*!< @brief the key can be set for the odd parity */
#define NSK2HDI_CMCHANNEL_ODD_CLEAR_PARITY         0x10U      /*!< @brief the same key can be set for the clear and odd parity */
#define NSK2HDI_CMCHANNEL_ODD_EVEN_PARITY          0x20U      /*!< @brief the same key can be set for the odd and even parity */
#define NSK2HDI_CMCHANNEL_ODD_EVEN_CLEAR_PARITY    0x40U      /*!< @brief the same key can be set for the clear, odd and even  parity */
/** @}
* End of NSK2HDI_CMCHANNEL_PARITY_FLAGS group belonging
*/
/** 
** @addtogroup NSK2HDI_CMCHANNEL_SWITCH CmChannel Switch Flags
** @{
*/
#define NSK2HDI_CMCHANNEL_AL_SWITCH_THRU      0x01U /*!< @brief If not set, AL switch is by-passed */
#define NSK2HDI_CMCHANNEL_BL_SWITCH_THRU      0x02U /*!< @brief If not set, BL switch is by-passed */
#define NSK2HDI_CMCHANNEL_DL_SWITCH_THRU      0x04U /*!< @brief If not set, DL switch is by-passed */
#define NSK2HDI_CMCHANNEL_EL_SWITCH_THRU      0x08U /*!< @brief If not set, packet is not delivered to the lower destination */
#define NSK2HDI_CMCHANNEL_AU_SWITCH_THRU      0x10U /*!< @brief If not set, AU switch is by-passed */
#define NSK2HDI_CMCHANNEL_BU_SWITCH_THRU      0x20U /*!< @brief If not set, BU switch is by-passed */
#define NSK2HDI_CMCHANNEL_DU_SWITCH_THRU      0x40U /*!< @brief If not set, DU switch is by-passed */
#define NSK2HDI_CMCHANNEL_EU_SWITCH_THRU      0x80U /*!< @brief If not set, packet is not delivered to the upper destination  */
/** @}
* End of NSK2HDI_CMCHANNEL_SWITCH group belonging
*/


/** @addtogroup NSK2HDI_CMCHANNEL_ALGORITHM_TYPE CmChannel Algorithm Types
** @{
*/
#define NSK2HDI_CMCHANNEL_LSA_ALGORITHM_TYPE      0x01U /*!< @brief LSA algrorithm */
#define NSK2HDI_CMCHANNEL_ESA_ALGORITHM_TYPE      0x02U /*!< @brief ESA algrorithm */
#define NSK2HDI_CMCHANNEL_LDA_ALGORITHM_TYPE      0x03U /*!< @brief LDA algrorithm  */
/** @}
* End of NSK2HDI_CMCHANNEL_ALGORITHM_TYPE group belonging
*/

/** 
** @addtogroup NSK2HDI_CMCHANNEL_CAPABILITY_FLAGS CmChannel Capability Flags
** @{
*/
#define NSK2HDI_CMCHANNEL_IV1_CONFIGURE_CAPABILITY_FLAG     0x01U  /*!< @brief if set, CmChannel may be configured with the explicit IV1 via NSK2HDI_CmChannelDevice_Configure*/
#define NSK2HDI_CMCHANNEL_IV2_CONFIGURE_CAPABILITY_FLAG     0x02U  /*!< @brief if set, CmChannel may be configured with the explicit IV2 via NSK2HDI_CmChannelDevice_Configure*/
#define NSK2HDI_CMCHANNEL_IV_WRITEKEY_CAPABILITY_FLAG       0x04U  /*!< @brief if set, explicit IV and/or IV2 may be loaded into CmChannel via NSK2HDI_CmChannelDevice_WriteTransportKey.*/
#define NSK2HDI_CMCHANNEL_NON_NSK_CAPABILITY_FLAG           0x08U /*!< @brief if set, may use CmChannel with keys and switches from non-NSK source. Not used in HDI, reserved for use in CDI. */  
#define NSK2HDI_CMCHANNEL_NSK_CAPABILITY_FLAG               0x10U /*!< @brief if set, may use CmChannel with NSK (via NSK2HDI_CmChannel_WriteTransportKey) .if not set, may use CmChannel with keys and switches from non-NSK source. Not used in HDI, reserved for use in CDI. */
/** @}
* End of NSK2HDI_CMCHANNEL_CAPABILITY_FLAGS group belonging
*/


/** 
** @addtogroup NSK2HDI_CMCHANNELGROUP_REQUEST_IDS CmChannel Group Request IDs
** @{
*/
#define NSK2HDI_CMCHANNELGROUP_READ_ALL_DESCRIPTORS_REQUEST     0x00U  /*!< @brief ID of the request used for retrieving all CmChannelGroup properties */
/** @}
* End of NSK2HDI_CMCHANNELGROUP_REQUEST_IDS group belonging
*/

/** 
** @addtogroup NSK2HDI_CMCHANNELGROUP_DESC CmChannel Group Descriptor Tags
** @{
*/
#define NSK2HDI_CMCHANNELGROUP_CAPABILITY_DESC_TAG        0x01U  /*!< @brief Tag of the descriptor carrying the capability flags. */
#define NSK2HDI_CMCHANNELGROUP_ALGORITHM_RECORD_DESC_TAG        0x02U  /*!< @brief Tag of the descriptor carrying the CmChannel LDA record information. There is one record per each pair of node and algorithm supported by CmChannel*/
/** @}
* End of NSK2HDI_CMCHANNELGROUP_DESC group belonging
*/

/** @}
* End of NSK2HDI_CMCHANNELGROUP_DEFS group belonging
*/

/*===========================================================================*/
/** @brief Creates an instance of the CmChannel Group device and returns its handle to the caller.

@param[in] CmChannelGroup_ID - the ID of the CmChannel Group device to open as returned by #NSK2HDI_CmChannelTableDevice_EnumerateChildren function.
@param[out] CmChannelGroup_handle - pointer to the device instance handle to be returned to a caller.

@retval #NSK2HDI_STATUS_OK if device instance is created successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if request failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_CmChannelGroupDevice_Open (  NSK2HDI_DEVICE_ID        CmChannelGroup_ID, 
                                                            NSK2HDI_HANDLE	         *CmChannelGroup_handle);

/*===========================================================================*/
/** @brief Destroys the specified instance of the CmChannel Group device.
Releases all the resources used by this device instance.

@param[in] CmChannelGroup_handle - handle of the device instance to be destroyed.

@retval #NSK2HDI_STATUS_OK if the device instance was destroyed or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if an invalid device handle was passed.
*/
typedef NSK2HDI_STATUS NSK2HDI_CmChannelGroupDevice_Close ( NSK2HDI_HANDLE CmChannelGroup_handle);


/*===========================================================================*/
/** @brief Retrieves the Properties of the CmChannel Group device.
If desc_size is zero, the function is used to retrieve the size of the buffer to be allocated for the properties, only and the desc parameter is ignored.

@param[in] CmChannelGroup_handle - a handle to the opened CmChannel Group device.
@param[in] request_id - the ID of the properties retrieval request.See \ref NSK2HDI_CMCHANNELGROUP_REQUEST_IDS for possible values.
@param[in, out] desc_size - size of the buffer allocated by the caller for the device properties as an input, actual size of the device properties returned by the device as an output.
@param[out] desc - pointer to the buffer receiving the device properties.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_CmChannelGroupDevice_GetProperties(  NSK2HDI_HANDLE CmChannelGroup_handle, 
                                                                    NDS_ULONG        request_id, 
                                                                    NDS_ULONG        *desc_size, 
                                                                    NDS_UBYTE	     *desc);

/** @}
* End of NSK2HDI_CMCHANNELGROUP group belonging
*/

/** 
** @addtogroup NSK2HDI_CMCHANNEL CmChannel Device
** @{
*/
/** 
** @addtogroup NSK2HDI_CMCHANNEL_DEF Definitions
** @{
*/

/** 
** @addtogroup NSK2HDI_CMCHANNEL_DESC CmChannel Descriptor Tags
** @{
*/
#define NSK2HDI_CMCHANNEL_IV1_DESC_TAG            0x01U  /*!< @brief IV1 for local scrambling/descrambling. */
#define NSK2HDI_CMCHANNEL_IV2_DESC_TAG            0x02U   /*!< @brief IV2 for local scrambling/descrambling.*/
#define NSK2HDI_CMCHANNEL_ALGORITHM_DESC_TAG      0x03U   /*!< @brief algorithm for scrambling/descrambling.*/
#define NSK2HDI_CMCHANNEL_IV_SIGNALING_DESC_TAG   0x04U   /*!< @brief IV signaling mode.*/
/** @}
* End of NSK2HDI_CMCHANNEL_DESC group belonging
*/
/** 
** @addtogroup NSK2HDI_CMCHANNEL_IV_SIGNALING_MODES CmChannel IV Signaling Modes
** @{
*/
#define NSK2HDI_CMCHANNEL_IV_NOT_SET_MODE         0x01U   /*!< @brief IV1/IV2 not configured. */
#define NSK2HDI_CMCHANNEL_IV_AT_CONFIG_MODE       0x02U   /*!< @brief IV1/2 are defined via Configure call.*/
#define NSK2HDI_CMCHANNEL_IV_AT_WRITEKEY_MODE     0x03U   /*!< @brief IV1/2 are defined via WriteTransportKey call.*/
/** @}
* End of NSK2HDI_CMCHANNEL_IV_SIGNALING_MODES group belonging
*/

/** @}
* End of NSK2HDI_CMCHANNEL_DEF group belonging
*/

/*===========================================================================*/
/** @brief Creates an instance of the CmChannel device and returns its handle to the caller.

@param[in] CmChannelGroup_ID - the ID of the CmChannelGroup device where the requested channel belongs to
@param[out] CmChannel_handle - pointer to the device instance handle to be returned to a caller.

@retval #NSK2HDI_STATUS_OK if device instance is created successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if request failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_CmChannelDevice_Open ( NSK2HDI_DEVICE_ID	CmChannelGroup_ID, 
                                                    NSK2HDI_HANDLE	*CmChannel_handle);


/*===========================================================================*/
/** @brief Destroys the specified instance of the CmChannel device.
Releases all the resources used by this device instance.

@param[in] CmChannel_handle - handle of the device instance to be destroyed.

@retval #NSK2HDI_STATUS_OK if the device instance was destroyed or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if an invalid device handle was passed.
*/
typedef NSK2HDI_STATUS NSK2HDI_CmChannelDevice_Close( NSK2HDI_HANDLE CmChannel_handle);


/*===========================================================================*/
/** @brief Configures the CmChannel device.

@param[in] CmChannel_handle - a handle to the opened CmChannel device.
@param[in] lda_desc_size - size of the lda descriptors buffer.
@param[in] lda_desc - buffer containing the descriptors configuring the local descrambler. See \ref NSK2HDI_CMCHANNEL_DESC for possible descriptors
@param[in] esa_desc_size - size of the esa descriptors buffer.
@param[in] esa_desc - buffer containing the descriptors configuring the esa descrambler. See \ref NSK2HDI_CMCHANNEL_DESC for possible descriptors
@param[in] lsa_desc_size - size of the lsa descriptors buffer.
@param[in] lsa_desc - buffer containing the descriptors configuring the local scrambler descriptors. See \ref NSK2HDI_CMCHANNEL_DESC for possible descriptors

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_CmChannelDevice_Configure(NSK2HDI_HANDLE   CmChannel_handle, 
                                                        NDS_ULONG         lda_desc_size, 
                                                        const NDS_UBYTE   *lda_desc,
                                                        NDS_ULONG         esa_desc_size, 
                                                        const NDS_UBYTE   *esa_desc,
                                                        NDS_ULONG         lsa_desc_size, 
                                                        const NDS_UBYTE   *lsa_desc);

/*===========================================================================*/
/** @brief Request the NSK device to copy the keys and the control switches from the NSK KTE to the NSK chip key table.

@param[in] CmChannel_handle - a handle to the opened CmChannel device.
@param[in] scb_in_out_size - the number of the entries in the scb_in and scb_out arrays. Each pair of the entries (scb_in[i], scb_out[i]) specifies the parity conversion between the incoming and outgoing packets.
@param[in] scb_in - array where each NDS_UBYTE entry signals input packet parity for which the key shall be written. 
                    Each entry in array can be set to #NSK2HDI_CMCHANNEL_CLEAR_PARITY, #NSK2HDI_CMCHANNEL_ODD_PARITY or NSK2HDI_CMCHANNEL_EVEN_PARITY.
@param[in] scb_out - array where each NDS_UBYTE entry signals the output packet parity for the corresponding input parity signaled by scb_in array.
                    Each entry in array can be set to #NSK2HDI_CMCHANNEL_CLEAR_PARITY, #NSK2HDI_CMCHANNEL_ODD_PARITY or NSK2HDI_CMCHANNEL_EVEN_PARITY.
@param[in] lda_desc_size - size of the lda descriptors buffer.
@param[in] lda_desc - buffer containing the descriptors configuring the IVs for local descrambler. See \ref NSK2HDI_CMCHANNEL_DESC for possible descriptors
@param[in] esa_desc_size - size of the esa descriptors buffer.
@param[in] esa_desc - buffer containing the descriptors configuring the IVs for the esa descrambler. See \ref NSK2HDI_CMCHANNEL_DESC for possible descriptors
@param[in] lsa_desc_size - size of the lsa descriptors buffer.
@param[in] lsa_desc - buffer containing the descriptors configuring the the IVs for local scrambler descriptors. See \ref NSK2HDI_CMCHANNEL_DESC for possible descriptors

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_CmChannelDevice_WriteTransportKey(
                                                                 NSK2HDI_HANDLE		 	CmChannel_handle,
                                                                 NDS_ULONG	 			scb_in_out_size,
                                                                 NDS_UBYTE	 			*scb_in,
                                                                 NDS_UBYTE	 			*scb_out,                                                                
                                                                 NDS_ULONG              lda_desc_size,
                                                                 const NDS_UBYTE        *lda_desc,
                                                                 NDS_ULONG              esa_desc_size,
                                                                 const NDS_UBYTE        *esa_desc,
                                                                 NDS_ULONG              lsa_desc_size,
                                                                 const NDS_UBYTE        *lsa_desc);

/*===========================================================================*/
/** @brief Disables scrambling and descrambling for the selected CmChannel

@param[in] CmChannel_handle - a handle to the opened CmChannel device.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_CmChannelDevice_InvalidateKeys(NSK2HDI_HANDLE CmChannel_handle);

/*===========================================================================*/
/** @brief Retrieves the cookie associated with CmChannel device.

@param[in] CmChannel_handle - a handle to the opened CmChannel device.
@param[out] cookie - pointer to the variable receiving the device cookie.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_CmChannelDevice_GetCookie(NSK2HDI_HANDLE	        CmChannel_handle,
                                                        void 		            **cookie);

/** @}
* End of NSK2HDI_CMCHANNEL group belonging
*/
/** 
** @addtogroup NSK2HDI_CAPIDPROXY CaPIDProxy Device
** @{
*/

/** 
** @addtogroup NSK2HDI_CAPIDPROXY_FLAGS CAPIDProxy Merge Mode Flags
** @{
*/
#define NSK2HDI_CAPIDPROXY_MERGED_CONTINUITY_MODE       0U  /*!< @brief continuity counter shall not be modified on the merged PID */
#define NSK2HDI_CAPIDPROXY_SEPARATE_CONTINUITY_MODE     1U  /*!< @brief the continuity counters shall be repaced for the merged PID such that the merged PID has valid continuity counters*/
/** @}
* End of NSK2HDI_CAPIDPROXY_FLAGS group belonging
*/
/*===========================================================================*/
/** @brief Creates an instance of the CaPIDProxy device and returns its handle to the caller.

@param[in] xconn - the associated x_connection.
@param[out] CaPIDProxy_handle - pointer to the device instance handle to be returned to a caller.

@retval #NSK2HDI_STATUS_OK if device instance is created successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if request failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_CaPIDProxy_Open (NDS_ULONG	xconn, NSK2HDI_HANDLE  *CaPIDProxy_handle);

/*===========================================================================*/
/** @brief Destroys the specified instance of the CaPIDProxy device.
Releases all the resources used by this device instance.

@param[in] CaPIDProxy_handle - handle of the device instance to be destroyed.

@retval #NSK2HDI_STATUS_OK if the device instance was destroyed or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if an invalid device handle was passed.
*/
typedef NSK2HDI_STATUS NSK2HDI_CaPIDProxy_Close (NSK2HDI_HANDLE CaPIDProxy_handle);

/*===========================================================================*/
/** @brief Sets the PID of the elementary stream for the CaPIDProxy device.

@param[in] CaPIDProxy_handle - a handle to the opened CaPIDProxy device.
@param[in] es_pid - primary elementary stream PID.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_CaPIDProxy_SetPrimaryStreamPID(  NSK2HDI_HANDLE  CaPIDProxy_handle, 
                                                                NDS_USHORT      es_pid);

/*===========================================================================*/
/** @brief Sets the CmChannel associated with primary elementary stream for the CaPIDProxy device.

@param[in] CaPIDProxy_handle - a handle to the opened CaPIDProxy device.
@param[in] CmChannel_cookie - a cookie associated with theCmChannel device to be used with the primary stream.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_CaPIDProxy_SetPrimaryStreamCmChannel(NSK2HDI_HANDLE  CaPIDProxy_handle, 
                                                                    void            *CmChannel_cookie); 

/*===========================================================================*/
/** @brief Removes the CmChannel associated with primary elementary stream for the CaPIDProxy device.

@param[in] CaPIDProxy_handle - a handle to the opened CaPIDProxy device.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_CaPIDProxy_RemovePrimaryStreamCmChannel(NSK2HDI_HANDLE  CaPIDProxy_handle);

/*===========================================================================*/
/** @brief Configures secondary elementary stream from the CaPIDProxy device.

@param[in] CaPIDProxy_handle - a handle to the opened CaPIDProxy device.
@param[in] CmChannel_cookie - a cookie associated with the CmChannel device to be used with the secondary stream.
@param[in] es_pid - secondary elementary stream PID.
@param[in] merge_mode - set of flags controlling the merged stream requirements. See \ref NSK2HDI_CAPIDPROXY_FLAGS for possible values.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_CaPIDProxy_SetSecondaryStream(NSK2HDI_HANDLE  CaPIDProxy_handle, 
                                                            void             *CmChannel_cookie, 
                                                            NDS_USHORT       es_pid,
                                                            NDS_ULONG        merge_mode);

/*===========================================================================*/
/** @brief Removes secondary elementary stream from the CaPIDProxy device.

@param[in] CaPIDProxy_handle - a handle to the opened CaPIDProxy device.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_CaPIDProxy_RemoveSecondaryStream(NSK2HDI_HANDLE  CaPIDProxy_handle);


/** @}
* End of NSK2HDI_CAPIDPROXY group belonging
*/

/** @}
* End of NSK2HDI_CM group belonging
*/

/** 
** @addtogroup NSK2HDI_M2M M2M Devices
** @{
*/
/** 
** @addtogroup NSK2HDI_M2MTABLE M2MChannel Table Device
** @{
*/

/*===========================================================================*/
/** @brief Creates an instance of the M2MChannel Table device and returns its handle to the caller.

@param[in] M2MTable_ID - the ID of theM2MChannel Table device to open as returned by #NSK2HDI_CaSoCGroupDevice_EnumerateChildren function.
@param[out] M2MTable_handle - pointer to the device instance handle to be returned to a caller.

@retval #NSK2HDI_STATUS_OK if device instance is created successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if request failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_M2MTableDevice_Open (  NSK2HDI_DEVICE_ID        M2MTable_ID, 
                                                      NSK2HDI_HANDLE	   *M2MTable_handle);

/*===========================================================================*/
/** @brief Destroys the specified instance of the M2MChannel Table device.
Releases all the resources used by this device instance.

@param[in] M2MTable_handle - handle of the device instance to be destroyed.

@retval #NSK2HDI_STATUS_OK if the device instance was destroyed or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if an invalid device handle was passed.
*/
typedef NSK2HDI_STATUS NSK2HDI_M2MTableDevice_Close ( NSK2HDI_HANDLE M2MTable_handle);


/*===========================================================================*/
/** @brief Enumerates M2MChannel Table device children. Retrieves number of children and associated info for each child.
If child_device_num is zero, the function is used to retrieve the number of children only and the child_device parameter is ignored.

@param[in] M2MTable_handle - a handle to the opened M2MChannel Table device.
@param[in, out] child_device_num - the number of elements in the child_device array allocated by the caller as an input, the actual number of the M2MChannel Table child devices as an output.
@param[out] child_device - array that receives the child info for the M2MChannel Table child devices.  

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/

typedef NSK2HDI_STATUS NSK2HDI_M2MTableDevice_EnumerateChildren( NSK2HDI_HANDLE     M2MTable_handle, 
                                                                     NDS_ULONG              *child_device_num, 
                                                                     NSK2HDI_CHILD_DEVICE   *child_device);


/** @}
* End of NSK2HDI_M2MTABLE group belonging
*/
/** 
** @addtogroup NSK2HDI_M2MCHANNELGROUP M2MChannel Group Device
** @{
*/
/** 
** @addtogroup NSK2HDI_M2MCHANNELGROUP_DEFS Definitions
** @{
*/

#define NSK2HDI_M2MCHANNEL_USER_CONTEXT_SIZE 0x08U  /*!< @brief the size of the user context field  */

/** 
** @addtogroup NSK2HDI_M2MCHANNELGROUP_REQUEST_IDS M2MChannelGroup Request IDs
** @{
*/
#define NSK2HDI_M2MCHANNELGROUP_READ_ALL_DESCRIPTORS_REQUEST     0x00U  /*!< @brief ID of the request used for retrieving all M2MChannelGroup properties */
/** @}
* End of NSK2HDI_M2MCHANNELGROUP_REQUEST_IDS group belonging
*/

/** 
** @addtogroup NSK2HDI_M2MCHANNELGROUP_DESC M2MChannelGroup Descriptor Tags
** @{
*/
#define NSK2HDI_M2MCHANNELGROUP_CAPABILITY_DESC_TAG              0x01U  /*!< @brief Tag of the descriptor carrying the capability flags. */
#define NSK2HDI_M2MCHANNELGROUP_ALGORITHM_RECORD_DESC_TAG        0x02U  /*!< @brief Tag of the descriptor carrying the M2MChannel algorithm capabilities information.*/
/** @}
* End of NSK2HDI_M2MCHANNELGROUP_DESC group belonging
*/

/** 
** @addtogroup NSK2HDI_M2MCHANNELGROUP_CAPS M2MChannelGroup Capability Flags
** @{
*/
#define NSK2HDI_M2MCHANNELGROUP_NSK_CAPABILITY_FLAG                   0x01000000   /*!< @brief Channels within the group may be used only for NSK related opeartions. */
#define NSK2HDI_M2MCHANNELGROUP_NON_NSK_CAPABILITY_FLAG               0x02U        /*!< @brief Channels within the group may be used only for non NSK related opeartions. */
#define NSK2HDI_M2MCHANNELGROUP_ANY_OPERATION_ALLOWED_CAPABILITY_FLAG 0x04U        /*!< @brief Channels within the group may be used with NSK2HDI_M2MCHANNEL_ANY_OPERATION. */
/** @}
* End of NSK2HDI_M2MCHANNELGROUP_CAPS group belonging
*/

/** @}
* End of NSK2HDI_M2MCHANNELGROUP_DEFS group belonging
*/

/*===========================================================================*/
/** @brief Creates an instance of the M2MChannel Group device and returns its handle to the caller.

@param[in] M2MChannelGroup_ID - the ID of the M2MChannel Group device to open as returned by #NSK2HDI_M2MTableDevice_EnumerateChildren function.
@param[out] M2MChannelGroup_handle - pointer to the device instance handle to be returned to a caller.

@retval #NSK2HDI_STATUS_OK if device instance is created successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if request failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_M2MChannelGroupDevice_Open (  NSK2HDI_DEVICE_ID        M2MChannelGroup_ID, 
                                                            NSK2HDI_HANDLE	       *M2MChannelGroup_handle);

/*===========================================================================*/
/** @brief Destroys the specified instance of the M2MChannel Group device.
Releases all the resources used by this device instance.

@param[in] M2MChannelGroup_handle - handle of the device instance to be destroyed.

@retval #NSK2HDI_STATUS_OK if the device instance was destroyed or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if an invalid device handle was passed.
*/
typedef NSK2HDI_STATUS NSK2HDI_M2MChannelGroupDevice_Close ( NSK2HDI_HANDLE M2MChannelGroup_handle);


/*===========================================================================*/
/** @brief Retrieves the Properties of the M2MChannel Group device.
If desc_size is zero, the function is used to retrieve the size of the buffer to be allocated for the properties, only and the desc parameter is ignored.

@param[in] M2MChannelGroup_handle - a handle to the opened M2MChannel Group device.
@param[in] request_id - the ID of the properties retrieval request.See \ref NSK2HDI_M2MCHANNELGROUP_REQUEST_IDS for possible values.
@param[in, out] desc_size - size of the buffer allocated by the caller for the device properties as an input, actual size of the device properties returned by the device as an output.
@param[out] desc - pointer to the buffer receiving the device properties.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_M2MChannelGroupDevice_GetProperties(  NSK2HDI_HANDLE M2MChannelGroup_handle, 
                                                                    NDS_ULONG        request_id, 
                                                                    NDS_ULONG        *desc_size, 
                                                                    NDS_UBYTE	     *desc);

/** @}
* End of NSK2HDI_M2MCHANNELGROUP group belonging
*/

/** 
** @addtogroup NSK2HDI_M2MCHANNEL M2MChannel Channel Device
** @{
*/
/** @addtogroup NSK2HDI_M2MCHANNEL_OPERATION M2MChannel Operation
** @{
*/
#define NSK2HDI_M2MCHANNEL_DECRYPT_OPERATION        0x01U /*!< @brief Data chunk descrambling operation */
#define NSK2HDI_M2MCHANNEL_ENCRYPT_OPERATION        0x02U /*!< @brief Data chunk scrambling operation */
#define NSK2HDI_M2MCHANNEL_ANY_OPERATION            0x04U /*!< @brief Data chunk any operation */
/** @}
* End of NSK2HDI_M2MCHANNEL_OPERATION group belonging
*/
/** 
** @addtogroup NSK2HDI_M2MCHANNEL_DESC_TAGS M2MChannel Descriptor Tags
** @{
*/
#define NSK2HDI_M2MCHANNEL_ALGORITHM_DESC_TAG                      0x01U /*!< @brief M2MChannel algrorithm descriptor */
#define NSK2HDI_M2MCHANNEL_OPERATION_DESC_TAG                      0x02U /*!< @brief M2MChannel operation. See \ref NSK2HDI_M2MCHANNEL_OPERATION for possible values */
#define NSK2HDI_M2MCHANNEL_CLEAR_SWKEY_DESC_TAG                    0x03U /*!< @brief M2MChannel Clear SW key. Can be set only for M2MChannels supporting non NSK mode */
/** @}
* End of NSK2HDI_M2MCHANNEL_DESC_TAGS group belonging
*/

/*===========================================================================*/
/** @brief Creates an instance of the M2MChannel device and returns its handle to the caller.

@param[in] M2MChannelGroup_ID - the ID of the M2MChannelGroup device where the requested channel belongs to
@param[out] M2MChannel_handle - pointer to the device instance handle to be returned to a caller.

@retval #NSK2HDI_STATUS_OK if device instance is created successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if request failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_M2MChannelDevice_Open (  NSK2HDI_DEVICE_ID	M2MChannelGroup_ID, 
                                                        NSK2HDI_HANDLE	    *M2MChannel_handle);


/*===========================================================================*/
/** @brief Destroys the specified instance of the M2MChannel device.
Releases all the resources used by this device instance.

@param[in] M2MChannel_handle - handle of the device instance to be destroyed.

@retval #NSK2HDI_STATUS_OK if the device instance was destroyed or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if an invalid device handle was passed.
*/
typedef NSK2HDI_STATUS NSK2HDI_M2MChannelDevice_Close( NSK2HDI_HANDLE M2MChannel_handle);


/*===========================================================================*/
/** @brief Configures the M2MChannel device.

@param[in] M2MChannel_handle - a handle to the opened M2MChannel device.
@param[in] desc_size - size of the "desc" buffer.
@param[in] desc - buffer containing the descriptors configuring the local descrambler. See \ref NSK2HDI_M2MCHANNEL_DESC_TAGS for possible descriptors

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_M2MChannelDevice_Configure(NSK2HDI_HANDLE   M2MChannel_handle, 
                                                          NDS_ULONG         desc_size,
                                                          const NDS_UBYTE	*desc);

/*===========================================================================*/
/** @brief Request the NSK device to copy the keys and the control switches from the NSK KTE to the NSK chip key table.
Can be applied only to M2MChannels having the NSK capability.

@param[in] M2MChannel_handle - a handle to the opened M2MChannel device.
@param[in] desc_size - size of the descriptors buffer.
@param[in] desc - buffer containing the descriptors. See \ref NSK2HDI_M2MCHANNEL_DESC_TAGS for possible descriptors

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_M2MChannelDevice_NSKWriteM2MKey(NSK2HDI_HANDLE	M2MChannel_handle,
                                                         NDS_ULONG		 		desc_size,
                                                         const NDS_UBYTE		*desc);
/*===========================================================================*/
/** @brief Sets the clear SW key to M2MChannel.
Can be applied only to M2MChannels having the non-NSK capability

@param[in] M2MChannel_handle - a handle to the opened M2MChannel device.
@param[in] desc_size - size of the descriptors buffer.
@param[in] desc - buffer containing the descriptors. See \ref NSK2HDI_M2MCHANNEL_DESC_TAGS for possible descriptors

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_M2MChannelDevice_WriteM2MKey(NSK2HDI_HANDLE		 	M2MChannel_handle,
                                                         NDS_ULONG		 		desc_size,
                                                         const NDS_UBYTE		*desc);
/*===========================================================================*/
/** @brief Retrieves the cookie associated with M2MChannel device.

@param[in] M2MChannel_handle - a handle to the opened M2MChannel device.
@param[out] cookie - pointer to the variable receiving the device cookie.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_M2MChannelDevice_GetCookie(NSK2HDI_HANDLE		 	M2MChannel_handle,
                                                           void 		            **cookie);
/** @}
* End of NSK2HDI_M2MCHANNEL group belonging
*/

/** 
** @addtogroup NSK2HDI_DMA DMA Device
** @{
*/
/** @addtogroup NSK2HDI_DMA_DEF DMA Definitions
** @{
*/
#define NSK2HDI_DMA_UNLIMITED_DATA_SIZE             0xFFFFFFFFU    /*!< @brief Data chunk size is unlimited  */
/** @}
* End of NSK2HDI_DMA_DEF group belonging
*/
/** 
** @addtogroup NSK2HDI_DMA_GET_PROPERTIES_REQUEST  DMA Request IDs
** @{
*/
#define NSK2HDI_DMA_READ_ALL_DESCRIPTORS_REQUEST 0x00U /*!< @brief request for reading all DMA descriptors*/
/** @}
* End of NSK2HDI_DMA_GET_PROPERTIES_REQUEST group belonging
*/
/** @addtogroup NSK2HDI_DMA_OPERATION DMA Operation
** @{
*/
#define NSK2HDI_DMA_ENCRYPT_OPERATION  0x01U    /*!< @brief Data chunk decrypt request  */
#define NSK2HDI_DMA_DECRYPT_OPERATION  0x02U    /*!< @brief Data chunk encrypt request  */
/** @}
* End of NSK2HDI_DMA_OPERATION group belonging
*/

/** @addtogroup NSK2HDI_DMA_MEMORY_TYPE Data Chunk Memory Type
** @{
*/
#define NSK2HDI_DMA_CONTIGUOUS_MEMORY_TYPE      0x01U    /*!< @brief Data chunk memory type is contiguous  */
#define NSK2HDI_DMA_NONCONTIGUOUS_MEMORY_TYPE   0x02U    /*!< @brief Data chunk memory type is non-contiguous  */
/** @}
* End of NSK2HDI_DMA_MEMORY_TYPE group belonging
*/

/** @addtogroup NSK2HDI_DMA_CAPABILITIES Device Capabilities
** @{
*/
#define NSK2HDI_DMA_NON_CONTIGUOUS_MEMORY_ALLOWED_FLAG      0x01U    /*!< @brief DMA device supports non-contiguous memory*/
/** @}
* End of NSK2HDI_DMA_CAPABILITIES group belonging
*/

/** 
** @addtogroup NSK2HDI_DMA_DESCRIPTOR_TAGS  DMA Descriptor Tags
** @{
*/
#define NSK2HDI_DMA_IV1_DESC_TAG                  0x01U /*!< @brief Descriptor carrying IV1 */
#define NSK2HDI_DMA_IV2_DESC_TAG                  0x02U /*!< @brief Descriptor carrying IV2 */
#define NSK2HDI_DMA_OPERATION_DESC_TAG            0x03U /*!< @brief DMA operation. See \ref NSK2HDI_DMA_OPERATION for possible values */
#define NSK2HDI_DMA_SRC_MEMORY_TYPE_DESC_TAG      0x04U /*!< @brief Data chunk memory type. See \ref NSK2HDI_DMA_MEMORY_TYPE for possible values */
#define NSK2HDI_DMA_DST_MEMORY_TYPE_DESC_TAG      0x05U /*!< @brief Data chunk memory type. See \ref NSK2HDI_DMA_MEMORY_TYPE for possible values */
#define NSK2HDI_DMA_CAPABILITY_DESC_TAG           0x06U /*!< @brief DMA device capability descriptor. See \ref NSK2HDI_DMA_CAPABILITIES for possible values */
/** @}
* End of NSK2HDI_DMA_DESCRIPTOR_TAGS group belonging
*/

 

/*===========================================================================*/
/** @brief Creates an instance of the DMA device and returns its handle to the caller.

@param[in]  DMA_ID - the ID of the DMA device as returned by #NSK2HDI_M2MTableDevice_EnumerateChildren.
@param[out] DMA_handle - pointer to the device instance handle to be returned to a caller.

@retval #NSK2HDI_STATUS_OK if device instance is created successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if request failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_DMADevice_Open (  NSK2HDI_DEVICE_ID	DMA_ID, 
                                                  NSK2HDI_HANDLE	*DMA_handle);


/*===========================================================================*/
/** @brief Destroys the specified instance of the M2MChannel device.
Releases all the resources used by this device instance.

@param[in] DMA_handle - handle of the device instance to be destroyed.

@retval #NSK2HDI_STATUS_OK if the device instance was destroyed or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if an invalid device handle was passed.
*/
typedef NSK2HDI_STATUS NSK2HDI_DMADevice_Close( NSK2HDI_HANDLE DMA_handle);


/*===========================================================================*/
/** @brief Retrieves the Properties of the DMA device.
If desc_size is zero, the function is used to retrieve the size of the buffer to be allocated for the properties, only and the desc parameter is ignored.

@param[in] DMA_handle - a handle to the opened DMA device.
@param[in] request_id - the ID of the properties retrieval request.See \ref NSK2HDI_DMA_GET_PROPERTIES_REQUEST for possible values.
@param[in, out] desc_size - size of the buffer allocated by the caller for the device properties.
@param[out] desc - pointer to the buffer receiving the descriptors with device properties. See \ref NSK2HDI_DMA_DESCRIPTOR_TAGS

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_DMADevice_GetProperties( NSK2HDI_HANDLE  DMA_handle, 
                                                   NDS_ULONG        request_id, 
                                                   NDS_ULONG        *desc_size, 
                                                   NDS_UBYTE	     *desc);

/*===========================================================================*/
/** @brief Writes the data to the DMA device.

@param[in] DMA_handle - a handle to the opened M2MChannel device.
@param[in] M2MChannel_cookie -  a pointer that is associated with M2M channel used for this DMA operation. See \ref NSK2HDI_M2MChannelDevice_GetCookie
@param[in] desc_size - size of the "desc" buffer.
@param[in] desc - buffer containing the descriptors configuring the local descrambler. See \ref NSK2HDI_M2MCHANNEL_DESC_TAGS for possible descriptors
@param[in] data_size - size of the  written data.
@param[in] in_data_buffer -     buffer containing the data to be processed
@param[in] out_data_buffer -    buffer intended for processed data

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
typedef NSK2HDI_STATUS NSK2HDI_DMADevice_WriteData(
	                                            NSK2HDI_HANDLE   	DMA_handle,
	                                            void        		*M2MChannel_cookie,
	                                            NDS_ULONG        	desc_size,
	                                            const NDS_UBYTE 	*desc,
                                                NDS_ULONG         data_size,
                                                NDS_UBYTE         *in_data,
                                                NDS_UBYTE         *out_data);



/** @}
* End of NSK2HDI_DMA group belonging
*/

/** @}
* End of NSK2HDI_M2M group belonging
*/

/** 
** @addtogroup NSK2HDI_CA_MASTER CA Master Device
** @{
*/
/** 
** @addtogroup NSK2HDI_CA_MASTER_DEFINITIONS CA Master Device Definitions
** @{
*/

/** 
** @addtogroup NSK2HDI_CA_MASTER_FUNCTIONS_ID  Virtual Function IDs
** @{
*/
#define NSK2HDI_CASOCGROUPDEVICE_OPEN_FUNC_ID                               0x00000011U /*!< @brief ID of the registered function of the #NSK2HDI_CaSoCGroupDevice_Open type */
#define NSK2HDI_CASOCGROUPDEVICE_CLOSE_FUNC_ID                              0x00000012U /*!< @brief ID of the registered function of the #NSK2HDI_CaSoCGroupDevice_Close type */
#define NSK2HDI_CASOCGROUPDEVICE_ENUMERATECHILDREN_FUNC_ID                  0x00000014U /*!< @brief ID of the registered function of the #NSK2HDI_CaSoCGroupDevice_EnumerateChildren type */
#define NSK2HDI_CASOCGROUPDEVICE_REGISTERENUMERATIONCHANGECB_FUNC_ID        0x00000015U /*!< @brief ID of the registered function of the #NSK2HDI_CaSoCGroupDevice_RegisterEnumerationChangeCB type */
#define NSK2HDI_OTPDEVICE_OPEN_FUNC_ID                                      0x00000020U /*!< @brief ID of the registered function of the #NSK2HDI_OtpDevice_Open type */
#define NSK2HDI_OTPDEVICE_CLOSE_FUNC_ID                                     0x00000021U /*!< @brief ID of the registered function of the #NSK2HDI_OtpDevice_Close type */
#define NSK2HDI_OTPDEVICE_GETPROPERTIES_FUNC_ID                             0x00000022U /*!< @brief ID of the registered function of the #NSK2HDI_OtpDevice_GetProperties type */
#define NSK2HDI_NSKDEVICE_OPEN_FUNC_ID                                      0x00000030U /*!< @brief ID of the registered function of the #NSK2HDI_NskDevice_Open type */
#define NSK2HDI_NSKDEVICE_CLOSE_FUNC_ID                                     0x00000031U /*!< @brief ID of the registered function of the #NSK2HDI_NskDevice_Close type */
#define NSK2HDI_NSKDEVICE_READ8_FUNC_ID                                     0x00000032U /*!< @brief ID of the registered function of the #NSK2HDI_NskDevice_Read8 type */
#define NSK2HDI_NSKDEVICE_WRITE8_FUNC_ID                                    0x00000033U /*!< @brief ID of the registered function of the #NSK2HDI_NskDevice_Write8 type */
#define NSK2HDI_NSKDEVICE_READ32_FUNC_ID                                    0x00000034U /*!< @brief ID of the registered function of the #NSK2HDI_NskDevice_Read32 type */
#define NSK2HDI_NSKDEVICE_WRITE32_FUNC_ID                                   0x00000035U /*!< @brief ID of the registered function of the #NSK2HDI_NskDevice_Write32 type */
#define NSK2HDI_NSKDEVICE_EXECUTECMD_FUNC_ID                                0x00000036U /*!< @brief ID of the registered function of the #NSK2HDI_NskDevice_ExecuteCmd type */
#define NSK2HDI_NSKDEVICE_REGISTEREVENTCB_FUNC_ID                           0x00000037U /*!< @brief ID of the registered function of the #NSK2HDI_NskDevice_RegisterEventCB type */
#define NSK2HDI_NSKDEVICE_ACKNOWLEDGEKTE_FUNC_ID                            0x00000038U /*!< @brief ID of the registered function of the #NSK2HDI_NskDevice_AcknowledgeKte type */
#define NSK2HDI_NSKDEVICE_BASICINITIALIZATIONCOMPLETE_FUNC_ID               0x00000039U /*!< @brief ID of the registered function of the #NSK2HDI_NskDevice_BasicInitializationComplete type */
#define NSK2HDI_NSKDEVICE_UPDATENVCOUNTER_FUNC_ID                           0x0000003AU /*!< @brief ID of the registered function of the #NSK2HDI_NskDevice_UpdateNvCounter type */
#define NSK2HDI_NSKDEVICE_WRITEOTPKEY_FUNC_ID                               0x0000003BU /*!< @brief ID of the registered function of the #NSK2HDI_NskDevice_WriteOtpKey type */
#define NSK2HDI_NSKDEVICE_SETONESHOTGENINBITS_FUNC_ID                       0x0000003CU /*!< @brief ID of the registered function of the #NSK2HDI_NskDevice_SetOneShotGenInBits type */
#define NSK2HDI_CMCHANNELTABLEDEVICE_OPEN_FUNC_ID                           0x00000040U /*!< @brief ID of the registered function of the #NSK2HDI_CmChannelTableDevice_Open type */
#define NSK2HDI_CMCHANNELTABLEDEVICE_CLOSE_FUNC_ID                          0x00000041U /*!< @brief ID of the registered function of the #NSK2HDI_CmChannelTableDevice_Close type */
#define NSK2HDI_CMCHANNELTABLEDEVICE_ENUMERATECHILDREN_FUNC_ID              0x00000042U /*!< @brief ID of the registered function of the #NSK2HDI_CmChannelTableDevice_EnumerateChildren type */
#define NSK2HDI_CMCHANNELGROUPDEVICE_OPEN_FUNC_ID                           0x00000050U /*!< @brief ID of the registered function of the #NSK2HDI_CmChannelGroupDevice_Open type */
#define NSK2HDI_CMCHANNELGROUPDEVICE_CLOSE_FUNC_ID                          0x00000051U /*!< @brief ID of the registered function of the #NSK2HDI_CmChannelGroupDevice_Close type */
#define NSK2HDI_CMCHANNELGROUPDEVICE_GETPROPERTIES_FUNC_ID                  0x00000052U /*!< @brief ID of the registered function of the #NSK2HDI_CmChannelGroupDevice_GetProperties type */
#define NSK2HDI_CMCHANNELDEVICE_OPEN_FUNC_ID                                0x00000060U /*!< @brief ID of the registered function of the #NSK2HDI_CmChannelDevice_Open type */
#define NSK2HDI_CMCHANNELDEVICE_CLOSE_FUNC_ID                               0x00000061U /*!< @brief ID of the registered function of the #NSK2HDI_CmChannelDevice_Close type */
#define NSK2HDI_CMCHANNELDEVICE_CONFIGURE_FUNC_ID                           0x00000062U /*!< @brief ID of the registered function of the #NSK2HDI_CmChannelDevice_Configure type */
#define NSK2HDI_CMCHANNELDEVICE_WRITETRANSPORTKEY_FUNC_ID                   0x00000063U /*!< @brief ID of the registered function of the #NSK2HDI_CmChannelDevice_WriteTransportKey type */
#define NSK2HDI_CMCHANNELDEVICE_INVALIDATEKEYS_FUNC_ID                      0x00000064U /*!< @brief ID of the registered function of the #NSK2HDI_CmChannelDevice_InvalidateKeys type */
#define NSK2HDI_CMCHANNELDEVICE_GETCOOKIE_FUNC_ID                           0x00000065U /*!< @brief ID of the registered function of the #NSK2HDI_CmChannelDevice_GetCookie type */
#define NSK2HDI_CAPIDPROXY_OPEN_FUNC_ID                                     0x00000070U /*!< @brief ID of the registered function of the #NSK2HDI_CaPIDProxy_Open type */
#define NSK2HDI_CAPIDPROXY_CLOSE_FUNC_ID                                    0x00000071U /*!< @brief ID of the registered function of the #NSK2HDI_CaPIDProxy_Close type */
#define NSK2HDI_CAPIDPROXY_SETPRIMARYSTREAMPID_FUNC_ID                      0x00000072U /*!< @brief ID of the registered function of the #NSK2HDI_CaPIDProxy_SetPrimaryStreamPID type */
#define NSK2HDI_CAPIDPROXY_SETPRIMARYSTREAMCMCHANNEL_FUNC_ID                0x00000073U /*!< @brief ID of the registered function of the #NSK2HDI_CaPIDProxy_SetPrimaryStreamCmChannel type */
#define NSK2HDI_CAPIDPROXY_REMOVEPRIMARYSTREAMCMCHANNEL_FUNC_ID             0x00000074U /*!< @brief ID of the registered function of the #NSK2HDI_CaPIDProxy_RemovePrimaryStreamCmChannel type */
#define NSK2HDI_CAPIDPROXY_SETSECONDARYSTREAM_FUNC_ID                       0x00000075U /*!< @brief ID of the registered function of the #NSK2HDI_CaPIDProxy_SetSecondaryStream type */
#define NSK2HDI_CAPIDPROXY_REMOVESECONDARYSTREAM_FUNC_ID                    0x00000076U /*!< @brief ID of the registered function of the #NSK2HDI_CaPIDProxy_RemoveSecondaryStream type */
#define NSK2HDI_M2MTABLEDEVICE_OPEN_FUNC_ID                                 0x00000080U /*!< @brief ID of the registered function of the #NSK2HDI_M2MTableDevice_Open type */
#define NSK2HDI_M2MTABLEDEVICE_CLOSE_FUNC_ID                                0x00000081U /*!< @brief ID of the registered function of the #NSK2HDI_M2MTableDevice_Close type */
#define NSK2HDI_M2MTABLEDEVICE_ENUMERATECHILDREN_FUNC_ID                    0x00000082U /*!< @brief ID of the registered function of the #NSK2HDI_M2MTableDevice_EnumerateChildren type */
#define NSK2HDI_M2MCHANNELGROUPDEVICE_OPEN_FUNC_ID                          0x00000090U /*!< @brief ID of the registered function of the #NSK2HDI_M2MChannelGroupDevice_Open type */
#define NSK2HDI_M2MCHANNELGROUPDEVICE_CLOSE_FUNC_ID                         0x00000091U /*!< @brief ID of the registered function of the #NSK2HDI_M2MChannelGroupDevice_Close type */
#define NSK2HDI_M2MCHANNELGROUPDEVICE_GETPROPERTIES_FUNC_ID                 0x00000092U /*!< @brief ID of the registered function of the #NSK2HDI_M2MChannelGroupDevice_GetProperties type */
#define NSK2HDI_M2MCHANNELDEVICE_OPEN_FUNC_ID                               0x000000A0U /*!< @brief ID of the registered function of the #NSK2HDI_M2MChannelDevice_Open type */
#define NSK2HDI_M2MCHANNELDEVICE_CLOSE_FUNC_ID                              0x000000A1U /*!< @brief ID of the registered function of the #NSK2HDI_M2MChannelDevice_Close type */
#define NSK2HDI_M2MCHANNELDEVICE_CONFIGURE_FUNC_ID                          0x000000A2U /*!< @brief ID of the registered function of the #NSK2HDI_M2MChannelDevice_Configure type */
#define NSK2HDI_M2MCHANNELDEVICE_WRITETM2MKEY_FUNC_ID                       0x000000A3U /*!< @brief ID of the registered function of the #NSK2HDI_M2MChannelDevice_WriteM2MKey type */
#define NSK2HDI_M2MCHANNELDEVICE_NSKWRITETM2MKEY_FUNC_ID                    0x000000A4U /*!< @brief ID of the registered function of the #NSK2HDI_M2MChannelDevice_NSKWriteM2MKey type */
#define NSK2HDI_M2MCHANNELDEVICE_GETCOOKIE_FUNC_ID                          0x000000A5U /*!< @brief ID of the registered function of the #NSK2HDI_M2MChannelDevice_GetCookie type */
#define NSK2HDI_DMADEVICE_OPEN_FUNC_ID                                      0x000000B0U /*!< @brief ID of the registered function of the #NSK2HDI_DMADevice_Open type */
#define NSK2HDI_DMADEVICE_CLOSE_FUNC_ID                                     0x000000B1U /*!< @brief ID of the registered function of the #NSK2HDI_DMADevice_Close type */
#define NSK2HDI_DMADEVICE_GETPROPERTIES_FUNC_ID                             0x000000B2U /*!< @brief ID of the registered function of the #NSK2HDI_DMADevice_GetProperties type */
#define NSK2HDI_DMADEVICE_WRITEDATA_FUNC_ID                                 0x000000B3U /*!< @brief ID of the registered function of the #NSK2HDI_DMADevice_WriteData type */
/** @}
* End of NSK2HDI_CA_MASTER_FUNCTIONS_ID group belonging
*/

/** 
** @addtogroup NSK2HDI_CA_MASTER_VFUNCTION Virtual Function
** @{
*/
/** @brief The virtual function typedef */ 
typedef NSK2HDI_STATUS  NSK2HDI_VFUNCTION(void);

/** @brief The NSK2HDI_VFUNCTION_ITEM structure used for defining the specific virtual function 
exposed by registered device or one of its sub-devices and the associated ID 
*/
typedef struct _NSK2HDI_VFUNCTION
{
    NDS_ULONG           id;     /*!< @brief ID of the registered function. See \ref NSK2HDI_CA_MASTER_FUNCTIONS_ID for possible values */
    NSK2HDI_VFUNCTION   *func;  /*!< @brief pointer to registered function.*/
}NSK2HDI_VFUNCTION_ITEM;
/** @}
* End of NSK2HDI_CA_MASTER_VFUNCTION group belonging
*/

/** 
** @addtogroup NSK2HDI_CA_MASTER_VTABLE Virtual Function Table
** @{
*/
/** @brief The NSK2HDI_VTABLE structure used for setting the virtual table 
with the functions exposed by registered device and its sub-devices
*/
typedef struct _NSK2HDI_VTABLE
{
    NDS_ULONG               version;           /*!< @brief version of the structure. Must be set to zero.*/
    NDS_ULONG               num_vfunctions;    /*!< @brief number of the exposed functions*/
    NSK2HDI_VFUNCTION_ITEM  *vfunction;        /*!< @brief pointer to the list of the exposed function items*/
}NSK2HDI_VTABLE;

/** @}
* End of NSK2HDI_CA_MASTER_VTABLE group belonging
*/
/** @}
* End of NSK2HDI_CA_MASTER_DEFINITIONS group belonging
*/


/*===========================================================================*/
/** @brief DRIVER calls Verifier in order to register a new root device. 
The root device mast be already initialized before this call.

@param[in] device_type - the type of the registering device
@param[in] vtable - virtual table with the functions exposed by the child device
@param[out] root_device_id - the id assigned to the registered root device. 
@param[out] registration_handle - register device instance handle allocated by the drivers

The root_device_ID allocated by the Verifier for the SoC device should be used as a base 
for all device IDs returned by the drivers during via the enumeration functions from the provided vtable. 
The IDs assigned by the drivers should meet the following criteria:
root_device_id & NSK2HDI_CHILD_DEVICE_ID_MASK = child_device_id & NSK2HDI_CHILD_DEVICE_ID_MASK
where the value of root_device_id meets the following criteria:
root_device_id & NSK2HDI_CHILD_DEVICE_ID_MASK = root_device_id

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or 
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
NSK2HDI_STATUS NSK2HDI_CaMasterDevice_RegisterRootDevice (NDS_ULONG	            device_type,
                                                            NSK2HDI_VTABLE       *vtable, 
                                                            NSK2HDI_DEVICE_ID    *root_device_id,
                                                            NSK2HDI_HANDLE       *registration_handle);

/*===========================================================================*/
/** @brief Unregisters the registered child device.

@param[in] registration_handle - the handle of the previously registered child device.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or 
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
NSK2HDI_STATUS NSK2HDI_CaMasterDevice_UnregisterRootDevice (NSK2HDI_HANDLE registration_handle);


/** @}
* End of NSK2HDI_CA_MASTER group belonging
*/

/***********************************************************************************************************/
/***********************************************************************************************************/
/********************************           HAL             ***********************************************/
/***********************************************************************************************************/
/***********************************************************************************************************/
/** 
** @addtogroup NSK2HDI_NSK_HAL NSK HAL API
** @{
*/

/**
* @addtogroup NSK2HDI_HAL_COMMON Definitions
* @{
*/
/** @brief NSK2 HAL drivers event callback function 
*/
typedef void HALNSK2_NSKEVENT_CB_FN (NSK2HDI_HANDLE     Nsk_handle, 
                                    void	            *cookie);

/** @}
* End of NSK2HDI_HAL_COMMON group belonging
*/

/*===========================================================================*/
/** @brief Creates an instance of the HAL NSK device and returns its handle to the caller.

@param[in] Nsk_ID - HAL device instance identifier of the NSK device to be opened .
@param[out] Nsk_handle - pointer to the device instance handle to be returned to a caller.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or 
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
NSK2HDI_STATUS NSK2HAL_NskDevice_Open (NSK2HDI_DEVICE_ID     Nsk_ID, 
                                    NSK2HDI_HANDLE       *Nsk_handle);


/*===========================================================================*/
/** @brief Destroys the specified instance of the NSK device. 
Releases all the resources used by this device instance.

@param[in] Nsk_handle - handle of the device instance to be destroyed.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or 
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
NSK2HDI_STATUS NSK2HAL_NskDevice_Close (NSK2HDI_HANDLE Nsk_handle);


/*===========================================================================*/
/** @brief Reads data from registers

@param[in] Nsk_handle - a handle to the opened NSK device.
@param[in] register_offset - the offset within the device memory map at which to start the read.
@param[in] data_size - number of bytes to read, must be a positive multiple of 4.
@param[out] data - pointer to the buffer receiving the read data.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or 
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
NSK2HDI_STATUS NSK2HAL_NskDevice_Read8 (NSK2HDI_HANDLE     Nsk_handle, 
                                        NDS_ULONG           register_offset,
                                        NDS_ULONG           data_size,
                                        NDS_UBYTE            *data);


/*===========================================================================*/
/** @brief Writes data to the registers

@param[in] Nsk_handle - a handle to the opened NSK device.
@param[in] register_offset - the offset within the device memory map at which to start the read.
@param[in] data_size - number of bytes to write, must be a positive multiple of 4.
@param[out] data - pointer to the buffer containing the data to be written.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or 
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/

NSK2HDI_STATUS NSK2HAL_NskDevice_Write8 (NSK2HDI_HANDLE     Nsk_handle, 
                                        NDS_ULONG           register_offset,
                                        NDS_ULONG           data_size,
                                        const NDS_UBYTE	    *data);  


/*===========================================================================*/
/** @brief Reads data from registers

@param[in] Nsk_handle - a handle to the opened NSK device.
@param[in] register_offset - the offset within the device memory map at which to start the read, must be a multiple of 4.
@param[in] data_size - number of 32-bits words to read.
@param[out] data - pointer to the buffer receiving the read data.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or 
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
NSK2HDI_STATUS NSK2HAL_NskDevice_Read32 (NSK2HDI_HANDLE    Nsk_handle, 
                                        NDS_ULONG           register_offset,
                                        NDS_ULONG           data_size,
                                        NDS_ULONG            *data);

/*===========================================================================*/
/** @brief Writes data to the registers

@param[in] Nsk_handle - a handle to the opened NSK device.
@param[in] register_offset - the offset within the device memory map at which to start writing,must be a multiple of 4.
@param[in] data_size - number of bytes to write.
@param[out] data - pointer to the buffer containing the data to be written.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or 
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
NSK2HDI_STATUS NSK2HAL_NskDevice_Write32 (NSK2HDI_HANDLE        Nsk_handle, 
                                        NDS_ULONG                register_offset,
                                        NDS_ULONG                data_size,
                                        const NDS_ULONG	        *data);  



/*===========================================================================*/
/** @brief Registers the callback function for NSK events.
This method can be called once during the device instance life cycle.

@param[in] Nsk_handle - a handle to the opened NSK device.
@param[in] cb_routine - pointer to the #HALNSK2_NSKEVENT_CB_FN function.
@param[in] cookie -  a pointer that will be returned to the caller when the cb_routine is called.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or 
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
NSK2HDI_STATUS NSK2HAL_NskDevice_RegisterEventCB (NSK2HDI_HANDLE        Nsk_handle, 
                                                    HALNSK2_NSKEVENT_CB_FN   *cb_routine,
                                                    void                     *cookie);


/*===========================================================================*/
/** @brief Sends an acknowledgment signal to the NSK that the NSK chip has finished using the KTE contents.

@param[in] Nsk_handle - a handle to the opened NSK device.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or 
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
NSK2HDI_STATUS NSK2HAL_NskDevice_AcknowledgeKte(NSK2HDI_HANDLE Nsk_handle);

/*===========================================================================*/
/** @brief Called to indicate that NSK basic initialization is complete.

@param[in] Nsk_handle - a handle to the opened NSK device.
@param[in] stage - initialization stage. See \ref NSK2HDI_INITIALIZATION_STAGE for possible values.

@retval #NSK2HDI_STATUS_OK if the operation completed successfully or 
@retval #NSK2HDI_STATUS_INVALID_REQUEST if one or more parameters are invalid or
@retval #NSK2HDI_STATUS_FAILED if the operation failed.
*/
NSK2HDI_STATUS NSK2HAL_NskDevice_BasicInitializationComplete(NSK2HDI_HANDLE Nsk_handle, NDS_ULONG stage);


/** @}
* End of NSK2HDI_NSK_HAL group belonging
*/

/** @}
* End of NSK2HDI_CA_DEVICES group belonging
*/


#endif /*H_NSK_282*/
