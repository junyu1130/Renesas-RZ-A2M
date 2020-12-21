#ifndef PERIPHERALS_INTERNAL_R_MTU2_H_
#define PERIPHERALS_INTERNAL_R_MTU2_H_

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef enum {
    R_MTU2_PRESCALE_1 = 0,
    R_MTU2_PRESCALE_4,
    R_MTU2_PRESCALE_16,
    R_MTU2_PRESCALE_64,
    R_MTU2_PRESCALE_256,
    R_MTU2_PRESCALE_1024
} R_MTU2_prescale_t;


/******************************************************************************
Functions Prototypes
******************************************************************************/
void   R_MTU2_Initialize_CompareClear(uint8_t timer, R_MTU2_prescale_t prescale);
void   R_MTU2_StartTimer(uint8_t timer);
void   R_MTU2_StopTimer(uint8_t timer);
void   R_MTU2_LoadCompareValue(uint8_t timer, uint16_t value);
bool_t R_MTU2_WaitCompareMatch(uint8_t timer);

#endif /* PERIPHERALS_INTERNAL_R_MTU2_H_ */
