#include <smart_car.h>


#define NRF_CE_1 		direct_control(NRF_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p83_hi)
#define NRF_CE_0    	direct_control(NRF_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p83_lo)
#define NRF_CSN_1		direct_control(NRF_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p84_hi)
#define NRF_CSN_0     	direct_control(NRF_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p84_lo)
#define NRF_SCK_1		direct_control(NRF_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p85_hi)
#define NRF_SCK_0		direct_control(NRF_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p85_lo)
#define NRF_MOSI_1 		direct_control(NRF_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p86_hi)
#define NRF_MOSI_0      direct_control(NRF_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p86_lo)
#define NRF_MISO		Read_Bit(NRF_gpio_handle,s_p81)
#define NRF_IRQ			Read_Bit(NRF_gpio_handle,s_p00)
// SPI(nRF24L01) commands
#define READ_REG 0x00 // Define read command to register
#define WRITE_REG 0x20 // Define write command to register
#define RD_RX_PLOAD 0x61 // Define RX payload register address
#define WR_TX_PLOAD 0xA0 // Define TX payload register address
#define FLUSH_TX 0xE1 // Define flush TX register command
#define FLUSH_RX 0xE2 // Define flush RX register command
#define REUSE_TX_PL 0xE3 // Define reuse TX payload register command
#define NOP 0xFF // Define No Operation, might be used to read status register
//***************************************************//
// SPI(nRF24L01) registers(addresses)
#define CONFIG 0x00 // 'Config' register address
#define EN_AA 0x01 // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR 0x02 // 'Enabled RX addresses' register address
#define SETUP_AW 0x03 // 'Setup address width' register address
#define SETUP_RETR 0x04 // 'Setup Auto. Retrans' register address
#define RF_CH 0x05 // 'RF channel' register address
#define RF_SETUP 0x06 // 'RF setup' register address
#define STATUS 0x07 // 'Status' register address

#define MAX_TX  		0x10  //最大发送次数
#define TX_OK   		0x20  //TX发送完成中断
#define RX_OK   		0x40  //接收数据中断

#define OBSERVE_TX 0x08 // 'Observe TX' register address
#define CD 0x09 // 'Carrier Detect' register address
#define RX_ADDR_P0 0x0A // 'RX address pipe0' register address
#define RX_ADDR_P1 0x0B // 'RX address pipe1' register address
#define RX_ADDR_P2 0x0C // 'RX address pipe2' register address
#define RX_ADDR_P3 0x0D // 'RX address pipe3' register address
#define RX_ADDR_P4 0x0E // 'RX address pipe4' register address
#define RX_ADDR_P5 0x0F // 'RX address pipe5' register address
#define TX_ADDR 0x10 // 'TX address' register address
#define RX_PW_P0 0x11 // 'RX payload width, pipe0' register address
#define RX_PW_P1 0x12 // 'RX payload width, pipe1' register address
#define RX_PW_P2 0x13 // 'RX payload width, pipe2' register address
#define RX_PW_P3 0x14 // 'RX payload width, pipe3' register address
#define RX_PW_P4 0x15 // 'RX payload width, pipe4' register address
#define RX_PW_P5 0x16 // 'RX payload width, pipe5' register address
#define FIFO_STATUS 0x17 // 'FIFO Status Register' register address
//------------------------------------------------------------
//24L01发送接收数据宽度
#define TX_ADR_WIDTH    5
#define RX_ADR_WIDTH    5
#define TX_PLOAD_WIDTH  32
#define RX_PLOAD_WIDTH  32



int_t NRF_Init(void);
u8 SPI_RW(u8 byte);
u8 SPI_Write_Reg(u8 reg, u8 value);
u8 SPI_Read_Reg(u8 reg);
u8 NRF24L01_Check();
u8 SPI_Read_Buf(u8 reg, u8 *pBuf, u8 len);
u8 SPI_Write_Buf(u8 reg, u8 *pBuf, u8 len);
u8 nRF24L01_RxPacket(unsigned char* rx_buf);
u8 nRF24L01_TxPacket(unsigned char * tx_buf);
void NRF24L01_TX_Mode(u8 NRF2401Channel);
void NRF24L01_RX_Mode(u8 NRF2401Channel);
void nRF24L01_Config(void);
int Read_Bit(int_t handle ,st_r_drv_gpio_pin_rw_t pin);

//#define RX_DR 1
