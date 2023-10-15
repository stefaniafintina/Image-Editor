#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct
{
	int r, g, b;
}RGB;
//reprezentarea imaginii RGB
typedef struct
{
	char *magick;//P3, P6
	int x1, x2, y1, y2;//selectiile
	int n, m, val_max;//nr de linii, de coloane si val max
	RGB **mat;//matricea de pixeli
	RGB **a;
}imagine_rgb;
//reprezentarea imaginii Grayscale
typedef struct
{
	char *magick;//P2, P5
	int x1, x2, y1, y2;//selectiile
	int n, m, val_max;//nr de linii de coloane si valoarea maaxima
	int **mat;//matricea de pixeli
	int **a;
}grayscale;
//alocare sir de caractere
void aloc_char(char **v, int n)
{
	char *w = (char *)malloc(n * sizeof(char));
	if (!w) {
		printf("Malloc a esuat\n");
		return;
	}
	*v = w;
}

//alocarea matricei
void aloc_mat(int ***a, int l, int c)
{
	int **w;
	w = (int **)malloc(l * sizeof(int *));
	if (!w) {
		printf("Malloc a esuat!\n");
		return;
	}
	for (int i = 0; i < l; i++) {
		w[i] = (int *)malloc(c * sizeof(int));
		if (!w) {
			printf("Malloc a esuat!\n");
			return;
		}
	}
	*a = w;
}
//eliberare matrice
void elib_mat(int ***a, int n)
{
	int **pta = *a;
	for (int i = 0; i < n; i++)
		free(pta[i]);
	free(pta);
	*a = pta;
}
//alocare structura RGB
void aloc_rgb(imagine_rgb *img)
{
	img->mat=(RGB **)malloc(img->m * sizeof(RGB *));
	if (!img->mat) {
		printf("Malloc a esuat!\n");
		return;
	}
	for (int i = 0; i < img->m; i++) {
		img->mat[i] = (RGB *)malloc(img->n * sizeof(RGB));
		if (!img->mat[i]) {
			printf("Malloc a esuat!\n");
			return;
		}
	}
}
//alocare structura grayscale
void aloc_grayscale(grayscale *imag)
{
	imag->mat=(int **)malloc(imag->m * sizeof(int *));
	if (!imag->mat) {
		printf("Malloc a esuat!\n");
		return;
	}
	for (int i = 0; i < imag->m; i++) {
		imag->mat[i] = (int *)malloc(imag->n * sizeof(int));
		if (!imag->mat[i]) {
			printf("Malloc a esuat!\n");
			return;
		}
	}
}
//realocare structura grayscale
int **re_aloc_gs(int m, int n)
{
	int **a=(int **)malloc(m * sizeof(int *));
	if (!a) {
		printf("Malloc a esuat!\n");
		return NULL;
	}
	for (int i = 0; i < m; i++) {
		a[i] = (int *)malloc(n * sizeof(int));
		if (!a[i]) {
			printf("Malloc a esuat!\n");
			return NULL;
		}
	}
	return a;
}
//realocare structura RGB
RGB **re_aloc_rgb(int m, int n)
{
	RGB **a=(RGB **)malloc(m * sizeof(RGB *));
	if (!a) {
		printf("Malloc a esuat!\n");
		return NULL;
	}
	for (int i = 0; i < m; i++) {
		a[i] = (RGB *)malloc(n * sizeof(RGB));
		if (!a[i]) {
			printf("Malloc a esuat!\n");
			return NULL;
		}
	}
	return a;
}
//eloberare matrice de pixeli grayscale
void elib_grayscale(grayscale *imag) {
	for (int i = 0; i < imag->m; i++)
		free(imag->mat[i]);
	free(imag->mat);
}
//eliberare matrice selectie de pixeli
void elib_grayscale_1(grayscale *imag) {
	for (int i = 0; i < imag->x2 - imag->x1; i++)
		free(imag->a[i]);
	free(imag->a);
}
//eliberare matrice pizeli RGB
void elib_rgb(imagine_rgb *img) {
	for (int i = 0; i < img->m; i++)
		free(img->mat[i]);
	free(img->mat);
}
//eliberare matrice selectie pixeli
void elib_rgb_1(imagine_rgb *img) {
	for (int i = 0; i < img->x2 - img->x1; i++)
		free(img->a[i]);
	free(img->a);
}
//citire imagine P2
void P2(grayscale *imag, FILE* in)
{
	char c;
	fscanf(in, "%c", &c); //verific daca e comentariu
		if(c != '#') {// nu e comentariu
			fseek(in, -1, SEEK_CUR);//ma duc la inceputul randului
		} else {
			while(1) {
				while(c != '\n')
					fscanf(in, "%c", &c);
				fscanf(in, "%c", &c);
				//daca primul carcater de pe un rand e diferit de # opresc while -ul
				// inseamna ca nu am comentariu pe randul respectiv
				if(c != '#') {
					fseek(in, -1, SEEK_CUR);
					break;
				}
			}
		}
		fscanf(in, "%d %d\n", &imag->n, &imag->m);//citesc dimensiunile
		fscanf(in, "%c", &c);
		//verific daca e comentariu sau nu
		if(c != '#') {
			fseek(in, -1, SEEK_CUR);
		} else {
			while(1) {
				while(c != '\n')
					fscanf(in, "%c", &c);
				fscanf(in, "%c", &c);
				if(c != '#') {
					fseek(in, -1, SEEK_CUR);
					break;
				}
			}
		}
		//aloc matricea de pixeli Grayscale
		aloc_grayscale(imag);
		fscanf(in, "%d\n", &imag->val_max);
		for(int i = 0; i < imag->m; i++){
			for(int j = 0; j < imag->n; j++) {
				fscanf(in, "%d", &imag->mat[i][j]);
			}
		}
}
//citire imagine P5
void P5(grayscale *imag, FILE* in)
{
	imag->val_max = 0;
	char c;
	fread(&c, sizeof(char), 1, in);
		imag->n = 0;
		imag->m = 0;
		while(c!=' ') {
			imag->n= imag->n * 10 + c - '0';//formez numarul de coloane
			fread(&c, sizeof(char), 1, in);
		}
		fread(&c, sizeof(char), 1, in);
		while(c!='\n') {
			imag->m= imag->m * 10 + c - '0';//nr de linii
			fread(&c, sizeof(char), 1, in);
		}
		fread(&c, sizeof(char), 1, in);
		while(c!='\n') {
			imag->val_max = imag->val_max * 10 + c - '0';//valoarea maxima
			fread(&c, sizeof(char), 1, in);
		}
		unsigned char elem;
		//aloc matricea de pixeli
		aloc_grayscale(imag);
		for(int i = 0; i < imag->m; i++) {
			for(int j = 0; j < imag->n ; j++) {
				fread(&elem, sizeof(unsigned char), 1, in);
				imag->mat[i][j] = (int)elem;
			}
		}
}
//citire imagine P3
void P3(imagine_rgb *img, FILE* in)
{
	char c;
	fscanf(in, "%c", &c);
		if(c != '#') {
			fseek(in, -1, SEEK_CUR);
		} else {
			while(1) {
				while(c != '\n')
					fscanf(in, "%c", &c);
				fscanf(in, "%c", &c);
				if(c != '#') {
					fseek(in, -1, SEEK_CUR);
					break;
				}
			}
		}
		fscanf(in, "%d %d\n", &img->n, &img->m);
		aloc_rgb(img);
		fscanf(in, "%c", &c);
		if(c != '#') {
			fseek(in, -1, SEEK_CUR);
		} else {
			while(1) {
				while(c != '\n')
					fscanf(in, "%c", &c);
				fscanf(in, "%c", &c);
				if(c != '#') {
					fseek(in, -1, SEEK_CUR);
					break;
				}
			}
		}
		fscanf(in, "%d\n", &img->val_max);
		for(int i = 0; i < img->m; i++){
			for(int j = 0; j < img->n; j++) {
				fscanf(in, "%d %d %d", &img->mat[i][j].r, &img->mat[i][j].g, &img->mat[i][j].b);
			}
		}
}
//citire imagine P6
void P6(imagine_rgb *img, FILE* in)
{
	img->val_max = 0;
	char c;
	fread(&c, sizeof(char), 1, in);
	img->n = 0;
	img->m = 0;
	while(c!=' ') {
		img->n= img->n * 10 + c - '0';
		fread(&c, sizeof(char), 1, in);
	}
	fread(&c, sizeof(char), 1, in);
	while(c!='\n') {
		img->m= img->m * 10 + c - '0';
		fread(&c, sizeof(char), 1, in);
	}
	fread(&c, sizeof(char), 1, in);
	while(c!='\n') {
		img->val_max = img->val_max * 10 + c - '0';
		fread(&c, sizeof(char), 1, in);
	}
	unsigned char elem;
	aloc_rgb(img);
	for(int i = 0; i < img->m; i++) {
		for(int j = 0; j < img->n ; j++) {
			fread(&elem, sizeof(unsigned char), 1, in);
			img->mat[i][j].r = (int)elem;
			fread(&elem, sizeof(unsigned char), 1, in);
			img->mat[i][j].g = (int)elem;
			fread(&elem, sizeof(unsigned char), 1, in);
			img->mat[i][j].b = (int)elem;
		}
	}
}
//Operatia LOAD
void LOAD(imagine_rgb *img, grayscale *imag, FILE* in, int *tip)
{
	char *p;
	int cont = 0;
	aloc_char(&p, 3);
	char c;
	fread(&c, sizeof(char), 1, in);
	while(c!='\n') {
		p[cont] = c;
		fscanf(in, "%c", &c);
		cont++;
	}
	p[cont] = '\0';
	if(strcmp(p,"P2")==0) {
		strcpy(imag->magick, p);
		*tip = 1;
		P2(imag, in);
	}
	if(strcmp(p,"P5") == 0) {
		strcpy(imag->magick, p);
		*tip = 1;
		P5(imag, in);
	}
	if(strcmp(p,"P3") == 0) {
		strcpy(img->magick, p);
		*tip = 2;
		P3(img, in);
	}
	if(strcmp(p,"P6") == 0) {
		strcpy(img->magick, p);
		*tip = 2;
		P6(img, in);
	}
	free(p);
}
//retine cifrele din n intr un vector
int *ext_cifre(int n)
{
	int cpy = n;
	int cnt = 0;
	int *cif = malloc(10 * sizeof(int));
	if(!cif) {
		printf("Malloc a esuat\n");
		return NULL;
	}
	while(cpy) {
		cif[cnt++] = cpy % 10;
		cpy /= 10;
	}
	return cif;
}
//numar de cifre dintr-un numar
int nrcifre(int n)
{
	if (n < 10)
		return 1;
	else
		return 1 + nrcifre(n / 10);
}
//afisare imagine P2
void P25out_ascii (grayscale *imag, FILE* out)
{
	fprintf(out, "P2\n");
	fprintf(out, "%d %d\n", imag->n, imag->m);
	fprintf(out, "%d\n", imag->val_max);
	for(int i = 0; i < imag->m; i++) {
		for(int j = 0; j < imag->n; j++) {
			fprintf(out, "%d\n", imag->mat[i][j]);
		}
	}
}
//afisare imagine P3
void P36out_ascii (imagine_rgb *img, FILE* out)
{
	fprintf(out, "P3\n");
	fprintf(out, "%d %d\n", img->n, img->m);
	fprintf(out, "%d\n", img->val_max);
	for(int i = 0; i < img->m; i++) {
		for(int j = 0; j < img->n; j++) {
			fprintf(out, "%d %d %d\n", img->mat[i][j].r, img->mat[i][j].g, img->mat[i][j].b);
		}
	}
}
//Afisare imagine P5
void P25out_bin(grayscale *imag, FILE* out)
{
	fwrite("P5", sizeof(char), strlen("P5"), out);
	fwrite("\n", sizeof(char), strlen("\n"), out);
	int *cif;
	unsigned char new;
	cif = ext_cifre(imag->n);
	for (int i = nrcifre(imag->n) - 1; i  >= 0; i--) {
		new = '0' + cif[i];
		fwrite(&new, sizeof(char), 1, out);
	}
	free(cif);
	fwrite(" ", sizeof(char), strlen(" "), out);
	cif = ext_cifre(imag->m);
	for (int i = nrcifre(imag->m) - 1; i >= 0; i--) {
		new = '0' + cif[i];
		fwrite(&new, sizeof(char), 1, out);
	}
	free(cif);
	fwrite("\n", sizeof(char), strlen("\n"), out);
	cif = ext_cifre(imag->val_max);
	for (int i = nrcifre(imag->val_max) - 1; i >= 0; i--) {
		new = '0' + cif[i];
		fwrite(&new, sizeof(char), 1, out);
	}
	fwrite("\n", sizeof(char), strlen("\n"), out);
	for (int i = 0; i < imag->m; i++) {
		for (int j = 0; j < imag->n; j++) {
			new = (unsigned char)imag->mat[i][j];
			fwrite(&new, sizeof(unsigned char), 1, out);
		}
	}
	free(cif);
}
//verific daca un element e pe contur sau nu
int nu_pe_contur(int x, int y, int n, int m)
{
	if (x > 0 && y > 0 && x < n - 1 && y < m - 1)
		return 1;
	return 0;
}
//afisare imagine P6
void P36out_bin(imagine_rgb *img, FILE* out)
{
	fwrite("P6", sizeof(char), strlen("P6"), out);
	fwrite("\n", sizeof(char), strlen("\n"), out);
	int *cif;
	unsigned char new;
	cif = ext_cifre(img->n);
	for (int i = nrcifre(img->n) - 1; i >= 0; i--) {
		new = '0' + cif[i];
		fwrite(&new, sizeof(char), 1, out);
	}
	free(cif);
	fwrite(" ", sizeof(char), strlen(" "), out);
	cif = ext_cifre(img->m);
	for (int i = nrcifre(img->m) - 1; i >= 0; i--) {
		new = '0' + cif[i];
		fwrite(&new, sizeof(char), 1, out);
	}
	free(cif);
	fwrite("\n", sizeof(char), strlen("\n"), out);
	cif = ext_cifre(img->val_max);
	for (int i = nrcifre(img->val_max) - 1; i >= 0; i--) {
		new = '0' + cif[i];
		fwrite(&new, sizeof(char), 1, out);
	}
	fwrite("\n", sizeof(char), strlen("\n"), out);
	for (int i = 0; i < img->m; i++) {
		for (int j = 0; j < img->n; j++) {
			new = (unsigned char)img->mat[i][j].r;
			fwrite(&new, sizeof(unsigned char), 1, out);
			new = (unsigned char)img->mat[i][j].g;
			fwrite(&new, sizeof(unsigned char), 1, out);
			new = (unsigned char)img->mat[i][j].b;
			fwrite(&new, sizeof(unsigned char), 1, out);
		}
	}
	free(cif);
}
//aplicare efect EDGE
void EDGE_rgb(imagine_rgb *img)
{
	int red;
	int green;
	int blue;
	RGB **a;
	a =(RGB **)malloc(img->m * sizeof(RGB *));
	if (!a) {
		printf("Malloc a esuat!\n");
		return;
	}
	for (int i = 0; i < img->m; i++) {
		a[i] = (RGB *)malloc(img->n * sizeof(RGB));
		if (!a[i]) {
			printf("Malloc a esuat!\n");
			return;
		}
	}
	for(int i = img->y1; i < img->y2 ; i++) {
		for(int j = img->x1; j < img->x2; j++) {
			if (nu_pe_contur(j, i, img->n, img->m)) {
				red = 0;
				green = 0;
				blue = 0;
				red =  img->mat[i][j].r * 8 - img->mat[i][j - 1].r - img->mat[i][j + 1].r;
				red = red - (img->mat[i - 1][j].r + img->mat[i + 1][j].r + img->mat[i - 1][j - 1].r);
				red = red - (img->mat[i - 1][j + 1].r + img->mat[i + 1][j - 1].r + img->mat[i + 1][j + 1].r);
				if (red < 0)
					red = 0;
				if (red > 255)
					red = 255;
				green =  img->mat[i][j].g * 8 - img->mat[i][j - 1].g - img->mat[i][j + 1].g;
				green = green - (img->mat[i - 1][j].g + img->mat[i + 1][j].g + img->mat[i - 1][j - 1].g);
				green = green - (img->mat[i - 1][j + 1].g + img->mat[i + 1][j - 1].g + img->mat[i + 1][j + 1].g);
				if (green < 0)
					green = 0;
				if (green > 255)
					green = 255;
				blue =  img->mat[i][j].b * 8 - img->mat[i][j - 1].b - img->mat[i][j + 1].b;
				blue = blue - (img->mat[i - 1][j].b + img->mat[i + 1][j].b + img->mat[i - 1][j - 1].b);
				blue = blue - (img->mat[i - 1][j + 1].b + img->mat[i + 1][j - 1].b + img->mat[i + 1][j + 1].b);
				if(blue < 0)
					blue = 0;
				if(blue > 255)
					blue = 255;
				a[i][j].r = red;
				a[i][j].g = green;
				a[i][j].b = blue;
			}
		}
	}
	for(int i = img->y1; i < img->y2; i++) {
		for(int j = img->x1; j < img->x2; j++) {
			if (nu_pe_contur(j, i, img->n, img->m)) {
			img->mat[i][j].r = a[i][j].r;
			img->mat[i][j].g = a[i][j].g;
			img->mat[i][j].b = a[i][j].b;
			}
		}
	}
	for (int i = 0; i < img->m; i++) 
		free(a[i]);
	free(a);
}
//Aplicare efect sharpen
void sharpen_rgb(imagine_rgb *img)
{
	int red;
	int green;
	int blue;
	RGB **a;
	a =(RGB **)malloc(img->m * sizeof(RGB *));
	if (!a) {
		printf("Malloc a esuat!\n");
		return;
	}
	for (int i = 0; i < img->m; i++) {
		a[i] = (RGB *)malloc(img->n * sizeof(RGB));
		if (!a) {
			printf("Malloc a esuat!\n");
			return;
		}
	}
	for(int i = img->y1; i < img->y2; i++) {
		for(int j = img->x1; j < img->x2; j++) {
			if (nu_pe_contur(j, i, img->n, img->m)) {
				red = 0;
				green = 0;
				blue = 0;
				red +=  img->mat[i][j].r * 5 - img->mat[i][j - 1].r - img->mat[i][j + 1].r;
				red -= (img->mat[i - 1][j].r + img->mat[i + 1][j].r);
				if (red < 0)
					red = 0;
				if (red > 255)
					red = 255;
				green +=  img->mat[i][j].g * 5 - img->mat[i][j - 1].g - img->mat[i][j + 1].g;
				green -= (img->mat[i - 1][j].g + img->mat[i + 1][j].g);
				if (green < 0)
					green = 0;
				if (green > 255)
					green = 255;
				blue =  img->mat[i][j].b * 5 - img->mat[i][j - 1].b - img->mat[i][j + 1].b;
				blue -= (img->mat[i - 1][j].b + img->mat[i + 1][j].b);
				if (blue < 0)
					blue = 0;
				if (blue > 255)
					blue = 255;
				a[i][j].r = red;
				a[i][j].g = green;
				a[i][j].b = blue;
			}
		}
	}
	for(int i = img->y1; i < img->y2; i++) {
		for(int j = img->x1; j < img->x2; j++) {
			if (nu_pe_contur(j, i, img->n, img->m)) {
			img->mat[i][j].r = a[i][j].r;
			img->mat[i][j].g = a[i][j].g;
			img->mat[i][j].b = a[i][j].b;
			}
		}
	}
	for (int i = 0; i < img->m; i++) 
		free(a[i]);
	free(a);
}
//aplicare efect blur
void blur_rgb(imagine_rgb *img)
{
	double red;
	double green;
	double blue;
	RGB **a;
	a =(RGB **)malloc(img->m * sizeof(RGB *));
	if (!a) {
		printf("Malloc a esuat!\n");
		return;
	}
	for (int i = 0; i < img->m; i++) {
		a[i] = (RGB *)malloc(img->n * sizeof(RGB));
		if (!a) {
			printf("Malloc a esuat!\n");
			return;
		}
	}
	for(int i = img->y1; i < img->y2; i++) {
		for(int j = img->x1; j < img->x2; j++) {
			if (nu_pe_contur(j, i, img->n, img->m)) {
				red = 0;
				green = 0;
				blue = 0;
				red =  img->mat[i][j].r + img->mat[i][j - 1].r + img->mat[i][j + 1].r;
				red += (img->mat[i - 1][j].r + img->mat[i + 1][j].r + img->mat[i - 1][j - 1].r);
				red += (img->mat[i - 1][j + 1].r + img->mat[i + 1][j - 1].r + img->mat[i + 1][j + 1].r);
				red = red / 9;
				green =  img->mat[i][j].g + img->mat[i][j - 1].g + img->mat[i][j + 1].g;
				green += (img->mat[i - 1][j].g + img->mat[i + 1][j].g + img->mat[i - 1][j - 1].g);
				green += (img->mat[i - 1][j + 1].g + img->mat[i + 1][j - 1].g + img->mat[i + 1][j + 1].g);
				green = green / 9;
				blue =  img->mat[i][j].b + img->mat[i][j - 1].b + img->mat[i][j + 1].b;
				blue += (img->mat[i - 1][j].b + img->mat[i + 1][j].b + img->mat[i - 1][j - 1].b);
				blue += (img->mat[i - 1][j + 1].b + img->mat[i + 1][j - 1].b + img->mat[i + 1][j + 1].b);
				blue /= 9;
				a[i][j].r = round(red);
				a[i][j].g = round(green);
				a[i][j].b = round(blue);
			}
		}
	}
	for(int i = img->y1; i < img->y2; i++) {
		for(int j = img->x1; j < img->x2; j++) {
			if (nu_pe_contur(j, i, img->n, img->m)) {
			img->mat[i][j].r = a[i][j].r;
			img->mat[i][j].g = a[i][j].g;
			img->mat[i][j].b = a[i][j].b;
			}
		}
	}
	for (int i = 0; i < img->m; i++) 
		free(a[i]);
	free(a);
}
//Aplicare efect Gaussian blur
void g_blur_rgb(imagine_rgb *img)
{
	double red;
	double green;
	double blue;
	RGB **a;
	a =(RGB **)malloc(img->m * sizeof(RGB *));
	if (!a) {
		printf("Malloc a esuat!\n");
		return;
	}
	for (int i = 0; i < img->m; i++) {
		a[i] = (RGB *)malloc(img->n * sizeof(RGB));
		if (!a) {
			printf("Malloc a esuat!\n");
			return;
		}
	}
	for(int i = img->y1; i < img->y2; i++) {
		for(int j = img->x1; j < img->x2; j++) {
			if (nu_pe_contur(j, i, img->n, img->m)) {
				red = 0;
				green = 0;
				blue = 0;
				red =  img->mat[i][j].r * 4 + img->mat[i][j - 1].r * 2 + img->mat[i][j + 1].r * 2;
				red += (img->mat[i - 1][j].r * 2 + img->mat[i + 1][j].r * 2 + img->mat[i - 1][j - 1].r);
				red += (img->mat[i - 1][j + 1].r + img->mat[i + 1][j - 1].r + img->mat[i + 1][j + 1].r);
				red = red / 16;
				green =  img->mat[i][j].g * 4 + img->mat[i][j - 1].g * 2 + img->mat[i][j + 1].g * 2;
				green += (img->mat[i - 1][j].g * 2 + img->mat[i + 1][j].g * 2 + img->mat[i - 1][j - 1].g);
				green += (img->mat[i - 1][j + 1].g + img->mat[i + 1][j - 1].g + img->mat[i + 1][j + 1].g);
				green = green / 16;
				blue =  img->mat[i][j].b * 4 + img->mat[i][j - 1].b * 2 + img->mat[i][j + 1].b * 2;
				blue += (img->mat[i - 1][j].b * 2 + img->mat[i + 1][j].b * 2 + img->mat[i - 1][j - 1].b);
				blue += (img->mat[i - 1][j + 1].b + img->mat[i + 1][j - 1].b + img->mat[i + 1][j + 1].b);
				blue /= 16;
				a[i][j].r = round(red);
				a[i][j].g = round(green);
				a[i][j].b = round(blue);
			}
		}
	}
	for(int i = img->y1; i < img->y2; i++) {
		for(int j = img->x1; j < img->x2; j++) {
			if (nu_pe_contur(j, i, img->n, img->m)) {
			img->mat[i][j].r = a[i][j].r;
			img->mat[i][j].g = a[i][j].g;
			img->mat[i][j].b = a[i][j].b;
			}
		}
	}
	for (int i = 0; i < img->m; i++) 
		free(a[i]);
	free(a);
}

void rotate_full_G(grayscale *imag)
{
	int **a;
	a = (int **)calloc(imag->n, sizeof(int *));
	if(!a) {
		printf("Malloc failed\n");
		return;
	}
	for(int i = 0; i< imag->n; i++) {
		a[i] = (int *)calloc(imag->m, sizeof(int));
		if(!a[i]) {
			printf("Malloc failed\n");
			return;
		}
	}
	int j = 0;
	for (int i = 0; i < imag->n; i++) {
		for (int k = imag->m - 1; k >= 0; k--) {
			a[i][k] = imag->mat[imag->m - 1 - k][j];
		}
		j++;
	}		

	for (int i = 0; i < imag->m; i++) 
		free(imag->mat[i]);
	free(imag->mat);

	int aux = imag->m;
	imag->m = imag->n; 
	imag->n = aux;

	imag->mat = (int **)malloc(imag->m * sizeof(int *));
	for (int i = 0; i < imag->m; i++)
		imag->mat[i] = (int *)malloc(imag->n * sizeof(int));

	for(int i = 0; i < imag->m; i++) {
		for(int j = 0; j < imag->n; j++) {
			imag->mat[i][j] = a[i][j];
		}
	}

	for (int i = 0; i < imag->m; i++)
		free(a[i]);
	free(a);

	imag->x1 = 0;
		imag->x2 = imag->n;
		imag->y1 = 0;
		imag->y2 = imag->m;

}

void rotate_full_R(imagine_rgb *img)
{
	RGB **a;
	a = (RGB **)calloc(img->n, sizeof(RGB *));
	if(!a) {
		printf("Malloc failed\n");
		return;
	}
	for(int i = 0; i< img->n; i++) {
		a[i] = (RGB *)calloc(img->m, sizeof(RGB));
		if(!a[i]) {
			printf("Malloc failed\n");
			return;
		}
	}
	int j = 0;
	for (int i = 0; i < img->n; i++) {
		for (int k = img->m - 1; k >= 0; k--) {
			a[i][k] = img->mat[img->m - k - 1][j];
		}
		j++;
	}	
	

	for (int i = 0; i < img->m; i++) 
		free(img->mat[i]);
	free(img->mat);

	int aux = img->m;
	img->m = img->n; 
	img->n = aux;

	img->mat = (RGB **)malloc(img->m * sizeof(RGB *));
	for (int i = 0; i < img->m; i++)
		img->mat[i] = (RGB *)malloc(img->n * sizeof(RGB));

	for(int i = 0; i < img->m; i++) {
		for(int j = 0; j < img->n; j++) {
			img->mat[i][j] = a[i][j];
		}
	}
	
	for (int i = 0; i < img->m; i++)
		free(a[i]);
	free(a);

	img->x1 = 0;
		img->x2 = img->n;
		img->y1 = 0;
		img->y2 = img->m;
	
}

//functie de rotire la dreapta pentru imaginile Gray
void rotate_right_G(grayscale *imag) {
	

	int **a;
	int new_x, new_y;
	new_x = imag->x2 - imag->x1;
	new_y = imag->y2 - imag->y1;
	a = (int **)calloc(imag->n, sizeof(int *));
	if(!a) {
		printf("Malloc failed\n");
		return;
	}
	for(int i = 0; i< imag->n; i++) {
		a[i] = (int *)calloc(imag->m, sizeof(int));
		if(!a[i]) {
			printf("Malloc failed\n");
			return;
		}
	}
	int j = imag->x1;
	for (int i = imag->x1; i < imag->x2; i++) {
		for (int k = imag->y2 - 1; k >= imag->y1; k--) {
			a[i][k] = imag->mat[imag->y2 - k][j];
		}
		j++;
	}		
	for(int i = imag->y1; i < imag->y2; i++) {
		for(int j = imag->x1; j < imag->x2; j++) {
			imag->mat[i][j] = a[i][j];
		}
	}
	for (int i = 0; i < imag->n; i++)
		free(a[i]);
	free(a);

}
//functie pentru rotire la stanga pentru imaginile RGB
void rotate_right_R(imagine_rgb *img) {
	RGB **a;
	int new_x, new_y;
	new_x = img->x2 - img->x1;
	new_y = img->y2 - img->y1;
	a = (RGB **)calloc(img->n, sizeof(RGB *));
	if(!a) {
		printf("Malloc failed\n");
		return;
	}
	for(int i = 0; i< img->n; i++) {
		a[i] = (RGB *)calloc(img->m, sizeof(RGB));
		if(!a[i]) {
			printf("Malloc failed\n");
			return;
		}
	}
	int j = img->x1;
	for (int i = img->x1; i < img->x2; i++) {
		for (int k = img->y2 - 1; k >= img->y1; k--) {
			a[i][k] = img->mat[img->y2 - k][j];
		}
		j++;
	}
	
	for(int i = img->y1; i < img->y2; i++) {
		for(int j = img->x1; j < img->x2; j++) {
			img->mat[i][j] = a[i][j];
		}
	}
	for (int i = 0; i < img->n; i++)
		free(a[i]);
	free(a);
	
}



int main()
{
	char *operatie;
	grayscale imag;
	imagine_rgb img;
	int tip = 0; // 0 - fara imagine, 1 - grayscale, 2 - rgb
	char *param;
	char *s;
	int l;
	aloc_char(&s, 10);
	aloc_char(&param, 50);
	aloc_char(&operatie, 10);
	aloc_char(&img.magick, 3);
	aloc_char(&imag.magick, 3);
	strcpy(imag.magick, "ab");
	strcpy(img.magick, "ab");
	int cnt_rgb = 0, cnt_gray = 0;
	while(1) {
		scanf(" %s", operatie);
		//operatia load
		if(strcmp(operatie,"LOAD") == 0) {
			char nume[30];//numele fisierului
			scanf(" %s", nume);
			FILE *in = fopen(nume, "rb");
			if(!in) {
				fprintf(stdout, "Failed to load %s\n", nume);
				if (tip ==1)
					elib_grayscale(&imag);
				else if (tip == 2)
					elib_rgb(&img);
				tip = 0;
			} else {
				if (tip == 1 && cnt_gray == 1) {
					elib_grayscale(&imag);
				} else if (tip == 2 && cnt_rgb == 1) {
					elib_rgb(&img);
				}
				LOAD(&img, &imag, in, &tip);
				if (tip == 1) {
					cnt_gray = 1;
					imag.x1 = 0;
					imag.x2 = imag.n;
					imag.y1 = 0;
					imag.y2 = imag.m;
				} else if (tip ==2) {
					cnt_rgb = 1;
					img.x1 = 0;
					img.x2 = img.n;
					img.y1 = 0;
					img.y2 = img.m;
				}
				fclose(in);
				printf("Loaded %s\n", nume);
			}
		} else if (strcmp(operatie, "EXIT") == 0) {
			free(operatie);
			if (tip ==1)
				elib_grayscale(&imag); 
			else if (tip == 2)
				elib_rgb(&img);
			else {
				printf("No image loaded\n");
			}
			tip = 0;
			break;
		} else if (strcmp(operatie, "SAVE") == 0) {
			fgets(param, 50, stdin);
			char *q = strtok(param, "\n ");
			int cnt = 0 ;
			char *output;
			while(q) {
				if(cnt == 0)
					output = q;
				cnt++;
				q = strtok(NULL, "\n ");
			}
			if(cnt == 1) {
				if(tip == 2) {
					FILE* out = fopen(output, "wb");
					P36out_bin(&img, out);
					fclose(out);
					printf("Saved %s\n", output);
				} else if(tip == 1) {
					FILE* out = fopen(output, "wb");
					P25out_bin(&imag, out);
					fclose(out);
					printf("Saved %s\n", output);
				} else {
					printf("No image loaded\n");
				}
			} else {
				if(tip == 2) {
					FILE* out = fopen(output, "wt");
					P36out_ascii(&img, out);
					fclose(out);
					printf("Saved %s\n", output);
				} else if(tip == 1) {
					FILE* out = fopen(output, "wt");
					P25out_ascii(&imag, out);
					printf("Saved %s\n", output);
					fclose(out);
				} else {
					printf("No image loaded\n");
				}
			}
		} else if (strcmp(operatie, "SELECT") == 0) {
			fgets(param, 50, stdin);
			char *q;
			q = strtok(param, "\n ");
			char strpoz[4][20];
			int cnt = 0, ok = 1;
			while (q) {
				strcpy(strpoz[cnt], q);
				cnt++;
				int lungime = strlen(q);
				for (int i = 0; i < lungime; i++)
					if (!(q[i] >= '0' && q[i] <= '9') &&  !(i == 0 && q[i] == '-')) {
						ok = 0;
						break;
					}
				q = strtok(NULL, "\n ");
			}
			if ((cnt != 4 || ok == 0) && cnt != 1)//nr invalid de param
				printf("Invalid command\n");
			else if (cnt == 4) {//exact 4 param.
				if (tip == 2) {//imagine RGB
					int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
					l = strlen(strpoz[0]);
					for (int i = 0; i < l; i++)
						x1 = x1 * 10 + strpoz[0][i] - '0';//variabila de selectie x1
					l = strlen(strpoz[1]);
					for (int i = 0; i < l; i++)
						y1 = y1 * 10 + strpoz[1][i] - '0';// variabila de selectie y1
					l = strlen(strpoz[2]);
					for (int i = 0; i < l; i++)
						x2 = x2 * 10 + strpoz[2][i] - '0';//variabila de selectie x2
					l = strlen(strpoz[3]);
					for (int i = 0; i < l; i++)
						y2 = y2 * 10 + strpoz[3][i] - '0';//variabila de selectie y2
					if (x2 < x1) {
						int aux = x2;
						x2 = x1;
						x1 = aux;
					}
					if (y2 < y1) {
						int aux = y2;
						y2 = y1;
						y1 = aux;
					}
					if (!(x1 < 0 || x2 > img.n || y1 < 0 || y2 > img.m || x1 == x2 || y1 == y2)) {
						img.x1 = x1;
						img.x2 = x2;
						img.y1 = y1;
						img.y2 = y2;
						printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
					} else {
						printf("Invalid set of coordinates\n");
					}
				} else if (tip == 1) {//imagine Grayscale
					int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
					l = strlen(strpoz[0]);
					for (int i = 0; i < l; i++)
						x1 = x1 * 10 + strpoz[0][i] - '0';
					l = strlen(strpoz[1]);
					for (int i = 0; i < l; i++)
						y1 = y1 * 10 + strpoz[1][i] - '0';
					l = strlen(strpoz[2]);
					for (int i = 0; i < l; i++)
						x2 = x2 * 10 + strpoz[2][i] - '0';
					l = strlen(strpoz[3]);
					for (int i = 0; i < l; i++)
						y2 = y2 * 10 + strpoz[3][i] - '0';
					if (x2 < x1) {
						int aux = x2;
						x2 = x1;
						x1 = aux;
					}
					if (y2 < y1) {
						int aux = y2;
						y2 = y1;
						y1 = aux;
					}
					if (!(x1 < 0 || x2 > imag.n || y1 < 0 || y2 > imag.m || x1 == x2 || y1 == y2)) {
						imag.x1 = x1;
						imag.x2 = x2;
						imag.y1 = y1;
						imag.y2 = y2;
						printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
					} else {
						printf("Invalid set of coordinates\n");
					}
				} else {
					printf("No image loaded\n");
				}
			} else if (cnt == 1) {
				if (strcmp(strpoz[0], "ALL") == 0) {//se selecteaza toata imaginea
					if (tip == 2) {
						img.y1 = 0;
						img.x1 = 0;
						img.x2 = img.n;
						img.y2 = img.m;
						printf("Selected ALL\n");
					} else if (tip == 1) {
						imag.y1 = 0;
						imag.x1 = 0;
						imag.x2 = imag.n;
						imag.y2 = imag.m;
						printf("Selected ALL\n");
					} else {
						printf("No image loaded\n");
					}
				} else {
					printf("Invalid command\n");
				}
			}
		} else if (strcmp (operatie, "CROP") == 0) {
			if(tip == 2) {//imagine RGB
				RGB **cop_mat = re_aloc_rgb(img.y2 - img.y1, img.x2 - img.x1);
				//copiem matricea de pixeli din selectia precizata
				for(int i = img.y1; i < img.y2; i++) {
					for(int j = img.x1; j < img.x2; j++) {
						cop_mat[i - img.y1][j - img.x1].r = img.mat[i][j].r;
						cop_mat[i - img.y1][j - img.x1].g = img.mat[i][j].g;
						cop_mat[i - img.y1][j - img.x1].b = img.mat[i][j].b;
					}
				}
				//eliberam matricea initiala
				for (int i = 0; i < img.m; i++)
					free(img.mat[i]);
				free(img.mat);
				//realocam matricea initiala
				img.mat = re_aloc_rgb(img.y2 - img.y1, img.x2 - img.x1);
				img.n = img.x2 - img.x1;
				img.m = img.y2 - img.y1;
				img.y1 = 0;
				img.y2 = img.m;
				img.x1 = 0;
				img.x2 = img.n;
				for (int i = 0; i < img.m; i++) {
					for(int j = 0; j < img.n; j++) {
						img.mat[i][j].r = cop_mat[i][j].r;
						img.mat[i][j].g = cop_mat[i][j].g;
						img.mat[i][j].b = cop_mat[i][j].b;
					}
				}
				//eliberare copie matrice
				for (int i = 0; i < img.m; i++)
					free(cop_mat[i]);
				free(cop_mat);
				printf("Image cropped\n");
			} else if (tip == 1) {//imagine Grayscale
				int **cop_mat = re_aloc_gs(imag.y2 - imag.y1, imag.x2 - imag.x1);
				//copie matrice de pixeli
				for(int i = imag.y1; i < imag.y2; i++) {
					for(int j = imag.x1; j < imag.x2; j++) {
						cop_mat[i - imag.y1][j - imag.x1] = imag.mat[i][j];
					}
				}
				//eliberare matrice initiala
				for (int i = 0; i < imag.m; i++)
					free(imag.mat[i]);
				free(imag.mat);
				//realocare matrice initiala
				imag.mat = re_aloc_gs(imag.y2 - imag.y1, imag.x2 - imag.x1);
				imag.n = imag.x2 - imag.x1;
				imag.m = imag.y2 - imag.y1;
				imag.y1 = 0;
				imag.x2 = imag.n;
				imag.x1 = 0;
				imag.y2 = imag.m;
				for (int i = 0; i < imag.m; i++) {
					for(int j = 0; j < imag.n; j++) {
						imag.mat[i][j] = cop_mat[i][j];
					}
				}
				for (int i = 0; i < imag.m; i++)
					free(cop_mat[i]);
				free(cop_mat);
				printf("Image cropped\n");
			} else {
				printf("No image loaded\n");
			}
		} else if (strcmp(operatie, "APPLY") == 0) {
			fgets(param, 50, stdin);
			if (tip == 1) {//imagine Grayscale
				char *q = strtok(param, "\n ");
				int cnt = 0;
				while (q) { 
					cnt++;
					q = strtok(NULL, "\n ");
				}
				if (cnt != 1) {
					printf("Invalid command\n");
				} else {
					printf("Easy, Charlie Chaplin\n");
				}
			} else if (tip == 2) { //imagine RGB
				char *q = strtok(param, "\n ");
				int numara = 0;
				while (q) {
					numara++;
					q = strtok(NULL, "\n ");
				}
				if (numara != 1) {
					printf("Invalid command\n");
				} else {
					char aux[50];
					strcpy(aux, param + 1);
					strcpy(param, aux);
					if (strcmp(param, "EDGE") == 0) {
							EDGE_rgb(&img);
							printf("APPLY EDGE done\n");
					} else if (strcmp(param, "SHARPEN") == 0) {
							sharpen_rgb(&img);
							printf("APPLY SHARPEN done\n");
					} else if(strcmp(param, "BLUR") == 0) {
							blur_rgb(&img);
							printf("APPLY BLUR done\n");
					} else if(strcmp(param, "GAUSSIAN_BLUR") == 0) {
							g_blur_rgb(&img);
							printf("APPLY GAUSSIAN_BLUR done\n");
					} else {
						printf("APPLY parameter invalid\n");
					}
				}
			} else {
				printf("No image loaded\n");
			}
		} else if(strcmp(operatie, "ROTATE") == 0) {
			int grade, nr;
			
			scanf("%d", &grade);
			if (tip == 0) {
				printf("No image loaded\n");
				continue;
			}
			int copie = grade;
			if (grade < 0) {
				if ((-grade) % 90)
					printf("Unsupported rotation angle\n");
				else {
					if (grade < -360) {
						grade = (-grade) % 360;
						grade = -grade;
					}
					grade = 360 + grade;
					
					int ok = 1;
					for (int i = 0; i < grade / 90; i++) {
						if (tip == 1) {
							//printf("lin: %d %d %d col: %d %d %d\n", imag.y1, imag.y2, imag.m, imag.x1, imag.x2, imag.n);
							if (imag.x2 == imag.n && imag.x1 == 0 && imag.y2 == imag.m && imag.y1 == 0)
								rotate_full_G(&imag);
							else if (imag.x2 - imag.x1 == imag.y2 - imag.y1)
								rotate_right_G(&imag);
							else {
								printf("The selection must be square\n");
								ok = 0;
								break;
							}
						}
						else {
							
							if (img.x2 == img.n && img.x1 == 0 && img.y2 == img.m && img.y1 == 0)
								rotate_full_R(&img);
							else if (img.x2 - img.x1 == img.y2 - img.y1)
								rotate_right_R(&img);
							else {
								printf("The selection must be square\n");
								ok = 0;
								break;
							}
						}
					}
					if (ok == 1)
						printf("Rotated %d\n", copie);
				}
			} else {
				if (grade % 90) {
					printf("Unsupported rotation angle\n");
				} else {
					if (grade > 360)
						grade = grade % 360;
					int ok = 1;
					for (int i = 0; i < grade / 90; i++) {
						if (tip == 1) {
							if (imag.x2 == imag.n && imag.x1 == 0 && imag.y2 == imag.m && imag.y1 == 0)
								rotate_full_G(&imag);
							else if (imag.x2 - imag.x1 == imag.y2 - imag.y1)
								rotate_right_G(&imag);
							else {
								printf("The selection must be square\n");
								ok = 0;
								break;
							}
						}
						else {
							if (img.x2 == img.n && img.x1 == 0 && img.y2 == img.m && img.y1 == 0)
								rotate_full_R(&img);
							else if (img.x2 - img.x1 == img.y2 - img.y1)
								rotate_right_R(&img);
							else {
								printf("The selection must be square\n");
								ok = 0;
								break;
							}
						}
					}
					if (ok == 1)
						printf("Rotated %d\n", copie);
				}
				
			}
		} else if (strcmp(operatie, "HISTOGRAM") == 0) {
			int x = 0, val = 0, numara = 0;
			char strpoz[5][50];
			int ok = 1;
			fgets(param, 50, stdin);
			if (tip == 1) {
				char *q = strtok(param, "\n ");
				while (q) {
					strcpy(strpoz[numara], q);
					l = strlen(q);
					for (int i = 0; i < l; i++)
						if (q[i] < '0' || q[i] > '9')
							ok = 0;
					numara++;
					q = strtok(NULL, "\n ");
				}
				if (ok == 1 && numara == 2) {//verificam daca parametrii histogramei sunt exact 2
					l = strlen(strpoz[0]);
					for (int i = 0; i < l; i++)
						x = x * 10 + strpoz[0][i] - '0';
					l = strlen(strpoz[1]);
					for (int i = 0; i < l; i++)
						val = val * 10 + strpoz[1][i] - '0';
					ok = 1;
					int cp = val;
					while(cp > 1) {
						if(cp % 2 != 0) {
							ok = 0;
							break;
						}
						cp/=2;
					}
					if (ok == 1) {
						int *frecv;
						frecv = (int *)calloc(256, sizeof(int));
						if(!frecv) {
							printf("Malloc failed");
							return 1;
						}
						if(strcmp(imag.magick, "P2") == 0 || strcmp(imag.magick, "P5") == 0) {
							for(int i = 0; i < imag.m; i++) {
								for(int j = 0; j < imag.n; j++) {
									frecv[imag.mat[i][j]]++;
								}
							}
							int maxim = -1;
							int nr = val;
							int lung = 256 / val;
							int start = 0;
							int stop = lung - 1;
							while(nr) {
								int s = 0;
								for(int i = start; i <= stop; i++) {
									s += frecv[i];
								}
								if (s > maxim)
									maxim = s;
								start += lung;
								stop += lung;
								nr--;
							}
							nr = val;
							lung = 256 / val;
							start = 0;
							stop = lung - 1;
							while(nr) {
								int s = 0;
								for(int i = start; i <= stop; i++) {
									s += frecv[i];
								}
									int stelute = (s *x) / maxim ;
									printf("%d\t|\t", stelute);
									while(stelute) {
										printf("*");
										stelute--;
									}
									printf("\n");
								start += lung;
								stop += lung;
								nr--;
							}
						}
						free(frecv);
					} else {
						printf("Invalid set of parameters\n");
					}
				} else {
					printf("Invalid command\n");
				}
			} else if (tip == 2) {
				char *q = strtok(param, "\n ");
				while (q) {
					strcpy(strpoz[numara], q);
					l = strlen(q);
					for (int i = 0; i < l; i++)
						if (q[i] < '0' || q[i] > '9')
							ok = 0;
					numara++;
					q = strtok(NULL, "\n "   );
				}
				if (ok == 1 && numara == 2)
					printf("Black and white image needed\n");
				else
					printf("Invalid command\n");
			} else {
				printf("No image loaded\n");
			}
		} else if (strcmp(operatie, "EQUALIZE") == 0) {
			int *frecv;
			frecv = (int *)calloc(256, sizeof(int));
			if(!frecv) {
				printf("Malloc failed");
				return 1;
			}
			if(tip == 1) {
				for(int i = 0; i < imag.m; i++) {
					for(int j = 0; j < imag.n; j++) {
						frecv[imag.mat[i][j]]++;//vector de frecveta 
					}
				}
				int s = 0, rezolutie = imag.n * imag.m;
				for(int i = 0; i <imag.m; i++) {
					for(int j = 0; j < imag.n; j++) {
						s = 0;
						for(int z = 0; z <= imag.mat[i][j]; z++) {//vector de frecventa pe un interval
							s = s + frecv[z];
						}
						double nr;
						nr = (255 * s) / rezolutie;
						imag.mat[i][j] = (int) nr;
					}
				}
				printf("Equalize done\n");
			} else if (tip == 2) {
				printf("Black and white image needed\n");
			} else {
				printf("No image loaded\n");
			}
			free(frecv);
		} else {
			printf("Invalid command\n");
			fgets(param, 50, stdin);
		}
		
	}
	free(s);
	free(img.magick);
	free(imag.magick);
	free(param);
	return 0;
}