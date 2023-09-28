
#include "main.h"
#include "stm32f4xx_hal_exti.h"

GPIO_InitTypeDef Init;
EXTI_ConfigTypeDef EXTI_InitStr;
EXTI_HandleTypeDef EXTI_Handle;

int i = 0, sure = 1900;

void SystemClock_Config(void);

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15 | GPIO_PIN_14 |GPIO_PIN_13, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_15 | GPIO_PIN_14 | GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

}
// Pin 0 of port A is configured to interrupt line
void GPIO_EXTI_Config(){
	HAL_EXTI_SetConfigLine(&EXTI_Handle, &EXTI_InitStr);

	EXTI_InitStr.Line = EXTI_LINE_0;
	EXTI_InitStr.Mode = EXTI_MODE_INTERRUPT;
	EXTI_InitStr.Trigger = EXTI_TRIGGER_RISING;
	EXTI_InitStr.GPIOSel = EXTI_GPIOA;

	HAL_EXTI_GetConfigLine(&EXTI_Handle, &EXTI_InitStr);
}
//Debouncing alghorithm
uint32_t lastDebounceTime = 0;
const uint32_t debounceDelay = 200; 

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
     uint32_t currentTime = HAL_GetTick();

     if ((currentTime - lastDebounceTime) >= debounceDelay) {
         i++;
         sure = sure - 400;
         if (i == 4) {
             i = 0;
             sure = 1900;
            }
        lastDebounceTime = currentTime;
        }
    }


int main(void)
{
  HAL_Init();

  SystemClock_Config();

  GPIO_EXTI_Config();

  MX_GPIO_Init();

  HAL_GPIO_EXTI_Callback(GPIO_PIN_0);


  while (1)
  {
	  // Predetermined state
	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, SET);//Orange LED
	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14|GPIO_PIN_15, RESET);// Pin 14 is red, pin 15 is blue LED

	  if (i == 1){
	 	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, SET);
	 		  }
	  else if (i == 2){

		  HAL_Delay(sure);

	 	  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13 | GPIO_PIN_15);
		  	 }
	  else if (i == 3){
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, SET);//Blue LED is set

		  HAL_Delay(sure);

		  HAL_GPIO_TogglePin(GPIOD,  GPIO_PIN_13| GPIO_PIN_14);

		  HAL_Delay(sure);
	  }
  }

}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 50;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

