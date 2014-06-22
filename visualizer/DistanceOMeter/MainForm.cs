using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using ZedGraph;
using System.IO.Ports;
using System.IO;
using System.Xml.Serialization;

namespace DistanceOMeter
{
	public partial class MainForm : Form
	{
		public MainForm()
		{
			InitializeComponent();
		}

		private void MainForm_Load(object sender, EventArgs e)
		{
			if (File.Exists("settings.xml"))
			{
				XmlSerializer xs = new XmlSerializer(typeof(MySettings));
				TextReader tr = new StreamReader("settings.xml");
				settings = (MySettings)xs.Deserialize(tr);
				tr.Close();
			}

			propertyGridSettings.SelectedObject = settings;
			GraphPane pane = zedGraphControl.GraphPane;
			Color[] curveColors = new Color[] { Color.Red, Color.Green, Color.Blue, Color.Orange, Color.Yellow, Color.Violet, Color.Black, Color.Brown, Color.Cyan, Color.Magenta, Color.Lime, Color.Pink, Color.Purple, Color.Silver, Color.Gold, Color.Gray, Color.Indigo };

			for (int i = 0; i < pointLists.Length; ++i)
			{
				pointLists[i] = new RollingPointPairList(100);
				LineItem curve = pane.AddCurve("Echo " + i.ToString(), pointLists[i], curveColors[i], SymbolType.None);
				curve.Line.Width = 2;
				curve.Symbol.Fill = new Fill(Color.White);
				curve.Symbol.Size = 5;
				curve.Line.IsSmooth = true;
				curve.IsVisible = settings.Echos[i];
				curve.Label.IsVisible = settings.Echos[i];
			}

			pane.Title.IsVisible = false;
			pane.XAxis.Title.Text = "Probe, No.";
			pane.XAxis.Scale.Format = "f0";
			pane.XAxis.Scale.Min = 0;
			pane.XAxis.Scale.Max = 30;
			pane.YAxis.Title.Text = "Distance, cm";
			pane.YAxis.Scale.Format = "f0";
			pane.IsFontsScaled = false;
			pane.Chart.Fill = new Fill(Color.White, Color.SteelBlue, 45.0F);

			timerKeepalive.Interval = 500;
			timerKeepalive.Enabled = true;
			timerKeepalive.Start();

			pane.AxisChange();
		}

		private void buttonConnection_Click(object sender, EventArgs e)
		{
			if (connected)
			{
				Disconnect();
			}
			else
			{
				try { port = new SerialPort(settings.PortName, settings.BaudRate, settings.Parity, settings.DataBits, settings.StopBits); }
				catch (ArgumentOutOfRangeException ex)
				{
					MessageBox.Show(this, "Could not open the serial port: " + ex.Message, "Distance-O-Meter Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
					return;
				}

				port.ReadTimeout = settings.Timeout;

				try { port.Open(); }
				catch (Exception ex)
				{
					if (ex is InvalidOperationException || ex is ArgumentOutOfRangeException || ex is ArgumentException || ex is IOException || ex is UnauthorizedAccessException || ex is ObjectDisposedException)
					{
						MessageBox.Show(this, "Could not open the serial port: " + ex.Message, "Distance-O-Meter Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
						return;
					}
					else throw;
				}

				port.Write("c");
				byte respond = 0;

				try { respond = (byte)port.ReadByte(); }
				catch (TimeoutException)
				{
					Disconnect();
					MessageBox.Show(this, "Device did not respond in time for initiating the connection", "Distance-O-Meter Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
					return;
				}

				if (respond == 'a')
				{
					port.Write("a");
					connected = true;
					buttonConnection.Text = "Disconnect";
					buttonProbe.Enabled = !settings.ContinuousProbe;
					propertyGridSettings_PropertyValueChanged(null, null);
				}
				else
				{
					Disconnect();
					MessageBox.Show(this, "Device did not respond properly to handshake", "Distance-O-Meter Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
				}
			}
		}

		private void buttonProbe_Click(object sender, EventArgs e)
		{
			if (connected)
			{
				timerKeepalive.Stop();
				byte[] buffer = new byte[34];
				port.Write("r");
				
				for (int i = 0; i < 34; ++i)
				{
					try { buffer[i] = (byte)port.ReadByte(); }
					catch (TimeoutException)
					{
						Disconnect();
						MessageBox.Show(this, "Device did not respond in time for probe data", "Distance-O-Meter Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
						return;
					}
				}

				timerKeepalive.Start();
				listBox.Items.Clear();
				++currentProbe;

				for (int i = 0; i < 33; i += 2)
				{
					int value = buffer[i] << 8;
					value |= buffer[i + 1];
					listBox.Items.Add(value);

					IPointListEdit list = zedGraphControl.GraphPane.CurveList[i / 2].Points as IPointListEdit;
					list.Add(currentProbe, value);
				}

				if (settings.GraphFollow)
				{
					Scale scale = zedGraphControl.GraphPane.XAxis.Scale;
					if (currentProbe > scale.Max - scale.MajorStep)
					{
						scale.Max = currentProbe + scale.MajorStep;
						scale.Min = scale.Max - 30.0;
					}
				}

				zedGraphControl.AxisChange();
				zedGraphControl.Invalidate();
			}
		}

		private void timerKeepalive_Tick(object sender, EventArgs e)
		{
			if (connected)
			{
				port.Write("k");
				byte respond = 0;

				try { respond = (byte)port.ReadByte(); }
				catch (TimeoutException)
				{
					Disconnect();
					MessageBox.Show(this, "Device did not respond in time to keepalive", "Distance-O-Meter Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
					return;
				}

				if (respond != 'k')
				{
					Disconnect();
					MessageBox.Show(this, "Device did not respond correctly to keepalive", "Distance-O-Meter Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
				}
			}
		}

		private void propertyGridSettings_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
		{
			if (port != null)
				port.ReadTimeout = settings.Timeout;

			if (settings.ProbeInterval < 100)
				settings.ProbeInterval = 100;

			timerProbe.Interval = settings.ProbeInterval;

			if (settings.ContinuousProbe)
			{
				timerProbe.Enabled = true;
				timerProbe.Start();
				buttonProbe.Enabled = false;
			}
			else
			{
				timerProbe.Stop();
				timerProbe.Enabled = false;
				buttonProbe.Enabled = connected;
			}

			for (int i = 0; i < pointLists.Length; ++i)
			{
				zedGraphControl.GraphPane.CurveList[i].IsVisible = settings.Echos[i];
				zedGraphControl.GraphPane.CurveList[i].Label.IsVisible = settings.Echos[i];
			}

			zedGraphControl.AxisChange();
			zedGraphControl.Invalidate();
		}

		private void timerProbe_Tick(object sender, EventArgs e)
		{
			buttonProbe_Click(null, null);
		}

		private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
		{
			if (connected)
				Disconnect();

			XmlSerializer xs = new XmlSerializer(typeof(MySettings));
			TextWriter tw = new StreamWriter("settings.xml");
			xs.Serialize(tw, settings);
			tw.Close();
		}

		private void Disconnect()
		{
			port.Write("d");
			port.Close();
			port.Dispose();
			port = null;
			connected = false;
			buttonConnection.Text = "Connect";
			buttonProbe.Enabled = false;
		}

		private MySettings settings = new MySettings();
		private SerialPort port = null;
		private bool connected = false;
		private RollingPointPairList[] pointLists = new RollingPointPairList[17];
		private int currentProbe = 0;
	}
}
