#ifndef TIMEHELP_H
#define TIMEHELP_H

#include <ctime>

class Time {
    public:
    int     yr  = 0;
    int     mo  = 0;
    int     dy  = 0;
    int     hr  = 0;
    int     mi  = 0;
    int     sc  = 0;
    int     yd  = 0;

    time_t  epoch   = 0;

    Time() {
        epoch = time(0);
        struct tm* tval = localtime(&epoch);

        yr = tval->tm_year;
        mo = tval->tm_mon;
        dy = tval->tm_mday;
        hr = tval->tm_hour;
        mi = tval->tm_min;
        sc = tval->tm_sec;
        yd = tval->tm_yday;
    }

    Time(time_t epoch_p) : epoch(epoch_p) {
        struct tm* tval = localtime(&epoch_p);

        yr = tval->tm_year;
        mo = tval->tm_mon;
        dy = tval->tm_mday;
        hr = tval->tm_hour;
        mi = tval->tm_min;
        sc = tval->tm_sec;
        yd = tval->tm_yday;
    }

    Time(time_t& epoch_p) : epoch(epoch_p) {
        struct tm* tval = localtime(&epoch_p);

        yr = tval->tm_year;
        mo = tval->tm_mon;
        dy = tval->tm_mday;
        hr = tval->tm_hour;
        mi = tval->tm_min;
        sc = tval->tm_sec;
        yd = tval->tm_yday;
    }

    bool operator==(const Time& rhs) const {
        return ( epoch == rhs.epoch );
    }

};

#endif // TIMEHELP_H
