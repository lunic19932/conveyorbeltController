#ifndef UTIL_H
#define UTIL_H

// for Error
#include <cstdlib>
#include <cstdio>
#include <cerrno>

// for wait
#include <thread>
#include <chrono>


#define DBG_ERROR(err) std::perror(err);

#define WAIT(x) (std::this_thread::sleep_for(std::chrono::milliseconds(x)))

#define _BV(x) (1<<(x))

#endif
