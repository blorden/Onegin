#include "printer.c"

int main ()
{
	char *buf = "privet";

	FILE *out = fopen("privet", "wb");

	printer(out, buf, 6);
	printer_delete(out);

	fclose(out);

	return 0;
}