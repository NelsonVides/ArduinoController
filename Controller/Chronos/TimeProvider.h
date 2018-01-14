/*
 * TimeProvider.h
 *  Created on: Jan 8, 2018
 *      Author: Nelson Vides
 */
/*
 time.h - low level time and date functions
 */
/*
 July 3 2011 - fixed elapsedSecsThisWeek macro (thanks Vincent Valdy for this)
 - fixed  daysToTime_t macro (thanks maniacbug)
 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Forked by Nelson Vides to implement modern C++ features
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef CLASSES_TIMEPROVIDER_H_
#define CLASSES_TIMEPROVIDER_H_

#include <inttypes.h>

namespace Chronos {
    using time_t = unsigned long;
    using getExternalTime = time_t(*)();
    constexpr uint8_t dt_SHORT_STR_LEN = 3;
    constexpr uint8_t dt_MAX_STRING_LEN = 9;
    enum class timeStatus_t {
        timeNotSet, timeNeedsSync, timeSet
    };
    enum class timeDayOfWeek_t {
        dowInvalid,
        dowMonday,
        dowTuesday,
        dowWednesday,
        dowThursday,
        dowFriday,
        dowSaturday,
        dowSunday
    };
    enum class tmByteFields {
        tmSecond, tmMinute, tmHour, tmWday, tmDay, tmMonth, tmYear, tmNbrFields
    };

    class TimeProvider {
    public:
        TimeProvider();
        virtual ~TimeProvider();

        /*  time and date functions   */
        int hour();            // the hour now
        int hour(time_t t);    // the hour for the given time
        int minute();          // the minute now
        int minute(time_t t);  // the minute for the given time
        int second();          // the second now
        int second(time_t t);  // the second for the given time
        int day();             // the day now
        int day(time_t t);     // the day for the given time
        int weekday();         // the weekday now (Monday is day 1)
        int weekday(time_t t); // the weekday for the given time
        int month();           // the month now  (Jan is month 1)
        int month(time_t t);   // the month for the given time
        int year();            // the full four digit year: (2009, 2010 etc)
        int year(time_t t);    // the year for the given time

        /* Names of month and weekdays */
        char* monthStr(uint8_t month);
        char* dayStr(uint8_t day);
        char* monthShortStr(uint8_t month);
        char* dayShortStr(uint8_t day);

        /* managing timer services */
        void setTime(time_t t);
        void setTime(int hr, int min, int sec, int day, int month, int yr);
        void adjustTime(long adjustment);
        /* time sync functions  */
        timeStatus_t timeStatus(); // indicates if time has been set and recently synchronized
        void setSyncProvider(getExternalTime getTimeFunction); // identify the external time provider
        void setSyncInterval(time_t interval); // set the number of seconds between re-sync

    private:
        uint8_t _Second = 0;
        uint8_t _Minute = 0;
        uint8_t _Hour = 0;
        uint8_t _Wday = 0;   // day of week, Monday is day 1
        uint8_t _Day = 0;
        uint8_t _Month = 0;
        uint8_t _Year = 0;   // offset from 1970;

        uint32_t syncInterval = 300; // time sync will be attempted after this many seconds
        uint32_t sysTime = 0;
        uint32_t prevMillis = 0;
        uint32_t nextSyncTime = 0;
        time_t cacheTime = 0;   // the time the cache was updated
        getExternalTime getTimePtr = nullptr;  // pointer to external sync function
        timeStatus_t Status = timeStatus_t::timeNotSet;

        time_t now();         // return the current time as seconds since Jan 1 1970
        void refreshCache(time_t t);

        /* low level functions to convert to and from system time */
        void breakTime(time_t time); // break time_t into elements
        time_t makeTime(); // convert time elements into time_t
    };
}

#endif /* CLASSES_TIMEPROVIDER_H_ */
