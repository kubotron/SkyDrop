#include "time.h"

void widget_time_draw(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t flags)
{
	uint8_t sec;
	uint8_t min;
	uint8_t hour;

	time_from_epoch(time_get_local(), &sec, &min, &hour);

	uint8_t lh = widget_label_P(PSTR("Time"), x, y);

	char tmp[7];
	sprintf_P(tmp, PSTR("%02d:%02d"), hour, min);

	widget_value_int(tmp, x, y + lh, w, h - lh);
}

void widget_date_draw(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t flags)
{
	uint8_t sec;
	uint8_t min;
	uint8_t hour;

	uint8_t day;
	uint8_t wday;
	uint8_t month;
	uint16_t year;

	datetime_from_epoch(time_get_local(), &sec, &min, &hour, &day, &wday, &month, &year);

	uint8_t lh = widget_label_P(PSTR("Date"), x, y);

	char tmp[7];
	sprintf_P(tmp, PSTR("%02d/%02d"), day, month);

	widget_value_int(tmp, x, y + lh, w, h - lh);
}

void widget_flight_time_draw(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t flags)
{
	uint8_t lh = widget_label_P(PSTR("FTime"), x, y);

	char tmp[7];
	if (fc.flight.state == FLIGHT_FLIGHT)
	{
		uint32_t diff = (task_get_ms_tick() - fc.flight.timer) / 1000;
		uint8_t hour, min;

		hour = diff / 3600;
		diff %= 3600;

		min = diff / 60;
		diff %= 60;

		if (hour > 0)
		{
			sprintf_P(tmp, PSTR("%02d:%02d"), hour, min);
		}
		else
			sprintf_P(tmp, PSTR("%02d.%02d"), min, diff);

		widget_value_int(tmp, x, y + lh, w, h - lh);
	}

	if (fc.flight.state == FLIGHT_LAND)
	{
		uint32_t diff = fc.flight.timer / 1000;
		uint8_t hour, min;

		hour = diff / 3600;
		diff %= 3600;

		min = diff / 60;
		diff %= 60;

		if (hour > 0)
		{
			sprintf_P(tmp, PSTR("%02d:%02d"), hour, min);
		}
		else
			sprintf_P(tmp, PSTR("%02d.%02d"), min, diff);

		widget_value_int(tmp, x, y + lh, w, h - lh);
	}

	if (fc.flight.state == FLIGHT_WAIT)
	{
		sprintf_P(tmp, PSTR("Start"));
		widget_value_txt(tmp, x, y + lh, w, h - lh);
	}
}

void widget_flight_time_irqh(uint8_t type, uint8_t * buff, uint8_t index)
{
	if (type == B_MIDDLE && *buff == BE_LONG)
	{
		if (fc.flight.state == FLIGHT_WAIT)
		{
			fc_takeoff();
			return;
		}

		if (fc.flight.state == FLIGHT_FLIGHT)
		{
			fc_landing();
			return;
		}

		if (fc.flight.state == FLIGHT_LAND)
		{
			fc_reset();
			return;
		}
	}
}

register_widget1(w_time, "Time", widget_time_draw);
register_widget1(w_date, "Date", widget_date_draw);
register_widget2(w_ftime, "Flight time", widget_flight_time_draw, 0, widget_flight_time_irqh);
