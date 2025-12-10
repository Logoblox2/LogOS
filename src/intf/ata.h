#ifndef ATA_H
#define ATA_H

int ata_write_sector(uint32_t lba, const uint8_t *buf512);
int ata_read_sector(uint32_t lba, uint8_t *buf512);

#endif