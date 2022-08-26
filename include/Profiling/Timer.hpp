#pragma once

namespace Karbon
{

    struct Timer
    {
        Timer()
        {
            reset();
        }

        void reset()
        {
            m_Start = std::chrono::high_resolution_clock::now();
        }

        [[nodiscard]] float elapsed() const
        {
            return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f * 0.001f;
        }

        [[nodiscard]] float elapsed_millis() const
        {
            return elapsed() * 1000.0f;
        }

        // << operator
        friend std::ostream &operator<<(std::ostream &os, const Timer &timer)
        {
            os << timer.elapsed();
            return os;
        }

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
    };
}
