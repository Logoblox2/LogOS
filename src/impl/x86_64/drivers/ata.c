#include <stdint.h>

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" :: "a"(val), "Nd"(port));
}
static inline void outw(uint16_t port, uint16_t val) {
    __asm__ volatile ("outw %0, %1" :: "a"(val), "Nd"(port));
}
static inline uint16_t inw(uint16_t port) {
    uint16_t ret;
    __asm__ volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

#define ATA_REG_DATA      0x1F0
#define ATA_REG_SECCOUNT  0x1F2
#define ATA_REG_LBA0      0x1F3
#define ATA_REG_LBA1      0x1F4
#define ATA_REG_LBA2      0x1F5
#define ATA_REG_HDDEVSEL  0x1F6
#define ATA_REG_COMMAND   0x1F7
#define ATA_REG_STATUS    0x1F7

#define ATA_CMD_WRITE_PIO 0x30
#define ATA_CMD_READ_PIO  0x20
#define ATA_DEV_MASTER    0xE0

static void ata_wait_ready(void) {
    while (inb(ATA_REG_STATUS) & 0x80) { /* BSY */ }
}

int ata_write_sector(uint32_t lba, const uint8_t *buf512) {
    ata_wait_ready();

    outb(ATA_REG_HDDEVSEL, ATA_DEV_MASTER | ((lba >> 24) & 0x0F));
    outb(ATA_REG_SECCOUNT, 1);
    outb(ATA_REG_LBA0, (uint8_t)(lba & 0xFF));
    outb(ATA_REG_LBA1, (uint8_t)((lba >> 8) & 0xFF));
    outb(ATA_REG_LBA2, (uint8_t)((lba >> 16) & 0xFF));

    outb(ATA_REG_COMMAND, ATA_CMD_WRITE_PIO);

    uint8_t st;
    do { st = inb(ATA_REG_STATUS); } while ((st & 0x80) || !(st & 0x08));

    const uint16_t *w = (const uint16_t *)buf512;
    for (int i = 0; i < 256; i++) outw(ATA_REG_DATA, w[i]);

    ata_wait_ready();

    if (st & 0x01) return -1;
    return 0;
}

int ata_read_sector(uint32_t lba, uint8_t *buf512) {
    ata_wait_ready();

    outb(ATA_REG_HDDEVSEL, ATA_DEV_MASTER | ((lba >> 24) & 0x0F));
    outb(ATA_REG_SECCOUNT, 1);
    outb(ATA_REG_LBA0, (uint8_t)(lba & 0xFF));
    outb(ATA_REG_LBA1, (uint8_t)((lba >> 8) & 0xFF));
    outb(ATA_REG_LBA2, (uint8_t)((lba >> 16) & 0xFF));

    outb(ATA_REG_COMMAND, ATA_CMD_READ_PIO);

    uint8_t st;
    do { st = inb(ATA_REG_STATUS); } while ((st & 0x80) || !(st & 0x08));

    uint16_t *w = (uint16_t *)buf512;
    for (int i = 0; i < 256; i++) w[i] = inw(ATA_REG_DATA);

    ata_wait_ready();

    if (st & 0x01) return -1;
    return 0;
}