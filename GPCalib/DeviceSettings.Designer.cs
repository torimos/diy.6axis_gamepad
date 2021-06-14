
namespace GPCalib
{
    partial class DeviceSettings
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
            this.cbEnableCalib = new System.Windows.Forms.CheckBox();
            this.cbEnableUart = new System.Windows.Forms.CheckBox();
            this.btnSave = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.gbToggles = new System.Windows.Forms.GroupBox();
            this.btnRead = new System.Windows.Forms.Button();
            this.btnWrite = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.nLeftOffsetY = new System.Windows.Forms.NumericUpDown();
            this.nLeftOffsetX = new System.Windows.Forms.NumericUpDown();
            this.label21 = new System.Windows.Forms.Label();
            this.label22 = new System.Windows.Forms.Label();
            this.nLeftMaxY = new System.Windows.Forms.NumericUpDown();
            this.nLeftMinY = new System.Windows.Forms.NumericUpDown();
            this.label23 = new System.Windows.Forms.Label();
            this.label24 = new System.Windows.Forms.Label();
            this.nLeftMaxX = new System.Windows.Forms.NumericUpDown();
            this.nLeftMinX = new System.Windows.Forms.NumericUpDown();
            this.label25 = new System.Windows.Forms.Label();
            this.label26 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.nRightOffsetY = new System.Windows.Forms.NumericUpDown();
            this.label8 = new System.Windows.Forms.Label();
            this.nRightOffsetX = new System.Windows.Forms.NumericUpDown();
            this.label7 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.nRightMinX = new System.Windows.Forms.NumericUpDown();
            this.label4 = new System.Windows.Forms.Label();
            this.nRightMaxX = new System.Windows.Forms.NumericUpDown();
            this.nRightMaxY = new System.Windows.Forms.NumericUpDown();
            this.label6 = new System.Windows.Forms.Label();
            this.nRightMinY = new System.Windows.Forms.NumericUpDown();
            this.label5 = new System.Windows.Forms.Label();
            this.nLeftScaleTo = new System.Windows.Forms.NumericUpDown();
            this.label1 = new System.Windows.Forms.Label();
            this.nRightScaleTo = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.gbToggles.SuspendLayout();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nLeftOffsetY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nLeftOffsetX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nLeftMaxY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nLeftMinY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nLeftMaxX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nLeftMinX)).BeginInit();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nRightOffsetY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nRightOffsetX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nRightMinX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nRightMaxX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nRightMaxY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nRightMinY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nLeftScaleTo)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nRightScaleTo)).BeginInit();
            this.SuspendLayout();
            // 
            // cbEnableCalib
            // 
            this.cbEnableCalib.AutoSize = true;
            this.cbEnableCalib.Location = new System.Drawing.Point(5, 22);
            this.cbEnableCalib.Name = "cbEnableCalib";
            this.cbEnableCalib.Size = new System.Drawing.Size(110, 17);
            this.cbEnableCalib.TabIndex = 18;
            this.cbEnableCalib.Text = "Enable calibration";
            this.cbEnableCalib.UseVisualStyleBackColor = true;
            // 
            // cbEnableUart
            // 
            this.cbEnableUart.AutoSize = true;
            this.cbEnableUart.Location = new System.Drawing.Point(5, 45);
            this.cbEnableUart.Name = "cbEnableUart";
            this.cbEnableUart.Size = new System.Drawing.Size(135, 17);
            this.cbEnableUart.TabIndex = 19;
            this.cbEnableUart.Text = "Enabler UART Adapter";
            this.cbEnableUart.UseVisualStyleBackColor = true;
            // 
            // btnSave
            // 
            this.btnSave.Location = new System.Drawing.Point(336, 220);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(120, 23);
            this.btnSave.TabIndex = 1;
            this.btnSave.Text = "Save";
            this.btnSave.UseVisualStyleBackColor = true;
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.Location = new System.Drawing.Point(336, 249);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(120, 23);
            this.btnCancel.TabIndex = 2;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // gbToggles
            // 
            this.gbToggles.Controls.Add(this.cbEnableCalib);
            this.gbToggles.Controls.Add(this.cbEnableUart);
            this.gbToggles.Location = new System.Drawing.Point(298, 12);
            this.gbToggles.Name = "gbToggles";
            this.gbToggles.Size = new System.Drawing.Size(158, 67);
            this.gbToggles.TabIndex = 5;
            this.gbToggles.TabStop = false;
            this.gbToggles.Text = "Toggles";
            // 
            // btnRead
            // 
            this.btnRead.Location = new System.Drawing.Point(336, 162);
            this.btnRead.Name = "btnRead";
            this.btnRead.Size = new System.Drawing.Size(120, 23);
            this.btnRead.TabIndex = 20;
            this.btnRead.Text = "Read from Device";
            this.btnRead.UseVisualStyleBackColor = true;
            this.btnRead.Click += new System.EventHandler(this.btnRead_Click);
            // 
            // btnWrite
            // 
            this.btnWrite.Location = new System.Drawing.Point(336, 191);
            this.btnWrite.Name = "btnWrite";
            this.btnWrite.Size = new System.Drawing.Size(120, 23);
            this.btnWrite.TabIndex = 21;
            this.btnWrite.Text = "Write to Device";
            this.btnWrite.UseVisualStyleBackColor = true;
            this.btnWrite.Click += new System.EventHandler(this.btnWrite_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.nLeftScaleTo);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.nLeftOffsetY);
            this.groupBox1.Controls.Add(this.nLeftOffsetX);
            this.groupBox1.Controls.Add(this.label21);
            this.groupBox1.Controls.Add(this.label22);
            this.groupBox1.Controls.Add(this.nLeftMaxY);
            this.groupBox1.Controls.Add(this.nLeftMinY);
            this.groupBox1.Controls.Add(this.label23);
            this.groupBox1.Controls.Add(this.label24);
            this.groupBox1.Controls.Add(this.nLeftMaxX);
            this.groupBox1.Controls.Add(this.nLeftMinX);
            this.groupBox1.Controls.Add(this.label25);
            this.groupBox1.Controls.Add(this.label26);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(280, 133);
            this.groupBox1.TabIndex = 30;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Left Stick Parameters";
            // 
            // nLeftOffsetY
            // 
            this.nLeftOffsetY.Location = new System.Drawing.Point(192, 75);
            this.nLeftOffsetY.Maximum = new decimal(new int[] {
            32768,
            0,
            0,
            0});
            this.nLeftOffsetY.Minimum = new decimal(new int[] {
            32768,
            0,
            0,
            -2147483648});
            this.nLeftOffsetY.Name = "nLeftOffsetY";
            this.nLeftOffsetY.Size = new System.Drawing.Size(82, 20);
            this.nLeftOffsetY.TabIndex = 10;
            // 
            // nLeftOffsetX
            // 
            this.nLeftOffsetX.Location = new System.Drawing.Point(70, 75);
            this.nLeftOffsetX.Maximum = new decimal(new int[] {
            32768,
            0,
            0,
            0});
            this.nLeftOffsetX.Minimum = new decimal(new int[] {
            32768,
            0,
            0,
            -2147483648});
            this.nLeftOffsetX.Name = "nLeftOffsetX";
            this.nLeftOffsetX.Size = new System.Drawing.Size(77, 20);
            this.nLeftOffsetX.TabIndex = 9;
            // 
            // label21
            // 
            this.label21.AutoSize = true;
            this.label21.Location = new System.Drawing.Point(153, 78);
            this.label21.Name = "label21";
            this.label21.Size = new System.Drawing.Size(40, 13);
            this.label21.TabIndex = 15;
            this.label21.Text = "offsetY";
            // 
            // label22
            // 
            this.label22.AutoSize = true;
            this.label22.Location = new System.Drawing.Point(7, 77);
            this.label22.Name = "label22";
            this.label22.Size = new System.Drawing.Size(40, 13);
            this.label22.TabIndex = 14;
            this.label22.Text = "offsetX";
            // 
            // nLeftMaxY
            // 
            this.nLeftMaxY.Location = new System.Drawing.Point(192, 47);
            this.nLeftMaxY.Maximum = new decimal(new int[] {
            32768,
            0,
            0,
            0});
            this.nLeftMaxY.Minimum = new decimal(new int[] {
            32768,
            0,
            0,
            -2147483648});
            this.nLeftMaxY.Name = "nLeftMaxY";
            this.nLeftMaxY.Size = new System.Drawing.Size(82, 20);
            this.nLeftMaxY.TabIndex = 8;
            // 
            // nLeftMinY
            // 
            this.nLeftMinY.Location = new System.Drawing.Point(70, 47);
            this.nLeftMinY.Maximum = new decimal(new int[] {
            32768,
            0,
            0,
            0});
            this.nLeftMinY.Minimum = new decimal(new int[] {
            32768,
            0,
            0,
            -2147483648});
            this.nLeftMinY.Name = "nLeftMinY";
            this.nLeftMinY.Size = new System.Drawing.Size(77, 20);
            this.nLeftMinY.TabIndex = 7;
            // 
            // label23
            // 
            this.label23.AutoSize = true;
            this.label23.Location = new System.Drawing.Point(153, 50);
            this.label23.Name = "label23";
            this.label23.Size = new System.Drawing.Size(33, 13);
            this.label23.TabIndex = 11;
            this.label23.Text = "maxY";
            // 
            // label24
            // 
            this.label24.AutoSize = true;
            this.label24.Location = new System.Drawing.Point(7, 49);
            this.label24.Name = "label24";
            this.label24.Size = new System.Drawing.Size(30, 13);
            this.label24.TabIndex = 10;
            this.label24.Text = "minY";
            // 
            // nLeftMaxX
            // 
            this.nLeftMaxX.Location = new System.Drawing.Point(192, 19);
            this.nLeftMaxX.Maximum = new decimal(new int[] {
            32768,
            0,
            0,
            0});
            this.nLeftMaxX.Minimum = new decimal(new int[] {
            32768,
            0,
            0,
            -2147483648});
            this.nLeftMaxX.Name = "nLeftMaxX";
            this.nLeftMaxX.Size = new System.Drawing.Size(82, 20);
            this.nLeftMaxX.TabIndex = 6;
            // 
            // nLeftMinX
            // 
            this.nLeftMinX.Location = new System.Drawing.Point(70, 19);
            this.nLeftMinX.Maximum = new decimal(new int[] {
            32768,
            0,
            0,
            0});
            this.nLeftMinX.Minimum = new decimal(new int[] {
            32768,
            0,
            0,
            -2147483648});
            this.nLeftMinX.Name = "nLeftMinX";
            this.nLeftMinX.Size = new System.Drawing.Size(77, 20);
            this.nLeftMinX.TabIndex = 5;
            // 
            // label25
            // 
            this.label25.AutoSize = true;
            this.label25.Location = new System.Drawing.Point(153, 22);
            this.label25.Name = "label25";
            this.label25.Size = new System.Drawing.Size(33, 13);
            this.label25.TabIndex = 7;
            this.label25.Text = "maxX";
            // 
            // label26
            // 
            this.label26.AutoSize = true;
            this.label26.Location = new System.Drawing.Point(7, 21);
            this.label26.Name = "label26";
            this.label26.Size = new System.Drawing.Size(30, 13);
            this.label26.TabIndex = 6;
            this.label26.Text = "minX";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.nRightScaleTo);
            this.groupBox2.Controls.Add(this.nRightOffsetY);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Controls.Add(this.label8);
            this.groupBox2.Controls.Add(this.nRightOffsetX);
            this.groupBox2.Controls.Add(this.label7);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.nRightMinX);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.nRightMaxX);
            this.groupBox2.Controls.Add(this.nRightMaxY);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Controls.Add(this.nRightMinY);
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Location = new System.Drawing.Point(12, 151);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(280, 134);
            this.groupBox2.TabIndex = 31;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "RightStick Parameters";
            // 
            // nRightOffsetY
            // 
            this.nRightOffsetY.Location = new System.Drawing.Point(192, 77);
            this.nRightOffsetY.Maximum = new decimal(new int[] {
            32768,
            0,
            0,
            0});
            this.nRightOffsetY.Minimum = new decimal(new int[] {
            32768,
            0,
            0,
            -2147483648});
            this.nRightOffsetY.Name = "nRightOffsetY";
            this.nRightOffsetY.Size = new System.Drawing.Size(82, 20);
            this.nRightOffsetY.TabIndex = 16;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(7, 23);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(30, 13);
            this.label8.TabIndex = 18;
            this.label8.Text = "minX";
            // 
            // nRightOffsetX
            // 
            this.nRightOffsetX.Location = new System.Drawing.Point(70, 77);
            this.nRightOffsetX.Maximum = new decimal(new int[] {
            32768,
            0,
            0,
            0});
            this.nRightOffsetX.Minimum = new decimal(new int[] {
            32768,
            0,
            0,
            -2147483648});
            this.nRightOffsetX.Name = "nRightOffsetX";
            this.nRightOffsetX.Size = new System.Drawing.Size(77, 20);
            this.nRightOffsetX.TabIndex = 15;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(153, 24);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(33, 13);
            this.label7.TabIndex = 19;
            this.label7.Text = "maxX";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(153, 80);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(40, 13);
            this.label3.TabIndex = 27;
            this.label3.Text = "offsetY";
            // 
            // nRightMinX
            // 
            this.nRightMinX.Location = new System.Drawing.Point(70, 21);
            this.nRightMinX.Maximum = new decimal(new int[] {
            32768,
            0,
            0,
            0});
            this.nRightMinX.Minimum = new decimal(new int[] {
            32768,
            0,
            0,
            -2147483648});
            this.nRightMinX.Name = "nRightMinX";
            this.nRightMinX.Size = new System.Drawing.Size(77, 20);
            this.nRightMinX.TabIndex = 11;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(7, 79);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(40, 13);
            this.label4.TabIndex = 26;
            this.label4.Text = "offsetX";
            // 
            // nRightMaxX
            // 
            this.nRightMaxX.Location = new System.Drawing.Point(192, 21);
            this.nRightMaxX.Maximum = new decimal(new int[] {
            32768,
            0,
            0,
            0});
            this.nRightMaxX.Minimum = new decimal(new int[] {
            32768,
            0,
            0,
            -2147483648});
            this.nRightMaxX.Name = "nRightMaxX";
            this.nRightMaxX.Size = new System.Drawing.Size(82, 20);
            this.nRightMaxX.TabIndex = 12;
            // 
            // nRightMaxY
            // 
            this.nRightMaxY.Location = new System.Drawing.Point(192, 49);
            this.nRightMaxY.Maximum = new decimal(new int[] {
            32768,
            0,
            0,
            0});
            this.nRightMaxY.Minimum = new decimal(new int[] {
            32768,
            0,
            0,
            -2147483648});
            this.nRightMaxY.Name = "nRightMaxY";
            this.nRightMaxY.Size = new System.Drawing.Size(82, 20);
            this.nRightMaxY.TabIndex = 14;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(7, 51);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(30, 13);
            this.label6.TabIndex = 22;
            this.label6.Text = "minY";
            // 
            // nRightMinY
            // 
            this.nRightMinY.Location = new System.Drawing.Point(70, 49);
            this.nRightMinY.Maximum = new decimal(new int[] {
            32768,
            0,
            0,
            0});
            this.nRightMinY.Minimum = new decimal(new int[] {
            32768,
            0,
            0,
            -2147483648});
            this.nRightMinY.Name = "nRightMinY";
            this.nRightMinY.Size = new System.Drawing.Size(77, 20);
            this.nRightMinY.TabIndex = 13;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(153, 52);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(33, 13);
            this.label5.TabIndex = 23;
            this.label5.Text = "maxY";
            // 
            // nLeftScaleTo
            // 
            this.nLeftScaleTo.Location = new System.Drawing.Point(70, 105);
            this.nLeftScaleTo.Maximum = new decimal(new int[] {
            32768,
            0,
            0,
            0});
            this.nLeftScaleTo.Minimum = new decimal(new int[] {
            32768,
            0,
            0,
            -2147483648});
            this.nLeftScaleTo.Name = "nLeftScaleTo";
            this.nLeftScaleTo.Size = new System.Drawing.Size(77, 20);
            this.nLeftScaleTo.TabIndex = 17;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 107);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(44, 13);
            this.label1.TabIndex = 16;
            this.label1.Text = "scale to";
            // 
            // nRightScaleTo
            // 
            this.nRightScaleTo.Location = new System.Drawing.Point(70, 106);
            this.nRightScaleTo.Maximum = new decimal(new int[] {
            32768,
            0,
            0,
            0});
            this.nRightScaleTo.Minimum = new decimal(new int[] {
            32768,
            0,
            0,
            -2147483648});
            this.nRightScaleTo.Name = "nRightScaleTo";
            this.nRightScaleTo.Size = new System.Drawing.Size(77, 20);
            this.nRightScaleTo.TabIndex = 19;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 108);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(44, 13);
            this.label2.TabIndex = 18;
            this.label2.Text = "scale to";
            // 
            // DeviceSettings
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(468, 289);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.btnWrite);
            this.Controls.Add(this.btnRead);
            this.Controls.Add(this.gbToggles);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnSave);
            this.Name = "DeviceSettings";
            this.Text = "DeviceSettings";
            this.gbToggles.ResumeLayout(false);
            this.gbToggles.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nLeftOffsetY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nLeftOffsetX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nLeftMaxY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nLeftMinY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nLeftMaxX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nLeftMinX)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nRightOffsetY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nRightOffsetX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nRightMinX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nRightMaxX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nRightMaxY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nRightMinY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nLeftScaleTo)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nRightScaleTo)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.CheckBox cbEnableCalib;
        private System.Windows.Forms.CheckBox cbEnableUart;
        private System.Windows.Forms.Button btnSave;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.GroupBox gbToggles;
        private System.Windows.Forms.Button btnRead;
        private System.Windows.Forms.Button btnWrite;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.NumericUpDown nLeftOffsetY;
        private System.Windows.Forms.NumericUpDown nLeftOffsetX;
        private System.Windows.Forms.Label label21;
        private System.Windows.Forms.Label label22;
        private System.Windows.Forms.NumericUpDown nLeftMaxY;
        private System.Windows.Forms.NumericUpDown nLeftMinY;
        private System.Windows.Forms.Label label23;
        private System.Windows.Forms.Label label24;
        private System.Windows.Forms.NumericUpDown nLeftMaxX;
        private System.Windows.Forms.NumericUpDown nLeftMinX;
        private System.Windows.Forms.Label label25;
        private System.Windows.Forms.Label label26;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.NumericUpDown nRightOffsetY;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.NumericUpDown nRightOffsetX;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.NumericUpDown nRightMinX;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.NumericUpDown nRightMaxX;
        private System.Windows.Forms.NumericUpDown nRightMaxY;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.NumericUpDown nRightMinY;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.NumericUpDown nLeftScaleTo;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown nRightScaleTo;
        private System.Windows.Forms.Label label2;
    }
}