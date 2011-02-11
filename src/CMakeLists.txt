add_subdirectory(table-4.3.0phmodified)

add_library(AudioData audio_data.c)
target_link_libraries(AudioData sndfile samplerate mpg123)

add_library(pHashAudio phash_audio.c fft.c complex.c)


add_executable(auscout auscout.c)

add_executable(tblservd table_server.c)
target_link_libraries(tblservd pHashAudio)

add_executable(metadatadb metadatadb.c)