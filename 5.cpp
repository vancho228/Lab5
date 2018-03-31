#include <DOS.H>
#include <conio.h> 
#include <stdio.h> 
#include <emurtk.h> 

unsigned char SNS0, SNS1, SNS2;
int SN[18], i;

unsigned char inputD() { //Считываем данные с концевых датчиков
	unsigned char data = 0;
	outportb(0x37A, 0x2E);
	outportb(0x378, 0xFF);
	outportb(0x37A, 0x2F);
	delay(1);
	data = inportb(0x378);
	outportb(0x37A, 0x04);
	return data;
}
void writeRC(char data) {
	outportb(0x37A, 0x04);
	outportb(0x378, data);
	outportb(0x37A, 0x00);
	delay(1);
	outportb(0x37A, 0x04);
}
void writeRA(char data) {
	outportb(0x37A, 0x0E);
	outportb(0x378, data);
	outportb(0x37A, 0x0A);
	delay(1);
	outportb(0x37A, 0x0E);
}
void Doit(char DRV) {
	writeRC(0x11);
	writeRA(0x80);
	writeRA(DRV);
}

void Conclusion() {

	writeRC(0x13);
	SNS0 = inputD();
	SNS1 = inputD();
	SNS2 = inputD();

	int SNS[3] = { SNS0 ,SNS1 ,SNS2 };
	int u = 0, i, j;
	for (i = 0; i<3; i++) {
		for (j = 0; j<6; j++) {
			SN[u] = (SNS[i] + 1) % 2;//Инвентируем значения 
			SNS[i] /= 2;
			u++;
		}
	}
	printf("\n");
	printf(" M0 M1 M2 M3 M4 M5");

	for (i = 0; i<4; i++) {

		switch (i) {
		case 1: printf("Start"); break;
		case 2: printf("End "); break;
		case 3: printf("Move "); break;
		}

		for (j = 0; j<6; j++) {	//Для старта нам нужен каждый 3 элемент начиная с 0
								//Для датчика конца с 1 элемента
			printf(" ");	//Для импульсного датчика начиная со 2 элемента
			if (j > 0) { printf(" "); }

			printf("%d", SN[((i - 1) + (3 * j))]);

		}
	}
}
void Stop() {	//Остановка
	writeRC(0x11);
	writeRA(0x00);
	writeRA(0x00);
}

int Action()
{
	int text, up, down;
	
	switch (i)
	{
	case 49:
		text = 1;
		up = 0x01;
		down = 0x02;
		break;
	case 50:
		text = 2;
		up = 0x10;
		down = 0x20;
		break;
	case 51:
		text = 3;
		up = 0x04;
		down = 0x08;
		break;
	case 27:
		return 0;
		break;
	default:
		break;
	}
	printf("%d", text);
	switch (getch())
	{
	case 119:
		Doit(up);
		break;
	case 115:
		Doit(down);
		break;
	case 32:
		Stop();
		break;
	case 50:
		i = 50;
		Action();
		break;
	case 51:
		i = 51;
		Action();
		break;
	case 49:
		i = 49;
		Action();
		break;
	case 27:
		return 0;
		break;
	default:
		printf("Error");
		break;
	}
	return 1;
}

int main() {
	int flag = 1, c;
	i = getch();
	while (flag == 1) {
		
		flag = Action();
	}
	return 0;
}
