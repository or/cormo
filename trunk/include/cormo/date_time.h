/* Copyright (C) 2009 Oliver Runge (thi@thialfihar.org)
 *
 * This file is part of cormo.
 *
 * cormo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * cormo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with cormo.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CORMO_INCLUDE_CORMO_DATE_TIME_H_
#define CORMO_INCLUDE_CORMO_DATE_TIME_H_

#include <time.h>
#include <stdio.h>

#include <string>

namespace cormo {

class DateTime {
 public:
  static const int kDefaultTimeZone;
  DateTime() : year_(0), month_(0), day_(0),
               hour_(0), minute_(0), second_(0),
               time_zone_(kDefaultTimeZone) { }
  explicit DateTime(time_t timestamp) : year_(0), month_(0), day_(0),
                                        hour_(0), minute_(0), second_(0),
                                        time_zone_(kDefaultTimeZone) {
    struct tm tmp;
    localtime_r(&timestamp, &tmp);
    year_   = tmp.tm_year + 1900;
    month_  = tmp.tm_mon;
    day_    = tmp.tm_mday;
    hour_   = tmp.tm_hour;
    minute_ = tmp.tm_min;
    second_ = tmp.tm_sec;
  }
  explicit DateTime(const std::string value) : year_(0), month_(0), day_(0),
                                               hour_(0), minute_(0), second_(0),
                                               time_zone_(kDefaultTimeZone) {
    if (sscanf(value.c_str(), "%04d-%02d-%02d %02d:%02d:%02d+%02d",
               &year_, &month_, &day_,
               &hour_, &minute_, &second_, &time_zone_) == 7) {
      return;
    } else if (sscanf(value.c_str(), "%04d-%02d-%02d %02d:%02d:%02d",
                    &year_, &month_, &day_, &hour_, &minute_, &second_) == 6) {
      return;
    } else if (sscanf(value.c_str(), "%04d-%02d-%02d",
                    &year_, &month_, &day_) == 3) {
      return;
    } else {
      // couldn't parse it... leave everything at 0
    }
  }

  static DateTime now() {
    DateTime obj;
    time_t timestamp = ::time(NULL);
    struct tm tmp;
    localtime_r(&timestamp, &tmp);
    obj.year_   = tmp.tm_year + 1900;
    obj.month_  = tmp.tm_mon;
    obj.day_    = tmp.tm_mday;
    obj.hour_   = tmp.tm_hour;
    obj.minute_ = tmp.tm_min;
    obj.second_ = tmp.tm_sec;

    return obj;
  }

  std::string ToString() const {
    char buf[32];
    snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d+%02d",
             year_, month_, day_,
             hour_, minute_, second_,
             time_zone_);
    return buf;
  }

 private:
  int year_, month_, day_;
  int hour_, minute_, second_;
  int time_zone_;
};

}  // namespace cormo

#endif  // CORMO_INCLUDE_CORMO_DATE_TIME_H_
