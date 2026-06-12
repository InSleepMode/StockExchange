#include "datetime.h"
#include <stdexcept>
#include <sstream>
#include <iostream>
using namespace DateTime;
//----------------------------------------------- Class TimeDiff ---------------------------------------------------------------

timediff::timediff(int days, int hours, int minutes, int seconds) : d(days), h(hours), min(minutes), sec(seconds) {

	if (sec < -59 || sec > 59) {
		throw std::invalid_argument("seconds must be between -59 and 59");
	}
	if (min < -59 || min > 59) {
		throw std::invalid_argument("minutes must be between -59 and 59");
	}
	if (h < -23 || h > 23) {
		throw std::invalid_argument("hours must be between -23 and 23");
	}
}

timediff::timediff(int total_sec) {

	d = total_sec / (24 * 3600);
	total_sec %= (24 * 3600);
	h = total_sec / 3600;
	total_sec %= 3600;
	min = total_sec / 60;
	total_sec %= 60;
	sec = total_sec;

}

int timediff::days() const { return d; }
int timediff::hours() const { return h; }
int timediff::minutes() const { return min; }
int timediff::seconds() const { return sec; }

int timediff::total_hours() const {
	return d * 24 + h;
}
int timediff::total_minutes() const {
	return (d * 24 * 60) + (h * 60) + min;
}
int timediff::total_seconds() const {
	return (d * 24 * 3600) + (h * 3600) + (min * 60) + sec;
}

bool timediff::operator ==(const timediff& ts) const {
	return (total_seconds() == ts.total_seconds());
}

bool timediff::operator != (const timediff& ts) const {
	return (total_seconds() != ts.total_seconds());
}

bool timediff::operator < (const timediff& ts) const {
	return(total_seconds() < ts.total_seconds());
}

bool timediff::operator <= (const timediff& ts) const {
	return(total_seconds() <= ts.total_seconds());
}

bool timediff::operator > (const timediff& ts) const {
	return(total_seconds() > ts.total_seconds());
}

bool timediff::operator >= (const timediff& ts) const {
	return(total_seconds() >= ts.total_seconds());
}

timediff timediff::operator-(const timediff& ts) const {
	return timediff(total_seconds() - ts.total_seconds());
}

timediff timediff::operator+(const timediff& ts) const {
	return timediff(total_seconds() + ts.total_seconds());
}
//---------------------------------------------- Class Time -------------------------------------------------------------------

time::time(int hour, int minute, int second) {

	if (second < 0 || second > 59) {
		throw std::invalid_argument("seconds must be between 0 and 59");
	}
	if (minute < 0 || minute > 59) {
		throw std::invalid_argument("minutes must be between 0 and 59");

	}
	if (hour < 0 || hour > 23) {
		throw std::invalid_argument("hours must be between 0 and 23");
	}

	mm = minute;
	ss = second;
	hh = hour;

	mm += ss / 60;
	ss %= 60;

	if (ss < 0) {
		mm -= 1;
		ss += 60;
	}

	hh += mm / 60;
	mm %= 60;
	if (mm < 0) {
		hh -= 1;
		mm += 60;
	}

	hh %= 24;
	if (hh < 0) {
		hh += 24;
	}


}

int time::hour() const { return hh; }
int time::minute() const { return mm; }
int time::second() const { return ss; }

long long time::total_seconds() const {
	return hh * 3600 + mm * 60 + ss;
}

void time::add_hours(int nb_hours) {
	hh = (hh + nb_hours) % 24;
	if (hh < 0) {
		hh += 24;
	}
}

void time::add_minutes(int nb_minutes) {
	mm += nb_minutes;
	while (mm > 59) {
		hh += mm / 60;
		mm %= 60;
	}
	while (mm < 0) {
		hh -= 1;
		mm += 60;
	}
	hh %= 24;
	if (hh < 0) {
		hh += 24;
	}
}

void time::add_seconds(int nb_seconds) {

	ss += nb_seconds;
	while (ss > 59) {
		mm += ss / 60;
		ss %= 60;
	}
	while (mm > 59) {
		hh += mm / 60;
		mm %= 60;
	}
	while (hh > 23) {
		hh = 23;
		mm = 59;
		ss = 59;
	}

	while (ss < 0) {
		ss += 60;
		mm -= 1;
	}

	while (mm < 0) {
		mm += 60;
		hh -= 1;
	}
	while (hh < 0) {
		hh = 0;
		mm = 0;
		ss = 0;
	}

}

bool time::operator == (const time& tm) const {
	return (hh * 3600 + mm * 60 + ss) == (tm.hh * 3600 + tm.mm * 60 + tm.ss);
}

bool time::operator != (const time& tm) const {
	return (hh * 3600 + mm * 60 + ss) != (tm.hh * 3600 + tm.mm * 60 + tm.ss);
}

bool time::operator < (const time& tm) const {
	return (hh * 3600 + mm * 60 + ss) < (tm.hh * 3600 + tm.mm * 60 + tm.ss);
}

bool time::operator <= (const time& tm) const {
	return (hh * 3600 + mm * 60 + ss) <= (tm.hh * 3600 + tm.mm * 60 + tm.ss);
}

bool time::operator > (const time& tm) const {
	return (hh * 3600 + mm * 60 + ss) > (tm.hh * 3600 + tm.mm * 60 + tm.ss);
}

bool time::operator >= (const time& tm) const {
	return (hh * 3600 + mm * 60 + ss) >= (tm.hh * 3600 + tm.mm * 60 + tm.ss);
}

timediff time::operator-(const time& tm) const {
	int totalSeconds1 = hh * 3600 + mm * 60 + ss;
	int totalSeconds2 = tm.hh * 3600 + tm.mm * 60 + tm.ss;
	int res = totalSeconds1 - totalSeconds2;
	return timediff(res);
}



//----------------------------------------------- Class Date -------------------------------------------------------------------


date::date(int year, int month, int day) {

	if (!date::dateChecker(day, month, year)) {
		throw std::invalid_argument("Date is incorrect");

	}

	y = year;
	m = month;
	d = day;
}




int date::year() const { return y; }
int date::month() const { return m; }
int date::day() const { return d; }


DateTime::weekday DateTime::date::weekday() const {

	int a = (14 - m) / 12;
	int z = y - a;
	int v = m + 12 * a - 2;

	int l = (d + z + z / 4 - z / 100 + z / 400 + 31 * v / 12) % 7;

	return static_cast<DateTime::weekday>(l);

}


void date::add_days(int nb_days) {

	int newJulianDate = JulianDate(d, m, y) + nb_days;

	int a = newJulianDate + 32044;
	int b = (4 * a + 3) / 146097;
	int c = a - (146097 * b) / 4;
	int v = (4 * c + 3) / 1461;
	int e = c - (1461 * v) / 4;
	int h = (5 * e + 2) / 153;

	d = e - ((153 * h + 2) / 5) + 1;
	m = h + 3 - 12 * (h / 10);
	y = 100 * b + v - 4800 + (h / 10);

}

date date::next() const {
	date nextDate = *this;
	nextDate.add_days(1);
	return nextDate;
}

date date::prev() const {
	date prevDate = *this;
	prevDate.add_days(-1);
	return prevDate;
}






bool date::is_leapyear(int year) {

	if (year % 4 == 0) {
		if (year % 100 != 0 || year % 400 == 0) {
			return true;
		}
	}
	return false;
}

bool date::is_leapyear() const {
	return is_leapyear(y);
}


bool date::operator == (const date& dt) const {
	return (JulianDate(d, m, y)) == (JulianDate(dt.d, dt.m, dt.y));
}

bool date::operator != (const date& dt) const {
	return (JulianDate(d, m, y)) != (JulianDate(dt.d, dt.m, dt.y));
}

bool date::operator < (const date& dt) const {
	return (JulianDate(d, m, y)) < (JulianDate(dt.d, dt.m, dt.y));
}

bool date::operator <= (const date& dt) const {
	return (JulianDate(d, m, y)) <= (JulianDate(dt.d, dt.m, dt.y));
}

bool date::operator > (const date& dt) const {
	return (JulianDate(d, m, y)) > (JulianDate(dt.d, dt.m, dt.y));
}

bool date::operator >= (const date& dt) const {
	return (JulianDate(d, m, y)) >= (JulianDate(dt.d, dt.m, dt.y));
}


//post
date date::operator++ (int) {
	date temp = *this;
	add_days(1);
	return temp;
}

//pre
date& date::operator++ () {
	add_days(1);
	return *this;
}

//post
date date::operator-- (int) {
	date temp = *this;
	add_days(-1);
	return temp;
}

//pre
date& date::operator-- () {
	add_days(-1);
	return *this;
}

timediff date::operator-(const date& dt) const {
	int julDays = JulianDate(d, m, y) - JulianDate(dt.d, dt.m, dt.y);
	return timediff(julDays, 0, 0, 0);
}


//---------------------------------------------- Class DateTime -----------------------------------------------------------------


long long datetime::total_seconds() const {
	return static_cast<long long>(DateTime::date::JulianDate(dt.day(), dt.month(), dt.year()) * 86400) + tm.total_seconds();
}

void FromJulianDate(int julian, int& day, int& month, int& year) {
	int a = julian + 32044;
	int b = (4 * a + 3) / 146097;
	a = a - (146097 * b) / 4;
	int c = (4 * a + 3) / 1461;
	a = a - (1461 * c) / 4;
	int m = (5 * a + 2) / 153;
	day = a - (153 * m + 2) / 5 + 1;
	month = m + 3 - 12 * (m / 10);
	year = 100 * b + c - 4800 + (m / 10);
}


datetime::datetime(int year, int month, int day, int hour, int minute, int second) : dt(year, month, day), tm(hour, minute, second) {



	if (month < 1 || month > 12) {
		throw std::invalid_argument("month must be between 1 and 12");
	}

	int daysInMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	if (is_leapyear(year)) {
		daysInMonth[1] = 29;
	}

	if (day < 1 || day > daysInMonth[month - 1]) {
		throw std::invalid_argument("days must be between 1 and 31");
	}

	if (hour < 0 || hour > 23) {
		throw std::invalid_argument("seconds must be between 1 and 23");
	}

	if (minute < 0 || minute > 59) {
		throw std::invalid_argument("minutes must be between 1 and 59");
	}

	if (second < 0 || second > 59) {
		throw std::invalid_argument("seconds must be between 1 and 59");
	}

}


datetime::datetime(const date& dt, const time& tm) : dt(dt), tm(tm) { }

bool datetime::is_leapyear() const {
	return is_leapyear(dt.year());
}

bool datetime::is_leapyear(int year) {

	if (year % 4 == 0) {
		if (year % 100 != 0 || year % 400 == 0) {
			return true;
		}
	}
	return false;
}

int datetime::year() const { return dt.year(); }
int datetime::month() const { return dt.month(); }
int datetime::day() const { return dt.day(); }
int datetime::hour() const { return tm.hour(); }
int datetime::minute() const { return tm.minute(); }
int datetime::second() const { return tm.second(); }

DateTime::weekday DateTime::datetime::weekday() const {
	return dt.weekday();
}

void datetime::add_days(int nb_days) {
	dt.add_days(nb_days);

}

void datetime::add_hours(int nb_hours) {
	int extraDays = nb_hours / 24; // Количество полных дней в добавляемых часах
	int remainedH = nb_hours % 24;  // Оставшиеся часы

	// Добавляем оставшиеся часы
	int oldHour = tm.hour(); // Сохраняем текущий час
	tm.add_hours(remainedH);

	// Если часы переполнились (например, 23 + 2 = 25, но tm.hour() вернет 1), увеличиваем день
	if (tm.hour() < oldHour) {
		extraDays++;
	}

	// Добавляем дополнительные дни
	if (extraDays != 0) {
		dt.add_days(extraDays);
	}
}


void datetime::add_minutes(int nb_minutes) {
	int old_hour = tm.hour(); // Сохраняем текущий час

	tm.add_minutes(nb_minutes); // Добавляем минуты

	// Проверяем, произошел ли переход через полночь
	if (tm.hour() < old_hour) {
		// Если час уменьшился, увеличиваем дату на 1 день
		dt.add_days(1);
	}

	// Обрабатываем дополнительные часы (если минуты > 60)
	int extraHours = nb_minutes / 60;
	if (extraHours != 0) {
		add_hours(extraHours);
	}
}


void datetime::add_seconds(int nb_seconds) {
	// Преобразуем текущее время в общее количество секунд
	long long total_sec = tm.total_seconds() + nb_seconds;

	// Вычисляем количество полных дней в добавленных секундах
	long long extra_days = total_sec / 86400;
	total_sec %= 86400;

	if (total_sec < 0) {
		extra_days--;
		total_sec += static_cast<long long>(24 * 3600);
	}

	// Устанавливаем новое время
	tm = time(static_cast<int>(total_sec / 3600), static_cast<int>((total_sec % 3600) / 60), static_cast<int>(total_sec % 60));

	// Добавляем дополнительные дни
	if (extra_days != 0) {
		dt.add_days(static_cast<int>(extra_days));
	}
}



bool datetime::operator == (const datetime& other) const {
	return dt == other.dt && tm == other.tm;
}

bool datetime::operator != (const datetime& other) const {
	return dt != other.dt || tm != other.tm;

}

bool datetime::operator < (const datetime& other) const {
	return dt < other.dt || (dt == other.dt && tm < other.tm);

}

bool datetime::operator <= (const datetime& other) const {
	return dt <= other.dt && tm <= other.tm;
}

bool datetime::operator > (const datetime& other) const {
	return dt > other.dt || (dt == other.dt && tm > other.tm);
}

bool datetime::operator >= (const datetime& other) const {
	return dt >= other.dt && tm >= other.tm;
}

timediff datetime::operator-(const datetime& dt) const {
	long long sec1 = total_seconds();
	long long sec2 = dt.total_seconds();
	return timediff(static_cast<int>(sec1) - static_cast<int>(sec2));
}

datetime datetime::operator+(const timediff& ts) const {
	datetime result = *this;
	result.add_seconds(ts.total_seconds());
	return result;
}

datetime datetime::operator-(const timediff& ts) const {
	datetime result = *this;
	result.add_seconds(-ts.total_seconds());
	return result;
}


