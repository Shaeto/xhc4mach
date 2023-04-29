#pragma once

#define WHBxx_MAGIC 0xFDFE 
#define DEV_UNKNOW 0

#define DEV_WHB03  sizeof( struct whb03_out_data )
#define DEV_WHB04  sizeof( struct whb04_out_data )

#define WHBxx_VID 0x10CE
#define WHB03_PID 0xEB6E
#define WHB04_PID 0xEB70
#define WHB04B6_PID 0xEB93

#pragma pack( push, 1 )

#define WHB03_STATE_FLASH_YESNO 0x10
#define WHB03_STATE_AXIS_A 0x40
#define WHB03_STATE_UNIT_INCH 0x80

struct whb03_out_data
{
	/* header of our packet */
	uint16_t	magic;
	/* day of the month .. funny i know*/
	uint8_t		day;

	struct
	{
		uint16_t	p_int;
		uint8_t	p_frac;
	} pos[6];
	/* speed */
	uint16_t	feedrate_ovr;
	uint16_t	sspeed_ovr;
	uint16_t	feedrate;
	uint16_t	sspeed;

	uint8_t    step_mul;
	uint8_t    state;

};

#define WHB04_STATE_UNIT_INCH 0x80

struct whb04_out_data
{
	/* header of our packet */
	uint16_t	magic;
	/* day of the month .. funny i know*/
	uint8_t		day;
	struct
	{
		uint16_t	p_int;
		uint16_t	p_frac;
	} pos[6];

	/* speed */
	uint16_t	feedrate_ovr;
	uint16_t	sspeed_ovr;
	uint16_t	feedrate;
	uint16_t	sspeed;

	uint8_t    step_mul;
	uint8_t    state;

};

#if 0
struct whb03_out_data
{
	/* header of our packet */
	uint16_t	magic;
	/* day of the month .. funny i know*/
	uint8_t		day;
	/* work pos */
	uint16_t	x_wc_int;
	uint8_t		x_wc_frac;
	uint16_t	y_wc_int;
	uint8_t		y_wc_frac;
	uint16_t	z_wc_int;
	uint8_t		z_wc_frac;
	/* machine pos */
	uint16_t	x_mc_int;
	uint8_t		x_mc_frac;
	uint16_t	y_mc_int;
	uint8_t		y_mc_frac;
	uint16_t	z_mc_int;
	uint8_t		z_mc_frac;

	/* speed */
	uint16_t	feedrate_ovr;
	uint16_t	sspeed_ovr;
	uint16_t	feedrate;
	uint16_t	sspeed;

	uint8_t    step_mul;
	uint8_t    state;

};

struct whb04_out_data
{
	/* header of our packet */
	uint16_t	magic;
	/* day of the month .. funny i know*/
	uint8_t		day;
	/* work pos */
	uint16_t	x_wc_int;
	uint16_t	x_wc_frac;
	uint16_t	y_wc_int;
	uint16_t	y_wc_frac;
	uint16_t	z_wc_int;
	uint16_t	z_wc_frac;
	/* machine pos */
	uint16_t	x_mc_int;
	uint16_t	x_mc_frac;
	uint16_t	y_mc_int;
	uint16_t	y_mc_frac;
	uint16_t	z_mc_int;
	uint16_t	z_mc_frac;

	/* speed */
	uint16_t	feedrate_ovr;
	uint16_t	sspeed_ovr;
	uint16_t	feedrate;
	uint16_t	sspeed;

	uint8_t    step_mul;
	uint8_t    state;

};
#endif

struct whb0x_in_data
{
	uint8_t		id;
	uint8_t		btn_1;
	uint8_t		btn_2;
	uint8_t		axis;
	int8_t		jog_counts;
	uint8_t		xor_day;
};

// it is snatched from there https://github.com/machinekit/machinekit/blob/master/src/hal/user_comps/xhc-whb04b-6/README.md
struct whb04b6_in_data
{
	uint8_t		id;
	uint8_t		random;
	uint8_t		btn_1;
	uint8_t		btn_2;
	uint8_t		feed_mode;
	uint8_t		axis;
	int8_t		jog_counts;
	uint8_t		checksum;
};

struct whb04b6_out_data
{
	/* header of our packet */
	uint16_t	magic;
	/* day of the month .. funny i know*/
	uint8_t		day;

	struct {
		uint8_t step_mode : 2;
		uint8_t unknown_mode : 4;
		uint8_t reset_flag : 1;
		uint8_t mc_flag : 1;
	} display;

	struct
	{
		uint16_t	p_int;
		uint16_t	p_frac;
	} pos[3];

	/* speed */
	uint16_t	feedrate;
	uint16_t	sspeed;

	// do we need it ?
	uint16_t	dummy1;
	uint16_t	dummy2;
};

#pragma pack( pop )

//extern __IO uint8_t g_hw_type;
