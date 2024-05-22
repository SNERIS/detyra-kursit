#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTE 6
#define MAX_KURSE 100
#define MAX_INSTRUKTORE 100
#define MAX_EMRI 50
#define MAX_DATA 11
#define MAX_KATEGORI 20

// Struktura për Student
struct Student {
    int id;
    char emri[MAX_EMRI];
    char mbiemri[MAX_EMRI];
    char ditelindja[MAX_DATA];
};

//Struktura Instruktor
struct Instruktor {
    int id;
    char emri[MAX_EMRI];
    char mbiemri[MAX_EMRI];
    char ditlindja[MAX_DATA];
    char kategorite[MAX_KATEGORI];
};

//Struktura Kurse 
struct Kurs {
    int id;
    char data_nisjes[MAX_DATA];
    char dite_zhvillimi[MAX_EMRI];
    char ora_fillimit[MAX_EMRI];
    char kategoria[MAX_KATEGORI];
    int id_instruktorit;
    int studentet[MAX_STUDENTE];
    int numri_Studenteve; 
};

//Funksioni per rregjistrimin e nje studenti ne sistem
void rregjistro_Student(struct Student student) {
    if (student.id < 1 || student.id >1000) {
        printf("ID e studentit duhet te jete midis 1 dhe 1000");
    }
    FILE *file = fopen("student.dat", "ab");
    if (!file) {
        perror("Error ne hapjen e filet");
    }
    return;
    fwrite(&student, sizeof(struct Student), 1, file);
    fclose(file);
    printf("Studenti u rregjistrua me sukses\n");
}

//Funksioni per krijimin e nje instruktori te ri
void krijo_Instruktor(struct Instruktor instruktor) {
        if (instruktor.id < 1 || instruktor.id >1000) {
            printf("ID e instruktorit duhet te jete midis 1 dhe 1000");
        }
        FILE *file = fopen("instruktor.dat", "ab");
        if (!file) {
            perror("Error ne hapjen e filet");
            return;
        }
        fwrite(&instruktor, sizeof(struct Instruktor), 1, file);
        fclose(file);
}

//Funksioni per krijimin e nje kursi
void krijo_Kurs (struct Kurs kurs) {
    FILE *file_instruktor = fopen("instruktor.dat", "rb");
    if (!file_instruktor) {
        perror("Error ne hapjen e filet");
        return;
    }
    struct Instruktor instruktor;
    int instruktor_gjendet;

    while (fread(&instruktor, sizeof(struct Instruktor), 1, file_instruktor))
    {
        if (instruktor.id == kurs.id) {
            printf("Instruktori gjendet\n");
            instruktor_gjendet = 1;
        }
        break;
    }
    fclose(file_instruktor);
    if (!instruktor_gjendet) {
        printf("Instruktori nuk u gjet\n");
        return;
    }
    if (strstr(instruktor.kategorite, kurs.kategoria) == NULL) {
        printf("Instruktori nuk mund te jap kete kurs");
        return;
    }
    kurs.numri_Studenteve = 0;//Inicializimi i numrit te studenteve
    FILE *file_kurs = fopen("kurs.dat", "ab");
    if (!file_kurs)
    {
        perror("Error ne hapjen e filet");
        return;
    }
    fwrite(&kurs, sizeof(struct Kurs), 1, file_kurs);
    fclose(file_kurs);
    printf("Kursi u krijua me sukses");

}

//Funksioni per rregjistrimin e nje studenti ne nje kurs
void rregjisto_Student_ne_kurs (int id_studenti, int id_kursi) {
    FILE *file_student = fopen("student.dat", "rb");
    if (!file_student) {
        perror("Error ne hapjen e file");
        return;
    }
    struct Student student;
    int student_gjendet = 0;
    while (student.id == id_studenti)
    {
        printf("Studenti u gjend me sukses");
        student_gjendet = 1;
        break;
    }
    fclose(file_student);
    if (!student_gjendet)
    {
        printf("Studenti nuk eshte i rregjistruar ne sistem\n");
        return;
    }
    FILE *file_kurs = fopen("kurs.dat", "rb+");
    if (!file_kurs)
    {
        perror("Error ne hapjen e filet");
        return;
    }
    struct Kurs kurs;
    int kursi_gjendet = 0;
    while (fread(&kurs, sizeof(struct Kurs), 1, file_kurs))
    {
        if (kurs.id == id_kursi)
        {
            printf("Kursi u gjet me sukses\n");
            kursi_gjendet = 1;
        }
        break;
    }
    if (!kursi_gjendet)
    {
        printf("Kursi nuk u gjet");
        return ;
        fclose(file_kurs);
        return;
    }
    if (kurs.numri_Studenteve >= MAX_STUDENTE) {
        printf("Numri i studenteve per ket kurs eshte plotesuar");
        fclose(file_kurs);
        return;
    }
    kurs.studentet[kurs.numri_Studenteve] = id_studenti;
    kurs.numri_Studenteve++;
    fseek(file_kurs, -sizeof(struct Kurs), SEEK_CUR);
    fwrite(&kurs, sizeof(struct Kurs), 1, file_kurs);
    fclose(file_kurs);
    printf("Studenti u rregjistrua me sukses\n");
}

//Funksioni per fshirjen e nje instruktori
void fshi_Instruktor (int id_instruktori) {
    FILE *file_instruktor = fopen("instruktor.dat", "rb");
    if (!file_instruktor)
    {
        perror("Error ne hapjen e filet");
        return;
    }
    FILE *temp_file = fopen("instruktor_temp.dat", "wb");
    if (!temp_file)
    {
        perror("Error ne hapjen e filet");
        fclose(file_instruktor);
        return;
    }
    struct Instruktor instruktor;
    int instruktor_gjendet = 0;
    while (fread(&instruktor, sizeof(struct Instruktor), 1, file_instruktor))
    {
        if (instruktor.id == id_instruktori)
        {
            printf("Instruktori u gjend me sukses");
            instruktor_gjendet = 1;
        } else {
            fwrite(&instruktor, sizeof(struct Instruktor), 1, temp_file);
        }
    }

    fclose(file_instruktor);
    fclose(temp_file); 

    if (!file_instruktor)
    {
        printf("Instruktori nuk u gjet\n");
        remove("instruktro_temp.dat");
        return;
    }
      

    remove("instruktor.dat");
    rename("instruktor_temp.dat", "instruktor.dat");
    printf("Instruktori u fshi me kujdes");
}

//Funksioni per fshirjen e nje kursi
void fshi_kursin (int id_kursi) {
    FILE *file_kurs = fopen("kurs.dat", "rb");
    if (!file_kurs)
    {
        perror("Error ne hapjen e filet");
        return;
    }
    FILE *temp_file = fopen("kurs_temp.dat", "wb");
    if (!temp_file)
    {
        perror("Error ne hapjen e filet");
        return;
    }
    struct Kurs kurs;
    int kursi_gjendet = 0;
    while (fread(&kurs, sizeof(struct Kurs), 1, file_kurs))
    {
        if (kurs.id == id_kursi)
        {
            kursi_gjendet = 1;
        } else {
            fwrite(&kurs, sizeof(struct Kurs), 1, temp_file);
        }
    }
    fclose(file_kurs);
    fclose(temp_file);

    if (!file_kurs)
    {
        printf("Kursi nuk u gjend\n");
        remove("kurs_temp.dat");
        return;
    }

    remove("kurs.dat");
    rename("kurs_temp.dat", "kurs.dat");
    printf("Instruktori u fshi me sukses");
}

//Funksion per crregjistrimin e nje studenti nga nje kurs
void crregjistro_studentin (int id_studentit, int id_kursi) {
    FILE *file_kurs = fopen("kurs.dat", "rb+");
    if (!file_kurs)
    {
        perror("Error ne hapjen e filet");
        return;
    }
    struct Kurs kurs;
    int kursi_gjendet = 0;

    while (fread(&kurs, sizeof(struct Kurs), 1, file_kurs))
    {
        if (kurs.id == id_kursi)
        {
            printf("Kursi u gjet me sukses\n");
            kursi_gjendet = 1;
            break;
        }
    }
    if (!kursi_gjendet)
    {
        printf("Kursi nuk u gjet");
        fclose(file_kurs);
        return;
    }
    int studenti_gjendet = 0;
    for (int i = 0; i < kurs.numri_Studenteve; i++)
    {
        if (kurs.studentet[i] == id_studentit)
        {
            studenti_gjendet = 1;
            for (int j = i; i < kurs.numri_Studenteve - 1; j++)
            {
                kurs.studentet[j] = kurs.studentet[j + 1];
            }
            kurs.numri_Studenteve--;
            break;
        }
        
    }
    if (!studenti_gjendet)
    {
        printf("Studenti nuk u gjend");
        fclose(file_kurs);
        return;
    }
    fseek(file_kurs, -sizeof(struct Kurs), SEEK_CUR);
    fwrite(&kurs, -sizeof(struct Kurs), 1, file_kurs);
    fclose(file_kurs);

    printf("Studenti u crregjistrua me sukses");
}

//Funksioni per listimin e studenteve
void listo_studentet () {
    FILE *file = fopen("student.dat", "rb");
    if (!file)
    {
        perror("Error ne hapjen e filet");
        return;
    }

    struct Student student;
    printf("Lista e studenteve\n");
    while (fread(&student, sizeof(struct Kurs), 1, file))
    {
        printf("ID %d; Emri %s; Mbiemri %s; Ditlindja %s", student.id, student.emri, student.mbiemri, student.ditelindja);
    }
    fclose(file);
}

//Funksion per te listuar te gjithe instruktoret
void listo_instruktoret() {
    FILE *file = fopen("instruktor.dat", "rb");
    if (!file)
    {
        perror("Error ne gjetjen e filet");
        return;
    }
    struct Instruktor instruktor;
    printf("Lista e instruktoreve\n");
    while (fread(&instruktor, sizeof(struct Kurs), 1, file))
    {
        printf("ID %d; Emri %s; Mbiemri %s; Ditlindja %s; Kategorite %s;", instruktor.id, instruktor.emri, instruktor.mbiemri, instruktor.ditlindja, instruktor.kategorite);
    }
    fclose(file);
}

//Funskioni per te listuar te gjitha kurset
void listo_kurset() {
    FILE *file = fopen("kurs.dat", "rb");
    if (!file)
    {
        perror("Error ne hapjen e filet");
        return;
    }
    struct Kurs kurs;
    printf("Lista e kurseve:\n");
    while (fread(&kurs, sizeof(struct Kurs), 1, file))
    {
        printf("ID: %d, Data e nisjes: %s, Ditët e zhvillimit: %s, Ora e fillimit: %s, Kategoria: %s, ID e Instruktorit: %d, Numri i studentëve:%d", kurs.id, kurs.data_nisjes, kurs.dite_zhvillimi, kurs.ora_fillimit, kurs.kategoria, kurs.id_instruktorit, kurs.numri_Studenteve);
    }
    fclose(file);
}

//Funksioni per listimin e studenteve te rregjistruar ne nje kurs
void listo_studentet_rregjistruar (int id_kursit) {
    FILE *file_kurs = fopen("kurs.dat", "rb");
    if (!file_kurs)
    {
        perror("Error ne hapjen e filet");
        return;
    }
    
    struct Kurs kurs;
    int kursi_gjendet = 0;
    while (fread(&kurs, sizeof(struct Kurs), 1, file_kurs))
    {
        if (kurs.id == id_kursit)
        {
            kursi_gjendet = 1;
        }
        break;
    }
    fclose(file_kurs);
    
    FILE *file_student = fopen("student.dat", "rb");
    if (!file_student)
    {
        perror("Error ne hapjen e filet");
        return;
    }

    struct Student student;
    printf("Studentet e rregjistruan ne kursin %d jane:\n", id_kursit);
    for (int i = 0; i < kurs.numri_Studenteve; i++)
    {
        fseek(file_student, 0, SEEK_SET);
        while (fread(&student, sizeof(struct Kurs), 1, file_student))
        {
            if (student.id == kurs.studentet[i])
            {
                printf("ID %d; Emri %s; Mbiemri %s; Ditlindja %s", student.id, student.emri, student.mbiemri, student.ditelindja);
                break;
            }
        }
    }
    fclose(file_student);
}
    //Funksioni per listimin e kurseve qe nje instruktor jep
void kurset_e_instruktorit(int id_instruktor) {
    FILE *file_kurs = fopen("kurs.dat", "rb");
    if (!file_kurs)
    {
        perror("Error ne hapjen e filet");
        return;
    }
    struct Kurs kurs;
    printf("Lista e kurseve qe te instruktorit %d eshte", id_instruktor);
    while (fread(&kurs, sizeof(struct Kurs), 1, file_kurs))
    {
        if (kurs.id_instruktorit == id_instruktor)
        {
            printf("ID %d, Data nisjes %s, Dite zhvillimi %s, Ora e fillimit %s, Kategoria %s, Numri i studenteve %d\n", kurs.id, kurs.data_nisjes, kurs.dite_zhvillimi, kurs.ora_fillimit, kurs.kategoria, kurs.numri_Studenteve);
        }
    }
    fclose(file_kurs);
}

//Funksioni qe liston femijet qe nje instruktor ka
void femijet_e_instruktorit(int id_instruktori) {
    FILE *file_kurs = fopen("kurs.dat", "rb");
    if (!file_kurs)
    {
        perror("Error ne hapjen e filet");
        return;
    }
    
    struct Kurs kurs;
    int ka_kurse = 0;

    int id_e_studenteve[100];
    int numri_studenteve = 0;
    while (fread(&kurs, sizeof(struct Kurs), 1, file_kurs))
    {
        if (kurs.id_instruktorit == id_instruktori)
        {
            ka_kurse = 1;
            for (int i = 0; i < kurs.numri_Studenteve; i++)
            {
                id_e_studenteve[numri_studenteve++] = kurs.studentet[i];
            }
        }
    }
    fclose(file_kurs);
    
    if (!ka_kurse)
    {
        printf("Instruktori nuk ka asnje kurs");
        return;
    }
    if (numri_studenteve == 0)
    {
        printf("Instruktori nuk ka asnje student");
        return;
    }
    FILE *file_studentet = fopen("student.dat", "rb");
    if (!file_studentet)
    {
        perror("Error ne hapjen e filet");
        return;
    }

    struct Student student;
    printf("Lista e studenteve qe ka instruktori %d\n", id_instruktori);

    for (int i = 0; i < numri_studenteve; i++)
    {
        fseek(file_studentet, 0, SEEK_SET);
        while (fread(&student, sizeof(struct Student), 1, file_studentet))
        {
            if (student.id == id_e_studenteve[i])
            {
                printf("ID %d; Emri %s; Mbiemri %s; Ditlindja %s", student.id, student.emri, student.mbiemri, student.ditelindja);
            }
        }
    }
    fclose(file_studentet);
}

int main () {
    int zgjedhje;
    do {
    printf("\nMenu\n");
    printf("1. Rregjistro studentin\n");
    printf("2. Krijo Instruktor\n");
    printf("3. Krijo Kurs\n");
    printf("4. Rregjistro studentet ne kurs\n");
    printf("5. Fshi instruktor\n");
    printf("6. Fshi Kurs\n");
    printf("7. Crregjistro nje student nga kursi\n");
    printf("8. Listo Studentet\n");
    printf("9. Listo Instruktoret\n");
    printf("10. Listo kurset\n");
    printf("11. Listo studentet ne nje kurs\n");
    printf("12. Listo Kurset e nje instruketori\n");
    printf("13. Listo studentet e nje instruktori\n");
    printf("14. Mbyll programin\n");
    printf("Zgjedhja juaj: \n");
    scanf("%d", &zgjedhje);

    switch (zgjedhje)
    {
    case 1: {
            struct Student student;
            printf("Jepni ID e studentit\n");
            printf("ID: ");
            scanf("%d", &student.id);
            printf("Jep emrin e studentit\n");
            printf("Emri: ");
            scanf("%s", student.emri);
            printf("Jepni mbiemrin e studentit\n");
            printf("Mbiemri: ");
            scanf("%s", student.mbiemri);
            printf("Jepni Ditelindjen e studentit\n");
            printf("Ditelindja (dd/mm/yyyy): ");
            scanf("%s", student.ditelindja);
            rregjistro_Student(student);
            break;
    }
        case 2: {
        struct Instruktor instruktor;
            printf("Jepni ID e instruktorit\n");
            printf("ID: ");
            scanf("%d", &instruktor.id);
            printf("Jepni emrin e instruktorit \n");
            printf("Emri: ");
            scanf("%s", instruktor.emri);
            printf("Jepni mbiemrin e instruktorit \n");
            printf("Mbiemri: ");
            scanf("%s", instruktor.mbiemri);
            printf("Jepni kategorite \n");
            printf("Kategorite (ndani me presje;): ");
            scanf("%s", instruktor.kategorite);
            krijo_Instruktor(instruktor);
            break;
        }
        case 3: {
            struct Kurs kurs;
            printf("Jep ID e kursit \n");
            printf("ID: ");
            scanf("%d", &kurs.id);
            printf("Jep daten e fillimit te kursit\n");
            printf("Data (dd/mm/yyyy): ");
            scanf("%s", kurs.data_nisjes);
            printf("Jep ditet e zhvillimit te kursit\n");
            printf("Dita: ");
            scanf("%s", kurs.dite_zhvillimi);
            printf("Jep oren e fillimit te kursit\n");
            printf("Ora: ");
            scanf("%s", kurs.ora_fillimit);
            printf("Jep Kategorine e kursit\n");
            printf("Kategoria: ");
            scanf("%s", kurs.kategoria);
            printf("Jepni ID e instruktorit\n");
            printf("ID: ");
            scanf("%d", &kurs.id_instruktorit);
            krijo_Kurs(kurs);
            break;
        }
        case 4: {
            int id_studenti; int id_kursi;
            printf("Jep ID e studentit qe doni te rregjistroni:\n");
            scanf("%d", &id_studenti);
            printf("Jepni ID e kursit:\n");
            scanf("%d", &id_kursi);
            rregjisto_Student_ne_kurs(id_studenti, id_kursi);
            break;
        }
        case 5: {
            int id_instruktori;
            printf("Jep ID e instruktorit:\n");
            scanf("%d", &id_instruktori);
            fshi_Instruktor(id_instruktori);
            break;
        }
        case 6: {
            int id_kursi;
            printf("Jep ID e kursit:\n");
            scanf("%d", &id_kursi);
            fshi_kursin(id_kursi);
            break;
        }
        case 7: {
            int id_student; int id_kursi;
            printf("Jep ID e studentit:\n");
            scanf("%d", &id_student);
            printf("Jep ID e kursit:\n");
            scanf("%d", &id_kursi);
            crregjistro_studentin(id_student, id_kursi);
            break;
        }
        case 8:
            listo_studentet();
        break;
        case 9:
            listo_instruktoret();
        break;
        case 10:
            listo_kurset();
        break;
        case 11: {
            int id_kursi;
            printf("Jep ID e kursit:\n");
            scanf("%d", &id_kursi);
            listo_studentet_rregjistruar(id_kursi);
            break;
        }
        case 12: {
            int id_instruktori;
            printf("Jepni ID e instruktorit");
            scanf("%d", &id_instruktori);
            kurset_e_instruktorit(id_instruktori);
            break;
        }
        case 13: {
            int id_instruktori;
            printf("Jepni ID e instruktorit");
            scanf("%d", &id_instruktori);
            femijet_e_instruktorit(id_instruktori);
            break;
        }
        case 14:
            printf("Dalje nga programi\n");
        break;
    default:
        printf("Zgjedhje e pavlefshme!");
    } 
    } while (zgjedhje != 14);

    return 0;
    //Good Job Neim! :)
}
