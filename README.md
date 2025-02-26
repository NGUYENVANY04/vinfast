Vinfast is a projects create by Nguyen Van Y
#BUG 1 stack over flow sys_evt  
Please note that the handlers registered for WiFi and network (WIFI_EVENT, IP_EVENT) are called from system_event task, which has a limited stack size. Seems like your code trying to use most of that stack for local variables:
----------------------------------------------------------------------
static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    ... char stats_buffer[16384];
    vTaskList(stats_buffer);
    printf("%s\n", stats_buffer);
    ...
}
-----------------------------------------------------------------------
#while the sdkconfig in your project says CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE =                \
    16384.In that setup the stack overflow in task sys_evt is expected.

#You can use either static or dynamic allocation to address the issue,                             \
    possibly prefixing the buffer definition with static should fix the issue.
                                                                      -----------------------------------------------------------------------------------------------------
#BUG 2
                                                                                                                                                                          Guru Meditation
                                                                                                                                                                              Error
    : Core 0 panic'ed (LoadProhibited). Exception was unhandled.

      Core 0 register dump
    : PC : 0x400014fd PS : 0x00060030 A0 : 0x8015a336 A1 : 0x3ffba180 ---0x400014fd
    : strlen in
          ROM-- -----------------------------------------------------------------------------------------------
#NOTE : POINTER OF CHAR
/*** #include "string.h"
#include <stdio.h>
int main(){

    char str[100];
    printf("%p\n", str);
    printf("%p\n", &str);
    printf("%p", &str[0]);
}**
****/
#OUTPUT SAME
