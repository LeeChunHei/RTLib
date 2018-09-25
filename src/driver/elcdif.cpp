/*
 * elcdif.cpp
 *
 *  Created on: Sep 14, 2018
 *      Author: LeeChunHei
 */

#include "driver/elcdif.h"
#include "system/clock.h"
#include "system/system.h"

namespace Driver {

using namespace System;

const clock_ip_name_t s_elcdifApbClocks[] = LCDIF_CLOCKS;
const clock_ip_name_t s_elcdifPixClocks[] = LCDIF_PERIPH_CLOCKS;

const uint32_t pixel_format_reg[5][2] = {
/* kELCDIF_PixelFormatRAW8 */
{/* Register CTRL. */
LCDIF_CTRL_WORD_LENGTH(1U),
/* Register CTRL1. */
LCDIF_CTRL1_BYTE_PACKING_FORMAT(0x0FU) },
/* kELCDIF_PixelFormatRGB565 */
{/* Register CTRL. */
LCDIF_CTRL_WORD_LENGTH(0U),
/* Register CTRL1. */
LCDIF_CTRL1_BYTE_PACKING_FORMAT(0x0FU) },
/* kELCDIF_PixelFormatRGB666 */
{/* Register CTRL. */
LCDIF_CTRL_WORD_LENGTH(3U) | LCDIF_CTRL_DATA_FORMAT_24_BIT(1U),
/* Register CTRL1. */
LCDIF_CTRL1_BYTE_PACKING_FORMAT(0x07U) },
/* kELCDIF_PixelFormatXRGB8888 */
{/* Register CTRL. 24-bit. */
LCDIF_CTRL_WORD_LENGTH(3U),
/* Register CTRL1. */
LCDIF_CTRL1_BYTE_PACKING_FORMAT(0x07U) },
/* kELCDIF_PixelFormatRGB888 */
{/* Register CTRL. 24-bit. */
LCDIF_CTRL_WORD_LENGTH(3U),
/* Register CTRL1. */
LCDIF_CTRL1_BYTE_PACKING_FORMAT(0x0FU) } };

eLCDIF::eLCDIF(Config& config) {

	uint8_t module = 0;

	uint32_t desire_pclk = (config.width + config.hsync_pulse_width + config.horizontal_front_porch + config.horizontal_back_porch) * (config.height + config.vsync_pulse_width + config.vertical_front_porch + config.vertical_back_porch) * config.frame_rate;
	desire_pclk = video_pll_clock / desire_pclk;
	if (desire_pclk < 9) {	//maximum value for pre div is 8
		CLOCK_SetDiv(kCLOCK_LcdifPreDiv, desire_pclk);
		CLOCK_SetDiv(kCLOCK_LcdifDiv, 1);
	} else if (desire_pclk / 8 < 9) {	//maximum value for div is 8
		CLOCK_SetDiv(kCLOCK_LcdifPreDiv, 8);
		CLOCK_SetDiv(kCLOCK_LcdifDiv, desire_pclk / 8);
	} else {
		return;
	}

	/* Enable the clock. */
	CLOCK_EnableClock(s_elcdifApbClocks[module]);
	CLOCK_EnableClock(s_elcdifPixClocks[module]);

	elcdif_base = (LCDIF_Type*) (0x402B8000u);

	/* Reset. */
	Reset();

	elcdif_base->CTRL = pixel_format_reg[(uint32_t) config.pixel_format][0] | (uint32_t)(config.data_bus) | LCDIF_CTRL_DOTCLK_MODE_MASK | /* RGB mode. */
	LCDIF_CTRL_BYPASS_COUNT_MASK | /* Keep RUN bit set. */
	LCDIF_CTRL_MASTER_MASK;

	elcdif_base->CTRL1 = pixel_format_reg[(uint32_t) config.pixel_format][1];

	elcdif_base->TRANSFER_COUNT = ((uint32_t) config.height << LCDIF_TRANSFER_COUNT_V_COUNT_SHIFT) | ((uint32_t) config.width << LCDIF_TRANSFER_COUNT_H_COUNT_SHIFT);

	elcdif_base->VDCTRL0 = LCDIF_VDCTRL0_ENABLE_PRESENT_MASK | /* Data enable signal. */
	LCDIF_VDCTRL0_VSYNC_PERIOD_UNIT_MASK | /* VSYNC period in the unit of display clock. */
	LCDIF_VDCTRL0_VSYNC_PULSE_WIDTH_UNIT_MASK | /* VSYNC pulse width in the unit of display clock. */
	(uint32_t)(config.vsync_active_high << LCDIF_VDCTRL0_VSYNC_POL_SHIFT | config.hsync_active_high << LCDIF_VDCTRL0_HSYNC_POL_SHIFT | config.data_enable_active_high << LCDIF_VDCTRL0_ENABLE_POL_SHIFT | config.drive_data_on_rising_clk_edge << LCDIF_VDCTRL0_DOTCLK_POL_SHIFT) | (uint32_t) config.vsync_pulse_width;

	elcdif_base->VDCTRL1 = config.vsync_pulse_width + config.height + config.vertical_front_porch + config.vertical_back_porch;
	elcdif_base->VDCTRL2 = ((uint32_t) config.hsync_pulse_width << LCDIF_VDCTRL2_HSYNC_PULSE_WIDTH_SHIFT) | ((uint32_t)(config.horizontal_front_porch + config.horizontal_back_porch + config.width + config.hsync_pulse_width)) << LCDIF_VDCTRL2_HSYNC_PERIOD_SHIFT;

	elcdif_base->VDCTRL3 = (((uint32_t) config.horizontal_back_porch + config.hsync_pulse_width) << LCDIF_VDCTRL3_HORIZONTAL_WAIT_CNT_SHIFT) | (((uint32_t) config.vertical_back_porch + config.vsync_pulse_width) << LCDIF_VDCTRL3_VERTICAL_WAIT_CNT_SHIFT);

	elcdif_base->VDCTRL4 = LCDIF_VDCTRL4_SYNC_SIGNALS_ON_MASK | ((uint32_t) config.width << LCDIF_VDCTRL4_DOTCLK_H_VALID_DATA_CNT_SHIFT);

	elcdif_base->CUR_BUF = config.buffer_addr;
	elcdif_base->NEXT_BUF = config.buffer_addr;
}

inline void eLCDIF::Start() {
	elcdif_base->CTRL_SET = LCDIF_CTRL_RUN_MASK | LCDIF_CTRL_DOTCLK_MODE_MASK;
}

void eLCDIF::Reset() {
	/* Disable the clock gate. */
	elcdif_base->CTRL_CLR = LCDIF_CTRL_CLKGATE_MASK;
	/* Confirm the clock gate is disabled. */
	while (elcdif_base->CTRL & LCDIF_CTRL_CLKGATE_MASK) {
	}

	/* Reset the block. */
	elcdif_base->CTRL_SET = LCDIF_CTRL_SFTRST_MASK;
	/* Confirm the reset bit is set. */
	while (!(elcdif_base->CTRL & LCDIF_CTRL_SFTRST_MASK)) {
	}

	volatile uint32_t i = 0x100;
	/* Delay for the reset. */
	while (i--) {
	}

	/* Bring the module out of reset. */
	elcdif_base->CTRL_CLR = LCDIF_CTRL_SFTRST_MASK;
	/* Disable the clock gate. */
	elcdif_base->CTRL_CLR = LCDIF_CTRL_CLKGATE_MASK;
}

}
