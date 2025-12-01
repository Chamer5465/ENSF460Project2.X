# File: Receiver.py
# Author: Chase Mackenzie, Aaron Montesines, Patricia Agdamag
# Created on November 1, 2025, 1:05 PM

import serial
import time
import csv
import matplotlib.pyplot as plt


# Settings

PORT = "COM3"      
BAUD = 9600        
DURATION = 60 #60s duration
GROUP_NAME = "ENSF460-Group12"


def main():
    ser = serial.Serial(PORT, BAUD, timeout=1)
    print(f"Connected to {PORT} at {BAUD} baud\n")
    print("Press PB3 on microcontroller to start data transmission...\n")

    filename = f"{GROUP_NAME}.csv"
    start = None

    timestamps = []
    adc_values = []
    intensity_values = []

    adc = None
    intensity = None

    with open(filename, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["time_s", "adc", "intensity"])

        while True:
     
            # Read Uart line
            
            raw = ser.readline().decode(errors="ignore").strip()
            raw = raw.replace("\x00", "")   

            print("RAW:", repr(raw))        # Debug output

            if not raw:
                continue

            # start timer only when data starts coming in
            if start is None and (raw.startswith("ADC:") or raw.startswith("INT:")):
                start = time.time()
                print("Data transmission started...\n")
            
            # Parse ADC lines
            if raw.startswith("ADC:"):
                try:
                    adc = int(raw.split(":")[1])
                except:
                    adc = None

            
            # Parse INT lines
            elif raw.startswith("INT:"):
                try:
                    intensity = int(raw.split(":")[1])
                except:
                    intensity = None

            
            # Log data when both exist
            if adc is not None and intensity is not None:
                t = time.time() - start
                timestamps.append(t)
                adc_values.append(adc)
                intensity_values.append(intensity)
                writer.writerow([t, adc, intensity])

                adc = None
                intensity = None

            if start is not None and (time.time() - start) >= DURATION:
                break

    ser.close()
    print("\nSaved CSV:", filename)

    
    # debug
    if len(timestamps) == 0:
        print("No data recorded! Check RAW output.")
        return

    #  ADC Plot 
    plt.figure()
    plt.plot(timestamps, adc_values)
    plt.title("ADC vs Time")
    plt.xlabel("Time (s)")
    plt.ylabel("ADC Value")
    plt.grid(True)
    plt.savefig(f"{GROUP_NAME}_adc_plot.png")

    #  Intensity Plot 
    plt.figure()
    plt.plot(timestamps, intensity_values)
    plt.title("Intensity vs Time")
    plt.xlabel("Time (s)")
    plt.ylabel("Intensity (%)")
    plt.grid(True)
    plt.savefig(f"{GROUP_NAME}_intensity_plot.png")

    plt.show()
    print(f"Plots saved as {GROUP_NAME}_adc_plot.png and {GROUP_NAME}_intensity_plot.png")


if __name__ == "__main__":
    main()
