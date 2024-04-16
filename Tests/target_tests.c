#include "main.h"
#include "string.h"
#include <stddef.h>
#include "unity_config.h"
#include "unity_fixture.h"
#include "jdy23_driver.h"

UART_HandleTypeDef huart1; // usb
UART_HandleTypeDef huart2; // jdy23
DMA_HandleTypeDef hdma_usart2_rx;

//UART_HandleTypeDef *jdy23_uart = &huart2;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);
static void run_all_tests(void);

// IO functions
jdy23_status io_read(uint8_t *const data, const uint16_t data_size);
jdy23_status io_external_read(uint8_t *const data, const uint16_t data_size);
jdy23_status io_write(const uint8_t *const data, const uint16_t data_size);
jdy23_status io_set_baudrate(const uint32_t baudrate);
jdy23_status io_set_pwrc(bool is_high);

// Specific functions for tests

void unity_config_put_c(uint8_t a);
int _kill(int, int);
int _getpid();
void _exit(int);

int main(void)
{
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();

  jdy23_create((jdy23_io_functions){
    .io_read = io_read,
    .io_external_read = io_external_read,
    .io_write = io_write,
    .io_set_baudrate = io_set_baudrate,
    .io_set_pwrc = io_set_pwrc
  });

  return UnityMain(0, NULL, run_all_tests);
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    Error_Handler();

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    Error_Handler();
}

static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
    Error_Handler();
}

static void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
    Error_Handler();
}

static void MX_DMA_Init(void)
{
  __HAL_RCC_DMA1_CLK_ENABLE();

  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  HAL_GPIO_WritePin(JDY23_PWRC_GPIO_Port, JDY23_PWRC_Pin, GPIO_PIN_SET);

  GPIO_InitStruct.Pin = JDY23_PWRC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(JDY23_PWRC_GPIO_Port, &GPIO_InitStruct);
}

static void run_all_tests()
{
	RUN_TEST_GROUP(jdy23_driver);
}

jdy23_status io_read(uint8_t *const data, const uint16_t data_size)
{
  return (jdy23_status)HAL_UART_Receive(
    &huart2, data, data_size, JDY23_TIMEOUT
  );
}

jdy23_status io_external_read(uint8_t *const data, const uint16_t data_size)
{
  return (jdy23_status)HAL_UART_Receive_DMA(&huart2, data, data_size);
}

jdy23_status io_write(const uint8_t *const data, const uint16_t data_size)
{
  return (jdy23_status)HAL_UART_Transmit(
    &huart2, data, data_size, JDY23_TIMEOUT
  );
}

jdy23_status io_set_baudrate(const uint32_t baudrate)
{
  HAL_UART_Abort_IT(&huart2);
  HAL_UART_DeInit(&huart2);
  huart2.Init.BaudRate = baudrate;

  return (jdy23_status)HAL_UART_Init(&huart2);
}

jdy23_status io_set_pwrc(bool is_high)
{
  HAL_GPIO_WritePin(
    JDY23_PWRC_GPIO_Port,
    JDY23_PWRC_Pin,
    is_high ? GPIO_PIN_SET : GPIO_PIN_RESET
  );
}

void unity_config_put_c(uint8_t a)
{
  (void) HAL_UART_Transmit(&huart1, &a, sizeof(a), 100U);
}

int _kill(int a, int b)
{
  return(-1);
}
 
int _getpid()
{
  return(1);
}
 
void _exit(int a)
{
  while(1)
	{			
	}
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

