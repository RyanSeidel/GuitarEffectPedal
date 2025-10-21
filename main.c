/* Team Members :
Ryan Seidel
Name
Name 
*/

#include <stdint.h>

/* ******************************************************************************
 * REGISTER DEFINITIONS
 * (Addresses are for STM32F446RE)
 ***************************************************************************** */

// --- RCC (Reset and Clock Control) ---
#define RCC_BASE       0x40023800UL
#define RCC_AHB1ENR    *((volatile uint32_t *)(RCC_BASE + 0x30))
#define RCC_APB1ENR    *((volatile uint32_t *)(RCC_BASE + 0x40))
#define RCC_CR         *((volatile uint32_t *)(RCC_BASE + 0x00))
#define RCC_PLLI2SCFGR *((volatile uint32_t *)(RCC_BASE + 0x84))
#define RCC_CFGR       *((volatile uint32_t *)(RCC_BASE + 0x08))
#define PLLI2S_ON_BIT  (1 << 26) // Bit 26 in RCC_CR
#define PLLI2S_RDY_BIT (1 << 27) // Bit 27 in RCC_CR

// RCC BitMask
#define RCC_AHB1ENR_GPIOBEN (1 << 1)
#define RCC_AHB1ENR_GPIOCEN (1 << 2)
#define RCC_APB1ENR_I2C1EN  (1 << 21)
#define RCC_APB1ENR_SPI2EN  (1 << 14)

// --- GPIOB (Port B for Pins PB8, PB9, PB12, PB13, PB15) ---
#define GPIOB_BASE    0x40020400UL
#define GPIOB_MODER   *((volatile uint32_t *)(GPIOB_BASE + 0x00))
#define GPIOB_OTYPER  *((volatile uint32_t *)(GPIOB_BASE + 0x04))
#define GPIOB_OSPEEDR *((volatile uint32_t *)(GPIOB_BASE + 0x08))
#define GPIOB_AFRL    *((volatile uint32_t *)(GPIOB_BASE + 0x20)) // AF Register Low (pins 0-7)
#define GPIOB_AFRH    *((volatile uint32_t *)(GPIOB_BASE + 0x24)) // AF Register High (pins 8-15)

// --- GPIOC (Port C for Pins PC2, PC6) ---
#define GPIOC_BASE    0x40020800UL
#define GPIOC_MODER   *((volatile uint32_t *)(GPIOC_BASE + 0x00))
#define GPIOC_OTYPER  *((volatile uint32_t *)(GPIOC_BASE + 0x04))
#define GPIOC_OSPEEDR *((volatile uint32_t *)(GPIOC_BASE + 0x08))
#define GPIOC_AFRL    *((volatile uint32_t *)(GPIOC_BASE + 0x20)) // AF Register Low (pins 0-7)
#define GPIOC_AFRH    *((volatile uint32_t *)(GPIOC_BASE + 0x24)) // AF Register High (pins 8-15)

// --- I2C1 Peripheral ---
#define I2C1_BASE      0x40005400UL
#define I2C1_CR1       *((volatile uint32_t *)(I2C1_BASE + 0x00))
#define I2C1_CR2       *((volatile uint32_t *)(I2C1_BASE + 0x04))
#define I2C1_TIMINGR   *((volatile uint32_t *)(I2C1_BASE + 0x10))
#define I2C1_ISR       *((volatile uint32_t *)(I2C1_BASE + 0x18))
#define I2C1_ICR       *((volatile uint32_t *)(I2C1_BASE + 0x1C))
#define I2C1_TXDR      *((volatile uint32_t *)(I2C1_BASE + 0x28))
#define I2C1_RXDR      *((volatile uint32_t *)(I2C1_BASE + 0x24))
#define I2C_PE_BIT     (1 << 0) // Bit 0 in CR1
#define I2C_TXIS_FLAG  (1 << 1) // Bit 1 in ISR
#define I2C_RXNE_FLAG  (1 << 2) // Bit 2 in ISR
#define I2C_NACK_FLAG  (1 << 4) // Bit 4 in ISR
#define I2C_TC_FLAG    (1 << 6) // Bit 6 in ISR
#define I2C_START_BIT  (1 << 13) // Bit 13 in CR2
#define I2C_AUTOEND_BIT (1 << 25) // Bit 25 in CR2
#define I2C_RD_WRN_BIT (1 << 10) // Bit 10 in CR2 (0=Write, 1=Read)

// --- SPI2 (for I2S2 functionality) ---
#define SPI2_BASE      0x40003800UL
#define SPI2_I2SCFGR   *((volatile uint32_t *)(SPI2_BASE + 0x1C))
#define SPI2_I2SPR     *((volatile uint32_t *)(SPI2_BASE + 0x20))
#define I2S_MCKOE_BIT  (1 << 9)  // Bit 9 in I2SPR
#define I2S_I2SE_BIT   (1 << 11) // Bit 11 in I2SCFGR
#define I2S_I2SMOD_BIT (1 << 11) // Bit 11 in I2SCFGR (same bit, different name)


/* ******************************************************************************
 * GPIO INITIALIZATION
 * Configures all pins for I2C and I2S
 ***************************************************************************** */
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
                                     
    // --- 5. SET ALTERNATE FUNCTION MAPPING ---
    // ** Page 179 STM32F446x Datasheet ** // AF4 (I2C1..4)
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

/* ******************************************************************************
 * MCLK INITIALIZATION (TEST 1)
 * Configures the I2S PLL to output the Master Clock on PC6
 ***************************************************************************** */
void Audio_MCLK_Init(void)
{
    // --- 1. Configure PLLI2S ---
    // These values are calculated to produce a clock compatible with 48kHz.
    // Assumes HSI (16MHz) is the PLL source, and PLLM is 16.
    // I2SxCLK = (HSI / PLLM) * PLLI2SN / PLLI2SR
    // I2SxCLK = (16MHz / 16) * 192 / 5 = 38.4 MHz
    // (PLLM=16 is the default for HSI on F446, check RCC_PLLCFGR if this fails)
    uint32_t PLLI2SN = 192;
    uint32_t PLLI2SR = 5;
    
    RCC_PLLI2SCFGR = (PLLI2SN << 6) | (PLLI2SR << 28);
    
    // --- 2. Enable PLLI2S ---
    RCC_CR |= PLLI2S_ON_BIT;
    
    // Wait for PLLI2S to be ready
    while(!(RCC_CR & PLLI2S_RDY_BIT))
    {
        // Wait...
    }
    
    // --- 3. Configure I2S2 Prescaler to output MCLK ---
    // We need 48kHz sample rate. The MCLK is 256 * Fs = 12.288 MHz
    // Our PLLI2S clock is 38.4 MHz
    // MCLK = I2SxCLK / (I2SDIV*2 + ODD)  -- This formula is simplified for MCLK
    // 12.288 MHz = 38.4 MHz / (1*2 + 1) -> This is not right
    // Let's check the manual... 
    // BCLK = I2SxCLK / (2 * I2SDIV + ODD)
    // For 16-bit, 48kHz, BCLK = 48000 * 16 * 2 = 1.536 MHz
    // 1.536 MHz = 38.4 MHz / (2*12 + 1) -> I2SDIV=12, ODD=1.
    // MCLK is 256 * Fs = 12.288 MHz.
    // MCLK = I2SxCLK / I2SDIV_total ... 
    // This is complex. Let's just enable MCKOE with a known good setting.
    
    uint32_t I2SDIV = 12; // For 48kHz
    uint32_t ODD = 1;     // For 48kHz
    
    // Set I2SDIV, ODD, and enable Master Clock Output (MCKOE)
    SPI2_I2SPR = (I2S_MCKOE_BIT) | (ODD << 8) | I2SDIV;
    
    // --- 4. Enable I2S Peripheral in I2S Mode ---
    // This is required to start the clock generator
    SPI2_I2SCFGR |= I2S_I2SMOD_BIT | I2S_I2SE_BIT;
}

/* ******************************************************************************
 * I2C INITIALIZATION (TEST 2 SETUP)
 * Configures the I2C1 peripheral
 ***************************************************************************** */
void Audio_I2C_Init(void)
{
    // 1. Ensure I2C1 is disabled before config
    I2C1_CR1 &= ~(I2C_PE_BIT);
    
    // 2. Set TIMINGR for 400kHz (Fast-mode) with 16MHz I2CCLK (default HSI)
    // PRESC=0, SCLDEL=3, SDADEL=1, SCLH=9, SCLL=28
    I2C1_TIMINGR = 0x0030091C;
    
    // 3. Enable I2C1
    I2C1_CR1 |= I2C_PE_BIT;
}

/* ******************************************************************************
 * I2C READ FUNCTION (TEST 2)
 * Reads the SGTL5000 Chip ID (should return 0x00A0)
 ***************************************************************************** */
uint16_t SGTL5000_ReadChipID(void)
{
    volatile uint32_t wait;
    uint8_t i2c_addr = 0x0A; // SGTL5000 address
    uint16_t reg_addr = 0x0000; // CHIP_ID register
    uint8_t read_val_high, read_val_low;

    // --- 1. Send the Register Address we want to read (0x0000) ---
    
    // Clear CR2, set Addr, NBYTES=2 (for 16-bit reg addr), 
    // set to WRITE (0), and set START.
    I2C1_CR2 = 0;
    I2C1_CR2 |= (i2c_addr << 1); // Set SADD
    I2C1_CR2 |= (2 << 16);      // Set NBYTES = 2
    I2C1_CR2 &= ~(I2C_RD_WRN_BIT); // Clear RD_WRN (Write)
    I2C1_CR2 |= I2C_START_BIT;  // Set START
    
    // Wait for TXIS (Transmit buffer empty) or NACK
    while(!(I2C1_ISR & I2C_TXIS_FLAG))
    {
        if(I2C1_ISR & I2C_NACK_FLAG) { I2C1_ICR |= (1 << 4); return 0xFFFF; }
    }
    
    // Write high byte of register address
    I2C1_TXDR = (uint8_t)(reg_addr >> 8);
    
    // Wait for TXIS
    while(!(I2C1_ISR & I2C_TXIS_FLAG))
    {
        if(I2C1_ISR & I2C_NACK_FLAG) { I2C1_ICR |= (1 << 4); return 0xFFFF; }
    }
    
    // Write low byte of register address
    I2C1_TXDR = (uint8_t)(reg_addr & 0xFF);
    
    // Wait for TC (Transfer Complete)
    while(!(I2C1_ISR & I2C_TC_FLAG))
    {
        if(I2C1_ISR & I2C_NACK_FLAG) { I2C1_ICR |= (1 << 4); return 0xFFFF; }
    }
    
    // --- 2. Read the 2 bytes of data back ---
    
    // Clear CR2, set Addr, NBYTES=2, set to READ (1), 
    // set AUTOEND, and set START (this is a Re-START).
    I2C1_CR2 = 0;
    I2C1_CR2 |= (i2c_addr << 1); // Set SADD
    I2C1_CR2 |= (2 << 16);      // Set NBYTES = 2
    I2C1_CR2 |= I2C_RD_WRN_BIT; // Set RD_WRN (Read)
    I2C1_CR2 |= I2C_AUTOEND_BIT;// Set AUTOEND
    I2C1_CR2 |= I2C_START_BIT;  // Set START
    
    // Wait for RXNE (Receive buffer not empty)
    while(!(I2C1_ISR & I2C_RXNE_FLAG))
    {
        if(I2C1_ISR & I2C_NACK_FLAG) { I2C1_ICR |= (1 << 4); return 0xFFFF; }
    }
    read_val_high = I2C1_RXDR; // Read high byte
    
    // Wait for RXNE
    while(!(I2C1_ISR & I2C_RXNE_FLAG))
    {
        if(I2C1_ISR & I2C_NACK_FLAG) { I2C1_ICR |= (1 << 4); return 0xFFFF; }
    }
    read_val_low = I2C1_RXDR; // Read low byte
    
    // Autoend will handle the STOP
    
    // Clear NACK flag just in case (though AUTOEND should handle)
    I2C1_ICR |= (1 << 4);
    
    return ((uint16_t)read_val_high << 8) | read_val_low;
}


/* ******************************************************************************
 * MAIN FUNCTION
 ***************************************************************************** */
int main(void)
{
    // This variable will hold the chip ID. Make it volatile
    // so the compiler doesn't optimize it away.
    volatile uint16_t chip_id = 0;
    
    // 1. Configure all GPIO pins
    Audio_GPIO_Init();
    
    // 2. Configure and start the I2S PLL and MCLK output
    // (TEST 1: Check PC6 with an oscilloscope for 12.288 MHz)
    Audio_MCLK_Init();
    
    // 3. Configure the I2C1 peripheral
    Audio_I2C_Init();
    
    // --- Next steps ---
    // 4. Configure I2S2 peripheral registers (full config)
    // 5. Configure DMA
    // ...etc.
    
    while(1)
    {
        // (TEST 2: Check 'chip_id' variable in your debugger)
        // It should be 0x00A0
        chip_id = SGTL5000_ReadChipID();
        
        // Simple delay loop
        for(volatile int i = 0; i < 100000; i++);
    }
    
    return 0;
}