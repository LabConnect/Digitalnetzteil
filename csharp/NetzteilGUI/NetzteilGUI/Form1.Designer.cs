namespace NetzteilGUI
{
    partial class Netzteil
    {
        /// <summary>
        /// Erforderliche Designervariable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Verwendete Ressourcen bereinigen.
        /// </summary>
        /// <param name="disposing">True, wenn verwaltete Ressourcen gelöscht werden sollen; andernfalls False.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Vom Windows Form-Designer generierter Code

        /// <summary>
        /// Erforderliche Methode für die Designerunterstützung.
        /// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.btnOpen = new System.Windows.Forms.Button();
            this.btnClose = new System.Windows.Forms.Button();
            this.numVoltage = new System.Windows.Forms.NumericUpDown();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label2 = new System.Windows.Forms.Label();
            this.numCurrent = new System.Windows.Forms.NumericUpDown();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.txtCurrent = new System.Windows.Forms.MaskedTextBox();
            this.txtVoltage = new System.Windows.Forms.MaskedTextBox();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.numVoltage)).BeginInit();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numCurrent)).BeginInit();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnOpen
            // 
            this.btnOpen.Location = new System.Drawing.Point(12, 12);
            this.btnOpen.Name = "btnOpen";
            this.btnOpen.Size = new System.Drawing.Size(102, 23);
            this.btnOpen.TabIndex = 0;
            this.btnOpen.Text = "Device open";
            this.btnOpen.UseVisualStyleBackColor = true;
            this.btnOpen.Click += new System.EventHandler(this.btnOpen_Click);
            // 
            // btnClose
            // 
            this.btnClose.Enabled = false;
            this.btnClose.Location = new System.Drawing.Point(220, 12);
            this.btnClose.Name = "btnClose";
            this.btnClose.Size = new System.Drawing.Size(102, 23);
            this.btnClose.TabIndex = 1;
            this.btnClose.Text = "Device close";
            this.btnClose.UseVisualStyleBackColor = true;
            this.btnClose.Click += new System.EventHandler(this.btnClose_Click);
            // 
            // numVoltage
            // 
            this.numVoltage.DecimalPlaces = 2;
            this.numVoltage.Enabled = false;
            this.numVoltage.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.numVoltage.Location = new System.Drawing.Point(6, 19);
            this.numVoltage.Maximum = new decimal(new int[] {
            20,
            0,
            0,
            0});
            this.numVoltage.Minimum = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.numVoltage.Name = "numVoltage";
            this.numVoltage.Size = new System.Drawing.Size(102, 20);
            this.numVoltage.TabIndex = 2;
            this.numVoltage.Tag = "";
            this.numVoltage.Value = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.numVoltage.ValueChanged += new System.EventHandler(this.numVoltage_ValueChanged);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.numCurrent);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.numVoltage);
            this.groupBox1.Location = new System.Drawing.Point(142, 41);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(180, 77);
            this.groupBox1.TabIndex = 3;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Soll Werte";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(114, 47);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(59, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "Strom max.";
            // 
            // numCurrent
            // 
            this.numCurrent.DecimalPlaces = 2;
            this.numCurrent.Enabled = false;
            this.numCurrent.Increment = new decimal(new int[] {
            1,
            0,
            0,
            131072});
            this.numCurrent.Location = new System.Drawing.Point(6, 45);
            this.numCurrent.Maximum = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.numCurrent.Name = "numCurrent";
            this.numCurrent.Size = new System.Drawing.Size(102, 20);
            this.numCurrent.TabIndex = 4;
            this.numCurrent.Value = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.numCurrent.ValueChanged += new System.EventHandler(this.numCurrent_ValueChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(114, 21);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(56, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Spannung";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.txtCurrent);
            this.groupBox2.Controls.Add(this.txtVoltage);
            this.groupBox2.Location = new System.Drawing.Point(12, 41);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(124, 77);
            this.groupBox2.TabIndex = 4;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Ist Werte";
            // 
            // txtCurrent
            // 
            this.txtCurrent.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.txtCurrent.Location = new System.Drawing.Point(7, 47);
            this.txtCurrent.Mask = "0,00 \\Ampere";
            this.txtCurrent.Name = "txtCurrent";
            this.txtCurrent.ReadOnly = true;
            this.txtCurrent.Size = new System.Drawing.Size(100, 13);
            this.txtCurrent.TabIndex = 1;
            // 
            // txtVoltage
            // 
            this.txtVoltage.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.txtVoltage.Location = new System.Drawing.Point(7, 20);
            this.txtVoltage.Mask = "00,00 Volt";
            this.txtVoltage.Name = "txtVoltage";
            this.txtVoltage.ReadOnly = true;
            this.txtVoltage.Size = new System.Drawing.Size(100, 13);
            this.txtVoltage.TabIndex = 0;
            // 
            // timer1
            // 
            this.timer1.Interval = 1000;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // Netzteil
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(332, 126);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.btnClose);
            this.Controls.Add(this.btnOpen);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Netzteil";
            this.Text = "Netzteil © Malte Soennichsen";
            ((System.ComponentModel.ISupportInitialize)(this.numVoltage)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numCurrent)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnOpen;
        private System.Windows.Forms.Button btnClose;
        private System.Windows.Forms.NumericUpDown numVoltage;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.NumericUpDown numCurrent;
        private System.Windows.Forms.MaskedTextBox txtCurrent;
        private System.Windows.Forms.MaskedTextBox txtVoltage;
        private System.Windows.Forms.Timer timer1;
    }
}

