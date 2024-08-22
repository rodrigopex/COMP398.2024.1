/**
 * @file
 * @brief [TODO:description]
 */

#include <stdint.h>
#include <stdio.h>

#define PI_MATH 3.1425
#define BIT(_n) (1 << _n)

#if defined(CONFIG_MY_LOG)
void log_dbg(const char *message)
{
	printf("%s\n", message);
}
#else
void log_dbg(const char *message)
{
}
#endif /* CONFIG_MY_LOG */

enum color {
	BLACK,
	WHITE,
	BLUE,
	YELLOW,
} __attribute__((__packed__));

/**
 * @brief Pixel struct represents a screen pixel.
 * 128x64
 */
struct pixel2 {
	uint8_t x; /**< x axis*/
	uint8_t y;
	enum color color;
	unsigned char alpha;
};

struct pixel {
	uint8_t x: 7;
	uint8_t y: 7;
	uint8_t color: 2;
#if defined(CONFIG_HAS_ALPHA)
	uint8_t alpha: 6;
#endif /* CONFIG_HAS_ALPHA */
};

int pixel_show(struct pixel *self)
{
	if (self == NULL) {
		return -1;
	}

#if defined(CONFIG_HAS_ALPHA)
	if (self->alpha == 0) {
		return -2;
	}

	printf("Pixel(x=%d, y=%d, color=%d, alpha=%d)\n", self->x, self->y, self->color,
	       self->alpha);
#else

	printf("Pixel(x=%d, y=%d, color=%d)\n", self->x, self->y, self->color);
#endif /* CONFIG_HAS_ALPHA */

	printf("Sizeof Pixel %d\n", (int)sizeof(struct pixel));
	return 0;
}
union config0_register {
	struct {
		uint32_t HPFDIS: 1;
		uint32_t: 1;
		uint32_t INTEN_BI: 1;
		uint32_t ZX_SRC_SEL: 1;
		uint32_t RMS_OC_SRC: 1;
		uint32_t VNOMA_EN: 1;
		uint32_t: 1;
		uint32_t DISAPLPF: 1;
		uint32_t DISRPLPF: 1;
	} bits;
	uint32_t value;
};

static const union config0_register rx = {.bits = {.DISRPLPF = 0, .DISAPLPF = 1, .RMS_OC_SRC = 1}};
int config0_register_cmp(const union config0_register *reg1, const union config0_register *reg2)
{
	return reg1->value - reg2->value;
}

int main(int argc, char *argv[])
{
	int *const b;
	const int a = 10;
	// struct pixel p = {.x = 10, .y = 54, .color = BLUE, .alpha = 19};
	struct pixel p = {.x = 10, .y = 54, .color = BLUE};
	pixel_show(&p);

	/* uint32_t config0 = (1 << 8) | (1 << 7) | (1 << 4); // 0b 0000 1111 */
	uint32_t c3 = (1 << 8) & (1 << 7) & (1 << 4); // Esse numero
	uint32_t config0 = BIT(8) | BIT(7) | BIT(4);  // 0b_1_1001_0000

	union config0_register r = {.bits = {.DISRPLPF = 1, .DISAPLPF = 1, .RMS_OC_SRC = 1}};
	union config0_register r2 = {.value = 400};

	int result = config0_register_cmp(&r, &r2);
	printf("Result = %d\n", result);
	printf("r = %d, r2 = %d\n", r.value, r2.value);

	log_dbg("HELLO LOG!");

	return 0;
}
