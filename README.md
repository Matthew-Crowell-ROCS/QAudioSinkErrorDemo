# QAudioSink Error Demo

## The Error
QAudioSink encounters a buffer underrun error in a variety of situations on macOS relating to differences in the output's preferred format and the input's format.

### Output Configured for 48kHz with a bit depth of 24
The following audio file configurations trigger buffer underruns:
- 44.1kHz/24 bit
- MP3 files (sample format of float seems to be the problem)

### Output Configured for 44.1kHz with a bit depth of 16
The following audio file configurations trigger buffer underruns:
- 48kHz/24 bit

Increasing the QAudioSink's buffer size to 16384 seems to eliminate the performance issue on macOS, but Windows and Linux have no problem with these situations at all.
