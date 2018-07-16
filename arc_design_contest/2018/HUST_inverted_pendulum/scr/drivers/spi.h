#ifndef __SPI_H_
#define __SPI_H_

int32_t cpld_spi_init(void);
int32_t spi_read_qei(int16_t *qei, int dir);
int32_t spi_write_pwm(int16_t *pwm, int dir);
#endif