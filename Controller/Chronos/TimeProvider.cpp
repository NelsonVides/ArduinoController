/*
 * TimeProvider.cpp
 *  Created on: Jan 8, 2018
 *      Author: Nelson Vides
 */

#include "Arduino.h"
#include "TimeProvider.h"

using Chronos::time_t;
using Chronos::getExternalTime;
using Chronos::TimeProvider;
using Chronos::timeStatus_t;
using Chronos::dt_SHORT_STR_LEN;
using Chronos::dt_MAX_STRING_LEN;

namespace {
    static char buffer[dt_MAX_STRING_LEN + 1]; // must be big enough for longest string and the terminating null

    const char monthStr0[] PROGMEM = "";
    const char monthStr1[] PROGMEM = "January";
    const char monthStr2[] PROGMEM = "February";
    const char monthStr3[] PROGMEM = "March";
    const char monthStr4[] PROGMEM = "April";
    const char monthStr5[] PROGMEM = "May";
    const char monthStr6[] PROGMEM = "June";
    const char monthStr7[] PROGMEM = "July";
    const char monthStr8[] PROGMEM = "August";
    const char monthStr9[] PROGMEM = "September";
    const char monthStr10[] PROGMEM = "October";
    const char monthStr11[] PROGMEM = "November";
    const char monthStr12[] PROGMEM = "December";

    const PROGMEM char * const PROGMEM monthNames_P[] = {
            monthStr0, monthStr1, monthStr2, monthStr3,
            monthStr4, monthStr5, monthStr6, monthStr7,
            monthStr8, monthStr9, monthStr10, monthStr11, monthStr12 };

    const char monthShortNames_P[] PROGMEM
    = "ErrJanFebMarAprMayJunJulAugSepOctNovDec";

    const char dayStr0[] PROGMEM = "Err";
    const char dayStr1[] PROGMEM = "Monday";
    const char dayStr2[] PROGMEM = "Tuesday";
    const char dayStr3[] PROGMEM = "Wednesday";
    const char dayStr4[] PROGMEM = "Thursday";
    const char dayStr5[] PROGMEM = "Friday";
    const char dayStr6[] PROGMEM = "Saturday";
    const char dayStr7[] PROGMEM = "Sunday";

    const PROGMEM char * const PROGMEM dayNames_P[] = { dayStr0, dayStr1,
            dayStr2, dayStr3, dayStr4, dayStr5, dayStr6, dayStr7 };

    const char dayShortNames_P[] PROGMEM = "ErrMonTueWedThuFriSatSun";
    //convenience macros to convert to and from tm years
    constexpr int tmYearToCalendar(uint8_t Y) { return Y + 1970;}
    constexpr uint8_t CalendarYrToTm(int Y) { return Y - 1970;}
    constexpr int tmYearToY2k(uint8_t Y) { return Y - 30;}
    constexpr uint8_t y2kYearToTm(int Y) { return Y + 30;}

    /*==============================================================================*/
    /* Useful Constants */
    constexpr auto SECS_PER_MIN = ((time_t) (60UL));
    constexpr auto SECS_PER_HOUR = ((time_t) (3600UL));
    constexpr auto SECS_PER_DAY = ((time_t) (SECS_PER_HOUR * 24UL));
    constexpr auto DAYS_PER_WEEK = ((time_t) (7UL));
    constexpr auto SECS_PER_WEEK = ((time_t) (SECS_PER_DAY * DAYS_PER_WEEK));
    constexpr auto SECS_PER_YEAR = ((time_t) (SECS_PER_DAY * 365UL)); // TODO: ought to handle leap years
    constexpr auto SECS_YR_2000 = ((time_t) (946684800UL));

    /* Useful Macros for getting elapsed time */
    constexpr auto numberOfSeconds(auto _time_) {return (_time_ % SECS_PER_MIN);};
    constexpr auto numberOfMinutes(auto _time_) {return ((_time_ / SECS_PER_MIN) % SECS_PER_MIN);};
    constexpr auto numberOfHours(auto _time_) {return (( _time_% SECS_PER_DAY) / SECS_PER_HOUR);};
    constexpr auto dayOfWeek(auto _time_) {return ((( _time_ / SECS_PER_DAY + 4)  % DAYS_PER_WEEK)+1);};
    constexpr auto elapsedDays(auto _time_) {return ( _time_ / SECS_PER_DAY);};
    constexpr auto elapsedSecsToday(auto _time_) {return (_time_ % SECS_PER_DAY);};

// The following macros are used in calculating alarms and assume the clock is set to a date later than Jan 1 1971
// Always set the correct time before settting alarms
    constexpr auto previousMidnight(auto _time_) {return (( _time_ / SECS_PER_DAY) * SECS_PER_DAY);};
    constexpr auto nextMidnight(auto _time_) {return ( previousMidnight(_time_)  + SECS_PER_DAY );};
    constexpr auto elapsedSecsThisWeek(auto _time_) {return (elapsedSecsToday(_time_) +  ((dayOfWeek(_time_)-1) * SECS_PER_DAY) );};
    constexpr auto previousSunday(auto _time_) {return (_time_ - elapsedSecsThisWeek(_time_));};
    constexpr auto nextSunday(auto _time_) {return ( previousSunday(_time_)+SECS_PER_WEEK);};

    /* Useful Macros for converting elapsed time to a time_t */
    constexpr auto minutesToTime_t(auto M) {return (M * SECS_PER_MIN);};
    constexpr auto hoursToTime_t(auto H) {return (H * SECS_PER_HOUR);};
    constexpr auto daysToTime_t(auto D) {return (D * SECS_PER_DAY);};
    constexpr auto weeksToTime_t(auto W) {return (W * SECS_PER_WEEK);};
    // leap year calulator expects year argument as years offset from 1970
    constexpr bool LEAP_YEAR(uint8_t Y) {return ( ((1970+Y)>0) && !((1970+Y)%4) && ( ((1970+Y)%100) || !((1970+Y)%400) ) );};
    constexpr uint8_t monthDays[] = {
            31, 28, 31, 30, 31, 30,
            31, 31, 30, 31, 30, 31 }; // API starts months from 1, this array starts from 0
}


TimeProvider::TimeProvider(){}
TimeProvider::~TimeProvider(){}

int TimeProvider::hour()
{ // the hour now
    return hour(now());
}
int TimeProvider::hour(time_t t)
{ // the hour for the given time
    refreshCache(t);
    return this->_Hour;
}

int TimeProvider::minute()
{
    return minute(now());
}
int TimeProvider::minute(time_t t)
{ // the minute for the given time
    refreshCache(t);
    return this->_Minute;
}

int TimeProvider::second()
{
    return second(now());
}
int TimeProvider::second(time_t t)
{  // the second for the given time
    refreshCache(t);
    return this->_Second;
}

int TimeProvider::day()
{
    return (day(now()));
}
int TimeProvider::day(time_t t)
{ // the day for the given time (0-6)
    refreshCache(t);
    return this->_Day;
}

int TimeProvider::weekday()
{   // Sunday is day 1
    return weekday(now());
}
int TimeProvider::weekday(time_t t)
{
    refreshCache(t);
    return this->_Wday;
}

int TimeProvider::month()
{
    return month(now());
}
int TimeProvider::month(time_t t)
{  // the month for the given time
    refreshCache(t);
    return this->_Month;
}

int TimeProvider::year()
{  // as in Processing, the full four digit year: (2009, 2010 etc)
    return year(now());
}
int TimeProvider::year(time_t t)
{ // the year for the given time
    refreshCache(t);
    return tmYearToCalendar(this->_Year);
}
/* functions to return date strings */
char* TimeProvider::monthStr(uint8_t month)
{
    strcpy_P(buffer, (PGM_P) pgm_read_word(&(monthNames_P[month])));
    return buffer;
}

char* TimeProvider::monthShortStr(uint8_t month)
{
    for (int i = 0; i < dt_SHORT_STR_LEN; i++)
        buffer[i] = pgm_read_byte(
                &(monthShortNames_P[i + (month * dt_SHORT_STR_LEN)]));
    buffer[dt_SHORT_STR_LEN] = 0;
    return buffer;
}

char* TimeProvider::dayStr(uint8_t day)
{
    strcpy_P(buffer, (PGM_P) pgm_read_word(&(dayNames_P[day])));
    return buffer;
}

char* TimeProvider::dayShortStr(uint8_t day)
{
    uint8_t index = day * dt_SHORT_STR_LEN;
    for (int i = 0; i < dt_SHORT_STR_LEN; i++)
        buffer[i] = pgm_read_byte(&(dayShortNames_P[index + i]));
    buffer[dt_SHORT_STR_LEN] = 0;
    return buffer;
}

void TimeProvider::setTime(time_t t)
{
    sysTime = (uint32_t) t;
    nextSyncTime = (uint32_t) t + syncInterval;
    Status = timeStatus_t::timeSet;
    prevMillis = millis(); // restart counting from now (thanks to Korman for this fix)
}

void TimeProvider::setTime(int hr, int min, int sec, int dy, int mnth, int yr)
{
    // year can be given as full four digit year or two digts (2010 or 10 for 2010);
    //it is converted to years since 1970
    if (yr > 99)
        yr = yr - 1970;
    else
        yr += 30;
    this->_Year = yr;
    this->_Month = mnth;
    this->_Day = dy;
    this->_Hour = hr;
    this->_Minute = min;
    this->_Second = sec;
    setTime(makeTime());
}

void TimeProvider::adjustTime(long adjustment)
{
    sysTime += adjustment;
}

// indicates if time has been set and recently synchronized
timeStatus_t TimeProvider::timeStatus()
{
    now(); // required to actually update the status
    return Status;
}

void TimeProvider::setSyncProvider(getExternalTime getTimeFunction)
{
    getTimePtr = getTimeFunction;
    nextSyncTime = sysTime;
    now(); // this will sync the clock
}

void TimeProvider::setSyncInterval(time_t interval)
{ // set the number of seconds between re-sync
    syncInterval = (uint32_t) interval;
    nextSyncTime = sysTime + syncInterval;
}








void TimeProvider::refreshCache(time_t t)
{
    if (t != cacheTime) {
        breakTime(t);
        cacheTime = t;
    }
}

// break the given time_t into time components
// this is a more compact version of the C library localtime function
// note that year is offset from 1970 !!!
void TimeProvider::breakTime(time_t timeInput)
{
    uint8_t year;
    uint8_t month, monthLength;
    uint32_t time;
    unsigned long days;

    time = (uint32_t) timeInput;
    this->_Second = time % 60;
    time /= 60; // now it is minutes
    this->_Minute = time % 60;
    time /= 60; // now it is hours
    this->_Hour = time % 24;
    time /= 24; // now it is days
    this->_Wday = ((time + 4) % 7) + 1;  // Sunday is day 1

    year = 0;
    days = 0;
    while ((unsigned) (days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
        year++;
    }
    this->_Year = year; // year is offset from 1970

    days -= LEAP_YEAR(year) ? 366 : 365;
    time -= days; // now it is days in this year, starting at 0

    days = 0;
    month = 0;
    monthLength = 0;
    for (month = 0; month < 12; month++) {
        if (month == 1) { // february
            if (LEAP_YEAR(year)) {
                monthLength = 29;
            } else {
                monthLength = 28;
            }
        } else {
            monthLength = monthDays[month];
        }

        if (time >= monthLength) {
            time -= monthLength;
        } else {
            break;
        }
    }
    this->_Month = month + 1;  // jan is month 1
    this->_Day = time + 1;     // day of month
}

// assemble time elements into time_t
// note year argument is offset from 1970 (see macros in time.h to convert to other formats)
// previous version used full four digit year (or digits since 2000),i.e. 2009 was 2009 or 9
time_t TimeProvider::makeTime()
{
    int i;
    uint32_t seconds;

    // seconds from 1970 till 1 jan 00:00:00 of the given year
    seconds = this->_Year * (SECS_PER_DAY * 365);
    for (i = 0; i < this->_Year; i++) {
        if (LEAP_YEAR(i)) {
            seconds += SECS_PER_DAY;   // add extra days for leap years
        }
    }

    // add days for this year, months start from 1
    for (i = 1; i < this->_Month; i++) {
        if ((i == 2) && LEAP_YEAR(this->_Year)) {
            seconds += SECS_PER_DAY * 29;
        } else {
            seconds += SECS_PER_DAY * monthDays[i - 1]; //monthDay array starts from 0
        }
    }
    seconds += (this->_Day - 1) * SECS_PER_DAY;
    seconds += this->_Hour * SECS_PER_HOUR;
    seconds += this->_Minute * SECS_PER_MIN;
    seconds += this->_Second;
    return (time_t) seconds;
}

time_t TimeProvider::now()
{
    // calculate number of seconds passed since last call to now()
    while (millis() - prevMillis >= 1000) {
        // millis() and prevMillis are both unsigned ints thus the subtraction will always be the absolute value of the difference
        sysTime++;
        prevMillis += 1000;
    }
    if (nextSyncTime <= sysTime) {
        if (this->getTimePtr != 0) {
            time_t t = getTimePtr();
            if (t != 0) {
                setTime(t);
            } else {
                nextSyncTime = sysTime + syncInterval;
                Status =
                        (Status == timeStatus_t::timeNotSet) ?
                                timeStatus_t::timeNotSet : timeStatus_t::timeNeedsSync;
            }
        }
    }
    return (time_t) sysTime;
}
