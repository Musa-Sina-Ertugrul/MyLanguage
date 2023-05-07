#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
int yasakliKarakter(char); // yasakli karakter kontrol fonksiyonu
int readFile(char[]);
int lex(char *);                       // lex analiz fonksiyonu
int atama(char *, int *, int);         // atama fonksiyonu
int egerYada(char *, int *);           // egerYada fonksiyonu
int dongu(char *, int *);              // dongu fonksiyonu
int islem(char *, int *, int);         // islem fonksiyonu
int term(char *, int *, int);          // term fonksiyonu
int factor(char *, int *, int);        // factor fonksiyonu
int nextOperation(char *, int *, int); // nextOperation fonksiyonu
int degiskenKontrolu(char *, int *);   // değişken kontrolü
enum LOOKUP
{
    SON = 0,
    EGER = 1,
    YADA = 2,
    BITIS = 3,
    DOLLAR = 4,
    ARTI = 5,
    EKSI = 6,
    BOLME = 7,
    YILDIZ = 8,
    SOLPARAENTEZ = 9,
    SAGPARANTEZ = 10,
    SORUISARETI = 11,
    SEMICOLON = 12,
    ESITESITTIR = 13,
    IKINOKTA = 14,
    SOLKOSELIPARANTEZ = 15,
    SAGKOSELIPARANTEZ = 16,
    DONGU = 17,
    BUYUKTUR = 18,
    KUCUKTUR = 19
};

char LOOKUPARRAY[20][9] = {
    {'[', 'S', 'O', 'N', ']', '\000', '\000', '\000', '\000'},
    {'[', 'e', 'g', 'e', 'r', ']', '\000', '\000', '\000'},
    {'[', 'y', 'a', 'd', 'a', ']', '\000', '\000', '\000'},
    {'[', 'b', 'i', 't', 'i', 's', ']', '\000', '\000'},
    {'$', '\000', '\000', '\000', '\000', '\000', '\000', '\000', '\000'},
    {'+', '\000', '\000', '\000', '\000', '\000', '\000', '\000', '\000'},
    {'-', '\000', '\000', '\000', '\000', '\000', '\000', '\000', '\000'},
    {'/', '\000', '\000', '\000', '\000', '\000', '\000', '\000', '\000'},
    {'*', '\000', '\000', '\000', '\000', '\000', '\000', '\000', '\000'},
    {'(', '\000', '\000', '\000', '\000', '\000', '\000', '\000', '\000'},
    {')', '\000', '\000', '\000', '\000', '\000', '\000', '\000', '\000'},
    {'=', '\000', '\000', '\000', '\000', '\000', '\000', '\000', '\000'},
    {';', '\000', '\000', '\000', '\000', '\000', '\000', '\000', '\000'},
    {'?', '\000', '\000', '\000', '\000', '\000', '\000', '\000', '\000'},
    {':', '\000', '\000', '\000', '\000', '\000', '\000', '\000', '\000'},
    {'[', '\000', '\000', '\000', '\000', '\000', '\000', '\000', '\000'},
    {']', '\000', '\000', '\000', '\000', '\000', '\000', '\000', '\000'},
    {'[', 'd', 'o', 'n', 'g', 'u', ']', '\000', '\000'},
    {'>', '\000', '\000', '\000', '\000', '\000', '\000', '\000', '\000'},
    {'<', '\000', '\000', '\000', '\000', '\000', '\000', '\000', '\000'}};

char VARIABLEARRAY[5][10];
int variableCount = 0;

int main()
{
    int result = readFile("test1.txt");
    return result;
};

int readFile(char file[])
{
    char c;
    FILE *fp;
    fp = fopen(file, "r");
    if (fp == NULL)
    {
        printf("Dosya açilamadi");
        return -1;
    }
    char currentLine[100];
    for (int i = 0; i < 100; i++)
    {
        currentLine[i] = '\000';
    }
    while ((c = fgetc(fp)) != EOF)
    {
        int error = yasakliKarakter(c);
        if (error == -1)
        {
            printf("syntax hatasi");
            return -1;
        }
        int index = 0;
        while (c != ';' && c != EOF)
        {
            if (c != '\n')
            {
                currentLine[index] = c;
                index++;
            }
            c = fgetc(fp);
        }
        index = 0;
        if (lex(currentLine) == -1)
        {
            printf("syntax hatasi");
            return -1;
        }
    }
    fclose(fp);
    return 0;
};

int yasakliKarakter(char c)
{
    switch (c)
    {
    case '.':
        printf("syntax hatasi .");
        return -1;
        break;
    case ',':
        printf("syntax hatasi ,");
        return -1;
        break;
    case '^':
        printf("syntax hatasi ^");
        return -1;
        break;
    case '%':
        printf("syntax hatasi modula");
        return -1;
        break;
    case '_':
        printf("syntax hatasi -");
        return -1;
        break;
    case '!':
        printf("syntax hatasi !");
        return -1;
        break;
    case '&':
        printf("syntax hatasi &");
        return -1;
        break;
    case '|':
        printf("syntax hatasi |");
        return -1;
        break;
    case '{':
        printf("syntax hatasi {");
        return -1;
        break;
    case '}':
        printf("syntax hatasi }");
        return -1;
        break;
    case '"':
        printf("syntax hatasi çift tirnak");
        return -1;
        break;
    }
    return 0;
}

int lex(char *line)
{
    int index = 0;
    char c;
    char currentVariable[10];
    c = line[index];
    while (index < 100 && c != ';')
    {
        if (yasakliKarakter(c) == -1)
        {
            printf("syntax hatasi");
            return -1;
        }
        if (c == '$')
        {
            int tmpIndex = 0;
            while (c != ' ' && c != ';')
            {
                if (tmpIndex == 10)
                {
                    printf("syntax hatasi uzun değişken ismi");
                    return -1;
                }
                if (c < 'a' && c > 'z')
                {
                    printf("syntax hatasi değişken ismi küçük harf kullan");
                    return -1;
                }
                currentVariable[index] = c;
                index++;
                tmpIndex++;
                c = line[index];
            }
            for (int i = 0; i < 10; i++)
            {
                VARIABLEARRAY[variableCount][i] = currentVariable[i];
            }
            variableCount++;
            if (variableCount == 5)
            {
                return -1;
            }
        }
        else if (c == '=')
        {
            index++;
            c = line[index];
            while (c == '\000')
            {
                index++;
                c = line[index];
            }
            if (c != ' ')
            {
                printf("syntax hatasi = den sonra boşluk olmali");
                return -1;
            }
            index++;
            return atama(line, &index, 99);
        }
        else if (c == '[')
        {
            char tmp[9];
            for (int i = 0; i < 9; i++)
            {
                tmp[i] = '\000';
            }
            int tmpIndex = 0;
            while (c != ']')
            {
                if (tmpIndex == 9)
                {
                    printf("syntax hatasi uzun keyword");
                    return -1;
                }
                if (c != '\000')
                {
                    tmp[tmpIndex] = c;
                    tmpIndex++;
                }
                index++;
                c = line[index];
            }
            tmp[tmpIndex] = c;
            tmpIndex++;
            int tmpCheck = -1;
            for (int i = 0; i < 20; i++)
            {
                int check = 0;
                for (int j = 0; j < 9; j++)
                {
                    if (tmp[j] == LOOKUPARRAY[i][j])
                    {
                        check++;
                    }
                }
                if (check == 9)
                {
                    tmpCheck = i;
                    break;
                }
            }
            if (tmpCheck == -1)
            {
                printf("syntax hatasi böyle bir keyword yok");
                return -1;
            }
            switch (tmpCheck)
            {
            case 1:
                return egerYada(line, &index);
                break;
            case 17:
                return dongu(line, &index);
                break;
            default:
                printf("syntax hatasi bilinmeyen keyword");
                return -1;
                break;
            }
        }

        while (c == ' ' || c == '\000')
        {
            index++;
            c = line[index];
        }
    }
};

int atama(char *line, int *index, int lastIndex)
{
    if (lastIndex < *index)
    {
        return 0;
    }
    // atama işlemi
    char c = line[99];
    while ((line[*index] == '\000' || line[*index] == ' ') && *index < 100)
    {
        (*index)++;
    }
    if (*index == 100)
    {
        printf("syntax hatasi atama işlemi yapilamadi");
        return -1;
    }
    if (line[*index] != '(')
    {
        printf("syntax hatasi atama işlemi yapilamadi");
        return -1;
    }
    int tmpIndex = lastIndex;
    if (line[*index] == '(')
    {
        while (tmpIndex > 5)
        {
            if (yasakliKarakter(c) == -1)
            {
                printf("syntax hatasi");
                return -1;
            }
            if (c == ')')
            {
                return islem(line, index, tmpIndex);
                break;
            }
            c = line[tmpIndex];
            tmpIndex--;
        }
        if (tmpIndex == 5)
        {
            printf("syntax hatasi ) yok");
            return -1;
        }
    }
    else
    {
        c = line[*index];
        while (c >= '0' && c <= '9')
        {
            (*index)++;
            c = line[*index];
        }
        if ((c >= 'a' && c <= 'z') || yasakliKarakter(c) == -1)
        {
            printf("syntax hatasi");
            return -1;
        }
    }
};

int egerYada(char *line, int *index)
{
    // egerYada işlemi
    (*index)++;
    while (line[*index] == ' ' || line[*index] == '\000')
    {
        (*index)++;
    }
    if (*index == 100)
    {
        printf("syntax hatasi uzun satir");
        return -1;
    }
    if (line[*index] == '$')
    {
        if (degiskenKontrolu(line, index) == -1)
        {
            return -1;
        }
    }
    else if (line[*index] >= '0' && line[*index] <= '9')
    {
        while (line[*index] >= '0' && line[*index] <= '9')
        {
            (*index)++;
        }
    }
    while (line[*index] == ' ' || line[*index] == '\000')
    {
        (*index)++;
    }
    if (*index >= 100)
    {
        printf("syntax hatasi uzun satir");
        return -1;
    }
    if (line[*index] != '?' && line[*index] != '<' && line[*index] != '>')
    {
        printf("syntax hatasi bilinmeyen operatör veya yanliş operatör");
        return -1;
    }
    (*index)++;
    while (line[*index] == ' ' || line[*index] == '\000')
    {
        (*index)++;
    }
    if (*index >= 100)
    {
        printf("syntax hatasi uzun satir");
        return -1;
    }
    if (line[*index] == '$')
    {
        if (degiskenKontrolu(line, index) == -1)
        {
            return -1;
        }
    }
    else if (line[*index] >= '0' && line[*index] <= '9')
    {
        while (line[*index] >= '0' && line[*index] <= '9')
        {
            (*index)++;
        }
    }
    while (line[*index] == ' ' || line[*index] == '\000')
    {
        (*index)++;
    }
    if (*index >= 100)
    {
        printf("syntax hatasi uzun satir");
        return -1;
    }
    if (line[*index] != ':')
    {
        printf("syntax hatasi eger tamamlanmamiş : eksik");
        return -1;
    }
    (*index)++;
    while (line[*index] == ' ' || line[*index] == '\000')
    {
        (*index)++;
    }
    if (*index >= 100)
    {
        printf("syntax hatasi uzun satir");
        return -1;
    }
    if (line[*index] == '$')
    {
        if (degiskenKontrolu(line, index) == -1)
        {
            return -1;
        }
    }
    while (line[*index] == ' ' || line[*index] == '\000')
    {
        (*index)++;
    }
    if (line[*index] == '=')
    {
        (*index)++;
        while (line[*index] == '\000')
        {
            index++;
        }
        if (line[*index] != ' ')
        {
            printf("syntax hatasi = den sonra boşluk olmali");
            return -1;
        }
        (*index)++;
        return atama(line, index, 99);
    }
    else
    {
        printf("syntax hatasi eger yapisinda bir tane atama olmalidir");
        return -1;
    }
    while (line[*index] == ' ' || line[*index] == '\000')
    {
        (*index)++;
    }
    if (*index >= 100)
    {
        printf("syntax hatasi uzun satir");
        return -1;
    }
    if (line[*index] == '[')
    {
        char tmp[9];
        for (int i = 0; i < 9; i++)
        {
            tmp[i] = '\000';
        }
        int tmpIndex = 0;
        while (line[*index] != ']')
        {
            if (tmpIndex == 9)
            {
                printf("syntax hatasi uzun keyword");
                return -1;
            }
            if (line[*index] != '\000')
            {
                tmp[tmpIndex] = line[*index];
                tmpIndex++;
            }
            (*index)++;
        }
        tmp[tmpIndex] = line[*index];
        tmpIndex++;
        int tmpCheck = -1;
        for (int i = 0; i < 20; i++)
        {
            int check = 0;
            for (int j = 0; j < 9; j++)
            {
                if (tmp[j] == LOOKUPARRAY[i][j])
                {
                    check++;
                }
            }
            if (check == 9)
            {
                tmpCheck = i;
                break;
            }
        }
        if (tmpCheck == -1)
        {
            printf("syntax hatasi böyle bir keyword yok");
            return -1;
        }
        switch (tmpCheck)
        {
        case 2:
            break;
        default:
            printf("syntax hatasi bilinmeyen keyword");
            return -1;
            break;
        }
    }
    while (line[*index] == ' ' || line[*index] == '\000')
    {
        (*index)++;
    }
    if (*index >= 100)
    {
        printf("syntax hatasi uzun satir");
        return -1;
    }
    if (line[*index] == '$')
    {
        if (degiskenKontrolu(line, index) == -1)
        {
            return -1;
        }
    }
    while (line[*index] == ' ' || line[*index] == '\000')
    {
        (*index)++;
    }
    if (line[*index] == '=')
    {
        (*index)++;
        while (line[*index] == '\000')
        {
            index++;
        }
        if (line[*index] != ' ')
        {
            printf("syntax hatasi = den sonra boşluk olmali");
            return -1;
        }
        (*index)++;
        return atama(line, index, 99);
    }
    while (line[*index] == ' ' || line[*index] == '\000')
    {
        (*index)++;
    }
    if (*index >= 100)
    {
        printf("syntax hatasi uzun satir");
        return -1;
    }
    if (line[*index] == '[')
    {
        char tmp[9];
        for (int i = 0; i < 9; i++)
        {
            tmp[i] = '\000';
        }
        int tmpIndex = 0;
        while (line[*index] != ']')
        {
            if (tmpIndex == 9)
            {
                printf("syntax hatasi uzun keyword");
                return -1;
            }
            if (line[*index] != '\000')
            {
                tmp[tmpIndex] = line[*index];
                tmpIndex++;
            }
            (*index)++;
        }
        tmp[tmpIndex] = line[*index];
        tmpIndex++;
        int tmpCheck = -1;
        for (int i = 0; i < 20; i++)
        {
            int check = 0;
            for (int j = 0; j < 9; j++)
            {
                if (tmp[j] == LOOKUPARRAY[i][j])
                {
                    check++;
                }
            }
            if (check == 9)
            {
                tmpCheck = i;
                break;
            }
        }
        if (tmpCheck == -1)
        {
            printf("syntax hatasi böyle bir keyword yok");
            return -1;
        }
        switch (tmpCheck)
        {
        case 3:
            break;
        default:
            printf("syntax hatasi bilinmeyen keyword");
            return -1;
            break;
        }
    }else{
        printf("syntax hatasi eger ifadesi [bitis] ile bitmelidir");
        return -1;
    }
    return 0;

};
int degiskenKontrolu(char *line, int *index)
{
    if (line[*index] == '$')
    {
        char tmp[10];
        for (int i = 0; i < 10; i++)
        {
            tmp[i] = '\000';
        }
        int tmpIndex = 0;
        while (line[*index] != ' ')
        {
            tmp[tmpIndex] = line[*index];
            tmpIndex++;
            (*index)++;
            if (tmpIndex == 10)
            {
                printf("syntax hatasi uzun değişken");
                return -1;
            }
            if (line[*index] < 'a' && line[*index] > 'z')
            {
                printf("syntax hatasi değişken ismi sadece küçük harf olmali");
                return -1;
            }
        }
        int errorVariable = -1;
        for (int i = 0; i < 5; i++)
        {
            if (strcmp(tmp, VARIABLEARRAY[i]) == 0)
            {
                errorVariable = 0;
                break;
            }
        }
        if (errorVariable == -1)
        {
            printf("syntax hatasi böyle bir değişken yok");
            return -1;
        }
        while (line[*index] == ' ' || line[*index] == '\000')
        {
            (*index)++;
        }
    }
    else
    {
        return -1;
    }
    return 0;
};
int dongu(char *line, int *index){
    // dongu işlemi
};

int islem(char *line, int *index, int lastIndex)
{
    // islem işlemi
    (*index)++;
    while (line[*index] == ' ' || line[*index] == '\000')
    {
        (*index)++;
    }
    return term(line, index, lastIndex);
};

int term(char *line, int *index, int lastIndex)
{
    // term işlemi
    int error = factor(line, index, lastIndex);
    if (error == -1)
    {
        return -1;
    }
    if (lastIndex < *index)
    {
        return 0;
    }
    while (line[*index] == ' ' || line[*index] == '\000')
    {
        (*index)++;
    }
    if (line[*index] == '+' || line[*index] == '-')
    {
        (*index)++;
        while (line[*index] == ' ' || line[*index] == '\000')
        {
            (*index)++;
        }
        if (line[*index] == '$')
        {
            if (degiskenKontrolu(line, index) == -1)
            {
                return -1;
            }
        }
        else if (line[*index] >= '0' && line[*index] <= '9')
        {
            while (line[*index] >= '0' && line[*index] <= '9')
            {
                (*index)++;
            }
        }
        else if (line[*index] == '(')
        {
            return nextOperation(line, index, lastIndex);
        }
        else
        {
            printf("syntax hatasi yanliş operatör");
            return -1;
        }
        return 0;
    }
    return 0;
};

int factor(char *line, int *index, int lastIndex)
{
    // factor işlemi
    int error = nextOperation(line, index, lastIndex);
    if (error == -1)
    {
        return -1;
    }
    while (line[*index] == ' ' || line[*index] == '\000')
    {
        (*index)++;
    }
    if (lastIndex < *index)
    {
        return 0;
    }
    if (line[*index] == '*' || line[*index] == '/')
    {
        (*index)++;
        while (line[*index] == ' ')
        {
            (*index)++;
        }
        if (line[*index] == '$')
        {
            if (degiskenKontrolu(line, index) == -1)
            {
                return -1;
            }
        }
        else if (line[*index] >= '0' && line[*index] <= '9')
        {
            while (line[*index] >= '0' && line[*index] <= '9')
            {
                (*index)++;
            }
        }
        else
        {
            printf("syntax hatasi yanliş operatör");
            return -1;
        }
        return 0;
    }
    return 0;
};

int nextOperation(char *line, int *index, int lastIndex)
{
    // nextOperation işlemi
    if (lastIndex < *index)
    {
        return 0;
    }
    if (line[*index] == '(')
    {
        char c = line[lastIndex];
        while (c != ')')
        {
            lastIndex--;
            c = line[lastIndex];
        }
        return atama(line, index, lastIndex);
    }
    else if (line[*index] == '$')
    {
        if (degiskenKontrolu(line, index) == -1)
        {
            return -1;
        }
    }
    else if (line[*index] >= '0' && line[*index] <= '9')
    {
        while (line[*index] >= '0' && line[*index] <= '9')
        {
            (*index)++;
        }
    }
    return 0;
};
