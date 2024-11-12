#include <iostream>
#include <libusb-1.0/libusb.h>
#include <stdio.h>
using namespace std;
void printdev(libusb_device *dev);
void print_serial_number(libusb_device *dev);
void print_manufacturer(libusb_device *dev, libusb_device_handle *handle, libusb_device_descriptor &desc);
void print_device_class(uint8_t device_class);

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
        libusb_exit(ctx);
        fprintf(stderr, "Ошибка: список USB устройств не получен.\n");
        return 1;
    }

    printf("%5s  | %-38s | %-33s | %-33s | %-30s | %-30s\n", "#", "Класс устройства", "Идентификатор производителя   ", "Идентификатор устройства      ", "Производитель                  ", "Серийный номер");
    printf("%5s--+-%-18s-+-%-30s-+-%-30s-+-%-30s-+-%-30s\n", "-----", "-----------------------", "------------------------------", "------------------------------", "------------------------------", "------------------------------");

    for (i = 0; i < cnt; i++) { // цикл перебора всех устройств
        printf("%5zd  | ", i + 1); // Печать индекса устройства
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

    print_device_class(desc.bDeviceClass);
    printf("%-30x | %-30x | ", desc.idVendor, desc.idProduct);

    libusb_device_handle *handle = NULL;
    r = libusb_open(dev, &handle);
    if (r < 0 || !handle) {
        fprintf(stderr, "Ошибка: не удалось открыть устройство для получения информации о производителе.\n");
        printf("%-30s | ", "N/A");
    } else {
        print_manufacturer(dev, handle, desc);
        libusb_close(handle);
    }
}

void print_serial_number(libusb_device *dev) {
    libusb_device_handle *handle = NULL;
    int r = libusb_open(dev, &handle);
    if (r < 0 || !handle) {
        fprintf(stderr, "Ошибка: не удалось открыть устройство для получения серийного номера.\n");
        printf("%-30s\n", "N/A");
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
            printf("%-30s\n", serial);
        } else {
            printf("%-30s\n", "N/A");
        }
    } else {
        printf("%-30s\n", "N/A");
    }
    libusb_close(handle);
}

void print_manufacturer(libusb_device *dev, libusb_device_handle *handle, libusb_device_descriptor &desc) {
    if (desc.iManufacturer) {
        unsigned char manufacturer[256];
        int ret = libusb_get_string_descriptor_ascii(handle, desc.iManufacturer, manufacturer, sizeof(manufacturer));
        if (ret > 0) {
            printf("%-31s | ", manufacturer);
        } else {
            printf("%-32s | ", "N/A");
        }
    } else {
        printf("%-31s | ", "N/A");
    }
}

void print_device_class(uint8_t device_class) {
    switch (device_class) {
        case 0x00:
            printf("%-35s | ", "Код отсутствует        ");
            break;
        case 0x01:
            printf("%-35s | ", "Аудиоустройство");
            break;
        case 0x02:
            printf("%-35s | ", "Коммуникационное устройство");
            break;
        case 0x03:
            printf("%-35s | ", "Устройство пользовательского интерфейса");
            break;
        case 0x05:
            printf("%-35s | ", "Физическое устройство");
            break;
        case 0x06:
            printf("%-35s | ", "Изображения");
            break;
        case 0x07:
            printf("%-35s | ", "Принтер");
            break;
        case 0x08:
            printf("%-35s | ", "Устройство хранения данных");
            break;
        case 0x09:
            printf("%-35s | ", "Концентратор");
            break;
        case 0x0A:
            printf("%-35s | ", "CDC-Data");
            break;
        case 0x0B:
            printf("%-35s | ", "Smart Card");
            break;
        case 0x0D:
            printf("%-35s | ", "Content Security");
            break;
        case 0x0E:
            printf("%-35s | ", "Видеоустройство");
            break;
        case 0x0F:
            printf("%-35s | ", "Персональное медицинское устройство");
            break;
        case 0x10:
            printf("%-35s | ", "Аудио- и видеоустройства");
            break;
        case 0xDC:
            printf("%-35s | ", "Диагностическое устройство");
            break;
        case 0xE0:
            printf("%-35s | ", "Беспроводный контроллер");
            break;
        case 0xEF:
            printf("%-35s | ", "Различные устройства   ");
            break;
        case 0xFE:
            printf("%-35s | ", "Специфическое устройство");
            break;
        default:
            printf("%-35s | ", "Неизвестный класс устройства");
            break;
    }
}
