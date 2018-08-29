/*
 * MIMXRT1052.cpp
 *
 *  Created on: Aug 3, 2018
 *      Author: LeeChunHei
 */

#include "system/pinout/MIMXRT1052.h"
#include "system/clock.h"
#include <assert.h>
#include <bitset>

namespace System {
namespace Pinout {

std::bitset<124> pin_active;	//record which pin is using

bool GetADCPinConfig(Config& config) {
	if (pin_active[(uint8_t) config.pin]) {
		return false;
	}
	config.mux_mode = Config::MuxMode::kAlt5;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kDisable;
	config.force_input = false;
	return true;
}

bool GetPWMPinConfig(Config& config, PWM_Type* pwm_base, uint8_t sub_module) {
	if (pin_active[(uint8_t) config.pin]) {
		return false;
	}
	Name pin_name = config.pin;
	if (pin_name <= Name::kGPIO_EMC_05) {
		if (pwm_base != PWM4 || sub_module != ((uint8_t) pin_name) >> 1) {
			return false;
		}
		config.mux_mode = Config::MuxMode::kAlt1;
	} else if (pin_name <= Name::kGPIO_EMC_11) {
		if (pwm_base != PWM2 || sub_module != ((uint8_t) pin_name - (uint8_t) Name::kGPIO_EMC_06) >> 1) {
			return false;
		}
		config.mux_mode = Config::MuxMode::kAlt1;
	} else if (pin_name <= Name::kGPIO_EMC_13) {
		if (pwm_base != PWM2 || sub_module != 3u) {
			return false;
		}
		config.mux_mode = Config::MuxMode::kAlt4;
	} else if (pin_name <= Name::kGPIO_EMC_16) {
		return false;
	} else if (pin_name <= Name::kGPIO_EMC_18) {
		if (pwm_base != PWM4 || sub_module != 3u) {
			return false;
		}
		config.mux_mode = Config::MuxMode::kAlt1;
	} else if (pin_name <= Name::kGPIO_EMC_20) {
		if (pwm_base != PWM2 || sub_module != 3u) {
			return false;
		}
		config.mux_mode = Config::MuxMode::kAlt1;
	} else if (pin_name <= Name::kGPIO_EMC_22) {
		if (pwm_base != PWM3 || sub_module != 3u) {
			return false;
		}
		config.mux_mode = Config::MuxMode::kAlt1;
	} else if (pin_name <= Name::kGPIO_EMC_28) {
		if (pwm_base != PWM1 || sub_module != ((uint8_t) pin_name - (uint8_t) Name::kGPIO_EMC_23) >> 1) {
			return false;
		}
		config.mux_mode = Config::MuxMode::kAlt1;
	} else if (pin_name <= Name::kGPIO_EMC_34) {
		if (pwm_base != PWM3 || sub_module != ((uint8_t) pin_name - (uint8_t) Name::kGPIO_EMC_29) >> 1) {
			return false;
		}
		config.mux_mode = Config::MuxMode::kAlt1;
	} else if (pin_name <= Name::kGPIO_EMC_37) {
		return false;
	} else if (pin_name <= Name::kGPIO_EMC_39) {
		if (pwm_base != PWM1 || sub_module != 3u) {
			return false;
		}
		config.mux_mode = Config::MuxMode::kAlt1;
	} else if (pin_name <= Name::kGPIO_EMC_41) {
		return false;
	} else if (pin_name <= Name::kGPIO_AD_B0_01) {
		if (pwm_base != PWM2 || sub_module != 3u) {
			return false;
		}
		config.mux_mode = Config::MuxMode::kAlt0;
	} else if (pin_name <= Name::kGPIO_AD_B0_03) {
		if (pwm_base != PWM1 || sub_module != 1u) {
			return false;
		}
		config.mux_mode = Config::MuxMode::kAlt4;
	} else if (pin_name <= Name::kGPIO_AD_B0_08) {
		return false;
	} else if (pin_name <= Name::kGPIO_AD_B0_09) {
		if (pwm_base != PWM2 || sub_module != 3u) {
			return false;
		}
		config.mux_mode = Config::MuxMode::kAlt1;
	} else if (pin_name <= Name::kGPIO_AD_B0_11) {
		if (pwm_base != PWM1 || sub_module != 3u) {
			return false;
		}
		config.mux_mode = Config::MuxMode::kAlt1;
	} else if (pin_name <= Name::kGPIO_AD_B0_13) {
		if (pwm_base != PWM1 || sub_module != (uint8_t) pin_name - (uint8_t) Name::kGPIO_AD_B0_10) {
			return false;
		}
		config.mux_mode = Config::MuxMode::kAlt4;
	} else if (pin_name <= Name::kGPIO_AD_B1_07) {
		return false;
	} else if (pin_name <= Name::kGPIO_AD_B1_09) {
		if (pwm_base != PWM4 || sub_module != (uint8_t) pin_name - (uint8_t) Name::kGPIO_AD_B1_08) {
			return false;
		}
		config.mux_mode = Config::MuxMode::kAlt1;
	} else if (pin_name <= Name::kGPIO_B0_05) {
		return false;
	} else if (pin_name <= Name::kGPIO_B0_11) {
		if (pwm_base != PWM2 || sub_module != ((uint8_t) pin_name - (uint8_t) Name::kGPIO_B0_06) >> 1) {
			return false;
		}
		config.mux_mode = Config::MuxMode::kAlt2;
	} else if (pin_name <= Name::kGPIO_B0_15) {
		return false;
	} else if (pin_name <= Name::kGPIO_B1_01) {
		if (pwm_base != PWM1 || sub_module != 3u) {
			return false;
		}
		config.mux_mode = Config::MuxMode::kAlt6;
	} else if (pin_name <= Name::kGPIO_B1_03) {
		if (pwm_base != PWM2 || sub_module != 3u) {
			return false;
		}
		config.mux_mode = Config::MuxMode::kAlt6;
	} else if (pin_name <= Name::kGPIO_B1_13) {
		return false;
	} else if (pin_name <= Name::kGPIO_B1_15) {
		if (pwm_base != PWM4 || sub_module != (uint8_t) pin_name - (uint8_t) Name::kGPIO_B1_12) {
			return false;
		}
		config.mux_mode = Config::MuxMode::kAlt1;
	} else if (pin_name <= Name::kGPIO_SD_B0_05) {
		if (pwm_base != PWM1 || sub_module != ((uint8_t) pin_name - (uint8_t) Name::kGPIO_SD_B0_00) >> 1) {
			return false;
		}
		config.mux_mode = Config::MuxMode::kAlt1;
	} else if (pin_name <= Name::kGPIO_SD_B1_01) {
		if (pwm_base != PWM1 || sub_module != 3u) {
			return false;
		}
		config.mux_mode = Config::MuxMode::kAlt2;
	} else if (pin_name <= Name::kGPIO_SD_B1_03) {
		if (pwm_base != PWM2 || sub_module != 3u) {
			return false;
		}
		config.mux_mode = Config::MuxMode::kAlt2;
	} else {
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetUartTXPinConfig(Config& config, LPUART_Type* uart_base) {
	if (pin_active[(uint8_t) config.pin]) {
		return false;
	}
	Name pin = config.pin;
	if (uart_base == LPUART1) {
		if (pin == Name::kGPIO_AD_B0_12) {
			config.mux_mode = Config::MuxMode::kAlt2;
		} else {
			return false;
		}
	} else if (uart_base == LPUART2) {
		if (pin == Name::kGPIO_AD_B1_02 || pin == Name::kGPIO_SD_B1_11) {
			config.mux_mode = Config::MuxMode::kAlt2;
		} else {
			return false;
		}
	} else if (uart_base == LPUART3) {
		if (pin == Name::kGPIO_EMC_13 || pin == Name::kGPIO_AD_B1_06) {
			config.mux_mode = Config::MuxMode::kAlt2;
		} else if (pin == Name::kGPIO_B0_08) {
			config.mux_mode = Config::MuxMode::kAlt3;
		} else {
			return false;
		}
	} else if (uart_base == LPUART4) {
		if (pin == Name::kGPIO_EMC_19 || pin == Name::kGPIO_B1_00) {
			config.mux_mode = Config::MuxMode::kAlt2;
		} else if (pin == Name::kGPIO_SD_B1_00) {
			config.mux_mode = Config::MuxMode::kAlt4;
		} else {
			return false;
		}
	} else if (uart_base == LPUART5) {
		if (pin == Name::kGPIO_EMC_23) {
			config.mux_mode = Config::MuxMode::kAlt2;
		} else if (pin == Name::kGPIO_B1_12) {
			config.mux_mode = Config::MuxMode::kAlt0;
		} else {
			return false;
		}
	} else if (uart_base == LPUART6) {
		if (pin == Name::kGPIO_EMC_25 || pin == Name::kGPIO_AD_B0_02) {
			config.mux_mode = Config::MuxMode::kAlt2;
		} else {
			return false;
		}
	} else if (uart_base == LPUART7) {
		if (pin == Name::kGPIO_EMC_31 || pin == Name::kGPIO_SD_B1_08) {
			config.mux_mode = Config::MuxMode::kAlt2;
		} else {
			return false;
		}
	} else if (uart_base == LPUART8) {
		if (pin == Name::kGPIO_EMC_38 || pin == Name::kGPIO_AD_B1_10 || pin == Name::kGPIO_SD_B0_04) {
			config.mux_mode = Config::MuxMode::kAlt2;
		} else {
			return false;
		}
	} else {
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetUartRXPinConfig(Config& config, LPUART_Type* uart_base) {
	if (pin_active[(uint8_t) config.pin]) {
		return false;
	}
	Name pin = config.pin;
	if (uart_base == LPUART1) {
		if (pin == Name::kGPIO_AD_B0_13) {
			config.mux_mode = Config::MuxMode::kAlt2;
		} else {
			return false;
		}
	} else if (uart_base == LPUART2) {
		if (pin == Name::kGPIO_AD_B1_03 || pin == Name::kGPIO_SD_B1_10) {
			config.mux_mode = Config::MuxMode::kAlt2;
		} else {
			return false;
		}
	} else if (uart_base == LPUART3) {
		if (pin == Name::kGPIO_EMC_14 || pin == Name::kGPIO_AD_B1_07) {
			config.mux_mode = Config::MuxMode::kAlt2;
		} else if (pin == Name::kGPIO_B0_09) {
			config.mux_mode = Config::MuxMode::kAlt3;
		} else {
			return false;
		}
	} else if (uart_base == LPUART4) {
		if (pin == Name::kGPIO_EMC_20 || pin == Name::kGPIO_B1_01) {
			config.mux_mode = Config::MuxMode::kAlt2;
		} else if (pin == Name::kGPIO_SD_B1_01) {
			config.mux_mode = Config::MuxMode::kAlt4;
		} else {
			return false;
		}
	} else if (uart_base == LPUART5) {
		if (pin == Name::kGPIO_EMC_24) {
			config.mux_mode = Config::MuxMode::kAlt2;
		} else if (pin == Name::kGPIO_B1_13) {
			config.mux_mode = Config::MuxMode::kAlt1;
		} else {
			return false;
		}
	} else if (uart_base == LPUART6) {
		if (pin == Name::kGPIO_EMC_26 || pin == Name::kGPIO_AD_B0_03) {
			config.mux_mode = Config::MuxMode::kAlt2;
		} else {
			return false;
		}
	} else if (uart_base == LPUART7) {
		if (pin == Name::kGPIO_EMC_32 || pin == Name::kGPIO_SD_B1_09) {
			config.mux_mode = Config::MuxMode::kAlt2;
		} else {
			return false;
		}
	} else if (uart_base == LPUART8) {
		if (pin == Name::kGPIO_EMC_39 || pin == Name::kGPIO_AD_B1_11 || pin == Name::kGPIO_SD_B0_05) {
			config.mux_mode = Config::MuxMode::kAlt2;
		} else {
			return false;
		}
	} else {
		return false;
	}
	config.force_input = false;
	return true;
}

void InitPin(Config& config) {
	if (pin_active[(uint8_t) config.pin]) {
		assert(false);
	} else {
		pin_active[(uint8_t) config.pin] = true;
	}
	System::CLOCK_EnableClock(System::kCLOCK_Iomuxc); /* iomuxc clock (iomuxc_clk_enable): 0x03u */
	*((volatile uint32_t*) (0x401F8014U + ((uint32_t) config.pin) * 4u)) = IOMUXC_SW_MUX_CTL_PAD_MUX_MODE((uint8_t) config.mux_mode) | IOMUXC_SW_MUX_CTL_PAD_SION(config.force_input);
	uint32_t pin_config_reg = 0;
	pin_config_reg |= (uint32_t) config.pin_config.fast_slew_rate;
	pin_config_reg |= (uint32_t)(((uint32_t) config.pin_config.drive_strength) << 3);
	pin_config_reg |= (uint32_t)(((uint32_t) config.pin_config.speed) << 6);
	pin_config_reg |= (uint32_t)(((uint32_t) config.pin_config.open_drain_enable) << 11);
	if (config.pin_config.pull_keep_config != Config::PinConfig::PullKeepConfig::kDisable) {
		pin_config_reg |= 1 << 12;
		if (config.pin_config.pull_keep_config == Config::PinConfig::PullKeepConfig::kPull) {
			pin_config_reg |= 1 << 13;
			pin_config_reg |= (uint32_t)(((uint32_t) config.pin_config.pull_config) << 14);
		}
	}
	pin_config_reg |= (uint32_t)(((uint32_t) config.pin_config.hysteresis_enable) << 16);
	*((volatile uint32_t*) (0x401F8204U + ((uint32_t) config.pin) * 4u)) = pin_config_reg;
}

}
}
