#include "../include/server.h"

void set_bluetooth_discoverable() {
    int dev_id = hci_get_route(NULL);
    int sock = hci_open_dev(dev_id);

    if (dev_id < 0 || sock < 0) {
        perror("Error opening Bluetooth device");
        exit(EXIT_FAILURE);
    }

    // 0x200408 = audio class
    if (hci_write_class_of_dev(sock, 0x200408, 0) < 0) {
        perror("Error setting device class");
        close(sock);
        exit(EXIT_FAILURE);
    }

    uint8_t scan_enable = SCAN_PAGE | SCAN_INQUIRY;
    if (hci_send_cmd(sock, OGF_HOST_CTL, OCF_WRITE_SCAN_ENABLE, 1, &scan_enable) < 0) {
        perror("Error enabling discoverable mode");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Bluetooth device made discoverable.\n");
    close(sock);
}

void enable_pairing() {
    int dev_id = hci_get_route(NULL);
    int sock = hci_open_dev(dev_id);

    if (dev_id < 0 || sock < 0) {
        perror("Error opening Bluetooth device");
        exit(EXIT_FAILURE);
    }

    uint8_t disable_auth = 0x00;
    if (hci_send_cmd(sock, OGF_HOST_CTL, OCF_WRITE_AUTH_ENABLE, 1, &disable_auth) < 0) {
        perror("Error disabling authentication");
        close(sock);
        exit(EXIT_FAILURE);
    }

    uint8_t enable_auth = 0x01;
    if (hci_send_cmd(sock, OGF_HOST_CTL, OCF_WRITE_AUTH_ENABLE, 1, &enable_auth) < 0) {
        perror("Error enabling pairing mode");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Pairing mode enabled successfully.\n");
    close(sock);
}


void start_bluetooth_server() {
    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    char buf[1024] = { 0 };
    int s, client, bytes_read;
    socklen_t opt = sizeof(rem_addr);

    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    if (s < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = (uint8_t)1;

    if (bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr)) < 0) {
        perror("Error binding socket");
        close(s);
        exit(EXIT_FAILURE);
    }

    if (listen(s, 1) < 0) {
        perror("Error setting socket to listen");
        close(s);
        exit(EXIT_FAILURE);
    }

    printf("Waiting for Bluetooth connection...\n");

    client = accept(s, (struct sockaddr *)&rem_addr, &opt);
    if (client < 0) {
        perror("Error accepting connection");
        close(s);
        exit(EXIT_FAILURE);
    }

    char client_addr[18] = { 0 };
    ba2str(&rem_addr.rc_bdaddr, client_addr);
    printf("Connection accepted from %s\n", client_addr);

    bytes_read = read(client, buf, sizeof(buf));
    if (bytes_read > 0) {
        printf("Received: %s\n", buf);
    }

    close(client);
    close(s);
}
