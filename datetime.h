#pragma once
#include <iostream>
#include <cassert>

namespace DateTime {

	enum class weekday { sunday, monday, tuesday, wednesday, thursday, friday, saturday };



	//----------------------------------------------- Class TimeDiff -----------------------------------------------------------------
	class timediff {
	private:
		int sec = 0;
		int min = 0;
		int h = 0;
		int d;
	public:

		timediff(int days, int hours, int minutes, int seconds);
		timediff(int total_sec);

		int days() const;
		int hours() const;
		int minutes() const;
		int seconds() const;

		//int JulianDate(int day, int month, int year) const;

		int total_hours() const;
		int total_minutes() const;
		int total_seconds() const;

		bool operator ==(const timediff& ts) const;

		bool operator != (const timediff& ts) const;

		bool operator < (const timediff& ts) const;

		bool operator <= (const timediff& ts) const;

		bool operator > (const timediff& ts) const;

		bool operator >= (const timediff& ts) const;

		timediff operator -(const timediff& ts) const;

		timediff operator +(const timediff& ts) const;

	};


	//----------------------------------------------- Class Time -----------------------------------------------------------------

	class time {
	private:

		int hh;
		int mm;
		int ss;

	public:

		time(int hour, int minute, int second);

		int hour() const;
		int minute() const;
		int second() const;

		long long total_seconds() const;
		void add_hours(int nb_hours);
		void add_minutes(int nb_minutes);
		void add_seconds(int nb_seconds);

		bool operator == (const time& tm) const;

		bool operator != (const time& tm) const;

		bool operator < (const time& tm) const;

		bool operator <= (const time& tm) const;

		bool operator > (const time& tm) const;

		bool operator >= (const time& tm) const;

		timediff operator- (const time& tm) const;

	};


	//----------------------------------------------- Class Date -------------------------------------------------------------------


	class date {

	private:

		int y;
		int m;
		int d;

	public:

		date(int year, int month, int day);

		static bool dateChecker(int day, int month, int year) {

			int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

			if (date::is_leapyear(year)) {
				daysInMonth[1] = 29;
			}

			if (month < 1 || month > 12) {
			return false;
			}

			if (day < 1 || day > daysInMonth[month - 1]) {
			return false;
			}

		return true;
		}

		int year() const;
		int month() const;
		int day() const;

		DateTime::weekday weekday() const;

		void add_days(int nb_days);

		date next() const;

		date prev() const;

		static int JulianDate(int day, int month, int year) {

			int a = (14 - month) / 12;
			int v = year + 4800 - a;
			int h = month + 12 * a - 3;

		return day + (153 * h + 2) / 5 + 365 * v + v / 4 - v / 100 + v / 400 - 32045;
		}

		static bool is_leapyear(int year);

		bool is_leapyear() const;

		bool operator == (const date& dt) const;

		bool operator != (const date& dt) const;

		bool operator < (const date& dt) const;

		bool operator <= (const date& dt) const;

		bool operator > (const date& dt) const;

		bool operator >= (const date& dt) const;

		//post
		date operator++ (int);

		//pre
		date& operator++ ();

		//post
		date operator-- (int);

		//pre
		date& operator-- ();


		timediff operator- (const date& dt) const;

	};


	//---------------------------------------------- Class DateTime -----------------------------------------------------------------


	class datetime {
	private:
		date dt;
		time tm;
	public:

		datetime() : dt(1970, 1, 1), tm(0, 0, 0) {};

		long long total_seconds() const;

		datetime(int year, int month, int day, int hour = 0, int minute = 0, int second = 0);

		datetime(const date& dt, const time& tm);

		bool is_leapyear() const;

		static bool is_leapyear(int year);

		int year() const;
		int month() const;
		int day() const;
		int hour() const;
		int minute() const;
		int second() const;

		DateTime::weekday weekday() const;

		void add_days(int nb_days);
		void add_hours(int nb_hours);
		void add_minutes(int nb_minutes);
		void add_seconds(int nb_seconds);


		static int JulianDate(int day, int month, int year) {
			int a = (14 - month) / 12;
			int y = year + 4800 - a;
			int m = month + 12 * a - 3;
		return day + (153 * m + 2) / 5 + y * 365 + y / 4 - y / 100 + y / 400 - 32045;
		}


		bool operator== (const datetime& other) const;

		bool operator!= (const datetime& other) const;

		bool operator< (const datetime& other) const;

		bool operator<= (const datetime& other) const;

		bool operator> (const datetime& other) const;

		bool operator>= (const datetime& other) const;

		timediff operator-(const datetime& dt) const;

		datetime operator+ (const timediff& ts) const;

		datetime operator- (const timediff& ts) const;

		static datetime dateSeparator(const char* dateStr) {
			int day;
			int month;
			int year; 
			int hour;
			int minute;
			int second;
			sscanf(dateStr, "%d.%d.%d %d:%d:%d", &day, &month, &year, &hour, &minute, &second);
			
			return datetime(year, month, day, hour, minute, second);
		}

	};




}
