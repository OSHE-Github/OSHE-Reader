#include "SPI.h"
#include "EPD_5IN83_V2.h"

#include "OSHE_Logo.h"
#include "OSHE_Logo_Inverted.h"

void setup()
{
  Serial.begin(115200);

  pinMode(EPD_CS_PIN, OUTPUT);
  pinMode(EPD_DC_PIN, OUTPUT);
  pinMode(EPD_RST_PIN, OUTPUT);
  pinMode(EPD_BUSY_PIN, INPUT);

  digitalWrite(EPD_CS_PIN, 1);
  SPI.begin(EPD_SCK_PIN, 3, EPD_MOSI_PIN, EPD_CS_PIN);
  
  EPD_5IN83_V2_Init();
  EPD_5IN83_V2_Clear();

  delay(500);
  
  // Display the OSHE logo in 8 distinct chunks

  uint32_t length = EPD_5IN83_V2_WIDTH * EPD_5IN83_V2_WIDTH / 8 / 8;

  EPD_5IN83_V2_SendCommand(0x13);
  for(uint32_t i = 0; i < 8; i++)
  {
    uint16_t start_x = 0;
    uint16_t start_y = EPD_5IN83_V2_HEIGHT / 8 * i;
    uint8_t* logo_chunk = (uint8_t*) OSHE_Logo_Bitmap + length * i;
    //Serial.printf("Printing pixels %d-%d\n", length * i, length * i + length);
    EPD_5IN83_V2_SendData(logo_chunk, length);
    //delay(100);
  }
  EPD_5IN83_V2_TurnOnDisplay();

  delay(1000);

  for(uint8_t i = 0; i < 4; i++)
  {
    EPD_5IN83_V2_Display(OSHE_Logo_Bitmap);
    EPD_5IN83_V2_Display(OSHE_Logo_Inverted_Bitmap);
  }

  delay(10000);

  Serial.println("ENDED");
  return;

  EPD_5IN83_V2_Clear();

  EPD_5IN83_V2_Sleep();
}

void loop()
{
  
}
