#include "FileSystems.h"
#include "miniz.h"

SET_LOOP_TASK_STACK_SIZE(64 * 1024); // 64KB

#define PIN_SD_CARD_CS 18 
#define PIN_SD_CARD_MISO 2
#define PIN_SD_CARD_MOSI 7
#define PIN_SD_CARD_CLK  6

void setup() {
// setup logger
  Serial.begin(115200);
  file_systems::FSLogger.begin(file_systems::FSDebug, Serial); 

// setup SD
  SPI.begin(PIN_SD_CARD_CLK, PIN_SD_CARD_MISO, PIN_SD_CARD_MOSI, PIN_SD_CARD_CS);
  // "sd" is default mount point
  while(!SD.begin(PIN_SD_CARD_CS)){
    delay(500);
  }

// read directory
  DIR *dir;
  struct dirent *entry;

  if ((dir = opendir("/sd")) == NULL)
  {
    Serial.println("opendir() error");
  }

  mz_zip_archive zip_archive;
  
  mz_zip_zero_struct(&zip_archive);

  if (!mz_zip_reader_init_file_v2(&zip_archive, "/sd/MobyDick.epub", 0, 0, 0))
  {
      Serial.println(mz_zip_get_error_string(zip_archive.m_last_error));
      return;
  }

  uint32_t num_files = mz_zip_reader_get_num_files(&zip_archive);

  Serial.printf("\nFound %d files in /sd/MobyDick.epub:\n", num_files);

  for(uint32_t i = 0; i < num_files; i++)
  {
    char buf[128];
    mz_zip_reader_get_filename(&zip_archive, i, buf, 127);
    buf[127] = '\0'; // add extra null terminator
    Serial.println(buf);
  }

  Serial.println("Searching for OEBPS/content.opf");

  uint32_t index;
  mz_zip_reader_locate_file_v2(&zip_archive, "OEBPS/content.opf", NULL, 0, (mz_uint32*) &index);
  Serial.println(index);

  size_t size_read;
  if(!mz_zip_reader_extract_to_heap(&zip_archive, index, &size_read, 0))
  {
    Serial.println(mz_zip_get_error_string(zip_archive.m_last_error));
    return;
  }

  uint8_t* pBuf = (uint8_t*) malloc(8192);
  uint8_t* pUser_read_buf = (uint8_t*) malloc(8192);

  if(!mz_zip_reader_extract_to_mem_no_alloc(&zip_archive, index, pBuf, 8192, 0, pUser_read_buf, 8192));
  {
    Serial.println(mz_zip_get_error_string(zip_archive.m_last_error));
    return;
  }

  Serial.println();
  for(uint32_t i = 0; i < 100; i++)
    Serial.print((char) pUser_read_buf[i]);

  closedir(dir);
}

void loop(){}