#include "am2320.h"

#include <ctime>
#include <chrono>
#include <thread>
#include <iostream>
#include <fstream>
#include <iomanip>

int main(int argc, char *argv[]) {
	Adafruit::AM2320 sensor(argv[1]);

	while (true) {
		std::ofstream log("history.txt", std::ios::app);		

		auto now = std::chrono::system_clock::now();
		auto now_time_t = std::chrono::system_clock::to_time_t(now);		

		sensor.temperature(); //just to wake it up
		log << std::put_time(std::localtime(&now_time_t), "%Y-%m-%d %H:%M:%S"); 
		log << ": " << sensor.temperature() << "C" << ";";
		log << sensor.humidity() << "%" << std::endl;

		std::this_thread::sleep_for(std::chrono::minutes(1));
		log.close();
	}
}