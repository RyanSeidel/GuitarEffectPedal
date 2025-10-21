/* Team Members :
Ryan Seidel
Name
Name 
*/

#include <stdint.h>

/* ******************************************************************************
 * REGISTER DEFINITIONS
 * (Addresses are common for STM32F4, but VERIFY for your chip)
 ***************************************************************************** */

// RCC Base Address
#define RCC_BASE      0x40023800UL
#define RCC_AHB1ENR   *((volatile uint32_t *)(RCC_BASE + 0x30))
#define RCC_APB1ENR   *((volatile uint32_t *)(RCC_BASE + 0x40))

// RCC BitMask
#define RCC_AHB1ENR_GPIOBEN (1 << 1)
#define RCC_AHB1ENR_GPIOCEN (1 << 2)
#define RCC_APB1ENR_I2C1EN  (1 << 21)
#define RCC_APB1ENR_SPI2EN  (1 << 14)

// GPIO Port B for Pins PB8 PB9 PB12 PB13 PB15
#define GPIOB_BASE 0x40020400UL
#define GPIOB_MODER   *((volatile uint32_t *)(GPIOB_BASE + 0x00))
#define GPIOB_OTYPER  *((volatile uint32_t *)(GPIOB_BASE + 0x04))
#define GPIOB_OSPEEDR *((volatile uint32_t *)(GPIOB_BASE + 0x08))
#define GPIOB_AFRL    *((volatile uint32_t *)(GPIOB_BASE + 0x20)) // AF Register Low (pins 0-7)
#define GPIOB_AFRH    *((volatile uint32_t *)(GPIOB_BASE + 0x24))


// GPIO Port C for Pins PC2 PC6
#define GPIOC_BASE 0x40020800UL
#define GPIOC_MODER   *((volatile uint32_t *)(GPIOC_BASE + 0x00))
#define GPIOC_OTYPER  *((volatile uint32_t *)(GPIOC_BASE + 0x04))
#define GPIOC_OSPEEDR *((volatile uint32_t *)(GPIOC_BASE + 0x08))
#define GPIOC_AFRL    *((volatile uint32_t *)(GPIOC_BASE + 0x20)) // AF Register Low (pins 0-7)
#define GPIOC_AFRH    *((volatile uint32_t *)(GPIOC_BASE + 0x24)) // AF Register High (pins 8-15)

// I2C Peripheral 

// SPI2 for I2S2 functionally

void Audio_GPIO_Init(void)
{
	// 1. Enable Peripheral Clock
	// Enable GPIO Port B and Port C Clocks
	RCC_AHB1ENR |= (RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN);
	
	// Enable I2C1 and SPI2 (for I2S2) clocks
	RCC_APB1ENR |= (RCC_APB1ENR_I2C1EN | RCC_APB1ENR_SPI2EN);
	
	// --- 2. SET PIN MODE to "ALTERNATE FUNCTION" (mode 10) ---
	
	// GPIOB Pins  8, 9, 12, 13, 15
	GPIOB_MODER |= (2 << (8*2))  | // PB8  (I2C1_SCL)
                 (2 << (9*2))  | // PB9  (I2C1_SDA)
                 (2 << (12*2)) | // PB12 (I2S2_WS)
                 (2 << (13*2)) | // PB13 (I2S2_CK)
                 (2 << (15*2));  // PB15 (I2S2_SD)
	
	// GPIOC: Pins 2, 6
    GPIOC_MODER |= (2 << (2*2))  | // PC2 (I2S2ext_SD)
                   (2 << (6*2));   // PC6 (I2S2_MCK)
	
	// --- 3. SET OUTPUT TYPE ---
	// I2C pins (PB8, PB9) to Open-Drain (mode 1). I2S pins remain Push-Pull (0).
    GPIOB_OTYPER |= (1 << 8) | (1 << 9);
		
	// --- 4. SET OUTPUT SPEED ---
    
    // I2C (PB8, PB9) to High Speed (mode 10 = 2)
    // I2S (all others) to Very High Speed (mode 11 = 3)
    GPIOB_OSPEEDR |= (2 << (8*2))  | // PB8
                     (2 << (9*2))  | // PB9
                     (3 << (12*2)) | // PB12
                     (3 << (13*2)) | // PB13
                     (3 << (15*2));  // PB15

    GPIOC_OSPEEDR |= (3 << (2*2)) |  // PC2
                     (3 << (6*2));   // PC6
										 
// --- 5. SET ALTERNATE FUNCTION MAPPING (Using Your AF List) ---
		// ** Page 179 STM32F446x Datasheet ** 
    // AF4 (I2C1..4)
    // AF5 (SPI1/2/3/4) -> for I2S2
    // AF6 (SPI2/3/4)   -> for I2S2_ext
    
    // Configure Port B pins (8-15) using AFRH (AF Register High)
		// Page 191
    GPIOB_AFRH |= (4 << ((8-8)*4))   | // PB8  (I2C1_SCL) -> AF4
                  (4 << ((9-8)*4))   | // PB9  (I2C1_SDA) -> AF4
                  (5 << ((12-8)*4))  | // PB12 (I2S2_WS)  -> AF5
                  (5 << ((13-8)*4))  | // PB13 (I2S2_CK)  -> AF5
                  (5 << ((15-8)*4));   // PB15 (I2S2_SD)  -> AF5

    // Configure Port C pins (0-7) using AFRL (AF Register Low)
    GPIOC_AFRL |= (6 << (2*4)) |  // PC2 (I2S2ext_SD) -> AF6
                  (5 << (6*4));   // PC6 (I2S2_MCK)   -> AF5
	
	
	
	
	
}

int main(void)
{
	// 1. Audio_GPIO_Init();
	
	// --- Next steps ---
    // 2. Configure Clock Tree (PLLs) for 12.288 MHz
    // 3. Configure I2C1 peripheral registers
    // 4. Configure I2S2 peripheral registers
    // 5. Configure DMA
    // ...etc.
    
    while(1)
    {
        // Your main application loop
    }
    
    return 0;
	
	
}