//
// Created by zaqwes on 23.08.2022.
//

//    YouTube (https://www.youtube.com/watch?v=Q8vCi3ns0bs)
//    Real-Time Linux on Embedded Multicore Processors - Andreas Ehmanns, Technical Advisor

//    YouTube (https://www.youtube.com/watch?v=T-Qamm11UfI&ab_channel=TheLinuxFoundation)
//    Asymmetric Multiprocessing and Embedded Linux - Marek Novak & Dušan Červenka, NXP Semiconductor

// https://rt.wiki.kernel.org/index.php/Frequently_Asked_Questions
// https://rt.wiki.kernel.org/index.php/HOWTO:_Build_an_RT-application

// Measurements
// https://stackoverflow.com/questions/88/is-gettimeofday-guaranteed-to-be-of-microsecond-resolution
// https://stackoverflow.com/questions/25583498/clock-monotonic-vs-clock-monotonic-raw-truncated-values

// Clock scaling
// https://askubuntu.com/questions/523640/how-i-can-disable-cpu-frequency-scaling-and-set-the-system-to-performance
// http://www.servernoobs.com/avoiding-cpu-speed-scaling-in-modern-linux-distributions-running-cpu-at-full-speed-tips/

// Tick generation
// https://stackoverflow.com/questions/5833550/how-do-i-get-the-most-accurate-realtime-periodic-interrupts-in-linux

// Shielded CPU
// https://www.linuxjournal.com/article/6900

// irq balanser
// https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux_for_real_time/8/html/optimizing_rhel_8_for_real_time_for_low_latency_operation/assembly_binding-interrupts-and-processes_optimizing-rhel8-for-real-time-for-low-latency-operation

// [!!!] https://www.youtube.com/watch?v=NrjXEaTSyrw&t=2s&ab_channel=TheLinuxFoundation
// [!!!] https://www.coursera.org/lecture/real-time-systems/the-concepts-of-real-time-systems-tJncu - best course

// Alarm! Now to build graph with multiple cores? Use only anomics? how to wait?
// Alarm! Multi graphs on one core

#include "models.hpp"

#include <gtest/gtest.h>

#include <farbot/fifo.hpp>
#include <thread>

#include "rtos_rte_cp/RT-Clock/posix_clock.h"

using namespace std::chrono;  // for example brevity
using namespace std::chrono_literals;

enum class ErrorCode { kNone, kOverflow, kSleepError };

using TimeStamp = std::chrono::nanoseconds;

struct ErrorFrame {
    ErrorFrame() = default;
    explicit ErrorFrame(const ErrorCode& error_code) : error_code_{error_code} {}
    TimeStamp frame_stamp_{};
    ErrorCode error_code_{ErrorCode::kNone};
};

struct SensorFrame {
    SensorFrame() = default;
    explicit SensorFrame(const TimeStamp& frame_stamp) : frame_stamp_{frame_stamp} {}
    TimeStamp frame_stamp_{};
};

using SensorFifo =
    farbot::fifo<SensorFrame, farbot::fifo_options::concurrency::single, farbot::fifo_options::concurrency::single>;
using ErrorCodeFifo =
    farbot::fifo<ErrorFrame, farbot::fifo_options::concurrency::single, farbot::fifo_options::concurrency::single>;

constexpr nanoseconds timespecToDuration(const timespec& ts) {
    const auto duration = seconds{ts.tv_sec} + nanoseconds{ts.tv_nsec};
    return duration_cast<nanoseconds>(duration);
}

TEST(Models, Thread) {
    print_scheduler();

    struct timespec rtclk_resolution {};
    if (clock_getres(MY_CLOCK, &rtclk_resolution) == ERROR) {
        perror("clock_getres");
        exit(-1);
    } else {
        printf("\nPOSIX Clock demo using system RT clock with resolution:\n\t%ld secs, %ld microsecs, %ld nanosecs\n",
               rtclk_resolution.tv_sec, (rtclk_resolution.tv_nsec / 1000), rtclk_resolution.tv_nsec);
    }

    auto error_code_fifo = ErrorCodeFifo{256};
    auto sensor_fifo = SensorFifo{256};

    auto model_fn = [&error_code_fifo, &sensor_fifo]() {
        // Sleep? But it's system call

        // filter for ADC

        // Push filtered to queue?

        // Time markers? realtime_safe

        struct timespec tim {};
        tim.tv_sec = 0;
        tim.tv_nsec = 500'000L;

        while (true) {
            // clock_nanosleep
            const auto status = nanosleep(&tim, nullptr);
            if (status != 0) {
                (void)error_code_fifo.push(ErrorFrame{ErrorCode::kSleepError});
                printf("SleepError\n");
                continue;  // ?
//                return;
            }

            auto rt_clk_time_stamp = timespec{};
            clock_gettime(MY_CLOCK, &rt_clk_time_stamp);

            const auto time_stamp = timespecToDuration(rt_clk_time_stamp);

            const bool pushed = sensor_fifo.push(SensorFrame{time_stamp});
            if (!pushed) {
                (void)error_code_fifo.push(ErrorFrame{ErrorCode::kOverflow});
                printf("Overflow\n");
            }
        }
    };

    auto model_th = std::thread(std::move(model_fn));

    // How to change priority after?
    auto handle = model_th.native_handle();

    long  long last_tick = 0;
    while(true) {
        std::this_thread::sleep_for(50ms);
        SensorFrame frame{};

        while (sensor_fifo.pop(frame)) {
            printf("T: %lld\n", frame.frame_stamp_.count() - last_tick);
            last_tick = frame.frame_stamp_.count();
        }
        printf("Next\n");
    }

    model_th.join();
}