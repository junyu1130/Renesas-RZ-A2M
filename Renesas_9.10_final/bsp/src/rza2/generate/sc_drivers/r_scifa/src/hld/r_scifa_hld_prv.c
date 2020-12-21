/*******************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only
 * intended for use with Renesas products. No other uses are authorized. This
 * software is owned by Renesas Electronics Corporation and is protected under
 * all applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
 * LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
 * TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
 * ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
 * FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
 * ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
 * BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software
 * and to discontinue the availability of this software. By using this software,
 * you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
 *******************************************************************************/
/*******************************************************************************
 * @file           r_scifa_hld_prv.c
 * @brief          SCIFA driver support for all available SCIFA channels
 * @version        1.00
 * @date           15.05.2018
 *******************************************************************************/
/*******************************************************************************
 * History       : DD.MM.YYYY Version Description
 *               : 11.05.2018 1.00
 *******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "r_os_abstraction_api.h"

/* Driver Includes */
#include "r_scifa_drv_api.h"     /* SCIFA Driver API Header*/
#include "r_scifa_drv_link.h"    /* Link include to low level driver*/
#include "r_scifa_drv_sc_cfg.h"  /* Smart Configurator set up*/
#include "r_scifa_hld_prv.h"     /* Private HLD functions*/
#include "r_stb_lld_rza2m.h"     /* Standby driver needed here */
#include "r_intc_drv_api.h"      /* TODO: replace with ssgstarc driver*/
#include "r_cpg_lld_rza2m.h"     /* TODO: replace with real cpg HLD driver not just FIT LLD interface */
#include "driver.h"              /* Device Driver common header */


/* Middleware */
#include "r_cbuffer.h"

/*******************************************************************************
 Macro definitions
 *******************************************************************************/

/**Size of internal TX and RX circular buffers for channels*/
//#define BUFF_SIZE_PRV_              (1024)
#define BUFF_SIZE_PRV_              (8192)

typedef struct
{
	scifa_config_t          cfg;                    /*!< Configuration struct */
    st_scifa_rx_error_t     rx_err;                 /*!< Current State of RX */
    scifa_rx_status_t        tx_state;               /*!< Current State of TX */
    PCBUFF                  p_rx_cbuff;             /*!< Pointer to RX Buffer */
    PCBUFF                  p_tx_cbuff;             /*!< Pointer to TX Buffer */
    void *                  tx_mutex;               /*!< transmit mutex */
    void *                  rx_mutex;               /*!< receive mutex */
    void *                  control_mutex;          /*!< control mutex */
    int_t					open_count;			    /*!< open count */
    int_t					buffer_overflow_count;	/*!< receive buffer overflow count */
} st_r_scifa_hld_ch_t;

/*****************************************************************************
 Private Functions
 ******************************************************************************/
static int_t set_config(int_t channel, const scifa_config_t* p_config);
static int_t scifa_setup(int_t channel, const st_r_drv_scifa_sc_config_t* p_config);
static void start_tx(int_t channel, const uint8_t *p_buffer, uint32_t ui_count);
static void TXI_handler(uint32_t sense);
static void RXI_handler(uint32_t sense);

/* Channel control */
static st_r_scifa_hld_ch_t gs_ch_ctrl[SCIFA_CFG_LLD_NUM_CHANNELS];

/**
 * @brief r_scifa_hld_prv_open opens the SCIFA device driver.
 *
 * Configures the SCIFA channel, transmission and reception buffer depending on
 * the smart configurator.
 *
 * @param[in]  channel Channel to open.
 * @param[in]  file_mode File mode for R/W access.
 *
 * @retval 0  Success
 * @retval -1 Fail
*/
int_t r_scifa_hld_prv_open(int_t channel, int_t file_mode)
{
    int_t ret = DRV_ERROR;
    int_t sc_index = 0;
    int_t sc_table_size = (sizeof(SCIFA_SC_TABLE)) / (sizeof(st_r_drv_scifa_sc_config_t));

    if ((channel > R_CFG_SCIFA_CHANNELS_SUPPORTED) || (channel < 0))
	{
		return (DRV_ERROR);
	}

    /* create the control mutex if it hasn't been created yet */
    if (NULL == gs_ch_ctrl[channel].control_mutex)
    {
        R_OS_EnterCritical();
        gs_ch_ctrl[channel].control_mutex = R_OS_MutexCreate();
        R_OS_MutexAcquire(gs_ch_ctrl[channel].control_mutex);
        R_OS_ExitCritical();
    }

    if (0 == gs_ch_ctrl[channel].open_count)
    {
		for (sc_index = 0; sc_index < sc_table_size; sc_index++)
		{
			/* Search for channel Smart Configurator configuration */
			if (channel == SCIFA_SC_TABLE[sc_index].channel)
			{
				/* Pass in specific channel set up */
				ret = scifa_setup(channel, &SCIFA_SC_TABLE[sc_index]);

				/* Start channel if successful */
				if (DRV_SUCCESS == ret)
				{
					/* is the write attribute set? */
					if ((file_mode & __SWR) > 0)
					{
						/* Allocate transmit Buffer */
						if (NULL == gs_ch_ctrl[channel].p_tx_cbuff)
						{
							gs_ch_ctrl[channel].p_tx_cbuff = cbCreate(BUFF_SIZE_PRV_);
						}

						if (NULL == gs_ch_ctrl[channel].tx_mutex)
						{
							gs_ch_ctrl[channel].tx_mutex = R_OS_MutexCreate();
						}

						/* Set to Idle state */
						gs_ch_ctrl[channel].tx_state = SCIFA_TX_STATUS_IDLE;
					}

					/* is the read attribute set? */
					if ((file_mode & __SRD) > 0)
					{
						/* Allocate receive buffer */
						if (NULL == gs_ch_ctrl[channel].p_rx_cbuff)
						{
							gs_ch_ctrl[channel].p_rx_cbuff = cbCreate(BUFF_SIZE_PRV_);
						}

						if (NULL == gs_ch_ctrl[channel].rx_mutex)
						{
							gs_ch_ctrl[channel].rx_mutex = R_OS_MutexCreate();
						}
					}

					/* Start channel */
					ret = R_SCIFA_Start(channel, file_mode);
				}

				/* Terminate loop */
				sc_index += sc_table_size;
			}
		}
    }
    else
    {
    	ret = DRV_SUCCESS;
    }

    /* if we were successful then update the open counter */
    if (DRV_SUCCESS == ret)
    {
    	gs_ch_ctrl[channel].open_count++;
    }

    R_OS_MutexRelease(gs_ch_ctrl[channel].control_mutex);

    /* return status */
    return (ret);
}
/******************************************************************************
 * End of Function r_scifa_hld_prv_open
 ******************************************************************************/

/**
 * @brief r_scifa_hld_prv_close closes the SCIFA device driver
 *
 * The close function destroys the resources used in opening
 * a channel.
 *
 * @param[in] channel Channel to close.
 *
 * @return None.
 *
*/
void r_scifa_hld_prv_close(int_t channel)
{
    if ((channel >= R_CFG_SCIFA_CHANNELS_SUPPORTED_OLD) || (channel < 0))
    {
        return;
    }

    /* if the mutex has been destroyed, then we must already be closed */
    if (0 == gs_ch_ctrl[channel].control_mutex)
    {
    	return;
    }

    R_OS_MutexAcquire(gs_ch_ctrl[channel].control_mutex);

    gs_ch_ctrl[channel].open_count--;

    if (0 == gs_ch_ctrl[channel].open_count)
    {
		/* stop transmit and receive */
		R_SCIFA_Stop(channel);
		
	    switch (channel)
	    {
			case (0):
	        {
                R_STB_StopModule(MODULE_SCIFA0);
                break;
	        }

			case (1):
	        {
                R_STB_StopModule(MODULE_SCIFA1);
                break;
	        }

			case (2):
	        {
                R_STB_StopModule(MODULE_SCIFA2);
                break;
	        }

			case (3):
	        {
                R_STB_StopModule(MODULE_SCIFA3);
                break;
	        }

			case (4):
	        {
                R_STB_StopModule(MODULE_SCIFA4);
                break;
	        }

			default:
		    {
                break;
		    }
        }

		if (0 != gs_ch_ctrl[channel].p_tx_cbuff)
		{
			/* delete transmit buffer and mutex */
			cbDestroy(gs_ch_ctrl[channel].p_tx_cbuff);
			gs_ch_ctrl[channel].p_tx_cbuff = 0;
			R_OS_MutexDelete(&gs_ch_ctrl[channel].tx_mutex);
			gs_ch_ctrl[channel].tx_mutex = 0;
		}

		if (0 != gs_ch_ctrl[channel].p_rx_cbuff)
		{
			/* delete receive buffer and mutex */
			cbDestroy(gs_ch_ctrl[channel].p_rx_cbuff);
			gs_ch_ctrl[channel].p_rx_cbuff = 0;
			R_OS_MutexDelete(&gs_ch_ctrl[channel].rx_mutex);
			gs_ch_ctrl[channel].rx_mutex = 0;
		}

		R_OS_EnterCritical();
		R_OS_MutexDelete(&gs_ch_ctrl[channel].control_mutex);
		gs_ch_ctrl[channel].control_mutex = 0;
		R_OS_ExitCritical();
    }
    else
    {
        R_OS_MutexRelease(gs_ch_ctrl[channel].control_mutex);
    }
}
/******************************************************************************
 * End of Function r_scifa_hld_prv_close
******************************************************************************/

/**
 * @brief r_scifa_hld_prv_read reads data from the SCIFA
 *
 * This function reads the data received from the chosen channel 
 * and stores it in the passed by reference buffer.
 *
 * @param[in] channel Channel to read from.
 *
 * @param[out] *p_buffer Buffer used to store data.
 *
 * @param[in] max_count Max amount of bytes to be read.
 *
 * @retval  N Where N is the number of bytes read
 * @retval -1 Fail
*/
int_t r_scifa_hld_prv_read(int_t channel, uint8_t *p_buffer, uint32_t max_count)
{
    int_t stored_data_length;
    int_t bytes_read = 0;
	int_t result;
	uint8_t data;
	int_t fifo_empty;

    if ((channel >= R_CFG_SCIFA_CHANNELS_SUPPORTED_OLD) || (channel < 0))
    {
        return (DRV_ERROR);
    }

    /* control access by multiple threads */
    R_OS_MutexAcquire(gs_ch_ctrl[channel].rx_mutex);

    /* don't allow the receive interrupt routine to change the buffer while we're accessing it */
    R_OS_EnterCritical();

    stored_data_length = cbUsed(gs_ch_ctrl[channel].p_rx_cbuff);

    if (stored_data_length > 0)
    {
        /* transfer data from the receive buffer */
        for (bytes_read = 0; (bytes_read < stored_data_length) && (bytes_read < max_count); bytes_read++)
        {
            cbGet(gs_ch_ctrl[channel].p_rx_cbuff, &p_buffer[bytes_read]);
        }

		/* if we haven't read as many characters as requested */
		if (bytes_read < max_count)
		{
			/* there may be a few characters in the FIFO that haven't reached the level that triggers a receive interrupt */
			do
			{
				R_SCIFA_IsRxFifoEmpty(channel, &fifo_empty);

				if (!fifo_empty)
				{
					R_SCIFA_GetByte(channel, &data);
					p_buffer[bytes_read] = data;
					bytes_read++;
				}
			}
			while ((bytes_read < max_count) && (!fifo_empty));
		}
    }

    /* we've finished with the buffer */
    R_OS_ExitCritical();

    R_OS_MutexRelease(gs_ch_ctrl[channel].rx_mutex);

    return (bytes_read);
}
/******************************************************************************
 * End of Function r_scifa_hld_prv_read
******************************************************************************/

/**
 * @brief r_scifa_hld_prv_write writes data to the SCIFA device.
 *
 * Contains a state machine to handle writing when the SCIFA,
 * is closed, Running, idle or broken.
 *
 * @param[in] channel Channel to be written to.
 *
 * @param[in] p_buffer Buffer of data to be written
 *
 * @param[in] ui_count Number of bytes to be written.
 *
 * @retval 0  Success
 * @retval -1 Fail
*/
int_t r_scifa_hld_prv_write(int_t channel, const uint8_t *p_buffer, uint32_t count)
{
    int_t ret = DRV_ERROR;

    if ((channel >= R_CFG_SCIFA_CHANNELS_SUPPORTED_OLD) || (channel < 0))
    {
        return (DRV_ERROR);
    }

    /* control access by multiple threads */
    R_OS_MutexAcquire(gs_ch_ctrl[channel].tx_mutex);

    switch (gs_ch_ctrl[channel].tx_state)
    {
        /* Driver not open */
        case SCIFA_TX_STATUS_CLOSED:
        {
            ret = DRV_ERROR;
        }
        break;

        /* Currently running */
        case SCIFA_TX_STATUS_RUNNING:
        {
			/* Set to busy */
			gs_ch_ctrl[channel].tx_state = SCIFA_TX_STATUS_RUNNING;

			/* Start TX */
			start_tx(channel, p_buffer, count);

			/* Success */
			ret = DRV_SUCCESS;
        }
        break;

        /* TX Break */
        case SCIFA_TX_STATUS_BREAK:
        {
    		__asm volatile("NOP");
        }
        break;

        /* TX Available */
        case SCIFA_TX_STATUS_IDLE:
        {
            /* Set to busy */
            gs_ch_ctrl[channel].tx_state = SCIFA_TX_STATUS_RUNNING;

            /* Start TX */
            start_tx(channel, p_buffer, count);

            /* Success */
            ret = DRV_SUCCESS;
        }
        break;

        default:
        {
            /* Strange Place */
            ret = DRV_ERROR;
        }
        break;
    }

    R_OS_MutexRelease(gs_ch_ctrl[channel].tx_mutex);

    return (ret);
}
/******************************************************************************
 * End of Function r_scifa_hld_prv_write
******************************************************************************/

/**
 * @brief r_scifa_hld_prv_set_config sets SCIFA configuration.
 *
 * This function configures and starts the SCIFA.
 *
 * @param[in] channel Channel to configure.
 *
 * @param[in] p_config The data to be used to configure the channel.
 *                                     
 * @param[in] file_mode The required file mode (read, write etc.)
 *
 * @retval 0  Success
 * @retval -1 Fail
 */
int_t r_scifa_hld_prv_set_config(int_t channel, scifa_config_t* p_config, int_t file_mode)
{
    int_t ret;

    if ((channel >= R_CFG_SCIFA_CHANNELS_SUPPORTED_OLD) || (channel < 0))
    {
        return (DRV_ERROR);
    }

    R_OS_MutexAcquire(gs_ch_ctrl[channel].control_mutex);

    ret = R_SCIFA_Stop(channel);

    /* Call setup */
    ret = set_config(channel, p_config);

    /* Start channel */
    ret = R_SCIFA_Start(channel, file_mode);

    R_OS_MutexRelease(gs_ch_ctrl[channel].control_mutex);

    return (ret);
}
/******************************************************************************
 * End of Function r_scifa_hld_prv_set_config
******************************************************************************/

/**
 * @brief r_scifa_hld_prv_get_config Retrieves the current configuration 
 * of the SCIFA driver.
 *
 * Allows for the reading of current SCIFA settings. This allows for safe
 * changes to the current config of the SCIFA.
 *
 * @param[in] channel:  Channel to get configuration for.
 * @param[in] p_config: Configuration struct to populate with
 *                      retrieved configuration.
 *
 * @retval  0 Success
 * @retval -1 Fail
*/
int_t r_scifa_hld_prv_get_config(int_t channel, scifa_config_t* p_config)
{
    if ((channel >= R_CFG_SCIFA_CHANNELS_SUPPORTED_OLD) || (channel < 0))
    {
        return (DRV_ERROR);
    }

    R_OS_MutexAcquire(gs_ch_ctrl[channel].control_mutex);

    /* Copy currently held settings for channel */
    *p_config = gs_ch_ctrl[channel].cfg;

    R_OS_MutexRelease(gs_ch_ctrl[channel].control_mutex);

    return (DRV_SUCCESS);
}
/******************************************************************************
 * End of Function r_scifa_hld_prv_get_config
******************************************************************************/

/**
 * @brief start_tx starts data transmission
 *
 * @param[in] channel Channel to send data.
 *
 * @param[in] p_buffer Location of  data to send.
 *
 * @param[in] ui_count Length of data to send.
 *
 * @retval  0 Success
 * @retval -1 Fail
 */
static void start_tx(int_t channel, const uint8_t *p_buffer, uint32_t ui_count)
{
    int_t free_space;
    int_t i;

    /* we mustn't be interrupted while manipulating the buffer */
    R_OS_EnterCritical();

    /* find out how much free space there is in the buffer */
    free_space = cbFree(gs_ch_ctrl[channel].p_tx_cbuff);

    /* copy as much of the data as possible into the transmit buffer */
    for (i = 0; (i < ui_count) && (i < free_space); i++)
    {
        cbPut(gs_ch_ctrl[channel].p_tx_cbuff, p_buffer[i]);
    }

    /* we're done with the buffer */
    R_OS_ExitCritical();

    /* set SCR.TIE bit to start transmission */
    R_SCIFA_StartTXI(channel);

    /* put the remaining data into the buffer as space becomes available */
    while (i < ui_count)
    {
        /* we mustn't be interrupted while manipulating the buffer */
        R_OS_EnterCritical();

        free_space = cbFree(gs_ch_ctrl[channel].p_tx_cbuff);

        if (free_space > 0)
        {
            /* fill up the free space */
            while ((i < ui_count) && (free_space > 0))
            {
                cbPut(gs_ch_ctrl[channel].p_tx_cbuff, p_buffer[i]);
                i++;
                free_space--;
            }

            /* we're done with the buffer */
            R_OS_ExitCritical();

            /* just in case the buffer emptied completely */
            R_SCIFA_StartTXI(channel);
        }
        else
        {
            /* we're done with the buffer */
            R_OS_ExitCritical();

            /* hand over CPU cycles to other tasks while we wait */
            R_OS_TaskSleep(5);
        }
    }
}
/******************************************************************************
 * End of Function start_tx
 ******************************************************************************/

/**
 * @brief setup_interrupts
 *        This will be replaced by the R_INTC driver
 * @return None
 */
static int_t setup_interrupts(int_t channel, const scifa_config_t *p_cfg)
{
    if ((channel > R_PRV_SCIF_LAST_CHANNEL) || (channel < 0))
    {
        return DRV_ERROR;
    }

	/* Set up interrupts */
    switch (channel)
    {
    	case 0: /* channel 0 */
    	{
			R_INTC_RegistIntFunc(INTC_ID_SCIFA_TXI0, TXI_handler);
			R_INTC_Enable(INTC_ID_SCIFA_TXI0);
			R_INTC_SetPriority(INTC_ID_SCIFA_TXI0, p_cfg->txi_priority);

			R_INTC_RegistIntFunc(INTC_ID_SCIFA_RXI0, RXI_handler);
			R_INTC_Enable(INTC_ID_SCIFA_RXI0);
			R_INTC_SetPriority(INTC_ID_SCIFA_RXI0, p_cfg->rxi_priority);
			break;
    	}

    	case 1: /* channel 1 */
    	{
			R_INTC_RegistIntFunc(INTC_ID_SCIFA_TXI1, TXI_handler);
			R_INTC_Enable(INTC_ID_SCIFA_TXI1);
			R_INTC_SetPriority(INTC_ID_SCIFA_TXI1,  p_cfg->txi_priority);

			R_INTC_RegistIntFunc(INTC_ID_SCIFA_RXI1, RXI_handler);
			R_INTC_Enable(INTC_ID_SCIFA_RXI1);
			R_INTC_SetPriority(INTC_ID_SCIFA_RXI1, p_cfg->rxi_priority);
			break;
    	}

    	case 2: /* channel 2 */
    	{
			R_INTC_RegistIntFunc(INTC_ID_SCIFA_TXI2, TXI_handler);
			R_INTC_Enable(INTC_ID_SCIFA_TXI2);
			R_INTC_SetPriority(INTC_ID_SCIFA_TXI2,  p_cfg->txi_priority);

			R_INTC_RegistIntFunc(INTC_ID_SCIFA_RXI2, RXI_handler);
			R_INTC_Enable(INTC_ID_SCIFA_RXI2);
			R_INTC_SetPriority(INTC_ID_SCIFA_RXI2, p_cfg->rxi_priority);
			break;
    	}

    	case 3: /* channel 3 */
    	{
			R_INTC_RegistIntFunc(INTC_ID_SCIFA_TXI3, TXI_handler);
			R_INTC_Enable(INTC_ID_SCIFA_TXI3);
			R_INTC_SetPriority(INTC_ID_SCIFA_TXI3,  p_cfg->txi_priority);

			R_INTC_RegistIntFunc(INTC_ID_SCIFA_RXI3, RXI_handler);
			R_INTC_Enable(INTC_ID_SCIFA_RXI3);
			R_INTC_SetPriority(INTC_ID_SCIFA_RXI3, p_cfg->rxi_priority);
			break;
    	}

    	case 4: /* channel 4 */
    	{
			R_INTC_RegistIntFunc(INTC_ID_SCIFA_TXI4, TXI_handler);
			R_INTC_Enable(INTC_ID_SCIFA_TXI4);
			R_INTC_SetPriority(INTC_ID_SCIFA_TXI4, p_cfg->txi_priority);

			R_INTC_RegistIntFunc(INTC_ID_SCIFA_RXI4, RXI_handler);
			R_INTC_Enable(INTC_ID_SCIFA_RXI4);
			R_INTC_SetPriority(INTC_ID_SCIFA_RXI4, p_cfg->rxi_priority);
			break;
    	}
    }
    return DRV_SUCCESS;
}
/******************************************************************************
 * End of Function setup_interrupts
 ******************************************************************************/

/**
 * @brief set_config calls LLD functions to set up the SCIFA driver
 *
 * @param[in] p_config Pointer to configuration struct.
 *
 * @param[in] p_config Channel to setup.
 *
 * @retval 0  Success
 * @retval -1 Fail
 */
static int_t set_config(int_t channel, const scifa_config_t* p_config)
{
    int_t ret;
    uint32_t achieved_baud;
    uint32_t current_clk_hz = 0;
    uint32_t achieved_baud_rate;

    /* Stop the device */
    ret = R_SCIFA_Stop(channel);

    switch (channel)
    {
		case (0):
	    {
		    R_STB_StartModule(MODULE_SCIFA0);
		    break;
	    }

		case (1):
	    {
		    R_STB_StartModule(MODULE_SCIFA1);
		    break;
	    }

		case (2):
        {
		    R_STB_StartModule(MODULE_SCIFA2);
		    break;
        }

		case (3):
	    {
		    R_STB_StartModule(MODULE_SCIFA3);
		    break;
        }

		case (4):
	    {
		    R_STB_StartModule(MODULE_SCIFA4);
		    break;
        }

		default:
        {
			ret = DRV_ERROR;
        }
		break;
    }

    if (DRV_SUCCESS == ret)
    {
        /* Initialise the device for setup */
        ret = R_SCIFA_Init(channel);
    }

    if ((DRV_SUCCESS == ret) && (p_config->parity_en == SCIFA_PARITY_ON))
    {
        ret = R_SCIFA_SetParity(channel, p_config->parity_type);                   	/* Set parity */
    }
    else
    {
    	/* Set Parity off as SCIFA_PARITY_NONE no longer supported */
        ret = R_SCIFA_SetParity(channel, SCIFA_NONE_PARITY);                        /* Set parity */
    }

    if (DRV_SUCCESS == ret)
    {
    	/* Store parity setting or SCIFA_NONE_PARITY if disabled */
        if (p_config->parity_en == SCIFA_PARITY_ON)
        {
        	gs_ch_ctrl[channel].cfg.parity_type = p_config->parity_type;	        /* Save new parity setting */
        }
        else
        {
        	gs_ch_ctrl[channel].cfg.parity_type = SCIFA_NONE_PARITY;	            /* Save new parity setting */
        }

        ret = R_SCIFA_SetStopBits(channel, p_config->stop_bits);                    /* Set stop Bits */
    }

    if (DRV_SUCCESS == ret)
    {
        gs_ch_ctrl[channel].cfg.stop_bits = p_config->stop_bits;                    /* Save new stop bits setting */
        ret = R_SICFA_SetTransmissionMode(channel, p_config->sync_mode);            /* Set transmission mode */
    }

    if (DRV_SUCCESS == ret)
    {
        gs_ch_ctrl[channel].cfg.sync_mode = p_config->sync_mode;                    /* Save new mode setting */
        ret = R_SCIFA_SetDataBits(channel, p_config->data_size);                    /* Set data bits */
    }

    if (DRV_SUCCESS == ret)
    {
        double_t p1clk;
        uint32_t current_clk_hz = 66000000;

        gs_ch_ctrl[channel].cfg.data_size = p_config->data_size;                    /* Save new data bits setting */

        R_CPG_GetClock(CPG_FREQ_P1CLK, &p1clk);

        /* convert data from cpg to scifa requirements; */
//        current_clk_hz = ((uint32_t) (p1clk)) * 1000;

        ret = R_SCIFA_SetBaud(channel, p_config->baud_rate, current_clk_hz, p_config->sync_mode, &achieved_baud);    /* Set baud */


    }

    if (DRV_SUCCESS == ret)
    {
        gs_ch_ctrl[channel].cfg.baud_rate = achieved_baud;                          /* Save new baud setting */
        ret = R_SCIFA_SetLoopBack(channel, p_config->loopback);                     /* Set Loop back */
    }

    if (DRV_SUCCESS == ret)
    {
        gs_ch_ctrl[channel].cfg.loopback = p_config->loopback;                      /* Save loop back setting */
        ret = R_SCIFA_SetDataOrder(channel, p_config->data_direction);              /* Set data order */
    }

    if (DRV_SUCCESS == ret)
    {
    	gs_ch_ctrl[channel].cfg.data_direction = p_config->data_direction;          /* Save data order setting */
    	ret = R_SCIFA_SetTXDOutput(channel, p_config->txd_init_value);              /* set TXD output */
    }

    if (DRV_SUCCESS == ret)
    {
    	gs_ch_ctrl[channel].cfg.txd_init_value = p_config->txd_init_value;          /* Save TXD initial value setting */
    	ret = R_SCIFA_SetSCKOutput(channel, p_config->sck_init_value);              /* set SCK output */
    }

    if (DRV_SUCCESS == ret)
    {
    	gs_ch_ctrl[channel].cfg.sck_init_value = p_config->sck_init_value;          /* Save SCK initial value setting */

    	/* RTS not available on channel 3 or channel 4 */
    	if ((3 == channel) || (4 == channel))
    	{
            ret = DRV_SUCCESS;
    	}
    	else
    	{
    	    ret = R_SCIFA_SetRTSOutput(channel, p_config->rts_init_value);          /* set RTS output */
    	}
    }

    if (DRV_SUCCESS == ret)
    {
    	gs_ch_ctrl[channel].cfg.rts_init_value = p_config->rts_init_value;          /* Save RTS initial value setting */

    	/* CTS not available on channel 3 or channel 4 */
    	if ((3 == channel) || (4 == channel))
    	{
            ret = DRV_SUCCESS;
    	}
    	else
    	{
    	    ret = R_SCIFA_SetCTSOutput(channel, p_config->cts_init_value);          /* set CTS output */
    	}
    }

    if (DRV_SUCCESS == ret)
    {
    	gs_ch_ctrl[channel].cfg.cts_init_value = p_config->cts_init_value;          /* Save CTS initial value setting */
        ret = R_SCIFA_SetExtendedCfg(channel, p_config);                            /* Set extended features */
    }

    if (DRV_SUCCESS == ret)
    {
    	gs_ch_ctrl[channel].cfg.clk_enable      = p_config->clk_enable;
    	gs_ch_ctrl[channel].cfg.noise_canceller = p_config->noise_canceller;
    	gs_ch_ctrl[channel].cfg.modem_control   = p_config->modem_control;
    	gs_ch_ctrl[channel].cfg.tftc            = p_config->tftc;
    	gs_ch_ctrl[channel].cfg.rftc            = p_config->rftc;

        setup_interrupts(channel,  p_config);
    }

    return(ret);
}


/**
 * @brief scifa_setup set up the SCIFA driver
 *
 * @param[in] p_config Pointer to configuration struct.
 *
 * @param[in] p_config Channel to setup.
 *
 * @retval 0  Success
 * @retval -1 Fail
 */
static int_t scifa_setup(int_t channel, const st_r_drv_scifa_sc_config_t* p_config)
{
    int_t ret;

    ret = set_config(channel, &p_config->config);

    /* Set up IO pins */
    if (DRV_SUCCESS == ret)
    {
        int_t gpio_handle = (-1);
    	gpio_handle = direct_open("gpio", 0);

    	if ((-1) != gpio_handle)
    	{
    		/* direct_control as direct_control assumes parameter 3 is i/o, but using the parameter in input only mode is acceptable */
    		direct_control(gpio_handle, CTL_GPIO_INIT_BY_TABLE, (st_r_drv_scifa_sc_config_t *) &p_config->pin);
    		direct_close(gpio_handle);
    	}
    }

    return (ret);
}
/******************************************************************************
 * End of Function scifa_setup
******************************************************************************/

/**
 * @brief TXI_handler handles TXI interrupts
 *
 * @param[in] sense: Not used, but required by current INTC driver.
 *                   Expected to remove following new intc driver.
 *
 * @return None
 */
static void TXI_handler(uint32_t sense)
{
    int_t channel;                              /* Channel search index */
    volatile int_t channel_state;
    volatile int_t active_int = 0;              /* Active Interrupt channel */
    uint8_t data_to_send;
    int_t fifo_full;

    for (channel = 0; channel < SCIFA_CFG_LLD_NUM_CHANNELS; channel++)
    {
        /* Get channel state */
        channel_state = R_SCIFA_GetTXIState(channel);

        /* Check if interrupt flag is high */
        if (channel_state)
        {
            /* Assign active interrupt */
            active_int = channel;

            /* break out of loop */
            break;
        }
    }

    /* clear the interrupt flags */
    R_SCIFA_ClearTXIFlags(active_int);

    /* while the FIFO is not full, and we still have data to transmit */
    while ((R_SCIFA_IsTxFifoFull(active_int, &fifo_full) == DRV_SUCCESS) && (!fifo_full) && (cbUsed(gs_ch_ctrl[active_int].p_tx_cbuff) > 0))
    {
        /* Retrieve byte from buffer */
        if (cbGet(gs_ch_ctrl[active_int].p_tx_cbuff, &data_to_send))
        {
            /* Send a byte (write to FIFO) */
            R_SCIFA_PutByte(active_int, data_to_send);
        }
    }

    /* Check if data transfer finished */
    if (0 == cbUsed(gs_ch_ctrl[active_int].p_tx_cbuff))
    {
        /* Stop TX */
        R_SCIFA_StopTXI(active_int);

        /* Return to Idle */
        gs_ch_ctrl[active_int].tx_state = SCIFA_TX_STATUS_IDLE;
    }
}
/******************************************************************************
 * End of Function TXI_handler
******************************************************************************/

/**
 * @brief RXI_handler handles RXI interrupts
 *
 * @param[in] sense: Not used, but required by current INTC driver.
 *                   Expected to remove following new intc driver.
 *
 * @return None
 */
static void RXI_handler(uint32_t sense)
{
    int_t channel;                              /* Channel Search index */
    volatile int_t channel_state;
    volatile int_t active_int = 0;              /* Active Interrupt channel */
    uint8_t received_data;
    int_t fifo_empty;

    for (channel = 0; channel < SCIFA_CFG_LLD_NUM_CHANNELS; channel++)
    {
        /* Get channel state */
        channel_state = R_SCIFA_GetRXIState(channel);

        /* Check if interrupt flag is high */
        if (channel_state)
        {
            /* Assign active interrupt */
            active_int = channel;

            /* break out of loop */
            break;
        }
    }

    /* Check error RX flags for channel */
    if (R_SCIFA_CheckRXIError(active_int, &gs_ch_ctrl[active_int].rx_err) == DRV_SUCCESS)
    {
        /* transfer data from the FIFO to the receive buffer */
        while ((R_SCIFA_IsRxFifoEmpty(active_int, &fifo_empty) == DRV_SUCCESS) && (!fifo_empty))
        {
            /* Retrieve byte from FIFO */
            if (R_SCIFA_GetByte(channel, &received_data) == DRV_SUCCESS)
            {
                /* if there's any space in the buffer, store the character, otherwise lose it */
                if (cbFree(gs_ch_ctrl[active_int].p_rx_cbuff) > 0)
                {
                    /* Store byte in buffer */
                    cbPut(gs_ch_ctrl[active_int].p_rx_cbuff, received_data);
                }
                else
                {
                	gs_ch_ctrl[active_int].buffer_overflow_count++;
                }
            }
        }
    }

    /* Clear RX Flags */
    R_SCIFA_ClearRXIFlags(active_int);
}
/******************************************************************************
 * End of Function RXI_handler
******************************************************************************/
