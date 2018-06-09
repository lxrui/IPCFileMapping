using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Runtime.InteropServices;

namespace CSharpFMClientTest
{

    class Program
    {
        [DllImport("IPC_FileMapping.dll")]
        private static extern void createFileMappingServer(string mappingFileName);
        [DllImport("IPC_FileMapping.dll")]
        private static extern void destroyFileMappingServer();
        [DllImport("IPC_FileMapping.dll")]
        private static extern void sendCmd(UInt64 nCmdSize, Byte[] cmdArray);
        static void Main(string[] args)
        {
            createFileMappingServer("Local\\WWJIPCFileMappingObject");
            Byte[] B = new Byte[32];
            Byte uB = 0;
            while (true)
            {
                if (Console.KeyAvailable)
                {
                    ConsoleKeyInfo key = Console.ReadKey(true);
                    if (key.Key == ConsoleKey.Escape) break;
                }
                for (int i = 0; i < 32; ++i)
                {
                    if (0 == i)
                    {
                        B[i] = 0xaa;
                    }
                    else if (31 == i)
                    {
                        B[i] = 0xee;
                    }
                    else
                    {
                        B[i] = uB;
                    }
                }
                sendCmd(32, B);
                Console.WriteLine(BitConverter.ToString(B));
                ++uB;
                Thread.Sleep(15);
            }

            destroyFileMappingServer();
        }
    }
}
