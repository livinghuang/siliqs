#line 1 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/rawsum.c"
// #include <stdint.h>
// #include <stdio.h>

// uint16_t raw_sum(uint8_t *buffer, int len)
// {
//   uint16_t sum = 0;
//   for (int i = 0; i < len - 2; i++)
//   {
//     sum += buffer[i];
//   }
//   return sum;
// }

// int main()
// {
//   // 示例数据
//   uint8_t data[] = {0x42, 0x4D, 0x00, 0x08, 0x04, 0x41, 0x00, 0x00, 0x0C, 0xD8, 0x01, 0xC0};
//   int length = sizeof(data) / sizeof(data[0]);

//   uint16_t sum = raw_sum(data, length);

//   printf("sum: %04X\n", sum);
// }
