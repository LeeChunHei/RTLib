/*
 * elcdif.h
 *
 *  Created on: Sep 14, 2018
 *      Author: LeeChunHei
 */

#ifndef INC_DRIVER_ELCDIF_H_
#define INC_DRIVER_ELCDIF_H_

#include "system/cmsis/access_layer/access_layer.h"

namespace Driver {

class eLCDIF {
public:
	struct Config {
		enum struct PixelFormat {
			kRAW8 = 0, /*!< RAW 8 bit, four data use 32 bits. */
			kRGB565 = 1, /*!< RGB565, two pixel use 32 bits. */
			kRGB666 = 2, /*!< RGB666 unpacked, one pixel uses 32 bits, high byte unused, upper 2 bits of other bytes unused. */
			kXRGB8888 = 3, /*!< XRGB8888 unpacked, one pixel uses 32 bits, high byte unused. */
			kRGB888 = 4, /*!< RGB888 packed, one pixel uses 24 bits. */
		};
		enum struct DataBus {
			k8Bit = LCDIF_CTRL_LCD_DATABUS_WIDTH(1), /*!< 8-bit data bus. */
			k16Bit = LCDIF_CTRL_LCD_DATABUS_WIDTH(0), /*!< 16-bit data bus, support RGB565. */
			k18Bit = LCDIF_CTRL_LCD_DATABUS_WIDTH(2), /*!< 18-bit data bus, support RGB666. */
			k24Bit = LCDIF_CTRL_LCD_DATABUS_WIDTH(3), /*!< 24-bit data bus, support RGB888. */
		};
		uint16_t width = 480; /*!< Display panel width, pixels per line. */
		uint16_t height = 272; /*!< Display panel height, how many lines per panel. */
		uint8_t frame_rate = 60;
		uint8_t hsync_pulse_width = 41; /*!< HSYNC pulse width. */
		uint8_t horizontal_front_porch = 4; /*!< Horizontal front porch. */
		uint8_t horizontal_back_porch = 8; /*!< Horizontal back porch. */
		uint8_t vsync_pulse_width = 10; /*!< VSYNC pulse width. */
		uint8_t vertical_front_porch = 4; /*!< Vrtical front porch. */
		uint8_t vertical_back_porch = 2; /*!< Vertical back porch. */
		bool vsync_active_high = false;
		bool hsync_active_high = false;
		bool data_enable_active_high = false;
		bool drive_data_on_rising_clk_edge = false;
		uint32_t buffer_addr = 0; /*!< Frame buffer address. */
		PixelFormat pixel_format = PixelFormat::kRGB888; /*!< Pixel format. */
		DataBus data_bus = DataBus::k24Bit; /*!< LCD data bus. */
	};
	eLCDIF(Config& config);
	inline void Start();
private:
	void Reset();
	LCDIF_Type* elcdif_base;
};

}

#endif /* INC_DRIVER_ELCDIF_H_ */
