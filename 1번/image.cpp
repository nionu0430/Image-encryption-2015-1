/*
	LSB 방식을 사용한 이미지 암호화
	남벤져스
	김경훈 장세훈 남재민 김형기 정용현
	최종수정일 15.07.21
*/
/*
	문제점
	->크기가 1kb정도 커지는 것에대한 이유를 알아낼 수 없었음
	개선방향 및 추후 목표
	1) 크기가 커지는 것을 잡아야함
	2) API 등을 이용하여서 원하는 파일들로 암호화를 할 수 있는 프로그램 만들기
	3) 암호가 저장될 떄 암호화 기법사용
*/
#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <conio.h>


int fileSize = 1;
int size = 0;

void deco(FILE* coded); //암호 추출
void code(FILE* origin, FILE* coded); //암호 입력
void arraySet(int bit[]); //배열 초기화

int getSize(FILE* fp);
void getString(char* sentence);
int resizeString(char* sentence);


int main(void)
{
	FILE* input; // 그림파일 원본
	FILE* output;// 암호화된 그림파일
	FILE* decode; // 해석할때 사용하는 그림파일
	FILE* textIn, *textOut; //입력과 출력되는 암호문
	
	//불러오는것과 검사
	fopen_s(&input, "test2.bmp", "r");
	if (input == NULL){
		printf("입력하고자 하는 bmp 파일이 없습니다.\n"); return 0;}
	fopen_s(&output, "coded.bmp", "w");

	if (input == NULL) { printf("입력하고자 하는 bmp 파일이 없습니다.\n"); return 0; }
	code(input, output);

	fopen_s(&decode, "coded.bmp", "r");
	deco(decode);
}

void deco(FILE* coded)
{
	int bit[8];
	FILE* textOut;
	BITMAPINFOHEADER hInfo; //정보
	BITMAPFILEHEADER hf; //파일 헤더
	RGBQUAD hRGB[256]; //팔레트를 받아옴
	BYTE *lpImg; //이미지의 값 
	int i, j;
	int ch = 1;//8개의 비트로 부터 숫자를 저장 받음 shifting을 통해서
	char character; //숫자를 문자로 바꿔줌
	char* code; //저장되는값
	int sz = 0, head = 0; //저장 된 문자열의 주소와 크기 //메모리 안의 위치




	fread(&hf, sizeof(BITMAPFILEHEADER), 1, coded);
	if (hf.bfType != 0x4D42) printf("비트맵이미지가 아닌디\n"); ///0x4D42 = BM
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, coded);//입력
	fread(hRGB, sizeof(RGBQUAD), 256, coded); //팔레트

	lpImg = (BYTE*)malloc(hInfo.biSizeImage);
	fread(lpImg, sizeof(char), hInfo.biSizeImage, coded); //데이터를 읽어옴
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
		fputc(ch,textOut);//입력
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
	BITMAPINFOHEADER hInfo; //정보
	BITMAPFILEHEADER hf; //파일 헤더
	RGBQUAD hRGB[256]; //팔레트를 받아옴
	BYTE *lpImg; //이미지의 값 //실제 그림파일의 데이터 만큼 읽어들임

	FILE* textIn;

	
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, origin);//입력
	if (hf.bfType != 0x4D42) printf("비트맵이미지가 아닌디\n"); ///0x4D42 = BM
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, origin);//입력
	fread(hRGB, sizeof(RGBQUAD), 256, origin); //팔레트

	lpImg = (BYTE*)malloc(hInfo.biSizeImage);
	fread(lpImg, sizeof(char), hInfo.biSizeImage, origin); //데이터를 읽어옴
	fclose(origin);

	size = hInfo.biSizeImage;

	textIn = fopen("input.txt", "r");
	if (textIn == NULL){
		printf("입력하고자 하는 텍스트 파일이 없습니다.\n");
		exit(1);
	}


	while (1) //파일에 들어있는 암호문의 크기를 구함
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
		for (i = 0; i < 8 * (fileSize + 1); i++) //0으로 모든 lsb를 만들어줌
		{
			if ((lpImg[i] >> 0) % 2 == 1)
				lpImg[i] -= 1;
		}
	}
	else
	{
		for (i = 0; i < size; i++) //0으로 모든 lsb를 만들어줌
		{
			if ((lpImg[i] >> 0) % 2 == 1)
				lpImg[i] -= 1;
		}
		max = size - 1;
	}
	fseek(textIn, 0L, SEEK_SET);

	for (j = 0; j < max; j++) //구한 파일의 size 만큼 입력해줌
	{
		c = getc(textIn);
		printf("%c", c);
		C = c; //숫자로
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
	BITMAPINFOHEADER hInfo; //정보
	BITMAPFILEHEADER hf;

	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);//입력
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);//입력


	size = (hInfo.biSizeImage / 3);
	return 0;
}