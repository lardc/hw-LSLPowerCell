// Include
//
#include "Global.h"
#include "Controller.h"
#include "Interrupts.h"
#include "SysConfig.h"
#include "BoardConfig.h"
#include "BCCIxParams.h"

// Forward functions
//
void ConfigSysClk();
void ConfigGPIO();
void ConfigCAN();
void ConfigUART();
void ConfigTimer2();
void ConfigWatchDog();

// Functions
//
int main()
{
	// Set request flag if firmware update is required
	if(*ProgramAddressStart == 0xFFFFFFFF || BOOT_LOADER_VARIABLE == BOOT_LOADER_REQUEST)
		WaitForFWUpload = TRUE;
	
	// Init peripherals
	ConfigSysClk();
	ConfigGPIO();
	ConfigCAN();
	ConfigUART();
	ConfigTimer2();
	ConfigWatchDog();
	
	// Init controller
	CONTROL_Init();
	
	// Infinite cycle
	while(true)
		CONTROL_Idle();
	
	return 0;
}
//--------------------------------------------

void ConfigSysClk()
{
	RCC_PLL_HSE_Config(QUARTZ_FREQUENCY, PREDIV_4, PLL_14);
}
//--------------------------------------------

void ConfigGPIO()
{
	// ��������� ������������ ������
	RCC_GPIO_Clk_EN(PORTA);
	RCC_GPIO_Clk_EN(PORTB);
	
	//������
	GPIO_Config(LED_BLINK_PORT, LED_BLINK_PIN, Output, PushPull, HighSpeed, NoPull);
	GPIO_Config(DAC_PORT, DAC_PIN, Output, PushPull, HighSpeed, NoPull);
	GPIO_Bit_Rst(DAC_PORT, DAC_PIN);
	
	//�������������� ������� ������
	GPIO_Config(GPIOA, Pin_9, AltFn, PushPull, HighSpeed, NoPull); //PA9(USART1 TX)
	GPIO_AltFn(GPIOA, Pin_9, AltFn_7);
	
	GPIO_Config(GPIOA, Pin_10, AltFn, PushPull, HighSpeed, NoPull); //PA10(USART1 RX)
	GPIO_AltFn(GPIOA, Pin_10, AltFn_7);
	
	GPIO_Config(GPIOA, Pin_11, AltFn, PushPull, HighSpeed, NoPull); //PA11(CAN RX)
	GPIO_AltFn(GPIOA, Pin_11, AltFn_9);
	
	GPIO_Config(GPIOA, Pin_12, AltFn, PushPull, HighSpeed, NoPull); //PA12(CAN TX)
	GPIO_AltFn(GPIOA, Pin_12, AltFn_9);
	
}
//--------------------------------------------

void ConfigCAN()
{
	RCC_CAN_Clk_EN(CAN_1_ClkEN);
	NCAN_Init(SYSCLK, CAN_BAUDRATE, FALSE);
	NCAN_FIFOInterrupt(TRUE);
	NCAN_FilterInit(0, CAN_SLAVE_FILTER_ID, CAN_MASTER_FILTER_ID);
}
//--------------------------------------------

void ConfigUART()
{
	USART_Init(USART1, SYSCLK, USART_BAUDRATE);
	USART_Recieve_Interupt(USART1, 0, true);
}
//--------------------------------------------

void ConfigTimer2()
{
	TIM_Clock_En(TIM_2);
	TIM_Config(TIM2, SYSCLK, TIMER2_uS);
	TIM_Interupt(TIM2, 0, true);
	TIM_Start(TIM2);
}
//--------------------------------------------

void ConfigWatchDog()
{
	IWDG_Config();
	IWDG_ConfigureFastUpdate();
}
//--------------------------------------------
