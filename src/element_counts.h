// element_counts.h
#ifndef ELEMENT_COUNTS_H
#define ELEMENT_COUNTS_H

typedef struct {
    int transform_count;
    int square_count;
    // Add more counts for other element types as needed
} ElementCounts;

extern ElementCounts element_count;
// Function to initialize the counts
void element_counts_init(ElementCounts *counts);

#endif // ELEMENT_COUNTS_H
