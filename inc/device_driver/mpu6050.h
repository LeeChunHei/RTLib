/*
 * mpu6050.h
 *
 *  Created on: Sep 21, 2018
 *      Author: LeeChunHei
 */

#ifndef INC_DEVICE_DRIVER_MPU6050_H_
#define INC_DEVICE_DRIVER_MPU6050_H_

#include "driver/i2c_master.h"

namespace DeviceDriver {

class MPU6050 {
public:
	struct Config {

	};
	MPU6050(Config& config);

private:
	Driver::I2CMaster i2c_master;
};

}

#endif /* INC_DEVICE_DRIVER_MPU6050_H_ */
