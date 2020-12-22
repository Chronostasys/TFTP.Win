using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TFTP.WinFormApp
{
    public partial class Form1 : Form
    {
        private string prevDir;
        public Form1()
        {
            InitializeComponent();
        }

        private void folderBrowserDialog1_HelpRequest(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            var re = folderBrowserDialog1.ShowDialog();
            textBox1.Text = folderBrowserDialog1.SelectedPath;
            if (!string.IsNullOrEmpty(textBox1.Text))
            {
                Directory.SetCurrentDirectory(textBox1.Text);
                numericUpDown1.Enabled = true;
                button2.Enabled = true;
            }
        }
        protected override void OnClosed(EventArgs e)
        {
            cmd.Kill();
            base.OnClosed(e);
        }
        Process cmd = new Process();
        private void button2_Click(object sender, EventArgs e)
        {
            cmd.StartInfo.FileName = "cmd.exe";
            cmd.StartInfo.RedirectStandardInput = true;
            cmd.StartInfo.RedirectStandardOutput = true;
            cmd.StartInfo.CreateNoWindow = true;
            cmd.StartInfo.UseShellExecute = false;
            cmd.Start();

            cmd.StandardInput.WriteLine($"cd \"{textBox1.Text}\"");
            cmd.StandardInput.Flush();
            prevDir = Directory.GetCurrentDirectory();
            cmd.StandardInput.WriteLine($"{Path.Combine(Directory.GetCurrentDirectory(), "TFTP.Win.exe")} tftp -s port \"({(int)numericUpDown1.Value})\"");
            cmd.StandardInput.Flush();
            cmd.StandardInput.Close();
            //DllImportFuncs.StartServerAsync((int)numericUpDown1.Value);
            flowLayoutPanel1.Visible = false;
            MessageBox.Show($"Start server at port {(int)numericUpDown1.Value}");
            flowLayoutPanel2.Visible = true;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            try
            {
                if (checkBox1.Checked)
                    File.Copy(Path.Combine(prevDir, textBox2.Text), textBox2.Text);
                else
                    File.Copy(textBox2.Text, Path.Combine(prevDir, textBox2.Text));
            }
            catch (Exception err)
            {

                MessageBox.Show(err.Message);
            }
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
    }
}
