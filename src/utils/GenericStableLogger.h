#pragma once
#include <vector>
#include <set>
#include <cstddef>

template <typename T>
class GenericStableLogger
{
public:
    GenericStableLogger(size_t bufferSize = 5)
        : _bufferSize(bufferSize) {}

    // Gebruik: shouldLog(value, lastLogged, prevLogged)
    bool shouldLog(T value, T lastLogged, T prevLogged = T())
    {
        _lastValues.push_back(value);
        if (_lastValues.size() > _bufferSize)
        {
            _lastValues.erase(_lastValues.begin());
        }

        // Geen verandering
        if (value == lastLogged)
        {
            return false;
        }

        std::set<T> uniqueVals(_lastValues.begin(), _lastValues.end());

        // Flip-detectie: als we flippen tussen 2 waarden, log niet
        if (_lastValues.size() >= 2 && uniqueVals.size() != 1)
        {
            if (value == prevLogged)
            {
                return false;
            }
        }
        return true;
    }

private:
    size_t _bufferSize;
    std::vector<T> _lastValues;
};