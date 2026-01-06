#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "value.h"
#include "vm.h"

#define ALLOCATE_OBJ(size, type, objectType) \
    (type*)allocateObject(sizeof(type), objectType)

// Allocates an object on the heap, then initializes type. The size is passed so the caller can add bytes for extra fields needed by specific objects.
static Obj* allocateObject(size_t size, ObjType type) {
    Obj* object = (Obj*)reallocate(NULL, 0, size);
    object->type = type;

    object->next = vm.objects;
    vm.objects = object;
    return object;
}

// Creates an ObjString on the heap, then intializes its fields (like a constructor!).
static ObjString* allocateString(char* chars, int length) {
    ObjString* string = (ObjString*)allocateObject(sizeof(ObjString) + ((length + 1) * sizeof(char)), OBJ_STRING);
    string->length = length;
    memcpy(string->chars, chars, length);
    string->chars[length] = '\0';
    return string;
}

// Creates a ObjString from a string already allocated onto the heap.
ObjString* takeString(char* chars, int length) {
    ObjString* string = allocateString(chars, length);
    FREE(char, chars);
    return string;
}

// Now a wrapper, mainly so I don't have to replace every call of copyString. Same reason I'm leaving the parameter.
ObjString* copyString(const char* chars, int length) {
    char* heapChars = ALLOCATE(char, length + 1);
    memcpy(heapChars, chars, length);
    heapChars[length] = '\0';
    return takeString(heapChars, length);
}

void printObject(Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_STRING:
            printf("%s", AS_CSTRING(value));
            break;
    }
}