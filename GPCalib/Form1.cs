using HidLibrary;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GPCalib
{
    public partial class Form1 : Form
    {
        class ReportType {
            public ushort buttons;
            public ushort triggers;
            public List<short> axis;
            public List<short> mag;
            public List<short> accel;
            public List<short> gyro;
            public bool ready;
            public ReportType Copy()
            {
                return (ReportType)this.MemberwiseClone();
            }
        }
        class Item
        {
            public Queue<float> data = new Queue<float>();
            public float min = 0, max = 0;
        }
        HidDevice device;
        ReportType joyReport = new ReportType();
        Font fnt = new Font(FontFamily.GenericMonospace, 9);
        Dictionary<string, Item> buffers = new Dictionary<string, Item>();

        float pitch, roll, heading;
        short rx, ry, rn;
        float rh;
        bool _attached = false;

        public Form1()
        {
            InitializeComponent();
            this.SetStyle(
                ControlStyles.AllPaintingInWmPaint |
                ControlStyles.UserPaint |
                ControlStyles.DoubleBuffer,
                true);


            device = HidDevices.Enumerate(0xE502, 0xBBAB).FirstOrDefault();
            //device = HidDevices.Enumerate(0x0483, 0xBEAF).FirstOrDefault();
            if (device != null)
            {
                device.OpenDevice();
                device.Inserted += DeviceAttachedHandler;
                device.Removed += DeviceRemovedHandler;
                device.ReadReport(OnReport);
            }
        }

        private void DeviceAttachedHandler()
        {
            _attached = true;
            device.ReadReport(OnReport);
        }

        private void DeviceRemovedHandler()
        {
            _attached = false;
        }

        private void OnReport(HidReport report)
        {
            var br = new BinaryReader(new MemoryStream(report.Data));
            var r = new ReportType();
            r.buttons = br.ReadUInt16();
            r.triggers = br.ReadUInt16();
            r.axis = new List<short>();
            r.mag = new List<short>();
            r.accel = new List<short>();
            r.gyro = new List<short>();
            for (int i = 0; i < 4; i++) r.axis.Add(br.ReadInt16());
            for (int i = 0; i < 3; i++) r.mag.Add(br.ReadInt16());
            for (int i = 0; i < 3; i++) r.accel.Add(br.ReadInt16());
            for (int i = 0; i < 3; i++) r.gyro.Add(br.ReadInt16());


            //http://www.phipi.com/pthsrobotics/compass_correct.html
            //file:///D:/Downloads/compass_calib.pdf
            float x_ampl = 210;
            float x_offset = -80;
            float y_ampl = 192;
            float y_offset = -210;
            r.mag[0] = (short)((r.mag[0] - x_offset) * 300.0 / x_ampl);
            r.mag[1] = (short)((r.mag[1] - y_offset) * 300.0 / y_ampl);

            calcHeading(r);
            joyReport = r.Copy();
            joyReport.ready = true;

            device.ReadReport(OnReport);
            this.Invalidate();
        }

        void calcHeading(ReportType rep)
        {
            float accelDiv = 0.5408f;
            // roll: Rotation around the X-axis. -180 <= roll <= 180
            // a positive roll angle is defined to be a clockwise rotation about the
            // positive X-axis
            //
            //                    y
            //      roll = atan2(---)
            //                    z
            //
            // where:  y, z are returned value from accelerometer sensor
            roll = (float)Math.Atan2(rep.accel[1] / accelDiv, rep.accel[2] / accelDiv);

            // pitch: Rotation around the Y-axis. -180 <= roll <= 180
            // a positive pitch angle is defined to be a clockwise rotation about the
            // positive Y-axis
            //
            //                                 -x
            //      pitch = atan(-------------------------------)
            //                    y * sin(roll) + z * cos(roll)
            //
            // where:  x, y, z are returned value from accelerometer sensor
            if (rep.accel[1] * Math.Sin(roll) + rep.accel[2] * Math.Cos(roll) == 0)
                pitch = rep.accel[0] > 0 ? (float)(Math.PI / 2.0f) : (float)(-Math.PI / 2.0f);
            else
                pitch = (float)Math.Atan(-rep.accel[0] / (rep.accel[1] * Math.Sin(roll) + rep.accel[2] * Math.Cos(roll)));

            // heading: Rotation around the Z-axis. -180 <= roll <= 180
            // a positive heading angle is defined to be a clockwise rotation about the
            // positive Z-axis
            //
            //                                        z * sin(roll) - y * cos(roll)
            //   heading = atan2(--------------------------------------------------------------------------)
            //                   x * cos(pitch) + y * sin(pitch) * sin(roll) + z *  sin(pitch) * cos(roll))              
            //
            // where:  x, y, z are returned value from magnetometer sensor
            heading = (float)Math.Atan2(rep.mag[2] * Math.Sin(roll) - rep.mag[1] * Math.Cos(roll),
                             rep.mag[0] * Math.Cos(pitch) + rep.mag[1] * Math.Sin(pitch) * Math.Sin(roll) + rep.mag[2] * Math.Sin(pitch) * Math.Cos(roll));


            // Convert angular data to degree
            roll = roll * 180 / (float)Math.PI - 5;
            pitch = pitch * 180 / (float)Math.PI;
            heading = (heading * 180 / (float)Math.PI)-8.1333f;

            if (heading < 0) heading += 360;
            int z = (360 / 24);
            heading = z * (int)(heading / z);

        }

        void drawAxis(Graphics gfx, Brush br, int x, int y, int cx, int cy, int w, int h, int maxV = 32768)
        {
            var midH = w >> 1;
            var midV = h >> 1;
            var minSize = Math.Min(w, h);
            var mH = maxV / (float)minSize;
            var mV = maxV / (float)minSize;

            gfx.DrawRectangle(new Pen(br), cx, cy, w, h);
            gfx.DrawString($"{x}:{y}", fnt, br, cx+1, cy+1);
            gfx.FillEllipse(br, cx + midH + (x-(maxV >> 1))/mH, cy + midV + (y - (maxV >> 1)) / mV, 10, 10);
        }

        void drawPlot(Graphics gfx, Brush br, string title, float p, int cx, int cy, int w, int h, float scale, bool midZero)
        {
            float mV = (scale/1.0f)/ (float)(h);
            float defExtr = midZero ? 0 : scale / 2.0f;
            if (!buffers.ContainsKey(title)) buffers.Add(title, new Item() { max = defExtr, min = defExtr });
            var buffer = buffers[title].data;

            float v = p;

            buffers[title].max = Math.Max(buffers[title].max, v);
            buffers[title].min = Math.Min(buffers[title].min, v);

            buffer.Enqueue(v);

            if (buffer.Count >= (w - 2)) buffer.Dequeue();
            gfx.DrawRectangle(new Pen(Brushes.Black), cx, cy, w, h);
            gfx.DrawString($"{title}: {v} min:{buffers[title].min} max:{buffers[title].max}", fnt, br, cx + 1, cy - 16);
            int offset = 0;
            float prevX = buffer.Peek();
            int h2 = 0;
            if (midZero)
                h2 = (h>>1);
            foreach (var x in buffer) { 
                gfx.DrawLine(new Pen(br, 2), 
                    cx + w - 1 - offset-1, cy + h2 + prevX / mV, 
                    cx + w - 1 - offset, cy + h2 + x / mV);
                offset++;
                prevX = x;
            }
        }

        void drawRPH(Graphics gfx, bool rawMag = false)
        {
            const int m = 160;
            const int m2 = m + 20;
            int w = 500;
            int cx = 0;
            int cy1 = 30;
            int cy = m2+ cy1;

            drawAxis(gfx, Brushes.Blue, joyReport.axis[0], joyReport.axis[1], 0, cy1, m, m);
            drawAxis(gfx, Brushes.Green, joyReport.axis[3], joyReport.axis[2], m2, cy1, m, m);

            if (!rawMag)
            {
                drawPlot(gfx, Brushes.Red, $"roll", roll, cx, cy + m2 * 0, w, m, 360, true);
                drawPlot(gfx, Brushes.Green, $"pitch", pitch, cx, cy + m2 * 1, w, m, 180, true);
                drawPlot(gfx, Brushes.Blue, $"heading", heading, cx, cy + m2 * 2, w, m, 360, false);
            }
            else
            {
                drawPlot(gfx, Brushes.Red, $"mag_x", joyReport.mag[0] / 1, cx, cy + m2 * 0, w, m, 1000, true);
                drawPlot(gfx, Brushes.Red, $"mag_y", joyReport.mag[1] / 1, cx, cy + m2 * 1, w, m, 1000, true);
                drawPlot(gfx, Brushes.Red, $"mag_z", joyReport.mag[2] / 1, cx, cy + m2 * 2, w, m, 1000, true);
            }

            float hd = m >> 1;
            float hrad = (float)Math.PI * heading / 180.0f;
            PointF pc = new PointF(m2*2.5f, cy1+m2>>1);
            PointF pt = new PointF(pc.X + (float)Math.Sin(hrad) * hd, pc.Y + (float)Math.Cos(hrad) * hd);
            gfx.DrawEllipse(new Pen(Brushes.Gray, 1), pc.X - hd/2 - roll, pc.Y - hd / 2 - pitch, hd, hd);
            for (int i = 0; i < 24; i++)
            {
                float mrad = (float)Math.PI * i * (360 / 24) / 180.0f;
                PointF pm = new PointF(pc.X + (float)Math.Sin(mrad) * hd, pc.Y + (float)Math.Cos(mrad) * hd);
                gfx.DrawLine(new Pen(Brushes.Gray, 1), pc, pm);
            }
            gfx.DrawLine(new Pen(Brushes.Purple, 4), pc, pt);
            gfx.FillEllipse(Brushes.Red, pt.X-5, pt.Y-5, 10, 10);
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            if (joyReport.ready)
            {
                e.Graphics.DrawString($"{joyReport.buttons:X} {joyReport.triggers:X}", fnt, Brushes.Green, 0, 0);
                if (rn > 0) e.Graphics.DrawString($"{rh / rn:F2} {rx / rn} {ry / rn}", fnt, Brushes.Green, 0, 16);

                //var list = joyReport.accel; float scale = 1000; float div = 0.5408f; string title = "accel";
                //var list = joyReport.gyro; float scale = 1000; float div = 14.3750f; string title = "gyro";
                //var list = joyReport.mag; float scale = 1000; float div = 1f; string title = "mag";
                //drawPlot(e.Graphics, Brushes.Red, $"{title}1", list[0] / div, cx, cy + m2 * 0, w, m, scale, true);
                //drawPlot(e.Graphics, Brushes.Red, $"{title}2", list[1] / div, cx, cy + m2 * 1, w, m, scale, true);
                //drawPlot(e.Graphics, Brushes.Red, $"{title}3", list[2] / div, cx, cy + m2 * 2, w, m, scale, true);

                drawRPH(e.Graphics);
            }
        }

        protected override void OnKeyDown(KeyEventArgs e)
        {
            base.OnKeyDown(e);
            if (e.KeyCode == Keys.Space)
            {
                buffers.Clear();
                rx = ry = rn = 0;
            }
            else if (e.KeyCode == Keys.Q)
            {
                rx += joyReport.mag[0];
                ry += joyReport.mag[1];
                rh += heading;
                rn++;
            }
        }
    }
}
