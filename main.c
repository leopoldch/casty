#include <stdio.h>
#include <stdlib.h>
#include "include/server.h"


int main() {
    set_bluetooth_discoverable();
    enable_pairing();
    start_bluetooth_server();
    return 0;
}
