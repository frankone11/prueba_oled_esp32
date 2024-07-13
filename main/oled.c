#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "esp_log.h"
#include "esp_system.h"
#include "esp_err.h"

#include "driver/i2c_master.h"

#include "oled.h"
#include "arial.h"

#define I2C_SCL_PIN			GPIO_NUM_22       /*!< gpio number for I2C master clock */
#define I2C_SDA_PIN			GPIO_NUM_21       /*!< gpio number for I2C master data  */
#define MASTER_FREQUENCY	400000
#define PORT_NUMBER			I2C_NUM_0
#define I2C_TICKS_TO_WAIT	100
#define I2C_ADDRESS			ADDR

char buffer[BUFFER_TAM];

i2c_master_dev_handle_t oled_handle;
i2c_master_bus_handle_t bus_handle;


bool Oled_DameBuffer(uint8_t *buf, uint16_t tam)
{
	if(!buf)
		return false;

	if(tam > BUFFER_TAM)
		tam = BUFFER_TAM;

	memcpy(buf, buffer, tam);
	return true;

}

bool Oled_ColocaBuffer(const uint8_t *buf, uint16_t tam)
{
	if(!buf)
		return false;
	if(tam > BUFFER_TAM)
		tam = BUFFER_TAM;

	memcpy(buffer, buf, tam);
	return true;
}

uint16_t Oled_DameTamBuffer(void)
{
	return BUFFER_TAM;
}

void Comando(uint8_t comando)
{
	uint8_t buf[2] = { 0, 0 };
	buf[1] = comando;
	
	ESP_ERROR_CHECK(i2c_master_transmit(oled_handle, buf, 2, I2C_TICKS_TO_WAIT));
}

void Comandos(const uint8_t *buff, uint8_t tam)
{
	if(tam > MAX_DAT)
		return;

	uint8_t buf[MAX_DAT + 1] = {0};
	for(uint8_t i = 0; i < tam; i++)
		buf[i+1] = buff[i];
	
	ESP_ERROR_CHECK(i2c_master_transmit(oled_handle, buf, tam+1, I2C_TICKS_TO_WAIT));
}

void Dato(uint8_t dato)
{
	uint8_t buf[2] = { 0x40U, 0};
	buf[1] = dato;
	
	ESP_ERROR_CHECK(i2c_master_transmit(oled_handle, buf, 2, I2C_TICKS_TO_WAIT));
}

void Datos(const uint8_t *buff, uint8_t tam)
{
	if(tam > MAX_DAT)
		return;

	uint8_t buf[MAX_DAT + 1] = {0x40U};
	for(uint8_t i = 0; i < tam; i++)
		buf[i+1] = buff[i];
	
	ESP_ERROR_CHECK(i2c_master_transmit(oled_handle, buf, tam+1, I2C_TICKS_TO_WAIT));
}

void Oled_Inicializa(void)
{
	i2c_master_bus_config_t i2c_mst_config = {
		.clk_source = I2C_CLK_SRC_DEFAULT,
		.i2c_port = PORT_NUMBER,
		.scl_io_num = I2C_SCL_PIN,
		.sda_io_num = I2C_SDA_PIN,
		.glitch_ignore_cnt = 7,
		.flags.enable_internal_pullup = true,
	};

	i2c_device_config_t oled_cfg = {
		.dev_addr_length = I2C_ADDR_BIT_LEN_7,
		.device_address = I2C_ADDRESS,
		.scl_speed_hz = MASTER_FREQUENCY,
	};

	memset(buffer, 0, BUFFER_TAM);
	
	ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &bus_handle));
	ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &oled_cfg, &oled_handle));

	const uint8_t init1[] =
	{
			0xAEU,
			0xD5U,
			0x80U,
			0xA8U
	};

	Comandos( init1, sizeof(init1));
	Comando( 0x3FU);

	const uint8_t init2[] =
	{
			0xD3U,
			0x00U,
			0x40U,
			0x8DU
	};

	Comandos( init2, sizeof(init2));

	Comando( 0x14U);

	const uint8_t init3[] =
	{
			0x20U,
			0x00U,
			0xA1U, //A0 | 1
			0xC8U
	};

	Comandos( init3, sizeof(init3));

	Comando( 0xDAU);
	Comando( 0x12U);
	Comando( 0x81U);
	Comando( 0xCFU);
	Comando( 0xD9U);
	Comando( 0XF1U);

	const uint8_t init5[] =
	{
			0xDBU,
			0x40U,
			0xA4U,
			0xA6U,
			0x2EU,
			0xAFU,
	};

	Comandos( init5, sizeof(init5));

	const uint8_t initxy [] =
	{
			0x21U,
			0x00U,
			0x7FU,
			0x22U,
			0x00U,
			0x07U
	};

	Comandos( initxy, sizeof(initxy));

	Oled_LimpiaDisplay();
}

void Oled_Dibuja(void)
{
	const uint8_t initxy [] =
	{
			0x21U,
			0x00U,
			0x7FU,
			0x22U,
			0x00U,
			0x07U
	};

	Comandos(initxy, sizeof(initxy));

	for(uint8_t j = 0; j < 128; j++)
	{
		Datos((const uint8_t *)&buffer[j*8], 8);
	}
}

void Oled_LimpiaDisplay()
{
	memset(buffer, 0, 1024);
	Oled_Dibuja();
}

void Oled_ColocaCaracter(uint8_t *dir, const char caracter)
{
	uint8_t noval[8] = { 0x18U, 0x3CU, 0x76U, 0x9BU, 0x66U, 0x3CU, 0x18U, 0x00U };
	const tImage *img = NULL;

	if(caracter < 0x20 || caracter == 127)
		memcpy(dir, noval, 8);
	else
	{
		img = Arial.chars[caracter - 0x20U].image;
		memcpy(dir, img->data, img->dataSize);
	}
}

void Oled_EnviaCadenaXY(const char *txt, uint8_t fila, uint8_t columna)
{
	uint16_t offset;
	offset = (fila * 128) + (columna * 8);
	if(!txt)
		return;
	while(*txt)
	{
		Oled_ColocaCaracter((uint8_t *)(buffer + offset), *txt++);
		offset += 8;
	}
}

void Oled_Finaliza(void)
{
	i2c_del_master_bus(bus_handle);
}