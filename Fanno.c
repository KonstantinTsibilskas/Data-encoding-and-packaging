#define _CRT_SECURE_NO_WARNINGS
#pragma comment (linker,"/STACK:600000000")
#include <stdio.h>
#include <locale.h>
#include <string.h> 
#include<malloc.h>
#include <Windows.h>
#define N 1000
#define MAX_DEKOD_SIZE 100000
#define MAX_INPUT_FILE_SIZE 150000
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
size_t kolnull = 0; //Количество добавленных нулей
int n = 0; //Размер структуры
size_t kolbute = 0;

unsigned char dekodstr[MAX_DEKOD_SIZE];
unsigned char kode[MAX_DEKOD_SIZE];
unsigned char str[MAX_DEKOD_SIZE];
unsigned char srtcopy[MAX_DEKOD_SIZE];
// Структура содержит уникальный символ их уол-во в тексте и код 
typedef struct SKK {
    unsigned char simvol;
    int kol;
    char kod[500];
}SKK;

SKK simvolkol[N];

void SwapSKK(SKK* x, SKK* y)
{
    SKK z = *x;
    *x = *y;
    *y = z;
}

void SortSKK(SKK* Struct, int n)
{
    for (int j = 0; j < n; j++)
    {
        for (int i = 0; i < n - 1; i++)
        {
            if (Struct[i].kol > Struct[i + 1].kol)
            {
                SwapSKK(&Struct[i], &Struct[i + 1]);
            }
        }
    }
}

unsigned long int Sum(SKK* encodings, int beg, int end)
{
    unsigned long int sum = 0;
    for (int i = beg; i < end; i++)
    {
        sum += encodings[i].kol;
    }
    return sum;
}

int index(SKK* encodings, int beg, int end)
{
    unsigned long int sum = Sum(encodings, beg, end);
    unsigned long int sumend = 0;
    int i = beg;
    while (sumend < sum / 2 && i < end - 1)
    {
        if (sumend > 9000) {
            printf("otladka");
        }
        sumend = sumend + encodings[i].kol;
        i++;
    }
    return i;
}

void kodirovanie0(char* kod)
{
    int j = 0;
    while (kod[j] == '0' || kod[j] == '1') {
        j++;
    }
    kod[j] = '0';
}

void kodirovanie1(char* kod)
{
    int j = 0;
    while (kod[j] == '0' || kod[j] == '1') {
        j++;

    }
    kod[j] = '1';
}

void multikod(SKK* encodings, int beg, int end)
{
    int ind = index(encodings, beg, end);
    if ((beg == end - 1)/* || ind == end*/)           // бесконечный цикл
    {
        return;
    }
    if (beg == end - 2)
    {

        kodirovanie1(encodings[end - 1].kod);
        kodirovanie0(encodings[beg].kod);
        return;
    }

    for (int i = beg; i < ind; i++)
    {
        kodirovanie1(encodings[i].kod);
    }

    for (int j = ind; j < end; j++)
    {
        kodirovanie0(encodings[j].kod);
    }

    multikod(encodings, beg, ind);
    multikod(encodings, ind, end);
}

int coding(unsigned char* str, SKK* encodings, unsigned char* code, int n, size_t kolstr)
{
    int i = 0;
    int kod_counter = 0;
    while (i != (int)kolstr)
    {
        for (int j = 0; j < n; j++)
        {
            if (str[i] == encodings[j].simvol)
            {
                int k = 0;
                while (encodings[j].kod[k] != '\0')
                {
                    code[kod_counter++] = encodings[j].kod[k];
                    k++;
                }
            }
        }
        i++;
    }
    return kod_counter;
}

int dopnull(unsigned char* Code, size_t kolnull, int kollogik)
{
    for (size_t i = kollogik; i < kollogik + kolnull; i++)
    {
       
        Code[i] = '0';
    }
    size_t kolbute = (kollogik + kolnull) / 8;
    return kolbute;
}

void reduce_char(unsigned char* dekod, size_t kolbits)
{
    for (int i = (int)kolbits - kolnull; i < (int)kolbits; i++)
    {
        dekod[i] = 0;
    }
}

void delsimvols(unsigned char* dekod, int* kollogik, int kolsovp)
{
    int n = 0;
    for (int i = 0, j = kolsovp; j < *kollogik /*+ 1*/; i++, j++)                 /*-----------------------------------------------*/
    {
        dekod[i] = dekod[j];
        if (j == *kollogik)
        {
            dekod[i] = '\0';
        }
        n++;
    }
    //reduce_char(dekod, n); 
    dekod[n] = '\0';
    *kollogik = n;
}

void decoding(SKK* encodings, unsigned char* dekod, unsigned char* dekodstr, int sizeenkod, size_t kollogik)
{
    int SymbolCounter = 0;
    int k = 0;
    for (int i = 0; i < sizeenkod; i++)
    {
        int j = 0;
        int kolsovp = 0;
        while (encodings[i].kod[j] != '\0')
        {
            if (encodings[i].kod[j] == dekod[j])
            {
                kolsovp++; // Считает количество совпадений
            }
            j++;
        }
        if (kolsovp == j)
        {
            dekodstr[k++] = encodings[i].simvol;
            SymbolCounter++;
            delsimvols(dekod, &kollogik, kolsovp);
            i = -1;
        }
        if (dekod[0] == '\0') {
            dekodstr[SymbolCounter] = '\0';
            break;
        }
    }
}

char* decToBin(int x, char* t)
{
    int i = 0;
    do
    {
        t[i++] = x % 2 + '0';
        x = x / 2;
    } while (x != 0);
    t[i] = '\0';
    // инвертируем t
    int k = (int)strlen(t) - 2;
    // size_t k = strlen(t) - 2;
    int m = k / 2;
    for (int i = 0; i <= m; i++)
    {
        char tmp = t[i];
        t[i] = t[k - i + 1];
        t[k - i + 1] = tmp;
    }
    t[i] = '\0';
    return t;

}

int AddFirstNull(char* BinaryMass, int kolNull) {
    char NewBinaryMass[9] = { 0 };
    memset(NewBinaryMass, '0', 8);
    int  j = 0;
    int i;
    for (i = kolNull; i < 8; i++)
    {
        NewBinaryMass[i] = BinaryMass[j++];
    }
    NewBinaryMass[i] = '\0';
    strcpy(BinaryMass, NewBinaryMass);
    return EXIT_SUCCESS;
}
void create_keys_file(SKK* kode, int action) {
    FILE* key_1;
    FILE* key_2;
    FILE* key_3;
    int counter = 0;
    if (action == 1)
    {
        key_1 = fopen("keys_s.txt", "wb");
        key_2 = fopen("kolbute.txt", "wb");
        key_3 = fopen("kolsymb.txt", "wb");
        if (key_1 == NULL || key_2 == NULL || key_3 == NULL)
        {
            return;
        }
        fprintf(key_2, "%d %d %d", n, kolbute, kolnull);
        while (counter < n)
        {
            fprintf(key_1, "%c %s\n",kode[counter].simvol,kode[counter].kod);
            fprintf(key_3, "%d\n", kode[counter].kol);
            counter++;
        }     
    }
    if (action == 2)
    {
        key_1 = fopen("keys_s.txt", "rb");
        key_2 = fopen("kolbute.txt", "rb");
        key_3 = fopen("kolsymb.txt", "rb");
        if (key_1 == NULL || key_2 == NULL || key_3 == NULL)
        {
            return;
        }
        fseek(key_1, 0, SEEK_END);
        int size = ftell(key_1);
        fseek(key_1, 0, SEEK_SET);
        if ((fscanf(key_2, "%d %d %d", &n, &kolbute, &kolnull)) != 3) {
            return;
        }
     
        int kol_counter = 0;
        int simbol = 0;
        int kol = 0;
        while ( fscanf(key_3, "%d ",&kol) == 1) {
            kode[kol_counter++].kol = kol;
        }
        int length = 0;
        int simv_count = 0;
        int ch;
        int length_of_file = 0;
            while (1)
            {
                simv_count = 0;
                if ((ch = fgetc(key_1)) == -1) {
                    break;
                }
                if (ch != ' ' || (ch == ' ' && kode[length].kol != 0)) {
                    fseek(key_1, 1, SEEK_CUR);
                    kode[length].simvol = ch;
                
                    while ((ch = fgetc(key_1)) != '\n')
                    {
                        kode[length].kod[simv_count++] = ch;/*---------------------------*/
                        length_of_file++;
                    }
                }
                else
                {
                    length--;
                }
                length++;
            }      
    }
}

int main()
{
    setlocale(LC_ALL, "");
    char name[50];
    char nameout[50];
    if (dekodstr == NULL)
    {
        return EXIT_FAILURE;
    }
        printf("Choose an action\n");
        printf("1-Encode file\n2.Decode file\n3.Exit\n");
        int a = 0;
        if (scanf("%d", &a) != 1) { return EXIT_FAILURE; }
        if (a == 1)
        {
            printf("Input file is: \n");
            

            if (scanf("%s", name) != 1) {
                return EXIT_FAILURE;
            }
            LARGE_INTEGER freq, som, som1;

            QueryPerformanceFrequency(&freq);
            QueryPerformanceCounter(&som);
            FILE* fin;
            fin = fopen(name, "rb");
            if (fin == NULL)
            {
                return EXIT_FAILURE;
            }
            //size_t str_counter = 0;f
            fseek(fin, 0, SEEK_END);
            int str_counter = ftell(fin);
            fseek(fin, 0, SEEK_SET);
            fread(str, (size_t)sizeof(char), str_counter, fin);

            fclose(fin);
            memcpy(srtcopy, str, str_counter);
            size_t kolstr = str_counter;
            size_t kolstr1 = kolstr;
            // Поиск и подсчёт уникальных символов и запись их в структуру 

            while ((int)kolstr1 != 0)
            {
                simvolkol[n].simvol = srtcopy[0];
                int counter = 0;
                int j = 0;
                while (j < (int)kolstr1)
                {
                    if (srtcopy[j] == simvolkol[n].simvol)
                    {
                        counter++;
                        for (int k = j; k < (int)kolstr1; k++)
                        {
                            srtcopy[k] = srtcopy[k + 1];
                        }
                        kolstr1--;
                    }
                    else
                    {
                        j++;
                    }
                }
                simvolkol[n].kol = counter;
                n++; // Считает уникальные символы 
            }

            SortSKK(simvolkol, n);
            multikod(simvolkol, 0, n);
            memset(kode, 0, MAX_DEKOD_SIZE);

            int kollogik = coding(str, simvolkol, kode, n, kolstr);
            int encode = 1;
            kolnull = 8 - kollogik % 8;
            
            kolbute = dopnull(kode, kolnull, kollogik);
            create_keys_file(simvolkol, encode);
            unsigned char* butes = (unsigned char*)malloc(sizeof(unsigned char) * kolbute);
            if (butes == NULL)
            {
                return EXIT_FAILURE;
            }

            //Преобразует двоичный код в дeсятичный 
            for (int i = 0; i < (int)kolbute; i++)
            {
                unsigned char bute = 0;
                unsigned char one = 1;
                for (int j = i * 8 + 7; j >= i * 8; j--)
                {
                    if (kode[j] == '1')
                    {
                        bute +=one;
                    }
                    one = one << 1;
                }
                butes[i] = bute;
            }
            QueryPerformanceCounter(&som1);
            double elapsedTime = (double)(som1.QuadPart - som.QuadPart) / freq.QuadPart;
            printf("\nEncode time: %f\n", elapsedTime);
            /*-------------------------------------------------------------------*/

            printf("Enter the name of the file where you want to write the archived text\n");
            if (scanf("%s", nameout) != 1) {
                return EXIT_FAILURE;
            }
            FILE* fout;
            fout = fopen(nameout, "wb");
            if (fout == NULL)
            {
                return EXIT_FAILURE;
            }
            int NameOutLength = 0;
            while (nameout[NameOutLength++] != '\0');
            nameout[NameOutLength] = '\0';
            
          
            printf("\n\n");

            for (int i = 0; i < (int)kolbute; i++) {

                // fprintf(fout, "%c", butes[i]);
                fwrite(butes + i, sizeof(char), 1, fout);
            }
            free(butes);
            float CompressionRatio = (float)kolstr / (float)kolbute;
            printf("Степень сжатия = %.3f\n", CompressionRatio);
            fclose(fout); 
            //free(butes);
        }
        //decod
        if (a == 2) {
            
            char namein[50];
            printf("Input file is: \n");
            if (scanf("%s", namein) != 1) {
                return EXIT_FAILURE;
            }
            FILE* fin; 
            fin = fopen(namein, "rb");
            if (fin == NULL)
            {
                return EXIT_FAILURE;
            }

            LARGE_INTEGER freq, som, som1;
            int decode = 2;
            create_keys_file(simvolkol, decode);
            QueryPerformanceFrequency(&freq);
            QueryPerformanceCounter(&som);
            int LengftForFile = 0;
            while (namein[LengftForFile++] != '\0');
            namein[LengftForFile] = '\0';
            
           
            int kolbits = kolbute * 8;
            unsigned char* dekod = (unsigned char*)malloc(sizeof(unsigned char) * (kolbits + 1)+10);
            unsigned int* Butes = (unsigned int*)malloc(sizeof(unsigned int) * kolbute+10);
            if (dekod == NULL || Butes == NULL)
            {
                return EXIT_FAILURE;
            }
            int ch = 0;
            int i = -1;

            while (1)
            {
                if (i < (int)kolbute)
                {
                    i++;
                    Butes[i] = fgetc(fin);
                }
                else
                {
                    break;
                }
            }

            //fclose(fin);
            memset(dekod, 0, kolbits);
            char BinaryMass[9] = { 0 };
            for (int i = 0; i < (int)kolbute; i++)
            {
               
                decToBin(Butes[i], BinaryMass);
                if (strlen(BinaryMass) < 8)
                {
                    AddFirstNull(BinaryMass, 8 - strlen(BinaryMass));          //нужное кол во нулей
                }
                for (int j = i * 8, k = 0; j < i * 8 + 8; j++, k++)        //k для просмотра binaryMass
                {
                    dekod[j] = BinaryMass[k];
                }
            }

            reduce_char(dekod, kolbits);

            size_t Sizedekod = kolbits - kolnull;

            // Декодирование в текст и вывод на экран 
            QueryPerformanceCounter(&som1);
            /*-------------------------------------------------------------------------------------------------------------------------*/
            printf("\n");
            decoding(simvolkol, dekod, dekodstr, n, Sizedekod);
            double elapsedTime = (double)(som1.QuadPart - som.QuadPart) / freq.QuadPart;
            QueryPerformanceCounter(&som1);
            printf("\nDecod time: %f\n", elapsedTime);
            printf("\nSelect the file where you want to write the decoded text\n");
            FILE* output;
            char nameoutdec[50];
            if (scanf("%s", nameoutdec) != 1) {
                return EXIT_FAILURE;
            }
            int LengthFornameoutdec = 0;
            while (nameoutdec[LengthFornameoutdec++] != '\0');
            nameoutdec[LengthFornameoutdec++] = '\0';
            output = fopen(nameoutdec, "wb");

            if (output == NULL)
            {
                printf("\nFile not found");
                return EXIT_FAILURE;
            }

            int DekodLength = 0;
      
            DekodLength = strlen(dekodstr);
            free(dekod);
            free(Butes);

            for (int i = 0; i < DekodLength; i++)
            {
                fwrite(dekodstr + i, sizeof(unsigned char), 1, output);
                // fprintf(output, "%c", dekodstr[i]);
            }
        }
        return EXIT_SUCCESS;     
}