#include <drivers/rtc.h>

using namespace myos;
using namespace myos::common;
using namespace myos::drivers;

RTC::RTC()
:   addressPort(0x70),
    dataPort(0x71)
{
}

bool RTC::get_update_in_progress_flag()
{
    addressPort.Write(0x0A);
    return (dataPort.Read() & 0x80);
}
uint8_t RTC::get_RTC_register(int reg)
{
    addressPort.Write(reg);
    return dataPort.Read();
}

uint32_t RTC::GetSecond()
{
    while(get_update_in_progress_flag());
    uint32_t value = get_RTC_register(0x00);
    uint32_t registerB = get_RTC_register(0x0B);

    if(!(registerB & 0x04))
        value = (value & 0x0F) + ((value / 16) * 10); // Convert BCD to binary values if necessary
    return value;
}
uint32_t RTC::GetMinute()
{
    while(get_update_in_progress_flag());
    uint32_t value = get_RTC_register(0x02);
    uint32_t registerB = get_RTC_register(0x0B);

    if(!(registerB & 0x04))
        value = (value & 0x0F) + ((value / 16) * 10); // Convert BCD to binary values if necessary
    return value;
}
uint32_t RTC::GetHour()
{
    while(get_update_in_progress_flag());
    uint32_t value = get_RTC_register(0x04);
    uint32_t registerB = get_RTC_register(0x0B);

    if(!(registerB & 0x04))
        value = ((value & 0x0F) + (((value & 0x70) / 16) * 10) ) | (value & 0x80); // Convert BCD to binary values if necessary
    
    if(!(registerB & 0x02) && (value & 0x80))
        value = ((value & 0x7F) + 12) % 24;  // Convert 12 hour clock to 24 hour clock if necessary

    return value;
}
uint32_t RTC::GetDay()
{
    while(get_update_in_progress_flag());
    uint32_t value = get_RTC_register(0x07);
    uint32_t registerB = get_RTC_register(0x0B);

    if(!(registerB & 0x04))
        value = (value & 0x0F) + ((value / 16) * 10); // Convert BCD to binary values if necessary
    return value;
}
uint32_t RTC::GetMonth()
{
    while(get_update_in_progress_flag());
    uint32_t value = get_RTC_register(0x08);
    uint32_t registerB = get_RTC_register(0x0B);

    if(!(registerB & 0x04))
        value = (value & 0x0F) + ((value / 16) * 10); // Convert BCD to binary values if necessary
    return value;
}
uint32_t RTC::GetYear()
{
    while(get_update_in_progress_flag());
    uint32_t value = get_RTC_register(0x09);
    uint32_t registerB = get_RTC_register(0x0B);

    if(!(registerB & 0x04))
        value = (value & 0x0F) + ((value / 16) * 10); // Convert BCD to binary values if necessary
    
    // Calculate the full (4-digit) year
    value += (CURRENT_YEAR / 100) * 100;
    if(value < CURRENT_YEAR)
        value += 100;

    return value;
}