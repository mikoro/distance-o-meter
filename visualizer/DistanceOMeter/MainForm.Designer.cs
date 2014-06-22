namespace DistanceOMeter
{
	partial class MainForm
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.propertyGridSettings = new System.Windows.Forms.PropertyGrid();
            this.groupBoxSettings = new System.Windows.Forms.GroupBox();
            this.buttonConnection = new System.Windows.Forms.Button();
            this.buttonProbe = new System.Windows.Forms.Button();
            this.groupBoxGraph = new System.Windows.Forms.GroupBox();
            this.zedGraphControl = new ZedGraph.ZedGraphControl();
            this.timerKeepalive = new System.Windows.Forms.Timer(this.components);
            this.listBox = new System.Windows.Forms.ListBox();
            this.groupBoxValues = new System.Windows.Forms.GroupBox();
            this.timerProbe = new System.Windows.Forms.Timer(this.components);
            this.groupBoxSettings.SuspendLayout();
            this.groupBoxGraph.SuspendLayout();
            this.groupBoxValues.SuspendLayout();
            this.SuspendLayout();
            // 
            // propertyGridSettings
            // 
            this.propertyGridSettings.Location = new System.Drawing.Point(6, 19);
            this.propertyGridSettings.Name = "propertyGridSettings";
            this.propertyGridSettings.Size = new System.Drawing.Size(218, 308);
            this.propertyGridSettings.TabIndex = 0;
            this.propertyGridSettings.ToolbarVisible = false;
            this.propertyGridSettings.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.propertyGridSettings_PropertyValueChanged);
            // 
            // groupBoxSettings
            // 
            this.groupBoxSettings.Controls.Add(this.propertyGridSettings);
            this.groupBoxSettings.Location = new System.Drawing.Point(12, 12);
            this.groupBoxSettings.Name = "groupBoxSettings";
            this.groupBoxSettings.Size = new System.Drawing.Size(230, 333);
            this.groupBoxSettings.TabIndex = 1;
            this.groupBoxSettings.TabStop = false;
            this.groupBoxSettings.Text = "Settings";
            // 
            // buttonConnection
            // 
            this.buttonConnection.Location = new System.Drawing.Point(12, 351);
            this.buttonConnection.Name = "buttonConnection";
            this.buttonConnection.Size = new System.Drawing.Size(110, 23);
            this.buttonConnection.TabIndex = 2;
            this.buttonConnection.Text = "Connect";
            this.buttonConnection.UseVisualStyleBackColor = true;
            this.buttonConnection.Click += new System.EventHandler(this.buttonConnection_Click);
            // 
            // buttonProbe
            // 
            this.buttonProbe.Enabled = false;
            this.buttonProbe.Location = new System.Drawing.Point(132, 351);
            this.buttonProbe.Name = "buttonProbe";
            this.buttonProbe.Size = new System.Drawing.Size(110, 23);
            this.buttonProbe.TabIndex = 3;
            this.buttonProbe.Text = "Probe";
            this.buttonProbe.UseVisualStyleBackColor = true;
            this.buttonProbe.Click += new System.EventHandler(this.buttonProbe_Click);
            // 
            // groupBoxGraph
            // 
            this.groupBoxGraph.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBoxGraph.Controls.Add(this.zedGraphControl);
            this.groupBoxGraph.Location = new System.Drawing.Point(248, 12);
            this.groupBoxGraph.Name = "groupBoxGraph";
            this.groupBoxGraph.Size = new System.Drawing.Size(712, 625);
            this.groupBoxGraph.TabIndex = 4;
            this.groupBoxGraph.TabStop = false;
            this.groupBoxGraph.Text = "Graph";
            // 
            // zedGraphControl
            // 
            this.zedGraphControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.zedGraphControl.Location = new System.Drawing.Point(6, 19);
            this.zedGraphControl.Name = "zedGraphControl";
            this.zedGraphControl.ScrollGrace = 0D;
            this.zedGraphControl.ScrollMaxX = 0D;
            this.zedGraphControl.ScrollMaxY = 0D;
            this.zedGraphControl.ScrollMaxY2 = 0D;
            this.zedGraphControl.ScrollMinX = 0D;
            this.zedGraphControl.ScrollMinY = 0D;
            this.zedGraphControl.ScrollMinY2 = 0D;
            this.zedGraphControl.Size = new System.Drawing.Size(700, 600);
            this.zedGraphControl.TabIndex = 0;
            // 
            // timerKeepalive
            // 
            this.timerKeepalive.Tick += new System.EventHandler(this.timerKeepalive_Tick);
            // 
            // listBox
            // 
            this.listBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.listBox.FormattingEnabled = true;
            this.listBox.IntegralHeight = false;
            this.listBox.Location = new System.Drawing.Point(6, 19);
            this.listBox.Name = "listBox";
            this.listBox.Size = new System.Drawing.Size(212, 232);
            this.listBox.TabIndex = 6;
            // 
            // groupBoxValues
            // 
            this.groupBoxValues.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.groupBoxValues.Controls.Add(this.listBox);
            this.groupBoxValues.Location = new System.Drawing.Point(12, 380);
            this.groupBoxValues.Name = "groupBoxValues";
            this.groupBoxValues.Size = new System.Drawing.Size(224, 257);
            this.groupBoxValues.TabIndex = 7;
            this.groupBoxValues.TabStop = false;
            this.groupBoxValues.Text = "Raw echo values";
            // 
            // timerProbe
            // 
            this.timerProbe.Tick += new System.EventHandler(this.timerProbe_Tick);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(972, 649);
            this.Controls.Add(this.groupBoxValues);
            this.Controls.Add(this.groupBoxGraph);
            this.Controls.Add(this.buttonProbe);
            this.Controls.Add(this.buttonConnection);
            this.Controls.Add(this.groupBoxSettings);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "MainForm";
            this.Text = " Distance-O-Meter";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.groupBoxSettings.ResumeLayout(false);
            this.groupBoxGraph.ResumeLayout(false);
            this.groupBoxValues.ResumeLayout(false);
            this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.PropertyGrid propertyGridSettings;
		private System.Windows.Forms.GroupBox groupBoxSettings;
		private System.Windows.Forms.Button buttonConnection;
		private System.Windows.Forms.Button buttonProbe;
		private System.Windows.Forms.GroupBox groupBoxGraph;
		private ZedGraph.ZedGraphControl zedGraphControl;
		private System.Windows.Forms.Timer timerKeepalive;
		private System.Windows.Forms.ListBox listBox;
		private System.Windows.Forms.GroupBox groupBoxValues;
		private System.Windows.Forms.Timer timerProbe;
	}
}

