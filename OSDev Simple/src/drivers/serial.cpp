#include <drivers/serial.h>

using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;

void printf(char*);

// ------------------
void outportb(uint8_t p, uint8_t data){
    Port8Bit port(p);
    port.Write(data);
}
// ------------------

SerialPort::SerialPort(SerialCom port)
{
    this->port = port;
}

bool SerialPort::DataAvailibe()
{

}

void SerialPort::Send(uint8_t* data)
{

}

uint8_t* SerialPort::Receive()
{

}

// ------------------------------------------


SerialPortManager::SerialPortManager(myos::hardwarecommunication::InterruptManager* manager)
: InterruptHandler(manager, 0x04)
{
}

void SerialPortManager::EnableSerial(SerialCom port)
{
    //outportb(port + 1, 0x00);
	outportb(port + 3, 0x80); /* Enable divisor mode */
	outportb(port + 0, 0x03); /* Div Low:  03 Set the port to 38400 bps */
	outportb(port + 1, 0x00); /* Div High: 00 */
	outportb(port + 3, 0x03);
	outportb(port + 2, 0xC7);
    outportb(port + 4, 0x0B);

    outportb(port + 1, 0x01); //Enable interrupts on device
}

uint32_t SerialPortManager::HandleInterrupt(uint32_t esp)
{
    printf("GOT COM INTERRUPT");

    return esp;
}

SerialPort SerialPortManager::GetInstance(SerialCom port)
{
    SerialPort result(port);
    EnableSerial(port);
    return result;
}