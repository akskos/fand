/* temperature daemon V 0.1 */

#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>
#include <unistd.h>

#define FAN_1_MANUAL "/sys/devices/platform/applesmc.768/fan1_manual"
#define FAN_2_MANUAL "/sys/devices/platform/applesmc.768/fan2_manual"

#define CPU_1_TEMP "/sys/devices/platform/coretemp.0/hwmon/hwmon0/temp2_input"
#define CPU_2_TEMP "/sys/devices/platform/coretemp.0/hwmon/hwmon0/temp3_input"

#define FAN_1_SPEED_IN "/sys/devices/platform/applesmc.768/fan1_input"
#define FAN_1_SPEED_OUT "/sys/devices/platform/applesmc.768/fan1_output"
#define FAN_2_SPEED_IN "/sys/devices/platform/applesmc.768/fan2_input"
#define FAN_2_SPEED_OUT "/sys/devices/platform/applesmc.768/fan2_output"

#define MIN_SPEED 2000
#define MAX_SPEED 5800

int read_cpu_temp(const char *cpu_path);
void write_fan_manual(const char *fan_manual_path, int manual);
void write_fan_speed(const char *fan_speed_path, int speed);
int minmax_fanspeed(int fanspeed);

int main() {

	int old_fanspeed = -1;

	write_fan_manual(FAN_1_MANUAL, 1);
	write_fan_manual(FAN_2_MANUAL, 1);

	for (;;) {
		int cpu_1_temp = read_cpu_temp(CPU_1_TEMP) / 1000;
		int cpu_2_temp = read_cpu_temp(CPU_2_TEMP) / 1000;

		int avg_cpu_temp = 0;
		avg_cpu_temp += cpu_1_temp;
		avg_cpu_temp += cpu_2_temp;
		avg_cpu_temp /= 2;

		int fanspeed = (avg_cpu_temp - 38) * 160;
		fanspeed = minmax_fanspeed(fanspeed);

		if (fanspeed != old_fanspeed) {
			write_fan_speed(FAN_1_SPEED_OUT, fanspeed);
			write_fan_speed(FAN_2_SPEED_OUT, fanspeed);
			old_fanspeed = fanspeed;
		}

		usleep(2500000);
	}

	return 0;
}

int read_cpu_temp(const char *cpu_path) {
	int temp;
	FILE *file;

	if ((file = fopen(cpu_path, "r")) != NULL) {
		fscanf(file, "%d", &temp);
		fclose(file);
		return temp;
	} else {
		syslog(LOG_ERR, "Failed to read %s", cpu_path);		
		exit(1);
	}
}

void write_fan_manual(const char *fan_manual_path, int manual) {
	FILE *file;
	if ((file = fopen(fan_manual_path, "w")) != NULL) {
		fprintf(file, "%d", manual);
	} else {
		syslog(LOG_ERR, "Failed to write to %s", fan_manual_path);
		exit(1);
	}
}

void write_fan_speed(const char *fan_speed_path, int speed) {
	FILE *file;

	if ((file = fopen(fan_speed_path, "w")) != NULL) {
		fprintf(file, "%d", speed);
		fclose(file);
	} else {
		syslog(LOG_ERR, "Failed to write to %s", fan_speed_path);
		exit(1);
	}
}

int minmax_fanspeed(int fanspeed) {
	if (fanspeed > MAX_SPEED) {
		return MAX_SPEED;
	}
	if (fanspeed < MIN_SPEED) {
		return MIN_SPEED;
	}
	return fanspeed;
}
