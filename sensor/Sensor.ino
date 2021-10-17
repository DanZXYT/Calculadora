byte Data[10];
byte data[9];
byte crc;
int sizebyte_total = 10;
int sizebyte_data = 9;
String Datos;
String valores[3];

double resultado;
String R;

byte Data_R[5];

void setup() {
  Serial.begin(115200);

}

void B_Buff()
{
  for(int i = 0;i<sizebyte_total;i++)
  {
    Data[i]=0x0;
  }
}

void loop() {
  while(Serial.available()>0)
  {
    //8 bytes con los numeros + 1 byte con la operacion + 1 byte (8bit)checksum = 10 byte
    B_Buff();
    int j=0;
    Serial.readBytes(Data,sizebyte_total);


    
    crc=uiCrc8Cal(Data,sizebyte_total);
    delay(100);


    if(crc == 0)
    {
      Data[sizebyte_total-1]=0x0;
      Datos = String((char *)(Data));

      valores[0]="";
      int j=0;
      for(int i=0;i<Datos.length();i++)
      {
        if(Datos[i]!='+' && Datos[i]!='-' && Datos[i]!='*' && Datos[i]!='/')
        {
          valores[j]+=Datos[i];
        }
        else
        {
          j++;
          valores[j]="";
        }
      }
      for(int i = 0;i<Datos.length();i++)
      {
        switch (Datos[i])
        {
          case '+': resultado = (valores[0].toDouble()) + (valores[1].toDouble());
          break;

          case '-': resultado = (valores[0].toDouble()) - (valores[1].toDouble());
          break;

          case '*': resultado = (valores[0].toDouble()) * (valores[1].toDouble());
          break;

          case '/': resultado = (valores[0].toDouble()) / (valores[1].toDouble());
          break;
        }
      }
      
      if(10<=resultado)
      {
        R = String(resultado,1);
      }
      else
      {
        R = String(resultado,2);
      }
      
      R.getBytes(Data_R,5);

      crc=uiCrc8Cal(Data_R,4);
      Data_R[4]=crc;

      Serial.write(Data_R,sizeof(Data_R));
      
    }
  }
}



unsigned int uiCrc8Cal(byte pucY[], byte ucX)
{
  const uint16_t PRESET_VALUE = 0xFF;
  const uint16_t POLYNOMIAL = 0x8C;
  byte ucI, ucJ;
  unsigned short int uiCrcValue = PRESET_VALUE;
  for (ucI = 0; ucI < ucX; ucI++)
  {
    uiCrcValue = uiCrcValue ^ pucY[ucI];
    for (ucJ = 0; ucJ < 8; ucJ++)
    {
      if ((uiCrcValue & 0x0001) != 0)
      {
        uiCrcValue = (uiCrcValue >> 1) ^ POLYNOMIAL;
      }
      else
      {
        uiCrcValue = (uiCrcValue >> 1);
      }
    }
  }
  return uiCrcValue;
}
