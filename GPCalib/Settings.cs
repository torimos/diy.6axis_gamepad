using System.Drawing;
using System.IO;

namespace GPCalib
{
    public class StickCalib
    {
        public short xmin, xmax, ymin, ymax, xoffs, yoffs, scaleTo;

        public void Update(short x, short y)
        {
            xoffs = (short)(x - 512);
            yoffs = (short)(y - 512);
        }

        public void Reset()
        {
            xoffs = yoffs = 0;
            xmin = xmax = ymin = ymax = (short)(512);
        }

        public override string ToString()
        {
            return $"Xz={xoffs} Yz={yoffs} XM=[{xmin},{xmax}] YM=[{ymin},{ymax}] ScaleTo={scaleTo}";
        }

        public void Draw(Graphics gfx, Font fnt, int x, int y)
        {
            gfx.DrawString(ToString(), fnt, Brushes.Green, x, y);
        }

        public void Apply(ref short x, ref short y)
        {
            ushort scaleFrom = 1024;
            short m2 = (short)(scaleFrom / 2);
            short mTo2 = (short)(scaleTo / 2);
            short cx = (short)(m2 + xoffs);
            short cy = (short)(m2 + yoffs);

            double scaleFactor = (scaleTo / (double)scaleFrom);
            double dXmin = m2 / (double)(cx - xmin);
            double dXmax = m2 / (double)(xmax - cx);
            double dYmin = m2 / (double)(cy - ymin);
            double dYmax = m2 / (double)(ymax - cy);

            x -= cx;
            y -= cy;

            if (x < 0) x = (short)((x) * dXmin);
            if (x >= 0) x = (short)((x) * dXmax);

            if (y < 0) y = (short)((y) * dYmin);
            if (y >= 0) y = (short)((y) * dYmax);

            x = (short)(x * scaleFactor);
            y = (short)(y * scaleFactor);

            if (x >= mTo2) x = (short)(mTo2 - 1);
            if (x < -mTo2) x = (short)(-mTo2);

            if (y >= mTo2) y = (short)(mTo2 - 1);
            if (y < -mTo2) y = (short)(-mTo2);
        }

        internal void Copy(StickCalib v)
        {
            xoffs = v.xoffs;
            yoffs = v.yoffs;
            xmin = v.xmin;
            xmax = v.xmax;
            ymin = v.ymin;
            ymax = v.ymax;
            scaleTo = v.scaleTo;
        }

        internal byte[] ToArray()
        {
            var ms = new MemoryStream();
            BinaryWriter bw = new BinaryWriter(ms);

            bw.Write(xmin);
            bw.Write(xmax);
            bw.Write(ymin);
            bw.Write(ymax);
            bw.Write(xoffs);
            bw.Write(yoffs);
            bw.Write(scaleTo);

            return ms.ToArray();
        }
    }

    public class Settings
    {
        public bool calib_enabled;
        public bool uart_adapter_enabled;
        public StickCalib calib_left;
        public StickCalib calib_right;

        public Settings()
        {
            calib_left = new StickCalib();
            calib_right = new StickCalib();
        }

        public void Reset()
        {
            calib_left.Reset();
            calib_right.Reset();
        }

        public void Load(string file = "dev_settings")
        {
            var fname = $"{file}.json";
            if (!File.Exists(fname)) return;
            var json = File.ReadAllText(fname);
            var s = Newtonsoft.Json.JsonConvert.DeserializeObject<Settings>(json);
            calib_enabled = s.calib_enabled;
            uart_adapter_enabled = s.uart_adapter_enabled;
            calib_left.Copy(s.calib_left);
            calib_right.Copy(s.calib_right);
        }

        public void Save(string file = "dev_settings")
        {
            var json = Newtonsoft.Json.JsonConvert.SerializeObject(this, Newtonsoft.Json.Formatting.Indented);
            File.WriteAllText($"{file}.json", json);
        }

        private byte[] ToArray()
        {
            var ms = new MemoryStream();
            BinaryWriter bw = new BinaryWriter(ms);
            bw.Write(calib_enabled);
            bw.Write(uart_adapter_enabled);
            bw.Write(calib_left.ToArray());
            bw.Write(calib_right.ToArray());
            return ms.ToArray();
        }

        public byte[] ToReport()
        {
            var ms = new MemoryStream();
            BinaryWriter bw = new BinaryWriter(ms);
            var data = ToArray();
            bw.Write((byte)data.Length);
            bw.Write(data);
            var toRep = ms.ToArray();
            return toRep;
        }

        public Settings Clone()
        {
            var s = new Settings();
            s.calib_enabled = calib_enabled;
            s.uart_adapter_enabled = uart_adapter_enabled;
            s.calib_left.Copy(calib_left);
            s.calib_right.Copy(calib_right);

            return s;
        }
    }
}
