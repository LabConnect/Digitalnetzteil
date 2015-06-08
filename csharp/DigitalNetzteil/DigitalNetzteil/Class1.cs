using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using HidLibrary;

namespace DigitalNetzteil
{
    /// <summary>
    /// Class for digital power supply
    /// </summary>
    public class Device
    {
        // 0 Report ID, 1 Choice, 2 Value
        private byte[] writevalues = { 0, 0, 0 };    
        private byte[] readvalues = { 0, 0, 0 };
        private HidDevice mydevice;
        /// <summary>
        /// True if device initialized
        /// </summary>
        public bool isInitialized = false;
        
        /// <summary>
        /// Initialize a new device
        /// </summary>
        /// <param name="vid">Vendor ID</param>
        /// <param name="pid">Product ID</param>
        public Device(int vid, int pid)
        {
            mydevice = HidDevices.Enumerate(vid, pid).FirstOrDefault();
            if (mydevice != null)
            {
                mydevice.OpenDevice();
                isInitialized = true;               
            }
        }

        /// <summary>
        /// Close the device
        /// </summary>
        public void close()
        {
            if ((mydevice != null) && (mydevice.IsOpen == true) && (mydevice.IsConnected == true))
            {
                mydevice.CloseDevice();
            }
        }
        
        /// <summary>
        /// Sets the voltage of the device
        /// </summary>
        /// <param name="voltage">Voltage value (2 to 20)</param>
        /// <returns>False if device not connected</returns>
        public bool setVoltage(double voltage) 
        {
            bool success = false;

            if (mydevice.IsConnected == true)
            {
                writevalues[1] = 0x00;
                if (20 <= voltage)
                {
                    writevalues[2] = 200;
                }
                else if (0 >= voltage)
                {
                    writevalues[2] = 0;
                }
                else
                {
                    writevalues[2] = (byte)(voltage*10);
                }
                success = mydevice.WriteFeatureData(writevalues);
            }
            return success;
        }
        
        /// <summary>
        /// Sets the maximal current of the device
        /// </summary>
        /// <param name="current">Max current value (0 to 2)</param>
        /// <returns>False if device not connected</returns>
        public bool setCurrent(double current) 
        {
            bool success = false;

            if (mydevice.IsConnected == true)
            {
                writevalues[1] = 0x01;
                if (2 <= current)
                {
                    writevalues[2] = 200;
                }
                else if (0 >= current)
                {
                    writevalues[2] = 0;
                }
                else
                {
                    writevalues[2] = (byte)(current * 100);
                }
                success = mydevice.WriteFeatureData(writevalues);
            }
            return success;
        }
        
        /// <summary>
        /// Read the present voltage and current of the device
        /// </summary>
        /// <param name="voltage">Present Voltage</param>
        /// <param name="current">Present Current</param>
        /// <returns>False if device not connected</returns>
        public bool readValues(out double voltage, out double current) 
        {
            bool success = false;
            voltage = -1;
            current = -1;

            if (mydevice.IsConnected == true)
            {
                success = mydevice.ReadFeatureData(out readvalues);
                voltage = (double)(readvalues[1] * 0.1);
                current = (double)(readvalues[2] * 0.01);
            }
            return success; ;
            
        }
    }
}
