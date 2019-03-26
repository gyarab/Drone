using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Globalization;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Drone
{
    public partial class Form1 : Form
    {
        bool isConnected = false;
        String[] ports;
        SerialPort port;
        private double[] dat = new double[500];
        


        public Form1()
        {

            InitializeComponent();
            //disableControls();
            getAvailableComPorts();

            foreach (string port in ports)
            {
                comboBox1.Items.Add(port);
                Console.WriteLine(port);
                if (ports[0] != null)
                {
                    comboBox1.SelectedItem = ports[0];
                }
            }

            





        }


        public void Read() {
            while (true)
            {
                try
                {
                    string message = port.ReadLine();
                    message.Remove(0, 1);

                    // string[] data = message.Split('|');
                    Console.WriteLine(message);
                    dat[dat.Length - 1] = double.Parse(message,CultureInfo.InvariantCulture); ;
                    Array.Copy(dat, 1, dat, 0, dat.Length - 1);
                    if (chart1.IsHandleCreated)
                    {
                        this.Invoke((MethodInvoker)delegate { UpdateChart(); });
                    }
                    else
                    {
                        //***************
                    }
                    
                }
                catch (Exception e)
                {

                }
            }
        }
       /* void dataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                
                string s = port.ReadExisting();
                //  Console.WriteLine(port.ReadLine());
                string[] data = s.Split('#');
                foreach (string l in data)
                {
                    if (l[l.Length - 1] == 'X')
                    {
                        
                                
                       
                          
                    }

                }
                
            }
            catch (Exception z)
            {
                // MessageBox.Show(z.Message);
            }
            return;
        }*/


        private void UpdateChart()
        {
            chart1.Series["Series1"].Points.Clear();

            for (int i = 0; i < dat.Length - 1; ++i) {
                chart1.Series["Series1"].Points.AddY(dat[i]);
            }
        }

        void getAvailableComPorts()
        {
            ports = SerialPort.GetPortNames();
        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (!isConnected)
            {
                connectToArduino();
            }
            else
            {
                disconnectFromArduino();
            }
        }

        private void connectToArduino()
        {
            try
            {
                isConnected = true;
                string selectedPort = comboBox1.GetItemText(comboBox1.SelectedItem);
                port = new SerialPort(selectedPort, 115200, Parity.None, 8, StopBits.One);
                //port.DataReceived += dataReceived;
                port.Open();
                //port.Write("S");
                button1.Text = "Disconnect";
                //enableControls();
                Thread readThread = new Thread(Read);
                readThread.Start();
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message);
            }
        }


        private void disconnectFromArduino()
        {
            isConnected = false;
            //port.Write("#STOP\n");
            port.Close();
            button1.Text = "Connect";
            //disableControls();
            //resetDefaults();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            string Angle = "";//String.Format("0x{0:X}", (int)(numericUpDown1.Value * 1000));
            string P = ((int)(numericUpDown1.Value * 1000)).ToString("X4");
            string I = ((int)(numericUpDown2.Value * 1000)).ToString("X4");
            string D = ((int)(numericUpDown3.Value * 1000)).ToString("X4");


            if (isConnected)
            {
                Console.WriteLine("#" + P + "|" + I + "|" + D + "|" + Angle + "X");
                port.Write("#" + P + "|" + I + "|" + D + "X");

            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            port.Write("*");
        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void numericUpDown4_ValueChanged(object sender, EventArgs e)
        {

        }

        private void label5_Click(object sender, EventArgs e)
        {

        }

        private void numericUpDown3_ValueChanged(object sender, EventArgs e)
        {

        }

        private void label4_Click(object sender, EventArgs e)
        {

        }

        private void numericUpDown2_ValueChanged(object sender, EventArgs e)
        {

        }

        private void label6_Click(object sender, EventArgs e)
        {

        }
    }
}
