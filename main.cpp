#include <iostream>
#include <libusb-1.0/libusb.h>
#include <stdio.h>
using namespace std;
void printdev(libusb_device *dev);
void print_serial_number(libusb_device *dev);

int main() {
    libusb_device **devs; // указатель на указатель на устройство,
    libusb_context *ctx = NULL; // контекст сессии libusb
    int r;                      // для возвращаемых значений
    ssize_t cnt;                // число найденных USB-устройств
    ssize_t i;                  // индексная переменная цикла перебора всех устройств

    // инициализировать библиотеку libusb, открыть сессию работы с libusb
    r = libusb_init(&ctx);
    if (r < 0) {
        fprintf(stderr, "Ошибка: инициализация не выполнена, код: %d.\n", r);
        return 1;
    }

    // задать уровень подробности отладочных сообщений
    libusb_set_debug(ctx, 3);

    // получить список всех найденных USB- устройств
    cnt = libusb_get_device_list(ctx, &devs);
    if (cnt < 0) {
        fprintf(stderr, "Ошибка: список USB устройств не получен.\n");
        return 1;
    }

    printf("%5s | %-22s | %-18s | %-18s | %-18s\n", "#", "Количество конфигураций", "Производитель", "Идентификатор устройства", "Серийный номер");
    printf("%5s-+-%-22s-+-%-18s-+-%-18s-+-%-18s\n", "-----", "----------------------", "------------------", "------------------", "------------------");

    for (i = 0; i < cnt; i++) { // цикл перебора всех устройств
        printf("%5zd | ", i + 1); // Печать индекса устройства
        printdev(devs[i]); // печать параметров устройства
        print_serial_number(devs[i]); // печать серийного номера устройства
    }

    // освободить память, выделенную функцией получения списка устройств
    libusb_free_device_list(devs, 1);
    libusb_exit(ctx); // завершить работу с библиотекой libusb, закрыть сессию работы с libusb
    return 0;
}

void printdev(libusb_device *dev) {
    libusb_device_descriptor desc; // дескриптор устройства
    int r = libusb_get_device_descriptor(dev, &desc);
    if (r < 0) {
        fprintf(stderr, "Ошибка: дескриптор устройства не получен, код: %d.\n", r);
        return;
    }

    printf("%-22d | %-18x | %-18x | ", desc.bNumConfigurations, desc.idVendor, desc.idProduct);
}

void print_serial_number(libusb_device *dev) {
    libusb_device_handle *handle = NULL;
    int r = libusb_open(dev, &handle);
    if (r < 0 || !handle) {
        fprintf(stderr, "Ошибка: не удалось открыть устройство для получения серийного номера.\n");
        return;
    }

    libusb_device_descriptor desc;
    r = libusb_get_device_descriptor(dev, &desc);
    if (r < 0) {
        fprintf(stderr, "Ошибка: дескриптор устройства не получен, код: %d.\n", r);
        libusb_close(handle);
        return;
    }

    if (desc.iSerialNumber) {
        unsigned char serial[256];
        int ret = libusb_get_string_descriptor_ascii(handle, desc.iSerialNumber, serial, sizeof(serial));
        if (ret > 0) {
            printf("%-18s\n", serial);
        } else {
            printf("%-18s\n", "N/A");
        }
    } else {
        printf("%-18s\n", "N/A");
    }
    libusb_close(handle);
}
