#include "usbd_custom_hid_if.h"
#include "main.h"


extern uint8_t USB_RX_Buffer[64];
extern FlagStatus USBDatainReady;

/** Usb HID report descriptor. */
__ALIGN_BEGIN static uint8_t CUSTOM_HID_ReportDesc_FS[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END =
{
	    0x06, 0x00, 0xff,              // USAGE_PAGE (Generic Desktop)
	    0x09, 0x01,                    // USAGE (Vendor Usage 1)
	    0xa1, 0x01,                    // COLLECTION (Application)
	    0x85, 0x01,                    //   REPORT_ID (1)
	    0x09, 0x01,                    //   USAGE (Vendor Usage 1)
	    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
	    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
	    0x75, 0x08,                    //   REPORT_SIZE (8)
	    0x95, 0x01,                    //   REPORT_COUNT (1)
	    0xb1, 0x82,                    //   FEATURE (Data,Var,Abs,Vol)
	    0x85, 0x01,                    //   REPORT_ID (1)
	    0x09, 0x01,                    //   USAGE (Vendor Usage 1)
	    0x91, 0x82,                    //   OUTPUT (Data,Var,Abs,Vol)

	    0x85, 0x02,                    //   REPORT_ID (2)
	    0x09, 0x02,                    //   USAGE (Vendor Usage 2)
	    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
	    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
	    0x75, 0x08,                    //   REPORT_SIZE (8)
	    0x95, 0x01,                    //   REPORT_COUNT (1)
	    0xb1, 0x82,                    //   FEATURE (Data,Var,Abs,Vol)
	    0x85, 0x02,                    //   REPORT_ID (2)
	    0x09, 0x02,                    //   USAGE (Vendor Usage 2)
	    0x91, 0x82,                    //   OUTPUT (Data,Var,Abs,Vol)

	    0x85, 0x03,                    //   REPORT_ID (3)
	    0x09, 0x03,                    //   USAGE (Vendor Usage 3)
	    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
	    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
	    0x75, 0x08,                    //   REPORT_SIZE (8)
	    0x95, ARRAY2ST, //RPT3_COUNT,                    //   REPORT_COUNT (N)
	    0xb1, 0x82,                    //   FEATURE (Data,Var,Abs,Vol)
	    0x85, 0x03,                    //   REPORT_ID (3)
	    0x09, 0x03,                    //   USAGE (Vendor Usage 3)
	    0x91, 0x82,                    //   OUTPUT (Data,Var,Abs,Vol)

	    0x85, 0x04,                    //   REPORT_ID (4)
	    0x09, 0x04,                    //   USAGE (Vendor Usage 4)
	    0x75, 0x08,                    //   REPORT_SIZE (8)
	    0x95, ARRAY2HOST, //RPT4_COUNT,                    //   REPORT_COUNT (N)
	    0x81, 0x82,                    //   INPUT (Data,Var,Abs,Vol)
  0xC0    /*     END_COLLECTION	             */
};

USBD_HandleTypeDef  *hUsbDevice_0;	// Need for CUSTOM_HID_OutEvent_FS

extern USBD_HandleTypeDef hUsbDeviceFS;

static int8_t CUSTOM_HID_Init_FS(void);
static int8_t CUSTOM_HID_DeInit_FS(void);
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state);

USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops_FS =
{
  CUSTOM_HID_ReportDesc_FS,
  CUSTOM_HID_Init_FS,
  CUSTOM_HID_DeInit_FS,
  CUSTOM_HID_OutEvent_FS
};

static int8_t CUSTOM_HID_Init_FS(void)
{
	hUsbDevice_0 = &hUsbDeviceFS;	// Need for CUSTOM_HID_OutEvent_FS
  return (USBD_OK);
}

static int8_t CUSTOM_HID_DeInit_FS(void)
{
  return (USBD_OK);
}

/**
  * @brief  Manage the CUSTOM HID class events
  * @param  event_idx: Event index
  * @param  state: Event state
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state)
{ 
	USBD_CUSTOM_HID_HandleTypeDef     *hhid = (USBD_CUSTOM_HID_HandleTypeDef *)hUsbDevice_0->pClassData;
	for (uint8_t i = 0; i < ARRAY2ST+1; i++)
	{
	  USB_RX_Buffer[i] = hhid->Report_buf[i]; // To read usb data from PC
	}
	USBDatainReady = SET;
  return (USBD_OK);
}

/**
  * @brief  Send the report to the Host
  * @param  report: The report to be sent
  * @param  len: The report length
  * @retval USBD_OK if all operations are OK else USBD_FAIL
/*static int8_t USBD_CUSTOM_HID_SendReport_FS(uint8_t *report, uint16_t len)
{
  return USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, report, len);
}*/
