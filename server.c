#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>

#define SERVER_PORT 9090
#define MAX_BUFFER_SIZE 2048
#define MAX_LOG_LINE 2048

// Fungsi untuk menangani koneksi dari setiap client
void manage_client_connection(int client_fd) {
    char buffer[MAX_BUFFER_SIZE] = {0};
    int is_first_message = 1;
    char client_name[MAX_BUFFER_SIZE];

    pid_t pid = fork();

    if (pid < 0) {
        perror("Gagal membuat proses baru");
        return;
    }

    if (pid == 0) {
        // Proses child: Menangani penerimaan dan penyimpanan pesan dari client
        while (1) {
            memset(buffer, 0, MAX_BUFFER_SIZE);

            int read_bytes = read(client_fd, buffer, MAX_BUFFER_SIZE);
            if (read_bytes <= 0) {
                printf("Client terputus atau terjadi error.\n");
                break;
            }

            buffer[read_bytes] = '\0';

            if (is_first_message) {
                // Menyimpan nama client dari pesan pertama
                strncpy(client_name, buffer, MAX_BUFFER_SIZE - 1);
                client_name[MAX_BUFFER_SIZE - 1] = '\0';
                is_first_message = 0;
                continue;
            }

            // Membuka file log untuk mencatat pesan
            FILE *log_file = fopen("chat_log.txt", "a");
            if (log_file == NULL) {
                perror("Error membuka file log");
                continue;
            }

            // Menulis pesan ke file log
            fprintf(log_file, "%s: %s\n", client_name, buffer);
            fclose(log_file);

            // Jika client mengirim "exit", akhiri koneksi
            if (strcmp(buffer, "exit") == 0) {
                printf("Client meminta untuk mengakhiri sesi.\n");
                break;
            }
        }

        // Menutup file descriptor untuk client
        close(client_fd);
        exit(0);

    } else {
        // Proses parent: Memantau log chat dan mengirimkan pembaruan ke client
        int last_sent_line = 0;

        while (1) {
            FILE *log_file = fopen("chat_log.txt", "r");
            if (log_file == NULL) {
                perror("Gagal membaca file log");
                break;
            }

            char line[MAX_LOG_LINE];
            int current_line = 0;

            // Membaca setiap baris dari file log
            while (fgets(line, sizeof(line), log_file) != NULL) {
                current_line++;
                if (current_line > last_sent_line) {
                    // Mengirimkan baris baru ke client
                    send(client_fd, line, strlen(line), 0);
                }
            }

            fclose(log_file);
            last_sent_line = current_line;

            sleep(1); // Tidur sejenak untuk menghindari busy-waiting
        }
    }
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr;
    int option = 1;

    // Inisialisasi dan membersihkan file log chat
    FILE *log_file = fopen("chat_log.txt", "w");
    if (log_file == NULL) {
        perror("Gagal menginisialisasi file log");
        return EXIT_FAILURE;
    }
    fclose(log_file);

    // Menangani signal untuk membersihkan zombie processes
    signal(SIGCHLD, SIG_IGN);

    // Membuat server socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error membuat socket");
        return EXIT_FAILURE;
    }

    // Mengatur opsi socket
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option)) < 0) {
        perror("Gagal mengatur opsi socket");
        return EXIT_FAILURE;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    // Binding socket ke alamat dan port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Gagal melakukan binding socket");
        return EXIT_FAILURE;
    }

    // Mendengarkan koneksi masuk
    if (listen(server_socket, 5) < 0) {
        perror("Error saat mendengarkan koneksi");
        return EXIT_FAILURE;
    }

    printf("Server chat berjalan di port %d\n", SERVER_PORT);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);

        // Menerima koneksi dari client
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            perror("Error menerima koneksi baru");
            continue;
        }

        printf("Client baru terhubung.\n");

        pid_t pid = fork();
        if (pid < 0) {
            perror("Gagal membuat proses untuk client");
            close(client_socket);
            continue;
        }

        if (pid == 0) {
            close(server_socket); // Menutup server socket di proses child
            manage_client_connection(client_socket);
        } else {
            close(client_socket); // Menutup client socket di proses parent
        }
    }

    return EXIT_SUCCESS;
}
