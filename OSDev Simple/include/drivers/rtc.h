#ifndef __MYOS__DRIVERS__RTC_H
#define __MYOS__DRIVERS__RTC_H

#define CURRENT_YEAR 2018

#include <common/types.h>
#include <hardwarecommunication/port.h>

namespace myos
{
    namespace drivers
    {
        class RTC
        {
        private:
            hardwarecommunication::Port8Bit addressPort;
            hardwarecommunication::Port8Bit dataPort;
        public:
            RTC();
            ~RTC();

            bool get_update_in_progress_flag();
            common::uint8_t get_RTC_register(int reg);

            common::uint32_t GetSecond();
            common::uint32_t GetMinute();
            common::uint32_t GetHour();
            common::uint32_t GetDay();
            common::uint32_t GetMonth();
            common::uint32_t GetYear();
        };
    }
}

#endif