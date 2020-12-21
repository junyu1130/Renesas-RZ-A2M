#include<NRF24L01.h>

//define some para
//---------High-signal---------//
//Pin-8
static st_r_drv_gpio_pin_rw_t s_p83_hi ={GPIO_PORT_8_PIN_3,GPIO_LEVEL_HIGH,GPIO_SUCCESS};
static st_r_drv_gpio_pin_rw_t s_p84_hi ={GPIO_PORT_8_PIN_4,GPIO_LEVEL_HIGH,GPIO_SUCCESS};
static st_r_drv_gpio_pin_rw_t s_p85_hi ={GPIO_PORT_8_PIN_5,GPIO_LEVEL_HIGH,GPIO_SUCCESS};
static st_r_drv_gpio_pin_rw_t s_p86_hi ={GPIO_PORT_8_PIN_6,GPIO_LEVEL_HIGH,GPIO_SUCCESS};

//---------Low-signal---------//
//Pin-8
static st_r_drv_gpio_pin_rw_t s_p83_lo ={GPIO_PORT_8_PIN_3,GPIO_LEVEL_LOW,GPIO_SUCCESS};
static st_r_drv_gpio_pin_rw_t s_p84_lo ={GPIO_PORT_8_PIN_4,GPIO_LEVEL_LOW,GPIO_SUCCESS};
static st_r_drv_gpio_pin_rw_t s_p85_lo ={GPIO_PORT_8_PIN_5,GPIO_LEVEL_LOW,GPIO_SUCCESS};
static st_r_drv_gpio_pin_rw_t s_p86_lo ={GPIO_PORT_8_PIN_6,GPIO_LEVEL_LOW,GPIO_SUCCESS};

static st_r_drv_gpio_pin_rw_t s_p00    ={GPIO_PORT_0_PIN_0, GPIO_LEVEL_SC_DEFAULT, GPIO_SUCCESS };
static st_r_drv_gpio_pin_rw_t s_p81    ={GPIO_PORT_8_PIN_1, GPIO_LEVEL_SC_DEFAULT, GPIO_SUCCESS };

static int_t NRF_gpio_handle;
static const r_gpio_port_pin_t NRF_pin_list[] =
{
	GPIO_PORT_8_PIN_3,
	GPIO_PORT_8_PIN_4,
	GPIO_PORT_8_PIN_5,
	GPIO_PORT_8_PIN_6,
};

//u8  MOSI,MISO,SCK,CSN,CE;
const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01};
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01};

int_t NRF_Init(void)
{
    int_t err;
    st_r_drv_gpio_pin_list_t pin_NRF;
    char_t data;
    NRF_gpio_handle = open(DEVICE_INDENTIFIER "gpio", O_RDWR);
    pin_NRF.p_pin_list = NRF_pin_list;
    pin_NRF.count = (sizeof(NRF_pin_list)) / (sizeof(NRF_pin_list[0]));
    err = direct_control(NRF_gpio_handle, CTL_GPIO_INIT_BY_PIN_LIST, &pin_NRF);
    NRF24L01_RX_Mode(90);
    return err;
}

// 写一个字节到24L01，同时读出一个字节
u8 SPI_RW(u8 byte)
{
	u8 bit_ctr;
	for(bit_ctr=0;bit_ctr<8;bit_ctr++) // output 8-bit
	{
		if  (byte & 0x80)
			NRF_MOSI_1;
		else NRF_MOSI_0;   // output 'u8', MSB to MOSI

		byte = (byte << 1); // shift next bit into MSB..
		NRF_SCK_1; // Set SCK high..
		byte |= NRF_MISO; // capture current MISO bit
		NRF_SCK_0; // ..then set SCK low again
	}
	return(byte); // return read byte
}

// SPI写寄存器
// reg：指定的寄存器
// value：写入的值
//向寄存器reg写一个字节，同时返回状态字节
u8 SPI_Write_Reg(u8 reg, u8 value)
{
	u8 status;
	NRF_CSN_0; // CSN low, init SPI transaction
	status = SPI_RW(reg); // select register
	SPI_RW(value); // ..and write value to it..
	NRF_CSN_1; // CSN high again
	return(status); // return nRF24L01 status u8
}
//SPI读寄存器
//reg:要读取的寄存器
u8 SPI_Read_Reg(u8 reg)
{
	u8 reg_val;
	NRF_CSN_0; // CSN low, init SPI transaction
	SPI_RW(reg); // select register
	reg_val=SPI_RW(0XFF); // ..and write value to it..
	NRF_CSN_1; // CSN high again
	return(reg_val); // return nRF24L01 status u8
}

u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 buf2[5]={0X00};

	u8 i;
	SPI_Write_Buf(WRITE_REG+TX_ADDR,buf,5);
	SPI_Read_Buf(TX_ADDR,buf2,5); //读出写的地址
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;
	if(i!=5)return 1;//error
	return 0;		 //normal
}

// 读出len字节的数据
u8 SPI_Read_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 status,u8_ctr;
	NRF_CSN_0; // Set CSN low, init SPI tranaction
	status = SPI_RW(reg); // Select register to write to and read status u8
	for(u8_ctr=0;u8_ctr<len;u8_ctr++)
		pBuf[u8_ctr] = SPI_RW(0xFF); //
	NRF_CSN_1;
	return(status); // return nRF24L01 status u8
}
// 写入len字节的数据
u8 SPI_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 status,u8_ctr;
	NRF_CSN_0;
	status = SPI_RW(reg);
	for(u8_ctr=0; u8_ctr<len; u8_ctr++) //
		SPI_RW(*pBuf++);
	NRF_CSN_1; // Set CSN high again
	return(status); //
}

// 接收函数，返回1表示有数据收到，否则没有数据接受到
u8 nRF24L01_RxPacket(unsigned char* rx_buf)
{
//	u8 revale=0;
	u8 sta;
	// set in RX mode
//	SPI_Write_Reg(WRITE_REG + CONFIG, 0x0f); // Set PWR_UP bit, enable CRC(2 u8s) &Prim:RX. RX_DR enabled..
//	NRF_CE_1; // Set CE pin high to enable RX device
//	R_OS_TaskSleep(130);
	sta=SPI_Read_Reg(STATUS); // read register STATUS's value
	SPI_Write_Reg(WRITE_REG+STATUS,sta);// clear RX_DR or TX_DS or MAX_RT interruptflag
	if(sta&RX_OK) // if receive data ready (RX_DR) interrupt
	{
//		NRF_CE_0; // stand by mode
		SPI_Read_Buf(RD_RX_PLOAD,rx_buf,RX_PLOAD_WIDTH);// read receive payload fromRX_FIFO buffer
		SPI_Write_Reg(FLUSH_RX,0xff);
		return 0;
//		revale =1;
	}

	return 1;
}

// 发送函数
u8 nRF24L01_TxPacket(unsigned char * tx_buf)
{
	u8 sta;
	NRF_CE_0;
	//SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH); // WritesTX_Address to nRF24L01
	//SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); //RX_Addr0 same as TX_Adr for Auto.Ack
	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); // Writes data to TX payload
	sta=SPI_Write_Reg(WRITE_REG + CONFIG, 0x0e); // Set PWR_UP bit, enable CRC(2 u8s) &Prim:TX. MAX_RT & TX_DS enabled..
	NRF_CE_1;
	while(NRF_IRQ!=0);
//	R_OS_TaskSleep(10);
//	NRF_CE_0;
	if(sta&MAX_TX)//最大发送次数
	{
		SPI_Write_Reg(FLUSH_TX,0xff); //清除FIFO缓存寄存器
		return MAX_TX;
	}
	if(sta&TX_OK)//发送完成
	{
		return TX_OK;
	}
	return 0xff;//未知原因
}



void NRF24L01_TX_Mode(u8 NRF2401Channel)
{
	NRF_CE_0;
  	SPI_Write_Buf(WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);//Ð´TX½ÚµãµØÖ·
  	SPI_Write_Buf(WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //ÉèÖÃTX½ÚµãµØÖ·,Ö÷ÒªÎªÁËÊ¹ÄÜACK

  	SPI_Write_Reg(WRITE_REG+EN_AA,0x01);     //Ê¹ÄÜÍ¨µÀ0µÄ×Ô¶¯Ó¦´ð
  	SPI_Write_Reg(WRITE_REG+EN_RXADDR,0x01); //Ê¹ÄÜÍ¨µÀ0µÄ½ÓÊÕµØÖ·
  	SPI_Write_Reg(WRITE_REG+SETUP_RETR,0x1a);//ÉèÖÃ×Ô¶¯ÖØ·¢¼ä¸ôÊ±¼ä:500us + 86us;×î´ó×Ô¶¯ÖØ·¢´ÎÊý:10´Î
  	SPI_Write_Reg(WRITE_REG+RF_CH,NRF2401Channel);       //ÉèÖÃRFÍ¨µÀÎª40
  	SPI_Write_Reg(WRITE_REG+RF_SETUP,0x0f);  //ÉèÖÃTX·¢Éä²ÎÊý,0dbÔöÒæ,2Mbps,µÍÔëÉùÔöÒæ¿ªÆô
  	SPI_Write_Reg(WRITE_REG+CONFIG,0x0e);    //ÅäÖÃ»ù±¾¹¤×÷Ä£Ê½µÄ²ÎÊý;PWR_UP,EN_CRC,16BIT_CRC,½ÓÊÕÄ£Ê½,¿ªÆôËùÓÐÖÐ¶Ï
	NRF_CE_1;//CEÎª¸ß,10usºóÆô¶¯·¢ËÍ
}

void NRF24L01_RX_Mode(u8 NRF2401Channel)
{
	NRF_CE_0;
  	SPI_Write_Buf(WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//Ð´RX½ÚµãµØÖ·

  	SPI_Write_Reg(WRITE_REG+EN_AA,0x01);    	//Ê¹ÄÜÍ¨µÀ0µÄ×Ô¶¯Ó¦´ð
  	SPI_Write_Reg(WRITE_REG+EN_RXADDR,0x01);	//Ê¹ÄÜÍ¨µÀ0µÄ½ÓÊÕµØÖ·
  	SPI_Write_Reg(WRITE_REG+RF_CH,NRF2401Channel);	    	//ÉèÖÃRFÍ¨ÐÅÆµÂÊ
  	SPI_Write_Reg(WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//Ñ¡ÔñÍ¨µÀ0µÄÓÐÐ§Êý¾Ý¿í¶È
  	SPI_Write_Reg(WRITE_REG+RF_SETUP,0x0f);	//ÉèÖÃTX·¢Éä²ÎÊý,0dbÔöÒæ,2Mbps,µÍÔëÉùÔöÒæ¿ªÆô
  	SPI_Write_Reg(WRITE_REG+CONFIG, 0x0f);		//ÅäÖÃ»ù±¾¹¤×÷Ä£Ê½µÄ²ÎÊý;PWR_UP,EN_CRC,16BIT_CRC,½ÓÊÕÄ£Ê½
  	NRF_CE_1; //进入接收模式
}
// 配置函数
void nRF24L01_Config(void)
{
	//initial io
	NRF_CE_0; // chip enable
	NRF_CSN_1; // Spi disable
	NRF_SCK_0; // Spi clock line init high
	NRF_CE_0;
	SPI_Write_Reg(WRITE_REG + CONFIG, 0x0f); // Set PWR_UP bit, enable CRC(2 u8s) &Prim:RX. RX_DR enabled..
	SPI_Write_Reg(WRITE_REG + EN_AA, 0x01);
	SPI_Write_Reg(WRITE_REG + EN_RXADDR, 0x01); // Enable Pipe0
	SPI_Write_Reg(WRITE_REG + SETUP_AW, 0x02); // Setup address width=5 BYTEs
	SPI_Write_Reg(WRITE_REG + SETUP_RETR, 0x1a); // 500us + 86us, 10 retrans...
	SPI_Write_Reg(WRITE_REG + RF_CH, 0);
	SPI_Write_Reg(WRITE_REG + RF_SETUP, 0x07); // TX_PWR:0dBm, Datarate:1Mbps,LNA:HCURR
	SPI_Write_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);
	SPI_Write_Buf(WRITE_REG + TX_ADDR, (u8*)TX_ADDRESS, TX_ADR_WIDTH);
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, (u8*)RX_ADDRESS, RX_ADR_WIDTH);
	NRF_CE_1; //
}


int Read_Bit(int_t handle ,st_r_drv_gpio_pin_rw_t pin){

    int_t gpio_err;
    e_r_drv_gpio_level_t ret;

    gpio_err = direct_control(handle, CTL_GPIO_PIN_READ, &pin);
    if (gpio_err < 0)
    {
        ret = GPIO_LEVEL_SC_DEFAULT;
    }
    else
    {
        ret = pin.level;
    }
    return (int)(ret-1);
}
