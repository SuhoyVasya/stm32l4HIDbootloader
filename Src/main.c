#include "stm32l4xx.h"                  // Device header
#include "main.h"
#include "usb_device.h"
#include "usbd_customhid.h"

extern USBD_HandleTypeDef hUsbDeviceFS;
//extern USBD_HandleTypeDef  *hUsbDevice_0;
uint8_t USB_RX_Buffer[64];
uint8_t USB_TX_Buffer[64];
FlagStatus USBDatainReady = RESET;
FlagStatus USBDataOutReady = RESET;


void SystemClock_Config(void);
static void MX_GPIO_Init(void);

int main(void)
{
 
	FlagStatus LED_Status;  
  HAL_Init();
	SystemClock_Config();
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();
	
  while (1)
  {
	  if(USBDatainReady == SET)
	  {
		  if(USB_RX_Buffer[1])
			  LED_Status = SET;
		  else
			  LED_Status = RESET;

		  switch(USB_RX_Buffer[0] & 0x3)
		  {
					case 1: 
						if(LED_Status == SET)
							HAL_GPIO_WritePin(LD_R_GPIO_Port, LD_R_Pin, GPIO_PIN_SET);
						else
		  	  		HAL_GPIO_WritePin(LD_R_GPIO_Port, LD_R_Pin, GPIO_PIN_RESET);
  	  	  	break;
					case 2:
						if(LED_Status == SET)
							HAL_GPIO_WritePin(LD_G_GPIO_Port, LD_G_Pin, GPIO_PIN_SET);
		  	  	else
		  	  		HAL_GPIO_WritePin(LD_G_GPIO_Port, LD_G_Pin, GPIO_PIN_RESET);
  	  	  	break;
					default: break;
		  }
		  USBDatainReady = RESET;
	  }

	  USB_TX_Buffer[1] = HAL_GPIO_ReadPin(JOY_CENTER_GPIO_Port, JOY_CENTER_Pin);
	  USB_TX_Buffer[2] = HAL_GPIO_ReadPin(JOY_LEFT_GPIO_Port, JOY_LEFT_Pin);
	  USB_TX_Buffer[3] = HAL_GPIO_ReadPin(JOY_RIGHT_GPIO_Port, JOY_RIGHT_Pin);
	  USB_TX_Buffer[4] = HAL_GPIO_ReadPin(JOY_UP_GPIO_Port, JOY_UP_Pin);
	  USB_TX_Buffer[5] = HAL_GPIO_ReadPin(JOY_DOWN_GPIO_Port, JOY_DOWN_Pin);
	  USB_TX_Buffer[6] = HAL_GPIO_ReadPin(LD_R_GPIO_Port, LD_R_Pin) | HAL_GPIO_ReadPin(LD_G_GPIO_Port, LD_G_Pin) << 1;
	  if (USB_TX_Buffer[1] || USB_TX_Buffer[2] || USB_TX_Buffer[3] || USB_TX_Buffer[4] || USB_TX_Buffer[5] || USB_TX_Buffer[6])
	  {
		  USB_TX_Buffer[0] = 4;	//   REPORT_ID (4)
		  USBDataOutReady = SET;
	  }
	  else
		  USBDataOutReady = RESET;

	  USB_TX_Buffer[7] = 0x55;
	  USB_TX_Buffer[8] = 0xAA;

	  if(USBDataOutReady == SET)
		  USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS,USB_TX_Buffer,ARRAY2HOST+1); // To send usb buffer to PC

//	  HAL_Delay(50);
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 24;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage 
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD_R_GPIO_Port, LD_R_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD_G_GPIO_Port, LD_G_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : JOY_CENTER_Pin JOY_LEFT_Pin JOY_RIGHT_Pin JOY_UP_Pin 
                           JOY_DOWN_Pin */
  GPIO_InitStruct.Pin = JOY_CENTER_Pin|JOY_LEFT_Pin|JOY_RIGHT_Pin|JOY_UP_Pin 
                          |JOY_DOWN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LD_R_Pin */
  GPIO_InitStruct.Pin = LD_R_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD_R_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD_G_Pin */
  GPIO_InitStruct.Pin = LD_G_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD_G_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  
}
