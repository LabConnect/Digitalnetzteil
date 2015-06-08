using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Timers;
using DigitalNetzteil;

namespace NetzteilGUI
{
    public partial class Netzteil : Form
    {
        Device myDevice = null;
        public Netzteil()
        {
            InitializeComponent();
        }

        private void btnOpen_Click(object sender, EventArgs e)
        {
            // Geraet oeffnen
            myDevice = new Device(0x1209, 0x3333);
            if (myDevice.isInitialized == true)
            {
                // Bei Erfolg Bedienelemente freigeben
                // und Timer fuer Datenabfrage starten
                btnOpen.Enabled = false;
                btnClose.Enabled = true;
                numCurrent.Enabled = true;
                numVoltage.Enabled = true;
                timer1.Enabled = true;
                timer1.Interval = 5000;
                timer1.Start();
            }
            else
            {
                MessageBox.Show("Device not found!");
            }
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            // Resourcen freigeben und Geraet trennen
            myDevice.close();
            myDevice = null;
            // Bedienelemente sperren
            btnOpen.Enabled = true;
            btnClose.Enabled = false;
            numCurrent.Enabled = false;
            numVoltage.Enabled = false;
            timer1.Enabled = false;
            timer1.Stop();
        }

        private void numVoltage_ValueChanged(object sender, EventArgs e)
        {
            // Spannung uebertragen
            if (!myDevice.setVoltage((double)numVoltage.Value))
                MessageBox.Show("Device not connected", "Failure", MessageBoxButtons.OK);
        }

        private void numCurrent_ValueChanged(object sender, EventArgs e)
        {
            // Strom max uebertragen
            if (!myDevice.setCurrent((double)numCurrent.Value))
                MessageBox.Show("Device not connected", "Failure", MessageBoxButtons.OK);
        }
        
        //Daten im Sekundentakt vom Netzteil empfangen
        private void timer1_Tick(object sender, EventArgs e)
        {
            updateData();
        }

        private void updateData()
        {
            double voltage;
            double current;

            if (myDevice.readValues(out voltage, out current))
            {
                if (Color.Red == txtVoltage.ForeColor)
                    txtVoltage.ForeColor = Color.Black;
                if (Color.Red == txtCurrent.ForeColor)
                    txtCurrent.ForeColor = Color.Black;

                txtVoltage.Text = voltage.ToString("00.00");
                txtCurrent.Text = current.ToString("00.00");
            }
            else
            {
                txtVoltage.ForeColor = Color.Red;
                txtCurrent.ForeColor = Color.Red;
            }
        }
    }
}
