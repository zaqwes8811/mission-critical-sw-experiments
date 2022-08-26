//
// Created by zaqwes on 23.08.2022.
//

#include "models.hpp"

#include <gtest/gtest.h>

#include <farbot/fifo.hpp>
#include <thread>

#include "rtos_rte_cp/RT-Clock/posix_clock.h"

enum class ErrorCode { kNone, kOverflow };

struct SensorFrame {
    int frame_tick_{};
};

using SensorFifo =
    farbot::fifo<SensorFrame, farbot::fifo_options::concurrency::single, farbot::fifo_options::concurrency::single>;
using ErrorCodeFifo =
    farbot::fifo<ErrorCode, farbot::fifo_options::concurrency::single, farbot::fifo_options::concurrency::single>;

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

    auto error_code_fifo = ErrorCodeFifo{8};

    auto model_fn = [&error_code_fifo]() {
        // Sleep? But it's system call

        // filter for ADC

        // Push filtered to queue?

        // Time markers? realtime_safe

        struct timespec tim {};
        tim.tv_sec = 0;
        tim.tv_nsec = 50000000L;

        while (true) {
            // clock_nanosleep
            auto status = nanosleep(&tim, nullptr);
            if (status != 0) {
                printf("Nano sleep system call failed: status=%d \n", status);
                return;
            }

            // TODO() Measure and push to queue
            bool pushed = error_code_fifo.push(ErrorCode::kOverflow);
            if(!pushed) {
                printf("Overflow\n");
            }
        }
    };

    auto model_th = std::thread(std::move(model_fn));

    // How to change priority after?
    auto handle = model_th.native_handle();

    model_th.join();

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
}