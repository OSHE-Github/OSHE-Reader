#define EPD_5IN83_V2_WIDTH 648
#define EPD_5IN83_V2_HEIGHT 480

#define EPD_SCK_PIN  13
#define EPD_MOSI_PIN 14
#define EPD_CS_PIN   15
#define EPD_RST_PIN  26
#define EPD_DC_PIN   27
#define EPD_BUSY_PIN 25

void EPD_5IN83_V2_Reset(void);
void EPD_5IN83_V2_SendCommand(uint8_t cmd);
void EPD_5IN83_V2_SendData(uint8_t data);
void EPD_5IN83_V2_ReadBusy(void);
void EPD_5IN83_V2_TurnOnDisplay(void);
void EPD_5IN83_V2_Sleep(void);
void EPD_5IN83_V2_Init(void);
void EPD_5IN83_V2_Clear(void);
void EPD_5IN83_V2_Display(uint8_t* buffer);