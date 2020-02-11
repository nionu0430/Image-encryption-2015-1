/*
	LSB ����� ����� �̹��� ��ȣȭ
	��������
	����� �弼�� ����� ������ ������
	���������� 15.07.21
*/
/*
	������
	->ũ�Ⱑ 1kb���� Ŀ���� �Ϳ����� ������ �˾Ƴ� �� ������
	�������� �� ���� ��ǥ
	1) ũ�Ⱑ Ŀ���� ���� ��ƾ���
	2) API ���� �̿��Ͽ��� ���ϴ� ���ϵ�� ��ȣȭ�� �� �� �ִ� ���α׷� �����
	3) ��ȣ�� ����� �� ��ȣȭ ������
*/
#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <conio.h>


int fileSize = 1;
int size = 0;

void deco(FILE* coded); //��ȣ ����
void code(FILE* origin, FILE* coded); //��ȣ �Է�
void arraySet(int bit[]); //�迭 �ʱ�ȭ

int getSize(FILE* fp);
void getString(char* sentence);
int resizeString(char* sentence);


int main(void)
{
	FILE* input; // �׸����� ����
	FILE* output;// ��ȣȭ�� �׸�����
	FILE* decode; // �ؼ��Ҷ� ����ϴ� �׸�����
	FILE* textIn, *textOut; //�Է°� ��µǴ� ��ȣ��
	
	//�ҷ����°Ͱ� �˻�
	fopen_s(&input, "test2.bmp", "r");
	if (input == NULL){
		printf("�Է��ϰ��� �ϴ� bmp ������ �����ϴ�.\n"); return 0;}
	fopen_s(&output, "coded.bmp", "w");

	if (input == NULL) { printf("�Է��ϰ��� �ϴ� bmp ������ �����ϴ�.\n"); return 0; }
	code(input, output);

	fopen_s(&decode, "coded.bmp", "r");
	deco(decode);
}

void deco(FILE* coded)
{
	int bit[8];
	FILE* textOut;
	BITMAPINFOHEADER hInfo; //����
	BITMAPFILEHEADER hf; //���� ���
	RGBQUAD hRGB[256]; //�ȷ�Ʈ�� �޾ƿ�
	BYTE *lpImg; //�̹����� �� 
	int i, j;
	int ch = 1;//8���� ��Ʈ�� ���� ���ڸ� ���� ���� shifting�� ���ؼ�
	char character; //���ڸ� ���ڷ� �ٲ���
	char* code; //����Ǵ°�
	int sz = 0, head = 0; //���� �� ���ڿ��� �ּҿ� ũ�� //�޸� ���� ��ġ




	fread(&hf, sizeof(BITMAPFILEHEADER), 1, coded);
	if (hf.bfType != 0x4D42) printf("��Ʈ���̹����� �ƴѵ�\n"); ///0x4D42 = BM
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, coded);//�Է�
	fread(hRGB, sizeof(RGBQUAD), 256, coded); //�ȷ�Ʈ

	lpImg = (BYTE*)malloc(hInfo.biSizeImage);
	fread(lpImg, sizeof(char), hInfo.biSizeImage, coded); //�����͸� �о��
	code = (char*)malloc(1);
	fclose(coded);

	textOut = fopen("output.txt", "w");
	while (ch != 0)
	{
		arraySet(bit);
		for (j = 0; j < 8; j++)
		{
			bit[j] = (lpImg[head] >> 1) % 2;
			ch = ch + (bit[j] << j);
		}
		character = ch;
		fputc(ch,textOut);//�Է�
		printf("%c", character);
	}
	fclose(textOut);
	return;
}
void arraySet(int bit[])
{
	int i;
	for (i = 0; i < 8; i++)
	{
		bit[i] = 0;
	}
	return;
}

void code(FILE* origin, FILE* coded)
{
	int i, j,max;
	int C, head = 0;
	char c;
	BITMAPINFOHEADER hInfo; //����
	BITMAPFILEHEADER hf; //���� ���
	RGBQUAD hRGB[256]; //�ȷ�Ʈ�� �޾ƿ�
	BYTE *lpImg; //�̹����� �� //���� �׸������� ������ ��ŭ �о����

	FILE* textIn;

	
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, origin);//�Է�
	if (hf.bfType != 0x4D42) printf("��Ʈ���̹����� �ƴѵ�\n"); ///0x4D42 = BM
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, origin);//�Է�
	fread(hRGB, sizeof(RGBQUAD), 256, origin); //�ȷ�Ʈ

	lpImg = (BYTE*)malloc(hInfo.biSizeImage);
	fread(lpImg, sizeof(char), hInfo.biSizeImage, origin); //�����͸� �о��
	fclose(origin);

	size = hInfo.biSizeImage;

	textIn = fopen("input.txt", "r");
	if (textIn == NULL){
		printf("�Է��ϰ��� �ϴ� �ؽ�Ʈ ������ �����ϴ�.\n");
		exit(1);
	}


	while (1) //���Ͽ� ����ִ� ��ȣ���� ũ�⸦ ����
	{
		c = getc(textIn);
		
		if (c == EOF)
			break;
		fileSize++;
	}
	
	printf("%d\n", fileSize);
	printf("%d", size);
	if (fileSize*8<size)
	{ 
		max = fileSize;
		for (i = 0; i < 8 * (fileSize + 1); i++) //0���� ��� lsb�� �������
		{
			if ((lpImg[i] >> 0) % 2 == 1)
				lpImg[i] -= 1;
		}
	}
	else
	{
		for (i = 0; i < size; i++) //0���� ��� lsb�� �������
		{
			if ((lpImg[i] >> 0) % 2 == 1)
				lpImg[i] -= 1;
		}
		max = size - 1;
	}
	fseek(textIn, 0L, SEEK_SET);

	for (j = 0; j < max; j++) //���� ������ size ��ŭ �Է�����
	{
		c = getc(textIn);
		printf("%c", c);
		C = c; //���ڷ�
		for (i = 0; i < 8; i++)
		{
			if ((C >> i) % 2 == 1)
			{
				lpImg[head] += 1;
			}
			head++;
		}
	}
	fclose(textIn);
	printf("\n");
	//////
	fwrite(&hf, sizeof(char), sizeof(BITMAPFILEHEADER), coded); //
	fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER), coded);
	fwrite(hRGB, sizeof(RGBQUAD), 256, coded);
	fwrite(lpImg, sizeof(char), hInfo.biSizeImage, coded);

	
	fclose(coded);
}
/*
int resizeString(char* sentence, int* sz)
{
	(*sz)++;
	sentence = realloc(sentence, *sz);

	return 0;
}
*/
int getSize(FILE* fp)
{
	BITMAPINFOHEADER hInfo; //����
	BITMAPFILEHEADER hf;

	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);//�Է�
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);//�Է�


	size = (hInfo.biSizeImage / 3);
	return 0;
}