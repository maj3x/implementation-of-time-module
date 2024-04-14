#include "date.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PRIVATE static
#define YEARBASE 1900
#define PUBLIC
#define RANDOM_YEAR_MIN 1940
#define RANDOM_YEAR_MAX 2023

#define ISLEAP(y) ((y) % 4 == 0 && ((y) % 100 != 0 || (y) % 400 == 0))
#define MDAY(y, m) (daytabs[ISLEAP(y)][m])
#define MONTH(pdate) (pdate->mon_)
#define MONTH_DAY(pdate) (pdate->day_)
#define YEAR(pdate) (pdate->year_)

PRIVATE const int daytabs[][13] = {
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
};

PRIVATE Date *set(Date *p, int day, int mon, int year);
PRIVATE int is_valid_date(int day, int mon, int year);
PRIVATE int day_of_week(int day, int mon, int year);
PRIVATE int totaldays(const Date *p);
PRIVATE Date *int_to_date(Date *p, int totaldays);

// PUBLIC FUNCTIONS
PUBLIC Date *set_date(Date *p, int day, int mon, int year) {

  return set(p, day, mon, year);
}

// GG-AA-YYYY
PUBLIC Date *set_date_from_str(Date *p, const char *pstr) {

  int day = atoi(pstr);
  int mon = atoi(pstr + 3);
  int year = atoi(pstr + 6);

  return set(p, day, mon, year);
}

PUBLIC Date *set_date_time_t(Date *p, time_t sec) {

  struct tm *tptr = localtime(&sec);
  int day = tptr->tm_mday;
  int mon = tptr->tm_mon + 1;
  int year = tptr->tm_year + 1900;

  return set(p, day, mon, year);
}

PUBLIC Date *set_date_today(Date *p) {
  time_t sec;
  time(&sec);

  return set_date_time_t(p, sec);
}

PUBLIC Date *set_date_random(Date *p) {
  int year = rand() % (RANDOM_YEAR_MAX - RANDOM_YEAR_MIN + 1) + RANDOM_YEAR_MIN;
  int mon = rand() % 12 + 1;
  int day = rand() % MDAY(year, mon) + 1;

  return set_date(p, day, mon, year);
}

PUBLIC Date *set_year(Date *p, int year) {

  return set_date(p, MONTH_DAY(p), MONTH(p), year);
}

PUBLIC Date *set_month(Date *p, int mon) {

  return set_date(p, MONTH_DAY(p), mon, YEAR(p));
}

PUBLIC Date *set_month_day(Date *p, int mday) {

  return set_date(p, mday, MONTH(p), YEAR(p));
}

//-------------------------------------------------------------
PUBLIC int get_year(const Date *p) { return YEAR(p); }
PUBLIC int get_month(const Date *p) { return MONTH(p); }
PUBLIC int get_mont_day(const Date *p) { return MONTH_DAY(p); }
PUBLIC int get_yearday(const Date *p) {
  int sum = MONTH_DAY(p);

  for (int i = 1; i < MONTH(p); ++i) {
    sum += daytabs[ISLEAP(YEAR(p))][i];
  }
  return sum;
}

PUBLIC int get_weekday(const Date *p) {
  return day_of_week(MONTH_DAY(p), MONTH(p), YEAR(p));
}

//---------------------------------------------------------------

PUBLIC void print_date(const Date *p) {
  static const char *const pmons[] = {
      "",       "Ocak",    "Subat", "Mart", "Nisan", "Mayis", "Haziran",
      "Temmuz", "Agustos", "Eylul", "Ekim", "Kasim", "Aralik"};
  static const char *const pdays[] = {
      "Pazar", "Pazartesi", "Sali", "Carsamba", "Persembe", "Cuma", "Cumartesi",
  };

  printf("%02d %s %d %s\n", MONTH_DAY(p), pmons[MONTH(p)], YEAR(p),
         pdays[get_weekday(p)]);
}

PUBLIC Date *scandate(Date *p) {
  int day, month, year;

  scanf("%d%d%d", &day, &month, &year);

  return set_date(p, day, month, year);
}

PUBLIC int cmp_date(const Date *p1, const Date *p2) {
  if (YEAR(p1) != YEAR(p2)) {
    return YEAR(p1) - YEAR(p2);
  }
  if (MONTH(p1) != MONTH(p2)) {
    return MONTH(p1) - MONTH(p2);
  }
  return MONTH_DAY(p1) - MONTH_DAY(p2);
}

PUBLIC int get_date_diff(const Date *p1, const Date *p2) {
  return abs(totaldays(p1) - totaldays(p2));
}

PUBLIC Date *ndays_after(Date *p, const Date *ps, int n) {
  return int_to_date(p, totaldays(ps) + n);
}
PUBLIC Date *ndays_before(Date *p, const Date *ps, int n) {
  return int_to_date(p, totaldays(ps) - n);
}

//---------------------------------------------------------------
// PRIVATE FUNCTIONS
PRIVATE int is_valid_date(int day, int mon, int year) {

  return year >= YEARBASE && mon > 0 && mon <= 12 && day > 0 &&
         day <= MDAY(year, mon);
}

PRIVATE Date *set(Date *p, int day, int mon, int year) {
  if (!is_valid_date(day, mon, year)) {
    fprintf(stderr, "invalid date!\n");
    exit(EXIT_FAILURE);
  }

  p->day_ = day;
  p->mon_ = mon;
  p->year_ = year;

  return p;
}

PRIVATE int day_of_week(int d, int m, int y) {
  static const int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};

  if (m < 3)
    y -= 1;

  return ((y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7);
}

PRIVATE int totaldays(const Date *p) {
  int sum = get_yearday(p);

  for (int y = YEARBASE; y < YEAR(p); ++y) {
    sum += ISLEAP(y) ? 366 : 365;
  }

  return sum;
}

PRIVATE Date *int_to_date(Date *p, int totaldays) {
  int year = YEARBASE;

  while (totaldays > (ISLEAP(year) ? 366 : 365)) {
    totaldays -= ISLEAP(year) ? 366 : 365;
    ++year;
  }

  int mon = 1;

  while (totaldays > MDAY(year, mon)) {
    totaldays -= MDAY(year, mon);
    ++mon;
  }

  int day = totaldays;

  return set(p, day, mon, year);
}