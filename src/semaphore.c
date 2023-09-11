#include <semaphore.h>

static uint32_t semaphores = 0;

bool semaphore_reserve(const uint8_t semaphore_id) 
{
    if (semaphore_id > SEMAPHORE_ID_MAX) return false;
    while (semaphores & (1ULL << semaphore_id));
    semaphores |= (1ULL << semaphore_id);
    return true;
}

bool semaphore_release(const uint8_t semaphore_id) 
{
    if (semaphore_id > SEMAPHORE_ID_MAX) return false;
    semaphores &= ~(1ULL << semaphore_id);
    return true;
}