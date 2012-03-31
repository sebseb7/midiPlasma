//	keyboard.h

typedef struct {
	unsigned char type, x, y;
} KeyboardEvent;

int		keyboard_init(void);
void	keyboard_kill(void);
int		keyboard_poll(KeyboardEvent* e);

