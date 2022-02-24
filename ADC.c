#include "stm32f4xx.h"                  // Device header
#include "ADC.h"
//#include "define.h"
#include "string.h"
typedef struct
{
		uint32_t  port   : 8;     //structure member ; use 8 bit
		uint32_t  pin    : 16;    //structure member 
		uint32_t  mode   : 8;     //structure member 
	  uint32_t  channel: 16;
}Pin_t;               	    //redefine this structure 
 
	static Pin_t pin_prv[8][16];                            //pin_prv[port][pin] ; variable of structure "pin_prv[8][16]"
	
GPIO_TypeDef *EnumeratPort(uint8_t port);                //built in fucction  

GPIO_TypeDef *EnumeratPort(uint8_t port)                 //built in fucction  
{
	 GPIO_TypeDef *ret = NULL;
 switch(port) {
 case PORTA:
   ret=GPIOA;
   break;
 case PORTB:
   ret=GPIOB;
   break;
 case PORTC:
   ret=GPIOC;
   break;
 default:
   break;
 }
 return ret;
}
/*******************************ADC CHANNEL PIN CONFIG***************************************/
uint8_t ADC_PORT_PIN_CONFIG(uint8_t port,uint16_t pin);
uint8_t ADC_PORT_PIN_CONFIG(uint8_t port,uint16_t pin)
{
uint16_t adc_channel=NULL;
	switch(port)
	{
		case PORTA:
			switch (pin)
			{
				case PIN_0:
					adc_channel=0;
					break;
				case PIN_1:
					adc_channel=1;
					break;
				case PIN_2:
					adc_channel=2;
					break;
				case PIN_3:
					adc_channel=3;
					break;
				case PIN_4:
					adc_channel=4;
					break;
				case PIN_5:
					adc_channel=5;
					break;
				case PIN_6:
					adc_channel=6;
					break;
				case PIN_7:
					break;
			  default:
			  break;
			}
			break;
		case PORTB:
			switch (pin)
			{
				case PIN_0:
					adc_channel=8;
					break;
				case PIN_1:
					adc_channel=9;
					break;
			default:
			break;
			}
			break;
		  default:
			break;
				
	}
return adc_channel;	
}

/******************************************ADC CHANNEL CONFIG END**************************************************/

static void pinMode(uint8_t port, uint16_t pin, uint8_t mode) // we  can't call this function from another c file 
{
	
	RCC->AHB1ENR |= 1 << port;                                 // enable port  
	int modeBit= pin * 2;                                      //enable moder bit for define pin mode 
	if (mode==OUTPUT)                                          //check pin mode
	{
		 
		EnumeratPort(port)->MODER |=1 << modeBit;                //enable mode bit for set output mode 
		EnumeratPort(port)->MODER &= ~(1 << (modeBit + 1));                //enable mode bit for set output mode 
	}
	else if(mode==INPUT)
	{
		EnumeratPort(port)->MODER &= ~(1<< modeBit );            //work as input 
		EnumeratPort(port)->MODER &= ~(1 << (modeBit + 1));                //enable mode bit for set output mode 
	}
		else if(mode == ALTF)
	{
		EnumeratPort(port)->MODER &= ~(1<< modeBit );            //work as input 
		EnumeratPort(port)->MODER |=(1 << (modeBit + 1)); 
	}
	else if (mode==ANALOG_MODE)
	{
		EnumeratPort(port)->MODER |=(1<< modeBit );
		EnumeratPort(port)->MODER |=(1 << (modeBit + 1));
	}
}


PinName PinCreate(uint8_t port, uint16_t pin, uint8_t mode)      //take user input "port" , "pin number" , "mode"
{
	pinMode(port, pin, mode);                                  //call "pinMode" function and set function parameter which come from user                   
	pin_prv[port][pin].mode = mode;
	pin_prv[port][pin].pin = pin;
	pin_prv[port][pin].port = port;
	pin_prv[port][pin].channel=ADC_PORT_PIN_CONFIG(port,pin);
	return (PinName)&pin_prv[port][pin];                      //return 
}

void digitalWrite_High(PinName pin)
{
	Pin_t * p = (Pin_t *)pin;                                 //type cast 
	EnumeratPort(p->port)->ODR |= 1 <<  p->pin;
}

void digitalWrite_Low(PinName pin)
{
	Pin_t * p = (Pin_t *)pin;                                 //type cast 
	EnumeratPort(p->port)->ODR &= ~ ( 1 << p->pin);
}

uint16_t analogRead(PinName pin)
{
	Pin_t * p = (Pin_t *)pin; 
	RCC->APB2ENR  |= 0x100;// Enable clock access to ADC
  ADC1->CR2 = 0;   			//Disable ADC 
	ADC1->SQR3 =p->channel;

	ADC1->CR2 |= 1;				//Eneable ADC 
	ADC1->CR2 |=0x0002;   //Continuous convertion 
	while(1)
	{  
		ADC1->CR2 |= 0x40000000;
		//wait for the conversion to be complete 
		while(!(ADC1->SR & 2)){}
		uint16_t analogValue =ADC1->DR;
			return analogValue;
	 }
}

