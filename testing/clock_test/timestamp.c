uint16_t days_from_year(uint16_t year) {
    return (year-1970) * 365 + (year-1968)/4;
}

uint16_t days_from_month(uint8_t month, bool leap) {
    uint16_t days[12] = {0, 31, 59, 90, 120, 151, 181, 212, 233, 263, 294, 324};
    
    if (leap) {
        for (int i = 2; i < 12; i++) {
          days[i] += 1;
        }
    }
    return days[month - 1];
}

int32_t seconds_from_date(uint8_t second, uint8_t minute, uint8_t hour, uint8_t day, uint8_t month, uint16_t year) {
    uint16_t days = days_from_year(year);
    days += days_from_month(month, year%4 == 0);
    int32_t seconds = days*86400 + minute*60 + hour*3600 + second;
    return seconds;
}
