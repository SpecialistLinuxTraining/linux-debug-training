#include <stdio.h>

void causeSegmentationFault() {
  int *ptr = NULL; // Initializing a pointer to NULL

  // Try to access the value pointed to by NULL (dereferencing NULL)
  int value = *ptr;

  printf(
      "The value is: %d\n",
      value); // This line will never be reached due to the segmentation fault
}

int main() {
  causeSegmentationFault(); // Call the function that causes the segmentation
                            // fault

  return 0;
}
