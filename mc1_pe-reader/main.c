#include "wnt.h"


void fprintf_info(FILE *file, char *title, DWORD data) {
    fprintf(file, "0x%08x %s\n", data, title);
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: mc1_pe-reader <path_to_exe>");
        return -1;
    }

    FILE *file, *sections_txt, *code_bin;
    IMAGE_DOS_HEADER header_dos = {0};
    IMAGE_NT_HEADERS header_image = {0};

    file = fopen(argv[1], "rb");
    sections_txt = fopen("sections.txt", "w");
    code_bin = fopen("code.bin", "wb");

    if (!file) {
        printf("Couldn't open file\n");
        return 1;
    }

    if (fread(&header_dos, sizeof header_dos, 1, file) != 1) {
        printf("Couldn't read DOS_HEADER\n");
        return 1;
    }

    if (fseek(file, header_dos.e_lfanew, SEEK_SET)) {
        printf("Couldn't seek with offset %d\n", header_dos.e_lfanew);
        return 1;
    }

    if (fread(&header_image, sizeof header_image, 1, file) != 1) {
        printf("Couldn't read NT_HEADERS\n");
        return 1;
    }

    IMAGE_FILE_HEADER header_file = header_image.FileHeader;

    fprintf(sections_txt, "%10d NumberOfSections\n", header_file.NumberOfSections);
    fprintf(sections_txt, "0x%08u OptionalHeader.AddressOfEntryPoint\n\n\n",
            header_image.OptionalHeader.AddressOfEntryPoint);

    IMAGE_SECTION_HEADER *current_section = (IMAGE_SECTION_HEADER *) (malloc(
            IMAGE_SIZEOF_SECTION_HEADER * header_file.NumberOfSections));

    for (int i = 0; i < header_file.NumberOfSections; i++) {
        if (fread(&(current_section[i]), sizeof(IMAGE_SECTION_HEADER), 1, file) != 1) {
            printf("Couldn't read section number %d\n", i);
            continue;
        }
        IMAGE_SECTION_HEADER info = current_section[i];

        fprintf(sections_txt, "%s\n", info.Name);
        fprintf_info(sections_txt, "VirtualSize", info.Misc.VirtualSize);
        fprintf_info(sections_txt, "VirtualAddress", info.VirtualAddress);
        fprintf_info(sections_txt, "PointerToRawData", info.PointerToRawData);
        fprintf_info(sections_txt, "SizeOfRawData", info.SizeOfRawData);
        fprintf_info(sections_txt, "PointerToRelocations", info.PointerToRelocations);
        fprintf_info(sections_txt, "NumberOfRelocations", info.NumberOfRelocations);
        fprintf_info(sections_txt, "PointerToLinenumbers", info.PointerToLinenumbers);
        fprintf_info(sections_txt, "NumberOfLinenumbers", info.NumberOfLinenumbers);
        fprintf_info(sections_txt, "Characteristics", info.Characteristics);
        fprintf(sections_txt, "\n\n");
    }

    for (int i = 0; i < header_file.NumberOfSections; ++i) {
        IMAGE_SECTION_HEADER info = current_section[i];
        if (info.Characteristics & 0x20) {
            size_t pointer = info.PointerToRawData;
            if (fseek(file, (long) pointer, 0)) {
                printf("Incorrect pointer %d\n", i);
                continue;
            }

            char *section_with_code = (char *) (malloc(info.SizeOfRawData));

            if (!section_with_code) {
                printf("Couldn't allocate memory SizeOfRawData %d\n", i);
                continue;
            }

            if (fread(section_with_code, info.SizeOfRawData, 1, file) != 1) {
                printf("Couldn't read code section %d\n", i);
                continue;
            }

            if (fwrite(section_with_code, info.SizeOfRawData, 1, code_bin) != 1) {
                printf("Couldn't write code section %d\n", i);
                continue;
            }

            free(section_with_code);
        }
    }

    return 0;
}