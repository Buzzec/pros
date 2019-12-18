/**
 * \file devices/vdml_imu.c
 *
 * Contains functions for interacting with the VEX Inertial sensor.
 *
 * Copyright (c) 2017-2019, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <errno.h>
#include "pros/imu.h"
#include "v5_api.h"
#include "vdml/registry.h"
#include "vdml/vdml.h"

#define ERROR_IMU_STILL_CALIBRATING(port, device, err_return)                  \
	if (vexDeviceImuStatusGet(device->device_info) & E_IMU_STATUS_CALIBRATING) { \
		errno = EAGAIN;                                                            \
		return_port(port - 1, err_return);                                         \
	}

int32_t imu_reset(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_IMU);
	ERROR_IMU_STILL_CALIBRATING(port, device, PROS_ERR);
	vexDeviceImuReset(device->device_info);
	return_port(port - 1, 1);
}

double imu_get_heading(uint8_t port) {
	claim_port_f(port - 1, E_DEVICE_IMU);
	ERROR_IMU_STILL_CALIBRATING(port, device, PROS_ERR_F);
	double rtn = vexDeviceImuHeadingGet(device->device_info);
	return_port(port - 1, rtn);
}

double imu_get_degrees(uint8_t port) {
	claim_port_f(port - 1, E_DEVICE_IMU);
	ERROR_IMU_STILL_CALIBRATING(port, device, PROS_ERR_F);
	double rtn = vexDeviceImuDegreesGet(device->device_info);
	return_port(port - 1, rtn);
}

#define QUATERNION_ERR_INIT \
	{ .a = PROS_ERR_F, .b = PROS_ERR_F, .c = PROS_ERR_F, .d = PROS_ERR_F }

quaternion_s_t imu_get_quaternion(uint8_t port) {
	quaternion_s_t rtn = QUATERNION_ERR_INIT;
	v5_smart_device_s_t* device;
	if (!claim_port_try(port - 1, E_DEVICE_IMU)) {
		return rtn;
	}
	device = registry_get_device(port - 1);
	ERROR_IMU_STILL_CALIBRATING(port, device, rtn);
	vexDeviceImuQuaternionGet(device->device_info, (V5_DeviceImuQuaternion*)&rtn);
	return_port(port - 1, rtn);
}

#define ATTITUDE_ERR_INIT \
	{ .pitch = PROS_ERR_F, .roll = PROS_ERR_F, .yaw = PROS_ERR_F }

attitude_s_t imu_get_attitude(uint8_t port) {
	attitude_s_t rtn = ATTITUDE_ERR_INIT;
	v5_smart_device_s_t* device;
	if (!claim_port_try(port - 1, E_DEVICE_IMU)) {
		return rtn;
	}
	device = registry_get_device(port - 1);
	ERROR_IMU_STILL_CALIBRATING(port, device, rtn);
	vexDeviceImuAttitudeGet(device->device_info, (V5_DeviceImuAttitude*)&rtn);
	return_port(port - 1, rtn);
}

#define RAW_IMU_ERR_INIT \
	{ .x = PROS_ERR_F, .y = PROS_ERR_F, .z = PROS_ERR_F, .w = PROS_ERR_F }

imu_s_t imu_get_raw_gyro(uint8_t port) {
	imu_s_t rtn = RAW_IMU_ERR_INIT;
	v5_smart_device_s_t* device;
	if (!claim_port_try(port - 1, E_DEVICE_IMU)) {
		return rtn;
	}
	device = registry_get_device(port - 1);
	ERROR_IMU_STILL_CALIBRATING(port, device, rtn);
	vexDeviceImuRawGyroGet(device->device_info, (V5_DeviceImuRaw*)&rtn);
	return_port(port - 1, rtn);
}

imu_s_t imu_get_raw_accel(uint8_t port) {
	imu_s_t rtn = RAW_IMU_ERR_INIT;
	v5_smart_device_s_t* device;
	if (!claim_port_try(port - 1, E_DEVICE_IMU)) {
		return rtn;
	}
	device = registry_get_device(port - 1);
	ERROR_IMU_STILL_CALIBRATING(port, device, rtn);
	vexDeviceImuRawAccelGet(device->device_info, (V5_DeviceImuRaw*)&rtn);
	return_port(port - 1, rtn);
}

imu_status_e_t imu_get_status(uint8_t port) {
	imu_status_e_t rtn = E_IMU_STATUS_ERROR;
	v5_smart_device_s_t* device;
	if (!claim_port_try(port - 1, E_DEVICE_IMU)) {
		return rtn;
	}
	device = registry_get_device(port - 1);
	ERROR_IMU_STILL_CALIBRATING(port, device, E_IMU_STATUS_ERROR);
	rtn = vexDeviceImuStatusGet(device->device_info);
	return_port(port - 1, rtn);
}
