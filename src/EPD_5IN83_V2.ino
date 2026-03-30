void EPD_5IN83_V2_Reset(void)
{
  digitalWrite(EPD_RST_PIN, 1);
  delay(200);
  digitalWrite(EPD_RST_PIN, 0);
  delay(5);
  digitalWrite(EPD_RST_PIN, 1);
  delay(200);
}

void EPD_5IN83_V2_SendCommand(uint8_t cmd)
{
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  digitalWrite(EPD_DC_PIN, 0);
  digitalWrite(EPD_CS_PIN, 0);
  SPI.transfer(cmd);
  digitalWrite(EPD_CS_PIN, 1);
  SPI.endTransaction();
}

void EPD_5IN83_V2_SendData(uint8_t data)
{
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  digitalWrite(EPD_DC_PIN, 1);
  digitalWrite(EPD_CS_PIN, 0);
  SPI.transfer(data);
  digitalWrite(EPD_CS_PIN, 1);
  SPI.endTransaction();
}

void EPD_5IN83_V2_SendData_Invert(uint8_t* data, uint32_t length)
{
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  digitalWrite(EPD_DC_PIN, 1);
  digitalWrite(EPD_CS_PIN, 0);

  for(int i = 0; i < length; i++)
  {
    SPI.transfer(~data[i]);
  }

  digitalWrite(EPD_CS_PIN, 1);
  SPI.endTransaction();
}

void EPD_5IN83_V2_SendData(uint8_t* data, uint32_t length)
{
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  digitalWrite(EPD_DC_PIN, 1);
  digitalWrite(EPD_CS_PIN, 0);

  for(int i = 0; i < length; i++)
  {
    SPI.transfer(data[i]);
  }

  digitalWrite(EPD_CS_PIN, 1);
  SPI.endTransaction();
}

void EPD_5IN83_V2_ReadBusy(void)
{
  do
  {
    EPD_5IN83_V2_SendCommand(0x71);
    delay(50);
  }
  while(!digitalRead(EPD_BUSY_PIN));
  
  delay(50);
}

void EPD_5IN83_V2_TurnOnDisplay(void)
{
  EPD_5IN83_V2_SendCommand(0x12);
  delay(100);
  EPD_5IN83_V2_ReadBusy();
}

void EPD_5IN83_V2_Sleep(void)
{
  EPD_5IN83_V2_SendCommand(0x02); // POWER_OFF
  EPD_5IN83_V2_ReadBusy();
  EPD_5IN83_V2_SendCommand(0x07); // DEEP_SLEEP
  EPD_5IN83_V2_SendData(0XA5);
}

void EPD_5IN83_V2_Init(void)
{
	EPD_5IN83_V2_Reset();

	EPD_5IN83_V2_SendCommand(0x01);		//POWER SETTING
	EPD_5IN83_V2_SendData (0x07);
	EPD_5IN83_V2_SendData (0x07);     //VGH=20V,VGL=-20V
	EPD_5IN83_V2_SendData (0x3f);		  //VDH=15V
	EPD_5IN83_V2_SendData (0x3f);		  //VDL=-15V

	EPD_5IN83_V2_SendCommand(0x04);   //POWER ON
	delay(100);  
	EPD_5IN83_V2_ReadBusy();          //waiting for the electronic paper IC to release the idle signal

	EPD_5IN83_V2_SendCommand(0X00);		//PANNEL SETTING
	EPD_5IN83_V2_SendData(0x1F);      //KW-3f   KWR-2F	BWROTP 0f	BWOTP 1f

	EPD_5IN83_V2_SendCommand(0x61);   //tres			
	EPD_5IN83_V2_SendData (0x02);		  //source 648
	EPD_5IN83_V2_SendData (0x88);
	EPD_5IN83_V2_SendData (0x01);		  //gate 480
	EPD_5IN83_V2_SendData (0xE0);

	EPD_5IN83_V2_SendCommand(0X15);		
	EPD_5IN83_V2_SendData(0x00);		

	EPD_5IN83_V2_SendCommand(0X50);		//VCOM AND DATA INTERVAL SETTING
	//EPD_5IN83_V2_SendData(0x10);
	EPD_5IN83_V2_SendData(0x12); // use new only mode, instead of new/old mode
	EPD_5IN83_V2_SendData(0x07);

	EPD_5IN83_V2_SendCommand(0X60);		//TCON SETTING
	EPD_5IN83_V2_SendData(0x22);
}

void EPD_5IN83_V2_Clear(void)
{
  uint32_t width_bytes = EPD_5IN83_V2_WIDTH / 8;
  uint32_t height = EPD_5IN83_V2_HEIGHT;

	EPD_5IN83_V2_SendCommand(0x13);

  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  digitalWrite(EPD_DC_PIN, 1);
  digitalWrite(EPD_CS_PIN, 0);

	for(uint32_t i = 0; i < width_bytes * height; i++)
  {
    SPI.transfer(0x00);
	}

  digitalWrite(EPD_CS_PIN, 1);
  SPI.endTransaction();

	EPD_5IN83_V2_TurnOnDisplay();
}

void EPD_5IN83_V2_Display(uint8_t* buffer)
{
	EPD_5IN83_V2_SendCommand(0x13);

  EPD_5IN83_V2_SendData(buffer, EPD_5IN83_V2_WIDTH * EPD_5IN83_V2_HEIGHT / 8);  

	EPD_5IN83_V2_TurnOnDisplay();
}