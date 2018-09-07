/*
 * i2c_master.h
 *
 *  Created on: Sep 4, 2018
 *      Author: LeeChunHei
 */

#ifndef INC_DRIVER_I2C_MASTER_H_
#define INC_DRIVER_I2C_MASTER_H_

#include "system/cmsis/access_layer/access_layer.h"
#include "system/pinout/pinout.h"

namespace Driver {

class I2CMaster {
public:
	struct Config {
		enum struct PinConfig {
			k2PinOpenDrain, /*!< LPI2C Configured for 2-pin open drain mode */
			k2PinOutputOnly, /*!< LPI2C Configured for 2-pin output only mode (ultra-fast mode) */
			k2PinPushPull, /*!< LPI2C Configured for 2-pin push-pull mode */
			k4PinPushPull, /*!< LPI2C Configured for 4-pin push-pull mode */
			k2PinOpenDrainWithSeparateSlave, /*!< LPI2C Configured for 2-pin open drain mode with separate LPI2C slave */
			k2PinOutputOnlyWithSeparateSlave, /*!< LPI2C Configured for 2-pin output only mode(ultra-fast mode) with separate LPI2C slave */
			k2PinPushPullWithSeparateSlave, /*!< LPI2C Configured for 2-pin push-pull mode with separate LPI2C slave */
			k4PinPushPullWithInvertedOutput /*!< LPI2C Configured for 4-pin push-pull mode(inverted outputs) */
		};
		struct HostRequest {
			enum struct Source {
				kExternalPin, /*!< Select the LPI2C_HREQ pin as the host request input */
				kInputTrigger /*!< Select the input trigger as the host request input */
			};
			bool enable = false; /*!< Enable host request. */
			Source source = Source::kExternalPin; /*!< Host request source. */
			bool is_active_high = true; /*!< Host request pin polarity. */
		};
		System::Pinout::Name scl;
		System::Pinout::Name sda;
		bool enable_master = true; /*!< Whether to enable master mode. */
		bool enable_doze = true; /*!< Whether master is enabled in doze mode. */
		bool debug_enable = false; /*!< Enable transfers to continue when halted in debug mode. */
		bool ignore_ack = false; /*!< Whether to ignore ACK/NACK. */
		PinConfig pin_config = PinConfig::k2PinOpenDrain; /*!< The pin configuration option. */
		uint32_t baud_rate_Hz = 100000u; /*!< Desired baud rate in Hertz. */
		uint32_t bus_idle_timeout_ns = 0; /*!< Bus idle timeout in nanoseconds. Set to 0 to disable. */
		uint32_t pin_low_timeout_ns = 0; /*!< Pin low timeout in nanoseconds. Set to 0 to disable. */
		uint8_t sda_glitch_filter_width_ns = 0; /*!< Width in nanoseconds of glitch filter on SDA pin. Set to 0 to disable. */
		uint8_t scl_glitch_filter_width_ns = 0; /*!< Width in nanoseconds of glitch filter on SCL pin. Set to 0 to disable. */
		uint32_t send_wait_time = 0;	//The time in ns that i2c will wait for the tx fifo is empty, default 0 means no waiting time given
		uint32_t recieve_wait_time=0;	//The time in ns that i2c will wait for the data come in, default 0 means no waiting time given
		HostRequest host_request;/*!< Host request options. */
	};
	I2CMaster(Config& config);
	bool SendByte(const uint8_t slave_addr, const uint8_t reg_addr, const uint8_t data);
	bool SendByte(const uint8_t slave_addr, const uint16_t reg_addr, const uint8_t data);
	bool GetByte(const uint8_t slave_addr, const uint8_t reg_addr, uint8_t& data);
	bool GetByte(const uint8_t slave_addr, const uint16_t reg_addr, uint8_t& data);
private:
	inline void Reset() {
		i2c_base->MCR = LPI2C_MCR_RST_MASK;
		i2c_base->MCR = 0;
	}
	inline void Enable(bool enable) {
		i2c_base->MCR = (i2c_base->MCR & ~LPI2C_MCR_MEN_MASK) | LPI2C_MCR_MEN(enable);
	}
	inline void SetWatermarks(uint8_t tx_words, uint8_t rx_words) {
		i2c_base->MFCR = LPI2C_MFCR_TXWATER(tx_words) | LPI2C_MFCR_RXWATER(rx_words);
	}
	void SetBaudRate(uint32_t source_clock_Hz, uint32_t baud_rate_Hz);
	uint32_t GetCyclesForWidth(uint32_t source_clock_hz, uint32_t width_ns, uint32_t max_cycles, uint32_t prescaler);
	bool CheckForBusyBus();
	bool CheckAndClearError();
	bool WaitForTxReady();
	bool SendByteWithStartCMD(const uint8_t byte);
	bool SendStopCMD();
	bool SendByte(const uint8_t byte);
	bool GetByte(uint8_t& byte);

	LPI2C_Type* i2c_base;
	uint32_t send_wait_time;
	uint32_t recieve_wait_time;
	System::Pinout::Name scl;
	System::Pinout::Name sda;
}
;

}

#endif /* INC_DRIVER_I2C_MASTER_H_ */
