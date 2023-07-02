#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<string.h>
#include <locale.h>
#include<malloc.h>
#include <windows.h>
#define MAX_DEKOD_SIZE 800000
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define TRUE 1
#define FALSE 0
#define M 100000

size_t kolnull = 0;
//int N = 3;
int N = 0;
size_t kolbute = 0;
char endcode[19] = { '0','0','0','0', '0','0','0','0','0','0','0','0','0','0','0','0','0','0','\0'};
int kollogik;
int kolbite;
//unsigned char* dekodstr = (unsigned char*)malloc(sizeof(unsigned char) * MAX_DEKOD_SIZE);
unsigned char dekodstr[MAX_DEKOD_SIZE];
unsigned char Code[MAX_DEKOD_SIZE];
unsigned char str[MAX_DEKOD_SIZE];

typedef struct LZW {
    char simvols[1000];
    char code[19];
}LZW;

LZW TABL[M];

void plusodin(void)
{

    size_t kol = 18;
    
    if (endcode[kol - 1] == '1')
    {
        int i = 1;
        while (endcode[kol - i] != '0')
        {
            endcode[kol - i] = '0';
            i++;
        }
        endcode[kol - i] = '1';
    }
    else
        endcode[kol - 1] = '1';
}

int A(LZW* Struct, unsigned char* S, int* n)
{
    //int k = 0;
    for (int i = 0; i < N; i++)
    {        
        if (strcmp((const char*)S, Struct[i].simvols)==0)
        {
            *n = i;
            return TRUE;
        }       
    }
    *n = N;
    return FALSE;
}


void sravneie(LZW* Struct)
{
    int i = 0;
    int n = 0;
    int flag = FALSE;
    unsigned char S[1000];
    memset(S, 0, 1000);
    int S_counter = 0;
   // int Code_counter = 0;
    while (str[i] != '\0')
    {
        S[S_counter] = str[i];                                    //Читает очередной символ строки
        int a = A(Struct, S, &n);
        int j = 1;
        while (a)
        {
            if (str[i + j] == '\0')
            {
                flag = TRUE;
                break;
            }
            S[++S_counter] = str[i + j];
            a = A(Struct, S, &n);
            j++;
        }
        if (flag)
        {
            strcat(( char*)Code, Struct[n].code);
            strcpy(Struct[N].simvols, (const char*)S);
            strcpy(Struct[N].code, endcode);
            break;
        }
        N++;
        strcpy(Struct[N - 1].simvols,(const char*) S);
        strcpy(Struct[N - 1].code, endcode);
        plusodin();
        //Dop(Struct, S);
       
        strcat(( char*)Code, Struct[n].code);

         
         int prirostI = (int)strlen(Struct[n].simvols);
        i += prirostI;
        S_counter=0;
        memset(S, 0, 1000);
    }
}

void reduce_char(unsigned char* dekod)
{

    unsigned char* newdekod = (unsigned char*)malloc(sizeof(unsigned char) * (kollogik + 1));
    if (newdekod == NULL)
    {
        return;
    }
    memcpy(newdekod, dekod, kollogik);
    dekod = newdekod;
    free(dekod);
}

void delsimvols(unsigned char* dekod, int kolsovp)
{
    int n = 0;
    for (int i = 0, j = kolsovp; j < kollogik; i++, j++)
    {
        dekod[i] = dekod[j];
        if (j == kollogik)
        {
            dekod[i + 1] = '\0';
        }
        n++;
    }
    //reduce_char(dekod, n);
    kollogik = n;
}

void decoding(LZW* TABLE, unsigned char* dekod)
{
    int SymbolCounter = 0;
    for (int i = 0; i < N; i++)
    {
        int j = 0;
        int kolsovp = 0;
        while (TABLE[i].code[j] != '\0')
        {
            if (TABLE[i].code[j] == dekod[j])
            {
                kolsovp++;
            }
            j++;
        }
        if (kolsovp == j)
        {
            int k = 0;
            while (TABLE[i].simvols[k] != '\0')
            {
                dekodstr[SymbolCounter++] = TABLE[i].simvols[k++];                
            }
          
            delsimvols(dekod, kolsovp);
            if (kollogik<50)
            {
                printf("");
            }
           
            i = -1;
        }
        if (kollogik == 1) {
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
    int k = (int)strlen(t) - 2;

    int m = k / 2;
    for (int counter = 0; counter <= m; counter++)
    {
        char tmp = t[counter];
        t[counter] = t[k - counter + 1];
        t[k - counter + 1] = tmp;
    }
    t[i] = '\0';
    return t;

}

int AddFirstNull(char* BinaryMass, int kolNull) {
    char NewBinaryMass[9] = { 0 };
    for (int i = 0; i < 8; i++)
    {
        NewBinaryMass[i] = '0';
    }
    NewBinaryMass[8] = '\0';
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

int main()
{
    setlocale(LC_ALL, "rus");    
    FILE* byte;
    FILE* key_1;
    FILE* key_2;
    int action;
    char InputText_1[50];
    printf("Choose an action\n");
    printf("1-Encode file\n2.Decode file\n3.Exit\n");
    if (scanf("%d",&action) != 1)
    {
        return EXIT_FAILURE;
    }
        if (action == 1)
        {          
            printf("Input file is:\n");
            scanf("%s", InputText_1);
            FILE* fin;
            fin = fopen(InputText_1, "rb");
            if (fin == NULL)
            {
                return EXIT_FAILURE;
            }
            //size_t str_counter = 0;
            memset(str, 0, MAX_DEKOD_SIZE);
            fseek(fin, 0, SEEK_END);
            int str_counter = ftell(fin);
            fseek(fin, 0, SEEK_SET);
            fread(str, (size_t)sizeof(unsigned char), str_counter, fin);
            //while ((ch = fgetc(fin)) != EOF)
            //{
            //    str[str_counter++] = ch;                 /////////////////////////
            //}
            int  kol = 0;
            while (str[kol++] != '\0');
            LARGE_INTEGER freq, som, som1;
                                                                                                                                                                                                                                                                                        
            QueryPerformanceFrequency(&freq);
            QueryPerformanceCounter(&som);
            memset(Code, 0, MAX_DEKOD_SIZE);   
            sravneie(TABL);
          
            key_1 = fopen("keys_s.txt", "wb");
            int counter = 0;
            while (TABL[counter].simvols[0] != 0 )
            {
                                
                fputs(TABL[counter].simvols, key_1);                
                counter++;
                fprintf(key_1, "~");
            }
            fclose(key_1);
            int cntr = 0;
            key_2 = fopen("keys_c.txt", "wb");
            while (cntr < counter)
            {                
               fputs(TABL[cntr].code, key_2);
               fprintf(key_2, " ");
               cntr++;
            }
            fclose(key_2);
            kollogik = 0;
           
            while (Code[kollogik++] != '\0');
            
            kolnull = 8 - kollogik % 8;
            for (int i = kollogik; i < kollogik + (int)kolnull; i++)
            {
                Code[i] = '0';
            }
            
            kolbite = (kollogik + (int)kolnull) / 8;
             byte= fopen("kolbyte.txt", "wb");
             if (byte == NULL)
             {
                 return EXIT_FAILURE;
             }
            fprintf(byte, "%d %d", kolbite,kollogik);
            fclose(byte);
            unsigned char* bites = (unsigned char*)malloc(sizeof(unsigned char) * kolbite);
            if (bites == NULL)
            {
                return EXIT_FAILURE;
            }

            for (int i = 0; i < kolbite; i++)
            {
                unsigned char bite = 0;
                unsigned char edenichka = 1;
                for (int j = i * 8 + 7; j >= i * 8; j--)
                {
                    if (Code[j] == '1')
                    {
                        bite = bite | edenichka;
                    }
                    edenichka = edenichka << 1;
                }
                bites[i] = bite;
            }

            //записать bites в файл
            float CompressionRatio = (float)kol / (float)kolbite;
            printf("\nCompression ratio = %.3f\n", CompressionRatio);
            QueryPerformanceCounter(&som1);
            double elapsedTime = (double)(som1.QuadPart - som.QuadPart) / freq.QuadPart;
            printf("\nEncode time: %f\n", elapsedTime);
            FILE* fout;
            char OutputDecode_1[50];
            printf("\n\n");
            printf("Enter the name of the file where you want to write the encoded text\n");
            scanf("%s", OutputDecode_1);
            fout = fopen(OutputDecode_1, "wb");
           

            for (int i = 0; i < kolbite; i++) {

                //fprintf(fout, "%c", bites[i]);
                fwrite(bites + i, (size_t)sizeof(unsigned char), 1, fout);
            }
           
            fclose(fout); 
        }


        if (action == 2)
        {
           
            LARGE_INTEGER freq, som, som1;
            key_1 = fopen("keys_s.txt", "rb");
            if (key_1 == NULL)
            {
                return EXIT_FAILURE;
            }
            QueryPerformanceFrequency(&freq);
            QueryPerformanceCounter(&som);
             N = 0;
            fseek(key_1, 0, SEEK_END);
            int file_size = ftell(key_1);
            fseek(key_1, 0, SEEK_SET);
            unsigned char ch;
            int simv_count;
            int z = 0;
            //ch = fgetc(key_1);
            while (z < file_size)
            {               
                simv_count = 0;
                while (1)
                {
                    fread(&ch, sizeof(char), 1, key_1);
                    if (ch == '~' && simv_count != 0)
                    {
                        break;
                    }
                    TABL[N].simvols[simv_count++] = ch;
                    z++;
                }
                z++;
                N++;
            }
           //int kolnul = 
            key_2 = fopen("keys_c.txt", "rb");
            if (key_2 == NULL)
            {
                return EXIT_FAILURE;
            }
            fseek(key_2, 0, SEEK_END);
            int size = ftell(key_2);
            fseek(key_2, 0, SEEK_SET);
            int int_count;
            int length = 0;
            N = 0;
            while (length < size)
            {
                int_count = 0;
                while ((ch = (unsigned char)fgetc(key_2)) != ' ')
                {
                    //  fseek(key_1, -1, SEEK_CUR);
                    TABL[N].code[int_count++] = ch;
                    length++;
                }
                length++;
                N++;
            }
           
            // Декодирование
            FILE* fin;
            char DecodeFile_2[50];
            printf("enter the name of the encoded file\n");
            scanf("%s", DecodeFile_2);
            fin = fopen(DecodeFile_2, "rb");
            if (fin == NULL)
            {
                return EXIT_FAILURE;
            }
            
            byte = fopen("kolbyte.txt", "rb");
            if (byte == NULL)
            {
                return -1;
            }
            fscanf(byte, "%d %d", &kolbite,&kollogik);
            int kolbits = kolbite * 8;
          
            unsigned char* dekod = (unsigned char*)malloc(sizeof(unsigned char) * (kolbits + 1));
            unsigned int* butes = (unsigned int*)malloc(sizeof(unsigned int) * (kolbite + 1));
             //unsigned int butes[100000];

            if (butes == NULL || dekod == NULL)
            {
                return EXIT_FAILURE;
            }
           // memset(dekod, 0, sizeof(dekod));
            int kol = -1;
            int c;
            printf("\n"); 
            //fread(butes, (size_t)sizeof(unsigned int), kolbite, fin);
            while ((c = fgetc(fin)) != EOF && kol < kolbite)
            {
                kol++;
                butes[kol] = c;
            }
            fclose(fin);

            char BinaryMass[9] = { 0 };
            for (int i = 0; i < (int)kolbite; i++)
            {                
                    decToBin(butes[i], BinaryMass);             
                if (strlen(BinaryMass) < 8)
                {
                    AddFirstNull(BinaryMass, 8 - (int)strlen(BinaryMass));          //нужное кол во нулей
                }
                for (int j = i * 8, k = 0; j < i * 8 + 8; j++, k++)        //k для просмотра binaryMass
                {
                    dekod[j] = BinaryMass[k];
                }
            }
            printf("\n");
            reduce_char(dekod);
           // size_t Sizedekod = kolbits - kolnull;       
            decoding(TABL, dekod );
            char Output_File_2[50];
            printf("Enter name of file which you want to write decoded text\n");
            scanf("%s", Output_File_2);
            FILE* output;
            output = fopen(Output_File_2, "wb");

            if (output == NULL)
            {
                printf("\nFile not found");
                return EXIT_FAILURE;
            }
            int DekodLength = 0;
            DekodLength = (int)strlen((const char*)dekodstr);

            for (int cntr = 0; cntr < DekodLength; cntr++)
            {
                fwrite(dekodstr + cntr, sizeof(unsigned char), 1, output);
            }
            QueryPerformanceCounter(&som1);
            double elapsedTime = (double)(som1.QuadPart - som.QuadPart) / freq.QuadPart;
            printf("\nВремя раскодирования: %f\n", elapsedTime);
            fclose(output); 

        }
        
    return 0;
}