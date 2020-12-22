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
                numericUpDown1.Enabled = true;
                button2.Enabled = true;
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Process cmd = new Process();
            cmd.StartInfo.FileName = "cmd.exe";
            cmd.StartInfo.RedirectStandardInput = true;
            cmd.StartInfo.RedirectStandardOutput = true;
            cmd.StartInfo.CreateNoWindow = true;
            cmd.StartInfo.UseShellExecute = false;
            cmd.Start();

            cmd.StandardInput.WriteLine($"cd \"{textBox1.Text}\"");
            cmd.StandardInput.Flush();
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
            DllImportFuncs.Port = (int)numericUpDown2.Value;
            if (checkBox1.Checked)
            {
                DllImportFuncs.Read(textBox2.Text);
            }
            else
            {
                DllImportFuncs.Write(textBox2.Text);
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
