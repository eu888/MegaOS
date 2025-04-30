#include <stdint.h>
#include <stddef.h>
#include "memory.h"
#include "strings.h"
#include "print.h"

#define BLOCK_SIZE 512
#define MAX_FILES 100

typedef struct {
    char name[20];
    int size;
    char* data;
} File;

typedef struct {
    File* files[MAX_FILES];
    int file_count;
} FileSistem;

FileSistem* creat_files_system(){
    FileSistem *fs = kmalloc(sizeof(FileSistem));
    fs->file_count = 0;
    return fs;
}

int creat_file(FileSistem fs, const char* name, const char* data){
    if (fs->file_count >= MAX_FILES) return -1;
    File* file = kmalloc(sizeof(File));
    strncmp(file->name, name, 20);
    file->size = strlen(data);
    file->data = kmalloc(file->size + 1);
    memcpy(file->data, data, strlen(data));
    fs->files[fs->file_count++] = file;
}

void list_files(FileSistem* fs){
    for (int i = 0; i < fs->file_count; i++){
        print_str("File: ");
        print_str(fs->files[i]->name);
    }
}

// TODO:add this to kernel.c
// int main() { 
//     FileSystem *fs = create_file_system(); 
//     create_file(fs, "example.txt", "Hello, File System!"); 
//     list_files(fs); 
//     free_file_system(fs); 
//     return 0; 
// } 