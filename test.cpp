#include <iostream>
#include <string>
#include <vector>

#include <vl53l1_api_core.h>
#include <vl53l1_api.h>
#include <vl53l1_platform.h>

#include <math.h>

#include <i2c.h>

#define xSTR(x) #x
#define STR(x) xSTR(x)

#define CHECK_STATUS(func) { \
	VL53L1_Error status = func; \
	if (status != VL53L1_ERROR_NONE) { \
		cout << "VL53L1X: Error %d on %s" << status << STR(func); \
	} \
}

using namespace std;

int main(int argc, char **argv)
{
	// Read parameters
	int mode, i2c_bus, i2c_address;
	double poll_rate, timing_budget, offset;
	bool ignore_range_status;
	
	double min_signal = 0.0f;
	double max_sigma = 4.0f;
	
	std::vector<int> pass_statuses { VL53L1_RANGESTATUS_RANGE_VALID,
	                                 VL53L1_RANGESTATUS_RANGE_VALID_NO_WRAP_CHECK_FAIL,
	                                 VL53L1_RANGESTATUS_RANGE_VALID_MERGED_PULSE };
									 
	mode = 3;
	i2c_bus = 1;
	i2c_address = 0x29;
	poll_rate = 100.0;
	ignore_range_status = false;
	timing_budget = 0.1;
	offset = 0.0;
		

	if (timing_budget < 0.02 || timing_budget > 1) {
		cout << "Error: timing_budget should be within 0.02 and 1 s (%g is set)" << timing_budget << endl;
		exit(-1);
	}

	// The minimum inter-measurement period must be longer than the timing budget + 4 ms (*)
	// double inter_measurement_period = timing_budget + 0.004;
	double inter_measurement_period = timing_budget + 0.009;

	// Setup I2C bus
	i2c_setup(i2c_bus, i2c_address);

	// Init sensor
	VL53L1_Dev_t dev;
	VL53L1_Error dev_error;
	VL53L1_software_reset(&dev);
	VL53L1_WaitDeviceBooted(&dev);
	VL53L1_DataInit(&dev);
	VL53L1_StaticInit(&dev);
	VL53L1_SetPresetMode(&dev, VL53L1_PRESETMODE_AUTONOMOUS);

	// Print device info
	VL53L1_DeviceInfo_t device_info;
	CHECK_STATUS(VL53L1_GetDeviceInfo(&dev, &device_info));
	
	printf("VL53L1X: Device name: %." STR(VL53L1_DEVINFO_STRLEN) "s", device_info.Name);
	printf("\r\n");
	printf("VL53L1X: Device type: %." STR(VL53L1_DEVINFO_STRLEN) "s", device_info.Type);
	printf("\r\n");
	printf("VL53L1X: Product ID: %." STR(VL53L1_DEVINFO_STRLEN) "s", device_info.ProductId);
	printf("\r\n");
	printf("VL53L1X: Type: %u Version: %u.%u \r\n", device_info.ProductType,
	          device_info.ProductRevisionMajor, device_info.ProductRevisionMinor);
	printf("\r\n");
	
	/*cout << "VL53L1X: Device name: " << device_info.Name << endl;
	cout << "VL53L1X: Device type: " << device_info.Type << endl;
	cout << "VL53L1X: Product ID: " << device_info.ProductId << endl;
	cout << "VL53L1X: Type:" << device_info.ProductType << "Version:" << device_info.ProductRevisionMajor << "." << device_info.ProductRevisionMinor << endl;*/

	// Setup sensor
	CHECK_STATUS(VL53L1_SetDistanceMode(&dev, mode));
	CHECK_STATUS(VL53L1_SetMeasurementTimingBudgetMicroSeconds(&dev, round(timing_budget * 1e6)));
	
	
	CHECK_STATUS(VL53L1_SetLimitCheckValue(&dev, VL53L1_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, min_signal * 65536));
	CHECK_STATUS(VL53L1_SetLimitCheckValue(&dev, VL53L1_CHECKENABLE_SIGMA_FINAL_RANGE, max_sigma * 1000 * 65536));
	

	// Start sensor
	for (int i = 0; i < 100; i++) {
		CHECK_STATUS(VL53L1_SetInterMeasurementPeriodMilliSeconds(&dev, round(inter_measurement_period * 1e3)));
		dev_error = VL53L1_StartMeasurement(&dev);
		if (dev_error == VL53L1_ERROR_INVALID_PARAMS) {
			inter_measurement_period += 0.001; // Increase inter_measurement_period to satisfy condition (*)
		} else break;
	}

	// Check for errors after start
	if (dev_error != VL53L1_ERROR_NONE) {
		// ROS_FATAL("VL53L1X: Can't start measurement: error %d", dev_error);
		// ros::shutdown();
	}

	// ROS_INFO("VL53L1X: ranging");

	VL53L1_RangingMeasurementData_t measurement_data;

	
	while (true) {
	
		// Check the data is ready
		uint8_t data_ready = 0;
		VL53L1_GetMeasurementDataReady(&dev, &data_ready);
		if (!data_ready) {
			continue;
		}

		// Read measurement
		VL53L1_GetRangingMeasurementData(&dev, &measurement_data);
		VL53L1_ClearInterruptAndStartMeasurement(&dev);
		
		cout << measurement_data.RangeMilliMeter + offset << endl;

		
	}

	// Release
	// ROS_INFO("VL53L1X: stop ranging");
	VL53L1_StopMeasurement(&dev);
	i2c_release();
}