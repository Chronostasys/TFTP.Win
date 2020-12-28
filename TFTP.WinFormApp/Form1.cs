using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TFTP.WinFormApp
{
    public partial class Form1 : Form
    {
        private string prevDir;
        public Form1()
        {
            prevDir = Directory.GetCurrentDirectory();
            InitializeComponent();
        }

        private void folderBrowserDialog1_HelpRequest(object sender, EventArgs e)
        {

        }
        protected override void OnFormClosing(FormClosingEventArgs e)
        {
            try
            {
                cmdc.Kill(true);
            }
            catch (Exception)
            {
            }
            base.OnFormClosing(e);
        }
        protected override void OnClosed(EventArgs e)
        {
            try
            {
                cmdc.Kill(true);
            }
            catch (Exception)
            {
            }
            base.OnClosed(e);
        }
        Process cmdc = new Process();
        volatile bool processing = false;

        private void button3_Click(object sender, EventArgs e)
        {
            if (processing)
            {
                MessageBox.Show("Still processing last request! Please wait", "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            if (!checkBox1.Checked&&!File.Exists(Path.Combine(textBox3.Text, textBox2.Text)))
            {
                MessageBox.Show("File not exist!", "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            processing = true;
            cmdc.StartInfo.FileName = "cmd.exe";
            cmdc.StartInfo.RedirectStandardInput = true;
            cmdc.StartInfo.RedirectStandardOutput = true;
            cmdc.StartInfo.CreateNoWindow = true;
            cmdc.StartInfo.UseShellExecute = false;
            var op = checkBox1.Checked ? "read" : "write";
            cmdc.StartInfo.Arguments = "";
            cmdc.Start();
            var encode = checkBox2.Checked ? "octet" : "netascii";
            cmdc.StandardInput.WriteLine($"cd \"{textBox3.Text}\" & {Path.Combine(prevDir, "TFTP.Win.exe")} tftp -c host \"({textBox1.Text})\" port \"({(int)numericUpDown2.Value})\" -proto \"(udp)\" -{op} \"(./{textBox2.Text})\" -encode {encode} & exit");
            cmdc.StandardInput.Flush();

            Task.Run(() =>
            {
                cmdc.WaitForExit();
                var outs = cmdc.StandardOutput.ReadToEnd().Split('\n');
                if (outs[outs.Length - 3].ToUpper().Contains("ERROR"))
                {
                    MessageBox.Show(outs[outs.Length - 3],"ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
                else
                    MessageBox.Show(outs[outs.Length-2],"SPEED");
                processing = false;
            });
        }

        private void button4_Click(object sender, EventArgs e)
        {
            var re = folderBrowserDialog1.ShowDialog();
            textBox3.Text = folderBrowserDialog1.SelectedPath;
            if (!string.IsNullOrEmpty(textBox3.Text))
            {
                Directory.SetCurrentDirectory(textBox3.Text);
                textBox2.Enabled = true;
                button3.Enabled = true;
            }
        }

        private void textBox3_TextChanged(object sender, EventArgs e)
        {

        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }
    }
}
