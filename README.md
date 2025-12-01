## How to Run the Python code and Receive Transmissions from the Microcontroller
### 1. Connect the Microcontroller
  - Plug the Microcontroller and the UART into the PC.
  - Confirm that you are connected to the correct COM port (COM3). If you are connected to a different port, please modify this at the top of the Python Script file. It is under PORT = "COM3".
### 2. Build and Run the App 2 Project
### 3. Start the Receiver Program
  - When you are ready to transmit data, open a terminal and navigate to the project folder where Receiver.py is located.
  - Run the python script. The program will initalize the serial connection and wait for data.
### 4. Begin Data Transmission
- While the microcontroller is running, press PB3 on the microcontroller to start sending UART data.
- Note: Although the Python Script is already running, it will not start reading and recording data until PB3 has been pressed on the microcontroller.
### 5. Wait until the recording is completed
- Data will be sent in alternating lines: ADC:(value) and INT:(value)
- Script will continue recording unless the User presses PB1 (OFF Mode) or PB3 or until the 60 second interval has been completed.
- Note: If the User decides to press PB1 or PB3 to stop transmissions before the period is completed, the Python Script will still continue to run however will not be recording data until PB3 in ON mode is pressed again.
### 6. Transmission is complete!
- After a period of 1 minute, two graphs, a ADC vs Time and Intensity vs Time will be generated.
- The two graphs will be saved as png files.
- A csv data file will be saved capturing the intensity of the LED and the ADC readings with time.
  
