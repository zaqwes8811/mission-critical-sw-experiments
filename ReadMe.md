# Idea
- Study low latency, high performance code(minimal avg runtime)
- Rt/AMP
- Tech for interraction rt and non-rt

# Cases

Case0:
Sigle core - one rt thread + interrupt
Q: ISR -> RT thread how?

Case1:
Single core + two rt thread with preority + 2 IRQ
Q: How to do it without locks? How to wait?

# Buffer

https://stackoverflow.com/questions/9886668/openal-playback-captured-audio-data-c

Read audion rt in linux
process in realtime

https://github.com/thestk/rtaudio - RtAudio - crashes found

PortAudio
https://aws.amazon.com/ru/blogs/developer/real-time-streaming-transcription-with-the-aws-c-sdk/
https://github.com/PortAudio/portaudio

https://juce.com/



Case: rt thread take buffers from pool(hoofs), fill with noize and send (rt-way) to another thread
Q: how to measure latency?
Q: how to check, that code is minimal (no copy, no alloc, etc)

[!!!!]
https://www.youtube.com/watch?v=j2AgjFSFgRc&ab_channel=MeetingCpp