using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace TFTP.WinFormApp
{
    internal static class DllImportFuncs
    {
        internal static int Port;
        [DllImport("TFTP.Win.dll", CharSet = CharSet.Ansi)]
        internal extern static int main(int argc, string[] argv);
        internal static Task StartServerAsync(int port = 69)
        {
            Port = port;
            return Task.Run(() =>
            {
                var serverParam = $"tftp tftp -s port ({port})";
                var argv = serverParam.Split(' ');
                Task.Run(() =>
                {
                    var i = main(argv.Length, argv);
                });
            });
        }
        internal static void Read(string filePath)
        {
            var serverParam = $"tftp tftp -c host (127.0.0.1) port ({Port}) -proto (udp) -read (./{filePath})";
            var argv = serverParam.Split(' ');
            var i = main(argv.Length, argv);
        }
        internal static void Write(string filePath)
        {
            var serverParam = $"tftp -c host (127.0.0.1) port ({Port}) -proto (udp) -write (./{filePath})";
            var argv = serverParam.Split(' ');
            var i = main(argv.Length, argv);
        }
    }
}
