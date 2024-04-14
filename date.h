#ifndef DATE_H
#define DATE_H

#include <time.h>

typedef struct {
  int day_;
  int mon_;
  int year_;
} Date;

// setters
Date *set_date(Date *p, int day, int mon, int year);
Date *set_date_from_str(Date *p, const char *pstr);
Date *set_date_time_t(Date *p, time_t sec);
Date *set_date_today(Date *p);
Date *set_date_random(Date *p);
Date *set_year(Date *p, int y);
Date *set_month(Date *p, int m);
Date *set_month_day(Date *p, int mday);

// getters
int get_year(const Date *);
int get_month(const Date *);
int get_mont_day(const Date *);
int get_weekday(const Date *);
int get_yearday(const Date *);

// utilities
int cmp_date(const Date *p1, const Date *p2);
int get_date_diff(const Date *p1, const Date *p2);

// formatted input/output funcs
void print_date(const Date *);
Date *scandate(Date *p);

Date *ndays_after(Date *p, const Date *ps, int n);
Date *ndays_before(Date *p, const Date *ps, int n);

#endif