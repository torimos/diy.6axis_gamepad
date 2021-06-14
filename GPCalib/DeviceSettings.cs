using HidLibrary;
using System;
using System.IO;
using System.Windows.Forms;

namespace GPCalib
{
    public partial class DeviceSettings : Form
    {
        const int PARAMS_REP_ID = 0x2;
        private readonly HidDevice device;

        public Settings settings { get; set; }

        public DeviceSettings(HidLibrary.HidDevice device)
        {
            InitializeComponent();
            this.device = device;
        }

        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);
            BindToSettings();
        }

        private void BindToSettings()
        {
            cbEnableCalib.Checked = settings.calib_enabled;
            cbEnableUart.Checked = settings.uart_adapter_enabled;

            nLeftMinX.Value = settings.calib_left.xmin;
            nLeftMaxX.Value = settings.calib_left.xmax;
            nLeftMinY.Value = settings.calib_left.ymin;
            nLeftMaxY.Value = settings.calib_left.ymax;
            nLeftOffsetX.Value = settings.calib_left.xoffs;
            nLeftOffsetY.Value = settings.calib_left.yoffs;
            nLeftScaleTo.Value = settings.calib_left.scaleTo;

            nRightMinX.Value = settings.calib_right.xmin;
            nRightMaxX.Value = settings.calib_right.xmax;
            nRightMinY.Value = settings.calib_right.ymin;
            nRightMaxY.Value = settings.calib_right.ymax;
            nRightOffsetX.Value = settings.calib_right.xoffs;
            nRightOffsetY.Value = settings.calib_right.yoffs;
            nRightScaleTo.Value = settings.calib_right.scaleTo;
        }

        private void SerializeFromUI()
        {
            settings.calib_enabled = cbEnableCalib.Checked;
            settings.uart_adapter_enabled = cbEnableUart.Checked;

            settings.calib_left.xmin = (short)nLeftMinX.Value;
            settings.calib_left.xmax = (short)nLeftMaxX.Value;
            settings.calib_left.ymin = (short)nLeftMinY.Value;
            settings.calib_left.ymax = (short)nLeftMaxY.Value;
            settings.calib_left.xoffs = (short)nLeftOffsetX.Value;
            settings.calib_left.yoffs = (short)nLeftOffsetY.Value;
            settings.calib_left.scaleTo = (short)nLeftScaleTo.Value;

            settings.calib_right.xmin = (short)nRightMinX.Value;
            settings.calib_right.xmax = (short)nRightMaxX.Value;
            settings.calib_right.ymin = (short)nRightMinY.Value;
            settings.calib_right.ymax = (short)nRightMaxY.Value;
            settings.calib_right.xoffs = (short)nRightOffsetX.Value;
            settings.calib_right.yoffs = (short)nRightOffsetY.Value;
            settings.calib_right.scaleTo = (short)nRightScaleTo.Value;
        }

        private void btnRead_Click(object sender, EventArgs e)
        {
            this.Enabled = false;
            var report = device.ReadReportSync(PARAMS_REP_ID);
            if (report.Exists)
            {
                var br = new BinaryReader(new MemoryStream(report.Data));
                settings.calib_enabled = br.ReadBoolean();
                settings.uart_adapter_enabled = br.ReadBoolean();

                settings.calib_left.xmin = br.ReadInt16();
                settings.calib_left.xmax = br.ReadInt16();
                settings.calib_left.ymin = br.ReadInt16();
                settings.calib_left.ymax = br.ReadInt16();
                settings.calib_left.xoffs = br.ReadInt16();
                settings.calib_left.yoffs = br.ReadInt16();
                settings.calib_left.scaleTo = br.ReadInt16();

                settings.calib_right.xmin = br.ReadInt16();
                settings.calib_right.xmax = br.ReadInt16();
                settings.calib_right.ymin = br.ReadInt16();
                settings.calib_right.ymax = br.ReadInt16();
                settings.calib_right.xoffs = br.ReadInt16();
                settings.calib_right.yoffs = br.ReadInt16();
                settings.calib_right.scaleTo = br.ReadInt16();

                BindToSettings();
            }
            this.Enabled = true;
        }

        private void btnWrite_Click(object sender, EventArgs e)
        {
            this.Enabled = false;
            DialogResult r = MessageBox.Show("Are you sure want to update device settings?", "Device Settings", MessageBoxButtons.YesNo);
            if (r == DialogResult.Yes)
            {
                SerializeFromUI();
                var data = settings.ToReport();
                var rep = new HidReport(data.Length);
                rep.ReportId = PARAMS_REP_ID;
                rep.Data = data;
                device.WriteReport(rep);
            }
            this.Enabled = true;
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            SerializeFromUI();
            settings.Save();
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }
    }
}
