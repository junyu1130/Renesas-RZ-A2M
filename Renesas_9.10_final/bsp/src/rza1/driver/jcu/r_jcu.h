#ifndef _R_JCU_H
#define _R_JCU_H

/******************************************************************************
Includes <System Includes> , “Project Includes”
******************************************************************************/
#include "r_bsp.h"


/******************************************************************************
Typedef definitions
******************************************************************************/
typedef enum {
    R_JCU_I_YCbCr444 = 0b000,
    R_JCU_I_YCbCr422 = 0b001,
    R_JCU_I_YCbCr411 = 0b110,
    R_JCU_I_YCbCr420 = 0b010
} jcu_input_format_t;

typedef enum {
    R_JCU_O_YCbCr422,
    R_JCU_O_ARGB888,
    R_JCU_O_RGB565
} jcu_output_format_t;

typedef enum {
    R_JCU_SWAP_NONE,
    R_JCU_SWAP_BYTE,
    R_JCU_SWAP_WORD,
    R_JCU_SWAP_WORD_BYTE,
    R_JCU_SWAP_LONGWORD,
    R_JCU_SWAP_LONGWORD_BYTE,
    R_JCU_SWAP_LONGWORD_WORD,
    R_JCU_SWAP_LONGWORD_WORD_BYTE
} jcu_swap_t;

typedef enum {
    R_JCU_RANGE_SIGNED = 0x0000,
    R_JCU_RANGE_UNSIGNED = 0x8000
} jcu_range_t;


typedef enum {
    R_JCU_SCALE_1_1 = 0b00,
    R_JCU_SCALE_1_2 = 0b01,
    R_JCU_SCALE_1_4 = 0b10,
    R_JCU_SCALE_1_8 = 0b11
} jcu_scale_t;


typedef enum
{
    JCU_NO_ERROR,
    JCU_DECOMP_ERROR
} jcu_error_t;



typedef struct {
    uint8_t   * addr;
    uint16_t     width;
    uint16_t     height;

    jcu_range_t cbcr_range;
    jcu_swap_t     swap;
} jcu_encode_input_t;

typedef struct {
    uint8_t   * addr;

    jcu_range_t cbcr_range;
    jcu_swap_t     swap;

    uint32_t     size;
} jcu_encode_output_t;

typedef struct {
    jcu_encode_input_t     input;
    jcu_encode_output_t output;
} jcu_encode_t;



typedef struct {
    uint8_t           * addr;

    jcu_input_format_t    format;
    jcu_swap_t             swap;
} jcu_decode_input_t;

typedef struct {
    uint8_t           * addr;

    jcu_output_format_t format;
    jcu_swap_t             swap;

    uint16_t             stride;

    jcu_scale_t            horizontal_scale;
    jcu_scale_t            vertical_scale;

    jcu_error_t         error;
} jcu_decode_output_t;

typedef struct {
    jcu_decode_input_t     input;
    jcu_decode_output_t output;
} jcu_decode_t;


/*[jcu_table_no_t] The index of the table */
typedef enum
{
    JCU_TABLE_NO_0 = 0,
    JCU_TABLE_NO_1 = 1,
    JCU_TABLE_NO_2 = 2,
    JCU_TABLE_NO_3 = 3
} jcu_table_no_t;

/*[jcu_huff_t] Kind of Huffman table */
typedef enum
{
    JCU_HUFFMAN_AC,
    JCU_HUFFMAN_DC
} jcu_huff_t;


/******************************************************************************
Functions Prototypes
******************************************************************************/
void R_JCU_EncodeInitialize(jcu_encode_t * params);
void R_JCU_EncodeStart(void);


void R_JCU_DecodeInitialize(jcu_decode_t * params);
void R_JCU_DecodeStart(void);

void r_jcu_set_quantization_table(jcu_table_no_t number, const uint8_t * table);
void r_jcu_set_huffman_table(jcu_table_no_t number, jcu_huff_t huff, const uint8_t * table);

#endif // _R_JCU_H
