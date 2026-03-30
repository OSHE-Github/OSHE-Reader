#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "miniz.h"
#include "stdlib.h"

uint32_t sck = 6;
uint32_t miso = 2;
uint32_t mosi = 7;
uint32_t cs = 18;

#define MINIZ_USE_UNALIGNED_LOADS_AND_STORES 0
#define MINIZ_LITTLE_ENDIAN 1
#define MINIZ_HAS_64BIT_REGISTERS 0

#define MZ_FOPEN(path, mode) fopen_wrapper(path, mode)
#define MZ_FCLOSE(file) (file).close()
#define MZ_FREAD(buf, size, count, file) (file).read(buf, size)
#define MZ_FWRITE(buf, size, count, file) (file).write(buf, size)
#define MZ_FTELL64(file) (file).position()
#define MZ_FSEEK64(file, offset, origin) (file).seek(offset, origin)
#define MZ_FFLUSH(file) (file).flush
#define MZ_FREOPEN(path, mode, file) freopen_wrapper(path, mode, file)
#define MZ_DELETE_FILE(path) SD.remove(path)
#pragma message("TEST")

static File fopen_wrapper(const char *pPath, const char *pMode)
{
  Serial.printf("Opening file: %s with mode: %s\n", pPath, pMode);

  if(pMode[0] == 'r') // catches "r" and "rb"
  {
    return SD.open(pPath, FILE_READ);
  }
  else if(pMode[0] == 'w' && SD.exists(pPath)) // gives expected overwriting behavior of "w" and "wb"
  {
    SD.remove(pPath); // delete existing file
    return SD.open(pPath, FILE_WRITE); // open new empty file
  }
  else // FILE_WRITE and FILE_APPEND function the same
  {
    return SD.open(pPath, FILE_WRITE);
  }
}

static void freopen_wrapper(const char *pPath, const char *pMode, File *pStream)
{
  pStream->close(); // close file currently open
  free(pStream);
  *pStream = fopen_wrapper(pPath, pMode); // overwrite old file pointer with new file
}

void setup()
{
  Serial.begin(115200);

  delay(1000);

  SPI.begin(sck, miso, mosi, cs);

  SD.begin(cs);

  File file = SD.open("/foo.txt");

  uint8_t buf[8];
  file.read(buf, 8);

  Serial.println();
  for(uint32_t i = 0; i < 8; i++)
  {
    Serial.print((char) buf[i]);
  }
  Serial.println();

  Serial.println("before miniz");

  size_t size;
  mz_zip_error error;
  char* buf2;
  buf2 = (char*) mz_zip_extract_archive_file_to_heap_v2("/MobyDick.epub", "/OEBPS/content.opf", NULL, &size, 0, &error);

  Serial.println(mz_zip_get_error_string(error));

  while(1);

  for(uint32_t i = 0; i < 100; i++)
  {
    Serial.print((char) (buf2[i]));
  }

  /*mz_zip_archive zip_archive;
  memset(&zip_archive, 0, sizeof(zip_archive));

  bool status = mz_zip_reader_init_file(&zip_archive, "/MobyDick.epub", 0);
  if (!status)
  {
    Serial.println("mz_zip_reader_init_file() failed!\n");
    Serial.println("Error %s\n", mz_zip_get_error_string(zip_archive.m_last_error));
    return false;
  }

  // Run through the archive and find the requiested file
  for (int i = 0; i < (int)mz_zip_reader_get_num_files(&zip_archive); i++)
  {
    mz_zip_archive_file_stat file_stat;
    if (!mz_zip_reader_file_stat(&zip_archive, i, &file_stat))
    {
      Serial.println("mz_zip_reader_file_stat() failed!\n");
      Serial.println("Error %s\n", mz_zip_get_error_string(zip_archive.m_last_error));
      mz_zip_reader_end(&zip_archive);
      return false;
    }
    // is this the file we're looking for?
    if (strcmp("/OEBPS/content.opf", file_stat.m_filename) == 0)
    {
      Serial.println("Extracting %s\n", file_stat.m_filename);
      mz_zip_reader_extract_file_to_file(&zip_archive, file_stat.m_filename, dest, 0);
      mz_zip_reader_end(&zip_archive);
      return true;
    }
  }

  mz_zip_reader_end(&zip_archive);*/
}

void loop()
{

}
