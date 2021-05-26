#ifndef FT812_h
#define FT812_h
   
/* Include Files */

#include <Arduino.h>

/* Declarations */

#define FT_DL_SIZE            (8*1024)  //8KB Display List buffer size
#define FT_CMD_FIFO_SIZE      (4*1024)  //4KB coprocessor Fifo size
#define FT_CMD_SIZE           (4)       //4 byte per copro. cmd of EVE

#define FT812_VERSION         "1.0"

// Refer to the FT812 Datasheet

// FT812 Memory Map Addresses
#define RAM_G                 0x000000UL
#define ROM_FONT              0x1E0000UL
#define ROM_FONT_ADDR         0x2FFFFCUL
#define RAM_DL                0x300000UL
#define RAM_REG               0x302000UL
#define RAM_CMD               0x308000UL

// FT812 Register Addresses
#define REG_ID                0x302000UL
#define REG_FRAMES            0x302004UL
#define REG_CLOCK             0x302008UL
#define REG_FREQUENCY         0x30200CUL
#define REG_RENDERMODE        0x302010UL
#define REG_SNAPY             0x302014UL
#define REG_SNAPSHOT          0x302018UL
#define REG_SNAPFORMAT        0x30201CUL
#define REG_CPURESET          0x302020UL
#define REG_TAP_CRC           0x302024UL
#define REG_TAP_MASK          0x302028UL
#define REG_HCYCLE            0x30202CUL
#define REG_HOFFSET           0x302030UL
#define REG_HSIZE             0x302034UL
#define REG_HSYNC0            0x302038UL
#define REG_HSYNC1            0x30203CUL
#define REG_VCYCLE            0x302040UL
#define REG_VOFFSET           0x302044UL
#define REG_VSIZE             0x302048UL
#define REG_VSYNC0            0x30204CUL
#define REG_VSYNC1            0x302050UL
#define REG_DLSWAP            0x302054UL
#define REG_ROTATE            0x302058UL
#define REG_OUTBITS           0x30205CUL
#define REG_DITHER            0x302060UL
#define REG_SWIZZLE           0x302064UL
#define REG_CSPREAD           0x302068UL
#define REG_PCLK_POL          0x30206CUL
#define REG_PCLK              0x302070UL
#define REG_TAG_X             0x302074UL
#define REG_TAG_Y             0x302078UL
#define REG_TAG               0x30207CUL
#define REG_VOL_PB            0x302080UL
#define REG_VOL_SOUND         0x302084UL
#define REG_SOUND             0x302088UL
#define REG_PLAY              0x30208CUL
#define REG_GPIO_DIR          0x302090UL
#define REG_GPIO              0x302094UL
#define REG_GPIOX_DIR         0x302098UL
#define REG_GPIOX             0x30209CUL
#define REG_INT_FLAGS         0x3020A8UL
#define REG_INT_EN            0x3020ACUL
#define REG_INT_MASK          0x3020B0UL
#define REG_PLAYBACK_START    0x3020B4UL
#define REG_PLAYBACK_LENGTH   0x3020B8UL
#define REG_PLAYBACK_READPTR  0x3020BCUL
#define REG_PLAYBACK_FREQ     0x3020C0UL
#define REG_PLAYBACK_FORMAT   0x3020C4UL
#define REG_PLAYBACK_LOOP     0x3020C8UL
#define REG_PLAYBACK_PLAY     0x3020CCUL
#define REG_PWM_HZ            0x3020D0UL
#define REG_PWM_DUTY          0x3020D4UL
#define REG_MACRO_0           0x3020D8UL
#define REG_MACRO_1           0x3020DCUL
#define REG_CMD_READ          0x3020F8UL
#define REG_CMD_WRITE         0x3020FCUL
#define REG_CMD_DL            0x302100UL
#define REG_TOUCH_MODE        0x302104UL
#define REG_TOUCH_ADC_MODE    0x302108UL
#define REG_CTOUCH_EXTENDED   0x302108UL
#define REG_TOUCH_CHARGE      0x30210CUL
#define REG_TOUCH_SETTLE      0x302110UL
#define REG_TOUCH_OVERSAMPLE  0x302114UL
#define REG_TOUCH_RZTHRESH    0x302118UL
#define REG_TOUCH_RAW_XY      0x30211CUL
#define REG_CTOUCH_TOUCH1_XY  0x30211CUL
#define REG_TOUCH_RZ          0x302120UL
#define REG_CTOUCH_TOUCH4_Y   0x302120UL
#define REG_TOUCH_SCREEN_XY   0x302124UL
#define REG_CTOUCH_TOUCH0_XY  0x302124UL
#define REG_TOUCH_TAG_XY      0x302128UL
#define REG_TOUCH_TAG         0x30212CUL
#define REG_TOUCH_TAG1_XY     0x302130UL
#define REG_TOUCH_TAG1        0x302134UL
#define REG_TOUCH_TAG2_XY     0x302138UL
#define REG_TOUCH_TAG2        0x30213CUL
#define REG_TOUCH_TAG3_XY     0x302140UL
#define REG_TOUCH_TAG3        0x302144UL
#define REG_TOUCH_TAG4_XY     0x302148UL
#define REG_TOUCH_TAG4        0x30214CUL
#define REG_TOUCH_TRANSFORM_A 0x302150UL
#define REG_TOUCH_TRANSFORM_B 0x302154UL
#define REG_TOUCH_TRANSFORM_C 0x302158UL
#define REG_TOUCH_TRANSFORM_D 0x30215CUL
#define REG_TOUCH_TRANSFORM_E 0x302160UL
#define REG_TOUCH_TRANSFORM_F 0x302164UL
#define REG_TOUCH_CONFIG      0x302168UL
#define REG_CTOUCH_TOUCH4_X   0x30216CUL
#define REG_BIST_EN           0x302174UL
#define REG_TRIM              0x302180UL
#define REG_ANA_COMP          0x302184UL
#define REG_SPI_WIDTH         0x302188UL
#define REG_TOUCH_DIRECT_XY   0x30218CUL
#define REG_CTOUCH_TOUCH2_XY  0x30218CUL
#define REG_TOUCH_DIRECT_Z1Z2 0x302190UL
#define REG_CTOUCH_TOUCH3_XY  0x302190UL
#define REG_DATESTAMP         0x302564UL
#define REG_CMDB_SPACE        0x302574UL
#define REG_CMDB_WRITE        0x302578UL

// Chip ID
#define CHIP_ID               0x0C0000UL

// Graphics Engine Commands
// Refer to the FT812 Programmers Guide
#define CMDBUF_SIZE          4096UL
#define CMD_APPEND           0xffffff1eUL
#define CMD_BGCOLOR          0xffffff09UL
#define CMD_BUTTON           0xffffff0dUL
#define CMD_CALIBRATE        0xffffff15UL
#define CMD_CLOCK            0xffffff14UL
#define CMD_COLDSTART        0xffffff32UL
#define CMD_CSKETCH          0xffffff35UL
#define CMD_DIAL             0xffffff2dUL
#define CMD_DLSTART          0xffffff00UL
#define CMD_FGCOLOR          0xffffff0aUL
#define CMD_GAUGE            0xffffff13UL
#define CMD_GETMATRIX        0xffffff33UL
#define CMD_GETPTR           0xffffff23UL
#define CMD_GETPROPS         0xffffff25UL
#define CMD_GRADCOLOR        0xffffff34UL
#define CMD_GRADIENT         0xffffff0bUL
#define CMD_INFLATE          0xffffff22UL
#define CMD_INTERRUPT        0xffffff02UL
#define CMD_KEYS             0xffffff0eUL
#define CMD_LOADIDENTITY     0xffffff26UL
#define CMD_LOADIMAGE        0xffffff24UL
#define CMD_LOGO             0xffffff31UL
#define CMD_MEDIAFIFO        0xffffff39UL
#define CMD_MEMCPY           0xffffff1dUL
#define CMD_MEMCRC           0xffffff18UL
#define CMD_MEMSET           0xffffff1bUL
#define CMD_MEMWRITE         0xffffff1aUL
#define CMD_MEMZERO          0xffffff1cUL
#define CMD_NUMBER           0xffffff2eUL
#define CMD_PLAYVIDEO        0xffffff3aUL
#define CMD_PROGRESS         0xffffff0fUL
#define CMD_REGREAD          0xffffff19UL
#define CMD_ROTATE           0xffffff29UL
#define CMD_ROMFONT          0xffffff3fUL
#define CMD_SCALE            0xffffff28UL
#define CMD_SCREENSAVER      0xffffff2fUL
#define CMD_SCROLLBAR        0xffffff11UL
#define CMD_SETSCRATCH       0xffffff3cUL
#define CMD_SETBASE          0xffffff38UL
#define CMD_SETBITMAP        0xffffff43UL
#define CMD_SETFONT          0xffffff2bUL
#define CMD_SETFONT2         0xffffff3bUL
#define CMD_SETMATRIX        0xffffff2AUL
#define CMD_SETROTATE        0xffffff36UL
#define CMD_SNAPSHOT         0xffffff1fUL
#define CMD_SNAPSHOT2        0xffffff37UL
#define CMD_SKETCH           0xffffff30UL
#define CMD_SLIDER           0xffffff10UL
#define CMD_SNAPSHOT         0xffffff1fUL
#define CMD_SPINNER          0xffffff16UL
#define CMD_STOP             0xffffff17UL
#define CMD_SWAP             0xffffff01UL
#define CMD_TEXT             0xffffff0CUL
#define CMD_TOGGLE           0xffffff12UL
#define CMD_TRACK            0xffffff2CUL
#define CMD_TRANSLATE        0xffffff27UL
#define CMD_VIDEOSTART       0xffffff40UL
#define CMD_VIDEOFRAME       0xffffff41UL

// Display list commands to be embedded in Graphics Processor
#define DL_ALPHA_FUNC       0x09000000UL // requires OR'd arguments
#define DL_BITMAP_HANDLE    0x05000000UL // requires OR'd arguments
#define DL_BITMAP_LAYOUT    0x07000000UL // requires OR'd arguments
#define DL_BITMAP_LAYOUT_H  0x28000000UL // requires OR'd arguments
#define DL_BITMAP_SIZE      0x08000000UL // requires OR'd arguments
#define DL_BITMAP_SIZE_H    0x29000000UL // requires OR'd arguments
#define DL_BITMAP_SOURCE    0x01000000UL // requires OR'd arguments
#define DL_BITMAP_TFORM_A   0x15000000UL // requires OR'd arguments
#define DL_BITMAP_TFORM_B   0x16000000UL // requires OR'd arguments
#define DL_BITMAP_TFORM_C   0x17000000UL // requires OR'd arguments
#define DL_BITMAP_TFORM_D   0x18000000UL // requires OR'd arguments
#define DL_BITMAP_TFORM_E   0x19000000UL // requires OR'd arguments
#define DL_BITMAP_TFORM_F   0x1A000000UL // requires OR'd arguments
#define DL_BLEND_FUNC       0x0B000000UL // requires OR'd arguments
#define DL_BEGIN            0x1F000000UL // requires OR'd arguments
#define DL_CALL             0x1D000000UL // requires OR'd arguments
#define DL_CLEAR            0x26000000UL // requires OR'd arguments
#define DL_CELL             0x06000000UL // requires OR'd arguments
#define DL_CLEAR_RGB        0x02000000UL // requires OR'd arguments
#define DL_CLEAR_STENCIL    0x11000000UL // requires OR'd arguments
#define DL_CLEAR_TAG        0x12000000UL // requires OR'd arguments
#define DL_CLEAR_COLOR_A    0x0F000000UL // requires OR'd arguments
#define DL_COLOR_A          0x10000000UL // requires OR'd arguments
#define DL_COLOR_MASK       0x20000000UL // requires OR'd arguments
#define DL_COLOR_RGB        0x04000000UL // requires OR'd arguments
#define DL_DISPLAY          0x00000000UL
#define DL_END              0x21000000UL
#define DL_JUMP             0x1E000000UL // requires OR'd arguments
#define DL_LINE_WIDTH       0x0E000000UL // requires OR'd arguments
#define DL_NOP              0x2D000000UL
#define DL_PALETTE_SOURCE   0x2A000000UL // requires OR'd arguments
#define DL_MACRO            0x25000000UL // requires OR'd arguments
#define DL_POINT_SIZE       0x0D000000UL // requires OR'd arguments
#define DL_RESTORE_CONTEXT  0x23000000UL
#define DL_RETURN           0x24000000UL
#define DL_SAVE_CONTEXT     0x22000000UL
#define DL_SCISSOR_SIZE     0x1C000000UL // requires OR'd arguments
#define DL_SCISSOR_XY       0x1B000000UL // requires OR'd arguments
#define DL_STENCIL_FUNC     0x0A000000UL // requires OR'd arguments
#define DL_STENCIL_MASK     0x13000000UL // requires OR'd arguments
#define DL_STENCIL_OP       0x0C000000UL // requires OR'd arguments
#define DL_TAG              0x03000000UL // requires OR'd arguments
#define DL_TAG_MASK         0x14000000UL // requires OR'd arguments
#define DL_VERTEX2F         0x40000000UL // requires OR'd arguments
#define DL_VERTEX2II        0x80000000UL // requires OR'd arguments
#define DL_VERTEX_FORMAT    0x27000000UL // requires OR'd arguments
#define DL_VERTEX_TRANS_X   0x2B000000UL // requires OR'd arguments
#define DL_VERTEX_TRANS_Y   0x2C000000UL // requires OR'd arguments

// Command and register value options
#define ALWAYS               7UL
#define ARGB1555             0UL
#define ARGB2                5UL
#define ARGB4                6UL
#define BARGRAPH             11UL
#define CLR_COL              0x4
#define CLR_STN              0x2
#define CLR_TAG              0x1
#define DECR                 4UL
#define DECR_WRAP            7UL
#define DLSWAP_DONE          0UL
#define DLSWAP_FRAME         2UL
#define DLSWAP_LINE          1UL
#define DST_ALPHA            3UL
#define EDGE_STRIP_A         7UL
#define EDGE_STRIP_B         8UL
#define EDGE_STRIP_L         6UL
#define EDGE_STRIP_R         5UL
#define EQUAL                5UL
#define GEQUAL               4UL
#define GREATER              3UL
#define INCR                 3UL
#define INCR_WRAP            6UL
#define INT_CMDEMPTY         32UL
#define INT_CMDFLAG          64UL
#define INT_CONVCOMPLETE     128UL
#define INT_PLAYBACK         16UL
#define INT_SOUND            8UL
#define INT_SWAP             1UL
#define INT_TAG              4UL
#define INT_TOUCH            2UL
#define INVERT               5UL
#define KEEP                 1UL
#define L1                   1UL
#define L2                   17UL
#define L4                   2UL
#define L8                   3UL
#define LEQUAL               2UL
#define LESS                 1UL
#define LINEAR_SAMPLES       0UL
#define LINES                3UL
#define LINE_STRIP           4UL
#define NEAREST              0UL
#define NEVER                0UL
#define NOTEQUAL             6UL
#define ONE                  1UL
#define ONE_MINUS_DST_ALPHA  5UL
#define ONE_MINUS_SRC_ALPHA  4UL
#define OPT_CENTER           1536UL    // 0x6000
#define OPT_CENTERX          512UL     // 0x0200
#define OPT_CENTERY          1024UL    // 0x0400
#define OPT_FLAT             256UL     // 0x0100
#define OPT_MONO             1UL
#define OPT_NOBACK           4096UL    // 0x1000
#define OPT_NODL             2UL
#define OPT_NOHANDS          49152UL   // 0xC168
#define OPT_NOHM             16384UL   // 0x4000
#define OPT_NOPOINTER        16384UL   // 0x4000
#define OPT_NOSECS           32768UL   // 0x8000
#define OPT_NOTICKS          8192UL    // 0x2000
#define OPT_RIGHTX           2048UL    // 0x0800
#define OPT_SIGNED           256UL     // 0x0100
#define PALETTED             8UL
#define PALETTED565          14UL
#define PALETTED4444         15UL
#define PALETTED8            16UL
#define PLAYCOLOR            0x00a0a080
#define FTPOINTS             2UL       // "POINTS" is a reserved word
#define RECTS                9UL
#define FTREPEAT             1UL       // "REPEAT" is a reverved word
#define REPLACE              2UL
#define RGB332               4UL
#define RGB565               7UL
#define SRC_ALPHA            2UL
#define TEXT8X8              9UL
#define TEXTVGA              10UL
#define TOUCHMODE_CONTINUOUS 3UL
#define TOUCHMODE_FRAME      2UL
#define TOUCHMODE_OFF        0UL
#define TOUCHMODE_ONESHOT    1UL
#define ULAW_SAMPLES         1UL
#define ZERO                 0UL

// Useful Macros
#define RGB(r, g, b)         ((((r) << 16) | (g) << 8) | (b))
#define SQ(v)                ((v) * (v))
#define MIN(x,y)             ((x) > (y) ? (y) : (x))
#define MAX(x,y)             ((x) > (y) ? (x) : (y))
#define NOTE(n, sharp)       (((n) - 'C') + ((sharp) * 128))
#define F16(s)               (((s) * 65536))
#define INVALID_TOUCH_XY     0x8000
#define ABS(x)               ((x) > (0) ? (x) : (-x))

#endif  //FT812_h
/** EOF FT812.h *******************************************************/
