//
//  main.c
//  Vector Rar
//
//  Created by George Mircea on 2/7/17.
//  Copyright © 2017 Grosu George-Mircea. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

typedef struct sparseArray {
    int ItemsNO;
    int nenullItemsNO;
    int *nenullItems;
    int *nenullItemsPOZ;
} sparseArray;

int *uncompressArray( sparseArray sparsedArray);
sparseArray compressArray( int itemsNumber, int *array);
void arraySum( char *input, char *output, int minimumLenght, int maximumLenght);

int main( int argc, const char * argv[]) {
    
    if ( argc > 1) {
        arraySum( (char*)argv[1], (char*)argv[2], atoi( argv[3]), atoi( argv[4]));
    }
    else{
        printf( "Introduceti parametri!\n");
    }
    return 0;
}

void arraySum( char *input, char *output, int minimumLenght, int maximumLenght) {
    sparseArray sparsedArray1;
    sparseArray *sparsedArray2;
    int *array1, *array2;
    long endOfFile;
    int i, j, k, arraysNumber=0;
    
    if ( minimumLenght > maximumLenght) {
        printf( "Introduceti datele corect!\n");
    }
    else {
        FILE *inputFile = fopen( input, "rb");
        FILE *outputFile = fopen( output, "wb");
        
        fseek( inputFile, 0, SEEK_END);
        endOfFile = ftell( inputFile);
        fseek( inputFile, 0, SEEK_SET);
        
        sparsedArray1.nenullItems = malloc(1);
        sparsedArray1.nenullItemsPOZ = malloc(1);
        
        while( ftell( inputFile) != endOfFile) {
            fread( &sparsedArray1.ItemsNO, sizeof(int), 1, inputFile);
            fread( &sparsedArray1.nenullItemsNO, sizeof(int), 1, inputFile);
        
            sparsedArray1.nenullItems = realloc( sparsedArray1.nenullItems,
                                                sparsedArray1.nenullItemsNO * sizeof(int));
            sparsedArray1.nenullItemsPOZ = realloc( sparsedArray1.nenullItemsPOZ,
                                                sparsedArray1.nenullItemsNO * sizeof(int));
        
            for ( i=0; i < sparsedArray1.nenullItemsNO; i++) {
                fread( &sparsedArray1.nenullItems[i], sizeof(int), 1, inputFile);
            }
        
            for ( i=0; i < sparsedArray1.nenullItemsNO; i++) {
                fread( &sparsedArray1.nenullItemsPOZ[i], sizeof(int), 1, inputFile);
            }
            if ( sparsedArray1.ItemsNO >= minimumLenght && sparsedArray1.ItemsNO <= maximumLenght) {
                arraysNumber ++;
            }
        }
        
        free( sparsedArray1.nenullItemsPOZ);
        free( sparsedArray1.nenullItems);
        
        if ( arraysNumber != 0) {
        
            sparsedArray2 = calloc( arraysNumber, sizeof(sparseArray));
            
            fseek( inputFile, 0, SEEK_SET);
            
            for ( i=0; i < arraysNumber; i++) {
                fread( &sparsedArray2[i].ItemsNO, sizeof(int), 1, inputFile);
                
                if ( sparsedArray2[i].ItemsNO >= minimumLenght && sparsedArray2[i].ItemsNO <= maximumLenght) {
                    fread( &sparsedArray2[i].nenullItemsNO, sizeof(int), 1, inputFile);
                
                    sparsedArray2[i].nenullItems = malloc(                                          sparsedArray2[i].nenullItemsNO * sizeof(int));
                    sparsedArray2[i].nenullItemsPOZ = malloc(                                                sparsedArray2[i].nenullItemsNO * sizeof(int));
                
                    for ( j=0; j < sparsedArray2[i].nenullItemsNO; j++) {
                        fread( &sparsedArray2[i].nenullItems[j], sizeof(int), 1, inputFile);
                    }
                
                    for ( j=0; j < sparsedArray2[i].nenullItemsNO; j++) {
                        fread( &sparsedArray2[i].nenullItemsPOZ[j], sizeof(int), 1, inputFile);
                    }
                }
            }
            
            for ( i=0; i < arraysNumber; i++) {
                if ( sparsedArray2[i].ItemsNO != 0) {
                    for ( j=0; j < arraysNumber; j++) {
                        if ( sparsedArray2[j].ItemsNO != 0) {
                            if ( sparsedArray2[i].ItemsNO == sparsedArray2[j].ItemsNO) {
                                array1 = uncompressArray( sparsedArray2[i]);
                                array2 = uncompressArray( sparsedArray2[j]);
                                for ( k=0; k < sparsedArray2[i].ItemsNO; k++) {
                                    array1[k] += array2[k];
                                }
                                sparsedArray1 = compressArray( sparsedArray2[i].ItemsNO, array1);
                                sparsedArray2[i].ItemsNO = 0;
                                sparsedArray2[j].ItemsNO = 0;
                                fwrite( &sparsedArray1.ItemsNO, sizeof(sparsedArray1.ItemsNO), 1, outputFile);
                                fwrite( &sparsedArray1.nenullItemsNO, sizeof(sparsedArray1.nenullItemsNO), 1, outputFile);
                                for ( k=0; k < sparsedArray1.nenullItemsNO; k++) {
                                    fwrite( &sparsedArray1.nenullItems[k], sizeof(sparsedArray1.nenullItems[k]), 1, outputFile);
                                }
                                for ( k=0; k < sparsedArray1.nenullItemsNO; k++) {
                                    fwrite( &sparsedArray1.nenullItemsPOZ[k], sizeof(sparsedArray1.nenullItemsPOZ[k]), 1, outputFile);
                                }
                            }
                        }
                    }
                }
            }
            
            for ( i=0; i < arraysNumber; i++) {
                free( sparsedArray2[i].nenullItems);
                free( sparsedArray2[i].nenullItemsPOZ);
            }

            free( sparsedArray2);
        }
        else{
            printf( "Nu exista vectori de aceasta lungime!\n");
        }
        
        fclose( inputFile);
        fclose( outputFile);
    
    }
}

// uncompress an array

int *uncompressArray( sparseArray sparsedArray) {
    int *uncompressedArray;
    int i;
    
    uncompressedArray = calloc( sparsedArray.ItemsNO, sizeof(int) );
    
    for ( i=0; i < sparsedArray.nenullItemsNO; i++) {
        uncompressedArray[ sparsedArray.nenullItemsPOZ[i] ] = sparsedArray.nenullItems[i];
    }
    
    return uncompressedArray;
}

// compress an array

sparseArray compressArray( int itemsNumber, int *array) {
    sparseArray compressedArray;
    int i, nenullItemsNO=0, contor=0;
    
    for ( i=0; i < itemsNumber; i++) {
        if ( array[i] != 0) {
            nenullItemsNO ++;
        }
    }
    
    compressedArray.nenullItemsNO = nenullItemsNO;
    compressedArray.ItemsNO = itemsNumber;
    compressedArray.nenullItems = calloc( nenullItemsNO, sizeof(int));
    compressedArray.nenullItemsPOZ = calloc( nenullItemsNO, sizeof(int));
    
    for ( i=0; i < itemsNumber; i++) {
        if ( array[i] != 0) {
            compressedArray.nenullItemsPOZ[contor] = i;
            compressedArray.nenullItems[contor++] = array[i];
            nenullItemsNO --;
        }
        if ( nenullItemsNO == 0) {
            break;
        }
    }
    
    return compressedArray;
}
