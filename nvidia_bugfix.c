#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// ---------------------------------------------------------
// FUNCTION: Fetch the system's low battery threshold
// Reads the UPower daemon configuration dynamically.
// ---------------------------------------------------------
int get_low_battery_threshold() {
    FILE *upower_file = fopen("/etc/UPower/UPower.conf", "r");
    int threshold = 20; // Fallback default if file can't be read

    if (upower_file == NULL) {
        return threshold;
    }

    char line[256];
    // Read the file line by line to find "PercentageLow="
    while (fgets(line, sizeof(line), upower_file)) {
        if (strncmp(line, "PercentageLow=", 14) == 0) {
            sscanf(line, "PercentageLow=%d", &threshold);
            break;
        }
    }
    
    fclose(upower_file);
    return threshold;
}

int main() {
    // ---------------------------------------------------------
    // CHECK 1: Is the NVIDIA driver active?
    // ---------------------------------------------------------
    if (access("/usr/bin/nvidia-smi", F_OK) != 0) {
        return 0; // Not NVIDIA or driver not loaded, exit.
    }

    // ---------------------------------------------------------
    // CHECK 2: Battery Percentage dynamically fetched
    // ---------------------------------------------------------
    FILE *bat_file = fopen("/sys/class/power_supply/BAT1/capacity", "r");
    if (bat_file == NULL) {
        return 1; // Failsafe
    }
    
    int battery_pct = 0;
    fscanf(bat_file, "%d", &battery_pct);
    fclose(bat_file);

    int power_save_threshold = get_low_battery_threshold();

    // If battery is strictly greater than the threshold, exit.
    if (battery_pct > power_save_threshold) {
        return 0;
    }

    // ---------------------------------------------------------
    // ACTION: Restart nvidia-powerd
    // ---------------------------------------------------------
    system("/usr/bin/systemctl restart nvidia-powerd");

    return 0;
}
