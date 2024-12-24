#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void to_lowercase_and_convert_tr_chars(char *str) {
    char *p = str;
    while (*p) {
        *p = tolower(*p);
        switch (*p) {
            case 'ç': *p = 'c';
                break;
            case 'ğ': *p = 'g';
                break;
            case 'ı': *p = 'i';
                break;
            case 'ö': *p = 'o';
                break;
             case 'ş': *p = 's';
                break;
            case 'ü': *p = 'u';
                break;
        }
        p++;
    }
}

#define MAX_LINE_LENGTH 35

const char *valid_months[] = {
    "ocak", "subat", "mart", "nisan", "mayis", "haziran",
    "temmuz", "agustos", "eylul", "ekim", "kasim", "aralik"
};

int main(void) {
    FILE *input;
    FILE *output;
    char satir[MAX_LINE_LENGTH];
    char hedef_ay[10], ay_ismi[10];
    float toplam_upload = 0, toplam_download = 0, odenecek_tutar = 0;
    float upload, download;
    // Girdi dosyasını aç
    const char *input_file_path = "D:/CLessons/DonemOdevi/adsl.txt";
    input = fopen(input_file_path, "r");
    if (input == NULL) {
        printf("Girdi dosyasi acilamadi.\n");
        return 1;
    }
    // kullanıcıdan ay ismi al ve doğruluğunu kontrol et
    bool valid = false;
    while (!valid) {
        printf(
            "Ay ismini girin (ocak, subat, mart, nisan, mayis, haziran, temmuz, agustos, eylul, ekim, kasim, aralik):");
        scanf("%s", hedef_ay);
        to_lowercase_and_convert_tr_chars(hedef_ay);
        for (int i = 0; i < 12; i++) {
            if (strcmp(hedef_ay, valid_months[i]) == 0) {
                valid = true;
                break;
            }
        }
        if (!valid) {
            printf("Gecersiz ay ismi, lutfen tekrar deneyin.\n");
        }
    }
    // Dosyadan satır satır oku ve verileri topla
    while (fgets(satir, sizeof(satir), input) != NULL) {
        sscanf(satir, "%s %*d %f %f", ay_ismi, &upload, &download); // %*d ile gün verisini atlıyoruz
        to_lowercase_and_convert_tr_chars(hedef_ay);
        if (strcmp(ay_ismi, hedef_ay) == 0) {
            toplam_upload += upload;
            toplam_download += download;
        }
    }
    // Girdi dosyasını kapat
    fclose(input);

    // Ödenecek tutarı hesapla
    if (toplam_upload + toplam_download > 6 * 1024) {
        odenecek_tutar = (toplam_upload + toplam_download - 6 * 1024) * 0.011719;
    }

    // Çıktı dosyasını başlat
    const char *output_file_path = "D:/CLessons/DonemOdevi/fatura.txt";
    output = fopen(output_file_path, "w");
    if (output == NULL) {
        printf("Cikti dosyasi acilamadi.\n");
        return 1;
    }
    // Sonuçları fatura.txt dosyasına yaz
    fprintf(output, "%s\n", hedef_ay);
    fprintf(output, "TOPLAM\n");
    fprintf(output, "UPLOAD: %.2f GB\n", toplam_upload / 1024); // MB to GB dönüşümü
    fprintf(output, "TOPLAM\n");
    fprintf(output, "DOWNLOAD: %.2f GB\n", toplam_download / 1024); // MB to GB dönüşümü
    fprintf(output, "\nÖDENECEK TUTAR: %.2f TL\n", odenecek_tutar);

    // Çıktı dosyasını kapat
    fclose(output);

    printf("%s 2012 dosyaniz olusturulmustur", hedef_ay);

    return 0;
}
