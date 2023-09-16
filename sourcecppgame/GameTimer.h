#pragma once

class Timer
{
private:
    int startTime;
    int pausedTime;
    bool isStarted;
    bool isPaused;

public:
    Timer()
        : startTime(0), pausedTime(0), isStarted(false), isPaused(false) {}

    void Start();
    void Stop();
    void Pause();
    void Unpause();
    int GetTicks();

    inline bool IsStarted() const { return isStarted; }
    inline bool IsPaused() const { return isPaused; }
};