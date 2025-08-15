#pragma once
extern void (*fill_random_bytes)(void *dst, UINTN n);
void init_rng();