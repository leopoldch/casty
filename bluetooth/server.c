#include "server.h"


void set_bluetooth_discoverable() {
    int dev_id = hci_get_route(NULL);
    int sock = hci_open_dev(dev_id);

    if (dev_id < 0 || sock < 0) {
        perror("Erreur lors de l'ouverture de l'appareil Bluetooth");
        exit(EXIT_FAILURE);
    }

    // 0x200408 = audio class
    if (hci_write_class_of_dev(sock, 0x200408, 0) < 0) {
        perror("Erreur lors de la configuration de la classe d'appareil");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Activer le mode découverte et connectable
    uint8_t scan_enable = SCAN_PAGE | SCAN_INQUIRY;
    if (hci_send_cmd(sock, OGF_HOST_CTL, OCF_WRITE_SCAN_ENABLE, 1, &scan_enable) < 0) {
        perror("Erreur lors de l'activation du mode découvert");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Périphérique Bluetooth rendu découvrable.\n");
    close(sock);
}

void enable_pairing() {
    int dev_id = hci_get_route(NULL);
    int sock = hci_open_dev(dev_id);

    if (dev_id < 0 || sock < 0) {
        perror("Erreur lors de l'ouverture de l'appareil Bluetooth");
        exit(EXIT_FAILURE);
    }

    uint8_t enable_auth = 0x01; // Activer l'authentification
    if (hci_send_cmd(sock, OGF_HOST_CTL, OCF_WRITE_AUTH_ENABLE, 1, &enable_auth) < 0) {
        perror("Erreur lors de l'activation de l'appairage");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Mode appairage activé.\n");
    close(sock);
}

void start_bluetooth_server() {
    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    char buf[1024] = { 0 };
    int s, client, bytes_read;
    socklen_t opt = sizeof(rem_addr);

    // Créer le socket Bluetooth
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    if (s < 0) {
        perror("Erreur lors de la création du socket");
        exit(EXIT_FAILURE);
    }

    // Lier le socket à l'interface Bluetooth locale
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = (uint8_t)1;

    if (bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr)) < 0) {
        perror("Erreur lors de l'association du socket");
        close(s);
        exit(EXIT_FAILURE);
    }

    // Mettre le périphérique en mode écoute
    if (listen(s, 1) < 0) {
        perror("Erreur lors de la mise en écoute");
        close(s);
        exit(EXIT_FAILURE);
    }

    printf("En attente de connexion Bluetooth...\n");

    // Accepter une connexion entrante
    client = accept(s, (struct sockaddr *)&rem_addr, &opt);
    if (client < 0) {
        perror("Erreur lors de l'acceptation de la connexion");
        close(s);
        exit(EXIT_FAILURE);
    }

    char client_addr[18] = { 0 };
    ba2str(&rem_addr.rc_bdaddr, client_addr);
    printf("Connexion acceptée depuis %s\n", client_addr);

    // Lire les données envoyées par le client
    bytes_read = read(client, buf, sizeof(buf));
    if (bytes_read > 0) {
        printf("Reçu : %s\n", buf);
    }

    close(client);
    close(s);
}
