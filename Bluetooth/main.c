#include <stdint.h>
#include <stdbool.h>
#include "stdlib.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_timer.h"
#include "inc/hw_uart.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include <string.h>
#include "ServiceLayer/std_types.h"
#include "HAL/EF_Bluetooth.h"

int main(void)
{
//    U8_t Status[10] = {0};
    U8_t RxData_ptr[20] = {0} ;
    volatile U8_t status=0;
    U16_t iterator = 0;
    U8_t ReturnedAddresses [50] = {0};
    U8_t ReturnSlaveName[10] = {0};
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

    EF_BOOLEAN_Bluetooth_ModuleInit ( MASTER_MODE, "1234" , "EF_Bluetooth" , strlen ("EF_Bluetooth") );

    EF_BOOLEAN_Bluetooth_MasterScan ( 5, 9, ReturnedAddresses );

    for (iterator =0; ( (iterator < 3) && (status == 0) ); iterator++)
    {
        status = EF_BOOLEAN_Bluetooth_MasterConnect ( "E84E,84,D6FB48" , 15,  ReturnSlaveName );

    }
    while (1)
    {

//        EF_u8_Bluetooth_GetModuleStatus ( Status );

//        _delay_ms(1000);

        EF_BOOLEAN_Bluetooth_SendData ("It is Tiva\n", strlen ("It is Tiva\n") );
        {
            EF_BOOLEAN_Bluetooth_GetDataIfExist ( RxData_ptr );

        }

        _delay_ms(500);

    }


}
