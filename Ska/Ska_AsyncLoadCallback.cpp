#include "SkaAnimation.h"

void Ska_AsyncLoadCallback(AsyncStream* stream, int status, uint32_t flags, AnimData* animData) {
    if (status == 3) { // 3 = ASYNC_STATUS_COMPLETE
        Ska_InitAnimationInstance(animData);
    }
}