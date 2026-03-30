#include <stdio.h>
#include "miniz.h"

int main()
{
  mz_zip_archive zip_archive;
  
  mz_zip_zero_struct(&zip_archive);

  if (!mz_zip_reader_init_file_v2(&zip_archive, "/media/ian/3CB3-E795/MobyDick.epub", 0, 0, 0))
  {
      printf("%s\n", mz_zip_get_error_string(zip_archive.m_last_error));
      return 0;
  }

  uint32_t num_files = mz_zip_reader_get_num_files(&zip_archive);

  printf("\nFound %d files in /media/ian/3CB3-E795/MobyDick.epub:\n", num_files);

  for(uint32_t i = 0; i < num_files; i++)
  {
    char buf[128];
    mz_zip_reader_get_filename(&zip_archive, i, buf, 127);
    buf[127] = '\0'; // add extra null terminator
    printf("%s\n", buf);
  }

  printf("%s\n", "Searching for OEBPS/content.opf");

  uint32_t index;
  mz_zip_reader_locate_file_v2(&zip_archive, "OEBPS/content.opf", NULL, 0, (mz_uint32*) &index);
  printf("%d\n", index);

  mz_zip_reader_extract_to_file(&zip_archive, index, "/media/ian/3CB3-E795/uncomp_content.opf", 0);

  printf("%s\n", "Done!\n");
}
