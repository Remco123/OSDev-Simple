#include <system.h>

using namespace myos;

GlobalDescriptorTable*                                            System::GDT                = 0;
MemoryManager*                                                    System::memManager         = 0;
TaskManager*                                                      System::taskManager        = 0;
hardwarecommunication::InterruptManager*                          System::interruptsManager  = 0;
drivers::DriverManager*                                           System::driverManager      = 0;
hardwarecommunication::PeripheralComponentInterconnectController* System::pci                = 0;
drivers::PIT*                                                     System::pit                = 0;
drivers::RTC*                                                     System::rtc                = 0;
gui::Shell*                                                       System::shell              = 0;
gui::Canvas*                                                      System::canvas             = 0;