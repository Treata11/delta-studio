#include "../include/yds_timing.h"

#define NOMINMAX

#if defined(__APPLE__) && defined(__MACH__) // Apple OSX & iOS (Darwin)
    #include "win32/window.h"
#elif defined(_WIN64)
    #include <Windows.h>
#endif

#include <mmsystem.h>

#include <intrin.h>

static bool qpcFlag;
static LARGE_INTEGER qpcFrequency;

ysTimingSystem *ysTimingSystem::g_instance = nullptr;

uint64_t SystemTime() {
    if (qpcFlag) {
        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);
        return uint64_t(currentTime.QuadPart);
    } else {
        // Convert output to microseconds
        return uint64_t(timeGetTime()) * 1000;
    }
}

ysTimingSystem::ysTimingSystem() {
    m_frameDurations = nullptr;
    m_durationSamples = 0;

    SetPrecisionMode(Precision::Microsecond);
    Initialize();
}

ysTimingSystem::~ysTimingSystem() { /* void */
}

uint64_t ysTimingSystem::GetTime() { return SystemTime(); }

inline unsigned __int64 SystemClock() { return __rdtsc(); }

unsigned __int64 ysTimingSystem::GetClock() {
    return (unsigned long long) SystemClock();
}

void ysTimingSystem::SetPrecisionMode(Precision mode) {
    m_precisionMode = mode;

    if (mode == Precision::Millisecond) {
        m_div = 1000.0;
    } else if (mode == Precision::Microsecond) {
        m_div = 1000000.0;
    }
}

double ysTimingSystem::ConvertToSeconds(uint64_t t_u) { return t_u / m_div; }

void ysTimingSystem::Update() {
    if (!m_isPaused) { m_frameNumber++; }

    const uint64_t thisTime = GetTime();
    m_lastFrameDuration = ((thisTime - m_lastFrameTimestamp) * 1000000) /
                          qpcFrequency.QuadPart;
    m_lastFrameTimestamp = thisTime;

    const uint64_t thisClock = GetClock();
    m_lastFrameClockTicks = (thisClock - m_lastFrameClockstamp);
    m_lastFrameClockstamp = thisClock;

    if (m_frameNumber > 1) {
        /*
        m_averageFrameDuration *= 0.95;
        m_averageFrameDuration += 0.05 * (double)m_lastFrameDuration / m_div;

        m_fps = (float)(1 / m_averageFrameDuration);*/
        m_frameDurations[m_durationSampleWriteIndex] =
                double(m_lastFrameDuration) / m_div;
        if (++m_durationSampleWriteIndex >= DurationSamples) {
            m_durationSampleWriteIndex = 0;
        }

        if (m_durationSamples < DurationSamples) { ++m_durationSamples; }

        m_averageFrameDuration = 0;
        for (int i = 0; i < m_durationSamples; ++i) {
            m_averageFrameDuration += m_frameDurations[i];
        }

        if (m_durationSamples > 0) {
            m_averageFrameDuration /= m_durationSamples;
        }

        if (m_averageFrameDuration != 0) {
            m_fps = 1 / float(m_averageFrameDuration);
        }
    }
}

void ysTimingSystem::RestartFrame() {
    const uint64_t thisTime = GetTime();
    m_lastFrameTimestamp = thisTime;
}

void ysTimingSystem::Initialize() {
    qpcFlag = (QueryPerformanceFrequency(&qpcFrequency) > 0);

    m_frameNumber = 0;

    m_lastFrameTimestamp = GetTime();
    m_lastFrameDuration = 0;

    m_lastFrameClockstamp = GetClock();
    m_lastFrameClockTicks = 0;

    m_isPaused = false;

    m_averageFrameDuration = 0;
    m_fps = 1024.0;

    m_frameDurations = new double[DurationSamples];
    m_durationSamples = 0;
    m_durationSampleWriteIndex = 0;
}

double ysTimingSystem::GetFrameDuration() {
    return m_lastFrameDuration / m_div;
}

uint64_t ysTimingSystem::GetFrameDuration_us() { return m_lastFrameDuration; }
