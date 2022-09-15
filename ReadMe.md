# Goal
- Safe/Mission critical SW design
- Study low latency, high performance code(minimal avg runtime)
- Rt/AMP
- Tech for interraction rt and non-rt
- Partition - cpu/cache/mem/numa/...
- Last C++ features

# Cases

Case0:
Sigle core - one rt thread + interrupt
Q: ISR -> RT thread how?

Case1:
Single core + two rt thread with preority + 2 IRQ
Q: How to do it without locks? How to wait?

[!!!!]
https://www.youtube.com/watch?v=j2AgjFSFgRc&ab_channel=MeetingCpp

# C++20
```
sudo apt install linuxbrew-wrapper
brew install gcc@12
export PATH="$HOME/.linuxbrew/bin:$PATH"

CC=gcc-12 CXX=g++-12 cmake ..

cmake -DCMAKE_C_COMPILER=gcc-12 -DCMAKE_CXX_COMPILER=g++-12 ..

cmake -DCMAKE_C_COMPILER=/home/zaqwes/.linuxbrew/bin/gcc-12 \
-DCMAKE_CXX_COMPILER=/home/zaqwes/.linuxbrew/bin/g++-12 -DCMAKE_BUILD_TYPE=Release ..
```

```
git submodule update --init --recursive
```