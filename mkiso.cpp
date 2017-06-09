// public domain

#define _CRT_SECURE_NO_WARNINGS

#include <malloc.h>
#include <stdio.h>
#include <string.h>

#define DEVICE_PREFIX	"\\\\.\\"
#define BUF_SIZE	102400

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s <Drive:> <image.iso>\n", argv[0]);
		return 1;
	}

	FILE *cd, *iso;
	char *ss;
	int rd;
	double progress;
	unsigned char *buf;

	ss = (char *)malloc(strlen(argv[1]) + strlen(DEVICE_PREFIX) + 1);
	if (!ss)
	{
		fprintf(stderr, "Alloc failed !\r\n");
		return 255;
	}
	strcpy(ss, DEVICE_PREFIX);
	strcat(ss, argv[1]);

	cd = fopen(ss, "rb");

	//cd=fopen("\\\\.\\cdrom0","rb");

	free(ss);
	if (!cd)
	{
		fprintf(stderr, "Unable to open drive %s !\r\n", argv[1]);
		return 1;
	}

	iso = fopen(argv[2], "wb");
	if (!iso)
	{
		fprintf(stderr, "Unable to create file %s !\r\n", argv[2]);
		fclose(cd);
		return 1;
	}

	progress = 0;
	buf = (unsigned char *)malloc(BUF_SIZE);
	if (!buf)
	{
		fprintf(stderr, "Buffer alloc failed !\r\n");
		return 255;
	}
	do
	{
		printf("Read: %.0f bytes\r", progress);


		rd = fread(buf, 1, BUF_SIZE, cd);
		if (rd < 0)
		{
			fprintf(stderr, "\r\n\r\nError reading drive %s !\r\n",
				argv[1]);
			free(buf);
			fclose(iso);
			fclose(cd);
			return 1;
		}
		progress += rd;
		if (rd > 0)
		{
			int wr;

			wr = fwrite(buf, 1, rd, iso);
			if (wr != rd)
			{
				fprintf(stderr,
					"\r\n\r\nError writing to file %s !\r\n",
					argv[2]);
				free(buf);
				fclose(iso);
				fclose(cd);
				return 1;
			}
		}
	}
	while (rd > 0);
	printf("Read: %.0f bytes\r", progress);

	fclose(iso);
	fclose(cd);
	printf("\r\n\r\nDone.\r\n");
	return 0;
}
