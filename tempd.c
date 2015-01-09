/* temperature daemon V 0.1 */

#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>

#define FAN_1_MANUAL "/sys/devices/platform/applesmc.768/fan1_manual"
#define FAN_2_MANUAL "/sys/devices/platform/applesmc.768/fan2_manual"

#define RD_CPU_1_TEMP "/sys/devices/platform/coretemp.0/hwmon/hwmon0/temp2_input"
#define RD_CPU_2_TEMP "/sys/devices/platform/coretemp.0/hwmon/hwmon0/temp3_input"

#define RD_FAN_1 "/sys/devices/platform/applesmc.768/fan1_input"
#define WR_FAN_1 "/sys/devices/platform/applesmc.768/fan1_output"
#define RD_FAN_2 "/sys/devices/platform/applesmc.768/fan2_input"
#define WR_FAN_2 "/sys/devices/platform/applesmc.768/fan2_output"

#define MIN_SPEED 2000
#define MAX_SPEED 5700

int read_cpu_temp(const char *cpu_path);
void write_fan_manual(const char *fan_manual_path, int manual);
void write_fan_speed(const char *fan_speed_path, int speed);

int main() {

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
