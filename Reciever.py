import serial
import time
import csv
import matplotlib.pyplot as plt


# Settings

PORT = "COM3"      
BAUD = 9600        
DURATION = 60 #60s duration


def main():
    ser = serial.Serial(PORT, BAUD, timeout=1)
    print(f"Connected to {PORT} at {BAUD} baud\n")

    filename = "capture.csv"
    start = time.time()

    timestamps = []
    adc_values = []
    intensity_values = []

    adc = None
    intensity = None

    with open(filename, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["time_s", "adc", "intensity"])

        while time.time() - start < DURATION:
            
            # Read Uart line
            
            raw = ser.readline().decode(errors="ignore").strip()
            raw = raw.replace("\x000", "")   

            print("RAW:", repr(raw))        # Debug output

            if not raw:
                continue

            
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
    plt.savefig("adc_plot.png")

    #  Intensity Plot 
    plt.figure()
    plt.plot(timestamps, intensity_values)
    plt.title("Intensity vs Time")
    plt.xlabel("Time (s)")
    plt.ylabel("Intensity (%)")
    plt.grid(True)
    plt.savefig("intensity_plot.png")

    plt.show()
    print("Plots saved as adc_plot.png and intensity_plot.png")


if __name__ == "__main__":
    main()
