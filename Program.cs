using System;
using System.IO.Ports;
using System.Text;
using System.Threading;

namespace Program
{

    class Program
    {
        static String valor;


        static void Main(string[] args)
        {
            String data;
            String Resultado;
            byte[] bytes = new byte[10] { 0x0,0x0,0x0,0x0, 0x0, 0x0, 0x0, 0x0 , 0x0, 0x0};
            byte[] bytes_R = new byte[5];
            byte crc;

            SerialPort _serialPort = new SerialPort();
            _serialPort.PortName = "COM3";
            _serialPort.BaudRate = 115200;
            _serialPort.DtrEnable = true;
            _serialPort.Open();

            while(true)
            {
                valor = Console.ReadLine();

                Console.CursorTop--;
                Console.CursorLeft = valor.Length;

                if (valor.Equals("exit"))
                {
                    break;
                }
                data = "";
                for(int i=0;i<valor.Length;i++)
                {
                    if(valor[i]!='=')
                    {
                        data = data + valor[i];
                    }
                }
                
                bytes = Encoding.ASCII.GetBytes(data);

                crc = ChecksumCRC8(bytes);

                Array.Resize(ref bytes, 10);
                bytes[9] = crc;
                


                _serialPort.Write(bytes,0,bytes.Length);
                _serialPort.DiscardInBuffer();

                Thread.Sleep(1000);
                _serialPort.Read(bytes_R,0,5);
                
                

                crc = ChecksumCRC8(bytes_R);

                if(crc == 0)
                {
                    bytes_R[4] = 0x0;
                    Resultado = System.Text.Encoding.UTF8.GetString(bytes_R);
                    Console.WriteLine(Resultado+"\n");
                }

            }
            _serialPort.Close();
        }


       
        static byte ChecksumCRC8(byte[] pucY)
        {
            byte preset_value = 0xFF; // 1111111111111111 valor de unos de 16 bits
            byte Polinomio = 0x8C;    // 1000010000001000

            byte uiCrcValue = preset_value; // valor inicial => 1111111111111111


            for (byte ucI = 0; ucI < pucY.Length; ucI++)
            {
                uiCrcValue = (byte)(uiCrcValue ^ pucY[ucI]); // suma XOR
                for (byte ucJ = 0; ucJ < 8; ucJ++)
                {
                    if ((uiCrcValue & 0x0001) != 0) // si el ultimo bit es 1
                    {
                        //desplazamos un bit a la derecha y hacemos la suma XOR con el Polinomio
                        uiCrcValue = (byte)((uiCrcValue >> 1) ^ Polinomio);
                    }
                    else
                    {
                        //desplazamos un bit a la derecha
                        uiCrcValue = (byte)(uiCrcValue >> 1);
                    }
                }
            }

            return uiCrcValue;
        }
    }
}
