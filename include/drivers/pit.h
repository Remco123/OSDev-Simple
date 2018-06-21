#ifndef MYOS__DRIVERS__PIT_h
#define MYOS__DRIVERS__PIT_h

#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/port.h>

namespace myos
{
    namespace drivers
    {
        class PIT : public hardwarecommunication::InterruptHandler
        {
        private:
            hardwarecommunication::Port8Bit DataPort;
            hardwarecommunication::Port8Bit CommandPort;

            common::uint32_t timer_ticks;
        public:
            PIT(hardwarecommunication::InterruptManager* manager);

            common::uint32_t HandleInterrupt(myos::common::uint32_t esp);
            void Sleep(common::uint32_t ms);

            //PCSpeaker
            void PlaySound(common::uint32_t nFrequence);
            void NoSound();

            void Beep();
            void Beep(common::uint32_t freq);
            void Beep(common::uint32_t freq, common::uint32_t duration);
        };
    }
}

#endif