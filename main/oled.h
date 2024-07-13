#define ADDR 		0x3CU
#define MAX_DAT		32U
#define BUFFER_TAM	1024U

/**
 * @brief Función que devuelve el contenido del buffer
 * 
 * @param buf dirección del buffer
 * @param tam tamaño del buffer
 * @return true Si se pudo copiar
 * @return false Si no se pudo copiar en caso de buffer = NULL
 */
bool Oled_DameBuffer(uint8_t *buf, uint16_t tam);

/**
 * @brief Función que copia el contenido del buffer en el buffer del LCD.
 * 
 * @param buf dirección del buffer
 * @param tam tamaño del buffer
 * @return true Si es posible copiarlo
 * @return false Si no se pudo copiar Buffer = NULL o tam > 1024.
 */
bool Oled_ColocaBuffer(const uint8_t *buf, uint16_t tam);

/**
 * @brief Función que devuelve el tamaño del Buffer
 * 
 * @return uint16_t Tamaño del buffer (1024 bytes).
 */
uint16_t Oled_DameTamBuffer(void);

/**
 * @brief Función que inicializa el Oled
 * 
 */
void Oled_Inicializa(void);

/**
 * @brief Función que dibuja el contenido del buffer
 * 
 */
void Oled_Dibuja(void);

/**
 * @brief Función que Limpia el display oled
 * 
 */
void Oled_LimpiaDisplay(void);

/**
 * @brief Función que coloca un caracter en el buffer
 * 
 * @param dir Dirección de memoria de los caracteres, se declara en Arial.h
 * @param caracter Caracter a colocar en el Oled
 */
void Oled_ColocaCaracter(uint8_t *dir, const char caracter);

/**
 * @brief Función que envia una cadena al buffer
 * 
 * @param txt Texto a colocar
 * @param fila Fila del Oled
 * @param columna Columna del Oled
 */
void Oled_EnviaCadenaXY(const char *txt, uint8_t fila, uint8_t columna);

/**
 * @brief Función que finaliza al Oled.
 * 
 */
void Oled_Finaliza(void);
