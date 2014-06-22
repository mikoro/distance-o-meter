using System;
using System.Collections.Generic;
using System.Text;
using System.IO.Ports;
using System.ComponentModel;

namespace DistanceOMeter
{
	public class MySettings
	{
		public MySettings()
		{
			echos[0] = true;
		}

		[CategoryAttribute("Port settings"), DescriptionAttribute("Name of the serial port (default: COM1)")]
		public string PortName
		{
			get { return portName; }
			set { portName = value; }
		}

		[CategoryAttribute("Port settings"), DescriptionAttribute("Baud rate of the connection (default: 9600)")]
		public int BaudRate
		{
			get { return baudRate; }
			set { baudRate = value; }
		}

		[CategoryAttribute("Port settings"), DescriptionAttribute("Type of parity bit to be sent with each character (default: None)")]
		public Parity Parity
		{
			get { return parity; }
			set { parity = value; }
		}

		[CategoryAttribute("Port settings"), DescriptionAttribute("Number of bits in each character (default: 8)")]
		public int DataBits
		{
			get { return dataBits; }
			set { dataBits = value; }
		}

		[CategoryAttribute("Port settings"), DescriptionAttribute("Number of stop bits to be sent with each character (default: One)")]
		public StopBits StopBits
		{
			get { return stopBits; }
			set { stopBits = value; }
		}

		[CategoryAttribute("Port settings"), DescriptionAttribute("Time to wait for the device, if exceeded, connection is closed (default: 1000)")]
		public int Timeout
		{
			get { return timeout; }
			set { timeout = value; }
		}

		[CategoryAttribute("Probe settings"), DescriptionAttribute("Whether to probe device continuously (default: False)")]
		public bool ContinuousProbe
		{
			get { return continuousProbe; }
			set { continuousProbe = value; }
		}

		[CategoryAttribute("Probe settings"), DescriptionAttribute("Interval of the probing in milliseconds (default: 250)")]
		public int ProbeInterval
		{
			get { return probeInterval; }
			set { probeInterval = value; }
		}

		[CategoryAttribute("Graph settings"), DescriptionAttribute("Which echos will be drawn on the graph")]
		public bool[] Echos
		{
			get { return echos; }
			set { echos = value; }
		}

		[CategoryAttribute("Graph settings"), DescriptionAttribute("Whether the graph follows automatically (default: True)")]
		public bool GraphFollow
		{
			get { return graphFollow; }
			set { graphFollow = value; }
		}

		private string portName = "COM1";
		private int baudRate = 9600;
		private Parity parity = Parity.None;
		private int dataBits = 8;
		private StopBits stopBits = StopBits.One;
		private bool continuousProbe = false;
		private int probeInterval = 250;
		private bool[] echos = new bool[17];
		private bool graphFollow = true;
		private int timeout = 1000;
	}
}
