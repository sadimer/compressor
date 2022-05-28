#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "ari.h"
#include "ppm.h"

#define CODE_VALUE_BITS 16
#define QTR1 USHRT_MAX / 4 + 1
#define HLF 2 * QTR1
#define QTR2 HLF
#define QTR3 3 * QTR1
#define MAX_FREQ USHRT_MAX >> 2
#define CHARS UCHAR_MAX + 1
#define SYMBOLS UCHAR_MAX + 2
#define EOF_S UCHAR_MAX + 2

int getFileSize(const char * fileName)
{
    struct stat file_stat;
    stat(fileName, &file_stat);
    return file_stat.st_size;
}

unsigned int freq_eng[SYMBOLS + 1] = 
   {0, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 45, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    1401, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 66, 6, 134, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 11, 
    1, 17, 6, 13, 7, 3, 3, 7, 16, 26, 3, 11, 9, 13, 5, 4, 
    6, 1, 10, 21, 26, 2, 4, 12, 1, 3, 1, 1, 1, 1, 1, 1, 
    1, 456, 77, 156, 252, 746, 109, 122, 328, 382, 4, 72, 233, 122, 393, 431, 
    105, 3, 324, 335, 493, 164, 58, 129, 7, 88, 11, 1, 1, 1, 1, 1, 
    126, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 34, 1, 1, 42, 42, 1, 1, 
    1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 126, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    1};
    
unsigned int freq_rus[SYMBOLS + 1] =
   {0, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 36, 1, 1, 36, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    1378, 9, 6, 1, 1, 1, 1, 4, 1, 1, 1, 1, 173, 57, 89, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 4, 1, 1, 1, 8, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 4, 1, 1, 
    1, 20, 2, 9, 8, 53, 2, 2, 4, 19, 1, 1, 13, 10, 21, 18, 
    7, 2, 19, 21, 17, 18, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 
    11, 8, 11, 2, 5, 1, 1, 1, 4, 1, 10, 1, 6, 12, 9, 11, 
    4, 5, 4, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 3, 
    473, 91, 254, 117, 169, 453, 57, 101, 379, 66, 197, 291, 164, 364, 647, 137, 
    258, 296, 323, 164, 12, 48, 23, 76, 53, 16, 3, 109, 112, 14, 37, 130, 
    1};
    
unsigned int cum_freq_eng[SYMBOLS + 1];
unsigned int char_to_index_eng[CHARS];
unsigned int index_to_char_eng[SYMBOLS + 1];

unsigned int cum_freq_rus[SYMBOLS + 1];
unsigned int char_to_index_rus[CHARS];
unsigned int index_to_char_rus[SYMBOLS + 1];

unsigned int char_to_index[CHARS];
unsigned int index_to_char[SYMBOLS + 1];
unsigned int cum_freq_table[SYMBOLS + 1];
unsigned int freq_table[SYMBOLS + 1];

unsigned char buffer;
unsigned int BUF_CAP = 8;
unsigned int bits_to_go;
unsigned int bits_fw = 0;

unsigned short min;
unsigned short max;

unsigned short code_value = 0;

unsigned char buffer_eng;
unsigned int bits_to_go_eng;
unsigned int bits_fw_eng = 0;

unsigned short min_eng;
unsigned short max_eng;

unsigned short code_value_eng = 0;

unsigned char buffer_rus;
unsigned int bits_to_go_rus;
unsigned int bits_fw_rus = 0;

unsigned short min_rus;
unsigned short max_rus;

unsigned short code_value_rus = 0;

char
min_file(int a, int b, int c) 
{
    int min = a;
    if (b < min) {
        return 2;
    }
    if (c < min) {
        return 3;
    }
    return 1;
}

void
init(void)
{
    for (int i = 0; i < SYMBOLS + 1; ++i) {
        freq_table[i] = 1;
    }
    freq_table[0] = 0;
    for (int i = 0; i < CHARS; i++) {
        char_to_index[i] = i + 1;
        index_to_char[i + 1] = i;
    }
    cum_freq_table[SYMBOLS] = 0;
    for (int i = SYMBOLS; i > 0; i--) {
        cum_freq_table[i - 1] = cum_freq_table[i] + freq_table[i];
    }
    
    for (int i = 0; i < CHARS; i++) {
        char_to_index_eng[i] = i + 1;
        index_to_char_eng[i + 1] = i;
    }
    
    cum_freq_eng[SYMBOLS] = 0;
    for (int i = SYMBOLS; i > 0; i--) {
        cum_freq_eng[i - 1] = cum_freq_eng[i] + freq_eng[i];
    }
    
    for (int i = 0; i < CHARS; i++) {
        char_to_index_rus[i] = i + 1;
        index_to_char_rus[i + 1] = i;
    }
    
    cum_freq_rus[SYMBOLS] = 0;
    for (int i = SYMBOLS; i > 0; i--) {
        cum_freq_rus[i - 1] = cum_freq_rus[i] + freq_rus[i];
    }
    
}

void
update(int symbol) 
{
    if (cum_freq_table[0] >= MAX_FREQ) {
        unsigned cum = 0;
        for (int i = SYMBOLS; i >= 0; i--) {
            freq_table[i] = 2 * (freq_table[i] + 1) / 3;
            if (freq_table[i] == 0) {
                freq_table[i] = 1;
            }
            cum_freq_table[i] = cum;
            cum += freq_table[i];
        }   
    }
    int i;
    for (i = symbol; freq_table[i] == freq_table[i-1]; i--);
    if (i < symbol) {
        unsigned int tmp_i = index_to_char[i];
        unsigned int tmp_symbol = index_to_char[symbol];
        
        index_to_char[i] = tmp_symbol;
        index_to_char[symbol] = tmp_i;
        
        char_to_index[tmp_i] = symbol;
        char_to_index[tmp_symbol] = i;
    }

    freq_table[i] += 20;
    while (i > 0) {
        i--;
        cum_freq_table[i] += 20;
    }
}

void
update_eng(int symbol) 
{
    if (cum_freq_eng[0] >= MAX_FREQ) {
        unsigned cum = 0;
        for (int i = SYMBOLS; i >= 0; i--) {
            freq_eng[i] = 3 * (freq_eng[i] + 1) / 4;
            if (freq_eng[i] == 0) {
                freq_eng[i] = 1;
            }
            cum_freq_eng[i] = cum;
            cum += freq_eng[i];
        }   
    }
    int i = symbol;

    freq_eng[i] += 4;
    while (i > 0) {
        i--;
        cum_freq_eng[i] += 4;
    }
}

void
update_rus(int symbol) 
{
    if (cum_freq_rus[0] >= MAX_FREQ) {
        unsigned cum = 0;
        for (int i = SYMBOLS; i >= 0; i--) {
            freq_rus[i] = 3 * (freq_rus[i] + 1) / 4;
            if (freq_rus[i] == 0) {
                freq_rus[i] = 1;
            }
            cum_freq_rus[i] = cum;
            cum += freq_rus[i];
        }   
    }
    int i = symbol;
    
    freq_rus[i] += 4;
    while (i > 0) {
        i--;
        cum_freq_rus[i] += 4;
    }
}

void
output(unsigned bit, FILE *stream)
{
    buffer >>= 1;
    if (bit != 0) {
        buffer = buffer | 0x80;
    }
    bits_to_go--;
    if (bits_to_go == 0) {
        fputc(buffer, stream);
        bits_to_go = 8;
    }
}

void
bits_plus_fw(unsigned bit, FILE *stream)
{
    output(bit, stream);
    while (bits_fw > 0) {
        output(!bit, stream);
        bits_fw--;
    }
}

void
encode_sym(int symbol, FILE *stream)
{
    unsigned int range = (unsigned int) max - min + 1;
    max = min + (unsigned int) ((range * cum_freq_table[symbol - 1]) / cum_freq_table[0] - 1);
    min = min + (unsigned int) ((range * cum_freq_table[symbol]) / cum_freq_table[0]);

    while (1) {
        if (max < HLF) {
            bits_plus_fw(0, stream);
        } else if (min >= HLF) {
            min -= HLF;
            max -= HLF;
            bits_plus_fw(1, stream);
        } else if (min >= QTR1 && max < QTR3) {
            min -= QTR1;
            max -= QTR1;
            bits_fw++;
        } else {
            break;
        }
        max = 1 + (max << 1);
        min = min << 1;
    }
}

void
output_eng(unsigned bit, FILE *stream)
{
    buffer_eng >>= 1;
    if (bit != 0) {
        buffer_eng = buffer_eng | 0x80;
    }
    bits_to_go_eng--;
    if (bits_to_go_eng == 0) {
        fputc(buffer_eng, stream);
        bits_to_go_eng = 8;
    }
}

void
bits_plus_fw_eng(unsigned bit, FILE *stream)
{
    output_eng(bit, stream);
    while (bits_fw_eng > 0) {
        output_eng(!bit, stream);
        bits_fw_eng--;
    }
}

void
encode_sym_eng(int symbol, FILE *stream)
{
    unsigned int range = (unsigned int) max_eng - min_eng + 1;
    max_eng = min_eng + (unsigned int) ((range * cum_freq_eng[symbol - 1]) / cum_freq_eng[0] - 1);
    min_eng = min_eng + (unsigned int) ((range * cum_freq_eng[symbol]) / cum_freq_eng[0]);

    while (1) {
        if (max_eng < HLF) {
            bits_plus_fw_eng(0, stream);
        } else if (min_eng >= HLF) {
            min_eng -= HLF;
            max_eng -= HLF;
            bits_plus_fw_eng(1, stream);
        } else if (min_eng >= QTR1 && max_eng < QTR3) {
            min_eng -= QTR1;
            max_eng -= QTR1;
            bits_fw_eng++;
        } else {
            break;
        }
        max_eng = 1 + (max_eng << 1);
        min_eng = min_eng << 1;
    }
}

void
output_rus(unsigned bit, FILE *stream)
{
    buffer_rus >>= 1;
    if (bit != 0) {
        buffer_rus = buffer_rus | 0x80;
    }
    bits_to_go_rus--;
    if (bits_to_go_rus == 0) {
        fputc(buffer_rus, stream);
        bits_to_go_rus = 8;
    }
}

void
bits_plus_fw_rus(unsigned bit, FILE *stream)
{
    output_rus(bit, stream);
    while (bits_fw_rus > 0) {
        output_rus(!bit, stream);
        bits_fw_rus--;
    }
}

void
encode_sym_rus(int symbol, FILE *stream)
{
    unsigned int range = (unsigned int) max_rus - min_rus + 1;
    max_rus = min_rus + (unsigned int) ((range * cum_freq_rus[symbol - 1]) / cum_freq_rus[0] - 1);
    min_rus = min_rus + (unsigned int) ((range * cum_freq_rus[symbol]) / cum_freq_rus[0]);

    while (1) {
        if (max_rus < HLF) {
            bits_plus_fw_rus(0, stream);
        } else if (min_rus >= HLF) {
            min_rus -= HLF;
            max_rus -= HLF;
            bits_plus_fw_rus(1, stream);
        } else if (min_rus >= QTR1 && max_rus < QTR3) {
            min_rus -= QTR1;
            max_rus -= QTR1;
            bits_fw_rus++;
        } else {
            break;
        }
        max_rus = 1 + (max_rus << 1);
        min_rus = min_rus << 1;
    }
}

void
compress_ari(char *ifile, char *ofile)
{
    FILE *inpf = (FILE *) fopen(ifile, "rb");
    FILE *outf = (FILE *) fopen(ofile, "wb");
    FILE *f1 = (FILE *) fopen("test1.bin", "wb");
    FILE *f2 = (FILE *) fopen("test2.bin", "wb");
    FILE *f3 = (FILE *) fopen("test3.bin", "wb");
    
    if (getFileSize(ifile) == 0) {
        return;
    }
    
    fseek(outf, sizeof(char), SEEK_SET);
    
    init();

    min = 0;
    max = USHRT_MAX;
    buffer = 0;
    bits_to_go = 8;
    bits_fw = 0;
    
    min_eng = 0;
    max_eng = USHRT_MAX;
    buffer_eng = 0;
    bits_to_go_eng = 8;
    bits_fw_eng = 0;
    
    min_rus = 0;
    max_rus = USHRT_MAX;
    buffer_rus = 0;
    bits_to_go_rus = 8;
    bits_fw_rus = 0;
    
    int c;
    while ((c = fgetc(inpf)) != EOF) {
        int symbol = char_to_index[c];
        encode_sym(symbol, f1);
        update(symbol);
        symbol = char_to_index_eng[c];
        encode_sym_eng(symbol, f2);
        update_eng(symbol);
        symbol = char_to_index_rus[c];
        encode_sym_rus(symbol, f3);
        update_rus(symbol);
    }
    encode_sym(EOF_S, f1);
    encode_sym_eng(EOF_S, f2);
    encode_sym_rus(EOF_S, f3);
    
    bits_fw++;
    if (min >= QTR1) {
        bits_plus_fw(1, f1);
    } 
    else {
        bits_plus_fw(0, f1);
    }
    fputc(buffer >> bits_to_go, f1);
    
    bits_fw_eng++;
    if (min_eng >= QTR1) {
        bits_plus_fw_eng(1, f2);
    } 
    else {
        bits_plus_fw_eng(0, f2);
    }
    fputc(buffer_eng >> bits_to_go_eng, f2);
    
    bits_fw_rus++;
    if (min_rus >= QTR1) {
        bits_plus_fw_rus(1, f3);
    } 
    else {
        bits_plus_fw_rus(0, f3);
    }
    fputc(buffer_rus >> bits_to_go_rus, f3);
    
    fclose(f1);
    fclose(f2);
    fclose(f3);
    
    char filename[20] = {0};
    char mode = min_file(getFileSize("test1.bin"), getFileSize("test2.bin"), getFileSize("test3.bin"));
    snprintf(filename, 20, "test%d.bin", mode);
        
    char tmp;
    FILE *rd = (FILE *) fopen(filename, "rb");
    while (fread(&tmp, sizeof(char), 1, rd) == 1) {
        fwrite(&tmp, sizeof(char), 1, outf);
    }
    
    fseek(outf, 0, SEEK_SET);
    fwrite(&mode, sizeof(char), 1, outf);
    
    fclose(inpf);
    fclose(outf);
    fclose(rd);
    remove("test1.bin");
    remove("test2.bin");
    remove("test3.bin");
}

unsigned
input(FILE *stream)
{
    if (bits_to_go == 0) {
        buffer = fgetc(stream);
        bits_to_go = 8;
    }
    unsigned int bit = buffer & 1;
    buffer >>= 1;
    bits_to_go--;
    return bit; 
}


int
decode_sym(FILE *stream)
{
    int symbol;
    unsigned int range = (unsigned int)(max - min) + 1;
    unsigned int cum_freq = (((unsigned int)(code_value - min) + 1) * cum_freq_table[0] - 1) / range;
    for (symbol = 1; cum_freq_table[symbol] > cum_freq; symbol++);
    unsigned int tmp = (range * cum_freq_table[symbol - 1]) / cum_freq_table[0];
    max = min + tmp - 1;
    tmp = (range * cum_freq_table[symbol]) / cum_freq_table[0];
    min = min + tmp;
    while (1) {
        if (max < HLF) {
            
        }   
        else if (min >= HLF) {
            min -= HLF;
            max -= HLF;
            code_value -= HLF;
        }
        else if (min >= QTR1 && max < QTR3) {
            max -= QTR1;
            min -= QTR1;
            code_value -= QTR1;
        }
        else {
            break;
        }
        max = 1 + (max << 1);
        min = min << 1;
        code_value = input(stream) + (code_value << 1);
    }
    
    return symbol;
}

unsigned
input_eng(FILE *stream)
{
    if (bits_to_go_eng == 0) {
        buffer_eng = fgetc(stream);
        bits_to_go_eng = 8;
    }
    unsigned int bit = buffer_eng & 1;
    buffer_eng >>= 1;
    bits_to_go_eng--;
    return bit; 
}


int
decode_sym_eng(FILE *stream)
{
    int symbol;
    unsigned int range = (unsigned int)(max_eng - min_eng) + 1;
    unsigned int cum_freq = (((unsigned int)(code_value_eng - min_eng) + 1) * cum_freq_eng[0] - 1) / range;
    for (symbol = 1; cum_freq_eng[symbol] > cum_freq; symbol++);
    unsigned int tmp = (range * cum_freq_eng[symbol - 1]) / cum_freq_eng[0];
    max_eng = min_eng + tmp - 1;
    tmp = (range * cum_freq_eng[symbol]) / cum_freq_eng[0];
    min_eng = min_eng + tmp;
    while (1) {
        if (max_eng < HLF) {
            
        }   
        else if (min_eng >= HLF) {
            min_eng -= HLF;
            max_eng -= HLF;
            code_value_eng -= HLF;
        }
        else if (min_eng >= QTR1 && max_eng < QTR3) {
            max_eng -= QTR1;
            min_eng -= QTR1;
            code_value_eng -= QTR1;
        }
        else {
            break;
        }
        max_eng = 1 + (max_eng << 1);
        min_eng = min_eng << 1;
        code_value_eng = input_eng(stream) + (code_value_eng << 1);
    }
    
    return symbol;
}

unsigned
input_rus(FILE *stream)
{
    if (bits_to_go_rus == 0) {
        buffer_rus = fgetc(stream);
        bits_to_go_rus = 8;
    }
    unsigned int bit = buffer_rus & 1;
    buffer_rus >>= 1;
    bits_to_go_rus--;
    return bit; 
}


int
decode_sym_rus(FILE *stream)
{
    int symbol;
    unsigned int range = (unsigned int)(max_rus - min_rus) + 1;
    unsigned int cum_freq = (((unsigned int)(code_value_rus - min_rus) + 1) * cum_freq_rus[0] - 1) / range;
    for (symbol = 1; cum_freq_rus[symbol] > cum_freq; symbol++);
    unsigned int tmp = (range * cum_freq_rus[symbol - 1]) / cum_freq_rus[0];
    max_rus = min_rus + tmp - 1;
    tmp = (range * cum_freq_rus[symbol]) / cum_freq_rus[0];
    min_rus = min_rus + tmp;
    while (1) {
        if (max_rus < HLF) {
            
        }   
        else if (min_rus >= HLF) {
            min_rus -= HLF;
            max_rus -= HLF;
            code_value_rus -= HLF;
        }
        else if (min_rus >= QTR1 && max_rus < QTR3) {
            max_rus -= QTR1;
            min_rus -= QTR1;
            code_value_rus -= QTR1;
        }
        else {
            break;
        }
        max_rus = 1 + (max_rus << 1);
        min_rus = min_rus << 1;
        code_value_rus = input_rus(stream) + (code_value_rus << 1);
    }
    
    return symbol;
}

void 
decompress_ari(char *ifile, char *ofile)
{
    FILE *outf = (FILE *) fopen(ifile, "rb");
    FILE *resf = (FILE *) fopen(ofile, "wb");
    
    if (getFileSize(ifile) == 0) {
        return;
    }
    char tmp;
    fread(&tmp, sizeof(char), 1, outf);
    if (tmp == 1) {
        init();
        min = 0;
        max = USHRT_MAX;
        buffer = 0;
        bits_to_go = 0;
        bits_fw = 0;
        code_value = 0;
    
        for (int i = 0; i < CODE_VALUE_BITS; i++) {
            code_value = input(outf) + (code_value << 1);
        }
        int c;
        while ((c = decode_sym(outf)) != EOF_S) {
            char ch = index_to_char[c];
            fputc(ch, resf);
            update(c);
        }
    } else if (tmp == 2) {
        init();
        min_eng = 0;
        max_eng = USHRT_MAX;
        buffer_eng = 0;
        bits_to_go_eng = 0;
        bits_fw_eng = 0;
        code_value_eng = 0;
    
        for (int i = 0; i < CODE_VALUE_BITS; i++) {
            code_value_eng = input_eng(outf) + (code_value_eng << 1);
        }
        int c;
        while ((c = decode_sym_eng(outf)) != EOF_S) {
            char ch = index_to_char_eng[c];
            fputc(ch, resf);
            update_eng(c);
        }
    } else if (tmp == 3) {
        init();
        min_rus = 0;
        max_rus = USHRT_MAX;
        buffer_rus = 0;
        bits_to_go_rus = 0;
        bits_fw_rus = 0;
        code_value_rus = 0;
    
        for (int i = 0; i < CODE_VALUE_BITS; i++) {
            code_value_rus = input_rus(outf) + (code_value_rus << 1);
        }
        int c;
        while ((c = decode_sym_rus(outf)) != EOF_S) {
            char ch = index_to_char_rus[c];
            fputc(ch, resf);
            update_rus(c);
        }
    }
    fclose(outf);
    fclose(resf);
}

