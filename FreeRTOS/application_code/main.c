/*
Amazon FreeRTOS
Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 http://aws.amazon.com/freertos
 http://www.FreeRTOS.org
*/

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOS_IP.h"
#include <stdio.h>
#include <string.h>

/* Renesas */
#include "serial_term_uart.h"

/* Demo includes */
#include "aws_demo_runner.h"

/* Aws Library Includes includes. */
#include "aws_system_init.h"
//#include "aws_wifi.h"
#include "aws_clientcredential.h"
#include "aws_application_version.h"

#define mainLOGGING_TASK_STACK_SIZE         ( configMINIMAL_STACK_SIZE * 6 )
#define mainLOGGING_MESSAGE_QUEUE_LENGTH    ( 15 )
#define mainTEST_RUNNER_TASK_STACK_SIZE    ( configMINIMAL_STACK_SIZE * 8 )

/* Declare the firmware version structure for all to see. */
const AppVersion32_t xAppFirmwareVersion = {
   .u.x.ucMajor = APP_VERSION_MAJOR,
   .u.x.ucMinor = APP_VERSION_MINOR,
   .u.x.usBuild = APP_VERSION_BUILD,
};

/* The MAC address array is not declared const as the MAC address will
normally be read from an EEPROM and not hard coded (in real deployed
applications).*/
static uint8_t ucMACAddress[ 6 ] =
{
    configMAC_ADDR0,
    configMAC_ADDR1,
    configMAC_ADDR2,
    configMAC_ADDR3,
    configMAC_ADDR4,
    configMAC_ADDR5
}; //XXX

/* Define the network addressing.  These parameters will be used if either
ipconfigUDE_DHCP is 0 or if ipconfigUSE_DHCP is 1 but DHCP auto configuration
failed. */
static const uint8_t ucIPAddress[ 4 ] =
{
    configIP_ADDR0,
    configIP_ADDR1,
    configIP_ADDR2,
    configIP_ADDR3
};
static const uint8_t ucNetMask[ 4 ] =
{
	configNET_MASK0,
	configNET_MASK1,
	configNET_MASK2,
	configNET_MASK3
};
static const uint8_t ucGatewayAddress[ 4 ] =
{
	configGATEWAY_ADDR0,
	configGATEWAY_ADDR1,
	configGATEWAY_ADDR2,
	configGATEWAY_ADDR3
};

/* The following is the address of an OpenDNS server. */
static const uint8_t ucDNSServerAddress[ 4 ] =
{
	configDNS_SERVER_ADDR0,
	configDNS_SERVER_ADDR1,
	configDNS_SERVER_ADDR2,
	configDNS_SERVER_ADDR3
};

/**
 * @brief Application task startup hook.
 */
void vApplicationDaemonTaskStartupHook( void );

/**
 * @brief Connects to WiFi.
 */
//static void prvWifiConnect( void );

/**
 * @brief Initializes the board.
 */
static void prvMiscInitialization( void );
/*-----------------------------------------------------------*/

/**
 * @brief Application runtime entry point.
 */
#include <iodefine.h>
int main( void )
{
    /* Perform any hardware initialization that does not require the RTOS to be
     * running.  */

    /* Start the scheduler.  Initialization that requires the OS to be running,
     * including the WiFi initialization, is performed in the RTOS daemon task
     * startup hook. */
    // vTaskStartScheduler();
	PORTA.PDR.BIT.B0 = 1;
	PORTA.PODR.BIT.B0 = 1;

#ifndef GRROSE
    while(1)
    {
    	vTaskDelay(10000);
    }
#else
	init();
    setup();

    for (;;) {
        loop();
    }
#endif
    return 0;
}
/*-----------------------------------------------------------*/

static void prvMiscInitialization( void )
{
    /* FIX ME. */
	uart_config();
	//configPRINT_STRING(("Hello World.\r\n"));
    /* Start logging task. */
    xLoggingTaskInitialize( mainLOGGING_TASK_STACK_SIZE,
                            tskIDLE_PRIORITY,
                            mainLOGGING_MESSAGE_QUEUE_LENGTH );
}
/*-----------------------------------------------------------*/

void vApplicationDaemonTaskStartupHook( void )
{
    prvMiscInitialization();

    if( SYSTEM_Init() == pdPASS )
    {
#if(1)
        /* Initialise the RTOS's TCP/IP stack.  The tasks that use the network
        are created in the vApplicationIPNetworkEventHook() hook function
        below.  The hook function is called when the network connects. */
        FreeRTOS_IPInit( ucIPAddress,
                         ucNetMask,
                         ucGatewayAddress,
                         ucDNSServerAddress,
                         ucMACAddress );
#endif

    	/* Connect to the wifi before running the demos */
        //prvWifiConnect();


        /* Run all demos. */
        DEMO_RUNNER_RunDemos();
#if(0)
        /* Create the task to run tests. */
        xTaskCreate( TEST_RUNNER_RunTests_task,
                     "RunTests_task",
                     mainTEST_RUNNER_TASK_STACK_SIZE,
                     NULL,
                     tskIDLE_PRIORITY,
                     NULL );
#endif
    }
}
/*-----------------------------------------------------------*/

#if(0)
void prvWifiConnect( void )
{
    WiFiNetworkParams_t xJoinAPParams;
    WiFiReturnCode_t xWifiStatus;

    xWifiStatus = WIFI_On();

    if( xWifiStatus == eWiFiSuccess )
    {
        configPRINTF( ( "WiFi module initialized. Connecting to AP...\r\n" ) );
    }
    else
    {
        configPRINTF( ( "WiFi module failed to initialize.\r\n" ) );

        while( 1 )
        {
        }
    }

    /* Setup parameters. */
    xJoinAPParams.pcSSID = clientcredentialWIFI_SSID;
    xJoinAPParams.pcPassword = clientcredentialWIFI_PASSWORD;
    xJoinAPParams.xSecurity = clientcredentialWIFI_SECURITY;

    xWifiStatus = WIFI_ConnectAP( &( xJoinAPParams ) );

    if( xWifiStatus == eWiFiSuccess )
    {
        configPRINTF( ( "WiFi Connected to AP. Creating tasks which use network...\r\n" ) );
    }
    else
    {
        configPRINTF( ( "WiFi failed to connect to AP.\r\n" ) );

        while( 1 )
        {
        }
    }
}
#endif
/*-----------------------------------------------------------*/

#if ( ipconfigUSE_LLMNR != 0 ) || ( ipconfigUSE_NBNS != 0 ) || ( ipconfigDHCP_REGISTER_HOSTNAME == 1 )

const char * pcApplicationHostnameHook( void )
{
    /* Assign the name "FreeRTOS" to this network node.  This function will
     * be called during the DHCP: the machine will be registered with an IP
     * address plus this name. */
    return "RX65N_FREERTOS_TCP_TEST";
}

#endif
